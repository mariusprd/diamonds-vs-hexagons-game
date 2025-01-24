#include "diamond.h"

Diamond::Diamond(int type, int x, int y) {
    this->name = this->base_name + to_string(this->id);
    this->type = type;
    this->x = x;
    this->y = y;

    this->tx = 0;
    this->ty = 0;
    this->to_del = false;
    this->scale = 1;
    this->cur_time = 2;

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
            printf("ERROR! Incorrect type => type 1\n");
    }

    // increment id => for next name
    this->id++;
};

long long Diamond::id = 0;
