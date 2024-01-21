#include "PhysicsObject.h"

#include <iostream>

void PhysicsObject::update(float deltaTime)
{
    float rigidity = 0.8f;
    vel = (pos - prev_pos) / deltaTime;
    acceleration = (prev_vel - vel) / deltaTime;
    const float gravity = 1000;
    prev_vel = vel;
    prev_pos = pos;
    acceleration.y = gravity;
    vel += acceleration * deltaTime;
    pos += vel * deltaTime;
}

void PhysicsObject::Spawn(glm::vec2 start)
{
    pos = prev_pos = start;
    editor_pos = { 0,0,0 };
}
