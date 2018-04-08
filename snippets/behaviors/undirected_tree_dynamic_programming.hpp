struct behavior {
	using state_type = ;
	using edge_type = ;

	state_type initial(loquat::vertex_t) const {
	}

	state_type merge(
		state_type y,
		const state_type& x,
		loquat::vertex_t u,
		const edge_type& e) const
	{
	}

	state_type purge(
		const state_type& y,
		const state_type& x,
		loquat::vertex_t u,
		const edge_type& e) const
	{
	}
};
