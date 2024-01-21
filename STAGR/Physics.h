#pragma once
#include "PhysicsObject.h"
#include <random>

class Physics {
public:
    static void Update(double deltaTime);
    static std::vector<PhysicsObject> all_sand;
    static void ResolveCollision(PhysicsObject& a, PhysicsObject& b, float magnitude, float deltaTime);
    static void ResolveBorderCollision(PhysicsObject& s, float ground_level, float left_border, float right_border, float deltaTime);
    static void previousToCurrent();
    static float getDistance(float ax, float bx, float ay, float by);
};

