#pragma once

#include "pch.h"

//This class uses GLM for the voxel engine, if you don't want to use it in your project, you can replace 'vector3' with individual double values.
#include "glm/glm.hpp"

class Vector3 {
public:
    Vector3() : vector3(glm::dvec3(0.0, 0.0, 0.0)) {}
    Vector3(double _x, double _y, double _z) : vector3(glm::dvec3(_x, _y, _z)) {}

    void Cross(Vector3& vec1, Vector3& vec2);
    void Normalize();

    double X() { 
        return vector3.x; 
    }

    double Y() { 
        return vector3.y; 
    }

    double Z() { 
        return vector3.z; 
    }
    
    void SetX(const double x) { 
        vector3.x = x; 
    }

    void SetY(const double y) { 
        vector3.y = y; 
    }

    void SetZ(const double z) { 
        vector3.z = z; 
    }

    void ModifyX(const double x) { 
        vector3.x += x; 
    }

    void ModifyY(const double y) { 
        vector3.y += y; 
    }

    void ModifyZ(const double z) { 
        vector3.z += z; 
    }

    bool operator==(Vector3 other) const {
        return (
            vector3.x == other.X() &&
            vector3.y == other.Y() &&
            vector3.z == other.Z()
        );
    }

    Vector3 operator+(Vector3& other) const {
        return {
            vector3.x + other.X(), 
            vector3.y + other.Y(), 
            vector3.z + other.Z()
        };
    }

    Vector3 operator-(Vector3& other) const {
        return {
            vector3.x - other.X(), 
            vector3.y - other.Y(), 
            vector3.z - other.Z()
        };
    }

    Vector3 operator*(Vector3& other) const {
        return {
            vector3.x * other.X(), 
            vector3.y * other.Y(), 
            vector3.z * other.Z()
        };
    }

    Vector3 operator*(double scalar) const {
        return {
            vector3.x * scalar, 
            vector3.y * scalar, 
            vector3.z * scalar
        };
    }

    Vector3 operator/(Vector3& other) const {
        return {
            vector3.x / other.X(), 
            vector3.y / other.Y(), 
            vector3.z / other.Z()
        };
    }

    Vector3 operator/(double divider) const {
        return {
            vector3.x / divider, 
            vector3.y / divider, 
            vector3.z / divider
        };
    }

    Vector3& operator+=(Vector3& other) {
        vector3.x += other.X();
        vector3.y += other.Y();
        vector3.z += other.Z();

        return *this;
    }

    Vector3& operator-=(Vector3& other) {
        vector3.x -= other.X();
        vector3.y -= other.Y();
        vector3.z -= other.Z();

        return *this;
    }

    Vector3& operator*=(Vector3& other) {
        vector3.x *= other.X();
        vector3.y *= other.Y();
        vector3.z *= other.Z();

        return *this;
    }

    Vector3& operator*=(double scalar) {
        vector3.x *= scalar;
        vector3.y *= scalar;
        vector3.z *= scalar;

        return *this;
    }

    Vector3& operator/=(Vector3& other) {
        vector3.x /= other.X();
        vector3.y /= other.Y();
        vector3.z /= other.Z();

        return *this;
    }

    Vector3& operator/=(double divider) {
        vector3.x /= divider;
        vector3.y /= divider;
        vector3.z /= divider;

        return *this;
    }

    double Distance(Vector3 other) {
        return sqrt(
            std::pow(std::abs(vector3.x - other.X()), 2.0) + 
            std::pow(std::abs(vector3.y - other.Y()), 2.0) +
            std::pow(std::abs(vector3.z - other.Z()), 2.0)
        );
    };

    double SecondaryDistance(Vector3 other) {
        double dX = std::abs(other.X() - vector3.x);
        double dY = std::abs(other.Y() - vector3.y);
        double dZ = std::abs(other.Z() - vector3.z);

        return std::sqrt(dX * dX + dY * dY + dZ * dZ);
    }

    double Dot(Vector3 normal) {
        return (
            vector3.x * normal.X() + 
            vector3.y * normal.Y() + 
            vector3.z * normal.Z()
        );
    }

    double Length() {
        return sqrt(
            vector3.x * vector3.x + 
            vector3.y * vector3.y +
            vector3.z * vector3.z
        );
    }

    double LengthSquared() {
        return (
            std::pow(vector3.x, 2) + 
            std::pow(vector3.y, 2) + 
            std::pow(vector3.z, 2)
        );
    }

    double MaxComponent() {
        return std::max(vector3.y, std::max(vector3.x, vector3.z));
    }

    static Vector3 Normalize(Vector3& vector) {
        double length = sqrt(
            vector.X() * vector.X() + 
            vector.Y() * vector.Y() + 
            vector.Z() * vector.Z()
        );

        if(length > 0.0) {
            return Vector3(
                vector.X() / length, 
                vector.Y() / length, 
                vector.Z() / length
            );
        } 

        else {
            return vector;
        }
    }   

    std::string ToString() {
        return std::to_string(vector3.x) + ", " + std::to_string(vector3.y) + ", " + std::to_string(vector3.z);
    }

    glm::dvec3 Raw() { 
        return vector3;
    }
    
private:
    glm::dvec3 vector3{};
};
