include ($ENV{vcpkgRoot}/../scripts/buildsystems/vcpkg.cmake)

find_package (pybind11 CONFIG REQUIRED)

pybind11_add_module (accelerate py_accelerate.cpp py_accelerate.hpp py_dataset.cpp py_dataset.hpp)