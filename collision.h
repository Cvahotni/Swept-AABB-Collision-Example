#pragma once

#include "pch.h"

//This class uses the Vector3 class in this repository, and also several private ones from the voxel engine; 
//to use this properly please adjust the source files here to work with your own system or engine.

#include "world/aabb.h"
#include "world/world.h"
#include "math/vector3.h"

class Collision {
public:
    static Vector3 MoveAndCollide(AABB& aabb, Vector3& velocity, std::unordered_map<Vector3Int, uint8_t, Vector3IntHash>& solidBlocks, 
        std::unordered_map<Vector3Int, uint8_t, Vector3IntHash>& blockRotations, std::shared_ptr<World> world, bool& onGround, bool& onSides, Vector3& sideHeight);

    static Vector3 Test(AABB playerAABB, Vector3 velocity, std::shared_ptr<World> world, const int32_t radius, bool& onGround, bool& onSides, Vector3& sideHeight);
    static void PrimitiveCollisionTest(std::shared_ptr<World> world, Vector3& position, Vector3 moveDelta);

    static double SnapToGrid(double value);
};
