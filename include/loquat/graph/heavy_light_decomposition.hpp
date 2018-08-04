#pragma once
#include <vector>
#include <stack>
#include <algorithm>
#include <limits>
#include "loquat/graph/adjacency_list.hpp"
#include "loquat/graph/tree_utility.hpp"
#include "loquat/container/range_query_behavior.hpp"
#include "loquat/container/lazy_range_query_behavior.hpp"

namespace loquat {

class heavy_light_decomposition {

public:
	using index_type = size_t;

	struct segment {
		index_type path_id;
		index_type first;
		index_type last;

		segment() : path_id(0), first(0), last(0) { }
		segment(index_type p, index_type f, index_type l)
			: path_id(p), first(f), last(l)
		{ }
	};

private:
	struct path {
		vertex_t parent;
		size_t rank;
		std::vector<vertex_t> vertices;
	};

	std::vector<path> m_paths;
	std::vector<index_type> m_vertex2path;
	std::vector<index_type> m_vertex2index;

public:
	heavy_light_decomposition()
		: m_paths()
		, m_vertex2path()
		, m_vertex2index()
	{ }

	template <typename EdgeType>
	explicit heavy_light_decomposition(
		const adjacency_list<EdgeType>& graph,
		vertex_t root = 0)
		: m_paths()
		, m_vertex2path(graph.size())
		, m_vertex2index(graph.size())
	{
		struct frame_type {
			vertex_t u;
			vertex_t p;
			frame_type() : u(0), p(0) { }
			frame_type(vertex_t u, vertex_t p) : u(u), p(p) { }
		};
		const vertex_t nil = std::numeric_limits<vertex_t>::max();
		const auto subtree_sizes = compute_subtree_sizes(graph, root);
		std::stack<frame_type> frame_stack;
		frame_stack.emplace(root, nil);
		while(!frame_stack.empty()){
			const auto f = frame_stack.top();
			frame_stack.pop();
			const index_type path_id = m_paths.size();
			path p;
			p.parent = f.p;
			p.rank = (f.p != nil ? m_paths[m_vertex2path[f.p]].rank + 1 : 0);
			vertex_t cur = f.u;
			while(cur != nil){
				const auto ss = subtree_sizes[cur];
				const auto threshold = ss / 2;
				m_vertex2path[cur] = path_id;
				m_vertex2index[cur] = p.vertices.size();
				p.vertices.push_back(cur);
				vertex_t heavy_edge = nil;
				for(const auto& e : graph[cur]){
					const auto v = e.to;
					if(subtree_sizes[v] > subtree_sizes[cur]){ continue; }
					if(heavy_edge == nil && subtree_sizes[v] >= threshold){
						heavy_edge = v;
					}else{
						frame_stack.emplace(v, cur);
					}
				}
				cur = heavy_edge;
			}
			m_paths.push_back(p);
		}
	}

	size_t size() const {
		return m_vertex2path.size();
	}

	size_t count_heavy_paths() const {
		return m_paths.size();
	}

	size_t path_length(index_type p) const {
		return m_paths[p].vertices.size();
	}

	vertex_t parent(index_type p) const {
		return m_paths[p].parent;
	}

	index_type path_id(vertex_t v) const {
		return m_vertex2path[v];
	}

	index_type local_index(vertex_t v) const {
		return m_vertex2index[v];
	}

	std::vector<segment> shortest_path(vertex_t s, vertex_t t) const {
		std::vector<segment> s_segments, t_segments;
		index_type s_path = path_id(s), s_index = local_index(s);
		index_type t_path = path_id(t), t_index = local_index(t);
		while(s_path != t_path){
			const size_t s_rank = m_paths[s_path].rank;
			const size_t t_rank = m_paths[t_path].rank;
			if(s_rank >= t_rank){
				const auto parent = m_paths[s_path].parent;
				s_segments.push_back(segment(s_path, s_index, 0));
				s_path  = path_id(parent);
				s_index = local_index(parent);
			}
			if(t_rank >= s_rank){
				const auto parent = m_paths[t_path].parent;
				t_segments.push_back(segment(t_path, 0, t_index));
				t_path  = path_id(parent);
				t_index = local_index(parent);
			}
		}
		std::vector<segment> result(s_segments);
		result.emplace_back(s_path, s_index, t_index);
		std::reverse(t_segments.begin(), t_segments.end());
		for(const auto& s : t_segments){ result.push_back(s); }
		return result;
	}

};


template <typename RangeQueryable>
class vertex_path_queryable_tree {
	
public:
	using range_queryable_type = RangeQueryable;
	using behavior_type = typename range_queryable_type::behavior_type;
	using value_type = typename behavior_type::value_type;

private:
	heavy_light_decomposition m_decomposition;
	std::vector<range_queryable_type> m_range_queryables;
	range_query_behavior_wrapper<behavior_type> m_behavior;

public:
	vertex_path_queryable_tree()
		: m_decomposition()
		, m_range_queryables()
		, m_behavior()
	{ }

	template <typename EdgeType>
	explicit vertex_path_queryable_tree(
		const adjacency_list<EdgeType>& graph,
		vertex_t root = 0,
		const behavior_type& behavior = behavior_type())
		: m_decomposition(graph, root)
		, m_range_queryables()
		, m_behavior(behavior)
	{
		const size_t n_paths = m_decomposition.count_heavy_paths();
		m_range_queryables.reserve(n_paths);
		for(size_t i = 0; i < n_paths; ++i){
			m_range_queryables.emplace_back(
				m_decomposition.path_length(i), behavior);
		}
	}

	template <typename Iterator, typename EdgeType>
	explicit vertex_path_queryable_tree(
		const adjacency_list<EdgeType>& graph,
		Iterator first,
		Iterator last,
		vertex_t root = 0,
		const behavior_type& behavior = behavior_type())
		: m_decomposition(graph, root)
		, m_range_queryables()
		, m_behavior(behavior)
	{
		const size_t n_paths = m_decomposition.count_heavy_paths();
		std::vector<std::vector<value_type>> initial_values(n_paths);
		for(size_t i = 0; i < n_paths; ++i){
			initial_values[i].resize(m_decomposition.path_length(i));
		}
		auto it = first;
		for(vertex_t i = 0; it != last; ++it, ++i){
			const auto path_id = m_decomposition.path_id(i);
			const auto local_index = m_decomposition.local_index(i);
			initial_values[path_id][local_index] = *it;
		}
		m_range_queryables.reserve(n_paths);
		for(size_t i = 0; i < n_paths; ++i){
			m_range_queryables.emplace_back(
				initial_values[i].begin(), initial_values[i].end(), behavior);
		}
	}

	void update(vertex_t v, const value_type& x){
		const auto path_id = m_decomposition.path_id(v);
		const auto local_index = m_decomposition.local_index(v);
		m_range_queryables[path_id].update(local_index, x);
	}

	value_type query(vertex_t s, vertex_t t){
		const auto shortest_path = m_decomposition.shortest_path(s, t);
		value_type acc = m_behavior.identity();
		for(const auto& s : shortest_path){
			const auto path_id = s.path_id;
			const auto lo = std::min(s.first, s.last);
			const auto hi = std::max(s.first, s.last) + 1;
			const auto partial = m_range_queryables[path_id].query(lo, hi);
			const auto directed_partial = s.first > s.last
				? m_behavior.reverse(hi - lo, partial)
				: partial;
			acc = m_behavior.merge(acc, directed_partial);
		}
		return acc;
	}

};


template <typename RangeQueryable>
class lazy_vertex_path_queryable_tree {
	
public:
	using range_queryable_type = RangeQueryable;
	using behavior_type = typename range_queryable_type::behavior_type;
	using value_type = typename behavior_type::value_type;
	using modifier_type = typename behavior_type::modifier_type;

private:
	heavy_light_decomposition m_decomposition;
	std::vector<range_queryable_type> m_range_queryables;
	lazy_range_query_behavior_wrapper<behavior_type> m_behavior;

public:
	lazy_vertex_path_queryable_tree()
		: m_decomposition()
		, m_range_queryables()
		, m_behavior()
	{ }

	template <typename EdgeType>
	explicit lazy_vertex_path_queryable_tree(
		const adjacency_list<EdgeType>& graph,
		vertex_t root = 0,
		const behavior_type& behavior = behavior_type())
		: m_decomposition(graph, root)
		, m_range_queryables()
		, m_behavior(behavior)
	{
		const size_t n_paths = m_decomposition.count_heavy_paths();
		m_range_queryables.reserve(n_paths);
		for(size_t i = 0; i < n_paths; ++i){
			m_range_queryables.emplace_back(
				m_decomposition.path_length(i), behavior);
		}
	}

	template <typename Iterator, typename EdgeType>
	explicit lazy_vertex_path_queryable_tree(
		const adjacency_list<EdgeType>& graph,
		Iterator first,
		Iterator last,
		vertex_t root = 0,
		const behavior_type& behavior = behavior_type())
		: m_decomposition(graph, root)
		, m_range_queryables()
		, m_behavior(behavior)
	{
		const size_t n_paths = m_decomposition.count_heavy_paths();
		std::vector<std::vector<value_type>> initial_values(n_paths);
		for(size_t i = 0; i < n_paths; ++i){
			initial_values[i].resize(m_decomposition.path_length(i));
		}
		auto it = first;
		for(vertex_t i = 0; it != last; ++it, ++i){
			const auto path_id = m_decomposition.path_id(i);
			const auto local_index = m_decomposition.local_index(i);
			initial_values[path_id][local_index] = *it;
		}
		m_range_queryables.reserve(n_paths);
		for(size_t i = 0; i < n_paths; ++i){
			m_range_queryables.emplace_back(
				initial_values[i].begin(), initial_values[i].end(), behavior);
		}
	}

	void update(vertex_t v, const value_type& x){
		const auto path_id = m_decomposition.path_id(v);
		const auto local_index = m_decomposition.local_index(v);
		m_range_queryables[path_id].update(local_index, x);
	}

	void modify(vertex_t s, vertex_t t, const modifier_type& m){
		const auto shortest_path = m_decomposition.shortest_path(s, t);
		modifier_type modifier = m;
		for(const auto& s : shortest_path){
			const auto path_id = s.path_id;
			const auto lo = std::min(s.first, s.last);
			const auto hi = std::max(s.first, s.last) + 1;
			const auto modifier_pair =
				m_behavior.split_modifier(modifier, hi - lo);
			const auto head_modifier = s.first > s.last
				? m_behavior.reverse_modifier(hi - lo, modifier_pair.first)
				: modifier_pair.first;
			m_range_queryables[path_id].modify(lo, hi, head_modifier);
			modifier = modifier_pair.second;
		}
	}

	value_type query(vertex_t s, vertex_t t){
		const auto shortest_path = m_decomposition.shortest_path(s, t);
		value_type acc = m_behavior.identity_value();
		for(const auto& s : shortest_path){
			const auto path_id = s.path_id;
			const auto lo = std::min(s.first, s.last);
			const auto hi = std::max(s.first, s.last) + 1;
			const auto partial = m_range_queryables[path_id].query(lo, hi);
			const auto directed_partial = s.first > s.last
				? m_behavior.reverse_value(hi - lo, partial)
				: partial;
			acc = m_behavior.merge_value(acc, directed_partial);
		}
		return acc;
	}

};

}
