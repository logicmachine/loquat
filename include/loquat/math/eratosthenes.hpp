#pragma once
#include <vector>

namespace loquat {

template <typename T>
std::vector<int> eratosthenes(T& sieve){
	const int n = sieve.size();
	for(int i = 0; i < n; ++i){ sieve[i] = true; }
	if(n >= 1){ sieve[0] = false; }
	if(n >= 2){ sieve[1] = false; }
	std::vector<int> primes;
	for(int i = 2; i < n; ++i){
		if(!sieve[i]){ continue; }
		primes.push_back(i);
		for(int j = i + i; j < n; j += i){ sieve[j] = false; }
	}
	return primes;
}

std::vector<int> eratosthenes(size_t n){
	std::vector<bool> sieve(n);
	return eratosthenes(sieve);
}

}

