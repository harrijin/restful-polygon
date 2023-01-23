#pragma once
#include <exception>

class NotEnoughVertices : public std::exception{
    public:
        const char* what() const throw(){
            return "Must provide at least 3 coplanar vertices to form a polygon.";
        }
};

class NonCoplanarVertices : public std::exception{
    public:
        const char* what() const throw(){
            return "Vertices must be coplanar.";
        }
};