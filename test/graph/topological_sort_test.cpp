#include <gtest/gtest.h>
#include "loquat/graph/edge.hpp"
#include "loquat/graph/topological_sort.hpp"
#include "random_graph_generator.hpp"

TEST(TopologicalSortTest, Ordered){
	using edge = loquat::edge<>;
	const size_t n = 500;
	const double p = 0.1;
	std::default_random_engine engine;
	std::uniform_real_distribution<double> e_dist(0.0, 1.0);
	std::vector<loquat::vertex_t> order(n);
	std::iota(order.begin(), order.end(), 0);
	std::shuffle(order.begin(), order.end(), engine);
	loquat::adjacency_list<edge> graph(n);
	for(size_t i = 0; i < n; ++i){
		const auto u = order[i];
		for(size_t j = i + 1; j < n; ++j){
			const auto v = order[j];
			if(e_dist(engine) < p){ graph.add_edge(u, v); }
		}
	}
	std::vector<std::vector<bool>> reachable(n, std::vector<bool>(n));
	for(loquat::vertex_t root = 0; root < n; ++root){
		std::queue<loquat::vertex_t> q;
		q.push(root);
		reachable[root][root] = true;
		while(!q.empty()){
			const auto u = q.front();
			q.pop();
			for(const auto& e : graph[u]){
				const auto v = e.to;
				if(reachable[root][v]){ continue; }
				reachable[root][v] = true;
				q.push(v);
			}
		}
	}
	const auto actual = loquat::topological_sort(graph);
	EXPECT_EQ(n, actual.size());
	std::vector<bool> occur(n);
	for(size_t i = 0; i < n; ++i){
		EXPECT_TRUE(actual[i] < n);
		EXPECT_FALSE(occur[actual[i]]);
		occur[actual[i]] = true;
	}
	for(size_t i = 0; i < n; ++i){
		const auto v = actual[i];
		for(size_t j = 0; j < i; ++j){
			const auto u = actual[j];
			EXPECT_FALSE(reachable[v][u]);
		}
	}
}

TEST(TopologicalSortTest, Unordered){
	using edge = loquat::edge<>;
	{	// Cyclic
		loquat::adjacency_list<edge> graph(5);
		graph.add_edge(0, 1);
		graph.add_edge(1, 2);
		graph.add_edge(2, 3);
		graph.add_edge(3, 0);
		EXPECT_THROW(
			loquat::topological_sort(graph),
			loquat::no_solution_error);
	}
	{	// Self-loop
		loquat::adjacency_list<edge> graph(4);
		graph.add_edge(0, 0);
		graph.add_edge(1, 2);
		graph.add_edge(2, 3);
		EXPECT_THROW(
			loquat::topological_sort(graph),
			loquat::no_solution_error);
	}
}

