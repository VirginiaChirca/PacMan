#pragma once

#include "components/simple_scene.h"
#include <vector>
#define LIMIT       (.2f)

namespace m1
{
    class PacMan : public gfxc::SimpleScene
    {
    public:
        PacMan();
        ~PacMan();

        void Init() override;

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

    private:
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

        void FrameStart() override;

        void Update(float deltaTimeSeconds);
        void FrameEnd() override;


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void ResetGrid(int grid[162]);
        int IndexGrid(int x, int y);
        int Bounds(int x, int y);
        void Visit(int x, int y, int grid[162]);

        glm::vec2 GetObstaclePosition(float i, float j);
        glm::vec2 GetRectPosition(float posplayerX, float posplayerY);
        glm::vec2 GetRectPositionFood(float posfoodX, float posfoodY);
        bool CheckCollisionPlayerObs();
        bool CheckCollisionPlayerFood();
        bool CheckCollisionGhostObs(float posghostx, float posghosty);
        bool CheckCollisionGhostPlayer(float posghostx, float posghosty);
        bool CheckCollisionSpecialPlayer();
        void RenderLives();
       

    protected:
        float cx, cy;
        glm::mat3 modelMatrix, playerrecMatrix, foodrecMatrix, ghost3Matrix, ghost2Matrix, foodMatrix, modelMatrix1, mapMatrix, playerMatrix, obstacleMatrix, ghost1Matrix, ghost1recMatrix, ghost2recMatrix, ghost3recMatrix, ghost4recMatrix, ghost4Matrix, foodspecialMatrix1, foodspecialMatrix2, foodspecialMatrix3, foodspecialMatrix4, playerMatrix_mouth;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        float obstaclex, obstacley;
        float player_speed, projectile_speed, enemy_speed;
        bool move_player, move_projectile;

        float posplayerX, posplayerY, mousecrtx, mousecrty, dy, dx, projectileX, projectileY;
        float posghost1x, posghost1y, posghost2x, posghost2y, posghost3x, posghost3y, posghost4x, posghost4y, posspecial1x, posspecial1y, posspecial2x, posspecial2y, posspecial3x, posspecial3y, posspecial4x, posspecial4y;

        int nr,index;
        int grid[162];
        int width, height;
        int food[1458] = { 0 };

        int right1 = 0;
        int left1 = 1;
        int right2 = 0;
        int left2 = 1;
        int right3 = 0;
        int left3 = 1;
        int right4 = 0;
        int left4 = 1;
        int d = 0;
        int lives = 3;
        int special1 = 0;
        int nr_ghost = 0;
        int score = 0;
        double time1;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 visMatrix;
        bool check = 0;
        bool check2 = 0;
        bool  moveW, moveA, moveD, moveS;
        glm::vec2 PosObstacle, RectPos, FoodPos;
        int nrg1 = 0;
        int nrg2 = 0;

    };
}   // namespace m1
