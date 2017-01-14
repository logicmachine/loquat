#pragma once
#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

namespace loquat {

template <typename Behavior>
class mo_algorithm {

public:
	using behavior_type = Behavior;
	using value_type = typename behavior_type::value_type;
	using result_type = typename behavior_type::result_type;

private:
	std::vector<value_type> m_values;
	behavior_type m_behavior;

public:
	mo_algorithm()
		: m_values()
		, m_behavior()
	{ }

	template <typename Iterator>
	mo_algorithm(
		Iterator first,
		Iterator last,
		const behavior_type& behavior = behavior_type())
		: m_values(first, last)
		, m_behavior(behavior)
	{ }

	template <typename Iterator>
	std::vector<result_type> query(Iterator first, Iterator last){
		using query_tuple_t = std::tuple<size_t, size_t, size_t>;
		size_t left_max = 0;
		for(auto it = first; it != last; ++it){
			left_max = std::max<size_t>(left_max, it->first);
		}
		const size_t b = static_cast<size_t>(ceil(sqrt(left_max + 1)));
		std::vector<std::vector<query_tuple_t>> buckets(b);
		for(auto it = first; it != last; ++it){
			buckets[it->first / b].emplace_back(
				it->second, it->first, it - first);
		}
		std::vector<result_type> results(std::distance(first, last));
		for(auto& bucket : buckets){
			if(bucket.empty()){ continue; }
			std::sort(bucket.begin(), bucket.end());
			result_type r = m_behavior.empty();
			size_t cl = std::get<1>(bucket.front()), cr = cl;
			for(const auto& t : bucket){
				const size_t nl = std::get<1>(t), nr = std::get<0>(t);
				while(cr < nr){ r = m_behavior.add_tail(r, m_values[cr++]); }
				while(cl > nl){ r = m_behavior.add_head(m_values[--cl], r); }
				while(cl < nl){ r = m_behavior.remove(m_values[cl++], r); }
				results[std::get<2>(t)] = r;
			}
		}
		return results;
	}

};

}

