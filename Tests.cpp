#include <catch2/catch_test_macros.hpp>
#include<iostream>
#include "DataStruct_class.h"

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

TEST_CASE("check_type() with int is checked", "[check_type_int()]"){
    string s = "42";
    CAPTURE(s);
    CHECK(DataFrame::check_type(s) == "int");
    s = "    43";
    CHECK(DataFrame::check_type(s) == "int");
    s = "   43   ";
    CHECK(DataFrame::check_type(s) == "int");
    s= "007";
    CHECK(DataFrame::check_type(s) == "int");

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