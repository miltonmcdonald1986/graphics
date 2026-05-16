#ifndef GRAPHICS_PLATFORM_I_PLATFORM_HPP
#define GRAPHICS_PLATFORM_I_PLATFORM_HPP

#include <memory>
#include <optional>

#include <graphics/core/status.hpp>
#include <graphics/platform/backend.hpp>

namespace graphics::platform
{

/// \brief Interface for platform backends.
///
/// Defines the minimal API required for platform initialization,
/// shutdown, and window/event management.
struct IPlatform
{
  public:
    virtual ~IPlatform() = default;
};

/// \brief Owning pointer to an IPlatform implementation.
///
/// PlatformPtr is the standard handle type returned by platform
/// initialization functions. It owns the underlying platform backend
/// instance via std::unique_ptr.
using PlatformPtr = std::unique_ptr<IPlatform>;

/// \brief Expected result type for platform creation.
///
/// ExpectedPlatformPtr represents either a valid PlatformPtr on success
/// or a Diagnostic describing the failure.
using ExpectedPlatformPtr = core::Expected<PlatformPtr>;

/// \brief Creates a platform backend instance.
///
/// Attempts to construct an IPlatform implementation using the specified
/// backend. If no backend is provided, the engine selects a default
/// backend. On success, returns a PlatformPtr. On failure, returns an
/// Unexpected containing a Diagnostic.
///
/// \param o_backend Optional backend selection. If std::nullopt is used,
///                  a default backend is chosen.
/// \return ExpectedPlatformPtr containing either a valid PlatformPtr or a
///         Diagnostic describing the failure.
auto create_platform (std::optional<Backend> o_backend = std::nullopt)
    -> ExpectedPlatformPtr;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_I_PLATFORM_HPP