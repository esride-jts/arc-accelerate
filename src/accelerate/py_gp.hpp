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

#ifndef GP_H
#define GP_H

#include <string>

namespace accelerate {

    namespace gp {

        class Parameter {

            public:
                Parameter static create_input_feature_param(const std::string& display_name, const std::string& name);
                Parameter static create_output_feature_param(const std::string& display_name, const std::string& name);

            private:
                std::string _display_name;
                std::string _name;
                std::string _datatype;
                std::string _parameter_type;
                std::string _direction;
        };
    }
}

#endif