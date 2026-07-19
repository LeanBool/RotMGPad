#include "Renderer.hpp"
#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>
#define GLFW_EXPOSE_NATIVE_X11
#elif defined(_WIN32)
#define GLFW_EXPOSE_NATIVE_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#endif
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_timer.h>
#include "../globals.hpp"
#include "controls/modes/InventoryMode.hpp"

// ---------- Shaders ----------
const char* textVertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec4 vertex; // vec2 pos, vec2 texCoords
out vec2 TexCoords;
uniform mat4 uProjection;
void main() {
    gl_Position = uProjection * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}
)";

const char* textFragmentShaderSrc = R"(
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D uText;
uniform vec3 uTextColor;
void main() {
    float alpha = texture(uText, TexCoords).r;
    FragColor = vec4(uTextColor, alpha);
}
)";

const char* rectVertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform mat4 uMVP;
void main() {
    gl_Position = uMVP * vec4(aPos, 0.0, 1.0);
}
)";

const char* rectFragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
uniform float uAlpha;
void main() {
    FragColor = vec4(uColor, uAlpha);
}
)";

void Renderer::loadFont(const char* fontPath, int pixelHeight) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init FreeType\n";
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face)) {
        std::cerr << "Failed to load font: " << fontPath << "\n";
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, pixelHeight);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < characters_.size(); c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load glyph: " << c << "\n";
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width, face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        characters_[c] = Character{
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    textShaderProgram_ = createShaderProgram(textVertexShaderSrc, textFragmentShaderSrc);

    glGenVertexArrays(1, &textVAO_);
    glGenBuffers(1, &textVBO_);
    glBindVertexArray(textVAO_);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Renderer::renderText(const std::string& text, float x, float y, float scale,
                           const glm::vec3& color, const glm::mat4& projection) {
    glUseProgram(textShaderProgram_);
    glUniform3f(glGetUniformLocation(textShaderProgram_, "uTextColor"), color.x, color.y, color.z);
    glUniformMatrix4fv(glGetUniformLocation(textShaderProgram_, "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO_);

    for (char c : text) {
        const auto idx = static_cast<unsigned char>(c);
        if (idx >= characters_.size() || characters_[idx].textureID == 0) continue;
        const Character& ch = characters_[idx];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y + (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        float vertices[6][4] = {
            { xpos,     ypos - h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos - h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos - h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Renderer::compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error: " << infoLog << "\n";
    }
    return shader;
}

GLuint Renderer::createShaderProgram(const char* vertSrc, const char* fragSrc) {
    GLuint vert = compileShader(GL_VERTEX_SHADER, vertSrc);
    GLuint frag = compileShader(GL_FRAGMENT_SHADER, fragSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vert);
    glAttachShader(program, frag);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program link error: " << infoLog << "\n";
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
    return program;
}

void Renderer::drawGameInfo(const glm::mat4& projection) {
    const glm::vec3 white(1.0f);
    renderText(controllerManager_.getModeName(), 25.0f, 50.0f, 1.0f, white, projection);

    if (controllerManager_.paused) {
        renderText("Paused", 25.0f, 100.0f, 1.0f, white, projection);
    }

    if (controllerManager_.isWheelOpen()) {
        drawWheel(projection);
        return;
    }

    drawModeHighlight(controllerManager_.currentMode()->id(), projection);
}


void Renderer::drawModeHighlight(MODE modeId, const glm::mat4& projection) {
    const glm::vec3 white(1.0f);
    constexpr float kOutlineWidth = 4.0f;

    // Fixed-region modes — no item grid, so handle them directly.
    if (modeId == teleportMode) {
        drawRectOutline(MAP_CENTER_X - MAP_RADIUS, MAP_CENTER_Y - MAP_RADIUS,
                         2 * MAP_RADIUS, 2 * MAP_RADIUS, kOutlineWidth, white, 1.0f, projection);
        return;
    }
    if (modeId == partyMode) {
        drawRectOutline(CHAT_X - CHAT_WIDTH / 2, CHAT_Y - CHAT_HEIGHT,
                         CHAT_WIDTH, CHAT_HEIGHT, kOutlineWidth, white, 1.0f, projection);
        return;
    }

    GridHighlight grid{};
    if (modeId == inventoryMode) {
        grid = {static_cast<float>(INV_SLOT0_X), static_cast<float>(INV_SLOT0_Y),
                static_cast<float>(INV_SLOT_WIDTH), static_cast<float>(INV_SLOT_HEIGHT),
                static_cast<float>(INV_SLOT_WIDTH), static_cast<float>(INV_SLOT_HEIGHT), 4};
    } else if (modeId == pickupMode) {
        grid = {static_cast<float>(PICKUP_SLOT0_X), static_cast<float>(PICKUP_SLOT0_Y),
                static_cast<float>(PICKUP_SLOT_WIDTH), static_cast<float>(PICKUP_SLOT_HEIGHT),
                static_cast<float>(PICKUP_SLOT_WIDTH), static_cast<float>(PICKUP_SLOT_HEIGHT), 4};
    } else if (modeId == vaultMode) {
        grid = {static_cast<float>(VAULT_SLOT0_X), static_cast<float>(VAULT_SLOT0_Y),
                static_cast<float>(INV_SLOT_WIDTH), static_cast<float>(INV_SLOT_HEIGHT),
                static_cast<float>(INV_SLOT_WIDTH), static_cast<float>(INV_SLOT_HEIGHT), 8};
    } else if (modeId == potionMode) {
        grid = {static_cast<float>(POTION_RACK_SLOT0_X), static_cast<float>(POTION_RACK_SLOT0_Y),
                static_cast<float>(POTION_RACK_SLOT_WIDTH), static_cast<float>(POTION_RACK_SLOT_HEIGHT),
                static_cast<float>(POTION_RACK_SLOT_OFFSET_X), static_cast<float>(POTION_RACK_SLOT_OFFSET_Y), 2};
    } else if (modeId == equipmentMode) {
        grid = {static_cast<float>(EQUIPMENT_SLOT0_X), static_cast<float>(EQUIPMENT_SLOT0_Y),
                static_cast<float>(EQUIPMENT_SLOT_WIDTH), static_cast<float>(EQUIPMENT_SLOT_HEIGHT),
                static_cast<float>(EQUIPMENT_SLOT_WIDTH), static_cast<float>(EQUIPMENT_SLOT_HEIGHT), 4};
    } else if (modeId == menuBarMode) {
        grid = {static_cast<float>(MENUBAR_X), static_cast<float>(MENUBAR_Y),
                static_cast<float>(MENUBAR_WIDTH), static_cast<float>(MENUBAR_HEIGHT),
                static_cast<float>(MENUBAR_WIDTH), static_cast<float>(MENUBAR_WIDTH), 4};
    } else if (modeId == petInventoryMode) {
        grid = {static_cast<float>(PET_INVENTORY_X), static_cast<float>(PET_INVENTORY_Y),
                static_cast<float>(PET_INVENTORY_WIDTH), static_cast<float>(PET_INVENTORY_HEIGHT),
                static_cast<float>(PET_INVENTORY_WIDTH), static_cast<float>(PET_INVENTORY_HEIGHT), 4};
    } else {
        return;
    }

    const int itemIndex = controllerManager_.currentMode()->itemIndex();
    const float x = grid.originX - grid.slotWidth / 2 + (itemIndex % grid.columns) * grid.strideX;
    const float y = grid.originY - grid.slotHeight / 2 + (itemIndex / grid.columns) * grid.strideY;
    drawRectOutline(x, y, grid.slotWidth, grid.slotHeight, kOutlineWidth, white, 1.0f, projection);
}

int Renderer::SDL_main() {
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    initScaledGlobals(mode->width, mode->height);

    GLFWwindow* window =
        glfwCreateWindow(mode->width, mode->height,
                         "Overlay",
                         nullptr,
                         nullptr);

    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
#ifdef __linux__

    Display* display = glfwGetX11Display();
    Window x11Window = glfwGetX11Window(window);

    XRectangle rect = {0, 0, 0, 0};

    XserverRegion region =
        XFixesCreateRegion(display, &rect, 1);

    XFixesSetWindowShapeRegion(
        display,
        x11Window,
        ShapeInput,
        0,
        0,
        region
    );

    XFixesDestroyRegion(display, region);

#elif defined(_WIN32)
    HWND hwnd = glfwGetWin32Window(window);

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE;
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_COLORKEY);

    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
#endif

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    rectShaderProgram_ =
        createShaderProgram(
            rectVertexShaderSrc,
            rectFragmentShaderSrc
        );

    glGenVertexArrays(1, &polyVAO_);
    glGenBuffers(1, &polyVBO_);

    glBindVertexArray(polyVAO_);

    glBindBuffer(GL_ARRAY_BUFFER, polyVBO_);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        0
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    glGenVertexArrays(1, &rectVAO_);
    glGenBuffers(1, &rectVBO_);

    glBindVertexArray(rectVAO_);

    glBindBuffer(
        GL_ARRAY_BUFFER,
        rectVBO_
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * 2 * 4,
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        0
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    loadFont(
        "fonts/DejaVuSans.ttf",
        48
    );


    glm::mat4 projection =
        glm::ortho(
            0.0f,
            (float)mode->width,
            (float)mode->height,
            0.0f,
            -1.0f,
            1.0f
        );


    while (
        !glfwWindowShouldClose(window) &&
        controllerManager_.running
    ) {

        controllerManager_.mainLoop();

        SDL_Delay(16);


        glClearColor(
            0.0f,
            0.0f,
            0.0f,
            0.0f
        );

        glClear(GL_COLOR_BUFFER_BIT);
        drawGameInfo(projection);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    cleanupGL();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}

float Renderer::measureTextWidth(const std::string& text, float scale) const {
    float width = 0.0f;
    for (char c : text) {
        const auto idx = static_cast<unsigned char>(c);
        if (idx >= characters_.size() || characters_[idx].textureID == 0) continue;
        width += (characters_[idx].advance >> 6) * scale;
    }
    return width;
}

void Renderer::cleanupGL() {
    for (auto& ch : characters_) {
        if (ch.textureID != 0) {
            glDeleteTextures(1, &ch.textureID);
            ch.textureID = 0;
        }
    }

    if (textVAO_) glDeleteVertexArrays(1, &textVAO_);
    if (textVBO_) glDeleteBuffers(1, &textVBO_);
    if (textShaderProgram_) glDeleteProgram(textShaderProgram_);

    if (polyVAO_) glDeleteVertexArrays(1, &polyVAO_);
    if (polyVBO_) glDeleteBuffers(1, &polyVBO_);
    if (rectVAO_) glDeleteVertexArrays(1, &rectVAO_);
    if (rectVBO_) glDeleteBuffers(1, &rectVBO_);
    if (rectShaderProgram_) glDeleteProgram(rectShaderProgram_);
}

void Renderer::drawRectOutline(float x, float y, float w, float h, float lineWidth,
                                const glm::vec3& color, float alpha, const glm::mat4& projection) const {
    float corners[4][2] = {
        { x,     y     },
        { x + w, y     },
        { x + w, y + h },
        { x,     y + h }
    };

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(corners), corners);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glm::mat4 mvp = projection; // no model transform needed, corners are already in screen space

    glUseProgram(rectShaderProgram_);
    glUniformMatrix4fv(glGetUniformLocation(rectShaderProgram_, "uMVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform3f(glGetUniformLocation(rectShaderProgram_, "uColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(rectShaderProgram_, "uAlpha"), alpha);

    glLineWidth(lineWidth);
    glBindVertexArray(rectVAO_);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::drawWheel(const glm::mat4& projection) {
    const auto& entries = ControllerManager::kWheelEntries;
    const int count = static_cast<int>(entries.size());
    const float wedgeDeg = 360.0f / count;
    const int selection = controllerManager_.wheelSelection();
    const glm::vec2 center(WHEEL_CENTER_X, WHEEL_CENTER_Y);
    constexpr int kCircleSegments = 48;
    constexpr int kWedgeSegments = 12;

    auto disc = buildArcPoints(center, WHEEL_RADIUS, 0.0f, 360.0f, kCircleSegments);
    disc.insert(disc.begin(), center);
    drawFilledPolygon(disc, glm::vec3(0.0f), 0.5f, projection);

    if (selection >= 0) {
        const float centerDeg = selection * wedgeDeg;
        auto wedge = buildArcPoints(center, WHEEL_RADIUS, centerDeg - wedgeDeg / 2.0f, centerDeg + wedgeDeg / 2.0f, kWedgeSegments);
        wedge.insert(wedge.begin(), center);
        drawFilledPolygon(wedge, glm::vec3(1.0f, 0.85f, 0.2f), 0.6f, projection);
    }

    drawPolygonOutline(buildArcPoints(center, WHEEL_RADIUS, 0.0f, 360.0f, kCircleSegments),
                        3.0f, glm::vec3(1.0f), 1.0f, projection);

    constexpr float kLabelRadiusRatio = 0.6f;
    constexpr float kScale = 1.0f;
    for (int i = 0; i < count; ++i) {
        const float deg = i * wedgeDeg;
        const float rad = deg * (3.14159265358979323846f / 180.0f);
        const glm::vec2 pos = center + WHEEL_RADIUS * kLabelRadiusRatio * glm::vec2(std::sin(rad), -std::cos(rad));
        const glm::vec3 color = (i == selection) ? glm::vec3(0.1f) : glm::vec3(1.0f);

        const float textWidth = measureTextWidth(entries[i].label, kScale);
        renderText(entries[i].label, pos.x - textWidth / 2.0f, pos.y + 12.0f, kScale, color, projection);
    }
}

std::vector<glm::vec2> Renderer::buildArcPoints(glm::vec2 center, float radius,
                                                 float startDeg, float endDeg, int segments) {
    std::vector<glm::vec2> points;
    points.reserve(segments + 1);
    constexpr float kPi = 3.14159265358979323846f;
    for (int i = 0; i <= segments; ++i) {
        const float deg = startDeg + (endDeg - startDeg) * (static_cast<float>(i) / segments);
        const float rad = deg * (kPi / 180.0f);
        // 0 deg = up, increasing clockwise — matches ControllerManager::computeWheelSelection
        points.emplace_back(center.x + radius * std::sin(rad), center.y - radius * std::cos(rad));
    }
    return points;
}

void Renderer::drawFilledPolygon(const std::vector<glm::vec2>& points, const glm::vec3& color,
                                  float alpha, const glm::mat4& projection) const {
    glBindBuffer(GL_ARRAY_BUFFER, polyVBO_);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(rectShaderProgram_);
    glUniformMatrix4fv(glGetUniformLocation(rectShaderProgram_, "uMVP"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(rectShaderProgram_, "uColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(rectShaderProgram_, "uAlpha"), alpha);

    glBindVertexArray(polyVAO_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<GLsizei>(points.size()));
    glBindVertexArray(0);
}

void Renderer::drawPolygonOutline(const std::vector<glm::vec2>& points, float lineWidth,
                                   const glm::vec3& color, float alpha, const glm::mat4& projection) const {
    glBindBuffer(GL_ARRAY_BUFFER, polyVBO_);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUseProgram(rectShaderProgram_);
    glUniformMatrix4fv(glGetUniformLocation(rectShaderProgram_, "uMVP"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(rectShaderProgram_, "uColor"), color.x, color.y, color.z);
    glUniform1f(glGetUniformLocation(rectShaderProgram_, "uAlpha"), alpha);

    glLineWidth(lineWidth);
    glBindVertexArray(polyVAO_);
    glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(points.size()));
    glBindVertexArray(0);
}