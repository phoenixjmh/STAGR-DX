#pragma once
#include <math.h>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "Model.h"
class PhysicsObject {
public:
    PhysicsObject(float rad) : radius{ rad }, id{ nxt_id++ }, mass{ float(pow(rad, 2)) } {}
public:
    unsigned int Model_ID;
    float mass;
    glm::vec2 acceleration;
    float radius;
    float editor_size=1;
    glm::vec3 editor_pos;
    glm::vec2 prev_pos;
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 prev_vel;
    void update(float deltaTime);
    void Spawn(glm::vec2 start);

    bool operator==(const PhysicsObject& other) const { return (id == other.id); }

    bool operator!=(const PhysicsObject& other) const { return !(id == other.id); }

private:
    int id;
    static int nxt_id;
};
