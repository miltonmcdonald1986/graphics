#ifndef GRAPHICS_PLATFORM_I_PLATFORM_HPP
#define GRAPHICS_PLATFORM_I_PLATFORM_HPP

#include <memory>
#include <optional>

#include <graphics/core/status.hpp>
#include <graphics/platform/backend.hpp>

namespace graphics::platform
{

	class IPlatform
	{
	public:
		virtual ~IPlatform() = default;

		virtual auto init()		-> core::Status = 0;
		virtual auto shutdown() -> core::Status = 0;
	};

	auto create_platform(std::optional<Backend> o_backend = std::nullopt) -> std::unique_ptr<IPlatform>;

}

#endif // GRAPHICS_PLATFORM_I_PLATFORM_HPP