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

namespace py = pybind11;

using namespace std;

namespace accelerate {
    
    namespace da {

        void Dataset::load(const string& in_table, const vector<string>& field_names, const string& where_clause="1=1")
        {
            // Use a search cursor and load all records into the memory
            py::object da = py::module::import("arcpy.da");
            py::object SearchCursor = da.attr("SearchCursor");
            py::object search_cursor = SearchCursor(in_table, field_names, where_clause);
            _field_names = field_names;
            bool determine_value_types = true;
            while (true)
            {
                try
                {
                    py::object row = search_cursor.attr("next")();
                    if (determine_value_types)
                    {
                        py::tuple values = row;
                        size_t field_index = 0;
                        for (auto const value : values)
                        {
                            PyTypeObject* value_type = Py_TYPE(value.ptr());
                            _field_types.insert({
                                field_index++,
                                value_type->tp_name
                            });
                        }
                        determine_value_types = false;
                    }
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
                    if (_field_names.size() == field_index)
                    {
                        break;
                    }

                    string field_name = _field_names[field_index];
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