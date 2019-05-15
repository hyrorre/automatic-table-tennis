cmake_minimum_required(VERSION 3.13)
project(AutomaticTableTennis)

set(CMAKE_CXX_STANDARD 98)

add_executable(AutomaticTableTennis main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(AutomaticTableTennis ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )