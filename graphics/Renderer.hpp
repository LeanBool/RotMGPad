//
// Created by alex on 7/10/26.
//

#ifndef GAMEPADREALM_RENDERER_HPP
#define GAMEPADREALM_RENDERER_HPP

#include "../globals.hpp"
#include "../controls/ControllerManager.hpp"
#include "../thirdparty/include/glad/glad.h"  // MUST come before glfw3.h

#include <glm/glm.hpp>
#include <array>
#include <string>
#include <vector>

struct Character {
    GLuint textureID = 0;
    glm::ivec2 size{0, 0};
    glm::ivec2 bearing{0, 0};
    GLuint advance = 0;
};

// Describes a repeating slot grid (inventory/pickup/vault/potion racks) so the
// highlight-drawing logic doesn't need one near-identical branch per mode.
struct GridHighlight {
    float originX, originY;
    float slotWidth, slotHeight;
    float strideX, strideY; // usually == slotWidth/slotHeight, but potion rack spaces slots wider than they are drawn
    int columns;
};

class Renderer {
public:
    explicit Renderer(ControllerManager& controllerManager) : controllerManager_(controllerManager) {}
    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    int SDL_main();

private:
    [[nodiscard]] float measureTextWidth(const std::string& text, float scale) const;

    void drawFilledPolygon(const std::vector<glm::vec2>& points, const glm::vec3& color,
                        float alpha, const glm::mat4& projection) const;
    void drawPolygonOutline(const std::vector<glm::vec2>& points, float lineWidth,
                             const glm::vec3& color, float alpha, const glm::mat4& projection) const;
    static std::vector<glm::vec2> buildArcPoints(glm::vec2 center, float radius,
                                                  float startDeg, float endDeg, int segments);
    void drawGameInfo(const glm::mat4& projection);
    void drawModeHighlight(MODE modeId, const glm::mat4& projection);
    void drawRectOutline(float x, float y, float w, float h, float lineWidth,
                          const glm::vec3& color, float alpha, const glm::mat4& projection) const;
    void drawWheel(const glm::mat4& projection);

    GLuint createShaderProgram(const char* vertSrc, const char* fragSrc);
    static GLuint compileShader(GLenum type, const char* src);

    void loadFont(const char* fontPath, int pixelHeight);
    void renderText(const std::string& text, float x, float y, float scale,
                     const glm::vec3& color, const glm::mat4& projection);

    void cleanupGL();

    ControllerManager& controllerManager_;

    GLuint polyVAO_ = 0;
    GLuint polyVBO_ = 0;

    // Text rendering state
    GLuint textShaderProgram_ = 0;
    GLuint textVAO_ = 0;
    GLuint textVBO_ = 0;

    GLuint rectVAO_ = 0;
    GLuint rectVBO_ = 0;
    GLuint rectShaderProgram_ = 0;

    std::array<Character, 128> characters_{}; // ASCII only; textureID == 0 means "not loaded"
};

#endif //GAMEPADREALM_RENDERER_HPP