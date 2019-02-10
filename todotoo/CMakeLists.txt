
# The CMake here is a bit of a mess, sorry
project (todotoo)

# Cross-platform, so we're going to use 'dear imgui' with OpenGL renderer and GLFW platform bindings
find_package(OpenGL REQUIRED)

file(GLOB IMGUI_SOURCES
    ${PROJECT_SOURCE_DIR}/../imgui/*.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/../imgui ${PROJECT_SOURCE_DIR}/../imgui/examples ${PROJECT_SOURCE_DIR}/../findme)

# Add a main function
add_executable(todotoo main.cpp ${IMGUI_SOURCES} ../imgui/examples/imgui_impl_glfw.cpp ../imgui/examples/imgui_impl_opengl2.cpp)

target_link_libraries(todotoo glfw ${OPENGL_gl_LIBRARY})
#target_include_directories(todotoo ${OPENGL_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/../imgui)

