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
#include "py_workspace.hpp"

namespace py = pybind11;
namespace da = accelerate::da;
namespace mg = accelerate::management;

using namespace std;

PYBIND11_MODULE(accelerate, m) {
    m.doc() = "Accelerates the development of Geoprocessing tools using C++.";
    
    py::class_<da::Dataset>(m, "Dataset")
        .def(py::init<>())
        .def("read", &da::Dataset::read, "Reads the table/feature class by using the specified path.")
        .def("write", &da::Dataset::write, "Writes the table/feature class by using the specified path.")
        .def("to_pandas", &da::Dataset::to_pandas, "Creates a new pandas Dataframe containing all rows/featurs of this dataset.")
        .def("__repr__",
            [] (const da::Dataset&)
            {
                return "Dataset representing a table or feature class.";
            });

    py::class_<mg::Table>(m, "Table")
        .def(py::init<string, string>())
        .def("__repr__",
            [] (const mg::Table&)
            {
                return "Table representing a table from a workspace.";
            });

    py::class_<mg::Workspace>(m, "Workspace")
        .def(py::init<>())
        .def("create_table", &mg::Workspace::create_table, "Creates a new table by using the specified path.")
        .def("__repr__",
            [] (const mg::Workspace&)
            {
                return "Workspace representing a local folder or geodatabase.";
            });

    
    // Load arcpy...
    py::module::import("arcpy");
}