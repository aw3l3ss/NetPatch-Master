cmake_minimum_required(VERSION 3.10)
project(NetPatch-Master.o)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB IMGUI_SOURCES
    "${CMAKE_SOURCE_DIR}/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${CMAKE_SOURCE_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)

add_executable(NetPatch-Master.o main.cpp sqlite3/sqlite3.c db.cpp apt_package.cpp cve.cpp python_api.cpp server.cpp glad/src/glad.c ${IMGUI_SOURCES})

target_include_directories(NetPatch-Master.o PRIVATE
    ${CMAKE_SOURCE_DIR}/imgui
    ${CMAKE_SOURCE_DIR}/imgui/backends
    ${CMAKE_SOURCE_DIR}/glad/include
    ${CMAKE_SOURCE_DIR}/glfw/include
    ${CMAKE_SOURCE_DIR}/libssh/include
)

find_package(glfw3 3.3 REQUIRED)
target_link_libraries(NetPatch-Master.o PRIVATE glfw GLU)

target_link_libraries(NetPatch-Master.o PRIVATE ssh)