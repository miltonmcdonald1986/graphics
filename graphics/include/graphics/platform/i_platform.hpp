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
    /// \brief Interface for platform backends.
    ///
    /// Defines the minimal API required for platform initialization,
    /// shutdown, and window/event management.
    virtual ~IPlatform();

    /// \brief Create a new window using the backend implementation.
    /// \param desc Window creation parameters.
    /// \return Window ID on success, or a Diagnostic on failure.
    auto create_window (const window::WindowDesc& desc)
        -> core::Expected<std::uint32_t>;

    /// \brief Destroy a previously created window.
    /// \param id Window ID to destroy.
    auto destroy_window (std::uint32_t id) -> void;

    /// \brief Retrieve all currently active window IDs.
    /// \return Vector of valid window IDs.
    auto get_all_window_ids() const -> std::vector<std::uint32_t>;
    
    /// \brief Check whether any windows are currently active.
    /// \return True if at least one window exists.
    auto has_windows() const -> bool;

    /// \brief Poll backend-specific events (input, window messages, etc.).
    auto poll_events() const -> void;

    /// \brief Swap buffers for the specified window.
    /// \param window Window ID.
    auto swap_buffers (std::uint32_t window) const -> void;

    /// \brief Query whether the specified window should close.
    /// \param window Window ID.
    /// \return True if the window requested closure.
    auto window_should_close (std::uint32_t window) const
        -> core::Expected<bool>;

  protected:
    /// \brief Construct the platform interface.
    ///
    /// Initializes internal state but does not create backend resources.
    IPlatform();

    /// \brief Backend-specific window destruction.
    virtual auto create_backend_window (const window::WindowDesc&) const
        -> std::unique_ptr<window::IWindow> = 0;

    /// \brief Backend-specific window destruction.
    virtual auto destroy_backend_window (window::IWindow* window) const
        -> void = 0;
    
    /// \brief Backend-specific event polling.
    virtual auto poll_backend_events() const -> void = 0;

    /// \brief Backend-specific buffer swap.
    virtual auto swap_backend_buffers (window::IWindow* window) const
        -> void = 0;
    
    /// \brief Backend-specific close-request query.
    virtual auto window_backend_should_close (window::IWindow* window) const
        -> core::Expected<bool> = 0;

  private:
    /// \brief Internal implementation details (PIMPL).
    struct Impl;
    
    /// \brief Pointer to internal implementation.
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