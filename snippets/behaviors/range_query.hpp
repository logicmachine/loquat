struct range_query_behavior {
	using value_type = ;

	value_type identity() const;

	value_type merge(const value_type& a, const value_type& b) const;
};
