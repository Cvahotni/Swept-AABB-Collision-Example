#pragma once
#include "math/vector3.h"

struct AABB {
public:
    bool IntersectsAtAll(AABB other) {
        return (
            min.X() < other.max.X() && max.X() > other.min.X() &&
            min.Y() < other.max.Y() && max.Y() > other.min.Y() &&
            min.Z() < other.max.Z() && max.Z() > other.min.Z()
        );
    }

    bool PointIntersects(AABB other) {
        return (
            pos.X() > other.min.X() && pos.X() < other.max.X() && 
            pos.Y() > other.min.Y() && pos.Y() < other.max.Y() && 
            pos.Z() > other.min.Z() && pos.Z() < other.max.Z()
        );
    }

    bool IntersectsBlock(int32_t blockX, int32_t blockY, int32_t blockZ) {
        Vector3 blockMin(blockX, blockY, blockZ);
        Vector3 blockMax(blockX + 1, blockY + 1, blockZ + 1);

        return (
            blockMin.X() >= min.X() && blockMax.X() <= max.X() &&
            blockMin.Y() >= min.Y() && blockMax.Y() <= max.Y() &&
            blockMin.Z() >= min.Z() && blockMax.Z() <= max.Z()
        );
    }

    bool RayIntersects(Vector3 origin, Vector3 direction, AABB aabb) {
        double dirFraction[3];

        dirFraction[0] = std::fabs(direction.X()) > 0.0 ? 1.0 / direction.X() : 0.0;
        dirFraction[1] = std::fabs(direction.X()) > 0.0 ? 1.0 / direction.Y() : 0.0;
        dirFraction[2] = std::fabs(direction.X()) > 0.0 ? 1.0 / direction.Z() : 0.0;

        double t1 = (aabb.min.X() - origin.X()) * dirFraction[0];
        double t2 = (aabb.max.X() - origin.X()) * dirFraction[0];
        double t3 = (aabb.min.Y() - origin.Y()) * dirFraction[1];
        double t4 = (aabb.max.Y() - origin.Y()) * dirFraction[1];
        double t5 = (aabb.min.Z() - origin.Z()) * dirFraction[2];
        double t6 = (aabb.max.Z() - origin.Z()) * dirFraction[2];

        double tmin = std::max(
            std::max(std::min(t1, t2), std::min(t3, t4)
        ), std::min(t5, t6));

        double tmax = std::min(
            std::min(std::max(t1, t2), std::max(t3, t4)
        ), std::max(t5, t6));

        if(tmax < 0) {
            return false;
        }

        if(tmin > tmax) {
            return false;
        }
        
        return true;
    }

    Vector3 min{};
    Vector3 max{};
    Vector3 pos{};
    Vector3 size{};
};
