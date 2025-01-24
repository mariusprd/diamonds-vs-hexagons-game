#include "enemy.h"

Enemy::Enemy(int type, int row) {
    this->name = this->base_name + to_string(this->id);
    this->type = type;
    this->x = 2560;
    this->y = 175 + row * 310;

    this->tx = 0;
    this->to_del = false;
    this->scale = 1;

    switch(type) {
        case 0:
            this->lives_left = 1;
            this->color = {1, 0.647, 0};
            break;
        case 1:
            this->lives_left = 2;
            this->color = {0, 0, 1};
            break;
        case 2:
            this->lives_left = 2;
            this->color = {1, 1, 0};
            break;
        case 3:
            this->lives_left = 3;
            this->color = {0.5, 0, 0.5};
            break;
        default:
            printf("ERROR! Incorrect type => type 1\n");
            this->lives_left = 1;
    }

    // increment id => for next name
    this->id++;
};

long long Enemy::id = 0;
