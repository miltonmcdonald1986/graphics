#ifndef GRAPHICS_INTERNAL_WINDOW_WINDOW_BASE_HPP
#define GRAPHICS_INTERNAL_WINDOW_WINDOW_BASE_HPP

#include <graphics/window/i_window.hpp>

namespace graphics::window
{

struct WindowBase : public IWindow
{
  public:
    ~WindowBase () override = default;

    [[nodiscard]] auto should_close() const -> bool final;

  private:
    bool m_should_close{false};
};

} // namespace graphics::window

#endif // GRAPHICS_INTERNAL_WINDOW_WINDOW_BASE_HPP