#pragma once

#include "components/simple_scene.h"
#include <string>

using namespace std;


class Projectile {

public:
    string name;
    float x, y, tx; // cordinates
    int type;
    glm::vec3 color;
    float angle;
    bool is_deleted;

    Projectile(int type, int x, int y);

protected:
    static long long id;
    const string base_name = "projectile";
};
