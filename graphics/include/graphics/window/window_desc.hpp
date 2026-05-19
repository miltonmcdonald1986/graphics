#ifndef GRAPHICS_WINDOW_WINDOW_DESC_HPP
#define GRAPHICS_WINDOW_WINDOW_DESC_HPP

#include <string>

namespace graphics::window
{

struct WindowDesc
{
    int width{};
    int height{};
    std::string title;
    bool fullscreen{};
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_WINDOW_DESC_HPP