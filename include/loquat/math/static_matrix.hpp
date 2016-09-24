#pragma once
#include <array>
#include <initializer_list>
#include <stdexcept>

namespace loquat {

template <typename T, size_t N, size_t M>
class static_matrix {

public:
	using value_type = T;
	using self_type = static_matrix<T, N, M>;

private:
	std::array<std::array<value_type, M>, N> m_data;


	template <typename F>
	self_type apply_unary(F&& f) const {
		self_type x;
		for(size_t i = 0; i < N; ++i){
			for(size_t j = 0; j < M; ++j){ x.m_data[i][j] = f(m_data[i][j]); }
		}
		return x;
	}

	template <typename F>
	self_type apply_binary(const self_type& x, F&& f) const {
		self_type y;
		for(size_t i = 0; i < N; ++i){
			for(size_t j = 0; j < M; ++j){
				y.m_data[i][j] = f(m_data[i][j], x.m_data[i][j]);
			}
		}
		return y;
	}


public:
	static_matrix()
		: m_data()
	{
		for(auto& row : m_data){
			for(auto& item : row){ item = value_type(); }
		}
	}

	explicit static_matrix(const value_type& x)
		: m_data()
	{
		for(auto& row : m_data){
			for(auto& item : row){ item = x; }
		}
	}

	static_matrix(std::initializer_list<value_type> l)
		: m_data()
	{
		if(l.size() != N * M){ throw std::logic_error("size mismatch"); }
		auto it = l.begin();
		for(auto& row : m_data){
			for(auto& item : row){ item = *(it++); }
		}
	}


	static self_type identity(){
		static_assert(
			N == M, "identity matrix is not defined for non-square matrix");
		self_type mat;
		for(size_t i = 0; i < N; ++i){ mat.m_data[i][i] = 1; }
		return mat;
	}


	size_t rows() const {
		return N;
	}

	size_t columns() const {
		return M;
	}


	value_type operator()(size_t i, size_t j) const {
		return m_data[i][j];
	}

	value_type& operator()(size_t i, size_t j){
		return m_data[i][j];
	}


	bool operator==(const self_type& x) const {
		for(size_t i = 0; i < N; ++i){
			for(size_t j = 0; j < M; ++j){
				if(m_data[i][j] != x.m_data[i][j]){ return false; }
			}
		}
		return true;
	}

	bool operator!=(const self_type& x) const {
		return !(*this == x);
	}


	self_type operator+() const {
		return *this;
	}

	self_type operator-() const {
		return apply_unary(std::negate<value_type>());
	}


	self_type operator+(const self_type& x) const {
		return apply_binary(x, std::plus<value_type>());
	}

	self_type operator-(const self_type& x) const {
		return apply_binary(x, std::minus<value_type>());
	}

	template <size_t K>
	static_matrix<T, N, K> operator*(const static_matrix<T, M, K>& x) const {
		static_matrix<T, N, K> y;
		for(size_t i = 0; i < N; ++i){
			for(size_t j = 0; j < M; ++j){
				for(size_t k = 0; k < K; ++k){
					y(i, k) += m_data[i][j] * x(j, k);
				}
			}
		}
		return y;
	}

	self_type operator*(const value_type& x) const {
		return apply_unary([x](const value_type& y){ return y * x; });
	}

	self_type operator/(const value_type& x) const {
		return apply_unary([x](const value_type& y){ return y / x; });
	}


	self_type& operator+=(const self_type& x){
		return (*this = *this + x);
	}

	self_type& operator-=(const self_type& x){
		return (*this = *this - x);
	}

	self_type& operator*=(const self_type& x){
		return (*this = *this * x);
	}

	self_type& operator*=(const value_type& x){
		return (*this = *this * x);
	}


	self_type operator~() const {
		return apply_unary([](const value_type& x){ return ~x; });
	}


	self_type operator&(const self_type& x) const {
		return apply_binary(x, std::bit_and<value_type>());
	}

	self_type operator|(const self_type& x) const {
		return apply_binary(x, std::bit_or<value_type>());
	}

	self_type operator^(const self_type& x) const {
		return apply_binary(x, std::bit_xor<value_type>());
	}


	self_type& operator&=(const self_type& x){
		return (*this = (*this & x));
	}

	self_type& operator|=(const self_type& x){
		return (*this = (*this | x));
	}

	self_type& operator^=(const self_type& x){
		return (*this = (*this ^ x));
	}

};


template <typename T, size_t N, size_t M>
static_matrix<T, N, M> operator*(const T& x, const static_matrix<T, N, M>& y){
	static_matrix<T, N, M> z;
	for(size_t i = 0; i < N; ++i){
		for(size_t j = 0; j < M; ++j){ z(i, j) = x * y(i, j); }
	}
	return z;
}

}

