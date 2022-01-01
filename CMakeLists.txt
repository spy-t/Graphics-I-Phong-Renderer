cmake_minimum_required(VERSION 3.10)
project(graphics_project VERSION 1.0 LANGUAGES CXX)

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
)

set(GLAD_LIBRARIES dl)

set(PROJECT_SOURCES src/main.cpp src/Sphere.cpp)
set(THIRD_PARTY_SOURCES third_party/glad/src/glad.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
	${THIRD_PARTY_SOURCES}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_NAME}
	PUBLIC
	$<$<CONFIG:Debug>:-DDEBUG>)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
