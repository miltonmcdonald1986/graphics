#ifndef GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP
#define GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP

#include <cstdint>
#include <string>

namespace graphics::core
{

enum class DiagnosticCategory : std::uint8_t
{
    Core,
    Platform,
    Unknown
};

auto translate_diagnostic_category_to_string (DiagnosticCategory category)
    -> std::string;

} // namespace graphics::core

#endif // GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP