//
// Created by nikolaj on 11/16/25.
//

#ifndef SKIRMISHSKETCH_PROJECTILE_H
#define SKIRMISHSKETCH_PROJECTILE_H
#include <SDL2/SDL_render.h>

#include "texwrap.h"


class projectile {
public:

    projectile(double x, double y, double speed, double targetX, double targetY);
    void display(SDL_Renderer* renderer, texwrap& texture) const;

    void update(double dt);

    bool isDead() const {return lifeTime < 0;}

private:
    double speed;
    double x;
    double y;
    double vx;
    double vy;
    double lifeTime;
};


#endif //SKIRMISHSKETCH_PROJECTILE_H