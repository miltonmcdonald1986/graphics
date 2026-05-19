#ifndef GRAPHICS_WINDOW_I_WINDOW_HPP
#define GRAPHICS_WINDOW_I_WINDOW_HPP

#include <memory>

#include <graphics/core/expected.hpp>

namespace graphics::window
{

/// @brief Interface representing a platform window.
///
/// This abstract base class defines the minimal API required for window
/// backends. Implementations provide backend‑specific behavior for querying
/// window state, handling close requests, and integrating with the platform.
struct IWindow
{
  public:
    virtual ~IWindow() = default;

    /// @brief Indicates whether the window has received a request to close.
    /// @return true if the window should close, false otherwise.
    ///
    /// This reports backend‑specific close requests (e.g., user clicking the
    /// close button, OS shutdown events, or platform‑specific window events).
    /// Implementations must not block; they should simply return the current
    /// close state.
    [[nodiscard]] virtual auto should_close() const -> bool = 0;
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_I_WINDOW_HPP