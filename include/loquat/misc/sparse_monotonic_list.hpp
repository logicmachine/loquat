#pragma once
#include <map>
#include <iterator>
#include <functional>

namespace loquat {

template <
	typename Key,
	typename Value,
	typename KeyComp = std::less<Key>,
	typename ValueComp = std::less<Value>>
class sparse_monotonic_list {

public:
	using key_type = Key;
	using value_type = Value;
	using key_comparator_type = KeyComp;
	using value_comparator_type = ValueComp;
	using map_type = std::map<key_type, value_type, key_comparator_type>;
	using const_iterator = typename map_type::const_iterator;
	using const_reverse_iterator = typename map_type::const_reverse_iterator;


private:
	key_comparator_type m_key_comparator;
	value_comparator_type m_value_comparator;
	map_type m_points;


public:
	sparse_monotonic_list()
		: m_key_comparator()
		, m_value_comparator()
		, m_points()
	{ }

	sparse_monotonic_list(
		const key_comparator_type& key_comp,
		const value_comparator_type& value_comp)
		: m_key_comparator(key_comp)
		, m_value_comparator(value_comp)
		, m_points(key_comp)
	{ }


	const_iterator begin() const {
		return m_points.begin();
	}

	const_iterator end() const {
		return m_points.end();
	}

	const_iterator cbegin() const {
		return m_points.cbegin();
	}

	const_iterator cend() const {
		return m_points.cend();
	}

	const_reverse_iterator rbegin() const {
		return m_points.rbegin();
	}

	const_reverse_iterator rend() const {
		return m_points.rend();
	}

	const_reverse_iterator crbegin() const {
		return m_points.crbegin();
	}

	const_reverse_iterator crend() const {
		return m_points.crend();
	}


	const_iterator lower_bound(const key_type& key) const {
		return m_points.lower_bound(key);
	}

	const_iterator upper_bound(const key_type& key) const {
		return m_points.upper_bound(key);
	}


	bool insert(const key_type& key, const value_type& value){
		const auto& kcomp = m_key_comparator;
		const auto& vcomp = m_value_comparator;
		auto it = m_points.lower_bound(key);
		if(it != m_points.end() && !kcomp(it->first, key) && !kcomp(key, it->first)){
			if(!vcomp(it->second, value)){ return false; }
			it = m_points.erase(it);
		}else if(it != m_points.begin()){
			if(!vcomp(std::prev(it)->second, value)){ return false; }
		}
		it = std::next(m_points.emplace_hint(it, key, value));
		while(it != m_points.end() && !vcomp(value, it->second)){
			it = m_points.erase(it);
		}
		return true;
	}

};

}
