#ifndef GRAPHICS_WINDOW_I_WINDOW_HPP
#define GRAPHICS_WINDOW_I_WINDOW_HPP

#include <memory>

#include <graphics/core/expected.hpp>

namespace graphics::window
{

struct IWindow
{
  public:
    virtual ~IWindow() = default;

    [[nodiscard]] virtual auto should_close() const -> bool = 0;
};

} // namespace graphics::window

#endif // GRAPHICS_WINDOW_I_WINDOW_HPP