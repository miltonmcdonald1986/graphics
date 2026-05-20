#include <internal/window/window_base.hpp>

namespace graphics::window
{

auto WindowBase::should_close() const -> bool { return m_should_close; }

} // namespace graphics::window