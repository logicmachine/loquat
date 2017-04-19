struct lazy_range_query_behavior {
	using value_type    = ;
	using modifier_type = ;

	value_type identity_value() const;

	modifier_type identity_modifier() const;

	std::pair<modifier_type, modifier_type>
	split_modifier(
		const modifier_type& m,
		size_t k) const;

	modifier_type merge_modifier(
		const modifier_type& a,
		const modifier_type& b) const;

	value_type merge_value(
		const value_type& a,
		const value_type& b) const;

	value_type modify(
		size_t n,
		const value_type& v,
		const modifier_type& m) const;
};
