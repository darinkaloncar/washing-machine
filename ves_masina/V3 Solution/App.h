#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

struct Vec2 {
    float x, y;
};

enum class AppState {
    LOAD,
    WASHING,
    FINISHED
};

struct Garment {
    Vec2 pos;
    Vec2 size;

    bool isRed = false;
    bool inMachine = false;
    bool visible = true;
    bool dragging = false;

    enum class Motion { ORBIT, FALL } motion = Motion::ORBIT;

    float orbitAngle = 0.0f;
    float fallVy = 0.0f;
    float rotation = 0.0f;

    // tint (white -> pink)
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;

    unsigned tex = 0;
    int contactSide = 2;
};

class App {
public:
    bool init(GLFWwindow* window);
    void update(double now, double dt);
    void render();
    void cleanup();

    void onCursorPos(double xpos, double ypos);
    void onMouseButton(int button, int action, int mods);
    void onKey(int key, int scancode, int action, int mods);

    static int endProgram(const std::string& msg);

private:
    float clampf(float x, float a, float b);
    float lerpf(float a, float b, float t);

    Vec2 screenToNDC(double sx, double sy) const;
    bool pointInRect(Vec2 p, Vec2 c, Vec2 half);
    bool pointInCircle(Vec2 p, Vec2 c, float r);

    void preprocessTexture(unsigned& texture, const char* filepath);

    void buildQuad();
    void buildShirtQuad(float w, float h);
    void buildMachineQuad(float w, float h);
    void buildCircle(int segments);
    void buildDrumQuad(float w, float h);


    void initGarments();
    void recomputeRedInside();
    void startWashing();
    void computeDrumFromMachineImage(float cxN, float cyN, float rN);

    void setBasicUniforms(float offx, float offy,
        float scx, float scy,
        float rot,
        float cr, float cg, float cb, float ca,
        float alpha);

private:
    GLFWwindow* window_ = nullptr;
    GLFWcursor* cursor_ = nullptr;

    int screenWidth_ = 800;
    int screenHeight_ = 800;

    double mouseX_ = 0.0;
    double mouseY_ = 0.0;
    bool lmbDown_ = false;
    int draggedIndex_ = -1;
    float aspect_ = 1.0f;

    AppState state_ = AppState::LOAD;

    unsigned idShader_ = 0;
    unsigned basicShader_ = 0;
    unsigned drumShader_ = 0;


    unsigned idTexture_ = 0;
    unsigned texWhiteShirt_ = 0;

    unsigned texWhiteSocks_ = 0;
    unsigned texWhiteDress_ = 0;
    unsigned texRedShirt_ = 0;
    unsigned texMachine_ = 0;
    unsigned texMachineWashOverlay_ = 0;

    unsigned VAO_ID_ = 0, VBO_ID_ = 0;

    unsigned VAO_quad_ = 0, VBO_quad_ = 0;
    unsigned VAO_circle_ = 0, VBO_circle_ = 0;
    int circleVertexCount_ = 0;

    unsigned VAO_shirt_ = 0, VBO_shirt_ = 0;
    unsigned VAO_machine_ = 0, VBO_machine_ = 0;

    unsigned texDrum_ = 0;
    unsigned VAO_drum_ = 0, VBO_drum_ = 0;

    float machineFadeT_ = 0.0f;

    Vec2 drumCenter_ = { 0.45f, -0.05f };
    float drumRadius_ = 0.23f;
    float drumVisualRadius_ = 0.0f; // grey circle
    float drumOrbitRadius_ = 0.0f; // garment orbit


    Vec2 machinePos_ = { 0.45f, -0.02f };
    float machineW_ = 1.2f;
    float machineH_ = 1.2f;

    std::vector<Garment> garments_;

    bool redInside_ = false;

    float drumAngle_ = 0.0f;
    float drumSpeed_ = 0.0f;
    float targetSpeed_ = 6.0f;
    float accel_ = 10.0f;

    bool holdPhase_ = false;
    double holdStartTime_ = 0.0;
    double washingStartTime_ = 0.0;

    float doorCloseT_ = 0.0f;
    bool doorClosed_ = false;

    float idOffsetX_ = 0.0f;
    float idOffsetY_ = 0.0f;
};
