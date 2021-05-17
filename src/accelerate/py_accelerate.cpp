//   This file is part of arc-accelerate and enables the development of Geoprocessing Tools using C++.
//   Copyright (C) 2021 Esri Deutschland GmbH
//   Contact: Jan Tschada (j.tschada@esri.de)
//   
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//   
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU Lesser General Public License for more details.
//   
//   You should have received a copy of the GNU Lesser General Public License
//   along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "py_accelerate.hpp"
#include "py_dataset.hpp"

namespace py = pybind11;
namespace da = accelerate::da;

PYBIND11_MODULE(accelerate, m) {
    m.doc() = "Accelerates the development of Geoprocessing tools using C++.";
    
    py::class_<da::Dataset>(m, "Dataset")
        .def(py::init<>())
        .def("load", &da::Dataset::load, "Loads the table/feature class by using the specified path.")
        .def("to_pandas", &da::Dataset::to_pandas, "Creates a new pandas Dataframe containing all rows/featurs of this dataset.")
        .def("__repr__",
            [] (const da::Dataset& da)
            {
                return "Dataset representing a table or feature class.";
            });

    
    // Load arcpy...
    py::module::import("arcpy");
}