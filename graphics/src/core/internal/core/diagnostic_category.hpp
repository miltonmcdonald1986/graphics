#ifndef GRAPHICS_INTERNAL_CORE_DIAGNOSTIC_CATEGORY_HPP
#define GRAPHICS_INTERNAL_CORE_DIAGNOSTIC_CATEGORY_HPP

#include <string>

#include <graphics/core/diagnostic_category.hpp>

namespace graphics::core
{

auto translate_diagnostic_category_to_string (DiagnosticCategory category)
    -> std::string;

} // namespace graphics::core

#endif // GRAPHICS_INTERNAL_CORE_DIAGNOSTIC_CATEGORY_HPP