#pragma once
#include <algorithm>
#include <cstdlib>
#include <cassert>
#include "loquat/math/identity.hpp"

namespace loquat {

class rational {

public:
	using value_type = long long;

private:
	value_type m_nume;
	value_type m_deno;

	static rational unsafe_construct(
		value_type nume, value_type deno) noexcept
	{
		rational r;
		r.m_nume = nume;
		r.m_deno = deno;
		return r;
	}

	void normalize() noexcept {
		assert(m_deno != 0);
		if(m_deno < 0){
			m_nume = -m_nume;
			m_deno = -m_deno;
		}
		const auto g = std::__gcd(llabs(m_nume), m_deno);
		m_nume /= g;
		m_deno /= g;
	}


public:
	rational() noexcept
		: m_nume(0)
		, m_deno(1)
	{ }

	rational(value_type x) noexcept
		: m_nume(x)
		, m_deno(1)
	{ }

	rational(long long nume, long long deno) noexcept
		: m_nume(nume)
		, m_deno(deno)
	{
		normalize();
	}


	long long nume() const noexcept {
		return m_nume;
	}

	long long deno() const noexcept {
		return m_deno;
	}

	double to_double() const noexcept {
		return static_cast<double>(m_nume) / m_deno;
	}


	bool operator==(const rational& r) const noexcept {
		return m_nume == r.m_nume && m_deno == r.m_deno;
	}
	
	bool operator!=(const rational& r) const noexcept {
		return !(*this == r);
	}

	bool operator<(const rational& r) const noexcept {
		return (m_nume * r.m_deno) < (r.m_nume * m_deno);
	}

	bool operator<=(const rational& r) const noexcept {
		return (m_nume * r.m_deno) <= (r.m_nume * m_deno);
	}

	bool operator>(const rational& r) const noexcept {
		return (m_nume * r.m_deno) > (r.m_nume * m_deno);
	}

	bool operator>=(const rational& r) const noexcept {
		return (m_nume * r.m_deno) >= (r.m_nume * m_deno);
	}


	rational operator+() const noexcept {
		return *this;
	}

	rational operator-() const noexcept {
		return unsafe_construct(-m_nume, m_deno);
	}


	rational operator+(const rational& r) const noexcept {
		return rational(
			m_nume * r.m_deno + r.m_nume * m_deno,
			m_deno * r.m_deno);
	}

	rational operator-(const rational& r) const noexcept {
		return rational(
			m_nume * r.m_deno - r.m_nume * m_deno,
			m_deno * r.m_deno);
	}

	rational operator*(const rational& r) const noexcept {
		return rational(m_nume * r.m_nume, m_deno * r.m_deno);
	}

	rational operator/(const rational& r) const noexcept {
		return rational(m_nume * r.m_deno, m_deno * r.m_nume);
	}


	rational& operator+=(const rational& r) noexcept {
		return (*this = *this + r);
	}

	rational& operator-=(const rational& r) noexcept {
		return (*this = *this - r);
	}

	rational& operator*=(const rational& r) noexcept {
		return (*this = *this * r);
	}

	rational& operator/=(const rational& r) noexcept {
		return (*this = *this / r);
	}

};


inline std::istream& operator>>(std::istream& is, rational& x){
	using value_type = rational::value_type;
	value_type nume = 0, deno = 1;
	is >> nume;
	if(is.peek() == '/'){
		is.ignore();
		is >> deno;
	}
	x = rational(nume, deno);
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const rational& x){
	os << x.nume();
	if(x.deno() != 1){ os << "/" << x.deno(); }
	return os;
}


template <>
struct identity<std::plus<rational>> {
	static rational value() noexcept {
		return rational(0);
	}
};

template <>
struct identity<std::multiplies<rational>> {
	static rational value() noexcept {
		return rational(1);
	}
};

}
