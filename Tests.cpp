#include <catch2/catch_test_macros.hpp>
#include<iostream>
#include "DataStruct_class.h"

TEST_CASE("check_type() is checked", "[check_type()]"){
    string s = "meow";
    REQUIRE(DataFrame::check_type(s) == "string");
    s = ".";
    REQUIRE(DataFrame::check_type(s) == "string");
    s = "   .    ";
    REQUIRE(DataFrame::check_type(s) == "string");
    INFO("hi1");
}

TEST_CASE("check_type() with int is checked", "[check_type_int()]"){
    string s = "42";
    REQUIRE(DataFrame::check_type(s) == "int");
    s = "    43";
    REQUIRE(DataFrame::check_type(s) == "int");
    s = "   43   ";
    REQUIRE(DataFrame::check_type(s) == "int");

    INFO("hi2");
}