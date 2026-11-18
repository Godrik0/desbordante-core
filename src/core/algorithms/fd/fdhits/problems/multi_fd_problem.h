#pragma once

#include "core/algorithms/fd/fdhits/edges/multi_fd.h"
#include "core/algorithms/fd/fdhits/pli_table.h"
#include "core/algorithms/fd/fdhits/types.h"

namespace algos::fd::fdhits::problems {

class MultiFDProblem {
private:
    PLITable const* table_;

public:
    using EdgeType = edges::MultiFD;

    explicit MultiFDProblem(PLITable const* table) : table_(table) {}

    template <typename C>
    void GetEdge(RowIndex r1, RowIndex r2, C& consumer) const {
        size_t const num_columns = table_->GetColumnCount();

        edges::DefaultEdge lhs = edges::DefaultEdge::EmptyEdge(num_columns);
        edges::DefaultEdge rhs = edges::DefaultEdge::EmptyEdge(num_columns);

        for (size_t col = 0; col < num_columns; ++col) {
            if (table_->IsDifferentOnColumn(r1, r2, col)) {
                lhs.Add(col);
            } else {
                rhs.Add(col);
            }
        }

        if (lhs.Count() > 0) {
            consumer.Consume(EdgeType(std::move(lhs), std::move(rhs)));
        }
    }

    bool IsValidResult(EdgeType const& result) const {
        return result.RhsCount() > 0;
    }

    bool IsViolatingPair(RowIndex r1, RowIndex r2, EdgeType const& candidate) const {
        for (size_t node : candidate.GetRhs().Nodes()) {
            if (table_->IsDifferentOnColumn(r1, r2, node)) return true;
        }
        return false;
    }
};

}  // namespace algos::fd::fdhits::problems
