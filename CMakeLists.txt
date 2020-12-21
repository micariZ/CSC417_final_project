cmake_minimum_required(VERSION 3.2)

project(final-project)

find_package(OpenGL)

#Important CMAKE stuff
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#setup libigl properly
# libigl
option(LIBIGL_WITH_OPENGL               "Use OpenGL"            ON)
option(LIBIGL_WITH_OPENGL_GLFW          "Use GLFW"              ON)
option(LIBIGL_WITH_OPENGL_GLFW_IMGUI    "Use ImGui"             ON)

find_package(LIBIGL REQUIRED QUIET)

#include source code
file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

#include directories
include_directories(${PROJECT_SOURCE_DIR}/includes)

#build executable 
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY} ${OPENGL_LIBRARIES} igl::core igl::opengl igl::opengl_glfw igl::opengl_glfw_imgui)


