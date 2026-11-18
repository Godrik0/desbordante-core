#pragma once

#include <cstddef>
#include <utility>
#include <vector>

namespace algos::fd::fdhits {

template <typename E>
class VecEdgeCollector {
private:
    std::vector<E> results_;

public:
    void Consume(E const& edge) {
        results_.push_back(edge);
    }

    void Consume(E&& edge) {
        results_.push_back(std::move(edge));
    }

    size_t Size() const {
        return results_.size();
    }

    auto begin() const {
        return results_.begin();
    }

    auto end() const {
        return results_.end();
    }
};

}  // namespace algos::fd::fdhits
