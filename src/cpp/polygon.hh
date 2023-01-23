#pragma once
#include <vector>
#include <limits>
#include <stdexcept>

const double EPSILON = std::numeric_limits<double>::epsilon();

bool nearlyEqual(double a, double b);

struct Point3D{
    double x, y, z;

    Point3D(double x, double y, double z): x(x), y(y), z(z){};

    // second const prevents setting the values, this operator is for read-only
    const double& operator[](int index) const{
        switch(index){
            case 0:
                return x;
                break;
            case 1:
                return y;
                break;
            case 2:
                return z;
                break;
            default:
                throw std::out_of_range("Index out of range for Point3D");
        }
    }

    bool operator==(const Point3D& other) const {
        return nearlyEqual(x, other.x) && nearlyEqual(y, other.y) && nearlyEqual(z, other.z);
    }
};

bool isCoplanar(const std::vector<Point3D>& points); 

class Polygon3D {
public:
    // Constructor
    Polygon3D(const std::vector<Point3D>& vertices);
    // Getter for vertices
    const std::vector<Point3D>& getVertices() const;
    // Check if a point is contained within the polygon
    bool contains(const Point3D& point) const;

private:
    // vertices of the polygon
    std::vector<Point3D> vertices;
};