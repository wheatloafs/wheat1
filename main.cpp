#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <algorithm>
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
SDL_Event keyboard;
int ScreenWrapX(int relposx);
int ScreenWrapY(int relposy);
const int WIDTH = 1400;
const int HEIGHT = 890;
const float MOMENTUMCONSTANT = 0.01;
int MAXASS = 4;
const int NUMBERSIZE = 10;
const int BULLETSIZE = 5;
const int MAXBULLET = 10;
const int MAXPREG = 3;
bool truefalse();
void DrawAsteroid();
void menu();
void Gratify(int startposx, int startposy);
void DisplayScore();
void lifecounter(int lives, int playerRadius);
class AsteroidParent
{
public:
    int assrelposy;
    int assrelposx;
    int assradius;
    int asssides;
    float assmomenty;
    float assmomentx;
    bool assisdead;
    bool assispreg;
    std ::vector<int> chaos;
    AsteroidParent(bool preg, int radius, int posx, int posy)
    {
        assisdead = false;
        bool negative;
        assradius = radius;
        asssides = rand() % 9 + 5;
        assrelposx = posx;
        assrelposy = posy;
        assmomentx = 2;
        negative = truefalse();
        if (negative)
        {
            assmomentx = assmomentx * -1;
        }
        negative = truefalse();
        assmomenty = 2;
        if (negative)
        {
            assmomenty = assmomenty * -1;
        }
        for (int j = 0; j < asssides; j++)
        {
            if (truefalse())
            {
                chaos.push_back(rand() % (assradius / 2 + 1));
            }
            else
            {
                chaos.push_back(0);
            }
        }
        assispreg = preg;
    }
};
std ::vector<AsteroidParent> asteroid;

class PlayerParent
{
public:
    float theta = 0;
    int PlayerRadius = 15;
    int PlayerRelPosx;
    int PlayerRelPosy;
    int PlayerMomentumx = 0;
    int PlayerMomentumy = 0;
    int PlayerCalcPosx(float PlayerMomentumx, int PlayerRelPosx);
    int PlayerCalcPosy(float PlayerMomentumy, int PlayerRelPosy);
    void DrawShip(float theta, int PlayerRelPosx, int PlayerRelPosy, int PlayerRadius);
    bool isdead = false;
    int lives = 3;
    int invuln = 0;
};
class bulletparent
{
public:
    float bulletmomentumx;
    float bulletmomentumy;
    int posx;
    int posy;
    int life = 600;
    bulletparent(int theta, int playerposx, int playerposy, int PlayerMomentumx, int PlayerMomentumy)
    {
        bulletmomentumx = 8 * cos(theta * M_PI / 180);
        bulletmomentumy = 8 * sin(theta * M_PI / 180);
        posx = playerposx + bulletmomentumx; // Initialize position with player position
        posy = playerposy + bulletmomentumy;
        if (bulletmomentumy > 20)
        {
            bulletmomentumy = 20;
        }
        else if (bulletmomentumy < -20)
        {
            bulletmomentumy = -20;
        }
        if (bulletmomentumx > 20)
        {
            bulletmomentumx = 20;
        }
        else if (bulletmomentumx < -20)
        {
            bulletmomentumx = -20;
        }
    }
};
std::vector<bulletparent> bullets;
bool collision(int playerposx, int playerposy, int playerradius);
int PlayerParent ::PlayerCalcPosx(float PlayerMomentumx, int PlayerRelPosx)
{
    PlayerRelPosx += PlayerMomentumx;
    return ScreenWrapX(PlayerRelPosx);
}
int PlayerParent ::PlayerCalcPosy(float PlayerMomentumy, int PlayerRelPosy)
{
    PlayerRelPosy += PlayerMomentumy;
    return ScreenWrapY(PlayerRelPosy);
}

void PlayerParent ::DrawShip(float theta, int PlayerRelPosx, int PlayerRelPosy, int PlayerRadius)
{
    int drawx = 0;
    int drawy = 0;
    std ::vector<SDL_Point> PlayerPoints;
    drawx = PlayerRelPosx + cos(theta * M_PI / 180) * 2 * PlayerRadius;
    drawy = PlayerRelPosy + sin(theta * M_PI / 180) * 2 * PlayerRadius;
    PlayerPoints.push_back({drawx, drawy});

    drawx = PlayerRelPosx + cos((theta - 126) * M_PI / 180) * PlayerRadius;
    drawy = PlayerRelPosy + sin((theta - 126) * M_PI / 180) * PlayerRadius;
    PlayerPoints.push_back({drawx, drawy});

    drawx = PlayerRelPosx + cos((theta + 126) * M_PI / 180) * PlayerRadius;
    drawy = PlayerRelPosy + sin((theta + 126) * M_PI / 180) * PlayerRadius;
    PlayerPoints.push_back({drawx, drawy});

    PlayerPoints.push_back(PlayerPoints[0]);
    SDL_RenderDrawLines(renderer, PlayerPoints.data(), PlayerPoints.size());
}
class gratifier
{
public:
    int life = 0;
    int posx = 0;
    int posy = 0;
    gratifier(int inposx, int inposy)
    {
        posx = inposx;
        posy = inposy;
        life = 200;
    }
};

void AssInit();

void DrawBullet(int posx, int posy);

bool collisionbullet(int playerposx, int playerposy, int playerradius, int i);

const int HighScore = 6;
int score[HighScore] = {0, 0, 0, 0, 0, 0};

int main()
{
    // initalize
    int relposx = WIDTH / 2;
    int relposy = HEIGHT / 2;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderSetScale(renderer, 1, 1);
 std ::vector<SDL_Point> stars;
    // player radius for drawing
    int radius = 10;
for (int i=0; i<400; i++)
{
stars.push_back({rand()%WIDTH, rand()% HEIGHT});
}
    std ::vector<gratifier> popups;

    // initialize player
    PlayerParent Player;
    Player.PlayerRelPosx = WIDTH / 2;
    Player.PlayerRelPosy = HEIGHT / 2;
    bool spaced = 1;
    bool open = true;
    while (spaced == true)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        menu();
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawPoints(renderer, stars.data(), stars.size());
        SDL_RenderPresent(renderer);
        SDL_Delay(5);

        while (SDL_PollEvent(&keyboard))
        {

            if (keyboard.type == SDL_KEYDOWN)
            {
                switch (keyboard.key.keysym.sym)
                {
                case SDLK_SPACE:
                    spaced = false;
                }
            }
        }
    }

    do
    {

        for (int i = 0; i < MAXASS; i++)
        {
            AsteroidParent newasteroid(true, rand() % 40 + 20, rand() % WIDTH, rand() % HEIGHT);
            asteroid.push_back(newasteroid);
        }

        while (asteroid.size() > 0 && open)
        {
            // clear the window
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (Player.isdead == true)
            {
                Player.lives--;
                Player.PlayerRelPosx = WIDTH / 2;
                Player.PlayerRelPosy = HEIGHT / 2;
                Player.PlayerMomentumx = 0;
                Player.PlayerMomentumy = 0;
                Player.isdead = false;
                if (Player.lives == 0)
                {
                    open = false;
                }
            }
            bool spaced = 0;
            while (SDL_PollEvent(&keyboard))
            {

                if (keyboard.type == SDL_KEYDOWN)
                {
                    switch (keyboard.key.keysym.sym)
                    {
                    case SDLK_SPACE:

                        if (bullets.size() < MAXBULLET && !spaced)
                        {
                            spaced = true;

                            bulletparent NewBullet(Player.theta, Player.PlayerRelPosx, Player.PlayerRelPosy, Player.PlayerMomentumx, Player.PlayerMomentumy);
                            bullets.push_back(NewBullet);
                        }

                        break;
                    case SDLK_UP:
                        Player.PlayerMomentumy += 2 * sin(Player.theta * M_PI / 180);
                        Player.PlayerMomentumx += 2 * cos(Player.theta * M_PI / 180);
                        if (Player.PlayerMomentumy > 10)
                        {
                            Player.PlayerMomentumy = 10;
                        }
                        else if (Player.PlayerMomentumy < -10)
                        {
                            Player.PlayerMomentumy = -10;
                        }
                        if (Player.PlayerMomentumx > 10)
                        {
                            Player.PlayerMomentumx = 10;
                        }
                        else if (Player.PlayerMomentumx < -10)
                        {
                            Player.PlayerMomentumx = -10;
                        }
                        break;
                    case SDLK_RIGHT:
                        Player.theta += 8;
                        break;
                    case SDLK_LEFT:
                        Player.theta -= 8;
                        break;
                    }
                }
                if (keyboard.type == SDL_QUIT)
                {
                    open = false;
                }
                if (keyboard.type = SDL_KEYUP)
                {
                    switch (keyboard.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                        spaced = true;
                        break;
                    }
                }
            }

            // update assisdead
            for (int i = 0; i < bullets.size(); i++)
            {
                for (int j = 0; j < asteroid.size(); j++)
                {
                    if (!asteroid[j].assisdead && collisionbullet(bullets[i].posx, bullets[i].posy, BULLETSIZE, j))
                    {
                        asteroid[j].assisdead = true;
                        bullets[i].life = 0;
                        if (asteroid[j].assispreg)
                        {
                            for (int i = 0; i < MAXPREG; i++)
                            {
                                AsteroidParent newass(false, asteroid[j].assradius / 2, asteroid[j].assrelposx, asteroid[j].assrelposy);
                                asteroid.push_back(newass);
                            }
                        }
                        score[5] += 20;

                        gratifier newpopup(asteroid[j].assrelposx, asteroid[j].assrelposy);
                        popups.push_back(newpopup);
                        break;
                    }
                }
            }
            asteroid.erase(
                std::remove_if(
                    asteroid.begin(), asteroid.end(),
                    [](AsteroidParent a)
                    { return a.assisdead; }),
                asteroid.end());
            // udate ass position
            for (int i = 0; i < asteroid.size(); i++)
            {

                asteroid[i].assrelposx += asteroid[i].assmomentx;
                asteroid[i].assrelposy += asteroid[i].assmomenty;
            }

            Player.PlayerRelPosx = Player.PlayerCalcPosx(Player.PlayerMomentumx, Player.PlayerRelPosx);
            Player.PlayerRelPosy = Player.PlayerCalcPosy(Player.PlayerMomentumy, Player.PlayerRelPosy);

            for (int i = 0; i < bullets.size(); i++)
            {
                bullets[i].posx += bullets[i].bulletmomentumx;
                bullets[i].posy += bullets[i].bulletmomentumy;
                DrawBullet(bullets[i].posx, bullets[i].posy);
                bullets[i].life--;
                if (bullets[i].life <= 0)
                {
                    bullets.erase(bullets.begin() + i);
                }
            }
            for (int i = 0; i < popups.size(); i++)
            {

                popups[i].life -= 1;

                if (popups[i].life == 0)
                {
                    popups.erase(popups.begin() + i);
                }
            }
             SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_RenderDrawPoints(renderer, stars.data(), stars.size());
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            // draw the ship
            
            for (int i = 0; i < popups.size(); i++)
            {

                Gratify(popups[i].posx, popups[i].posy);
            }
            if (Player.invuln == 0)
            {
                Player.DrawShip(Player.theta, Player.PlayerRelPosx, Player.PlayerRelPosy, Player.PlayerRadius);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                Player.DrawShip(Player.theta, Player.PlayerRelPosx, Player.PlayerRelPosy, Player.PlayerRadius);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            DisplayScore();

            DrawAsteroid();
            // render frame
            for (int i = 0; i < bullets.size(); i++)
            {
                DrawBullet(bullets[i].posx, bullets[i].posy);
            }
            lifecounter(Player.lives, Player.PlayerRadius);
            SDL_RenderPresent(renderer);
            SDL_Delay(5);

            if (Player.invuln == 0)
            {
                Player.isdead = collision(Player.PlayerRelPosx, Player.PlayerRelPosy, Player.PlayerRadius);
                if (collision(Player.PlayerRelPosx, Player.PlayerRelPosy, Player.PlayerRadius))
                {
                    Player.invuln = 100;
                }
            }
            if (Player.invuln > 0)
            {
                Player.invuln--;
            }

            // wall wrap
            Player.PlayerRelPosx = ScreenWrapX(Player.PlayerRelPosx);
            Player.PlayerRelPosy = ScreenWrapY(Player.PlayerRelPosy);

            for (int i = 0; i < bullets.size(); i++)
            {
                bullets[i].posx = ScreenWrapX(bullets[i].posx);
                bullets[i].posy = ScreenWrapY(bullets[i].posy);
            }
            // ass wall wrap
            for (int i = 0; i < MAXASS; i++)
            {
                asteroid[i].assrelposx = ScreenWrapX(asteroid[i].assrelposx);
                asteroid[i].assrelposy = ScreenWrapY(asteroid[i].assrelposy);
            }
        }
        score[2] += 1;
        MAXASS++;
        Player.lives++;
    } while (open);

    return 0;
}

void DrawAsteroid()
{

    int drawx = 0;
    int drawy = 0;

    std ::vector<SDL_Point> asspoints;
    for (int j = 0; j < asteroid.size(); j++)
    {
        int angle = 360 / asteroid[j].asssides;
        for (int i = 0; i < asteroid[j].asssides; i++)
        {
            drawx = asteroid[j].assrelposx + (cos(angle * i * M_PI / 180)) * asteroid[j].assradius + asteroid[j].chaos[i];
            drawy = asteroid[j].assrelposy + (sin(angle * i * M_PI / 180)) * asteroid[j].assradius;
            asspoints.push_back({drawx, drawy});
        }
        asspoints.push_back(asspoints[0]);
        SDL_RenderDrawLines(renderer, asspoints.data(), asspoints.size());
        asspoints.clear();
    }
}

int ScreenWrapX(int relposx)
{
    if (relposx <= 0)
    {
        relposx = WIDTH - 1;
    }
    else if (relposx >= WIDTH)
    {
        relposx = 1;
    }
    return relposx;
}

int ScreenWrapY(int relposy)
{
    if (relposy <= 0)
    {
        relposy = HEIGHT - 1;
    }
    else if (relposy >= HEIGHT)
    {
        relposy = 1;
    }
    return relposy;
}

bool truefalse()
{
    bool maybe = rand() % 2 == 0;
    return maybe;
}

bool collision(int playerposx, int playerposy, int playerradius)
{
    for (int i = 0; i < asteroid.size(); i++)
    {
        int distancex = asteroid[i].assrelposx - playerposx;
        int distancey = asteroid[i].assrelposy - playerposy;
        int squaredist = distancex * distancex + distancey * distancey;
        int combinedradsquare = (asteroid[i].assradius + playerradius) * (asteroid[i].assradius + playerradius);
        if (combinedradsquare >= squaredist)
        {
            return 1;
        }
    }
    return 0;
}
void DrawBullet(int posx, int posy)
{
    std ::vector<SDL_Point> bulletpoints;
    int drawx, drawy;
    drawx = posx - BULLETSIZE;
    drawy = posy;

    bulletpoints.push_back({drawx, drawy});
    drawx = posx;

    drawy = posy - BULLETSIZE;
    bulletpoints.push_back({drawx, drawy});
    drawx = posx + BULLETSIZE;
    drawy = posy;
    bulletpoints.push_back({drawx, drawy});
    drawx = posx;
    drawy = posy + BULLETSIZE;
    bulletpoints.push_back({drawx, drawy});
    bulletpoints.push_back({bulletpoints[0]});
    SDL_RenderDrawLines(renderer, bulletpoints.data(), bulletpoints.size());
}
bool collisionbullet(int playerposx, int playerposy, int playerradius, int i)
{
    int distancex = asteroid[i].assrelposx - playerposx;
    int distancey = asteroid[i].assrelposy - playerposy;
    int squaredist = distancex * distancex + distancey * distancey;
    int combinedradsquare = (asteroid[i].assradius + playerradius) * (asteroid[i].assradius + playerradius);
    if (combinedradsquare >= squaredist)
    {
        return true;
    }
    return false;
}

void DisplayScore()
{
    int startposx = 30;
    int startposy = 30;
    int val = 0;
    std ::vector<SDL_Point> numberpoints;
    for (int i = 0; i < HighScore; i++)
    {
        numberpoints.clear();
        if (i > 0)
        {
            while (score[i] > 9)
            {
                score[i] -= 10;
                score[i - 1] += 1;
            }
        }
        val = score[i];
        switch (val)
        {
        case 0:
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy});

            break;
        case 1:
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});

            break;
        case 2:

            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            break;
        case 3:
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            break;
        case 4:
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy});
            break;
        case 5:
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            break;
        case 6:
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            break;
        case 7:
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1), startposy});
            break;
        case 8:
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            break;
        case 9:
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx * (i + 1), startposy});
            numberpoints.push_back({startposx * (i + 1), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx * (i + 1) + NUMBERSIZE, startposy + NUMBERSIZE});
            break;
        }
        SDL_RenderDrawLines(renderer, numberpoints.data(), numberpoints.size());
    }
}
void lifecounter(int lives, int PlayerRadius)
{

    int drawx = 0;
    int drawy = 0;
    int theta = 270;
    int PlayerRelPosx = 50;
    int PlayerRelPosy = 90;
    for (int i = 0; i < lives; i++)
    {
        PlayerRelPosx = 50 * (i + 1);

        std ::vector<SDL_Point> PlayerPoints;
        drawx = PlayerRelPosx + cos(theta * M_PI / 180) * 2 * PlayerRadius;
        drawy = PlayerRelPosy + sin(theta * M_PI / 180) * 2 * PlayerRadius;
        PlayerPoints.push_back({drawx, drawy});

        drawx = PlayerRelPosx + cos((theta - 126) * M_PI / 180) * PlayerRadius;
        drawy = PlayerRelPosy + sin((theta - 126) * M_PI / 180) * PlayerRadius;
        PlayerPoints.push_back({drawx, drawy});

        drawx = PlayerRelPosx + cos((theta + 126) * M_PI / 180) * PlayerRadius;
        drawy = PlayerRelPosy + sin((theta + 126) * M_PI / 180) * PlayerRadius;
        PlayerPoints.push_back({drawx, drawy});

        PlayerPoints.push_back(PlayerPoints[0]);
        SDL_RenderDrawLines(renderer, PlayerPoints.data(), PlayerPoints.size());
    }
}
void Gratify(int startposx, int startposy)
{
    std ::vector<SDL_Point> numberpoints;

    for (int i = 0; i <= 1; i++)
    {
        numberpoints.clear();
        if (i == 0)
        {
            numberpoints.push_back({startposx + (i * 20), startposy});
            numberpoints.push_back({startposx + (i * 20) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx + (i * 20) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx + (i * 20), startposy + NUMBERSIZE});
            numberpoints.push_back({startposx + (i * 20), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx + (i * 20) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
        }
        else
        {
            numberpoints.push_back({startposx + (i * 20), startposy});
            numberpoints.push_back({startposx + (i * 20) + NUMBERSIZE, startposy});
            numberpoints.push_back({startposx + (i * 20) + NUMBERSIZE, startposy + NUMBERSIZE});
            numberpoints.push_back({startposx + (i * 20) + NUMBERSIZE, startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx + (i * 20), startposy + 2 * NUMBERSIZE});
            numberpoints.push_back({startposx + (i * 20), startposy});
        }

        SDL_RenderDrawLines(renderer, numberpoints.data(), numberpoints.size());
    }
}
void menu()
{
std ::vector<SDL_Point> menupoints;
int posx = (WIDTH / 2)-200;
int posy = (HEIGHT / 2 )-90;
int stepsize=20;
int i =1;
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+(stepsize ),posy- (2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+( 2 *stepsize ),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
i+=2;
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
menupoints.clear();
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(stepsize ),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+( 2 *stepsize ),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize)+( 2 *stepsize ),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+( 2 *stepsize ),posy});
menupoints.push_back({posx+(i * stepsize),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(stepsize ),posy});
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+( 2 *stepsize ),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(stepsize),posy- ( 2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize)+(stepsize ),posy});
menupoints.push_back({posx+(i * stepsize),posy});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();
i+=2;
SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
menupoints.push_back({posx+(i * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy-stepsize});
menupoints.push_back({posx+(i * stepsize),posy-(2 * stepsize)});
menupoints.push_back({posx+(i * stepsize)+(2 * stepsize),posy- ( 2 * stepsize)});
SDL_RenderDrawLines(renderer, menupoints.data(), menupoints.size());
menupoints.clear();

}