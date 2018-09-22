/**
 * @file residual_network.hpp
 */
#pragma once
#include <type_traits>
#include "loquat/graph/edge_traits.hpp"
#include "loquat/graph/adjacency_list.hpp"

namespace loquat {

namespace detail {

template <typename EdgeType>
auto negate_weight(EdgeType& e)
	-> typename std::enable_if<has_weight<EdgeType>::value, void>::type
{
	e.weight = -e.weight;
}

template <typename EdgeType>
auto negate_weight(EdgeType&)
	-> typename std::enable_if<!has_weight<EdgeType>::value, void>::type
{ }

}


/**
 * @brief 逆辺についての情報を付加した辺データ型。
 * @tparam EdgeType 基となる辺データ型。
 */
template <typename EdgeType>
struct residual_edge : public EdgeType {

	using base_type = EdgeType;

	size_t rev;

	/**
	 * @brief デフォルトコンストラクタ。
	 *
	 * 全ての辺パラメータをデフォルト値で初期化します。
	 */
	residual_edge()
		: base_type()
		, rev(0)
	{ }

	/**
	 * @brief パラメータ設定を伴うコンストラクタ。
	 * @param to   辺の終端となる頂点。
	 * @param rev  逆辺を示すインデックス。
	 * @param args 残りの辺パラメータのリスト。
	 */
	template <typename... Args>
	residual_edge(vertex_t to, size_t rev, Args&&... args)
		: base_type(to, std::forward<Args>(args)...)
		, rev(rev)
	{ }

	/**
	 * @brief パラメータ設定を伴うコンストラクタ。
	 * @param e   逆辺の情報を付加する前の辺データ。
	 * @param rev 逆辺を示すインデックス。
	 */
	residual_edge(const base_type& e, size_t rev)
		: base_type(e)
		, rev(rev)
	{ }

};


/**
 * @brief 残余ネットワークの生成。
 * @param graph 逆辺を追加する前のグラフデータ。
 * @tparam EdgeType 元ネットワークの辺データ型。
 *
 * グラフ graph のすべての辺について対応する逆辺を追加したグラフを生成します。
 * 逆辺のパラメータはそれぞれ以下のルールに沿って求められます:
 *   - 容量 (capacity): 0
 *   - 重み (weight, 存在する場合のみ): 符号を反転した値
 *   - その他: 元の辺と同じ値
 *
 * また、元グラフに存在していた辺に対応する辺のインデックスは元グラフと同じとなります。
 * つまり、<tt>graph[u][i]</tt>が存在する場合、<tt>residual[u][i]</tt>は<tt>graph[u][i]</tt>と同じ辺を示しています。
 */
template <typename EdgeType>
adjacency_list<residual_edge<EdgeType>>
make_residual(const adjacency_list<EdgeType>& graph){
	using edge_type = EdgeType;
	using residual_type = residual_edge<edge_type>;
	using capacity_type = typename edge_type::capacity_type;
	const size_t n = graph.size();
	adjacency_list<residual_type> result(n);
	for(vertex_t u = 0; u < n; ++u){
		for(const auto& e : graph[u]){
			result.add_edge(u, residual_type(e, 0));
		}
	}
	for(vertex_t u = 0; u < n; ++u){
		const size_t m = graph[u].size();
		for(size_t i = 0; i < m; ++i){
			auto e = graph[u][i];
			const auto v = e.to;
			e.to = u;
			e.capacity = capacity_type();
			detail::negate_weight(e);
			result[u][i].rev = result[v].size();
			result.add_edge(v, residual_type(e, i));
		}
	}
	return result;
}


}

