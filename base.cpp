//
// Created by nikolaj on 11/16/25.
//

#include "base.h"

#include <iostream>

base::base(double x, double y, double radius) {
    this->x = x;
    this->y = y;

    frontlineLength=0;
    frontline.emplace_back(x+radius,y-radius,x+radius,y+radius,frontlineLength);
    frontlineLength+=frontline[0].length;
    frontline.emplace_back(x+radius,y+radius,x-radius,y+radius,frontlineLength);
    frontlineLength+=frontline[1].length;
    frontline.emplace_back(x-radius,y+radius,x-radius,y-radius,frontlineLength);
    frontlineLength+=frontline[2].length;
    frontline.emplace_back(x-radius,y-radius,x+radius,y-radius,frontlineLength);
    frontlineLength+=frontline[3].length;
}

void base::addSoldier(std::shared_ptr<soldier> s) {
    if (!squads.contains(s->getAllegiance())) {
        squads.emplace(s->getAllegiance(),std::set<std::shared_ptr<soldier>>({s}));
    }
    else {
        squads[s->getAllegiance()].emplace(s);
    }

    updateSoldierLocations();
}

void base::updateSoldierLocations() {
    int squadId =0;
    for (auto& squad : squads) {
        //If the soldiers are spread evenly along the frontline, what will be their spacing
        double spacing = frontlineLength/squad.second.size();

        //Try to make sure different squads don't overlap
        double length=squadId*spacing/3;
        ++squadId;
        int lineId = 0;
        for (auto& s : squad.second) {
            for (int i = 1; i < frontline.size(); i++) {
                if (frontline[i].startLength > length) {
                    break;
                }
                lineId=i;
            }
            double fac = (length-frontline[lineId].startLength)/frontline[lineId].length;
            s->setPost(frontline[lineId].x1*fac+frontline[lineId].x0*(1-fac),frontline[lineId].y1*fac+frontline[lineId].y0*(1-fac));
            length+=spacing;
        }
    }
}


void base::display(SDL_Renderer* renderer) const {

    SDL_Rect rect = {(int)x-10,(int)y-10,20,20};
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderFillRect(renderer,&rect);

    for (const auto& line : frontline) {
        SDL_RenderDrawLine(renderer,line.x0,line.y0,line.x1,line.y1);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    for (const auto& squad : squads) {
        for (std::shared_ptr<soldier> s : squad.second) {
            SDL_RenderDrawLine(renderer,x,y,s->getX(),s->getY());
        }
    }
}

