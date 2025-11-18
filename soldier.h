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

class base;

class soldier {
public:

    void display(SDL_Renderer* renderer, const texwrap& soldierTexture) const;

    soldier(double _x, double _y, int _allegiance): x(_x), y(_y), postX(x), postY(y), range(200), fireRate(10), allegiance(_allegiance), home(nullptr) {}

    //Randomly shoot against enemies who are in range
    void shoot(std::list<projectile>& projectiles, const std::vector<soldier>& soldiers, std::default_random_engine &randomEngine, double dt);

    [[nodiscard]] bool unassigned() const {return home == nullptr;}

    void setBase(base* b) {home=b;};

    void setPost(double x, double y) {
        postX=x;
        postY=y;
    }

    [[nodiscard]] double getX() const {return x;}
    [[nodiscard]] double getY() const {return y;}
    [[nodiscard]] double getRange() const {return range;}
    [[nodiscard]] int getAllegiance() const {return allegiance;}

private:
    double x;
    double y;

    //The location of the post assigned to this soldier, they will try their best to go there
    double postX;
    double postY;

    double range;

    //Chance of firing a shooting at ANY target each second
    //Target selection is based on distance
    double fireRate;

    int allegiance;

    base* home;
};


#endif //SKIRMISHSKETCH_SOLDIER_H