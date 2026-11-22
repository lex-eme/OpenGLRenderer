#pragma once

#include <glad/glad.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks-inl.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.inl>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <imgui/imgui.h>

#include <iostream>
#include <string>
#include <ranges>
#include <sstream>
#include <fstream>
#include <format>
#include <memory>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <stack>
#include <deque>
#include <array>
#include <vector>
#include <array>
#include <filesystem>

#include "core/Log.h"

#ifdef BN_PLATFORM_WINDOWS
// TODO: Windows window
#elif BN_PLATFORM_APPLE
#include "platform/macOS/MacWindow.h"
#else
#error Only Windows and MacOS supported
#endif