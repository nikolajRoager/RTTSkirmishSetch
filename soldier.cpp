//
// Created by nikolaj on 11/15/25.
//

#include "soldier.h"
#include "base.h"

#include <iostream>

void soldier::display(SDL_Renderer *renderer, const texwrap& soldierTexture) const {
    soldierTexture.render(x,y,renderer,1.0,true);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(renderer,x,y,postX,postY);
    /*
    double p_angle=0;
    double dtheta = 0.1;
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (double angle = dtheta; angle < 360; angle += dtheta) {
        SDL_RenderDrawLine(renderer,x+range*cos(p_angle),y+range*sin(p_angle),x+range*cos(angle),y+range*sin(angle));
        p_angle = angle;
    }
    */
}


void soldier::shoot(std::list<projectile> &projectiles, const std::vector<soldier> &soldiers, std::default_random_engine &randomEngine, double dt) {
    //Create list of targets with weighter probability
    std::vector<std::pair<double/*Weight of this target*/,int/*index of target in list of soldiers*/> > targets;

    double totalWeight=0;
    for (int i = 0; i< soldiers.size(); ++i) {
        double dist = sqrt(pow(x-soldiers[i].x,2)+pow(y-soldiers[i].y,2));

        //We wouldn't want to shoot ourselves, and we wouldn't waste bullets on someone out of range
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
