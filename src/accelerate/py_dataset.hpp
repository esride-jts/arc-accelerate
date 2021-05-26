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

#ifndef DATASET_H
#define DATASET_H

#include <map>
#include <string>
#include <vector>

namespace pybind11 {
    class object;
}

namespace accelerate {
    namespace management {
        class Field;
    }

    namespace da {

        class Dataset {

            public:
                void read(const std::string& in_table, const std::vector<std::string>& field_names, const std::string& where_clause);

                void write(const std::string& out_path, const std::string& out_table, const std::vector<std::string>& out_fields);

                pybind11::object to_pandas() const;

            private:
                std::vector<management::Field> _fields;
                std::vector<pybind11::object> _rows;
        };



        template<typename TValue>
        class Datacolumn {
            
            public:
                void set_name(const std::string& name)
                {
                    _name = name;
                }

                std::string name() const
                {
                    return _name;
                }

                void add_value(TValue value)
                {
                    _values.push_back(value);
                }

                void add_value_ref(const TValue& value)
                {
                    _values.push_back(value);
                }

            private:
                std::vector<TValue> _values;
                std::string _name;
        };

        template<typename... TValues>
        class Datatable {

            public:
                Datatable(Datacolumn<TValues>... columns) : _columns(std::move(columns)...)
                {}

            private:
                std::tuple<Datacolumn<TValues>...> _columns;
        };

        class TypedDataset {

            public:
                template<typename... TValues>
                void read(const std::string& in_table, const Datacolumn<TValues>&... columns, const std::string& where_clause);
        };
    }
}

#endif