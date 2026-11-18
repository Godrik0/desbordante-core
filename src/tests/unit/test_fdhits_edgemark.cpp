#include <utility>

#include <gtest/gtest.h>

#include "core/algorithms/fd/fdhits/treesearch/dynamic_edgemark_store_multi.h"
#include "core/algorithms/fd/fdhits/treesearch/edgemark.h"

namespace tests {

using algos::fd::fdhits::Edgemark;

TEST(FDHitsEdgemarkTest, OperationsAfterGrowth) {
    Edgemark source(1);
    source.Set(0);
    Edgemark result(1);
    source.Grow(65);
    result.Grow(65);
    source.Set(64);

    result.SetTo(source);
    EXPECT_EQ(result.Size(), 65);
    EXPECT_EQ(result.Count(), 2);
    EXPECT_TRUE(result.IsSubset(source));

    result.SetToNot(source);
    EXPECT_EQ(result.Count(), 63);
    EXPECT_TRUE(result.IsSubsetInverse(source));
    result.IntersectWithInverse(source);
    EXPECT_EQ(result.Count(), 63);

    result.SetTo(source);
    result.IntersectWithInverse(source);
    EXPECT_TRUE(result.IsEmpty());
}

TEST(FDHitsEdgemarkTest, EmptyMasks) {
    Edgemark source;
    Edgemark result;
    result.SetTo(source);
    result.SetToNot(source);
    result.IntersectWithInverse(source);
    EXPECT_TRUE(result.IsEmpty());
    EXPECT_TRUE(result.IsSubsetInverse(source));
}

TEST(FDHitsEdgemarkTest, RhsHittingAfterGraphExtension) {
    using namespace algos::fd::fdhits;
    Hypergraph<edges::MultiFD> graph(4);
    auto edge = edges::MultiFD::EmptyEdge(4);
    edge.AddLhs(0);
    edge.Add(3);
    graph.AddEdge(edge);
    DynamicEdgemarkStoreMulti store(&graph);

    Hypergraph<edges::MultiFD> extra(4);
    auto new_edge = edges::MultiFD::EmptyEdge(4);
    new_edge.AddLhs(1);
    new_edge.Add(2);
    extra.AddEdge(new_edge);
    auto candidate = edges::MultiFD(edges::DefaultEdge::EmptyEdge(2),
                                   edges::DefaultEdge::Filled(2));
    store.Extend(candidate, std::move(extra));
    ASSERT_EQ(graph.GetEdgeCount(), 2);

    auto rhs = edges::DefaultEdge::EmptyEdge(2);
    rhs.Add(0);
    EXPECT_TRUE(store.AddRhs(rhs));
    store.Pop();
}

}
