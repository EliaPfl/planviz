#include "landmark_graph.h"

#include "landmark.h"

#include <cassert>
#include <list>
#include <set>
#include <sstream>
#include <vector>
#include <fstream>
#include "../utils/json.hpp"
#include "../algorithms/sccs.h"


using namespace std;
using json = nlohmann::json;

namespace landmarks {
LandmarkGraph::LandmarkGraph()
    : num_conjunctive_landmarks(0), num_disjunctive_landmarks(0) {
}

int LandmarkGraph::get_num_edges() const {
    int total = 0;
    for (auto &node : nodes)
        total += node->children.size();
    return total;
}

LandmarkNode *LandmarkGraph::get_node(int i) const {
    return nodes[i].get();
}

LandmarkNode &LandmarkGraph::get_simple_landmark(const FactPair &fact) const {
    assert(contains_simple_landmark(fact));
    return *(simple_landmarks_to_nodes.find(fact)->second);
}

// needed only by landmarkgraph-factories.
LandmarkNode &LandmarkGraph::get_disjunctive_landmark(const FactPair &fact) const {
    /* Note: this only works because every proposition appears in only one
       disjunctive landmark. */
    assert(!contains_simple_landmark(fact));
    assert(contains_disjunctive_landmark(fact));
    return *(disjunctive_landmarks_to_nodes.find(fact)->second);
}


bool LandmarkGraph::contains_simple_landmark(const FactPair &lm) const {
    return simple_landmarks_to_nodes.count(lm) != 0;
}

bool LandmarkGraph::contains_disjunctive_landmark(const FactPair &lm) const {
    return disjunctive_landmarks_to_nodes.count(lm) != 0;
}

bool LandmarkGraph::contains_overlapping_disjunctive_landmark(
    const set<FactPair> &lm) const {
    // Test whether ONE of the facts is present in some disjunctive landmark.
    for (const FactPair &lm_fact : lm) {
        if (contains_disjunctive_landmark(lm_fact))
            return true;
    }
    return false;
}

bool LandmarkGraph::contains_identical_disjunctive_landmark(
    const set<FactPair> &lm) const {
    /* Test whether a disjunctive landmark exists which consists EXACTLY of
       the facts in lm. */
    LandmarkNode *lmn = nullptr;
    for (const FactPair &lm_fact : lm) {
        auto it2 = disjunctive_landmarks_to_nodes.find(lm_fact);
        if (it2 == disjunctive_landmarks_to_nodes.end())
            return false;
        else {
            if (lmn && lmn != it2->second) {
                return false;
            } else if (!lmn)
                lmn = it2->second;
        }
    }
    return true;
}

bool LandmarkGraph::contains_landmark(const FactPair &lm) const {
    /* Note: this only checks for one fact whether it's part of a landmark,
       hence only simple and disjunctive landmarks are checked. */
    return contains_simple_landmark(lm) || contains_disjunctive_landmark(lm);
}

LandmarkNode &LandmarkGraph::add_landmark(Landmark &&landmark) {
    assert(landmark.conjunctive
           || all_of(landmark.facts.begin(), landmark.facts.end(),
                     [&](const FactPair &lm_fact) {
                         return !contains_landmark(lm_fact);
                     }));
    unique_ptr<LandmarkNode> new_node =
        make_unique<LandmarkNode>(move(landmark));
    LandmarkNode *new_node_p = new_node.get();
    const Landmark &lm = new_node->get_landmark();
    nodes.push_back(move(new_node));
    if (lm.disjunctive) {
        for (const FactPair &lm_fact : lm.facts) {
            disjunctive_landmarks_to_nodes.emplace(lm_fact, new_node_p);
        }
        ++num_disjunctive_landmarks;
    } else if (lm.conjunctive) {
        ++num_conjunctive_landmarks;
    } else {
        simple_landmarks_to_nodes.emplace(lm.facts.front(), new_node_p);
    }
    return *new_node_p;
}

void LandmarkGraph::remove_node_occurrences(LandmarkNode *node) {
    for (const auto &parent : node->parents) {
        LandmarkNode &parent_node = *(parent.first);
        parent_node.children.erase(node);
        assert(parent_node.children.find(node) == parent_node.children.end());
    }
    for (const auto &child : node->children) {
        LandmarkNode &child_node = *(child.first);
        child_node.parents.erase(node);
        assert(child_node.parents.find(node) == child_node.parents.end());
    }
    const Landmark &landmark = node->get_landmark();
    if (landmark.disjunctive) {
        --num_disjunctive_landmarks;
        for (const FactPair &lm_fact : landmark.facts) {
            disjunctive_landmarks_to_nodes.erase(lm_fact);
        }
    } else if (landmark.conjunctive) {
        --num_conjunctive_landmarks;
    } else {
        simple_landmarks_to_nodes.erase(landmark.facts[0]);
    }
}

void LandmarkGraph::remove_node(LandmarkNode *node) {
    remove_node_occurrences(node);
    auto it = find_if(nodes.begin(), nodes.end(),
                      [&node](unique_ptr<LandmarkNode> &n) {
                          return n.get() == node;
                      });
    assert(it != nodes.end());
    nodes.erase(it);
}

void LandmarkGraph::remove_node_if(
    const function<bool (const LandmarkNode &)> &remove_node_condition) {
    for (auto &node : nodes) {
        if (remove_node_condition(*node)) {
            remove_node_occurrences(node.get());
        }
    }
    nodes.erase(remove_if(nodes.begin(), nodes.end(),
                          [&remove_node_condition](const unique_ptr<LandmarkNode> &node) {
                              return remove_node_condition(*node);
                          }), nodes.end());
}

void LandmarkGraph::set_landmark_ids() {
    int id = 0;
    for (auto &lmn : nodes) {
        lmn->set_id(id);
        ++id;
    }
}

void LandmarkGraph::export_graph(const fs::path &output_path, const VariablesProxy &vars) {
    json jnodes = json::array();
    json jedges = json::array();

    std::vector<std::vector<int>> successors(nodes.size());

    for (const auto &node : nodes) {
        string landmark_name;
        const Landmark &landmark = node->get_landmark();
        bool first = true;
        
        for (const FactPair &fact : landmark.facts) {
            if (!first) {
                if (landmark.disjunctive) {
                    landmark_name += " | ";
                } else if (landmark.conjunctive) {
                    landmark_name += " & ";
                }
            }
            first = false;
            VariableProxy var = vars[fact.var];
            landmark_name += var.get_fact(fact.value).get_name();
        }

        json jnode;
        jnode["data"] = {
                {"id", std::to_string(node->get_id())},
                {"name", landmark_name}
            };
        jnodes.push_back(jnode);

        for (const auto &[child, edge_type] : node->children) {
            json jedge;
            jedge["data"] = {
                    {"id", std::to_string(node->get_id()) + "_" + std::to_string(child->get_id())},
                    {"source", std::to_string(node->get_id())},
                    {"target", std::to_string(child->get_id())},
                    {"type", static_cast<int>(edge_type)}
                };
            jedges.push_back(jedge);
            successors[node->get_id()].push_back(child->get_id());
        }
    }

    // scc
    auto sccs = sccs::compute_maximal_sccs(successors);
    std::vector<int> scc_ids(successors.size(), -1); //maps Node ID to SCC ID
    for (unsigned long i = 0; i < sccs.size(); ++i) {
        for (int node : sccs[i]) {
            scc_ids[node] = i;
        }
    }

    for (unsigned long i = 0; i < jnodes.size(); ++i) {
        jnodes[i]["data"]["scc_id"] = scc_ids[i];
    }

    json graph_json;
    graph_json["elements"] = {
        {"nodes", jnodes},
        {"edges", jedges}
    };
    graph_json["metadata"] = {
        {"num_landmarks", nodes.size()},
        {"num_sccs", sccs.size()},
        {"num_conjunctive_landmarks", num_conjunctive_landmarks},
        {"num_disjunctive_landmarks", num_disjunctive_landmarks}
    };


    std::ofstream out(output_path / ("landmark_graph.json"));
    out << graph_json.dump(2);
    out.close();
}

}
