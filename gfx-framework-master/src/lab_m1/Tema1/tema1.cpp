#include "lab_m1/Tema1/tema1.h"

#include <vector>
#include <iostream>
#include <unistd.h>

#include "lab_m1/Tema1/transform2D.h"
#include "lab_m1/Tema1/object2D.h"
#include "lab_m1/Tema1/enemy.h"
#include "lab_m1/Tema1/diamond.h"
#include "lab_m1/Tema1/projectile.h"
#include "lab_m1/Tema1/star.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // init objects on static scene
    InitStaticSceneObjects();

    // Init Gameplay variables
    this->money = 5;
    this->lives = 3;
    this->total_time = 0.0;

    // Init Random time till next enemy in seconds
    this->cur_enemy_max_time = 15;
    this->next_enemy = GenRandomFloat(2.0, this->cur_enemy_max_time);
    this->cur_time = 0;

    // Init rows
    this->enemy_rows.resize(3);
    this->diamond_rows.resize(3, vector<Diamond*>(3, nullptr));
    this->projectile_rows.resize(3);

    this->cur_star_time = 0;
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.12, 0.11, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    // if no lives left => game end :((
    if (this->lives == 0) {
        RenderScore();
        return;
    }

    // total in game time
    this->total_time += deltaTimeSeconds;


    // Enemy generation management
    if (this->cur_enemy_max_time > 5.0)
        this->cur_enemy_max_time -= deltaTimeSeconds / 7;

    this->cur_time += deltaTimeSeconds;
    if (this->cur_time >= this->next_enemy) {
        GenNewEnemy();

        // reinit random time till next enemy
        this->next_enemy = GenRandomFloat(2.0, this->cur_enemy_max_time);
        this->cur_time = 0;
    }

    // Star generation management
    this->cur_star_time += deltaTimeSeconds;
    if (this->cur_star_time >= this->next_star) {
        GenNewStars();

        // reset cur star time
        this->cur_star_time = 0;
    }


    DeleteEnemies();

    LaunchProjectiles(deltaTimeSeconds);

    DetectCollisions();

    RenderStaticScene();
    RenderDragNDrop();

    RenderEnemies(deltaTimeSeconds);
    RenderDiamonds(deltaTimeSeconds);
    RenderProjectiles(deltaTimeSeconds);
    RenderStars(deltaTimeSeconds);
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{

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
    // change mouse pos
    this->drag_tx += deltaX * 2;
    this->drag_ty -= deltaY * 2;
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // convert mouse coordinates
    mouseY = (720 - mouseY) * 2;
    mouseX *= 2;

    // create drag n drop obj
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        // Check if clicked on diamond
        for (int i = 0; i < 4; i++) {
            int center_x = this->empty_square_start_x + this->scene_square_len / 2 + i * (this->scene_square_len + this->empty_square_space);
            int center_y = this->empty_square_start_y;

            if (mouseX >= center_x - this->scene_square_len / 2 && mouseX <= center_x + this->scene_square_len / 2 &&
                mouseY >= center_y - this->scene_square_len / 2 && mouseY <= center_y + this->scene_square_len / 2) {
                // drag n drop obj creation
                Mesh* drag_n_drop_obj = object2D_tema::CreateDiamond("drag_n_drop_obj", glm::vec3(mouseX, mouseY, 10), this->scene_square_len / 5, diamond_no_stars[i].first, tun_color);
                AddMeshToList(drag_n_drop_obj);

                this->drag_n_drop_x = mouseX;
                this->drag_n_drop_y = mouseY;
                this->drag_tx = 0;
                this->drag_ty = 0;
                this->is_left_mouse_pressed = true;
                this->drag_n_drop_type = i;
            }
        }
    }

    // remove diamond on right click
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT)) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int center_x = this->scene_square_space + this->scene_start_x + this->scene_square_len / 2 + this->scene_square_len / 2 + i * (this->scene_square_len + this->scene_square_space);
                int center_y = this->scene_start_y + this->scene_square_len / 2 + j * (this->scene_square_len + this->scene_square_space);

                if (mouseX >= center_x - this->scene_square_len / 2 && mouseX <= center_x + this->scene_square_len / 2 &&
                    mouseY >= center_y - this->scene_square_len / 2 && mouseY <= center_y + this->scene_square_len / 2) {
                        this->diamond_rows[i][j]->to_del = true;
                }
            }
        }
    }

    // collect resources
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        for (auto it = star_vector.begin(); it < star_vector.end(); it++) {
            auto star = *it;

            // distance from cursor to center of star
            // sqrt(xdist ** 2 + ydist ** 2)
            float x_dist = abs(mouseX - star->x);
            float y_dist = abs(mouseY - star->y);
            float distance = sqrt(powf(x_dist, 2) + powf(y_dist, 2));

            if (distance <= this->star_size + 15) {
                it = star_vector.erase(it);
                if (this->money < 10)
                    this->money++;

                it--;
            }
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // convert mouse coordinates
    mouseY = (720 - mouseY) * 2;
    mouseX *= 2;

    // Create diamond on empty cell
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && this->is_left_mouse_pressed) {
        this->is_left_mouse_pressed = false;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int center_x = this->scene_square_space + this->scene_start_x + this->scene_square_len / 2 + this->scene_square_len / 2 + i * (this->scene_square_len + this->scene_square_space);
                int center_y = this->scene_start_y + this->scene_square_len / 2 + j * (this->scene_square_len + this->scene_square_space);

                // skip if cell is occupied
                if (diamond_rows[i][j] != nullptr)
                    continue;

                if (mouseX >= center_x - this->scene_square_len / 2 && mouseX <= center_x + this->scene_square_len / 2 &&
                mouseY >= center_y - this->scene_square_len / 2 && mouseY <= center_y + this->scene_square_len / 2) {
                    
                    // Check if enough money
                    if (this->money >= this->diamond_no_stars[this->drag_n_drop_type].second) {
                        // Buy diamond
                        this->money -= this->diamond_no_stars[this->drag_n_drop_type].second;

                        int diamond_x = center_x;
                        int diamond_y = center_y;

                        Diamond* new_diamond = new Diamond(this->drag_n_drop_type, diamond_x, diamond_y);
                        this->diamond_rows[i][j] = new_diamond;
                        Mesh* new_diamond_mesh = object2D_tema::CreateDiamond(new_diamond->name, glm::vec3(new_diamond->x, new_diamond->y, 1), this->scene_square_len / 5, new_diamond->color, tun_color);
                        AddMeshToList(new_diamond_mesh);

                        printf("Created new diamond %s at row %d col %d with type %d\n", new_diamond->name.c_str(), j, i, this->drag_n_drop_type);

                    } else {
                        printf("Not enough money to buy diamond with type %d\n", i);
                    }
                }
            }
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

// generate random float from low to high
float Tema1::GenRandomFloat(float low, float high) {
    return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
}

// generate random int from [low to high)
int Tema1::GenRandomInt(int low, int high) {
    return (rand() % (high - low)) + low;
}



// Render scene => static elements
void Tema1::RenderStaticScene() {
    // Render scene rectangle
    RenderMesh2D(meshes["scene_rectangle"], shaders["VertexColor"], glm::mat3(1));
    
    // Render scene squares
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            string name = "scene_square" + to_string(i) + to_string(j);
            RenderMesh2D(meshes[name], shaders["VertexColor"], glm::mat3(1));
        }
    }

    // Render shop items
    for (int i = 0; i < 4; i++) {
        string square_name = "empty_square" + to_string(i);
        RenderMesh2D(meshes[square_name], shaders["VertexColor"], glm::mat3(1));

        string diamond_name = "diamond_model" + to_string(i);
        RenderMesh2D(meshes[diamond_name], shaders["VertexColor"], glm::mat3(1));

        for (int j = 0; j < this->diamond_no_stars[i].second; j++) {
            string price_name = "diamond_price" + to_string(i) + to_string(j);
            RenderMesh2D(meshes[price_name], shaders["VertexColor"], glm::mat3(1));
        }
    }

    // Render lives
    for (int i = 0; i < this->lives; i++) {
        string name = "square_life" + to_string(i);
        RenderMesh2D(meshes[name], shaders["VertexColor"], glm::mat3(1));
    }

    // Render money
    for (int i = 0; i < this->money; i++) {
        string name = "money_star" + to_string(i);
        RenderMesh2D(meshes[name], shaders["VertexColor"], glm::mat3(1));
    }
}

// Init static objects for scene
void Tema1::InitStaticSceneObjects() {
    // Init scene rectangle
    Mesh* scene_rectangle = object2D_tema::CreateRectangle("scene_rectangle", glm::vec3(scene_start_x, scene_start_y, 0), this->scene_square_len / 2, this->scene_square_len * 3 + 2 * this->scene_square_space, this->scene_rectangle_color, true);
    AddMeshToList(scene_rectangle);

    // Init scene squares
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int center_x = this->scene_square_space + this->scene_start_x + this->scene_square_len / 2 + this->scene_square_len / 2 + i * (this->scene_square_len + this->scene_square_space);
            int center_y = this->scene_start_y + this->scene_square_len / 2 + j * (this->scene_square_len + this->scene_square_space);

            string name = "scene_square" + to_string(i) + to_string(j);
            Mesh* scene_square = object2D_tema::CreateSquare(name, glm::vec3(center_x, center_y, 0), this->scene_square_len, this->scene_square_color, true);
            AddMeshToList(scene_square);

        }
    }

    // Init empty squares
    for (int i = 0; i < 4; i++) {
        int center_x = this->empty_square_start_x + this->scene_square_len / 2 + i * (this->scene_square_len + this->empty_square_space);
        int center_y = this->empty_square_start_y;

        string square_name = "empty_square" + to_string(i);
        Mesh* empty_square = object2D_tema::CreateSquare(square_name, glm::vec3(center_x, center_y, 0), this->scene_square_len, this->empty_square_color);
        AddMeshToList(empty_square);

        string diamond_name = "diamond_model" + to_string(i);
        Mesh* diamond_model = object2D_tema:: CreateDiamond(diamond_name, glm::vec3(center_x, center_y, 0), this->scene_square_len / 5, this->diamond_no_stars[i].first, tun_color);
        AddMeshToList(diamond_model);

        for (int j = 0; j < this->diamond_no_stars[i].second; j++) {
            int price_center_x = center_x - this->scene_square_len * 3 / 8 + j * this->scene_square_len / 4;
            int price_center_y = center_y - this->scene_square_len / 2 - 40;

            string price_name = "diamond_price" + to_string(i) + to_string(j);
            Mesh* diamond_price = object2D_tema::CreateStar(price_name, glm::vec3(price_center_x, price_center_y, 0), this->diamond_price_size, this->diamond_price_color);
            AddMeshToList(diamond_price);
        }
    }

    // Init square lives
    for (int i = 0; i < 3; i++) {
        int center_x = 1700 + i * (this->scene_square_len * 0.7 + this->empty_square_space);
        int center_y = this->empty_square_start_y + 30;

        string name = "square_life" + to_string(i);
        Mesh* life = object2D_tema::CreateLife(name, glm::vec3(center_x, center_y, 0), this->scene_square_len * 0.45, glm::vec3(0.8, 0.05, 0.6));
        AddMeshToList(life);
    }

    // Init money_star
    for (int i = 0; i < 10; i++) {
        int center_x = 1650 + i * (this->scene_square_len / 4 + 10);
        int center_y = this->empty_square_start_y - this->scene_square_len / 2 - 40;

        string name = "money_star" + to_string(i);
        Mesh* money_star = object2D_tema::CreateStar(name, glm::vec3(center_x, center_y, 0), this->diamond_price_size, this->diamond_price_color);
        AddMeshToList(money_star);
    }

    // Init score vars
    Mesh* score_star = object2D_tema::CreateStar("score", glm::vec3(0, 0, 1), 50, glm::vec3(0.8, 0.64, 0));
    AddMeshToList(score_star);
}



// Generate new enemy
void Tema1::GenNewEnemy() {
    int rand_row = GenRandomInt(0, 3);
    int rand_type = GenRandomInt(0, 4);
    Enemy* enemy = new Enemy(rand_type, rand_row);
    this->enemy_rows[rand_row].push_back(enemy);
    Mesh* new_enemy = object2D_tema::CreateHexagon(enemy->name, glm::vec3(enemy->x, enemy->y, 2), 70, enemy->color, this->enemy_center_color);
    AddMeshToList(new_enemy);
    Mesh* new_enemy_center = object2D_tema::CreateHexagon(enemy->name + "center", glm::vec3(enemy->x, enemy->y, 3), 50, enemy->color, this->enemy_center_color);
    AddMeshToList(new_enemy_center);
    
    printf("Generated new enemy %s at row %d of type%d at coord x%.0f y%.0f\n", enemy->name.c_str(), rand_row, rand_type, enemy->x, enemy->y);
}

// Generate new stars
void Tema1::GenNewStars() {
    for (int i = 0; i < 3; i++) {
        int x = GenRandomInt(100, 2460);
        int y = GenRandomInt(100, 1340);

        Star* star = new Star(x, y);
        this->star_vector.push_back(star);

        Mesh* new_star = object2D_tema::CreateStar(star->name, glm::vec3(x, y, 10), this->star_size, this->star_color);
        AddMeshToList(new_star);

        // printf("Generated new star %s at %.0f %.0f\n", star->name.c_str(), star->x, star->y);
    }
}

// Generate new projectile
void Tema1::GenNewProjectile(int x, int y, int type, int row) {
    Projectile* projectile = new Projectile(type, x, y);
    this->projectile_rows[row].push_back(projectile);

    Mesh* new_projectile = object2D_tema::CreateStar(projectile->name, glm::vec3(x, y, 5), this->projectile_size, projectile->color);
    AddMeshToList(new_projectile);

    printf("===> Launched new projectile %s on row %d of type%d  at coord x%.0f y%.0f\n", projectile->name.c_str(), row, type, projectile->x, projectile->y);
}



// Renders temporary drag_n_drop object
void Tema1::RenderDragNDrop() {
    if (!this->is_left_mouse_pressed)
        return;

    glm::mat3 drag_n_drop_mat = glm::mat3(1);
    drag_n_drop_mat *= transform2D::Translate(this->drag_tx, this->drag_ty);

    RenderMesh2D(meshes["drag_n_drop_obj"], shaders["VertexColor"], drag_n_drop_mat);
}


// Renders score at the end of the game
void Tema1::RenderScore() {
    int score = this->total_time / 20;
    if (score > 5)
        score = 5;

    int tx = 600;
    int ty = 720;
    for (int i = 0; i < score; i++) {
        tx += 300;
        glm::mat3 matrix = glm::mat3(1);

        matrix *= transform2D::Translate(tx, ty);
        RenderMesh2D(meshes["score"], shaders["VertexColor"], matrix);
    }
}

// Render stars
void Tema1::RenderStars(float deltaTimeSeconds) {
    for (auto& star : star_vector) {
        glm::mat3 matrix = glm::mat3(1);

        if (star->ty >= 0) {
            star->ty -= deltaTimeSeconds * 600 * (1440 / star->y);
            matrix *= transform2D::Translate(0, star->ty);
        }

        RenderMesh2D(meshes[star->name], shaders["VertexColor"], matrix);
    }
}

// Renders diamonds
void Tema1::RenderDiamonds(float deltaTimeSeconds) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (diamond_rows[i][j] == nullptr)
                continue;

            auto diamond = diamond_rows[i][j];

            glm::mat3 matrix = glm::mat3(1);
            if (diamond->to_del) {
                diamond->scale -= deltaTimeSeconds * 2;
                matrix *= transform2D::Translate(diamond->x, diamond->y);
                matrix *= transform2D::Scale(diamond->scale, diamond->scale);
                matrix *= transform2D::Translate( -diamond->x, -diamond->y);
            }

            RenderMesh2D(meshes[diamond->name], shaders["VertexColor"], matrix);

            if (diamond->scale <= 0)
                diamond_rows[i][j] = nullptr;
        }
    }
}

// Render enemies
void Tema1::RenderEnemies(float deltaTimeSeconds) {
    for (int i = 0; i < 3; i++) {
        for (auto& enemy : enemy_rows[i]) {
            glm::mat3 matrix = glm::mat3(1);

            if (enemy->to_del) {
                // Scaling
                enemy->scale -= deltaTimeSeconds * 2;
                matrix *= transform2D::Translate(enemy->x + enemy->tx, enemy->y);
                matrix *= transform2D::Scale(enemy->scale, enemy->scale);
                matrix *= transform2D::Translate(-enemy->x, -enemy->y);
            } else {
                // Translation
                enemy->tx -= deltaTimeSeconds * 100;
                matrix *= transform2D::Translate(enemy->tx, 0);
            }

            // Render enemy
            RenderMesh2D(meshes[enemy->name], shaders["VertexColor"], matrix);
            RenderMesh2D(meshes[enemy->name + "center"], shaders["VertexColor"], matrix);
        }
    }
}

// Render projectiles
void Tema1::RenderProjectiles(float deltaTimeSeconds) {
    for (int i = 0; i < 3; i++) {
        for (const auto& proj : this->projectile_rows[i]) {
            glm::mat3 matrix = glm::mat3(1);

            proj->tx += deltaTimeSeconds * 500;
            matrix *= transform2D::Translate(proj->tx, 0);

            proj->angle -= deltaTimeSeconds * 3;
            matrix *= transform2D::Translate(proj->x, proj->y);
            matrix *= transform2D::Rotate(proj->angle);
            matrix *= transform2D::Translate(-proj->x, -proj->y);

            RenderMesh2D(meshes[proj->name], shaders["VertexColor"], matrix);
        }
    }

    // delete projectiles if out of scene
    for (int i = 0; i < 3; i++) {
        for (auto it = projectile_rows[i].begin(); it != projectile_rows[i].end(); it++) {
            auto p = *it;

            if (p->x + p->tx > 2700) {
                it = projectile_rows[i].erase(it);
                it--;
            }
        }
    }
}



// Launch projectiles when enemy_type == diamond_type
void Tema1::LaunchProjectiles(float deltaTimeSeconds) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            auto diamond = diamond_rows[i][j];
            if (diamond == nullptr)
                continue;

            bool to_launch = false;
            for (auto& enemy : enemy_rows[j]) {
                if (enemy->type == diamond->type)
                    to_launch = true;
            }

            if (to_launch) {
                diamond->cur_time += deltaTimeSeconds;
                if (diamond->cur_time >= this->launch_interval) {
                    // launch
                    int px = diamond->x;
                    int py = diamond->y;
                    int type = diamond->type;
                    int row = j;
                    GenNewProjectile(px, py, type, row);

                    diamond->cur_time = 0;
                }
            } else {
                diamond->cur_time = 0;
            }
        }
    }
}

// Detect collisions projectile-enemy && enemy-diamond
void Tema1::DetectCollisions() {
    // detect diamond collision with enemies
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (diamond_rows[i][j] == nullptr)
                continue;

            auto diamond = diamond_rows[i][j];

            for (int k = 0; k < 3; k++) {
                for (auto& enemy : enemy_rows[k]) {
                    // dist on axis
                    float x_dist = abs(diamond->x - (enemy->x + enemy->tx));
                    float y_dist = abs(diamond->y - enemy->y);
                    float dist = sqrt(powf(x_dist, 2) + powf(y_dist, 2));

                    if (dist < this->scene_square_len / 5 + 80)
                        diamond->to_del = true;
                }
            }
        }
    }


    // collision detection between proj and hexa
    for (int i = 0; i < 3; i++) {
        for (auto& e : enemy_rows[i]) {
            for (auto it = projectile_rows[i].begin(); it != projectile_rows[i].end(); it++) {
                auto p = *it;

                if (p->type != e->type)
                    continue;

                float dist = abs((p->x + p->tx) - (e->x + e->tx));
                if (dist < 80) {
                    it = projectile_rows[i].erase(it);
                    it--;

                    e->lives_left--;
                }
            }
        }
    }
}

// Delete enemies on red rectangle / projectile hit
void Tema1::DeleteEnemies() {
    for (int i = 0; i < 3; i++) {
        for (auto it = enemy_rows[i].begin(); it != enemy_rows[i].end(); it++) {
            auto enemy = *it;

            if (enemy->lives_left == 0)
                enemy->to_del = true;

            bool at_end_rectangle = false;
            if (enemy->x + enemy->tx < this->scene_start_x + this->scene_square_len / 4) {
                enemy->to_del = true;
                at_end_rectangle = true;
            }

            if (enemy->scale <= 0) {
                it = enemy_rows[i].erase(it);
                it--;
                // lost a life
                if (at_end_rectangle)
                    this->lives--;
            }
        }
    }
}
