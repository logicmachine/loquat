#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/heavy_light_decomposition.hpp"
#include "loquat/container/segment_tree.hpp"
#include "loquat/container/lazy_segment_tree.hpp"
#include "loquat/math/bitmanip.hpp"
#include "random_graph_generator.hpp"

namespace {

using edge = loquat::edge<>;

template <typename EdgeType>
std::vector<loquat::vertex_t> naive_shortest_path(
	loquat::vertex_t s, loquat::vertex_t t,
	const loquat::adjacency_list<EdgeType>& g)
{
	const size_t n = g.size();
	std::vector<loquat::vertex_t> prev(n);
	loquat::breadth_first_search(
		g, s, [&prev](loquat::vertex_t u, const EdgeType& e){
			prev[e.to] = u;
		});
	std::vector<loquat::vertex_t> result;
	loquat::vertex_t cur = t;
	while(cur != s){
		result.push_back(cur);
		cur = prev[cur];
	}
	result.push_back(cur);
	std::reverse(result.begin(), result.end());
	return result;
}

std::vector<loquat::vertex_t> decomp_shortest_path(
	const std::vector<typename loquat::heavy_light_decomposition::segment>& segments,
	const loquat::heavy_light_decomposition& decomp)
{
	using index_type = typename loquat::heavy_light_decomposition::index_type;
	const auto n = decomp.size();
	std::map<std::pair<index_type, index_type>, loquat::vertex_t> comp;
	for(loquat::vertex_t v = 0; v < n; ++v){
		const auto p = decomp.path_id(v);
		const auto i = decomp.local_index(v);
		comp[std::make_pair(p, i)] = v;
	}
	std::vector<loquat::vertex_t> result;
	for(const auto& s : segments){
		auto cur = s.first;
		while(cur != s.last){
			result.push_back(comp[std::make_pair(s.path_id, cur)]);
			cur += (s.first <= s.last ? 1 : -1);
		}
		result.push_back(comp[std::make_pair(s.path_id, cur)]);
	}
	return result;
}

}

TEST(HeavyLightDecompositionTest, RandomDecompositionHeight){
	std::default_random_engine engine;
	for(const size_t n : { 2, 10, 30, 64}){
		const auto graph = loquat::test::random_tree_generator<edge>(n)
			.generate(engine);
		const size_t height_limit =
			loquat::bitmanip::ctz(loquat::bitmanip::clp2(n));
		loquat::heavy_light_decomposition decomp(graph);
		for(loquat::vertex_t s = 0; s < n; ++s){
			for(loquat::vertex_t t = 0; t < n; ++t){
				const auto expect = naive_shortest_path(s, t, graph);
				const auto actual = decomp.shortest_path(s, t);
				EXPECT_LE(actual.size(), height_limit * 2);
				EXPECT_EQ(expect, decomp_shortest_path(actual, decomp));
			}
		}
	}
}


namespace {

struct behavior {
	using value_type = std::vector<int>;

	value_type identity() const { return {}; }

	value_type merge(value_type a, const value_type& b) const {
		for(const auto& x : b){ a.push_back(x); }
		return a;
	}

	value_type reverse(value_type x) const {
		std::reverse(x.begin(), x.end());
		return x;
	}
};

using segment_tree = loquat::segment_tree<behavior>;

}

TEST(HeavyLightDecompositionTest, VertexPathQuery){
	std::default_random_engine engine;
	const size_t n = 64;
	std::uniform_int_distribution<int> type_dist(0, 1);
	std::uniform_int_distribution<int> init_dist(-100, 100);
	std::uniform_int_distribution<loquat::vertex_t> vertex_dist(0, n - 1);
	const auto graph = loquat::test::random_tree_generator<edge>(n)
		.generate(engine);
	std::vector<std::vector<int>> naive_values(n);
	for(size_t i = 0; i < n; ++i){
		naive_values[i].push_back(init_dist(engine));
	}
	loquat::vertex_path_queryable_tree<segment_tree> decomp(
		graph, naive_values.begin(), naive_values.end());
	for(size_t iter = 0; iter < 1000; ++iter){
		const auto s = vertex_dist(engine);
		const auto t = vertex_dist(engine);
		if(type_dist(engine) == 0){
			const int x = init_dist(engine);
			naive_values[s][0] = x;
			decomp.update(s, {x});
		}else{
			std::vector<int> expect;
			for(const auto v : naive_shortest_path(s, t, graph)){
				expect.push_back(naive_values[v][0]);
			}
			const auto actual = decomp.query(s, t);
			EXPECT_EQ(expect, actual);
		}
	}
}


namespace {

struct modifier_t {
	int offset;
	int delta;

	modifier_t() : offset(0), delta(0) { }
	modifier_t(int o, int d) : offset(o), delta(d) { }
};

struct lazy_behavior {
	using value_type = int;
	using modifier_type = modifier_t;

	value_type identity_value() const {
		return 0;
	}

	modifier_type identity_modifier() const {
		return modifier_type();
	}

	std::pair<modifier_type, modifier_type>
	split_modifier(const modifier_type& m, size_t k) const {
		return std::make_pair(
			modifier_type(m.offset, m.delta),
			modifier_type(m.offset + m.delta * static_cast<int>(k), m.delta));
	}

	modifier_type merge_modifier(
		const modifier_type& a, const modifier_type& b) const
	{
		return modifier_type(a.offset + b.offset, a.delta + b.delta);
	}

	value_type merge_value(
		const value_type& a, const value_type& b) const
	{
		return a + b;
	}

	value_type modify(size_t n, const value_type& v, const modifier_type& m) const {
		const int k = static_cast<int>(n);
		return v + k * m.offset + (k * (k - 1) / 2) * m.delta;
	}

	modifier_type reverse_modifier(size_t n, const modifier_type& m) const {
		return modifier_type(
			m.offset + m.delta * static_cast<int>(n - 1), -m.delta);
	}
};

using lazy_segment_tree = loquat::lazy_segment_tree<lazy_behavior>;

}

TEST(HeavyLightDecompositionTest, LazyVertexPathQuery){
	std::default_random_engine engine;
	const size_t n = 64;
	std::uniform_int_distribution<int> type_dist(0, 1);
	std::uniform_int_distribution<int> init_dist(-10, 10);
	std::uniform_int_distribution<int> delta_dist(-5, 5);
	std::uniform_int_distribution<loquat::vertex_t> vertex_dist(0, n - 1);
	const auto graph = loquat::test::random_tree_generator<edge>(n)
		.generate(engine);
	std::vector<int> naive_values(n);
	for(size_t i = 0; i < n; ++i){ naive_values[i] = init_dist(engine); }
	loquat::lazy_vertex_path_queryable_tree<lazy_segment_tree> decomp(
		graph, naive_values.begin(), naive_values.end());
	for(size_t iter = 0; iter < 1000; ++iter){
		const auto s = vertex_dist(engine);
		const auto t = vertex_dist(engine);
		if(type_dist(engine) == 0){
			const modifier_t modifier = { init_dist(engine), delta_dist(engine) };
			int cur = modifier.offset;
			for(const auto v : naive_shortest_path(s, t, graph)){
				naive_values[v] += cur;
				cur += modifier.delta;
			}
			decomp.modify(s, t, modifier);
		}else{
			int expect = 0;
			for(const auto v : naive_shortest_path(s, t, graph)){
				expect += naive_values[v];
			}
			const int actual = decomp.query(s, t);
			EXPECT_EQ(expect, actual);
		}
	}
}
