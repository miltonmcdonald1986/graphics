#ifndef GRAPHICS_PLATFORM_I_PLATFORM_HPP
#define GRAPHICS_PLATFORM_I_PLATFORM_HPP

#include <memory>
#include <optional>
#include <vector>

#include <graphics/core/status.hpp>
#include <graphics/platform/backend.hpp>
#include <graphics/window/i_window.hpp>
#include <graphics/window/window_desc.hpp>

namespace graphics::platform
{

/// \brief Interface for platform backends.
///
/// Defines the minimal API required for platform initialization,
/// shutdown, and window/event management.
struct IPlatform
{
  public:
    virtual ~IPlatform();

    auto create_window (const window::WindowDesc& desc)
        -> core::Expected<std::uint32_t>;
    auto destroy_window (std::uint32_t id) -> void;
    auto get_all_window_ids() const -> std::vector<std::uint32_t>;
    auto has_windows() const -> bool;
    auto poll_events() const -> void;
    auto swap_buffers (std::uint32_t window) const -> void;
    auto window_should_close (std::uint32_t window) const
        -> core::Expected<bool>;

  protected:
    IPlatform();
    virtual auto create_backend_window (const window::WindowDesc&) const
        -> std::unique_ptr<window::IWindow> = 0;
    virtual auto destroy_backend_window (window::IWindow* window) const
        -> void = 0;
    virtual auto poll_backend_events() const -> void = 0;
    virtual auto swap_backend_buffers (window::IWindow* window) const
        -> void = 0;
    virtual auto window_backend_should_close (window::IWindow* window) const
        -> core::Expected<bool> = 0;

  private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

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
    -> std::unique_ptr<IPlatform>;

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_I_PLATFORM_HPP