cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("MazeGame")

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glm)

add_executable (MazeGame "src/MazeGame.cpp" "src/Application.cpp" "src/Window.cpp")

target_compile_features(MazeGame PRIVATE cxx_std_23)
target_include_directories(MazeGame 
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/libs/glm
)

target_link_libraries(MazeGame
    PRIVATE
        glad
        glfw
)