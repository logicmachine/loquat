#pragma once

namespace loquat {

namespace edge_param {

struct to_ {

	size_t to;

	explicit to_(size_t t = 0)
		: to(t)
	{ }

};

using to = to_;


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


template <typename... Params>
struct edge : public detail::edge_param_wrapper<edge_param::to_, Params...> {

	edge()
		: detail::edge_param_wrapper<edge_param::to_, Params...>()
	{ }

	template <typename... Args>
	explicit edge(Args&&... args)
		: detail::edge_param_wrapper<edge_param::to_, Params...>(
			std::forward<Args>(args)...)
	{ }

};

}

