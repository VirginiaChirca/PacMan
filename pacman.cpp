#include "lab_m1/PacMan/pacman.h"

#include <vector>
#include <iostream>
#include <cmath>
#include <math.h> 
#include <stdio.h>
#include <time.h>
#include <chrono>
#include "lab_m1/PacMan/transform2D-pacman.h"
#include "lab_m1/PacMan/object2D-pacman.h"

using namespace std;
using namespace m1;


#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

#define M_PI 3.1415926535897932384

PacMan::PacMan()
{
}


PacMan::~PacMan()
{
}


void PacMan::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 16;   // logic width
    logicSpace.height = 9;  // logic height

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // pozitie player
    posplayerX = 1.75;
    posplayerY = 1.7;
    // Initialize angularStep
    angularStep = 0.f;
    player_speed = 3;
    // CADRUL
    Mesh* map = object2D::CreateRectangle("map", corner, 7.6, 15.25, glm::vec3(0, 0, 0), true);
    AddMeshToList(map);
    nr = 0;
    nrg1 = 0;
    nrg2 = 0;
    moveW = false;
    moveA = false;
    moveS = false;
    moveD = false;
    check = false;
    posghost1x = 0;
    posghost2x = 0;
    posghost3x = 0;
    posghost4x = 0;
    
    //OBSTACOL
    Mesh* obstacle = object2D::CreateRectangle("obstacle", corner, 0.8, 0.8, glm::vec3(0.372549, 0.623529, 0.623529), true);
    AddMeshToList(obstacle);
    //PLAYER
    Mesh* player = object2D::CreateCircle("player", 30, 0.25, 0.25, glm::vec3(1, 1, 0));
    AddMeshToList(player);
    Mesh* player_rect = object2D::CreateRectangle("player_rect", corner, 0.6, 0.6, glm::vec3(0,0,0), true);
    AddMeshToList(player_rect);
    Mesh* player_mouth = object2D::CreateTriangle("player_mouth", corner, 0.5, glm::vec3(0, 0, 0), true);
    AddMeshToList(player_mouth);
    Mesh* food_rect = object2D::CreateRectangle("food_rect", corner, 0.3, 0.3, glm::vec3(0, 0, 0), true);
    AddMeshToList(food_rect);
    //FOOD
    Mesh* food = object2D::CreateCircle("food", 20, 0.07, 0.07, glm::vec3( 0.439216, 0.576471, 0.858824));
    AddMeshToList(food);
    Mesh* special_food = object2D::CreateCircle("special_food", 20, 0.2, 0.2, glm::vec3(0.576471, 0.858824, 0.439216));
    AddMeshToList(special_food);
    //GHOSTS
    Mesh* ghost1 = object2D::CreateCircle("ghost1", 20, 0.18, 0.30, glm::vec3(0.89, 0.47, 0.20));
    AddMeshToList(ghost1);
    Mesh* ghost2 = object2D::CreateCircle("ghost2", 20, 0.18, 0.30, glm::vec3(0.85, 0.85, 0.95));
    AddMeshToList(ghost2);
    Mesh* ghost3 = object2D::CreateCircle("ghost3", 20, 0.18, 0.30, glm::vec3(0.8, 0.196078, 0.6));
    AddMeshToList(ghost3);
    Mesh* ghost4 = object2D::CreateCircle("ghost4", 20, 0.18, 0.30, glm::vec3(0.30, 0.30, 1.00));
    AddMeshToList(ghost4);
    Mesh* rect_ghost1 = object2D::CreateRectangle("rect_ghost1", corner, 0.2, 0.4, glm::vec3(0, 0, 0), true);
    AddMeshToList(rect_ghost1);
    Mesh* rect_ghost2 = object2D::CreateRectangle("rect_ghost2", corner, 0.2, 0.4, glm::vec3(0, 0, 0), true);
    AddMeshToList(rect_ghost2);
    Mesh* rect_ghost3 = object2D::CreateRectangle("rect_ghost3", corner, 0.2, 0.4, glm::vec3(0, 0, 0), true);
    AddMeshToList(rect_ghost3);
    Mesh* rect_ghost4 = object2D::CreateRectangle("rect_ghost4", corner, 0.2, 0.4, glm::vec3(0, 0, 0), true);
    AddMeshToList(rect_ghost4);
    //eyes
    Mesh* eyes = object2D::CreateCircle("eyes", 10, 0.05, 0.07, glm::vec3(0, 0, 0));
    AddMeshToList(eyes);
    //heart
    Mesh* heart = object2D::Heart("heart", glm::vec3(0, 0, 0), 0.15, glm::vec3(1, 0, 0));
    AddMeshToList(heart);

    srand(time(0));
    ResetGrid(grid);
    Visit(1, 1, grid);

    float i = 0.5;
    float j = -0.35;

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 18; x++)
        {
            if (x == 0)
            {
                j = j + 0.85;
                i = 0.5;
            }
            if (grid[IndexGrid(x, y)] == 1)
            {   //aici se vor randa peretii labirintului 
                i = i + 0.85;
            }
            else if (grid[IndexGrid(x, y)] == 0)
            {
                //retin pozitiile celor patru fantome
                if (((x != 1) || (y != 1)) && (nrg1 == 0) && (x == 8)) 
                {
                    posghost1x = i + 0.13;
                    posghost1y = j;
                    
                    nrg1++;
                }

                if (((x != 1) || (y != 1)) && (nrg1 == 1) && (x == 6)) 
                {
                    posghost2x = i + 0.13;
                    posghost2y = j;
                
                    nrg1++;
                }

                if (((x != 1) || (y != 1)) && (nrg1 == 2) && (y == 7)) 
                {
                    posghost3x = i + 0.13;
                    posghost3y = j;
                  
                    nrg1++;
                }

                if (((x != 1) || (y != 1)) && (nrg1 == 3) && (x == 12)) 
                {
                    posghost4x = i + 0.13;
                    posghost4y = j;
                  
                    nrg1++;
                }

                //retin pozitiile pentru mancaruri speciale
                
                
                if (((x != 1) || (y != 1)) && (nrg2 == 0) &&(x==3)) 
                {
                    posspecial1x = i + 0.35;
                    posspecial1y = j + 0.35;
 
                    nrg2++;
                }

                if (((x != 1) || (y != 1)) && (nrg2 ==1) && (x == 6)) 
                {
                    posspecial2x = i + 0.35;
                    posspecial2y = j + 0.35;

                    nrg2++;
                }
                    
                if (((x != 1) || (y != 1)) && (nrg2 ==2) && y == 4) 
                {
                    posspecial3x = i + 0.35;
                    posspecial3y = j + 0.35;

                    nrg2++;
                }
                    
                if (((x != 1) || (y != 1)) && (nrg2 ==3) && x == 10) 
                {
                        posspecial4x = i + 0.35;
                        posspecial4y = j + 0.35;

                        nrg2++;
                }

                i = i + 0.85;
            }
        }
    }
    time1 = glfwGetTime();
}

glm::mat3 PacMan::VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    tx = viewSpace.x - sx * logicSpace.x;
    ty = viewSpace.y - sy * logicSpace.y;

    return glm::transpose(glm::mat3(
        sx, 0.0f, tx,
        0.0f, sy, ty,
        0.0f, 0.0f, 1.0f));
}


glm::mat3 PacMan::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}


void PacMan::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    //GetSceneCamera()->SetOrthographic((float)(viewSpace.x  ), (float)(viewSpace.x + viewSpace.width ), (float)(viewSpace.y ), (float)(viewSpace.y + viewSpace.height ), 0.1f, 400);
    //camera mai mica decat harta
    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void PacMan::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void PacMan::Update(float deltaTimeSeconds)
{
    time1 += deltaTimeSeconds;
    glm::ivec2 resolution = window->GetResolution();

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0.0f, 0.5f, 0.5f), true);

    // Compute the 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    //randare jucator 
    
    
    RenderLives();
    playerMatrix = glm::mat4(1);
    playerrecMatrix = glm::mat4(1);
    playerMatrix_mouth = glm::mat4(1);
    
    score = 0;
    //randare jucator/ verificare coliziune cu fantomele
    if ((CheckCollisionGhostPlayer(posghost1x, posghost1y) == 0 && CheckCollisionGhostPlayer(posghost2x, posghost2y) == 0
            && CheckCollisionGhostPlayer(posghost3x, posghost3y) == 0 && CheckCollisionGhostPlayer(posghost4x, posghost4y) == 0 )) {

        scaleX += 2 * deltaTimeSeconds;
        scaleY += 2 * deltaTimeSeconds;

        playerMatrix *= visMatrix * transform2D::Translate(posplayerX, posplayerY);
           
            
        playerMatrix_mouth *= visMatrix * transform2D::Translate(posplayerX + 0.3, posplayerY + 0.3);
        playerMatrix_mouth *= transform2D::Rotate(M_PI);
            
           
        if (scaleX < 5 && scaleY < 5) {
            playerMatrix *= transform2D::Scale(sin(scaleX), sin(scaleY));
        }

        if ((int)time1 % 2 == 0) {
                RenderMesh2D(meshes["player_mouth"], shaders["VertexColor"], playerMatrix_mouth);
        }
            
        RenderMesh2D(meshes["player"], shaders["VertexColor"], playerMatrix);
        playerrecMatrix *= visMatrix * transform2D::Translate(posplayerX - 0.3, posplayerY - 0.3);
        RectPos = GetRectPosition(posplayerX, posplayerY);
        RenderMesh2D(meshes["player_rect"], shaders["VertexColor"], playerrecMatrix);
    }
    else if(special1==0 && nr_ghost == 0){
        //repozitionare pe prima pozitie si decrementare vieti
        lives--;
        scaleX = 1;
        scaleY = 1;
        posplayerX = 1.75;
        posplayerY = 1.7;
    }
    //daca exista coliziune si e sub influenta mancarii speciale a trecut deja peste o fantoma
    else if ((CheckCollisionGhostPlayer(posghost1x, posghost1y) == 1 || CheckCollisionGhostPlayer(posghost2x, posghost2y) == 1
            || CheckCollisionGhostPlayer(posghost3x, posghost3y) == 1 || CheckCollisionGhostPlayer(posghost4x, posghost4y) == 1) && special1 == 1) {
         nr_ghost = 1;
    }
    //reinitializare valori ptr detectarea mancarii speciale
    if (nr_ghost == 1 && (CheckCollisionGhostPlayer(posghost1x, posghost1y) == 0 && CheckCollisionGhostPlayer(posghost2x, posghost2y) == 0
            && CheckCollisionGhostPlayer(posghost3x, posghost3y) == 0 && CheckCollisionGhostPlayer(posghost4x, posghost4y) == 0)) {
        special1 = 0;
        nr_ghost = 0;
    }
       
    check = false;
    nr = 0;
    nrg1 = 0;
   
    index = 1;
    float i = 0.5;
    float j = -0.35;
    
    
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 18; x++)
        { 
            if (x == 0)
            {
                j = j + 0.85;
                i = 0.5;
            }

            if (grid[IndexGrid(x, y)] == 1)
            {   //randare obstacole/peretii labirintului
                obstacleMatrix = glm::mat4(1);
                obstacleMatrix *= visMatrix * transform2D::Translate(i, j);

                RenderMesh2D(meshes["obstacle"], shaders["VertexColor"], obstacleMatrix);
                PosObstacle = GetObstaclePosition(i, j);
                //cu check verific in OnInputUpdate sa nu treaca prin pereti
                if (CheckCollisionPlayerObs()) {
                    check = true;
                }
                //se verifica existenta coliziunii fantomelor cu peretii stanga-dreapta
                if ((CheckCollisionGhostObs(posghost1x, posghost1y) == 1) && (right1 == 0))
                {
                    right1 = 1;
                    left1 = 1;
                }
                else if ((CheckCollisionGhostObs(posghost1x, posghost1y) == 1) && (left1 == 1)) {
                    right1 = 0;
                    left1 = 0;
                }

                
                if ((CheckCollisionGhostObs(posghost2x, posghost2y) == 1) && (right2 == 0))
                {
                    right2 = 1;
                    left2 = 1;
                }
                else if ((CheckCollisionGhostObs(posghost2x, posghost2y) == 1) && (left2 == 1)) {
                    right2 = 0;
                    left2 = 0;
                }
                
                if ((CheckCollisionGhostObs(posghost3x, posghost3y) == 1) && (right3 == 0))
                {
                    right3 = 1;
                    left3 = 1;
                }
                else if ((CheckCollisionGhostObs(posghost3x, posghost3y) == 1) && (left3 == 1)) {
                    right3 = 0;
                    left3 = 0;
                }
                if ((CheckCollisionGhostObs(posghost4x, posghost4y) == 1) && (right4 == 0))
                {
                    right4 = 1;
                    left4 = 1;
                }
                else if ((CheckCollisionGhostObs(posghost4x, posghost4y) == 1) && (left4 == 1)) {
                    right4 = 0;
                    left4 = 0;
                }
                //trecere la urmatorul spatiu din labirint
                i = i + 0.85;
            }
            else if (grid[IndexGrid(x, y)] == 0)
            {
                if (((x != 1) || (y != 1)) && (nrg1 == 0) && (x == 8)) {
                        ghost1Matrix = glm::mat4(1);
                        //dau directia de deplasare stanga sau dreapta in functie de coliziunea cu peretii si de limitele labirintului
                        if ((right1 == 0) && (posghost1x < 14.5)) {
                            posghost1x += deltaTimeSeconds;
                        }
                        else
                        if (left1 == 1 || (posghost1x > 14.5))
                        {
                             right1 = 1;
                             posghost1x -= deltaTimeSeconds;
                        }
                        //randare fantoma
                        ghost1Matrix *= visMatrix * transform2D::Translate(posghost1x + 0.22, posghost1y + 0.35);
                        ghost1recMatrix = glm::mat4(1);
                        ghost1recMatrix *= visMatrix * transform2D::Translate(posghost1x+0.15, posghost1y+0.5);
                        RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost1recMatrix);
                        ghost1recMatrix = glm::mat4(1);
                        ghost1recMatrix *= visMatrix * transform2D::Translate(posghost1x + 0.3, posghost1y + 0.5);
                        RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost1recMatrix);
                        ghost1recMatrix = glm::mat4(1);
                        ghost1recMatrix *= visMatrix * transform2D::Translate(posghost1x, posghost1y);
                        RenderMesh2D(meshes["rect_ghost1"], shaders["VertexColor"], ghost1recMatrix);
                        RenderMesh2D(meshes["ghost1"], shaders["VertexColor"], ghost1Matrix);
                    
                        nrg1++;
                }

                if (((x != 1) || (y != 1)) && (nrg1 == 1) && (x == 6)) {
                    ghost2Matrix = glm::mat4(1);


                    if ((right2 == 0)&&(posghost2x < 15)) {
                        posghost2x += deltaTimeSeconds;
                    }
                    else
                    if (left2 == 1 || (posghost2x > 15))
                    {
                        right2 = 1;
                        posghost2x -= deltaTimeSeconds;
                    }
                    ghost2Matrix *= visMatrix * transform2D::Translate(posghost2x + 0.22, posghost2y + 0.35);
                    ghost2recMatrix = glm::mat4(1);
                    ghost2recMatrix *= visMatrix * transform2D::Translate(posghost2x + 0.15, posghost2y + 0.5);
                    RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost2recMatrix);
                    ghost2recMatrix = glm::mat4(1);
                    ghost2recMatrix *= visMatrix * transform2D::Translate(posghost2x + 0.3, posghost2y + 0.5);
                    RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost2recMatrix);
                    ghost2recMatrix = glm::mat4(1);
                    ghost2recMatrix *= visMatrix * transform2D::Translate(posghost2x, posghost2y);
                    RenderMesh2D(meshes["rect_ghost2"], shaders["VertexColor"], ghost2recMatrix);
                    RenderMesh2D(meshes["ghost2"], shaders["VertexColor"], ghost2Matrix);
                    
                    nrg1++;
                    
                }
                
                if (((x != 1) || (y != 1)) && (nrg1 == 2) && (y == 7)) {
                    ghost3Matrix = glm::mat4(1);
                    if ((right3 == 0)&&(posghost3x < 15)) {
                        posghost3x += deltaTimeSeconds;
                    }
                    else
                    if (left3 == 1 || (posghost3x > 15))
                    {
                        right3 = 1;
                        posghost3x -= deltaTimeSeconds;
                    }

                    ghost3Matrix *= visMatrix * transform2D::Translate(posghost3x + 0.22, posghost3y + 0.35);
                    ghost3recMatrix = glm::mat4(1);
                    ghost3recMatrix *= visMatrix * transform2D::Translate(posghost3x + 0.15, posghost3y + 0.5);
                    RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost3recMatrix);
                    ghost3recMatrix = glm::mat4(1);
                    ghost3recMatrix *= visMatrix * transform2D::Translate(posghost3x + 0.3, posghost3y + 0.5);
                    RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost3recMatrix);
                    ghost3recMatrix = glm::mat4(1);
                    ghost3recMatrix *= visMatrix * transform2D::Translate(posghost3x, posghost3y);
                    RenderMesh2D(meshes["rect_ghost3"], shaders["VertexColor"], ghost3recMatrix);
                    RenderMesh2D(meshes["ghost3"], shaders["VertexColor"], ghost3Matrix);
                    
                    nrg1++;
                }
                
                if (((x != 1) || (y != 1)) && (nrg1 == 3) && (x == 12)) {
                    ghost4Matrix = glm::mat4(1);
                    if ((right4 == 0)&&(posghost4x < 15)) {
                        posghost4x += deltaTimeSeconds;
                    }
                    else
                    if (left4 == 1 || (posghost4x > 15))
                    {
                        right4 = 1;
                        posghost4x -= deltaTimeSeconds;
                    }

                    ghost4Matrix *= visMatrix * transform2D::Translate(posghost4x + 0.22, posghost4y + 0.35);
                    ghost4recMatrix = glm::mat4(1);
                    ghost4recMatrix *= visMatrix * transform2D::Translate(posghost4x + 0.15, posghost4y + 0.5);
                    RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost4recMatrix);
                    ghost4recMatrix = glm::mat4(1);
                    ghost4recMatrix *= visMatrix * transform2D::Translate(posghost4x + 0.3, posghost4y + 0.5);
                    RenderMesh2D(meshes["eyes"], shaders["VertexColor"], ghost4recMatrix);
                    ghost4recMatrix = glm::mat4(1);
                    ghost4recMatrix *= visMatrix * transform2D::Translate(posghost4x, posghost4y);
                    RenderMesh2D(meshes["rect_ghost4"], shaders["VertexColor"], ghost4recMatrix);
                    RenderMesh2D(meshes["ghost4"], shaders["VertexColor"], ghost4Matrix);
                    
                    nrg1++;
                }
                
                //randare mancare
                if ((x != 1) || (y != 1)) {
                    foodMatrix = glm::mat4(1);
                    foodMatrix *= visMatrix * transform2D::Translate(i + 0.35, j + 0.35);

                    foodrecMatrix = glm::mat4(1);
                    foodrecMatrix *= visMatrix * transform2D::Translate(i + 0.2, j + 0.2);
                    RectPos = GetRectPosition(posplayerX, posplayerY);
                   
                    FoodPos = GetRectPositionFood(i + 0.2, j + 0.2);
                    /*am declarat un vector destul de mare cat sa - mi retina evidenta mancarurile mancate
                    ca sa nu se mai randeze dupa ce exista o coliziune cu jucatorul*/
                    if (CheckCollisionPlayerFood() == 0 && (food[nr] == 0))
                    {
                        RenderMesh2D(meshes["food"], shaders["VertexColor"], foodMatrix);
                        RenderMesh2D(meshes["food_rect"], shaders["VertexColor"], foodrecMatrix);
                    }
                    else
                    {   
                        score += 100;
                        food[nr] = 1;
                    }
                }
                if (((x != 1) || (y != 1))&&((x == 3)|| (x == 6)|| (y == 4)|| (x == 10))) {
                    //in momentul coliziunii cu o mancare speciala special1=1
                    if (CheckCollisionSpecialPlayer()) {
                        special1 = 1;
                        score += 200;
                        nr_ghost = 0;
                    }     
                }

                nr++;
                i = i + 0.85;
            }
        }
    }
    //randare mancare speciala folosind pozitiile retinute in  init
    foodspecialMatrix1 = glm::mat4(1);
    foodspecialMatrix1 *= visMatrix * transform2D::Translate(posspecial1x, posspecial1y);
    RenderMesh2D(meshes["special_food"], shaders["VertexColor"], foodspecialMatrix1);
    foodspecialMatrix2 = glm::mat4(1);
    foodspecialMatrix2 *= visMatrix * transform2D::Translate(posspecial2x, posspecial2y);
    RenderMesh2D(meshes["special_food"], shaders["VertexColor"], foodspecialMatrix2);
    foodspecialMatrix3 = glm::mat4(1);
    foodspecialMatrix3 *= visMatrix * transform2D::Translate(posspecial3x, posspecial3y);
    RenderMesh2D(meshes["special_food"], shaders["VertexColor"], foodspecialMatrix3);
    foodspecialMatrix4 = glm::mat4(1);
    foodspecialMatrix4 *= visMatrix * transform2D::Translate(posspecial4x, posspecial4y);
    RenderMesh2D(meshes["special_food"], shaders["VertexColor"], foodspecialMatrix4);
    
    //randare cadru
    mapMatrix = glm::mat3(1);
    mapMatrix *= visMatrix * transform2D::Translate(0.5, 0.5);

    RenderMesh2D(meshes["map"], shaders["VertexColor"], mapMatrix);
}

void PacMan::FrameEnd()
{
}

//deplasari cu tastele player+proiectil
void PacMan::OnInputUpdate(float deltaTime, int mods)
{
    
    //miscare cu taste
    glm::ivec2 resolution = window->GetResolution();
    
    if (window->KeyHold(GLFW_KEY_W)&&((check==false) || (moveS == true) || (moveD == true) || (moveA == true)))
    {
        moveS = false;
        moveA = false;
        moveD = false;
        posplayerY += player_speed * deltaTime;
    }
    else
    if (window->KeyHold(GLFW_KEY_W) && (check == true) && (moveW == false)) {
        moveW = true;
    }

    if (window->KeyHold(GLFW_KEY_A) && ((check == false) || (moveS == true) || (moveD == true) || (moveW == true)))
    {
        moveS = false;
        moveW = false;
        moveD = false;
        posplayerX -= player_speed * deltaTime;
    }
    else
    if ((window->KeyHold(GLFW_KEY_A)) && (check == true)&& (moveA == false)) {
            moveA = true;
    }
     
    if (window->KeyHold(GLFW_KEY_S) && ((check == false) || (moveW == true) || (moveD == true) || (moveA == true)))
    {
        moveW = false;
        moveA = false;
        moveD = false;
        posplayerY -= player_speed * deltaTime;
    }
    else
    if ((window->KeyHold(GLFW_KEY_S)) && (check == true) && (moveS == false)) {
            moveS = true;
    }
     
    if (window->KeyHold(GLFW_KEY_D) && ((check == false) || (moveS == true) || (moveW == true) || (moveA == true)))
    {
        moveS = false;
        moveW = false;
        moveA = false;

        posplayerX += player_speed * deltaTime;
    }
    else
    if ((window->KeyHold(GLFW_KEY_D)) && (check == true) && (moveD == false)) {
        moveD = true;
    }
}


void PacMan::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void PacMan::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


//miscare dupa mouse
void PacMan::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}


void PacMan::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
 
}


void PacMan::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void PacMan::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void PacMan::OnWindowResize(int width, int height)
{
}

void PacMan::ResetGrid(int grid[162])
{
    //punem perecti in tot grid-ul
    for (int i = 0; i < 162; i++)
    {
        grid[i] = 1;
    }
}

int PacMan::IndexGrid(int x, int y)
{
    return y * 18 + x;
}

int PacMan::Bounds(int x, int y)
{
    // x si y trebuie sa se regaseasca in limitele labirintului
    if (x < 0 || x >= 18) return false;
    if (y < 0 || y >= 9) return false;
    return true;
}

void PacMan::Visit(int x, int y, int grid[162])
{
    // Starting at the given index, recursively visits every direction in a
    // randomized order.
    // Set my current location to be an empty passage.
    grid[IndexGrid(x, y)] = 0;
    // Create an local array containing the 4 directions and shuffle their order.
    int dirs[4];
    dirs[0] = NORTH;
    dirs[1] = EAST;
    dirs[2] = SOUTH;
    dirs[3] = WEST;
    for (int i = 0; i < 4; ++i)
    {
        int r = rand() & 3;
        int temp = dirs[r];
        dirs[r] = dirs[i];
        dirs[i] = temp;
    }
    // Loop through every direction and attempt to Visit that direction.
    for (int i = 0; i < 4; ++i)
    {
        // dx,dy are offsets from current location. Set them based
        // on the next direction I wish to try.
        int dx = 0, dy = 0;
        switch (dirs[i])
        {
        case NORTH: dy = -1; break;
        case SOUTH: dy = 1; break;
        case EAST: dx = 1; break;
        case WEST: dx = -1; break;
        }
        // Find the (x,y) coordinates of the grid cell 2 spots
        // away in the given direction.
        int x2 = x + (dx << 1);
        int y2 = y + (dy << 1);
        if (Bounds(x2, y2))
        {
            if (grid[IndexGrid(x2, y2)] == 1)
            {
                // (x2,y2) has not been visited yet... knock down the
                // wall between my current position and that position
                grid[IndexGrid(x2 - dx, y2 - dy)] = 0;
                // Recursively Visit (x2,y2)
                Visit(x2, y2, grid);
            }
        }
    }
}

glm::vec2 PacMan::GetObstaclePosition(float i, float j)
{
    return PosObstacle = glm::vec2(i , j);
}

glm::vec2 PacMan::GetRectPosition(float posplayerX, float posplayerY)
{
    return RectPos = glm::vec2(posplayerX - 0.3, posplayerY - 0.3);
}

glm::vec2 PacMan::GetRectPositionFood(float posfoodX, float posfoodY)
{
    return FoodPos = glm::vec2(posfoodX, posfoodY);
}

bool PacMan::CheckCollisionPlayerObs() // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = PosObstacle.x + 0.8 >= RectPos.x &&
        RectPos.x + 0.6 >= PosObstacle.x;
    // collision y-axis?
    bool collisionY = PosObstacle.y + 0.8 >= RectPos.y &&
        RectPos.y + 0.6 >= PosObstacle.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool PacMan::CheckCollisionPlayerFood() // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = FoodPos.x + 0.3 >= RectPos.x &&
        RectPos.x + 0.6 >= FoodPos.x;
    // collision y-axis?
    bool collisionY = FoodPos.y + 0.3 >= RectPos.y &&
        RectPos.y + 0.6 >= FoodPos.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool PacMan::CheckCollisionGhostObs(float posghostx,float posghosty) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = PosObstacle.x + 0.8 >= posghostx &&
        posghostx + 0.4 >= PosObstacle.x;
    // collision y-axis?
    bool collisionY = PosObstacle.y + 0.8 >= posghosty &&
        posghosty + 0.2 >= PosObstacle.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

bool PacMan::CheckCollisionGhostPlayer(float posghostx, float posghosty) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = RectPos.x + 0.6 >= posghostx &&
        posghostx + 0.4 >= RectPos.x;
    // collision y-axis?
    bool collisionY = RectPos.y + 0.6 >= posghosty &&
        posghosty + 0.2 >= RectPos.y;
    // collision only if on both axes
    return collisionX && collisionY;
}


bool PacMan::CheckCollisionSpecialPlayer() // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = FoodPos.x + 0.3 >= RectPos.x &&
        RectPos.x + 0.6 >= FoodPos.x;
    // collision y-axis?
    bool collisionY = FoodPos.y + 0.3 >= RectPos.y &&
        RectPos.y + 0.6 >= FoodPos.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

void PacMan::RenderLives() {
    int x = 0.5;
    if (lives == 0) {
        std::cout << "YOU LOST THE GAME! Your final score is: " << score <<  std::endl;
        exit(0);
    }
    
    for (int i = 1; i <= lives; i++) {
        modelMatrix = glm::mat3(1);
        modelMatrix *= visMatrix * transform2D::Translate(1 + x, 8.7);
        RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrix);
       x += 1;
    }
}