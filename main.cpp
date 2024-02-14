#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

SDL_Window* window= nullptr;
SDL_Renderer* renderer=nullptr;
SDL_Event keyboard;
int ScreenWrapX (int relposx);
int ScreenWrapY(int relposy);
const int WIDTH = 1400;
const int HEIGHT=890;
const float MOMENTUMCONSTANT=0.01;
const int MAXASS=10;
struct AsteroidParent{
    int assrelposy;
    int assrelposx;
    int assradius;
    int asssides;
   float assmomenty;
    float assmomentx;
    bool assisdead;
    std :: vector<int> chaos;
};
AsteroidParent asteroid[MAXASS];

class PlayerParent
{
    public:
    float theta =0;
    int PlayerRadius=15;
    int PlayerRelPosx;
    int PlayerRelPosy;
    int PlayerMomentumx=1;
    int PlayerMomentumy=1;
    int PlayerCalcPosx(float PlayerMomentumx, int PlayerRelPosx);
    int PlayerCalcPosy(float PlayerMomentumy, int PlayerRelPosy);
    void DrawShip(float theta, int PlayerRelPosx, int PlayerRelPosy, int PlayerRadius );
    bool isdead=false;
};

bool collision(int playerposx,int playerposy ,int playerradius);
int PlayerParent :: PlayerCalcPosx(float PlayerMomentumx, int PlayerRelPosx)
{
PlayerRelPosx +=PlayerMomentumx;
return ScreenWrapX(PlayerRelPosx);
}
int PlayerParent :: PlayerCalcPosy(float PlayerMomentumy, int PlayerRelPosy)
{
PlayerRelPosy +=PlayerMomentumy;
return ScreenWrapY(PlayerRelPosy);
}

void PlayerParent :: DrawShip(float theta, int PlayerRelPosx, int PlayerRelPosy, int PlayerRadius)
{
    int drawx=0;
    int drawy=0;
    std :: vector<SDL_Point> PlayerPoints;
    drawx=PlayerRelPosx+cos(theta*M_PI/180) * 2 * PlayerRadius;
    drawy=PlayerRelPosy+sin(theta*M_PI/180) * 2 * PlayerRadius;
    PlayerPoints.push_back({drawx,drawy}) ;

 drawx=PlayerRelPosx+cos((theta-126) * M_PI/ 180) * PlayerRadius;
    drawy=PlayerRelPosy+sin((theta-126) * M_PI/ 180) * PlayerRadius;
    PlayerPoints.push_back({drawx,drawy});

    
    drawx=PlayerRelPosx+cos((theta+126) * M_PI/180) * PlayerRadius;
    drawy=PlayerRelPosy+sin((theta+126) * M_PI/180) * PlayerRadius;
    PlayerPoints.push_back({drawx,drawy});

    PlayerPoints.push_back(PlayerPoints[0]);
   SDL_RenderDrawLines(renderer, PlayerPoints.data(), PlayerPoints.size());
}

void DrawAsteroid();

void AssInit();



bool truefalse();



int main()
{
//initalize
int relposx =WIDTH/2;
int relposy = HEIGHT/2;

SDL_Init(SDL_INIT_EVERYTHING);
SDL_CreateWindowAndRenderer(WIDTH,HEIGHT,0,&window,&renderer);

SDL_SetRenderDrawColor(renderer,0,0,0,255);
SDL_RenderClear(renderer);

SDL_SetRenderDrawColor(renderer,255,255,255,255);

SDL_RenderSetScale(renderer,1,1);    
//momentums

//player radius for drawing
int radius =10;

//initialize asteroid
AssInit();

// initialize player
PlayerParent Player;
Player.PlayerRelPosx=WIDTH/2;
Player.PlayerRelPosy=HEIGHT/2;
//should add a way to close the game sometime uhhhh gameloop
bool open=true;
while(open)
{
    //clear the window
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderClear(renderer);
if (Player.isdead==true)
{
open = false;
}
while (SDL_PollEvent(&keyboard))
{
    if (keyboard.type ==SDL_KEYDOWN)
    {
        switch (keyboard.key.keysym.sym)
        {
            case SDLK_UP:
                    Player.PlayerMomentumy += 2 * sin(Player.theta * M_PI/180);
                    Player.PlayerMomentumx += 2 *  cos(Player.theta * M_PI/180) ;
                    if ( Player.PlayerMomentumy >10)
                    {
                         Player.PlayerMomentumy=10;
                    }
                    else if (Player.PlayerMomentumy <-10)
                    {
                        Player.PlayerMomentumy =-10;
                    }
                    if (Player.PlayerMomentumx >10 )
                    {
                        Player.PlayerMomentumx=10;
                    }
                    else if (Player.PlayerMomentumx <-10)
                    {
                        Player.PlayerMomentumx =-10;
                    }
            break;
            case SDLK_RIGHT:
            Player.theta-=8;
            break;
            case SDLK_LEFT:
            Player.theta+=8;
            break;
        }
    }
    if (keyboard.type == SDL_QUIT)
    {
        open =false;
    }
}


//update position


//udate ass position
for (int i = 0; i<MAXASS; i++)
{
    asteroid[i].assrelposx += asteroid[i].assmomentx;
    asteroid[i].assrelposy += asteroid[i].assmomenty;
}

Player.PlayerRelPosx = Player.PlayerCalcPosx(Player.PlayerMomentumx, Player.PlayerRelPosx);
Player.PlayerRelPosy = Player.PlayerCalcPosy(Player.PlayerMomentumy, Player.PlayerRelPosy);

//draw the ship

std::cout << "Ship Position: (" << Player.PlayerRelPosx << ", " << Player.PlayerRelPosy << ")" << std::endl;
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
 Player.DrawShip(Player.theta, Player.PlayerRelPosx, Player.PlayerRelPosy, Player.PlayerRadius);


DrawAsteroid();
//render frame
SDL_RenderPresent(renderer);
SDL_Delay(5);

Player.isdead = collision( Player.PlayerRelPosx,Player.PlayerRelPosy ,Player.PlayerRadius);

//wall wrap
Player.PlayerRelPosx = ScreenWrapX(Player.PlayerRelPosx);
Player.PlayerRelPosy = ScreenWrapY(Player.PlayerRelPosy);

//ass wall wrap 
for (int i =0; i<MAXASS; i++)
{
    asteroid[i].assrelposx =ScreenWrapX(asteroid[i].assrelposx);
     asteroid[i].assrelposy =ScreenWrapY(asteroid[i].assrelposy);
}
}
return 0;
}


void DrawAsteroid()
{



int drawx=0;
int drawy =0;

std :: vector<SDL_Point> asspoints;
for (int j=0;j<MAXASS; j++)
{
    int angle= 360/asteroid[j].asssides;
for(int i =0; i<asteroid[j].asssides;i++)
{
drawx= asteroid[j].assrelposx+(cos(angle*i*M_PI/180))* asteroid[j].assradius+asteroid[j].chaos[i];
drawy= asteroid[j].assrelposy+(sin(angle*i*M_PI/180))*asteroid[j].assradius;
asspoints.push_back({drawx,drawy});

}
asspoints.push_back(asspoints[0]);
SDL_RenderDrawLines( renderer, asspoints.data(), asspoints.size());
asspoints.clear();
}

}
void AssInit()
{
    


    bool negative;
    for (int i = 0; i<MAXASS;i++)
    {
        negative= truefalse();
        asteroid[i].assradius =rand()%40+10;
        asteroid[i].asssides =rand()%9+5;
        asteroid[i].assrelposx =rand() %WIDTH;
        asteroid[i].assrelposy =rand() %HEIGHT;
        asteroid[i].assmomentx=rand()%3+-3;
        if (negative)
        {
            asteroid[i].assmomentx =asteroid[i].assmomentx * -1;
        }
        negative= truefalse();
        asteroid[i].assmomenty=rand()%3+-3;
         if (negative)
        {
            asteroid[i].assmomenty =asteroid[i].assmomenty * -1;
        }
       
        
            for (int j=0; j < asteroid[i].asssides; j++)
            {
                if (truefalse())
                {
                asteroid[i].chaos.push_back( rand()% (asteroid[i].assradius / 2 + 1)) ;
                
                }
                else 
                {
                     asteroid[i].chaos.push_back( 0) ;
                }
            }
        
    }
}
int ScreenWrapX(int relposx) {
    if (relposx <= 0) {
        relposx = WIDTH - 1;
    } else if (relposx >= WIDTH) {
        relposx = 1;
    }
    return relposx;
}

int ScreenWrapY(int relposy) {
    if (relposy <= 0) {
        relposy = HEIGHT - 1;
    } else if (relposy >= HEIGHT) {
        relposy = 1;
    }
    return relposy;
}


bool truefalse()
{
    bool maybe=rand()%2==0;
    return maybe;
}

bool collision(int playerposx,int playerposy ,int playerradius)
{
for (int i = 0; i<MAXASS; i ++)
{
    int distancex = asteroid[i].assrelposx-playerposx;
    int distancey =asteroid[i].assrelposy- playerposy;
    int squaredist = distancex * distancex + distancey *distancey;
    int combinedradsquare = (asteroid[i].assradius + playerradius) *(asteroid[i].assradius + playerradius);
    if (combinedradsquare > squaredist)
    {
        return 1;
    }
    
}
return 0;
}