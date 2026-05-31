
#include "CSFML/Graphics.h"
#include <stdlib.h>
#include <math.h>

#define SCREENWIDTH 1600
#define SCREENHEIGHT 900
#define PI 3.14159265f

float mazeRotationAngle = 15.0f;
float mazeTiltRatio = 0.7f;
float mazeScale = 1.4f;
float mazeHeight = 15.0f;
float mazePosX = 400.0f;
float mazePosY = 400.0f;

float pathSize = 0.3f;
void setupmazegui();
void updatemazegui();
void rendermaze(sfRenderWindow*);
sfVector2f transformpos(float, float);
sfVector2f transformrect(int, int);

int maze[13][13] = {
                        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
                        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
                        {-1, -1,  0,  0, 0,  0, 0,  0, 0,  0,  0, -1, -1},
                        {-1, -1,  0, -1, -1, -1, 0, -1, 0, -1,  0, -1, -1},
                        { 0,  0,  0,  -1, 0,  0, 0,  0, 0,  0,  0,  0,  0},
                        {-1, -1,  -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1},
                        { 0,  0,  0,  0, 0,  1, 1,  0, 0,  -1,  0,  0,  0},
                        {-1, -1,  0, -1, -1, -1, 1, -1, 0, -1,  0, -1, -1},
                        { 0,  0,  0,  -1, 0,  1, 1,  0, 0,  0,  0,  1,  0},
                        {-1, -1,  0, -1, 0, -1,1, -1, 0, -1,  0, -1, -1},
                        {-1, -1,  0,  -1, 0,  0, 0,  0, 0,  0,  0, -1, -1},
                        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1},
                        {-1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -1, -1, -1}
};
sfConvexShape* rects[13][13][3];
sfConvexShape* routeRects[13][13][5];
//sfVertexArray* rects[13][13][3];



int main(void)
{
    const sfVideoMode mode   = {{SCREENWIDTH, SCREENHEIGHT}, 32};
    sfRenderWindow*   window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, sfWindowed, NULL);
    if (!window) return EXIT_FAILURE;


    setupmazegui();

    sfEvent event;
    while (sfRenderWindow_isOpen(window))
    {
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        mazeRotationAngle+=0.03f;
        if(mazeRotationAngle>360.0f) mazeRotationAngle-=360.0f; 
        updatemazegui();
        sfRenderWindow_clear(window, sfBlack);
        rendermaze(window);
        sfRenderWindow_display(window);
    }


    sfRenderWindow_destroy(window);
    return EXIT_SUCCESS;
}


void setupmazegui(){ //Gets called once at start of program.
int i, j, k;
for(i=0; i<13; i++){
    for(j=0; j<13; j++){
        for(k=0; k<5; k++){
            routeRects[i][j][k] = sfConvexShape_create();
            sfConvexShape_setPointCount(routeRects[i][j][k], 4);
            sfConvexShape_setFillColor(routeRects[i][j][k], sfColor({0,255,0,255}));
        }
        for(k=0; k<3; k++){
            rects[i][j][k] = sfConvexShape_create();
            sfConvexShape_setPointCount(rects[i][j][k], 4);
        }
    }
}
}

void updatemazegui(){ //Gets called every frame to update the visuals.
int i, j,k;
sfVector2f pos, side1,side2;
sfVertex vertices[6];
for(i=0; i<13; i++){
    for(j=0; j<13; j++){
        pos = transformpos((float)i, (float)j);
        side1 = transformrect(0,0);
        side2 = transformrect(1,0);

        //Roof
        sfConvexShape_setPosition(rects[i][j][0], pos);
        sfConvexShape_setPoint(rects[i][j][0], 0, {0.0f,0.0f});
        sfConvexShape_setPoint(rects[i][j][0], 1, {side1.x,side1.y});
        sfConvexShape_setPoint(rects[i][j][0], 3, {side2.x,side2.y});
        sfConvexShape_setPoint(rects[i][j][0], 2, {side1.x+side2.x,side1.y+side2.y});

        //Walls coordinates depend on rotation angle.
        if(mazeRotationAngle>270.0f){
            pos = transformpos((float)(i+1), (float)j);
        }else if(mazeRotationAngle>180.0f){
            pos = transformpos((float)i, (float)j);
        }else if(mazeRotationAngle>90.0f){
            pos = transformpos((float)i, (float)(j+1));
        }else{
            pos = transformpos((float)(i+1), (float)(j+1));
        }
        side1 = transformrect(0,1);
        side2 = transformrect(1,1);

        //Wall 1
        sfConvexShape_setPosition(rects[i][j][1], pos);
        sfConvexShape_setPoint(rects[i][j][1], 0, {0.0f,0.0f});
        sfConvexShape_setPoint(rects[i][j][1], 1, {-1*side1.x,-1*side1.y});
        sfConvexShape_setPoint(rects[i][j][1], 3, {0.0f,mazeScale*mazeHeight*mazeTiltRatio});
        sfConvexShape_setPoint(rects[i][j][1], 2, {-1*side1.x,-1*side1.y+mazeScale*mazeHeight*mazeTiltRatio});
        //Wall2
        sfConvexShape_setPosition(rects[i][j][2], pos);
        sfConvexShape_setPoint(rects[i][j][2], 0, {0.0f,0.0f});
        sfConvexShape_setPoint(rects[i][j][2], 1, {-1*side2.x,-1*side2.y});
        sfConvexShape_setPoint(rects[i][j][2], 3, {0.0f,mazeScale*mazeHeight*mazeTiltRatio});
        sfConvexShape_setPoint(rects[i][j][2], 2, {-1*side2.x,-1*side2.y+mazeScale*mazeHeight*mazeTiltRatio});
        //Colors
        if(maze[i][j] == -1){
            sfConvexShape_setFillColor(rects[i][j][0], sfColor({255,0,100,255}));
            sfConvexShape_setFillColor(rects[i][j][1], sfColor({160,0,70,255}));
            sfConvexShape_setFillColor(rects[i][j][2], sfColor({210,0,80, 255}));
        } else {
            sfConvexShape_setFillColor(rects[i][j][0], sfColor({255,0,0,0}));
            sfConvexShape_setFillColor(rects[i][j][1], sfColor({0,0,0,0}));
            sfConvexShape_setFillColor(rects[i][j][2], sfColor({0,0,0,0}));
            
        }




        if(maze[i][j] == 1){
            pos = transformpos((float)i+0.4f, (float)j +0.4f);
            side1 = transformrect(0,0);
            side2 = transformrect(1,0);
            for(k=0; k<5; k++){
                sfConvexShape_setPosition(routeRects[i][j][k], {pos.x, pos.y+mazeScale*mazeHeight*mazeTiltRatio});
            }
            sfConvexShape_setPoint(routeRects[i][j][0], 0,{0.0f,0.0f});
            sfConvexShape_setPoint(routeRects[i][j][0], 1,{side1.x*pathSize, side1.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][0], 3,{side2.x*pathSize, side2.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][0], 2,{side2.x*pathSize+side1.x*pathSize, side2.y*pathSize+side1.y*pathSize});

            sfConvexShape_setPoint(routeRects[i][j][1], 0,{side1.x*pathSize,side1.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][1], 1,{side1.x*(0.5+0.5f*pathSize), side1.y*(0.5+0.5f*pathSize)});
            sfConvexShape_setPoint(routeRects[i][j][1], 3,{side2.x*pathSize+side1.x*pathSize, side2.y*pathSize+side1.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][1], 2,{side2.x*pathSize+side1.x*(0.5+0.5f*pathSize), side2.y*pathSize+side1.y*(0.5+0.5f*pathSize)});

            sfConvexShape_setPoint(routeRects[i][j][2], 0,{side2.x*pathSize,side2.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][2], 1,{side2.x*(0.5+0.5f*pathSize), side2.y*(0.5+0.5f*pathSize)});
            sfConvexShape_setPoint(routeRects[i][j][2], 3,{side1.x*pathSize+side2.x*pathSize, side1.y*pathSize+side2.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][2], 2,{side1.x*pathSize+side2.x*(0.5+0.5f*pathSize), side1.y*pathSize+side2.y*(0.5+0.5f*pathSize)});

            sfConvexShape_setPoint(routeRects[i][j][3], 0,{0.0f,0.0f});
            sfConvexShape_setPoint(routeRects[i][j][3], 1,{side2.x*(-0.5+0.5f*pathSize), side2.y*(-0.5+0.5f*pathSize)});
            sfConvexShape_setPoint(routeRects[i][j][3], 3,{side1.x*pathSize, side1.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][3], 2,{side1.x*pathSize+side2.x*(-0.5+0.5f*pathSize), side1.y*pathSize+side2.y*(-0.5+0.5f*pathSize)});
            
            sfConvexShape_setPoint(routeRects[i][j][4], 0,{0.0f,0.0f});
            sfConvexShape_setPoint(routeRects[i][j][4], 1,{side1.x*(-0.5+0.5f*pathSize), side1.y*(-0.5+0.5f*pathSize)});
            sfConvexShape_setPoint(routeRects[i][j][4], 3,{side2.x*pathSize, side2.y*pathSize});
            sfConvexShape_setPoint(routeRects[i][j][4], 2,{side2.x*pathSize+side1.x*(-0.5+0.5f*pathSize), side2.y*pathSize+side1.y*(-0.5+0.5f*pathSize)});
            
            if(maze[i][j+1] == 1){
                sfConvexShape_setFillColor(routeRects[i][j][1], sfColor({0,255,0,255}));
            }else{
                sfConvexShape_setFillColor(routeRects[i][j][1], sfColor({0,0,0,100}));
            }

            if(maze[i+1][j] == 1){
                sfConvexShape_setFillColor(routeRects[i][j][2], sfColor({0,255,0,255}));
            }else{
                sfConvexShape_setFillColor(routeRects[i][j][2], sfColor({0,0,0,100}));
            }

            if(maze[i-1][j] == 1){
                sfConvexShape_setFillColor(routeRects[i][j][3], sfColor({0,255,0,255}));
            }else{
                sfConvexShape_setFillColor(routeRects[i][j][3], sfColor({0,0,0,100}));
            }

            if(maze[i][j-1] == 1){
                sfConvexShape_setFillColor(routeRects[i][j][4], sfColor({0,255,0,255}));
            }else{
                sfConvexShape_setFillColor(routeRects[i][j][4], sfColor({0,0,0,100}));
            }


    }
}
}
}


sfVector2f transformpos(float y, float x){ //gets x and y integers and then gives appropriate coordinates using scaling rotation matrix.
    sfVector2f scaledPos;
    sfVector2f pos;
    x-=6;
    y-=6;
    float xf, yf;
    xf = (float)x-0.5f;
    yf = (float)y-0.5f;
    scaledPos.x = 30.0f*mazeScale*xf;
    scaledPos.y = 30.0f*mazeScale*yf;
    pos.x=scaledPos.x*cos(mazeRotationAngle*PI/180.0f) + scaledPos.y*-1*sin(mazeRotationAngle*PI/180.0f);
    pos.y=scaledPos.x*sin(mazeRotationAngle*PI/180.0f) + scaledPos.y*cos(mazeRotationAngle*PI/180.0f);
    pos.y*=mazeTiltRatio;
    pos.y-=mazeHeight*mazeScale;
    pos.x+=mazePosX;
    pos.y+=mazePosY;
    return pos;
}

sfVector2f transformrect(int side, int mode){  //This function receives the side of the rectangle (0 or 1) and the mode 
    //(0 for roof, 1 for wall). It then gives the wall coordinates relative to the block corner. It also scales with the mazeScale and tilts. 
    sfVector2f scaledPos;
    sfVector2f pos;
    float angle = mazeRotationAngle;
    if(mode){
        while(angle>90.0f) angle-=90.0f;
    }
    scaledPos.x = 30.0f*mazeScale;
    scaledPos.y = 30.0f*mazeScale;
    if(side==0){
    pos.x=scaledPos.x*cos(angle*PI/180.0f);
    pos.y=scaledPos.x*sin(angle*PI/180.0f);
    pos.y*=mazeTiltRatio;
    }else{
    pos.x=scaledPos.x*-1*sin(angle*PI/180.0f);
    pos.y=scaledPos.x*cos(angle*PI/180.0f);
    pos.y*=mazeTiltRatio;
    }
    return pos;
}

void rendermaze(sfRenderWindow* window){
    int i, j, k;
    for(i=0; i<13; i++){
    for(j=0; j<13; j++){
        if (maze[i][j] == 1){ //route
            for(k=0; k<5; k++){
                sfRenderWindow_drawConvexShape(window, routeRects[i][j][k], NULL);
            }
        }
    }}
    for(i=0; i<13; i++){
        for(j=0; j<13; j++){

            for(k=0; k<3; k++){//3 walls
                if(mazeRotationAngle>270.0f){//To draw walls far away frst shit.
                sfRenderWindow_drawConvexShape(window, rects[i][12-j][k], NULL);
                }else if(mazeRotationAngle>180.0f){
                sfRenderWindow_drawConvexShape(window, rects[12-i][12-j][k], NULL);
                }else if(mazeRotationAngle>90.0f){
                sfRenderWindow_drawConvexShape(window, rects[12-i][j][k], NULL);
                }else{
                sfRenderWindow_drawConvexShape(window, rects[i][j][k], NULL);
                }
            
            }
        }
    }
    }
