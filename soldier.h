//
// Created by nikolaj on 11/15/25.
//

#ifndef SKIRMISHSKETCH_SOLDIER_H
#define SKIRMISHSKETCH_SOLDIER_H
#include <list>
#include <random>
#include <vector>
#include <SDL2/SDL_render.h>

#include "projectile.h"
#include "texwrap.h"

#define BULLET_SPEED 500

class soldier {
public:

    void display(SDL_Renderer* renderer, const texwrap& soldierTexture) const;

    soldier(double _x, double _y, int _allegience): x(_x), y(_y), range(200), fireRate(10), allegience(_allegience) {}

    //Randomly shoot against enemies who are in range
    void shoot(std::list<projectile>& projectiles, const std::vector<soldier>& soldiers, std::default_random_engine &randomEngine, double dt);

private:
    double x;
    double y;

    double range;

    //Chance of firing a shooting at ANY target each second
    //Target selection is based on distance
    double fireRate;

    int allegience;
};


#endif //SKIRMISHSKETCH_SOLDIER_H