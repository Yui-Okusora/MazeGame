#pragma once

#include <Core.hpp>

struct Mesh
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei indexCount = 0;
};

enum class RenderPass {
    Forward,
    Shadow,
    Depth,
    GBuffer
};

struct Material {
    // Shaders for different passes
    std::unordered_map<RenderPass, GLuint> shaders;

    // Textures for this surface
    GLuint albedoTex = 0;
    GLuint normalTex = 0;
    GLuint roughnessTex = 0;
    GLuint metallicTex = 0;

    // Uniform parameters
    glm::vec4 baseColor = glm::vec4(1.0f);
    float roughness = 0.5f;
    float metallic = 0.0f;
};

struct RenderItem
{
    Mesh mesh;
    std::shared_ptr<Material> material;
    std::vector<glm::mat4> modelMatrices;
};

struct CameraData
{
    glm::mat4 view;
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 forward;

};

struct RenderData
{
    CameraData camera;
    std::vector<RenderItem> batches;
    glm::vec4 clearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
};