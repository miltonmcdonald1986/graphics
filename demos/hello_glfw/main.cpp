#include <cstdint>
#include <cstdio>
#include <format>
#include <utility>

#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/logging.hpp>
#include <graphics/platform/backend.hpp>
#include <graphics/platform/i_platform.hpp>
#include <graphics/window/window_desc.hpp>

using graphics::core::Expected;
using graphics::core::log_message;
using graphics::core::LogLevel;
using graphics::platform::Backend;
using graphics::platform::create_platform;
using graphics::platform::IPlatform;
using graphics::window::WindowDesc;
using std::format;
using std::uint32_t;
using std::unique_ptr;

auto main() -> int
{
    try
    {
        unique_ptr<IPlatform> platform = create_platform (Backend::GLFW);
        if (!platform)
        {
            log_message (LogLevel::Error, "Failed to initialize GLFW backend");
            return 1;
        }

        WindowDesc desc{.width = 800,
            .height = 600,
            .title = "Window1",
            .fullscreen = false};

        Expected<uint32_t> win1 = platform->create_window (desc);
        if (!win1)
        {
            log_message (LogLevel::Error, "Failed to create window");
            return 1;
        }

        desc.title = "Window2";
        Expected<uint32_t> win2 = platform->create_window (desc);
        if (!win2)
        {
            log_message (LogLevel::Error, "Failed to create window");
            return 1;
        }

        while (platform->has_windows())
        {
            // 1. Poll events
            platform->poll_events();

            // 2. Update input state

            // 3. Run game logic / ECS systems

            for (uint32_t id : platform->get_all_window_ids())
            {
                if (Expected<bool> should_close =
                        platform->window_should_close (id))
                {
                    if (should_close.has_value())
                    {
                        if (should_close.value())
                        {
                            platform->destroy_window (id);
                        }
                    }
                    else
                    {
                        log_message (LogLevel::Warn,
                            format ("It's not clear whether or not window {} "
                                    "should be closed",
                                id));
                    }
                }
                else
                {
                    //// 4. Render (glClear + draw calls)
                    // platform->render_on_window (id);

                    //// 5. Swap buffers
                    platform->swap_buffers (id);
                }
            }
        }

        return 0;
    }
    catch (...)
    {
        puts ("Something went wrong.\n");
        return 1;
    }
}