#ifndef GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP
#define GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP

#include <cstdint>
#include <string>

namespace graphics::core
{

/// \brief High-level category describing the subsystem that produced a
/// diagnostic.
///
/// `DiagnosticCategory` classifies diagnostics according to the engine
/// subsystem that emitted them. This allows log filtering, routing, and
/// structured reporting across different parts of the engine.
///
/// The categories are intentionally coarse-grained and represent the major
/// architectural layers of the engine.
///
/// - `Core` - Diagnostics originating from core engine logic.
/// - `Platform` - Diagnostics from platform backends.
/// - `Window` - Diagnostics from window management subsystems.
/// - `Unknown` - Diagnostics that cannot be attributed to a specific subsystem.
///
/// \see Diagnostic
/// \see LogLevel
enum class DiagnosticCategory : std::uint8_t
{
    Core,     ///< Core engine subsystem.
    Platform, ///< Platform subsystem.
    Window,   ///< Window management subsystem.
    Unknown,  ///< Unclassified or unspecified subsystem.
    Count     ///< Sentinel value representing the number of categories.
};

} // namespace graphics::core

#endif // GRAPHICS_CORE_DIAGNOSTIC_CATEGORY_HPP