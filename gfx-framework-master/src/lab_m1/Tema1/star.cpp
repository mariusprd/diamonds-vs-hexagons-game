#include "star.h"

Star::Star(int x, int y) {
    this->name = this->base_name + to_string(id);
    this->x = x;
    this->y = y;
    this->ty = 1440 - y;

    this->id++;
};

long long Star::id = 0;