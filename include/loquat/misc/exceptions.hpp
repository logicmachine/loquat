#pragma once
#include <stdexcept>

namespace loquat {

class no_solution_error : public std::runtime_error {

public:
	explicit no_solution_error(const char *what)
		: std::runtime_error(what)
	{ }

	explicit no_solution_error(const std::string& what)
		: std::runtime_error(what)
	{ }

};

}

