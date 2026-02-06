#include "App.h"
#include "Util.h"
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

static constexpr float PI = 3.14159265f;

int App::endProgram(const std::string& msg) {
    std::cout << msg << "\n";
    glfwTerminate();
    return -1;
}

float App::clampf(float x, float a, float b) { return (x < a) ? a : (x > b) ? b : x; }
float App::lerpf(float a, float b, float t) { return a + (b - a) * t; }

void App::preprocessTexture(unsigned& texture, const char* filepath)
{
    texture = loadImageToTexture(filepath);
    if (texture == 0) {
        std::cout << "Textura nije ucitana! Putanja texture: " << filepath << "\n";
        return;
    }
    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Vec2 App::screenToNDC(double sx, double sy) const
{
    float x = (float)((sx / (double)screenWidth_) * 2.0 - 1.0);
    float y = (float)(1.0 - (sy / (double)screenHeight_) * 2.0);
    return { x, y };
}

bool App::pointInRect(Vec2 p, Vec2 c, Vec2 half)
{
    return (p.x >= c.x - half.x && p.x <= c.x + half.x &&
        p.y >= c.y - half.y && p.y <= c.y + half.y);
}

bool App::pointInCircle(Vec2 p, Vec2 c, float r)
{
    float dx = p.x - c.x;
    float dy = p.y - c.y;
    return (dx * dx + dy * dy) <= r * r;
}

void App::buildQuad()
{
    float quad[] = {
        // x,y    u,v
        -0.5f,  0.5f, 0,1,
        -0.5f, -0.5f, 0,0,
         0.5f, -0.5f, 1,0,
         0.5f,  0.5f, 1,1
    };

    glGenVertexArrays(1, &VAO_quad_);
    glGenBuffers(1, &VBO_quad_);

    glBindVertexArray(VAO_quad_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_quad_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void App::buildShirtQuad(float w, float h)
{
    float hw = w * 0.5f;
    float hh = h * 0.5f;

    float quad[] = {
        -hw, +hh, 0,1,
        -hw, -hh, 0,0,
        +hw, -hh, 1,0,
        +hw, +hh, 1,1
    };

    glGenVertexArrays(1, &VAO_shirt_);
    glGenBuffers(1, &VBO_shirt_);

    glBindVertexArray(VAO_shirt_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_shirt_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void App::buildMachineQuad(float w, float h)
{
    float hw = w * 0.5f;
    float hh = h * 0.5f;

    float quad[] = {
        -hw, +hh, 0,1,
        -hw, -hh, 0,0,
        +hw, -hh, 1,0,
        +hw, +hh, 1,1
    };

    glGenVertexArrays(1, &VAO_machine_);
    glGenBuffers(1, &VBO_machine_);

    glBindVertexArray(VAO_machine_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_machine_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void App::buildCircle(int segments)
{
    std::vector<float> verts;
    verts.reserve((segments + 2) * 4);

    // center
    verts.push_back(0.0f); verts.push_back(0.0f);
    verts.push_back(0.5f); verts.push_back(0.5f);

    for (int i = 0; i <= segments; i++) {
        float a = (float)i / (float)segments * 2.0f * PI;
        float x = cosf(a);
        float y = sinf(a);
        verts.push_back(x); verts.push_back(y);
        verts.push_back((x + 1.0f) * 0.5f);
        verts.push_back((y + 1.0f) * 0.5f);
    }

    circleVertexCount_ = (int)(verts.size() / 4);

    glGenVertexArrays(1, &VAO_circle_);
    glGenBuffers(1, &VBO_circle_);

    glBindVertexArray(VAO_circle_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_circle_);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void App::initGarments()
{
    garments_.clear();
    std::vector<Vec2> pos = {
        {-0.78f, 0.45f}, {-0.78f, 0.20f}, {-0.78f, -0.05f}, {-0.78f, -0.30f}
    };

    // 0: bela majica, 1: bele čarape, 2: bela haljina, 3: crvena majica
    for (int i = 0; i < 4; i++) {
        Garment g;
        g.pos = pos[i];
        g.size = { 0.25f, 0.25f };

        g.isRed = (i == 3);

        if (g.isRed) { g.r = 1.0f; g.g = 0.15f; g.b = 0.15f; g.tex = texRedShirt_; }
        else {
            g.r = 1.0f; g.g = 1.0f; g.b = 1.0f;
            if (i == 0) g.tex = texWhiteShirt_;
            else if (i == 1) g.tex = texWhiteSocks_;
            else if (i == 2) g.tex = texWhiteDress_;
        }

        garments_.push_back(g);
    }
}


void App::recomputeRedInside()
{
    redInside_ = false;
    for (auto& g : garments_) {
        if (g.inMachine && g.isRed) { redInside_ = true; break; }
    }
}

void App::startWashing()
{
    state_ = AppState::WASHING;
    washingStartTime_ = glfwGetTime();
    holdPhase_ = false;
    drumAngle_ = 0.0f;
    drumSpeed_ = 0.0f;
    doorCloseT_ = 0.0f;
    doorClosed_ = false;

    for (auto& g : garments_) {
        if (!g.inMachine) g.visible = false;
        g.dragging = false;
    }
    draggedIndex_ = -1;

    recomputeRedInside();
}

void App::setBasicUniforms(float offx, float offy, float scx, float scy, float rot,
    float cr, float cg, float cb, float ca, float alpha)
{
    glUniform2f(glGetUniformLocation(basicShader_, "uOffset"), offx, offy);
    glUniform2f(glGetUniformLocation(basicShader_, "uScale"), scx, scy);
    glUniform1f(glGetUniformLocation(basicShader_, "uRotation"), rot);
    glUniform4f(glGetUniformLocation(basicShader_, "uColor"), cr, cg, cb, ca);
    glUniform1f(glGetUniformLocation(basicShader_, "uAlpha"), alpha);
}

bool App::init(GLFWwindow* window)
{
    window_ = window;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    screenWidth_ = mode->width;
    screenHeight_ = mode->height;
    aspect_ = (float)screenWidth_ / (float)screenHeight_;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    cursor_ = loadImageToCursor("res/kursor.png");
    if (!cursor_) cursor_ = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    glfwSetCursor(window_, cursor_);

    idShader_ = createShader("id.vert", "id.frag");
    glUseProgram(idShader_);
    glUniform1i(glGetUniformLocation(idShader_, "uTex"), 0);

    glUniform3f(glGetUniformLocation(idShader_, "uTint"), 1.0f, 1.0f, 1.0f);

    glUseProgram(0);


    basicShader_ = createShader("basic.vert", "basic.frag");

    preprocessTexture(idTexture_, "res/id_card.png");
    preprocessTexture(texWhiteShirt_, "res/white_shirt.png");
    preprocessTexture(texWhiteSocks_, "res/carape.png");
    preprocessTexture(texWhiteDress_, "res/haljina.png");
    preprocessTexture(texRedShirt_, "res/red_shirt.png");
    preprocessTexture(texMachine_, "res/ves_masina_bez_vrata.png");
    preprocessTexture(texMachineWashOverlay_, "res/mas.png");
    preprocessTexture(texDrum_, "res/drum.png");

    drumShader_ = createShader("drum.vert", "drum.frag");

    glUseProgram(drumShader_);
    glUniform1f(glGetUniformLocation(drumShader_, "uScale"), 1.2f);
    glUniform1i(glGetUniformLocation(drumShader_, "uTex"), 0);
    glUniform1f(glGetUniformLocation(drumShader_, "uAspect"), aspect_);
    glUniform2f(glGetUniformLocation(drumShader_, "uNudge"), 0.35f, -0.02f);
    glUniform3f(glGetUniformLocation(drumShader_, "uTint"), 1.0f, 1.0f, 1.0f);
    glUseProgram(0);
    

    // name and idx put in upper left corner
    float left = -0.98f, top = 0.98f;
    float w = 0.55f, h = 0.18f;
    float right = left + w;
    float bottom = top - h;

    float quad[] = {
        left,  top,    0.0f, 1.0f,
        left,  bottom, 0.0f, 0.0f,
        right, bottom, 1.0f, 0.0f,
        right, top,    1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO_ID_);
    glGenBuffers(1, &VBO_ID_);

    glBindVertexArray(VAO_ID_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    buildQuad();               
    buildShirtQuad(0.25f, 0.25f);
    buildCircle(90);

    int mw = 0, mh = 0, mc = 0;
    machineW_ = 1.20f;
    machineH_ = machineW_;
    if (stbi_info("res/ves_masina_bez_vrata.png", &mw, &mh, &mc) && mw > 0 && mh > 0) {
        machineH_ = machineW_ * ((float)mh / (float)mw) * ((float)screenWidth_ / (float)screenHeight_);
    }
    buildMachineQuad(machineW_, machineH_);

    computeDrumFromMachineImage(0.50f, 0.48f, 0.20f);
    drumVisualRadius_ = drumRadius_ * 1.9f; 
    drumOrbitRadius_ = drumRadius_ * 0.65f;
    float d = drumRadius_ * 3.30f;
    buildDrumQuad(d, d);
    initGarments();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

void App::update(double now, double dt)
{

    float target = (state_ == AppState::WASHING) ? 1.0f : 0.0f;

    // speed
    float fadeSpeed = (float)(dt / 0.6); // 0.6s
    machineFadeT_ = clampf(machineFadeT_ + (target - machineFadeT_) * fadeSpeed * 6.0f, 0.0f, 1.0f);

    const double targetFrame = 1.0 / 75.0;

    if (state_ != AppState::WASHING) return;

    // door closing
    doorCloseT_ = clampf(doorCloseT_ + (float)(dt / 0.6), 0.0f, 1.0f);
    doorClosed_ = (doorCloseT_ >= 1.0f);

    // spin: speed up to some speed and then 20s
    if (!holdPhase_) {
        drumSpeed_ += accel_ * (float)dt;
        if (drumSpeed_ >= targetSpeed_) {
            drumSpeed_ = targetSpeed_;
            holdPhase_ = true;
            holdStartTime_ = now;
        }
    }
    else {
        if (now - holdStartTime_ >= 20.0) {
            state_ = AppState::FINISHED;
            drumSpeed_ = 0.0f;
            doorCloseT_ = 0.0f;
            doorClosed_ = false;
        }
    }

    // clockwise
    drumAngle_ -= drumSpeed_ * (float)dt;

    double totalExpected = 0.6 + 20.0;
    float washProgress = (float)clampf((float)((now - washingStartTime_) / totalExpected), 0.0f, 1.0f);

    // white->pink tint
    if (redInside_) {
        for (auto& g : garments_) {
            if (!g.inMachine) continue;
            if (g.isRed) continue;

            float tr = 1.0f, tg = 0.70f, tb = 0.80f;
            g.r = lerpf(1.0f, tr, washProgress);
            g.g = lerpf(1.0f, tg, washProgress);
            g.b = lerpf(1.0f, tb, washProgress);
        }
    }

    for (auto& g : garments_) {
        if (!g.inMachine) continue;

        if (g.motion == Garment::Motion::ORBIT) {
            // clockwise
            g.orbitAngle -= 2.5f * (float)dt;

            // total angle includes drum rotation
            float a = drumAngle_ + g.orbitAngle;

            g.pos = {
                drumCenter_.x + drumOrbitRadius_ * cosf(a),
                drumCenter_.y + drumOrbitRadius_ * sinf(a)  
            };


            float base = a - PI / 2.0f;
            g.rotation = base + g.contactSide * (PI / 2.0f);

            if (sinf(a) > 0.98f) {
                g.motion = Garment::Motion::FALL;
                g.fallVy = 0.0f;
                g.pos.y = drumCenter_.y + drumOrbitRadius_;
            }
        }
        else {
            // FALL
            float gravity = -2.8f;
            g.fallVy += gravity * (float)dt;
            g.pos.y += g.fallVy * (float)dt;

            float bottomY = drumCenter_.y - drumOrbitRadius_;
            if (g.pos.y <= bottomY) {
                g.pos.y = bottomY;

                g.motion = Garment::Motion::ORBIT;

                g.orbitAngle = -PI / 2.0f - drumAngle_;

                g.contactSide = (g.contactSide + 2) % 4;

                g.fallVy = 0.0f;
            }
        }
    }

}

void App::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(drumShader_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texDrum_);

    glUniform2f(glGetUniformLocation(drumShader_, "uOffset"),
        drumCenter_.x, drumCenter_.y);

    float rot = (state_ == AppState::WASHING) ? drumAngle_ : 0.0f;
    glUniform1f(glGetUniformLocation(drumShader_, "uRotation"), rot);

    float alpha = (state_ == AppState::WASHING) ? 0.90f : 1.0f;
    glUniform1f(glGetUniformLocation(drumShader_, "uAlpha"), alpha);

    glUniform3f(glGetUniformLocation(drumShader_, "uTint"), 1.0f, 1.0f, 1.0f);

    glBindVertexArray(VAO_drum_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);



    glUseProgram(idShader_);
    glActiveTexture(GL_TEXTURE0);

    glUniform1f(glGetUniformLocation(idShader_, "uRotation"), 0.0f);
    glUniform3f(glGetUniformLocation(idShader_, "uTint"), 1.0f, 1.0f, 1.0f);
    glUniform2f(glGetUniformLocation(idShader_, "uOffset"), machinePos_.x, machinePos_.y);

    glBindVertexArray(VAO_machine_);

    // base fade-out
    glBindTexture(GL_TEXTURE_2D, texMachine_);
    glUniform1f(glGetUniformLocation(idShader_, "uAlpha"), 1.0f - machineFadeT_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    // washing fade-in
    glBindTexture(GL_TEXTURE_2D, texMachineWashOverlay_);
    glUniform1f(glGetUniformLocation(idShader_, "uAlpha"), machineFadeT_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);


    glUseProgram(idShader_);
    glActiveTexture(GL_TEXTURE0);

    for (auto& g : garments_) {
        if (!g.visible) continue;
        if (state_ == AppState::WASHING && !g.inMachine) continue;

        glBindTexture(GL_TEXTURE_2D, g.tex);

        glUniform2f(glGetUniformLocation(idShader_, "uOffset"), g.pos.x, g.pos.y);
        glUniform1f(glGetUniformLocation(idShader_, "uAlpha"), 1.0f);
        glUniform1f(glGetUniformLocation(idShader_, "uRotation"), g.rotation);

        glUniform3f(glGetUniformLocation(idShader_, "uTint"), g.r, g.g, g.b);

        glBindVertexArray(VAO_shirt_);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }


    glBindVertexArray(0);
    glUseProgram(0);

    if (state_ == AppState::WASHING) {
        glUseProgram(basicShader_);
        float alpha = 0.55f * doorCloseT_;
        setBasicUniforms(drumCenter_.x, drumCenter_.y,
            drumVisualRadius_ / aspect_, drumVisualRadius_,
            0.0f,
            0.15f, 0.15f, 0.16f, 1.0f,
            alpha);

        glBindVertexArray(VAO_circle_);
        glDrawArrays(GL_TRIANGLE_FAN, 0, circleVertexCount_);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    // ID CARD
    glUseProgram(idShader_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, idTexture_);

    glUniform3f(glGetUniformLocation(idShader_, "uTint"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(idShader_, "uRotation"), 0.0f);

    glUniform1f(glGetUniformLocation(idShader_, "uAlpha"), 0.80f);
    glUniform2f(glGetUniformLocation(idShader_, "uOffset"), idOffsetX_, idOffsetY_);

    glBindVertexArray(VAO_ID_);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(0);
    glUseProgram(0);
}

void App::cleanup()
{
    glDeleteBuffers(1, &VBO_ID_);
    glDeleteVertexArrays(1, &VAO_ID_);

    glDeleteProgram(idShader_);
    glDeleteProgram(basicShader_);

    glDeleteBuffers(1, &VBO_quad_);
    glDeleteVertexArrays(1, &VAO_quad_);
    glDeleteBuffers(1, &VBO_circle_);
    glDeleteVertexArrays(1, &VAO_circle_);

    glDeleteBuffers(1, &VBO_shirt_);
    glDeleteVertexArrays(1, &VAO_shirt_);

    glDeleteBuffers(1, &VBO_machine_);
    glDeleteVertexArrays(1, &VAO_machine_);
    glDeleteTextures(1, &texMachine_);
    glDeleteTextures(1, &texMachineWashOverlay_);
    glDeleteTextures(1, &texWhiteSocks_);
    glDeleteTextures(1, &texWhiteDress_);
    glDeleteTextures(1, &texDrum_);
    glDeleteProgram(drumShader_);
    glDeleteBuffers(1, &VBO_drum_);
    glDeleteVertexArrays(1, &VAO_drum_);


    if (cursor_) glfwDestroyCursor(cursor_);
}

void App::onCursorPos(double xpos, double ypos)
{
    mouseX_ = xpos;
    mouseY_ = ypos;

    if (state_ != AppState::LOAD) return;
    if (draggedIndex_ < 0) return;

    Vec2 ndc = screenToNDC(mouseX_, mouseY_);
    garments_[draggedIndex_].pos = ndc;
}

void App::onMouseButton(int button, int action, int /*mods*/)
{
    if (state_ != AppState::LOAD) return;

    if (button != GLFW_MOUSE_BUTTON_LEFT) return;

    if (action == GLFW_PRESS) {
        lmbDown_ = true;

        double mx, my;
        glfwGetCursorPos(window_, &mx, &my);
        Vec2 ndc = screenToNDC(mx, my);

        draggedIndex_ = -1;
        for (int i = (int)garments_.size() - 1; i >= 0; i--) {
            auto& g = garments_[i];
            if (!g.visible || g.inMachine) continue;

            Vec2 half = { g.size.x * 0.5f, g.size.y * 0.5f };
            if (pointInRect(ndc, g.pos, half)) {
                draggedIndex_ = i;
                g.dragging = true;
                break;
            }
        }
    }
    else if (action == GLFW_RELEASE) {
        lmbDown_ = false;

        if (draggedIndex_ >= 0) {
            auto& g = garments_[draggedIndex_];
            g.dragging = false;

            if (pointInCircle(g.pos, drumCenter_, drumRadius_)) {
                g.inMachine = true;

                float rnd = (float)(0.6 * (rand() / (double)RAND_MAX) - 0.3);
                g.motion = Garment::Motion::ORBIT;
                g.orbitAngle = -PI / 2.0f + rnd;
                g.rotation = 0.0f;

                float a = drumAngle_ + g.orbitAngle;
                g.pos = {
                    drumCenter_.x + drumOrbitRadius_ * cosf(a),
                    drumCenter_.y + drumOrbitRadius_ * sinf(a)
                };

            }

            draggedIndex_ = -1;
        }
    }
}

void App::onKey(int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
        return;
    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        if (state_ == AppState::LOAD) {
            bool anyInside = false;
            for (auto& g : garments_) if (g.inMachine) { anyInside = true; break; }
            if (anyInside) startWashing();
        }
    }
}
void App::computeDrumFromMachineImage(float cxN, float cyN, float rN)
{
    drumCenter_.x = machinePos_.x + (cxN - 0.51f) * machineW_;
    drumCenter_.y = machinePos_.y + (0.5f - cyN) * machineH_;

    drumRadius_ = rN * machineW_;
}

void App::buildDrumQuad(float w, float h)
{
    float hw = w * 0.5f;
    float hh = h * 0.5f;

    float quad[] = {
        -hw, +hh, 0,1,
        -hw, -hh, 0,0,
        +hw, -hh, 1,0,
        +hw, +hh, 1,1
    };

    glGenVertexArrays(1, &VAO_drum_);
    glGenBuffers(1, &VBO_drum_);

    glBindVertexArray(VAO_drum_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_drum_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
