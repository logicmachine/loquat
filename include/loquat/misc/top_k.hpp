#pragma once
#include <array>
#include <functional>
#include <utility>

namespace loquat {

template <typename T, size_t K, typename Comparator = std::less<T>>
class top_k {

public:
	using value_type = T;
	using const_iterator = typename std::array<T, K>::const_iterator;

private:
	Comparator m_comparator;
	size_t m_size;
	std::array<T, K> m_data;

public:
	top_k()
		: m_comparator()
		, m_size(0)
		, m_data()
	{ }

	explicit top_k(const Comparator& comparator)
		: m_comparator(comparator)
		, m_size(0)
		, m_data()
	{ }

	bool empty() const {
		return m_size == 0;
	}

	size_t size() const {
		return m_size;
	}

	const value_type& operator[](size_t i) const {
		return m_data[i];
	}


	const_iterator begin() const {
		return m_data.begin();
	}

	const_iterator end() const {
		return m_data.begin() + m_size;
	}

	const_iterator cbegin() const {
		return m_data.cbegin();
	}

	const_iterator cend() const {
		return m_data.cbegin() + m_size;
	}


	void push(const value_type& x){
		if(m_size == K && !m_comparator(x, m_data[m_size - 1])){ return; }
		if(m_size == K){
			m_data[m_size - 1] = x;
		}else{
			m_data[m_size++] = x;
		}
		for(size_t i = m_size - 1; i > 0; --i){
			if(m_comparator(m_data[i], m_data[i - 1])){
				std::swap(m_data[i], m_data[i - 1]);
			}
		}
	}

	template <typename... Args>
	void emplace(Args&&... args){
		push(value_type(std::forward<Args>(args)...));
	}

};

template <typename T, size_t K, typename Comparator>
std::ostream& operator<<(std::ostream& os, const top_k<T, K, Comparator>& x){
	os << "(";
	for(size_t i = 0; i < x.size(); ++i){
		if(i != 0){ os << " "; }
		os << x[i];
	}
	os << ")";
	return os;
}

}
