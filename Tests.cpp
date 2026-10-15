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
    s= "4 2";
    REQUIRE(DataFrame::check_type(s) == "string");
    s= "-";
    REQUIRE(DataFrame::check_type(s) == "string");
    s= "+";
    REQUIRE(DataFrame::check_type(s) == "string");
    s= "1.2.3";
    REQUIRE(DataFrame::check_type(s) == "string");
    s= "--5";
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
    s= "007";
    REQUIRE(DataFrame::check_type(s) == "int");

    INFO("hi2");
}

TEST_CASE("check_type() with double is checked", "[check_type_double()]"){
    string s = "5.3";
    REQUIRE(DataFrame::check_type(s) == "double");
    s = "-.5";
    REQUIRE(DataFrame::check_type(s) == "double");
    s = "0.0";
    CAPTURE(s);
    REQUIRE(DataFrame::check_type(s) == "double");

    s= "-3.14";
    REQUIRE(DataFrame::check_type(s) == "double");

    INFO("hi2");
}