#include "lab_m1/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/Tema1/Transform2D.h"
#include "lab_m1/Tema1/Object2D.h"

using namespace std;
using namespace m1;


bool up;
bool grow;
float radians;

Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    width = 200;
    height = 150;

    logicSpace.width = 80;
    logicSpace.height = 80;

    logicSpace.x = -40;
    logicSpace.y = -40;

    glm::vec3 corner = glm::vec3(0, 0, 0);

    Mesh* square1 = Object2D::CreateRectangle("square1", corner, width, height, glm::vec3(0.94f, 0.81f, 0.99f));
    AddMeshToList(square1);

    Mesh* circle1 = Object2D::CreateCircle("circle1", nr_tring, radius_body, glm::vec3(0.6f, 0.196078f, 0.8f));
    AddMeshToList(circle1);

    Mesh* circle2 = Object2D::CreateCircle("circle2", nr_tring, radius_eye, glm::vec3(0.6f, 0.196078f, 0.8f));
    AddMeshToList(circle2);

    Mesh* circle3 = Object2D::CreateCircle("circle3", nr_tring, radius_eye, glm::vec3(0.6f, 0.196078f, 0.8f));
    AddMeshToList(circle3);

    Mesh* square_obs = Object2D::CreateRectangle("square_obs", corner, square_obs_side, square_obs_side, glm::vec3(0.00f, 0.00f, 0.61f));
    AddMeshToList(square_obs);

    Mesh* obs_ox = Object2D::CreateRectangle("obs_ox", corner, obs_ox_width, obs_ox_height, glm::vec3(0.00f, 0.00f, 0.61f));
    AddMeshToList(obs_ox);

    Mesh* obs_oy = Object2D::CreateRectangle("obs_oy", corner, obs_oy_width, obs_oy_height, glm::vec3(0.00f, 0.00f, 0.61f));
    AddMeshToList(obs_oy);

    Mesh* enemy_body = Object2D::CreateRectangle("enemy_body", corner, enemy_side, enemy_side, glm::vec3(1, 0, 0));
    AddMeshToList(enemy_body);

    Mesh* enemy_eye1 = Object2D::CreateRectangle("enemy_eye1", corner, enemy_eye_side, enemy_eye_side, glm::vec3(1, 0, 0));
    AddMeshToList(enemy_eye1);

    Mesh* enemy_eye2 = Object2D::CreateRectangle("enemy_eye2", corner, enemy_eye_side, enemy_eye_side, glm::vec3(1, 0, 0));
    AddMeshToList(enemy_eye2);

    Mesh* projectile = Object2D::CreateRectangle("projectile", corner, projectile_width, projectile_height, glm::vec3(0, 0, 0));
    AddMeshToList(projectile);
}

glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    // Sets the screen area where to draw - the left half of the window
    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    modelMatrix = visMatrix * Transform2D::Translate(projectile_x, projectile_y);
    RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);
    MoveProjectile(deltaTimeSeconds);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    modelMatrix = visMatrix * Transform2D::Translate(enemy_x + enemy_side / 2 - enemy_eye_side / 2, enemy_y + enemy_side / 2 + enemy_eye_side / 2);
    RenderMesh2D(meshes["enemy_eye2"], shaders["VertexColor"], modelMatrix);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    modelMatrix = visMatrix * Transform2D::Translate(enemy_x - enemy_side / 2 + enemy_eye_side / 2, enemy_y + enemy_side / 2 + enemy_eye_side / 2);
    RenderMesh2D(meshes["enemy_eye1"], shaders["VertexColor"], modelMatrix);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    modelMatrix = visMatrix * Transform2D::Translate(enemy_x, enemy_y);
    RenderMesh2D(meshes["enemy_body"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * Transform2D::Translate(width / 2 - 40, height / 2 - 45);
    RenderMesh2D(meshes["obs_oy"], shaders["VertexColor"], modelMatrix);
    obs[index].obs_x = width / 2 - 40;
    obs[index].obs_y = height / 2 - 45;
    obs[index].obs_height = obs_oy_height;
    obs[index].obs_width = obs_oy_width;
    index++;

    modelMatrix = visMatrix * Transform2D::Translate(-width / 2 + 40, -height / 2 + 45);
    RenderMesh2D(meshes["obs_oy"], shaders["VertexColor"], modelMatrix);
    obs[index].obs_x = -width / 2 + 40;
    obs[index].obs_y = -height / 2 + 45;
    obs[index].obs_height = obs_oy_height;
    obs[index].obs_width = obs_oy_width;
    index++;

    modelMatrix = visMatrix * Transform2D::Translate(-width / 2 + 40, height / 2 - 45);
    RenderMesh2D(meshes["obs_ox"], shaders["VertexColor"], modelMatrix);
    obs[index].obs_x = -width / 2 + 40;
    obs[index].obs_y = height / 2 - 45;
    obs[index].obs_height = obs_ox_height;
    obs[index].obs_width = obs_ox_width;
    index++;

    modelMatrix = visMatrix * Transform2D::Translate(width / 2 - 40, -height / 2 + 45);
    RenderMesh2D(meshes["obs_ox"], shaders["VertexColor"], modelMatrix);
    obs[index].obs_x = width / 2 - 40;
    obs[index].obs_y = -height / 2 + 45;
    obs[index].obs_height = obs_ox_height;
    obs[index].obs_width = obs_ox_width;
    index++;

    modelMatrix = visMatrix * Transform2D::Translate(width / 2 - 85, height / 2 - 50);
    RenderMesh2D(meshes["square_obs"], shaders["VertexColor"], modelMatrix);
    obs[index].obs_x = width / 2 - 85;
    obs[index].obs_y = height / 2 - 50;
    obs[index].obs_height = square_obs_side;
    obs[index].obs_width = square_obs_side;
    index++;

    modelMatrix = visMatrix * Transform2D::Translate(-width / 2 + 85, -height / 2 + 50);
    RenderMesh2D(meshes["square_obs"], shaders["VertexColor"], modelMatrix);
    obs[index].obs_x = -width / 2 + 85;
    obs[index].obs_y = -height / 2 + 50;
    obs[index].obs_height = square_obs_side;
    obs[index].obs_width = square_obs_side;
    index = 0;
    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    modelMatrix = visMatrix * Transform2D::Translate(player_x, player_y);
    modelMatrix *= Transform2D::Rotate(angle);
    RenderMesh2D(meshes["circle1"], shaders["VertexColor"], modelMatrix);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    modelMatrix = visMatrix * Transform2D::Translate(player_x + sin(angle + PI / 6) * radius_body, player_y - cos(angle + PI / 6) * radius_body);
    RenderMesh2D(meshes["circle2"], shaders["VertexColor"], modelMatrix);

    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    modelMatrix = visMatrix * Transform2D::Translate(player_x + sin(angle - PI / 6) * radius_body, player_y - cos(angle - PI / 6) * radius_body);
    RenderMesh2D(meshes["circle3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = visMatrix * Transform2D::Translate(0, 0);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

bool Tema1::CheckUpperBorder(float player_y, float radius_body, float height) {
    if (player_y + radius_body >= height / 2) {
        return true;
    }
    return false;
}

bool Tema1::CheckBottomBorder(float player_y, float radius_body, float height) {
    if (player_y - radius_body <= - height / 2) {
        return true;
    }
    return false;
}

bool Tema1::CheckLeftBorder(float player_x, float radius_body, float width) {
    if (player_x - radius_body <= -width / 2) {
        return true;
    }
    return false;
}

bool Tema1::CheckRightBorder(float player_x, float radius_body, float width) {
    if (player_x + radius_body >= width / 2) {
        return true;
    }
    return false;
}

glm::vec2 Tema1::CheckObstacleCollision(Obstacle obs) {
    glm::vec2 playerCenter = { player_x, player_y };
    glm::vec2 obstacleCenter;
    glm::vec2 dif;
    glm::vec2 clamped;
    glm::vec2 p;
    glm::vec2 interval;

    obstacleCenter = { obs.obs_x, obs.obs_y };
    interval = { obs.obs_width / 2.0f, obs.obs_height / 2.0f };
    dif = playerCenter - obstacleCenter;
    clamped = glm::clamp(dif, -interval, interval);
    p = obstacleCenter + clamped;
    dif = p - playerCenter;

    if (glm::length(dif) < radius_body) {
        return p;
    }
    return { 0, 0 };
}

bool Tema1::CheckEnemyCollision() {
    glm::vec2 playerCenter = { player_x, player_y };
    glm::vec2 obstacleCenter;
    glm::vec2 dif;
    glm::vec2 clamped;
    glm::vec2 p;
    glm::vec2 interval;

    obstacleCenter = { enemy_x, enemy_y };
    interval = { enemy_side / 2.0f, enemy_side / 2.0f };
    dif = playerCenter - obstacleCenter;
    clamped = glm::clamp(dif, -interval, interval);
    p = obstacleCenter + clamped;
    dif = p - playerCenter;

    if (glm::length(dif) < radius_body) {
        return true;
    }
    return false;
}

bool Tema1::CheckEnemyProjectileCollision(float projectile_x, float projectile_y, float enemy_x, float enemy_y, float enemy_side) {
    bool collision_right = enemy_x + enemy_side * 0.75f >= projectile_x;
    bool collision_left = enemy_x - enemy_side * 0.75f <= projectile_x;
    bool collision_up = enemy_y + enemy_side * 0.75f >= projectile_y;
    bool collision_down = enemy_y - enemy_side * 0.75f <= projectile_y;

    return collision_right && collision_left && collision_up && collision_down;
}

Tema1::Position Tema1::GetRandomPosition(float width, float height) {
    Tema1::Position res;
    res.x = rand() % (int)(width / 2) + (int)(-width / 2);
    res.y = rand() % (int)(height / 2) + (int)(-height / 2);
    return res;
}

void Tema1::DrawScene(glm::mat3 visMatrix) {

}

void Tema1::FrameEnd()
{
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (deltaTime < 0 || isinf(deltaTime))
        return;

    if (window->KeyHold(GLFW_KEY_W)) {
        logicSpace.y += deltaTime * speed;
        player_y += deltaTime * speed;
    }
    if (CheckUpperBorder(player_y, radius_body, height)) {
        logicSpace.y -= deltaTime * speed;
        player_y -= deltaTime * speed;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        logicSpace.y -= deltaTime * speed;
        player_y -= deltaTime * speed;
    }
    if (CheckBottomBorder(player_y, radius_body, height)) {
        logicSpace.y += deltaTime * speed;
        player_y += deltaTime * speed;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        logicSpace.x -= deltaTime * speed;
        player_x -= deltaTime * speed;
    }
    if (CheckLeftBorder(player_x, radius_body, width)) {
        logicSpace.x += deltaTime * speed;
        player_x += deltaTime * speed;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        logicSpace.x += deltaTime * speed;
        player_x += deltaTime * speed;
    }
    if (CheckRightBorder(player_x, radius_body, width)) {
        logicSpace.x -= deltaTime * speed;
        player_x -= deltaTime * speed;
    }

    for (int i = 0; i < nr_obs; i++) {
        res_p = CheckObstacleCollision(obs[i]);
        if (res_p[0] != 0 && res_p[1] != 0) {
            if (res_p[0] == obs[i].obs_x + obs[i].obs_width / 2) {
                logicSpace.x += deltaTime * speed;
                player_x += deltaTime * speed;
            }
            else if (res_p[0] == obs[i].obs_x - obs[i].obs_width / 2) {
                logicSpace.x -= deltaTime * speed;
                player_x -= deltaTime * speed;
            }
            else if (res_p[1] == obs[i].obs_y + obs[i].obs_height / 2) {
                logicSpace.y += deltaTime * speed;
                player_y += deltaTime * speed;
            }
            else if (res_p[1] == obs[i].obs_y - obs[i].obs_height / 2) {
                logicSpace.y -= deltaTime * speed;
                player_y -= deltaTime * speed;
            }
        }
    }

    if (CheckEnemyProjectileCollision(projectile_x, projectile_y, enemy_x, enemy_y, enemy_side)) {
        pos = GetRandomPosition(width, height);
        enemy_x = pos.x;
        enemy_y = pos.y;
    }

    if (!Tema1::CheckEnemyCollision()) {
        enemy_angle = atan2(player_y - enemy_y, player_x - enemy_x);
        enemy_x += cos(enemy_angle) * enemy_speed * deltaTime;
        enemy_y += sin(enemy_angle) * enemy_speed * deltaTime;
    }
    else {
        pos = GetRandomPosition(width, height);
        enemy_x = pos.x;
        enemy_y = pos.y;
    }
}

void Tema1::MoveProjectile(float deltaTimeSeconds) {
    if (!shoot) {
        projectile_angle = angle;
        projectile_x = player_x;
        projectile_y = player_y;
    }
    else {
        projectile_x += sin(projectile_angle) * projectile_speed * deltaTimeSeconds;
        projectile_y -= cos(projectile_angle) * projectile_speed * deltaTimeSeconds;
        
        bool stop_right = projectile_x > player_x + max_distance;
        bool stop_left = projectile_x < player_x - max_distance;
        bool stop_up = projectile_y > player_y + max_distance;
        bool stop_down = projectile_y < player_y - max_distance;

        if (stop_right || stop_left || stop_up || stop_down) {
            projectile_x = player_x;
            projectile_y = player_y;
            projectile_angle = angle;
            projectile_speed = 0;
            shoot = false;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    angle = atan2((mouseX - (window->GetResolution().x) / 2), (mouseY - (window->GetResolution().y) / 2));
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        projectile_speed += 100;
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        shoot = true;
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}
