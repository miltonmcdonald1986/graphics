#include <cstdio>
#include <utility>

#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/logging.hpp>
#include <graphics/platform/backend.hpp>
#include <graphics/platform/i_platform.hpp>

using graphics::core::Expected;
using graphics::core::log_message;
using graphics::core::LogLevel;
using graphics::platform::Backend;
using graphics::platform::create_platform;

auto main() -> int
{
    try
    {
        auto expected_platform = create_platform (Backend::SDL2);
        if (!expected_platform)
        {
            log_message (LogLevel::Error,
                "Failed to create platform with the desired backend.");
            return 1;
        }

        const auto platform = std::move (*expected_platform);
        return 0;
    }
    catch (...)
    {
        puts ("Something went wrong.\n");
        return 1;
    }
}