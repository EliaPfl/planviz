#ifndef SEARCH_ALGORITHMS_GRAPH_ONLY_SEARCH_H
#define SEARCH_ALGORITHMS_GRAPH_ONLY_SEARCH_H

#include "../evaluation_context.h"
#include "../search_algorithm.h"
#include "../task_utils/successor_generator.h"
#include "../utils/system.h"

namespace graph_only_search {

class GraphOnlySearch : public SearchAlgorithm {
    std::shared_ptr<Evaluator> evaluator;
    EvaluationContext current_eval_context;

protected:
    virtual void initialize() override;
    virtual SearchStatus step() override;

public:
    GraphOnlySearch(
        const std::shared_ptr<Evaluator> &h,
        OperatorCost cost_type,
        int bound,
        double max_time,
        const std::string &description,
        utils::Verbosity verbosity);

    virtual void print_statistics() const override;
};

}

#endif
