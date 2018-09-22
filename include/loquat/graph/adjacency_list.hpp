#pragma once
#include <vector>
#include "loquat/graph/types.hpp"

namespace loquat {

/**
 * @brief グラフの隣接リスト表現。
 * @tparam EdgeType 辺の型。
 * @sa loquat::edge
 */
template <typename EdgeType>
class adjacency_list {

public:
	using edge_type = EdgeType;
	using edge_list = std::vector<edge_type>;

private:
	std::vector<std::vector<EdgeType>> m_edges;

public:
	/**
	 * @brief デフォルトコンストラクタ。
	 *
	 * 0個の頂点からなるグラフを生成します。
	 */
	adjacency_list()
		: m_edges()
	{ }

	/**
	 * @brief 頂点数指定付きコンストラクタ。
	 * @param n 生成されるグラフが持つ頂点の数。
	 *
	 * n個の頂点と0本の辺からなるグラフを生成します。
	 */
	explicit adjacency_list(size_t n)
		: m_edges(n)
	{ }


	/**
	 * @brief グラフの頂点数の取得。
	 */
	size_t size() const {
		return m_edges.size();
	}


	/**
	 * @brief ある辺を始端とする辺リストの取得。
	 * @param u 始端とする頂点。
	 */
	const edge_list& operator[](vertex_t u) const {
		return m_edges[u];
	}

	/**
	 * @brief ある辺を始端とする辺リストの取得。
	 * @param u 始端とする頂点。
	 */
	edge_list& operator[](vertex_t u){
		return m_edges[u];
	}


	/**
	 * @brief 辺の追加。
	 * @param from 始端とする頂点。
	 * @param args 辺パラメータ。
	 *             loquat::edge のコンストラクタに渡す引数列を渡す。
	 */
	template <typename... Args>
	void add_edge(vertex_t from, Args&&... args){
		m_edges[from].emplace_back(std::forward<Args>(args)...);
	}

	/**
	 * @brief 辺の追加。
	 * @param from 始端とする頂点。
	 * @param e    辺データ。
	 */
	void add_edge(vertex_t from, const edge_type& e){
		m_edges[from].emplace_back(e);
	}

};

}

