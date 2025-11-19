#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "base.h"
#include "projectile.h"
#include "soldier.h"
#include "texwrap.h"


int main() {
    std::cout << "Hello, World!" << std::endl;
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        throw std::runtime_error( "SDL_Init Error: " + std::string( SDL_GetError() ) );
    }

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );


    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    int windowWidthPx = DM.w;
    int windowHeightPx = DM.h;


    SDL_Window *window = SDL_CreateWindow("I can write whatever I want here, and nobody can stop me! HA HA HA",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidthPx,
                                          windowHeightPx,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
    if( window == nullptr)
        throw std::runtime_error( "Window could not be created! SDL Error:" + std::string( SDL_GetError() ) );
    //Create vsynced renderer for window
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if( renderer == nullptr)
    {
        throw std::runtime_error( "Renderer could not be created! SDL Error: "+ std::string(SDL_GetError()) );
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    //Initialize PNG loading
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        throw std::runtime_error( "SDL_image could not initialize! SDL Error:" + std::string( SDL_GetError() ) );
    }

    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        throw std::runtime_error( "SDL_mixer could not initialize! SDL Error:" + std::string( Mix_GetError() ) );
    }


    if (TTF_Init()==-1) {
        throw std::runtime_error( "SDL_TTF could not initialize! SDL Error:" + std::string( SDL_GetError() ) );
    }


    //Load stuff
    texwrap soldierTexture(fs::path("assets")/"soldier.png",renderer);
    texwrap projectileTexture(fs::path("assets")/"bullet.png",renderer);

    std::default_random_engine generator;

    std::list<std::shared_ptr<soldier>> soldiers;

    std::vector<base> bases;

    std::list<projectile> projectiles;

    //Randomly generate bases
    {
        int basesWidth = 6;
        int basesHeight = 3;
        double radius = 0.25*windowWidthPx / (basesWidth-1);
        std::uniform_real_distribution<double> widthDist (windowHeightPx/(basesHeight-1)*0.25,windowWidthPx/(basesWidth-1)*0.75 );
        std::uniform_real_distribution<double> heightDist (windowHeightPx/(basesHeight-1)*0.25,windowHeightPx/(basesHeight-1)*0.75 );
        std::uniform_int_distribution<int> allegiances (0,3);

        for (int j = 0; j < basesHeight-1; j++)
            for (int i = 0; i < basesWidth-1; i++)
                bases.emplace_back(widthDist(generator)+(i+0.5)*windowWidthPx/basesWidth,heightDist(generator)+(j+0.5)*windowHeightPx/basesHeight,radius);
    }


    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()+20,bases[0].getY(),0));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX(),bases[0].getY()+20,0));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX(),bases[0].getY()-20,0));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()-20,bases[0].getY(),0));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()-20,bases[0].getY()-20,0));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()-20,bases[0].getY()+20,0));

    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()+40,bases[0].getY(),1));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX(),bases[0].getY()+40,1));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX(),bases[0].getY()-40,1));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()-40,bases[0].getY(),1));
    soldiers.emplace_back(std::make_shared<soldier>(bases[0].getX()-40,bases[0].getY()-40,1));

    //Run
    bool quit = false;

    unsigned int pmillis=SDL_GetTicks();

    while (!quit) {

        //Milliseconds since program start is preferred time measurement for animations
        unsigned int millis = SDL_GetTicks();
        //MAY be 0 on the very first loop
        double dt =  ( millis - pmillis ) * 0.001;

        //Poll events
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        for (std::shared_ptr<soldier> s: soldiers) {
            //If Soldiers are unassigned, move to the nearest base
            if (s->unassigned()) {
                int nearestBase = -1;
                double nearestDistance = 0;
                for (int i = 0 ; i < bases.size(); i++) {
                    double distance = sqrt(pow(s->getX()-bases[i].getX(),2)+pow(s->getY()-bases[i].getY(),2));
                    if (nearestBase==-1|| distance < nearestDistance) {
                        nearestBase = i;
                        nearestDistance = distance;
                    }
                }
                s->setBase(&bases[nearestBase]);
                bases[nearestBase].addSoldier(s);
            }
            s->update(dt);
 //           s.shoot(projectiles,soldiers,generator,dt);
        }
        for (projectile& projectile : projectiles) {
            projectile.update(dt);
        }
        projectiles.remove_if([](const projectile& projectile) {
            return projectile.isDead();
        });



        //Black background
        SDL_SetRenderDrawColor(renderer, 0x40, 0xF0, 0x40, 0x00);
        SDL_RenderClear( renderer );

        for (const base& base : bases) {
            base.display(renderer);
        }

        for (std::shared_ptr<soldier> soldier : soldiers) {
            soldier->display(renderer,soldierTexture);
        }

        for (projectile& projectile : projectiles) {
            projectile.display(renderer,projectileTexture);
        }

        SDL_RenderPresent( renderer );
        pmillis = millis;
    }



    //deallocate

    if (renderer!=nullptr)
        SDL_DestroyRenderer(renderer);
    if (window!=nullptr)
        SDL_DestroyWindow(window);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();



    return 0;
}
