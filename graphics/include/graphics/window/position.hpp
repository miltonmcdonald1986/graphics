#ifndef GRAPHICS_WINDOW_POSITION_HPP
#define GRAPHICS_WINDOW_POSITION_HPP

namespace graphics::window
{

/// @brief Represents a 2D position in pixel coordinates.
///
/// The position is expressed as an (x, y) pair measured in pixels. The
/// interpretation of these coordinates depends on the context in which
/// they are used—for example, specifying a window’s placement, defining
/// a cursor location, or representing a point within a rendering surface.
///
/// No assumptions are made about coordinate origin or orientation; those
/// are defined by the subsystem consuming the value.
struct Position
{
    int x{}; ///< Horizontal coordinate in pixels.
    int y{}; ///< Vertical coordinate in pixels.
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_POSITION_HPP