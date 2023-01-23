#include <gtest/gtest.h>
#include <vector>
#include "exceptions.hh"
#include "polygon.hh"

TEST(isCoplanarTest, pointAreCoplanar) {
    std::vector<Point3D> coplanarPoints = {
        Point3D(0,0,0),
        Point3D(0,1,0),
        Point3D(1,1,0),
        Point3D(1,0,0)
    };
    std::vector<Point3D> nonCoplanarPoints = {
        Point3D(0,0,0),
        Point3D(0,1,0),
        Point3D(1,0,0),
        Point3D(0,0,1)
    };
    std::vector<Point3D> notEnoughVertices = {
        Point3D(1, 2, 3)
    };

    EXPECT_TRUE(isCoplanar(coplanarPoints));
    EXPECT_FALSE(isCoplanar(nonCoplanarPoints));
    EXPECT_THROW(isCoplanar(notEnoughVertices), NotEnoughVertices);
}

TEST(Polygon3DTest, containsPoints) {
    std::vector<Point3D> coplanarPoints = {
        Point3D(0,0,0),
        Point3D(0,1,0),
        Point3D(1,1,0),
        Point3D(1,0,0)
    };
    std::vector<Point3D> nonCoplanarPoints = {
        Point3D(0,0,0),
        Point3D(0,1,0),
        Point3D(1,0,0),
        Point3D(0,0,1)
    };
    Polygon3D coplanarPolygon(coplanarPoints);
    EXPECT_EQ(coplanarPolygon.getVertices(), coplanarPoints);
    EXPECT_TRUE(coplanarPolygon.contains(Point3D(0.25,0.25,0)));
    // Not coplanar
    EXPECT_FALSE(coplanarPolygon.contains(Point3D(1,1,1)));
    // Coplanar, but doesn't contain
    EXPECT_FALSE(coplanarPolygon.contains(Point3D(1,2,0)));

    EXPECT_THROW(Polygon3D nonCoplanarPolygon(nonCoplanarPoints), NonCoplanarVertices);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}