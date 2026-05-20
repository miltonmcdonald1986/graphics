#ifndef GRAPHICS_WINDOW_WINDOW_DESC_HPP
#define GRAPHICS_WINDOW_WINDOW_DESC_HPP

#include <string>

#include "position.hpp"

namespace graphics::window
{

/// @brief Describes the properties used to create a window.
///
/// This struct defines the initial configuration for a window, including
/// dimensions, title text, and whether the window should start in fullscreen
/// mode.
struct WindowDesc
{
    /// @brief Width of the window in pixels.
    int width{};

    /// @brief Height of the window in pixels.
    int height{};

    /// @brief Initial position of the window on the screen.
    Position position{};

    /// @brief Title text displayed in the window's title bar.
    std::string title;

    /// @brief Whether the window should be created in fullscreen mode.
    bool fullscreen{};
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_WINDOW_DESC_HPP