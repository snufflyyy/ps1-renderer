cmake_minimum_required(VERSION 3.16)
project(PS1-Renderer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

option(SDL_TESTS OFF)
option(SDL_EXAMPLES OFF)
add_subdirectory(external/SDL)

add_subdirectory(external/cimgui)
target_include_directories(cimgui PUBLIC
    external/cimgui
    external/cimgui/generator/output
    external/cimgui/imgui
    external/cimgui/imgui/backends
)

option(CGLM_SHARED "Shared build" ON)
option(CGLM_USE_C99 "" ON)
option(CGLM_USE_TEST "Enable Tests" OFF)
add_subdirectory(external/cglm)

add_subdirectory(external/freetype)

include_directories(
	external/cgltf/include
	external/stb_image/include
)

add_executable(
	${PROJECT_NAME}
	src/main.c
	src/window.c
	src/file.c
	src/shader.c
	src/texture.c
	src/camera.c
	src/player.c
	src/mesh.c
	src/node.c
	src/model.c
	#src/text.c


    external/cimgui/imgui/backends/imgui_impl_sdl3.cpp
    external/cimgui/imgui/backends/imgui_impl_opengl3.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC -DCIMGUI_USE_OPENGL3 -DCIMGUI_USE_SDL3)

target_link_libraries(${PROJECT_NAME} PRIVATE m ${OpenGL_LIBRARIES} glad SDL3::SDL3 cimgui cglm freetype)

target_compile_options(${PROJECT_NAME} PRIVATE
  -Wall -Wextra -Wpedantic
  -Wconversion -Wshadow -Wstrict-prototypes -Wformat=2 -Wnull-dereference
)
