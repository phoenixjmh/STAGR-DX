#include "Physics.h"

void Physics::Update(double deltaTime)
{

    const float bottom_border = 30;
    const float right_border = 20;
    const float left_border = -20;
    for (auto& s : all_sand)
    {
        ResolveBorderCollision(s, bottom_border, left_border, right_border, deltaTime);
        for (auto& other_s : all_sand)
        {
            if (s != other_s)
            {
                const float distance = getDistance(s.pos.x, other_s.pos.x, s.pos.y, other_s.pos.y);

                if (distance <= s.radius + other_s.radius)
                {
                    ResolveCollision(s, other_s, distance, deltaTime);
                }
            }
        }
        s.update(deltaTime);
    }
}

/// Collision between two bodies
void Physics::ResolveCollision(PhysicsObject& a, PhysicsObject& b, float magnitude, float deltaTime)
{
    glm::vec2 direction = b.pos - a.pos;
    glm::vec2 normal = { direction.x / magnitude, direction.y / magnitude };
    float relative_velocity = (a.vel.x - b.vel.x) * normal.x + (a.vel.y - b.vel.y) * normal.y;
    float totalMass = (a.mass + b.mass);
    float force = 5;
    float massRatioA = a.mass / totalMass;
    float massRatioB = b.mass / totalMass;
    float totalRadius = a.radius + b.radius;
    float overlap = (totalRadius - magnitude) / 2.0f;
    a.pos -= overlap * massRatioB * normal * deltaTime * force;
    b.pos += overlap * massRatioA * normal * deltaTime * force;
}

/// Collision between body and screen border
void Physics::ResolveBorderCollision(PhysicsObject& s, float ground_level, float left_border, float right_border, float deltaTime)
{
    float rigidity = 20;
    if (s.pos.y + s.radius >= ground_level)
    {
        float depth = (s.pos.y + s.radius) - ground_level;
        float direction = -1;
        s.pos.y += depth * rigidity * direction * deltaTime;
    }
    if (s.pos.x + s.radius > right_border)
    {
        float depth = (s.pos.x + s.radius) - right_border;
        float direction = -1;
        s.pos.x += depth * rigidity * direction * deltaTime;
    }
    if (s.pos.x - s.radius < left_border)
    {
        float depth = (s.pos.x - s.radius) - left_border;
        float direction = -1;
        s.pos.x += depth * rigidity * direction * deltaTime;
    }
}

void Physics::previousToCurrent()
{
    for (auto& obj : Physics::all_sand)
    {
        obj.prev_pos = obj.pos;
    }
}

float Physics::getDistance(float ax, float bx, float ay, float by)
{
    return sqrt((pow((ax)-(bx), 2)) + (pow((ay)-(by), 2)));
}
