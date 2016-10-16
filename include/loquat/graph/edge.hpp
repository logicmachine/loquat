#pragma once

namespace loquat {

namespace edge_param {

struct to_ {

	size_t to;

	explicit to_(size_t t)
		: to(t)
	{ }

};

using to = to_;


template <typename T>
struct weight_ {

	T weight;

	explicit weight_(const T& w)
		: weight(w)
	{ }

};

template <typename T>
using weight = weight_<T>;


template <typename T>
struct capacity_ {

	T capacity;

	explicit capacity_(const T& c)
		: capacity(c)
	{ }

};

template <typename T>
using capacity = capacity_<T>;

}


namespace detail {

template <typename T, typename... Params>
struct edge_param_wrapper : public T, edge_param_wrapper<Params...> {

	template <typename U, typename... Args>
	explicit edge_param_wrapper(U&& x, Args&&... args)
		: T(std::forward<U>(x))
		, edge_param_wrapper<Params...>(std::forward<Args>(args)...)
	{ }

};


template <typename T>
struct edge_param_wrapper<T> : public T {

	template <typename U>
	explicit edge_param_wrapper(U&& x)
		: T(std::forward<U>(x))
	{ }

};

}


template <typename... Params>
struct edge : public detail::edge_param_wrapper<edge_param::to_, Params...> {

	template <typename... Args>
	explicit edge(Args&&... args)
		: detail::edge_param_wrapper<edge_param::to_, Params...>(
			std::forward<Args>(args)...)
	{ }

};

}

