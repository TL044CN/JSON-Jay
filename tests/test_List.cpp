#include "catch2/catch_test_macros.hpp"
#include "List.hpp"
#include "Object.hpp"

using JSONJay::List;
using JSONJay::Object;

TEST_CASE("List operations", "[List]") {
    SECTION("Empty list") {
        GIVEN("An empty list") {
            List list;

            THEN("The list should be empty") {
                REQUIRE(list.empty());
                REQUIRE(list.size() == 0);
            }
        }
    }

    SECTION("Adding elements") {
        GIVEN("An empty list") {
            List list;

            WHEN("Adding elements to the list") {
                list.push_back(42);
                list.push_back("Hello");
                list.push_back(true);

                THEN("The list size should be updated") {
                    REQUIRE(list.size() == 3);
                }

                THEN("The added elements should be accessible") {
                    REQUIRE(list.get_int(0) == 42);
                    REQUIRE(list.get_string(1) == "Hello");
                    REQUIRE(list.get_bool(2) == true);
                }
            }
        }
    }

    SECTION("Removing elements") {
        GIVEN("A list with elements") {
            List list;
            list.push_back(1);
            list.push_back(2);
            list.push_back(3);

            WHEN("Removing an element from the list") {
                list.erase(1);

                THEN("The list size should be updated") {
                    REQUIRE(list.size() == 2);
                }

                THEN("The remaining elements should be correct") {
                    REQUIRE(list.get_int(0) == 1);
                    REQUIRE(list.get_int(1) == 3);
                }
            }
        }
    }

    SECTION("Nested lists") {
        GIVEN("A list with a nested list") {
            List innerList;
            innerList.push_back(10);
            innerList.push_back(20);

            List list;
            list.push_back(std::move(innerList));

            THEN("The list size should be updated") {
                REQUIRE(list.size() == 1);
            }

            THEN("The nested list should be accessible") {
                REQUIRE(list.get_list(0).size() == 2);
                REQUIRE(list.get_list(0).get_int(0) == 10);
                REQUIRE(list.get_list(0).get_int(1) == 20);
            }
        }
    }
    
    SECTION("Modifying elements") {
        GIVEN("A list with elements") {
            List list;
            list.push_back(1);
            list.push_back(2);
            list.push_back(3);

            WHEN("Modifying an element in the list") {
                list.get_int(1) = 4;

                THEN("The modified element should be updated") {
                    REQUIRE(list.get_int(1) == 4);
                }
            }
        }
    }

    SECTION("Iterating over the list") {
        GIVEN("A list with elements") {
            List list;
            list.push_back(1);
            list.push_back(2);
            list.push_back(3);

            WHEN("Iterating over the list") {
                size_t sum = 0;
                for (const auto& [element, type] : list) {
                    sum += std::get<int>(element);
                }

                THEN("The sum of the elements should be correct") {
                    REQUIRE(sum == 6);
                }
            }
        }
    }

    SECTION("Clearing the list") {
        GIVEN("A list with elements") {
            List list;
            list.push_back(1);
            list.push_back(2);
            list.push_back(3);

            WHEN("Clearing the list") {
                list.clear();

                THEN("The list should be empty") {
                    REQUIRE(list.empty());
                    REQUIRE(list.size() == 0);
                }
            }
        }
    }

    SECTION("Getting the type of an element") {
        GIVEN("A list with elements") {
            List list;
            list.push_back(1);
            list.push_back(2.0);
            list.push_back("Hello");
            list.push_back(true);
            list.push_back(JSONJay::Object());


            THEN("The types of the elements should be correct") {
                REQUIRE(list.get_type(0) == JSONJay::BaseDataType::INT);
                REQUIRE(list.get_type(1) == JSONJay::BaseDataType::DOUBLE);
                REQUIRE(list.get_type(2) == JSONJay::BaseDataType::STRING);
                REQUIRE(list.get_type(3) == JSONJay::BaseDataType::BOOL);
                REQUIRE(list.get_type(4) == JSONJay::BaseDataType::OBJECT);
            }
        }
    }

    SECTION("Retrieving an element of a type"){
        GIVEN("A list with elements") {
            List list;
            list.push_back(1);
            list.push_back(2.0);
            list.push_back("Hello");
            list.push_back(true);
            list.push_back(JSONJay::Object());

            THEN("The elements should be accessible") {
                REQUIRE(list.get_int(0) == 1);
                REQUIRE(list.get_double(1) == 2.0);
                REQUIRE(list.get_string(2) == "Hello");
                REQUIRE(list.get_bool(3) == true);
                REQUIRE(list.get_object(4).empty());
            }
            AND_THEN("The elements should be accessible with the at method") {
                REQUIRE(list.at<int>(0) == 1);
                REQUIRE(list.at<double>(1) == 2.0);
                REQUIRE(list.at<std::string>(2) == "Hello");
                REQUIRE(list.at<bool>(3) == true);
                REQUIRE(list.at<JSONJay::Object>(4).empty());
            }
            AND_THEN("The at function should throw an exception if the type is incorrect") {
                REQUIRE_THROWS_AS(list.at<List>(1), JSONJay::InvalidTypeException);
                REQUIRE_THROWS_AS(list.at<List*>(0), JSONJay::InvalidTypeException);
            }
            AND_THEN("The elements should be accessible with the operator[] method") {
                REQUIRE(std::get<int>(list[0]) == 1);
                REQUIRE(std::get<double>(list[1]) == 2.0);
                REQUIRE(std::get<std::string>(list[2]) == "Hello");
                REQUIRE(std::get<bool>(list[3]) == true);
                REQUIRE(std::get<Object*>(list[4])->empty());
            }
        }
    }
}