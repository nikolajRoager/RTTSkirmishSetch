//
// Created by nikolaj on 11/15/25.
//

#include "soldier.h"
#include "base.h"

#include <iostream>

void soldier::display(SDL_Renderer *renderer, const texwrap& soldierTexture) const {
    Uint8 r=255; Uint8 g=255; Uint8 b=255;
    switch (allegiance) {
        case 0:
            r=255; g=0; b=0;
            break;
        case 1:
            r=0; g=255; b=0;
            break;
        case 2:
            r=0; g=0; b=255;
            break;

        default:
            break;
    }

    soldierTexture.render(x,y,r,g,b,renderer,1.0,true);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer,x,y,postX,postY);
}


void soldier::update(double dt) {
    double targetDistanceX = postX - x;
    double targetDistanceY = postY - y;

    double distance = sqrt(targetDistanceX * targetDistanceX + targetDistanceY * targetDistanceY);

    if (distance>0.1) {
        x+=dt*WALK_SPEED*targetDistanceX /distance;
        y+=dt*WALK_SPEED*targetDistanceY /distance;
    }
    else {
        x=postX;
        y=postY;
    }
}


void soldier::shoot(std::list<projectile> &projectiles, const std::vector<soldier> &soldiers, std::default_random_engine &randomEngine, double dt) {
    //Create list of targets with weighter probability
    std::vector<std::pair<double/*Weight of this target*/,int/*index of target in list of soldiers*/> > targets;

    double totalWeight=0;
    for (int i = 0; i< soldiers.size(); ++i) {
        double dist = sqrt(pow(x-soldiers[i].x,2)+pow(y-soldiers[i].y,2));

        //We wouldn't want to shoot ourselves (the way bullets are handled, shooting at someone on top of me actually kills myself), and we wouldn't waste bullets on someone out of range
        if (dist>16 && dist<range && soldiers[i].allegiance!=allegiance) {
            totalWeight+=range-dist;
            targets.emplace_back(totalWeight,i);
        }
    }

    if (!targets.empty() && dt>0/*dt==0 (pause) would crash the poisson distribution*/) {
        std::uniform_real_distribution<double> targetWeightDist(0,totalWeight);
        double mean = dt*fireRate;
        std::poisson_distribution<int> dist(mean);

        int shots = dist(randomEngine);
        for (int i = 1; i < shots; ++i) {
            double targetWeight = targetWeightDist(randomEngine);

            //Find the first target with a higher weight than what we rolled, that is our target
            for (auto& target : targets) {
                if (targetWeight<target.first) {

                    projectiles.emplace_back(x,y,BULLET_SPEED,soldiers[target.second].x,soldiers[target.second].y);
                    break;
                }
            }

        }
    }
}
