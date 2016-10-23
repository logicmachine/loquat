#pragma once
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {
namespace test {

template <typename EdgeType>
bool validate_maxflow_result(
	typename EdgeType::capacity_type flow,
	const adjacency_list<EdgeType>& out_graph,
	vertex_t source,
	vertex_t sink,
	const adjacency_list<EdgeType>& in_graph)
{
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

	std::vector<bool> pass(n);
	std::queue<vertex_t> q;
	pass[source] = true;
	q.push(source);
	while(!q.empty()){
		const auto u = q.front();
		q.pop();
		for(const auto& e : out_graph[u]){
			const auto v = e.to;
			if(pass[v] || e.capacity <= 0){ continue; }
			pass[v] = true;
			q.push(v);
		}
	}
	if(pass[sink]){ return false; }

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
			if(in_flow[v] < out_flow[v]){ return false; }
		}
	}
	return true;
}

}
}

