#pragma once

#include "components/simple_scene.h"
#include <string>

using namespace std;


class Enemy {

public:
    string name;
    float x, y, tx; // cordinates
    int type;
    int lives_left;
    glm::vec3 color;
    bool to_del;
    float scale;

    Enemy(int type, int row);

protected:
    static long long id;
    const string base_name = "enemy";
};
