cmake_minimum_required(VERSION 3.16)
project(SeaChess)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

option(SDL_TESTS OFF)
option(SDL_EXAMPLES OFF)
add_subdirectory(external/SDL)

option(CGLM_SHARED "Shared build" ON)
option(CGLM_USE_C99 "" ON)
option(CGLM_USE_TEST "Enable Tests" OFF) # for make check - make test
add_subdirectory(external/cglm)

add_subdirectory(external/freetype)

add_executable(
	${PROJECT_NAME}
	src/main.c
	src/window.c
	src/input.c
	src/file.c
	src/shader.c
	src/texture.c
	src/camera.c
	src/quad.c
	src/text.c
	src/box.c
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenGL_LIBRARIES} glad SDL3::SDL3 cglm freetype)
