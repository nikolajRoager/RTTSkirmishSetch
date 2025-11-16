#include <iostream>
#include <list>
#include <random>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

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

    std::vector<soldier> soldiers;
    std::list<projectile> projectiles;

    //Randomly generate soldiers
    {
        int widthNodes = 16;
        int heightNodes = 8;
        double neighbourDistance = windowWidthPx*2 / widthNodes;
        std::uniform_real_distribution<double> widthDist (0,0.75*windowWidthPx/widthNodes );
        std::uniform_real_distribution<double> heightDist (0,0.75*windowHeightPx/heightNodes );
        std::uniform_int_distribution<int> allegiences (0,3);

        for (int j = 0; j < heightNodes; j++)
            for (int i = 0; i < widthNodes; i++)
                soldiers.emplace_back(widthDist(generator)+i*windowWidthPx/widthNodes,heightDist(generator)+j*windowHeightPx/heightNodes,allegiences(generator));
    }

    projectiles.emplace_back(5,5,500,1000,1000);

    //Run
    bool quit = false;

    unsigned int pmillis=-1;

    while (!quit) {

        //Milliseconds since program start is preferred time measurement for animations
        unsigned int millis = SDL_GetTicks();
        double dt = pmillis==-1? 0.0 : ( millis - pmillis ) * 0.001;

        //Poll events
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        for (soldier& s: soldiers) {
            s.shoot(projectiles,soldiers,generator,dt);
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

        for (soldier& soldier : soldiers) {
            soldier.display(renderer,soldierTexture);
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
