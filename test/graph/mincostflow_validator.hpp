#pragma once
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/topological_sort.hpp"
#include "loquat/graph/sssp_bellman_ford.hpp"

namespace loquat {
namespace test {

template <typename EdgeType>
bool validate_mincostflow_result(
	typename EdgeType::weight_type actual,
	const adjacency_list<EdgeType>& out_graph,
	vertex_t source,
	vertex_t sink,
	typename EdgeType::capacity_type flow,
	const adjacency_list<EdgeType>& in_graph)
{
	using weight_type = typename EdgeType::weight_type;
	using capacity_type = typename EdgeType::capacity_type;
	if(out_graph.size() != in_graph.size()){ return false; }
	const auto n = out_graph.size();
	for(vertex_t u = 0; u < n; ++u){
		if(out_graph[u].size() != in_graph[u].size()){ return false; }
		const auto m = out_graph[u].size();
		for(vertex_t i = 0; i < m; ++i){
			const auto& e_in = in_graph[u][i];
			const auto& e_out = out_graph[u][i];
			if(e_out.to != e_in.to){ return false; }
			if(e_out.rev != e_in.rev){ return false; }
			if(e_out.capacity < 0){ return false; }
			const auto cap_in =
				e_in.capacity + in_graph[e_in.to][e_in.rev].capacity;
			const auto cap_out =
				e_out.capacity + out_graph[e_out.to][e_out.rev].capacity;
			if(cap_in != cap_out){ return false; }
		}
	}

	std::vector<capacity_type> in_flow(n), out_flow(n);
	for(vertex_t u = 0; u < n; ++u){
		const auto m = out_graph[u].size();
		for(size_t i = 0; i < m; ++i){
			const auto& e_in = in_graph[u][i];
			const auto& e_out = out_graph[u][i];
			if(e_in.capacity == 0){ continue; }
			const auto v = e_in.to;
			const auto f = e_in.capacity - e_out.capacity;
			out_flow[u] += f;
			in_flow[v] += f;
		}
	}
	for(vertex_t v = 0; v < n; ++v){
		if(v == source){
			if(out_flow[v] - in_flow[v] != flow){ return false; }
		}else if(v == sink){
			if(in_flow[v] - out_flow[v] != flow){ return false; }
		}else{
			if(in_flow[v] != out_flow[v]){ return false; }
		}
	}

	using weighted_edge = edge<edge_param::weight<weight_type>>;
	adjacency_list<weighted_edge> flowing_graph(n);
	adjacency_list<weighted_edge> flowable_graph(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : out_graph[u]){
			const auto& r = out_graph[e.to][e.rev];
			if(r.capacity > 0 && e.weight > 0){
				flowing_graph.add_edge(u, e.to, e.weight);
			}
			if(e.capacity > 0){
				flowable_graph.add_edge(u, e.to, e.weight);
			}
		}
	}

	const auto neg_inf = negative_infinity<weight_type>();
	std::vector<weight_type> dp(n, neg_inf);
	for(const auto& u : topological_sort(flowing_graph)){
		if(u == source){ dp[u] = weight_type(); }
		if(is_negative_infinity(dp[u])){ continue; }
		for(const auto& e : flowing_graph[u]){
			dp[e.to] = std::max(dp[e.to], dp[u] + e.weight);
		}
	}
	const auto remaining = sssp_bellman_ford(source, flowable_graph);
	if(remaining[sink] < dp[sink]){ return false; }

	weight_type weight_sum = 0;
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : out_graph[u]){
			if(e.weight > 0){
				weight_sum += out_graph[e.to][e.rev].capacity * e.weight;
			}
		}
	}
	return weight_sum == actual;
}

}
}

