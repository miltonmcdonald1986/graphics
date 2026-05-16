add_library(graphics)

target_compile_features(graphics PUBLIC cxx_std_23)

find_package(glfw3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(graphics 
    PRIVATE 
        glfw 
        spdlog::spdlog
)

file(GLOB_RECURSE GRAPHICS_PRIVATE_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

target_sources(graphics
    PRIVATE
        ${GRAPHICS_PRIVATE_SOURCES}
)

target_include_directories(graphics
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# -----------------------------
# Add tests for the graphics lib
# -----------------------------
if (graphics_ENABLE_TESTING)
    add_subdirectory(tests)
endif()