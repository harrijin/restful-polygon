#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "polygon.hh"

namespace py = pybind11;

PYBIND11_MODULE(polygon3d, m) {
    py::class_<Point3D>(m, "Point3D")
        .def(py::init<double, double, double>());
    py::class_<Polygon3D>(m, "Polygon3D")
        .def(py::init<const std::vector<Point3D>&>())
        .def("contains", &Polygon3D::contains);
}