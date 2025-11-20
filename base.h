//
// Created by nikolaj on 11/16/25.
//

#ifndef SKIRMISHSKETCH_BASE_H
#define SKIRMISHSKETCH_BASE_H
#include <cmath>
#include <map>
#include <memory>
#include <set>
#include <vector>

#include "soldier.h"


class base {
public:

    base(double x, double y,double radius);

    void display(SDL_Renderer* renderer, bool selected, bool target) const;

    [[nodiscard]] double getX() const {return x;}
    [[nodiscard]] double getY() const {return y;}

    void addSoldier(std::shared_ptr<soldier> s);

    ///Order either all or half soldiers to move to the target base
    void moveSoldierTo (int allegiance,base& target, bool all);
private:
    double x;
    double y;


    struct frontlineSegment {
        const double x0,y0;
        const double x1,y1;
        const double length;
        const double startLength;
        frontlineSegment(double _x0, double _y0, double _x1, double _y1, double _startLength):x0(_x0),y0(_y0),x1(_x1),y1(_y1),length(std::sqrt(pow(x0-x1,2)+pow(y0-y1,2))), startLength(_startLength) {
        }
    };

    std::vector<frontlineSegment> frontline;
    double frontlineLength;

    //List of squads of soldiers with different allegiances positioned around this base, indexed by allegiance
    std::map<int, std::set<std::shared_ptr<soldier>> > squads;

    void updateSoldierLocations();

};


#endif //SKIRMISHSKETCH_BASE_H