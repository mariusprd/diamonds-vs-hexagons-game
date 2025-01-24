#pragma once

#include "components/simple_scene.h"
#include <string>

using namespace std;


class Diamond {

public:
    string name;
    float x, y, tx, ty; // cordinates
    int type;
    glm::vec3 color;
    bool to_del;
    float scale;

    float cur_time;

    Diamond(int type, int x, int y);

protected:
    static long long id;
    const string base_name = "diamond";
};
