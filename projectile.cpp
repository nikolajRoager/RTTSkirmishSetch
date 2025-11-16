//
// Created by nikolaj on 11/16/25.
//

#include "projectile.h"

#include <iostream>

projectile::projectile(double x, double y, double speed, double targetX, double targetY) {
    this->x = x;
    this->y = y;
    this->speed = speed;

    double dx = targetX-x;
    double dy = targetY-y;

    double distance = sqrt(dx*dx+dy*dy);
    vx= speed*dx/distance;
    vy= speed*dy/distance;

    lifeTime = distance/speed;
}

void projectile::display(SDL_Renderer *renderer, texwrap& texture) const {
    if (lifeTime > 0)
        texture.render(x,y,renderer,1,true);
}

void projectile::update(double dt) {

    if (lifeTime > 0)
    {
        x+=vx*dt;
        y+=vy*dt;
        lifeTime-=dt;
    }
}
