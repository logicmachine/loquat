struct mo_algorithm_behavior {
	using value_type  = ;
	using state_type  = ;
	using result_type = ;

	state_type empty() const;

	state_type add_head(const value_type& v, state_type s) const;

	state_type add_tail(state_type s, const value_type& v) const;

	state_type remove(const value_type& v, state_type s) const;

	result_type to_result(const state_type& s) const;
};
