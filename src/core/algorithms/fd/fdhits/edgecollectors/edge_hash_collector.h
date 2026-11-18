#pragma once

#include <algorithm>
#include <cstddef>
#include <ranges>
#include <utility>
#include <vector>

#include <boost/unordered_set.hpp>

#include "core/algorithms/fd/fdhits/treesearch/hypergraph.h"

namespace algos::fd::fdhits {

template <typename E>
class EdgeHashCollector {
private:
    boost::unordered_set<E, typename E::Hash> edges_;

public:
    void Consume(E const& edge) {
        edges_.insert(edge);
    }

    void Consume(E&& edge) {
        edges_.insert(std::move(edge));
    }

    auto Edges() const {
        return std::ranges::subrange(edges_.begin(), edges_.end());
    }

    bool IsEmpty() const {
        return edges_.empty();
    }

    Hypergraph<E> ToGraph(size_t vertex_count) {
        Hypergraph<E> graph(vertex_count);

        std::vector<E> sorted_edges;
        sorted_edges.reserve(edges_.size());
        while (!edges_.empty()) {
            sorted_edges.push_back(
                std::move(edges_.extract(edges_.begin()).value())
            );
        }
        edges_.clear();

        std::ranges::sort(sorted_edges, [](E const& a, E const& b) { return a < b; });

        for (auto& edge : sorted_edges) {
            if (!graph.Implies(edge)) {
                graph.AddEdge(std::move(edge));
            }
        }
        return graph;
    }
};

}  // namespace algos::fd::fdhits
