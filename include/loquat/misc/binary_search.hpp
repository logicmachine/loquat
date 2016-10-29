#pragma once
#include <type_traits>

namespace loquat {

template <typename T, typename F>
auto binary_search(T l, T r, F&& less)
	-> typename std::enable_if<std::is_integral<T>::value, T>::type
{
	while(l < r){
		const auto c = l + (r - l) / 2;
		if(less(c)){
			l = c + 1;
		}else{
			r = c;
		}
	}
	return l;
}

template <typename T, typename F>
auto binary_search(T l, T r, F&& less, int num_iteration = 100)
	-> typename std::enable_if<std::is_floating_point<T>::value, T>::type
{
	for(int i = 0; i < num_iteration; ++i){
		const auto c = l + (r - l) / 2;
		if(less(c)){
			l = c;
		}else{
			r = c;
		}
	}
	return l;
}

}

