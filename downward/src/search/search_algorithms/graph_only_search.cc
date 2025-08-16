#include "graph_only_search.h"

#include "../utils/logging.h"
#include "../task_utils/successor_generator.h"
#include "../task_utils/causal_graph.h"

#include "../plugins/plugin.h"

#include "../heuristics/domain_transition_graph.h"

#include "../landmarks/landmark_factory_reasonable_orders_hps.h"
#include "../landmarks/landmark_factory_rpg_sasp.h"


using namespace std;
using namespace domain_transition_graph;
using namespace landmarks;

namespace graph_only_search {

GraphOnlySearch::GraphOnlySearch(
    const std::string &output_filepath,
    OperatorCost cost_type,
    int bound,
    double max_time,
    const std::string &description,
    utils::Verbosity verbosity)
    : SearchAlgorithm(cost_type, bound, max_time, description, verbosity),
      output_path(output_filepath),
      current_eval_context(state_registry.get_initial_state(), &statistics) {}

void GraphOnlySearch::initialize() {
    using DTGs = std::vector<std::unique_ptr<DomainTransitionGraph>>;

    log << "Starting GraphOnlySearch for structure analysis. Export path: " << output_path << endl;

    // make sure the output directory exists
    if (!fs::exists(output_path)) {
        fs::create_directories(output_path);
        log << "Created output directory: " << output_path << endl;
    }

    State initial_state = state_registry.get_initial_state();
    GoalsProxy goals = task_proxy.get_goals();
    OperatorsProxy ops = task_proxy.get_operators();
    VariablesProxy vars = task_proxy.get_variables();

    std::unordered_map<int, int> goal_map;
    for (FactProxy g : goals) {
        goal_map[g.get_variable().get_id()] = g.get_value();
    }

    // causal graph
    task_proxy.get_causal_graph().export_graph(initial_state, goal_map, ops, vars, output_path);
    log << "Causal graph exported." << endl;

    // domain transition graphs
    function<bool(int, int)> pruning_condition =
        [](int, int) {return false;};
    DTGFactory factory(task_proxy, false, pruning_condition);
    DTGs transition_graphs = factory.build_dtgs();

    log << "Domain transition graphs computed with " << transition_graphs.size() << " graphs." << endl;
    
    for(const auto &dtg : transition_graphs) {
        dtg->export_graph(initial_state, goal_map, ops, vars, output_path);
    }
    log << "Domain transition graphs exported." << endl;

    // landmark graph
    auto base_lm_factory = std::make_shared<LandmarkFactoryRpgSasp>(
        true,
        true,
        utils::Verbosity::NORMAL
    );
    
    LandmarkFactoryReasonableOrdersHPS lm_factory(
        base_lm_factory,
        utils::Verbosity::NORMAL
    );
    
    auto landmark_graph = lm_factory.compute_lm_graph(task);
    log << "Landmark graph computed with " << landmark_graph->get_num_landmarks() << " landmarks." << endl;

    landmark_graph->export_graph(output_path, vars);
    log << "Landmark graph exported." << endl;
}


SearchStatus GraphOnlySearch::step() {
    log << "GraphOnlySearch completed. Terminating." << endl;
    utils::exit_with(utils::ExitCode::SUCCESS);
    return FAILED; // Will never be reached
}

void GraphOnlySearch::print_statistics() const {
    statistics.print_detailed_statistics();
    log << "GraphOnlySearch completed. No search performed, only graph analysis." << endl;
}

using namespace graph_only_search;

class GraphOnlySearchFeature
    : public plugins::TypedFeature<SearchAlgorithm, GraphOnlySearch> {
public:
    GraphOnlySearchFeature() : TypedFeature("graph_only") {
        document_title("Graph Only Analysis");
        document_synopsis("Exports causal, domain transition and landmark graphs, then exits.");

        add_option<std::string>("output", "Path to write analysis summary file (optional)");
        add_search_algorithm_options_to_feature(*this, "graph_only");
    }

    virtual std::shared_ptr<GraphOnlySearch>
    create_component(const plugins::Options &opts) const override {
        const string filepath = opts.get<std::string>("output");
        auto args = get_search_algorithm_arguments_from_options(opts);
        return plugins::make_shared_from_arg_tuples<GraphOnlySearch>(
            filepath,
            std::get<0>(args), // cost_type
            std::get<1>(args), // bound
            std::get<2>(args), // max_time
            std::get<3>(args), // description
            std::get<4>(args)  // verbosity
        );
    }
};

static plugins::FeaturePlugin<GraphOnlySearchFeature> _plugin;

}
