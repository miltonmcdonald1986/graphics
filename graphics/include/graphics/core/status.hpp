#ifndef GRAPHICS_CORE_STATUS_HPP
#define GRAPHICS_CORE_STATUS_HPP

#include <graphics/core/expected.hpp>

namespace graphics::core
{

/// \brief Convenience alias for operations that return only success or failure.
///
/// `Status` represents an operation that does not produce a value on success
/// but may fail with a `Diagnostic`. It is equivalent to `Expected<void>`
/// and is used for functions where the only meaningful outcome is whether
/// the operation succeeded.
///
/// This is the standard return type for initialization, shutdown, and other
/// side-effect-driven engine operations.
///
/// \see Expected
/// \see Unexpected
using Status = Expected<void>;

} // namespace graphics::core

#endif // GRAPHICS_CORE_STATUS_HPP