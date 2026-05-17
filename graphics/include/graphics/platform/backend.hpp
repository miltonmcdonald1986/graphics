#ifndef GRAPHICS_PLATFORM_BACKEND_HPP
#define GRAPHICS_PLATFORM_BACKEND_HPP

namespace graphics::platform
{

/// \brief Identifies the platform backend to use.
///
/// Backend selects which windowing and event system implementation the
/// engine should create. Additional backends may be added in the future.
///
/// - GLFW: Use the GLFW-based platform backend.
enum class Backend : std::uint8_t
{
    GLFW, ///< GLFW platform backend.
    SDL2  ///< SDL2 platform backend.
};

} // namespace graphics::platform

#endif // GRAPHICS_PLATFORM_BACKEND_HPP