#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema1/enemy.h"
#include "lab_m1/Tema1/diamond.h"
#include "lab_m1/Tema1/projectile.h"
#include "lab_m1/Tema1/star.h"



namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        float GenRandomFloat(float low, float high);
        int GenRandomInt(int low, int high);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void InitStaticSceneObjects();

        void RenderStaticScene();
        void RenderDragNDrop();
        void RenderScore();
        void RenderStars(float deltaTimeSeconds);
        void RenderDiamonds(float deltaTimeSeconds);
        void RenderEnemies(float deltaTimeSeconds);
        void RenderProjectiles(float deltaTimeSeconds);

        void GenNewEnemy();
        void GenNewStars();
        void GenNewProjectile(int x, int y, int type, int row);

        void LaunchProjectiles(float deltaTimeSeconds);
        void DetectCollisions();
        void DeleteEnemies();

     protected:
        // Scene squares variables
        const glm::vec3 scene_square_color = glm::vec3(0, 0.75, 0);
        const int scene_start_x = 50;
        const int scene_start_y = 50;
        const int scene_square_len = 250;
        const int scene_square_space = 60;

        // Scene rectangle variables
        const glm::vec3 scene_rectangle_color = glm::vec3(0.8, 0, 0);

        // Scene empty squares variables
        const glm::vec3 empty_square_color = glm::vec3(0.3, 0.3, 0.3);
        const int empty_square_start_x = 120;
        const int empty_square_start_y = 1250;
        const int empty_square_space = 100;
        const std::vector<std::pair<glm::vec3, int>> diamond_no_stars =
        {
            {glm::vec3(1, 0.647, 0), 1},
            {glm:: vec3(0, 0, 1), 2},
            {glm::vec3(1, 1, 0), 2},
            {glm::vec3(0.5, 0, 0.5), 4}
        };
        const glm::vec3 diamond_price_color = glm::vec3(0.8, 0.8, 0.8);
        const float diamond_price_size = 15;

        // enemy variables
        const glm::vec3 enemy_center_color = glm::vec3(0, 0, 0);
        vector<vector<Enemy*>> enemy_rows;
        float next_enemy; // random time to next enemy
        float cur_time;   // cur passed time since last enemy
        float cur_enemy_max_time;

        // Gameplay variables
        int money;
        int lives;
        float total_time;

        // drag n drop variables
        vector<vector<Diamond*>> diamond_rows;
        float drag_n_drop_x;
        float drag_n_drop_y;
        float drag_tx;
        float drag_ty;
        bool is_left_mouse_pressed;
        int drag_n_drop_type;
        glm::vec3 tun_color = glm::vec3(1, 0, 0);

        // star reward variables
        vector<Star*> star_vector;
        const glm::vec3 star_color = glm::vec3(0.5, 0.15, 0.8);
        const float star_size = 40;
        const float next_star = 10.0;
        float cur_star_time;

        // projectiles variables
        vector<vector<Projectile*>> projectile_rows;
        float launch_interval = 2;
        const float projectile_size = 35;

    };
}   // namespace m1
