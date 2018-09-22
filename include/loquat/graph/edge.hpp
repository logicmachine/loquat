/**
 * @file edge.hpp
 */
#pragma once
#include "loquat/graph/types.hpp"

namespace loquat {

namespace edge_param {

struct to_ {

	vertex_t to;

	explicit to_(vertex_t t = 0)
		: to(t)
	{ }

};

using to = to_;


/**
 * @brief 辺が重みを持つことを表す構造体。
 * @tparam T 重みを表す値の型。
 *
 * 通常は対応するエイリアスである loquat::edge_param::weight を使用してください。
 */
template <typename T>
struct weight_ {

	using weight_type = T;

	weight_type weight;

	explicit weight_(const weight_type& w = weight_type())
		: weight(w)
	{ }

};

template <typename T>
using weight = weight_<T>;


/**
 * @brief 辺が容量を持つことを表す構造体。
 * @tparam T 容量を表す値の型。
 *
 * 通常は対応するエイリアスである loquat::edge_param::capacity を使用してください。
 */
template <typename T>
struct capacity_ {

	using capacity_type = T;

	capacity_type capacity;

	explicit capacity_(const capacity_type& c = capacity_type())
		: capacity(c)
	{ }

};

template <typename T>
using capacity = capacity_<T>;

}


namespace detail {

template <typename T, typename... Params>
struct edge_param_wrapper : public T, edge_param_wrapper<Params...> {

	edge_param_wrapper()
		: T()
		, edge_param_wrapper<Params...>()
	{ }

	template <typename U, typename... Args>
	explicit edge_param_wrapper(U&& x, Args&&... args)
		: T(std::forward<U>(x))
		, edge_param_wrapper<Params...>(std::forward<Args>(args)...)
	{ }

};


template <typename T>
struct edge_param_wrapper<T> : public T {

	edge_param_wrapper()
		: T()
	{ }

	template <typename U>
	explicit edge_param_wrapper(U&& x)
		: T(std::forward<U>(x))
	{ }

};

}


/**
 * @brief 拡張可能な辺データ型。
 * @tparam Params  辺が持つ付加情報を表す型のリスト。
 */
template <typename... Params>
struct edge : public detail::edge_param_wrapper<edge_param::to_, Params...> {

	/**
	 * @brief デフォルトコンストラクタ。
	 *
	 * 辺の持つすべてのパラメータがデフォルト値で初期化されます。
	 */
	edge()
		: detail::edge_param_wrapper<edge_param::to_, Params...>()
	{ }

	/**
	 * @brief パラメータ設定を伴うコンストラクタ。
	 * @param args 辺のパラメータとして設定する値のリスト。
	 *             先頭要素が辺のさす先 (to) それ以降は Params に指定した順番で渡します。
	 */
	template <typename... Args>
	explicit edge(Args&&... args)
		: detail::edge_param_wrapper<edge_param::to_, Params...>(
			std::forward<Args>(args)...)
	{ }

};

}

