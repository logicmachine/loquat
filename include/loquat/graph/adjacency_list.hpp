#pragma once
#include <vector>
#include "loquat/graph/types.hpp"

namespace loquat {

template <typename EdgeType>
class adjacency_list {

public:
	using edge_type = EdgeType;
	using edge_list = std::vector<edge_type>;

private:
	std::vector<std::vector<EdgeType>> m_edges;

public:
	adjacency_list()
		: m_edges()
	{ }

	explicit adjacency_list(size_t n)
		: m_edges(n)
	{ }


	size_t size() const {
		return m_edges.size();
	}


	const edge_list& operator[](vertex_t u) const {
		return m_edges[u];
	}

	edge_list& operator[](vertex_t u){
		return m_edges[u];
	}


	template <typename... Args>
	void add_edge(vertex_t from, Args&&... args){
		m_edges[from].emplace_back(std::forward<Args>(args)...);
	}

	void add_edge(vertex_t from, const edge_type& e){
		m_edges[from].emplace_back(e);
	}

};

}

