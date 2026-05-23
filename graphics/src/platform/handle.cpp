#include <internal/platform/handle.hpp>

#include <format>

#include <graphics/core/diagnostic.hpp>
#include <graphics/core/diagnostic_category.hpp>
#include <graphics/core/log_level.hpp>

using graphics::core::log_diagnostic;
using graphics::core::DiagnosticCategory::Platform;
using graphics::core::LogLevel::Trace;
using std::format;

namespace graphics::platform
{

constexpr uint32_t INDEX_BITS = 16;
constexpr uint32_t INDEX_MASK = (1U << INDEX_BITS) - 1U;

auto pack_handle (uint32_t index, uint32_t gen) -> uint32_t
{
    uint32_t packed = (gen << INDEX_BITS) | index;
    log_diagnostic (Platform,
        format ("packed (index,gen)=({},{}) to {}", index, gen, packed), Trace);

    return packed;
}

auto unpack_handle (uint32_t handle) -> Handle
{
    Handle unpacked{.id = handle & INDEX_MASK, .gen = handle >> INDEX_BITS};
    log_diagnostic (Platform,
        format ("unpacked {} to (index,gen)=({},{})", handle, unpacked.id,
            unpacked.gen),
        Trace);

    return unpacked;
}

} // namespace graphics::platform