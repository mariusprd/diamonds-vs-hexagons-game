#include "projectile.h"

Projectile::Projectile(int type, int x, int y) {
    this->name = this->base_name + to_string(this->id);
    this->type = type;
    this->x = x;
    this->y = y;

    this->tx = 0;
    this->angle = 0;
    this->is_deleted = false;

    switch(type) {
        case 0:
            this->color = {1, 0.647, 0};
            break;
        case 1:
            this->color = {0, 0, 1};
            break;
        case 2:
            this->color = {1, 1, 0};
            break;
        case 3:
            this->color = {0.5, 0, 0.5};
            break;
        default:
            printf("ERROR! Incorrect PROJECTILE type => type 1\n");
    }

    // increment id => for next name
    this->id++;
};

long long Projectile::id = 0;
