#pragma once
#include <vector>
#include <stack>
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

template <typename EdgeType>
std::vector<size_t> tree_pre_ordering(
	vertex_t root, const adjacency_list<EdgeType>& g)
{
	struct frame_type {
		vertex_t u, p;
		size_t i;
		frame_type(vertex_t u, vertex_t p, size_t i)
			: u(u), p(p), i(i)
		{ }
	};
	const auto n = g.size();
	std::vector<size_t> result(n);
	std::stack<frame_type> frames;
	frames.emplace(root, root, 0);
	size_t index = 0;
	while(!frames.empty()){
		const auto f = frames.top();
		frames.pop();
		if(f.i == 0){ result[f.u] = index++; }
		if(f.i != g[f.u].size()){
			frames.emplace(f.u, f.p, f.i + 1);
			if(g[f.u][f.i].to != f.p){
				frames.emplace(g[f.u][f.i].to, f.u, 0);
			}
		}
	}
	return result;
}

template <typename EdgeType>
std::vector<size_t> tree_post_ordering(
	vertex_t root, const adjacency_list<EdgeType>& g)
{
	struct frame_type {
		vertex_t u, p;
		size_t i;
		frame_type(vertex_t u, vertex_t p, size_t i)
			: u(u), p(p), i(i)
		{ }
	};
	const auto n = g.size();
	std::vector<size_t> result(n);
	std::stack<frame_type> frames;
	frames.emplace(root, root, 0);
	size_t index = 0;
	while(!frames.empty()){
		const auto f = frames.top();
		frames.pop();
		if(f.i == g[f.u].size()){
			result[f.u] = index++;
		}else{
			frames.emplace(f.u, f.p, f.i + 1);
			if(g[f.u][f.i].to != f.p){
				frames.emplace(g[f.u][f.i].to, f.u, 0);
			}
		}
	}
	return result;
}

}
