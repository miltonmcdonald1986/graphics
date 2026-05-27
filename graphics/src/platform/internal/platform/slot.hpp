#ifndef GRAPHICS_INTERNAL_PLATFORM_SLOT_HPP
#define GRAPHICS_INTERNAL_PLATFORM_SLOT_HPP

namespace graphics::platform
{

struct Slot
{
    std::unique_ptr<window::IWindow> window{ nullptr };
    std::uint32_t generation = 0;
};

} // namespace graphics::platform

#endif // #define GRAPHICS_INTERNAL_PLATFORM_SLOT_HPP