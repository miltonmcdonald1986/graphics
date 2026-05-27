add_executable(hello_glfw main.cpp)

find_package(imgui CONFIG REQUIRED)

target_link_libraries(hello_glfw
    PRIVATE 
        graphics
        imgui::imgui
)