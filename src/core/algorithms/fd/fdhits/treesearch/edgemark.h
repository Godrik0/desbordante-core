#pragma once

#include <cassert>

#include <boost/dynamic_bitset.hpp>

namespace algos::fd::fdhits {

class Edgemark {
private:
    boost::dynamic_bitset<> bits_;

public:
    explicit Edgemark(size_t size = 0) : bits_(size) {}

    static Edgemark Empty(size_t size) {
        return Edgemark(size);
    }

    static Edgemark Filled(size_t size) {
        Edgemark em(size);
        em.bits_.set();
        return em;
    }

    bool IsEmpty() const {
        return bits_.none();
    }

    void Clear() {
        bits_.reset();
    }

    void Set(size_t idx) {
        bits_.set(idx);
    }

    bool IsSubset(Edgemark const& other) const {
        return bits_.is_subset_of(other.bits_);
    }

    void Grow(size_t size) {
        bits_.resize(size);
    }

    size_t Size() const {
        return bits_.size();
    }

    size_t Count() const {
        return bits_.count();
    }

    void SetTo(Edgemark const& other) {
        assert(Size() == other.Size());
        bits_ = other.bits_;
    }

    void SetToNot(Edgemark const& other) {
        assert(Size() == other.Size());
        bits_ = other.bits_;
        bits_.flip();
    }

    void IntersectWithInverse(Edgemark const& other) {
        assert(Size() == other.Size());
        bits_ -= other.bits_;
    }

    bool IsSubsetInverse(Edgemark const& other) const {
        assert(Size() == other.Size());
        return !bits_.intersects(other.bits_);
    }

    void CloneFrom(Edgemark const& other) {
        SetTo(other);
    }

    class OnesIterator {
    private:
        boost::dynamic_bitset<> const* bits_;
        size_t current_;

    public:
        OnesIterator(boost::dynamic_bitset<> const* bits, size_t pos)
            : bits_(bits), current_(pos) {}

        size_t operator*() const {
            return current_;
        }

        OnesIterator& operator++() {
            current_ = bits_->find_next(current_);
            return *this;
        }

        bool operator!=(OnesIterator const& other) const {
            return current_ != other.current_;
        }
    };

    OnesIterator OnesBegin() const {
        return OnesIterator(&bits_, bits_.find_first());
    }

    OnesIterator OnesEnd() const {
        return OnesIterator(&bits_, boost::dynamic_bitset<>::npos);
    }

    Edgemark& operator-=(Edgemark const& other) {
        bits_ -= other.bits_;
        return *this;
    }

    Edgemark& operator|=(Edgemark const& other) {
        bits_ |= other.bits_;
        return *this;
    }

    Edgemark& operator&=(Edgemark const& other) {
        bits_ &= other.bits_;
        return *this;
    }

};

}  // namespace algos::fd::fdhits
