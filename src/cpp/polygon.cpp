#include "polygon.hh"
#include "exceptions.hh"
#include <math.h>
#include <algorithm>

bool nearlyEqual(double a, double b)
{
    return fabs(a - b) < EPSILON;
}

bool isCoplanar(const std::vector<Point3D>& points){
    if(points.size() < 3){
        throw NotEnoughVertices();
    }
    // Create equation of plane using first 3 points by taking cross
    // product of two vectors (mathematical vectors, not c++ vectors)
    Point3D p1 = points[0];
    Point3D p2 = points[1];
    Point3D p3 = points[2];

    double x1 = p2.x - p1.x;
    double y1 = p2.y - p1.y;
    double z1 = p2.z - p1.z;
    double x2 = p3.x - p1.x;
    double y2 = p3.y - p1.y;
    double z2 = p3.z - p1.z;

    // Equation of a plane is a*x + b*y + c*z + d = 0
    // coefficients
    double a = y1*z2 - y2*z1;
    double b = x2*z1 - x1*z2;
    double c = x1*y2 - y1*x2;
    // find constant by plugging in a point (p1)
    double d = -1*(a*p1.x + b*p1.y + c*p1.z);

    // Check if other points in points vector (c++ vector) satisfy equation of plane
    for(unsigned int i = 3; i < points.size(); i++){
        Point3D point = points.at(i);
        // Return false if the point doesn't satisfy the plane's equation
        if(
            !nearlyEqual(
                (a*point.x + b*point.y + c*point.z + d),
                0
            )
        ){
            return false;
        }
    }
    return true;
}

Polygon3D::Polygon3D(const std::vector<Point3D>& vertices) {
    if(!isCoplanar(vertices)){
        throw NonCoplanarVertices();
    }
    this->vertices = vertices;
}

const std::vector<Point3D>& Polygon3D::getVertices() const {
    return vertices;
}

bool Polygon3D::contains(const Point3D& point) const {
    // First check if point is in the same plane as the polygon
    std::vector<Point3D> point_vec = { point };
    std::vector<Point3D> check_vec;
    check_vec.insert(check_vec.end(), vertices.begin(), vertices.end());
    check_vec.insert(check_vec.end(), point_vec.begin(), point_vec.end());
    if (!isCoplanar(check_vec)) {
        return false;
    }
    /*
    Since we now know that the point is on the same plane as the polygon,
    we can project the polygon into 2d space and use a 2d algorithm to check if 
    point is contained within the polygon. Any plane will work, we'll delete the
    coordinate with the smallest range for numerical stability.
    */

    double minX = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::min();
    double minY = std::numeric_limits<double>::max();
    double maxY = std::numeric_limits<double>::min();
    double minZ = std::numeric_limits<double>::max();
    double maxZ = std::numeric_limits<double>::min();
    for(const auto& vertex: vertices){
        minX = std::min(minX, vertex.x);
        minY = std::min(minY, vertex.y);
        minZ = std::min(minZ, vertex.z);
        maxX = std::max(maxX, vertex.x);
        maxY = std::max(maxY, vertex.y);
        maxZ = std::max(maxZ, vertex.z);
    }
    std::vector<double> ranges = {(maxX-minX), (maxY-minY), (maxZ-minZ)};
    int smallest_range = std::distance(std::begin(ranges), std::min_element(std::begin(ranges), std::end(ranges)));

    // Decide which coordinates we care about (not smallest range).
    int coor_1, coor_2;
    
    switch(smallest_range){
        case 0:
            coor_1 = 1;
            coor_2 = 2;
            break;
        case 1:
            coor_1 = 0;
            coor_2 = 2;
            break;
        case 2:
            coor_1 = 0;
            coor_2 = 1;
            break;
    }

    // ray casting algorithm in 2d, see https://wrfranklin.org/Research/Short_Notes/pnpoly.html for explanation
    // We will fix 
    bool contains = false;
    // j is always 1 less than i
    for (int i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++){
        Point3D vertex1 = vertices[j];
        Point3D vertex2 = vertices[i];
        if (
            (
                // Check that the "y" coordinate (coor_2) is between vertex1 and vertex2
                (vertex1[coor_2] > point[coor_2]) != (vertex2[coor_2] > point[coor_2])
            ) && (
                // check if point is to the left of intersection point using point-slope
                point[coor_1] < 
                (
                    (vertex1[coor_1] - vertex2[coor_1]) / (vertex1[coor_2]-vertex2[coor_2]) 
                    * (point[coor_2]-vertex2[coor_2]) 
                    + vertex2[coor_1]
                )
            )
        ){
            contains = !contains;
        }
    }
    return contains;

}