# arc-accelerate
Thin Python wrapper for implementing accelerated Geoprocessing Tools using C++.
This is just a proof of concept and should be useful for any developer who is interested in C++ and Python development using ArcGIS.

# Features
The following features are offered to C++:
- In-Memory dataset reading tables and feature classes using a ```Search Cursor```.
- Dataset writing into tables and feature classes using a ```Insert Cursor```.
- Copying the dataset as ```Pandas DataFrame```
- Creating tables by wrapping ```CreateTable_management```

## Notes
The offered C++ modules are compiled with language features of C++11.

## Requirements
For building python modules from C++ the awesome [pybind11](https://github.com/pybind/pybind11) library is used.
The build system relies on [CMake](https://cmake.org/).
The test folder contains the unit tests which are only based on Python.
The Python environment of ArcGIS Pro 2.7 is only supported.

## Contributing
Contributions are welcome from anyone and everyone. Please see our [guidelines](CONTRIBUTING.md).