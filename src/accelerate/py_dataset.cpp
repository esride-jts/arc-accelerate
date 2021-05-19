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

#include "py_dataset.hpp"
#include "py_accelerate.hpp"
#include "py_workspace.hpp"

#include <algorithm>

namespace py = pybind11;
namespace mg = accelerate::management;

using namespace std;

namespace accelerate {
    
    namespace da {

        void Dataset::read(const string& in_table, const vector<string>& field_names, const string& where_clause="1=1")
        {
            // List all fields
            py::object arcpy = py::module::import("arcpy");
            py::object ListFields = arcpy.attr("ListFields");
            py::list py_field_list = ListFields(in_table);
            vector<string> validated_field_names;
            _fields.clear();
            for (auto const py_field : py_field_list)
            {
                string field_name = py_field.attr("name").cast<string>();
                auto field_pos = find(field_names.begin(), field_names.end(), field_name);
                if (field_names.end() != field_pos)
                {
                    // Field name was defined
                    string field_type = py_field.attr("type").cast<string>();
                    string field_alias = py_field.attr("aliasName").cast<string>();
                    size_t field_length = py_field.attr("length").cast<size_t>();
                    mg::Field field(field_name, field_type);
                    field.set_alias(field_alias);
                    field.set_length(field_length);
                    _fields.push_back(field);  

                    // Field exists for the specified table
                    validated_field_names.push_back(field_name); 
                }
            }
            // TODO: Field names which do not exists, should lead to exception?
            
            // Use a search cursor and load all records into the memory
            py::object da = arcpy.attr("da");
            py::object SearchCursor = da.attr("SearchCursor");
            py::object search_cursor = SearchCursor(in_table, validated_field_names, where_clause);
            while (true)
            {
                try
                {
                    py::object row = search_cursor.attr("next")();
                    _rows.push_back(row);
                }
                catch (const py::error_already_set& err)
                {
                    if (err.matches(PyExc_StopIteration))
                    {
                        // Expected at the end of the cursor!
                        break;
                    }
                    else
                    {
                        // Rethrow unexpected error
                        throw;
                    }
                }
            }
        }

        void Dataset::write(const string& out_path, const string& out_table, const std::vector<std::string>& out_fields)
        {
            

            // Create a new table
            mg::Workspace workspace(out_path);

            // Use an insert cursor to write all records into the table
            /*
            py::object da = py::module::import("arcpy.da");
            py::object InsertCursor = da.attr("InsertCursor");
            vector<size_t> field_indices;
            vector<string> validated_out_field_names;
            for (const mg::Field& out_field : out_fields)
            {
                auto field_pos = find(_field_names.begin(), _field_names.end(), out_field_name);
                if (_field_names.end() != field_pos)
                {
                    size_t field_index = distance(_field_names.begin(), field_pos);
                    field_indices.push_back(field_index);
                    validated_out_field_names.push_back(out_field_name);
                }
                else
                {
                    // TODO: Field not found exception?
                }
            }

            py::object insert_cursor = InsertCursor(out_table, validated_out_field_names);
            for (const py::object& row : _rows)
            {
                py::tuple values = row;
                
                // TODO: Restrict out fields
                //values.operator[]
                
                insert_cursor.attr("insertRow")(values);
            }
            */
        }

        py::object Dataset::to_pandas() const
        {
            vector<py::dict> rows_as_dict;
            rows_as_dict.reserve(_rows.size());
            for (const py::object& row : _rows)
            {
                py::dict row_as_dict;
                size_t field_index = 0;
                for (auto const& value : row)
                {
                    if (_fields.size() == field_index)
                    {
                        break;
                    }

                    mg::Field field = _fields[field_index];
                    string field_name = field.name();
                    row_as_dict[field_name.c_str()] = value;

                    field_index++;
                }

                rows_as_dict.push_back(row_as_dict);
            }

            py::object pandas = py::module::import("pandas");
            return pandas.attr("DataFrame").attr("from_dict")(rows_as_dict);
        }

    }
}