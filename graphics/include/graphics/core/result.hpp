#ifndef GRAPHICS_CORE_RESULT_HPP
#define GRAPHICS_CORE_RESULT_HPP

#include <expected>

#include "diagnostic.hpp"

namespace graphics::core
{

	template<typename T>
	using Result = std::expected<T, Diagnostic>;

}

#endif // GRAPHICS_CORE_RESULT_HPP