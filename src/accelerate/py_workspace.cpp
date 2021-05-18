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

#include "py_workspace.hpp"
#include "py_accelerate.hpp"

namespace py = pybind11;

using namespace std;

namespace accelerate {

    namespace management {

        Field::Field(const std::string& name, const std::string& field_type) : 
            _name(name), _field_type(field_type),
            _alias_name(name), _field_length(0)
        {}

        void Field::set_alias(const string& alias_name)
        {
            _alias_name = alias_name;
        }

        void Field::set_length(size_t field_length)
        {
            _field_length = field_length;
        }

        string Field::name() const
        {
            return _name;
        }

        string Field::field_type() const
        {
            return _field_type;
        }

        string Field::alias() const
        {
            return _alias_name;
        }

        size_t Field::length() const
        {
            return _field_length;
        }



        Table::Table(const string& workspace_path, const string& table_name) :
            _workspace_path(workspace_path), _table_name(table_name)
        {}

        void Table::add_fields(const std::vector<Field>& fields)
        {
            _fields = fields;
        }



        Table Workspace::create_table(const string& out_path, const string& out_name, bool overwrite)
        {
            // If no error is thrown the table should be created
            py::object arcpy = py::module::import("arcpy");
            py::object env = arcpy.attr("env");
            env.attr("overwriteOutput") = overwrite;
            py::object CreateTable = arcpy.attr("CreateTable_management");
            py::object new_table = CreateTable(out_path, out_name);
            // TODO: Maybe we should inject the pyobject into the table instance?

            return Table(out_path, out_name);
        }

    }

}