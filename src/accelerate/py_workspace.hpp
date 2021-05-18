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

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <string>
#include <vector>

namespace accelerate {

    namespace management {

        class Field {

            public:
                Field(const std::string& name, const std::string& field_type);

                void set_alias(const std::string& alias_name);
                void set_length(size_t field_length);

                std::string name() const;
                std::string field_type() const;
                std::string alias() const;
                size_t length() const;

            private:
                std::string _name;
                std::string _field_type;
                std::string _alias_name;
                size_t _field_length;
        };



        class Table {

            public:
                Table(const std::string& workspace_path, const std::string& table_name);

                void add_fields(const std::vector<Field>& fields);

            private:
                std::string _workspace_path;
                std::string _table_name;

                std::vector<Field> _fields;
        };



        class Workspace {

            public:
                Table create_table(const std::string& out_path, const std::string& out_name, bool overwrite);
        };
    }
}

#endif