#include <catch2/catch_test_macros.hpp>
#include<iostream>
#include "DataStruct_class.h"
#include "DataAnalysis.h"
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("check_type() is checked", "[check_type()]"){
    string s = "meow";
    CAPTURE(s);
    CHECK(DataFrame::check_type(s) == "string");
    s = ".";
    CHECK(DataFrame::check_type(s) == "string");
    s = "   .    ";
    CHECK(DataFrame::check_type(s) == "string");
    s= "4 2";
    CHECK(DataFrame::check_type(s) == "string");
    s= "-";
    CHECK(DataFrame::check_type(s) == "string");
    s= "+";
    CHECK(DataFrame::check_type(s) == "string");
    s= "1.2.3";
    CHECK(DataFrame::check_type(s) == "string");
    s= "--5";
    CHECK(DataFrame::check_type(s) == "string");
    s= "abc12";
    CHECK(DataFrame::check_type(s) == "string");
    s= "0x1F";
    CHECK(DataFrame::check_type(s) == "string");
    INFO("hi1");
}

TEST_CASE("check_type() with int64_t is checked", "[check_type_int()]"){
    string s = "42";
    CAPTURE(s);
    CHECK(DataFrame::check_type(s) == "int64_t");
    s = "    43";
    CHECK(DataFrame::check_type(s) == "int64_t");
    s = "   43   ";
    CHECK(DataFrame::check_type(s) == "int64_t");
    s= "007";
    CHECK(DataFrame::check_type(s) == "int64_t");

    INFO("hi2");
}

TEST_CASE("check_type() with double is checked", "[check_type_double()]"){
    string s = "5.3";
    CAPTURE(s);
    CHECK(DataFrame::check_type(s) == "double");
    s = "-.5";
    CHECK(DataFrame::check_type(s) == "double");
    s = "0.0";
    CHECK(DataFrame::check_type(s) == "double");
    s= "-3.14";
    CHECK(DataFrame::check_type(s) == "double");
    s= "5.";
    CHECK(DataFrame::check_type(s) == "double");

    INFO("hi2");
}

TEST_CASE("Data Ingestion", "[data_ingestion]"){
    DataFrame::DataStruct my_data = DataFrame::DataStruct("diabetes_1.csv", true);
    REQUIRE(my_data.get_row_count()>0);

    SECTION("Execution of one_hot encoding"){
        DataFrame::DataStruct one_hot_encoded = my_data.one_hot_encoding("smoking_history");
        REQUIRE(one_hot_encoded.get_row_count() == my_data.get_row_count());
    }

    SECTION("Covariance check"){
        auto age_col = get<vector<double>>(my_data.get_column("age"));
        auto glucose_col = get<vector<int64_t>>(my_data.get_column("blood_glucose_level")); 
        double calculated_covariance = DataAnalysis::covariance<double, int64_t>(age_col, glucose_col);
        CHECK_THAT(calculated_covariance, Catch::Matchers::WithinRel(-18.7, 0.01));
    }

}