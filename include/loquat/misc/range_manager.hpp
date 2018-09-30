#pragma once
#include <map>
#include <iterator>
#include <cassert>

namespace loquat {

template <typename T>
class range_manager {

public:
	using coord_type = T;
	using pair_type = std::pair<coord_type, coord_type>;

private:
	std::map<coord_type, coord_type> m_ranges;

public:
	range_manager()
		: m_ranges()
	{ }

	template <typename InsertFunc, typename EraseFunc>
	void add(
		coord_type left,
		coord_type right,
		InsertFunc insert_callback,
		EraseFunc  erase_callback)
	{
		assert(left <= right);
		if(left == right){ return; }
		auto it = m_ranges.upper_bound(left);
		if(it != m_ranges.begin()){
			--it;
			if(it->first <= left && right <= it->second){ return; }
			if(left <= it->second){
				left = it->first;
				erase_callback(it->first, it->second);
				it = m_ranges.erase(it);
			}else{
				++it;
			}
		}
		while(it != m_ranges.end() && it->second <= right){
			erase_callback(it->first, it->second);
			it = m_ranges.erase(it);
		}
		if(it != m_ranges.end() && it->first <= right){
			right = it->second;
			erase_callback(it->first, it->second);
			it = m_ranges.erase(it);
		}
		insert_callback(left, right);
		m_ranges.emplace_hint(it, left, right);
	}

	void add(coord_type left, coord_type right){
		auto f = [](coord_type, coord_type){};
		add(left, right, f, f);
	}

	template <typename InsertFunc, typename EraseFunc>
	void remove(
		coord_type left,
		coord_type right,
		InsertFunc insert_callback,
		EraseFunc  erase_callback)
	{
		assert(left <= right);
		if(left == right){ return; }
		auto it = m_ranges.lower_bound(left);
		if(it != m_ranges.begin()){
			--it;
			if(it->first < left && right < it->second){
				const auto s = it->first, t = it->second;
				erase_callback(it->first, it->second);
				it = m_ranges.erase(it);
				insert_callback(s, left);
				it = std::next(m_ranges.emplace_hint(it, s, left));
				insert_callback(right, t);
				it = std::next(m_ranges.emplace_hint(it, right, t));
				return;
			}else if(left < it->second){
				const auto t = it->first;
				erase_callback(it->first, it->second);
				it = m_ranges.erase(it);
				insert_callback(t, left);
				it = std::next(m_ranges.emplace_hint(it, t, left));
			}else{
				++it;
			}
		}
		while(it != m_ranges.end() && it->second <= right){
			erase_callback(it->first, it->second);
			it = m_ranges.erase(it);
		}
		if(it != m_ranges.end() && it->first < right){
			const auto t = it->second;
			erase_callback(it->first, it->second);
			it = m_ranges.erase(it);
			insert_callback(right, t);
			m_ranges.emplace_hint(it, right, t);
		}
	}

	void remove(coord_type left, coord_type right){
		auto f = [](coord_type, coord_type){};
		remove(left, right, f, f);
	}

};

}
