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
    virtual ~IPlatform() = default;

    /// \brief Create a new window using the backend implementation.
    /// \param desc Window creation parameters.
    /// \return Window ID on success, or a Diagnostic on failure.
    virtual auto create_window (const window::WindowDesc& desc)
        -> core::Expected<std::uint32_t> = 0;

    /// \brief Destroy a previously created window.
    /// \param win_id Window ID to destroy.
    virtual auto destroy_window (std::uint32_t win_id) -> void = 0;

    /// \brief Retrieve all currently active window IDs.
    /// \return Vector of valid window IDs.
    [[nodiscard]] virtual auto get_all_window_ids() const
        -> std::vector<std::uint32_t> = 0;

    /// \brief Check whether any windows are currently active.
    /// \return True if at least one window exists.
    [[nodiscard]] virtual auto has_windows() const -> bool = 0;

    /// \brief Makes the OpenGL context for the given window ID current.
    /// \param win_id Window ID whose context should be made current.
    /// \return Status indicating success or failure.
    virtual auto make_context_current (std::uint32_t win_id) const
        -> core::Status = 0;

    /// \brief Poll backend-specific events (input, window messages, etc.).
    virtual auto poll_events() -> void = 0;

    /// \brief Set the close request state for a window.
    /// \param win_id Window ID.
    /// \param should_close True to request the window to close, false to clear
    ///                     the request.
    virtual auto set_window_should_close (std::uint32_t win_id,
        bool should_close) -> void = 0;

    /// \brief Swap buffers for the specified window.
    /// \param win_id Window ID.
    virtual auto swap_buffers (std::uint32_t win_id) const -> void = 0;

    /// \brief Query whether the specified window should close.
    /// \param win_id Window ID.
    /// \return True if the window requested closure.
    [[nodiscard]] virtual auto window_should_close (std::uint32_t win_id) const
        -> core::Expected<bool> = 0;
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