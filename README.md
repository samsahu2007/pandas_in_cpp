# my_pandas

A small pandas-like DataFrame library: load a CSV, infer column types, run basic analysis, with a Catch2 test suite.

> Status: v0.1, learning project. Not production-ready.

## Features

- Load a CSV into typed columns (`int64_t`, `double`, `string`) stored as `std::variant<std::vector<...>>`
- Automatic type inference per column, with whitespace and `\r\n` handling
- `display()` to print the header and the first 10 rows
- `get_column(name)` to fetch a column by header name
- `one_hot_encoding(name)` for string columns
- `DataAnalysis`: `deviation`, `dot_product`, `covariance` (templated, work on any numeric column type)

### Known limitations

<!-- Be honest here, it reads better than hiding it. Edit as you fix things. -->
- No exponent notation (`1e-5`) in type inference
- Missing values: <!-- describe how empty cells are handled once implemented -->
- No quoted fields (commas inside quotes)
- Single-threaded, whole file loaded in memory

## Requirements

- C++11 compiler
- CMake 3.23+
- [Catch2 v3](https://github.com/catchorg/Catch2) (tests only)

## Build and test

```bash
cmake -S . -B build
cmake --build build
./build/unit_tests \#to run unit tests
```

Disable tests: `cmake -S . -B build -DBUILD_TESTING=OFF`

For memory leaks, fsanitize command is embedded inside testing.

## Usage

```cpp
#include "DataStruct_class.h"
#include "DataAnalysis.h"

int main() {
    DataFrame::DataStruct df("data.csv", /*with_header=*/true);
    df.display();

    auto age     = std::get<std::vector<double>>(df.get_column("age"));
    auto glucose = std::get<std::vector<int64_t>>(df.get_column("blood_glucose_level"));

    double cov = DataAnalysis::covariance(age, glucose);
}
```

## Design notes

<!-- 3-5 lines: why variant instead of templates, why you chose the type inference rules, etc.
     This is the section interviewers actually read. -->
- Columns are heterogeneous, and the types are only known at runtime, so each column is a `std::variant` of vectors.
- Columns with at least one cell value of type `string` are fully designated as `string`.
- Columns with at least one cell value of type `double` but no type: `string` are fully designated as `double`.
- Columns with at least one cell value of type `int64_t` but no type: `string` or `dobule` are fully designated as `int64_t`
- Columns with at least one type `NaN` will throw an error. Handling of empty values has not been done.
- In constructor of class DataStruct, `with_header` flag must be passed false if there is no header row. In that case, DataStruct creates a header row on its own. 
- Procedure flow for DataStruct constructor: Open File ---> parse_stream() created a 2d array using vector of vector of strings ---> init_data_types() decides data types of each column ---> init_columns() creates another 2D array using vectors of types as decided in init_data_types() ---> populate_data_after_checking_type() populates the vector of vectors created by init_columns with the vector of vector of strings created by parse_stream() 
- The function `one_hot_encoding(key)` returns a reference to a `DataStruct` object which is allocated on heap. Hence, it is duty of the user to invoke delete on this object.  

## Project layout

```
DataStruct_class.h/.cpp   DataFrame::DataStruct and check_type()
DataAnalysis.h            statistics helpers (header-only)
Tests.cpp                 Catch2 tests
CMakeLists.txt
```

## Roadmap

- [ ] Missing-value handling
- [ ] Fix `one_hot_encoding` ownership (use move semantics)
- [ ] Exponent notation and NA tokens in `check_type`
- [ ] Quoted CSV fields
- [ ] Explain functioning of one_hot_encoding in README.md
- [ ] More analysis functions (mean, std, correlation)

