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
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_I_WINDOW_HPP