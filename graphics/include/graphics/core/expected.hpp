#ifndef GRAPHICS_CORE_EXPECTED_HPP
#define GRAPHICS_CORE_EXPECTED_HPP

#include <expected>

#include "diagnostic.hpp"

namespace graphics::core
{

/*---------*/
/* Aliases */
/*---------*/

template <typename T> using Expected = std::expected<T, Diagnostic>;

using Unexpected = std::unexpected<Diagnostic>;

/*-----------*/
/* Functions */
/*-----------*/

auto create_unexpected (const Diagnostic& diagnostic) -> Unexpected;

auto create_unexpected (DiagnosticCategory category,
    const std::string& message,
    LogLevel level = LogLevel::Error,
    std::source_location location = std::source_location::current())
    -> Unexpected;

} // namespace graphics::core

#endif // GRAPHICS_CORE_EXPECTED_HPP