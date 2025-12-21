cmake_minimum_required (VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project ("MazeGame")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glad)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/gl2d)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_truetype)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/miniaudio-master)

add_executable (MazeGame "src/MazeGame.cpp" "include/Application/Application.cpp" "include/Window/Window.cpp" "include/Processor/Processor.cpp" "include/State/State.cpp" "include/Shape/Rect.cpp" "include/Shape/Maze.cpp" "include/GameMechanics/GameplayState.cpp" "include/CRC32_64/CRC32_64.cpp" "include/AudioEngine/AudioEngine.cpp" "include/AudioEngine/Sound.cpp" "include/SaveFile/SaveManager.cpp")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else()
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
	target_compile_options(MazeGame PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/O2>)
endif()

target_compile_features(MazeGame PRIVATE cxx_std_23)
target_include_directories(MazeGame 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_compile_options(MazeGame PRIVATE -O3)

target_compile_definitions(MazeGame PUBLIC RESOURCES_PATH="${CMAKE_SOURCE_DIR}/resources/")

target_link_libraries(MazeGame
    PRIVATE
        glad
        glfw
        gl2d
        stb_image
        stb_truetype
		miniaudio
)

add_custom_command(TARGET MazeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:MazeGame>/resources"
)