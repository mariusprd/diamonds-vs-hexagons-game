#pragma once

#include "components/simple_scene.h"
#include <string>

using namespace std;


class Star {

public:
    string name;
    float x, y, ty; // cordinates

    Star(int x, int y);

protected:
    static long long id;
    const string base_name = "star";
};
