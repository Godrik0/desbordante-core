#include <cstddef>

#include <gtest/gtest.h>

#include "core/algorithms/fd/fdhits/edgecollectors/edge_hash_collector.h"
#include "core/algorithms/fd/fdhits/edges/default_edge.h"
#include "core/algorithms/fd/fdhits/edges/multi_fd.h"

namespace tests {

template <typename E>
class FDHitsEdgeCollectorTest : public ::testing::Test {};

using FDHitsEdgeTypes = ::testing::Types<algos::fd::fdhits::edges::DefaultEdge,
                                       algos::fd::fdhits::edges::MultiFD>;
TYPED_TEST_SUITE(FDHitsEdgeCollectorTest, FDHitsEdgeTypes);

TYPED_TEST(FDHitsEdgeCollectorTest, RemovesSupersetsRegardlessOfInsertionOrder) {
    size_t constexpr vertex_count = 8;
    for (size_t extra_node : {size_t{2}, size_t{5}}) {
        auto subset = TypeParam::EmptyEdge(vertex_count);
        subset.Add(1);
        auto superset = subset;
        superset.Add(extra_node);

        ASSERT_TRUE(subset.Implies(superset));
        ASSERT_FALSE(superset.Implies(subset));

        for (bool superset_first : {false, true}) {
            SCOPED_TRACE(::testing::Message() << "extra_node=" << extra_node
                                             << ", superset_first=" << superset_first);
            algos::fd::fdhits::EdgeHashCollector<TypeParam> collector;
            collector.Consume(superset_first ? superset : subset);
            collector.Consume(superset_first ? subset : superset);

            auto graph = collector.ToGraph(vertex_count);

            EXPECT_EQ(graph.GetVertexCount(), vertex_count);
            ASSERT_EQ(graph.GetEdgeCount(), 1);
            EXPECT_EQ(graph.GetEdge(0), subset);
            EXPECT_TRUE(collector.IsEmpty());
        }
    }
}

}
