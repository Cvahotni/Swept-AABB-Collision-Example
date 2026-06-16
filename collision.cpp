#include "pch.h"
#include "physics/collision.h"
#include "core/log.h"
#include "core/resource_manager.h"
#include "util/block_model_helper.h"

//Please see the header file for this class for details on private includes for your project.

Vector3 Collision::MoveAndCollide(AABB& aabb, Vector3& velocity, std::unordered_map<Vector3Int, uint8_t, Vector3IntHash>& solidBlocks, 
    std::unordered_map<Vector3Int, uint8_t, Vector3IntHash>& blockRotations, bool& onGround, bool& onSides) {

    Vector3 pos = aabb.pos;
    Vector3 remainingVel = velocity;

    const double EPSILON = 0.001;
    onGround = false;

    for(int32_t axis = 0; axis < 3; axis++) {
        double move = (axis == 0) ? remainingVel.X() : (axis == 1 ? remainingVel.Y() : remainingVel.Z());

        if(move == 0.0) {
            continue;
        }

        Vector3 step(0.0, 0.0, 0.0);

        if(axis == 0) {
            step.SetX(move);
        }

        if(axis == 1) {
            step.SetY(move);
        }

        if(axis == 2) {
            step.SetZ(move);
        }

        int32_t steps = static_cast<int32_t>(std::abs(move) * 10.0) + 1;
        Vector3 stepVel = step * (1.0 / steps);

        for(int32_t i = 0; i < steps; i++) {
            Vector3 nextPos = aabb.pos + stepVel;

            AABB nextAABB{
                nextPos,
                nextPos + aabb.size,
                nextPos,
                aabb.size
            };

            bool collided = false;
            AABB collidedWith{};

            int32_t minX = static_cast<int32_t>(std::floor(nextPos.X()));
            int32_t maxX = static_cast<int32_t>(std::ceil(nextPos.X() + aabb.size.X()));

            int32_t minY = static_cast<int32_t>(std::floor(nextPos.Y()));
            int32_t maxY = static_cast<int32_t>(std::ceil(nextPos.Y() + aabb.size.Y()));

            int32_t minZ = static_cast<int32_t>(std::floor(nextPos.Z()));
            int32_t maxZ = static_cast<int32_t>(std::ceil(nextPos.Z() + aabb.size.Z()));

            for(int32_t x = minX; x < maxX && !collided; ++x) {
                for(int32_t y = minY; y < maxY && !collided; ++y) {
                    for(int32_t z = minZ; z < maxZ && !collided; ++z) {
                        Vector3Int blockPos = {x, y, z};

                        if(solidBlocks.count(blockPos) && blockRotations.count(blockPos)) {
                            BlockModel model = BlockModelHelper::RotateBlockModel(ResourceManager::BlockModelAt(solidBlocks[blockPos]), blockRotations[blockPos]);

                            for(int32_t j = 0; j < model.ColliderCount(); j++) {
                                AABB collider = model.ColliderAt(j);

                                AABB worldCollider{
                                    {
                                        collider.min.X() + x,
                                        collider.min.Y() + y,
                                        collider.min.Z() + z
                                    },

                                    {
                                        collider.max.X() + x,
                                        collider.max.Y() + y,
                                        collider.max.Z() + z
                                    },

                                    {
                                        collider.pos.X() + x,
                                        collider.pos.Y() + y,
                                        collider.pos.Z() + z
                                    },

                                    collider.size
                                };

                                if(nextAABB.IntersectsAtAll(worldCollider)) {
                                    collided = true;
                                    collidedWith = worldCollider;

                                    break;
                                }
                            }
                        }
                    }
                }
            }

            if(collided) {
                double dir = 0.0;

                if(axis == 0) {
                    dir = stepVel.X();
                }

                if(axis == 1) {
                    dir = stepVel.Y();
                }

                if(axis == 2) {
                    dir = stepVel.Z();
                }
                
                double sign = (dir > 0.0) ? 1.0 : -1.0;

                if(axis == 0) {
                    if(sign > 0.0) {
                        aabb.pos.SetX(SnapToGrid(collidedWith.min.X() - aabb.size.X() - EPSILON));
                        onSides = true;
                    } 
                    
                    else {
                        aabb.pos.SetX(SnapToGrid(collidedWith.max.X() + EPSILON));
                        onSides = true;
                    }

                    remainingVel.SetX(0.0);
                } 
                
                else if(axis == 1) {
                    if(sign > 0.0) {
                        aabb.pos.SetY(SnapToGrid(collidedWith.min.Y() - aabb.size.Y() - EPSILON));
                    } 
                    
                    else {
                        aabb.pos.SetY(SnapToGrid(collidedWith.max.Y() + EPSILON));
                        onGround = true;
                    }

                    remainingVel.SetY(0.0);
                } 
                
                else if(axis == 2) {
                    if(sign > 0.0) {
                        aabb.pos.SetZ(SnapToGrid(collidedWith.min.Z() - aabb.size.Z() - EPSILON));
                        onSides = true;
                    } 
                    
                    else {
                        aabb.pos.SetZ(SnapToGrid(collidedWith.max.Z() + EPSILON));
                        onSides = true;
                    }

                    remainingVel.SetZ(0.0);
                }

                break;
            } 
            
            else {
                aabb.pos = nextPos;
            }
        }
    }

    return aabb.pos;
}

Vector3 Collision::Test(AABB playerAABB, Vector3 velocity, std::shared_ptr<World> world, const int32_t radius, bool& onGround, bool& onSides) {
    std::unordered_map<Vector3Int, uint8_t, Vector3IntHash> positions{};
    std::unordered_map<Vector3Int, uint8_t, Vector3IntHash> rotations{};

    AABB aabb = playerAABB;
    Vector3 sizeHalf = playerAABB.size * 0.5;

    aabb.pos -= sizeHalf;

    for(int32_t x = -radius; x <= radius; x++) {
        for(int32_t y = -radius; y <= radius; y++) {
            for(int32_t z = -radius; z <= radius; z++) {
                int32_t blockX = static_cast<int32_t>(floor(aabb.pos.X()));
                int32_t blockY = static_cast<int32_t>(floor(aabb.pos.Y()));
                int32_t blockZ = static_cast<int32_t>(floor(aabb.pos.Z()));

                int32_t addedX = blockX + x;
                int32_t addedY = blockY + y;
                int32_t addedZ = blockZ + z;

                uint16_t block = world->BlockAt(
                    addedX,
                    addedY,
                    addedZ
                );

                BlockState state = ResourceManager::BlockStateAt(block);

                if(!state.Collidable() || !state.Solid()) {
                    continue;
                }

                BlockModel model = BlockModelHelper::RotateBlockModel(ResourceManager::BlockModelAt(state.Model()), BlockID::Rotation(block));

                for(auto i = 0; i < model.ColliderCount(); i++) {
                    auto collider = model.ColliderAt(i);
                    auto amountToSplit = 2;

                    bool doSkip = false;

                    for(auto sX = 0; sX < amountToSplit + 1; sX++) {
                        for(auto sY = 0; sY < amountToSplit + 1; sY++) {
                            for(auto sZ = 0; sZ < amountToSplit + 1; sZ++) {
                                AABB checkAABB{};

                                Vector3 dividedSize{
                                    sX == 0 ? 0.0 : aabb.size.X() / sX,
                                    sY == 0 ? 0.0 : aabb.size.Y() / sY,
                                    sZ == 0 ? 0.0 : aabb.size.Z() / sZ
                                };

                                checkAABB.pos = aabb.pos + dividedSize;

                                AABB worldCollider{
                                    {
                                        collider.min.X() + addedX,
                                        collider.min.Y() + addedY,
                                        collider.min.Z() + addedZ
                                    },

                                    {
                                        collider.max.X() + addedX,
                                        collider.max.Y() + addedY,
                                        collider.max.Z() + addedZ
                                    },

                                    {
                                        collider.pos.X() + addedX,
                                        collider.pos.Y() + addedY,
                                        collider.pos.Z() + addedZ
                                    },

                                    collider.size
                                };

                                if(checkAABB.PointIntersects(worldCollider)) {
                                    doSkip = true;
                                }
                            }
                        }
                    }

                    if(doSkip) {
                        continue;
                    }

                    positions.emplace(Vector3Int{
                        addedX, 
                        addedY, 
                        addedZ
                    }, state.Model());

                    rotations.emplace(Vector3Int{
                        addedX, 
                        addedY, 
                        addedZ
                    }, BlockID::Rotation(block));
                }
            }
        }
    }

    return MoveAndCollide(aabb, velocity, positions, rotations, onGround, onSides) + sizeHalf;
}

double Collision::SnapToGrid(double value) {
    double precision = 0.01;
    return std::round(value / precision) * precision;
}
