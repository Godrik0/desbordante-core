#pragma once

#include <cstddef>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>
#include <boost/functional/hash.hpp>

namespace algos::fd::fdhits::edges {
class DefaultEdge {
private:
    boost::dynamic_bitset<> bits_;

    explicit DefaultEdge(boost::dynamic_bitset<> bits) : bits_(std::move(bits)) {}

    explicit DefaultEdge(size_t size) : bits_(size) {}

public:
    static constexpr auto kNpos = boost::dynamic_bitset<>::npos;

    static DefaultEdge EmptyEdge(size_t size) {
        return DefaultEdge(size);
    }

    static DefaultEdge Filled(size_t size) {
        DefaultEdge edge(size);
        edge.bits_.set();
        return edge;
    }

    void Add(size_t node) {
        bits_.set(node);
    }

    void Remove(size_t node) {
        bits_.reset(node);
    }

    bool Contains(size_t node) const {
        return bits_.test(node);
    }

    size_t Count() const {
        return bits_.count();
    }

    size_t Size() const {
        return bits_.size();
    }

    bool IsEmpty() const {
        return bits_.none();
    }

    bool Hits(DefaultEdge const& other) const {
        return bits_.intersects(other.bits_);
    }

    bool Implies(DefaultEdge const& other) const {
        return bits_.is_subset_of(other.bits_);
    }

    bool IsImpliedBy(std::vector<DefaultEdge> const& others) const {
        for (auto const& e : others) {
            if (e.Implies(*this)) return true;
        }
        return false;
    }

    void RemoveAll(DefaultEdge const& other) {
        bits_ -= other.bits_;
    }

    void AddAll(DefaultEdge const& other) {
        bits_ |= other.bits_;
    }

    DefaultEdge Intersect(DefaultEdge const& other) const {
        return DefaultEdge(bits_ & other.bits_);
    }

    void IntersectWith(DefaultEdge const& other) {
        bits_ &= other.bits_;
    }

    void Clear() {
        bits_.reset();
    }

    std::vector<size_t> GetNodes() const {
        std::vector<size_t> nodes;
        nodes.reserve(Count());
        for (size_t i = bits_.find_first(); i != kNpos; i = bits_.find_next(i)) {
            nodes.push_back(i);
        }
        return nodes;
    }

    class NodesIterator {
    private:
        boost::dynamic_bitset<> const* bits_;
        size_t current_;

    public:
        NodesIterator() : bits_(nullptr), current_(kNpos) {}

        NodesIterator(boost::dynamic_bitset<> const* bits, size_t pos)
            : bits_(bits), current_(pos) {}

        using value_type = size_t;
        using difference_type = std::ptrdiff_t;
        using iterator_concept = std::forward_iterator_tag;

        size_t operator*() const {
            return current_;
        }

        NodesIterator& operator++() {
            current_ = bits_->find_next(current_);
            return *this;
        }

        NodesIterator operator++(int) {
            NodesIterator previous = *this;
            ++(*this);
            return previous;
        }

        bool operator==(NodesIterator const& other) const = default;
    };

    static_assert(std::forward_iterator<NodesIterator>);

    NodesIterator NodesBegin() const {
        return NodesIterator(&bits_, bits_.find_first());
    }

    NodesIterator NodesEnd() const {
        return NodesIterator(&bits_, kNpos);
    }

    auto Nodes() const {
        return std::ranges::subrange(NodesBegin(), NodesEnd());
    }

    size_t FirstNode() const {
        return bits_.find_first();
    }

    size_t Overlap(DefaultEdge const& other) const {
        size_t count = 0;
        for (size_t node : Nodes()) {
            if (other.Contains(node)) ++count;
        }
        return count;
    }

    std::string ToString() const {
        std::ostringstream oss;
        bool first = true;
        for (size_t node : Nodes()) {
            if (!first) oss << ",";
            oss << node;
            first = false;
        }
        return oss.str();
    }

    bool operator==(DefaultEdge const& other) const = default;

    bool operator<(DefaultEdge const& other) const {
        size_t const count = Count();
        size_t const other_count = other.Count();
        if (count != other_count) return count < other_count;
        return bits_ < other.bits_;
    }

    boost::dynamic_bitset<>& GetBits() {
        return bits_;
    }

    boost::dynamic_bitset<> const& GetBits() const {
        return bits_;
    }

    struct Hash {
        std::size_t operator()(DefaultEdge const& edge) const {
            return boost::hash_value(edge.bits_);
        }
    };
};

}  // namespace algos::fd::fdhits::edges
