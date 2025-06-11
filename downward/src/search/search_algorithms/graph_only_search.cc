#include "graph_only_search.h"

#include "../utils/logging.h"
#include "../task_utils/successor_generator.h"
#include "../task_utils/causal_graph.h"

#include "../plugins/plugin.h"


using namespace std;

namespace graph_only_search {

GraphOnlySearch::GraphOnlySearch(
    const std::shared_ptr<Evaluator> &h,
    OperatorCost cost_type,
    int bound,
    double max_time,
    const std::string &description,
    utils::Verbosity verbosity)
    : SearchAlgorithm(cost_type, bound, max_time, description, verbosity),
      evaluator(h),
      current_eval_context(state_registry.get_initial_state(), &statistics) {}

void GraphOnlySearch::initialize() {
    log << "Starting GraphOnlySearch for structure analysis." << endl;

    State initial_state = state_registry.get_initial_state();
    current_eval_context = EvaluationContext(initial_state, &statistics);

    task_proxy.get_causal_graph().export_successors(task_proxy);
}


SearchStatus GraphOnlySearch::step() {
    log << "GraphOnlySearch completed graph analysis. Terminating." << endl;
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
        document_synopsis("Expands the reachable state space for structural analysis, then terminates.");

        add_option<std::shared_ptr<Evaluator>>("h", "heuristic evaluator for tracking");
        add_search_algorithm_options_to_feature(*this, "graph_only");
    }

    virtual std::shared_ptr<GraphOnlySearch>
    create_component(const plugins::Options &opts) const override {
        return plugins::make_shared_from_arg_tuples<GraphOnlySearch>(
            opts.get<std::shared_ptr<Evaluator>>("h"),
            get_search_algorithm_arguments_from_options(opts)
        );
    }
};

static plugins::FeaturePlugin<GraphOnlySearchFeature> _plugin;

}
