#ifndef GRAPHICS_INTERNAL_PLATFORM_HANDLE_HPP
#define GRAPHICS_INTERNAL_PLATFORM_HANDLE_HPP

#include <cstdint>

namespace graphics::platform
{

struct Handle
{
    std::uint32_t id{};
    std::uint32_t gen{};
};

auto pack_handle (std::uint32_t index, std::uint32_t gen) -> std::uint32_t;
auto unpack_handle (std::uint32_t handle) -> Handle;

} // namespace graphics::platform

#endif // GRAPHICS_INTERNAL_PLATFORM_HANDLE_HPP