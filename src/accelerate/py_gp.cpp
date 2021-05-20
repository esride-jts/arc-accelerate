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

#include "py_gp.hpp"

using namespace std;

namespace accelerate {

    namespace gp {

        Parameter Parameter::create_input_feature_param(const string& display_name, const string& name)
        {
            Parameter input_param;
            input_param._display_name = display_name;
            input_param._name = name;
            input_param._parameter_type = "GPFeatureLayer";
            input_param._direction = "Input";
            return input_param;
        }

        Parameter Parameter::create_output_feature_param(const string& display_name, const string& name)
        {
            Parameter output_param;
            output_param._display_name = display_name;
            output_param._name = name;
            output_param._parameter_type = "GPFeatureLayer";
            output_param._direction = "Output";
            return output_param;
        }

    }

}