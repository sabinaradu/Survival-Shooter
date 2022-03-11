#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        struct Obstacle
        {
            float obs_x;
            float obs_y;
            float obs_width;
            float obs_height;
        };

        struct Position
        {
            float x;
            float y;
        };

    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        bool CheckUpperBorder(float player_y, float radius, float height);
        bool CheckBottomBorder(float player_y, float radius_body, float height);
        bool CheckLeftBorder(float player_x, float radius_body, float width);
        bool CheckRightBorder(float player_x, float radius_body, float width);
        glm::vec2 CheckObstacleCollision(Obstacle obs);
        bool CheckEnemyCollision();
        Position GetRandomPosition(float width, float height);
        void MoveProjectile(float deltaTimeSeconds);
        bool CheckEnemyProjectileCollision(float projectile_x, float projectile_y, float enemy_x, float enemy_y, float enemy_side);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        //map
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
        float width, height;

        //player
        float radius_body = 5;
        float radius_eye = 1;
        float player_x = 0;
        float player_y = 0;
        int speed = 30;
        float nr_tring = 60;
        float angle = 0;

        //enemy
        float enemy_x = 10;
        float enemy_y = 10;
        float enemy_side = 5;
        float enemy_eye_side = 1;
        float enemy_angle = 0;
        float enemy_speed = 20;

        //obstacles
        Obstacle obs[6];
        int nr_obs = 6;
        float obs_ox_height = 20, obs_ox_width = 40;
        float obs_oy_height = 40, obs_oy_width = 20;
        float square_obs_side = 20;
        
        //collisions
        int index = 0;
        glm::vec2 res_p;

        //projectile
        float projectile_x = 0, projectile_y = 0;
        float projectile_width = 3;
        float projectile_height = 2;
        float projectile_speed = 100;
        float projectile_angle = 0;
        float max_distance = 100;
        const float PI = 3.2;
        Position pos;
        bool shoot;
    };
}   // namespace m1
