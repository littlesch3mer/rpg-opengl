#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Game
{
public:
    static const double PHYSICS_DELTA_TIME; 

    static bool* Keys;
    static int  Width, Height;
    static glm::vec2 MousePos;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void OnResizeWindow(float dt);

    // Loop
    void ProcessInput(float dt);
    void PhysicsLoop();
    void Update(float dt);
    void Render();
    
    // input, move to separate class
    void OnKeyDown(int code);
    void OnKeyUp(int code);
    void OnMouseButtonDown(int key);
    void OnMouseButtonUp(int key); 
    void OnScrollWheel(double dx, double dy);
private:
};

#endif

