#pragma once
#include <iostream>
#include "loquat/math/extended_gcd.hpp"
#include "loquat/math/identity.hpp"

namespace loquat {

template <int MOD>
class modulus_integer {

public:
	typedef modulus_integer<MOD> self_type;

private:
	int m_value;

	static self_type unsafe_construct(int x) noexcept {
		self_type y;
		y.m_value = x;
		return y;
	}

public:
	modulus_integer() noexcept
		: m_value(0)
	{ }

	modulus_integer(int x) noexcept
		: m_value(x % MOD)
	{
		if(m_value < 0){ m_value += MOD; }
	}


	int operator*() const noexcept { return m_value; }


	self_type& operator=(const self_type& x) noexcept {
		m_value = x.m_value;
		return *this;
	}


	bool operator==(const self_type& x) const noexcept {
		return m_value == x.m_value;
	}

	bool operator!=(const self_type& x) const noexcept {
		return m_value != x.m_value;
	}

	bool operator<(const self_type& x) const noexcept {
		return m_value < x.m_value;
	}

	bool operator<=(const self_type& x) const noexcept {
		return m_value <= x.m_value;
	}

	bool operator>(const self_type& x) const noexcept {
		return m_value > x.m_value;
	}

	bool operator>=(const self_type& x) const noexcept {
		return m_value >= x.m_value;
	}


	self_type operator+() const noexcept {
		return *this;
	}

	self_type operator-() const noexcept {
		return unsafe_construct(m_value > 0 ? MOD - m_value : 0);
	}


	self_type operator+(const self_type& x) const noexcept {
		const int y = m_value + x.m_value;
		return unsafe_construct(y >= MOD ? y - MOD : y);
	}

	self_type operator-(const self_type& x) const noexcept {
		const int y = m_value - x.m_value;
		return unsafe_construct(y < 0 ? y + MOD : y);
	}

	self_type operator*(const self_type& x) const noexcept {
		return unsafe_construct(
			static_cast<long long>(m_value) * x.m_value % MOD);
	}

	self_type operator/(const self_type& x) const {
		return (*this) * self_type(extended_gcd(x.m_value, MOD).first);
	}


	self_type& operator+=(const self_type& x) noexcept {
		return (*this = *this + x);
	}

	self_type& operator-=(const self_type &x) noexcept {
		return (*this = *this - x);
	}

	self_type& operator*=(const self_type& x) noexcept {
		return (*this = *this * x);
	}

	self_type& operator/=(const self_type& x){
		return (*this = *this / x);
	}


	self_type& operator++() noexcept {
		if(++m_value >= MOD){ m_value = 0; }
		return *this;
	}

	self_type& operator--() noexcept {
		if(--m_value < 0){ m_value = MOD - 1; }
		return *this;
	}

	self_type operator++(int) noexcept {
		self_type t = *this;
		if(++m_value >= MOD){ m_value = 0; }
		return t;
	}

	self_type operator--(int) noexcept {
		self_type t = *this;
		if(--m_value < 0){ m_value = MOD - 1; }
		return t;
	}

};


template <int MOD>
inline std::istream& operator>>(
	std::istream& is, const modulus_integer<MOD>& x)
{
	int y;
	is >> y;
	x = modulus_integer<MOD>(y);
	return is;
}

template <int MOD>
inline std::ostream& operator<<(
	std::ostream& os, const modulus_integer<MOD>& x)
{
	os << *x;
	return os;
}


template <int MOD>
struct identity<std::plus<modulus_integer<MOD>>> {
	static modulus_integer<MOD> value() noexcept {
		return modulus_integer<MOD>(0);
	}
};

template <int MOD>
struct identity<std::multiplies<modulus_integer<MOD>>> {
	static modulus_integer<MOD> value() noexcept {
		return modulus_integer<MOD>(1);
	}
};

}

