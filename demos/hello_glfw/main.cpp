#include <cstdint>
#include <cstdio>
#include <format>
#include <memory>

#include <graphics/core/expected.hpp>
#include <graphics/core/log_level.hpp>
#include <graphics/core/logging.hpp>
#include <graphics/platform/backend.hpp>
#include <graphics/platform/i_platform.hpp>
#include <graphics/window/position.hpp>
#include <graphics/window/window_desc.hpp>

using graphics::core::Expected;
using graphics::core::log_message;
using graphics::core::LogLevel;
using graphics::platform::Backend;
using graphics::platform::create_platform;
using graphics::platform::IPlatform;
using graphics::window::Position;
using graphics::window::WindowDesc;
using std::format;
using std::uint32_t;
using std::unique_ptr;

auto main() -> int
{
    try
    {
        unique_ptr<IPlatform> platform = create_platform (Backend::SDL2);
        if (!platform)
        {
            log_message (LogLevel::Error, "Failed to initialize GLFW backend");
            return 1;
        }

        const int posX{ 100 };
        const int posY{ 100 };
        const Position position{ .x = posX, .y = posY };
        const int width = 800;
        const int height = 600;
        WindowDesc desc{ .width = width,
            .height = height,
            .position = position,
            .title = "Window1",
            .fullscreen = false };

        const Expected<uint32_t> win1 = platform->create_window (desc);
        if (!win1)
        {
            log_message (LogLevel::Error, "Failed to create window");
            return 1;
        }

        const int delta_pos = 50;
        desc.position.x += delta_pos;
        desc.position.y += delta_pos;
        desc.title = "Window2";
        const Expected<uint32_t> win2 = platform->create_window (desc);
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

            for (uint32_t win_id : platform->get_all_window_ids())
            {
                if (Expected<bool> should_close =
                        platform->window_should_close (win_id))
                {
                    if (should_close.has_value())
                    {
                        if (should_close.value())
                        {
                            platform->destroy_window (win_id);
                        }
                    }
                    else
                    {
                        log_message (LogLevel::Warn,
                            format ("It's not clear whether or not window {} "
                                    "should be closed",
                                win_id));
                    }
                }
                else
                {
                    //// 4. Render (glClear + draw calls)
                    // platform->render_on_window (id);

                    //// 5. Swap buffers
                    platform->swap_buffers (win_id);
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