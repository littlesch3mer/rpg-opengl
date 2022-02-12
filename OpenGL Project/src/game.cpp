#include <iostream>
#include <random>

#include "game.h"
const double Game::PHYSICS_DELTA_TIME = 0.02f; // 50 updates per second
bool* Game::Keys;
glm::vec2 Game::MousePos;

Game::Game(unsigned int width, unsigned int height)
{
    if (Keys == nullptr)
    {
        Keys = new bool[1024];
    }
}
Game::~Game()
{
}
void Game::Init()
{
    
}
void Game::PhysicsLoop()
{

}
void Game::Update(float dt)
{
}
void Game::ProcessInput(float dt)
{

}
void Game::OnKeyDown(int key)
{
}
void Game::OnKeyUp(int key)
{

}
void Game::OnMouseButtonDown(int key)
{

}
void Game::OnMouseButtonUp(int key)
{

}
void Game::OnScrollWheel(double dx, double dy)
{

}
void Game::Render()
{
    
}