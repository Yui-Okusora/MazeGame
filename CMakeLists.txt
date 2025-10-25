cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MazeGame")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad)

add_executable (MazeGame "src/MazeGame.cpp")

target_include_directories(MazeGame PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(MazeGame
    PRIVATE
        glad
        glfw
)