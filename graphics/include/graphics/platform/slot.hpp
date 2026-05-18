#ifndef GRAPHICS_PLATFORM_SLOT_HPP
#define GRAPHICS_PLATFORM_SLOT_HPP

#include <cstdint>
#include <memory>

#include <graphics/window/i_window.hpp>

namespace graphics::platform
{

struct Slot
{
    std::unique_ptr<window::IWindow> window{nullptr};
    std::uint32_t generation = 0;
};

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_SLOT_HPP