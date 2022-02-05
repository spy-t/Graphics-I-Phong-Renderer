cmake_minimum_required(VERSION 3.10)
project(sphere VERSION 1.0 LANGUAGES CXX)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(third_party/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(third_party/assimp)

include_directories(
	third_party/glfw/include
	third_party/glad/include
	third_party/glm
	third_party/assimp/include

	src/
)

set(GLAD_LIBRARIES dl)

set(PROJECT_SOURCES
	src/Main.cpp
	src/Sphere.cpp
	src/ogl/Camera.cpp
	src/ogl/Core.cpp
	src/ogl/ElementBuffer.cpp
	src/ogl/Mesh.cpp
	src/ogl/Model.cpp
	src/ogl/Renderer.cpp
	src/ogl/Shader.cpp
	src/ogl/Text.cpp
	src/ogl/Texture.cpp
	src/ogl/VertexArray.cpp
	src/ogl/VertexBuffer.cpp
	)

set(THIRD_PARTY_SOURCES
	third_party/glad/src/glad.cpp
	src/third_party/cute_font.cpp
	src/third_party/stb_image.cpp
	)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
	${THIRD_PARTY_SOURCES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME}
	PUBLIC
	$<$<CONFIG:Debug>:-DDEBUG>)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw assimp ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
