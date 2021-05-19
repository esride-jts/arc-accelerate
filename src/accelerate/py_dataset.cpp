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
                string field_type = py_field.attr("type").cast<string>();
                auto field_pos = find(field_names.begin(), field_names.end(), field_name);
                if (field_names.end() != field_pos)
                {
                    // Field name was defined
                    mg::Field field(field_name, field_type);
                    string field_alias = py_field.attr("aliasName").cast<string>();
                    size_t field_length = py_field.attr("length").cast<size_t>();
                    field.set_alias(field_alias);
                    field.set_length(field_length);

                    // Field should be read for the specified table
                    _fields.push_back(field);
                    validated_field_names.push_back(field_name);
                }
                else
                {
                    // Special fields treatment
                    if (0 == field_type.compare("OID"))
                    {
                        auto oid_field_pos = find(field_names.begin(), field_names.end(), "OID@");
                        if (field_names.end() != oid_field_pos)
                        {
                            mg::Field oid_field(field_name, field_type);
                            string field_alias = py_field.attr("aliasName").cast<string>();
                            size_t field_length = py_field.attr("length").cast<size_t>();
                            oid_field.set_alias(field_alias);
                            oid_field.set_length(field_length);

                            // OID field should be read for the specified table
                            _fields.push_back(oid_field);
                            validated_field_names.push_back("OID@");
                        }
                    }
                    else if (0 == field_type.compare("Geometry"))
                    {
                        auto shape_field_pos = find(field_names.begin(), field_names.end(), "SHAPE@");
                        if (field_names.end() != shape_field_pos)
                        {
                            mg::Field shape_field(field_name, field_type);
                            string field_alias = py_field.attr("aliasName").cast<string>();
                            size_t field_length = py_field.attr("length").cast<size_t>();
                            shape_field.set_alias(field_alias);
                            shape_field.set_length(field_length);

                            // Shape field should be read for the specified table
                            _fields.push_back(shape_field);
                            validated_field_names.push_back("SHAPE@");
                        }
                    }
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
            mg::Table new_table = workspace.create_table(out_table, true);

            // Validate the output field names
            vector<size_t> field_indices;
            vector<mg::Field> validated_out_fields;
            vector<string> validated_out_field_names;
            for (const string& out_field_name : out_fields)
            {
                auto field_pos = find_if(_fields.begin(), _fields.end(), [&] (const mg::Field& field) {
                    return 0 == out_field_name.compare(field.name());
                });
                if (_fields.end() != field_pos)
                {
                    size_t field_index = distance(_fields.begin(), field_pos);
                    field_indices.push_back(field_index);
                    mg::Field field = *field_pos;
                    validated_out_fields.push_back(field);
                    validated_out_field_names.push_back(field.name());
                }
                else
                {
                    // TODO: Field not found exception?
                }
            }

            // Add output fields
            new_table.add_fields(validated_out_fields);

            // Use an insert cursor to write all records into the table
            py::object da = py::module::import("arcpy.da");
            py::object InsertCursor = da.attr("InsertCursor");
            py::object insert_cursor = InsertCursor(out_table, validated_out_field_names);
            for (const py::object& row : _rows)
            {
                // Restrict out values
                py::tuple values = row;
                py::tuple out_values(field_indices.size());
                size_t value_index = 0;
                for (size_t field_index : field_indices)
                {
                    out_values[value_index++] = values[field_index];
                }                
                insert_cursor.attr("insertRow")(out_values);
            }
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