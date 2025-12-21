#pragma once

//Third-party libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <gl2d/gl2d.h>
#include <stb_image/stb_image.h>
#include <stb_truetype/stb_truetype.h>


//Self-write libraries
#include <DoubleBuffer/DoubleBuffer.hpp>
#include <CircularBuffer/CircularBuffer.hpp>
#include <CRC32_64/CRC32_64.hpp>
#include <Arena/Arena.hpp>
#include <ViewportScale/ViewportScale.hpp>
#include <AudioEngine/AudioEngine.hpp>
#include <GameMechanics/MazeData.hpp>


// STL Libraries
#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <array>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include <concepts>
#include <type_traits>
#include <queue>
#include <optional>
#include <stdint.h>

#define FONT_PATH "resources\\fonts\\"

#define ASSETS_PATH "resources\\assets\\"
#define MAPS_PATH "resources\\maps\\"
#define SFX_PATH "resources\\sfx\\"

#define SAVE_DIR "saves\\"