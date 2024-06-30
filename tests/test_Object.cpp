#include "catch2/catch_test_macros.hpp"

#define private public
#define protected public

#include "Object.hpp"
#include "List.hpp"

#undef private
#undef protected

using JSONJay::Object;
using JSONJay::List;

TEST_CASE("Object operations", "[Object]") {
    SECTION("Empty object") {
        GIVEN("An empty object") {
            JSONJay::Object obj;

            THEN("The object should be empty") {
                REQUIRE(obj.empty());
                REQUIRE(obj.size() == 0);
            }
        }
    }

    SECTION("Adding elements") {
        GIVEN("An empty object") {
            JSONJay::Object obj;

            WHEN("Adding elements to the object") {
                obj.set("key1", 42);
                obj.set("key2", "Hello");
                obj.set("key3", true);
                obj.set("key4", 3.14);
                obj.set("key5", new List());
                obj.set("key6", new Object());

                THEN("The object size should be updated") {
                    REQUIRE(obj.size() == 6);
                }

                THEN("The added elements should be accessible") {
                    REQUIRE(obj.get_int("key1") == 42);
                    REQUIRE(obj.get_string("key2") == "Hello");
                    REQUIRE(obj.get_bool("key3") == true);
                    REQUIRE(obj.get_double("key4") == 3.14);
                    REQUIRE(obj.get_list("key5").empty());
                    REQUIRE(obj.get_object("key6").empty());
                }
            }
        }
    }

    SECTION("Removing elements") {
        GIVEN("An object with elements") {
            JSONJay::Object obj;
            obj.set("key1", 1);
            obj.set("key2", 2);
            obj.set("key3", 3);

            WHEN("Removing an element from the object") {
                obj.erase("key2");

                THEN("The object size should be updated") {
                    REQUIRE(obj.size() == 2);
                }

                THEN("The remaining elements should be correct") {
                    REQUIRE(obj.get_int("key1") == 1);
                    REQUIRE(obj.get_int("key3") == 3);
                }
            }
        }
    }

    SECTION("Checking keys") {
        GIVEN("An object with elements") {
            JSONJay::Object obj;
            obj.set("key1", 1);
            obj.set("key2", 2);
            obj.set("key3", 3);

            WHEN("Checking for valid keys without throwing") {
                THEN("Valid keys should return true") {
                    REQUIRE(obj.check_key_valid("key1", false));
                    REQUIRE(obj.check_key_valid("key2", false));
                    REQUIRE(obj.check_key_valid("key3", false));
                }

                THEN("Invalid keys should return false") {
                    REQUIRE_FALSE(obj.check_key_valid("", false));
                    REQUIRE_FALSE(obj.check_key_valid("key 1", false));
                }
            }
            AND_WHEN("Checking for valid keys with throwing") {
                THEN("Valid keys should return true") {
                    REQUIRE(obj.check_key_valid("key1", true));
                    REQUIRE(obj.check_key_valid("key2", true));
                    REQUIRE(obj.check_key_valid("key3", true));
                }

                THEN("Invalid keys should throw an exception") {
                    REQUIRE_THROWS_AS(obj.check_key_valid("", true), JSONJay::InvalidKeyException);
                    REQUIRE_THROWS_AS(obj.check_key_valid("key 1", true), JSONJay::InvalidKeyException);
                }
            }

            WHEN("Checking for existing keys") {
                THEN("Existing keys should return true") {
                    REQUIRE(obj.check_key_exists("key1"));
                    REQUIRE(obj.check_key_exists("key2"));
                    REQUIRE(obj.check_key_exists("key3"));
                }

                THEN("Non-existing keys should return false") {
                    REQUIRE_FALSE(obj.check_key_exists("key4"));
                    REQUIRE_FALSE(obj.check_key_exists("key5"));
                    REQUIRE_FALSE(obj.check_key_exists("key6"));
                }

            }
            AND_WHEN("Checking for existing keys with collision") {

                THEN("Existing keys should throw an exception") {
                    REQUIRE_THROWS_AS(obj.check_key_exists("key1", true, true), JSONJay::InvalidKeyException);
                    REQUIRE_THROWS_AS(obj.check_key_exists("key2", true, true), JSONJay::InvalidKeyException);
                    REQUIRE_THROWS_AS(obj.check_key_exists("key3", true, true), JSONJay::InvalidKeyException);
                }

                THEN("Non-existing keys should throw an exception") {
                    REQUIRE_THROWS_AS(obj.check_key_exists("key4", true, false), JSONJay::InvalidKeyException);
                    REQUIRE_THROWS_AS(obj.check_key_exists("key5", true, false), JSONJay::InvalidKeyException);
                    REQUIRE_THROWS_AS(obj.check_key_exists("key6", true, false), JSONJay::InvalidKeyException);
                }
            }
        }
    }

    SECTION("Getting Types") {
        GIVEN("An object with elements") {
            JSONJay::Object obj;
            obj.set("key1", 1);
            obj.set("key2", 2.0);
            obj.set("key3", "Hello");
            obj.set("key4", true);
            obj.set("key5", new List());
            obj.set("key6", new Object());

            THEN("The types of the elements should be correct") {
                REQUIRE(obj.get_type("key1") == JSONJay::BaseDataType::INT);
                REQUIRE(obj.get_type("key2") == JSONJay::BaseDataType::DOUBLE);
                REQUIRE(obj.get_type("key3") == JSONJay::BaseDataType::STRING);
                REQUIRE(obj.get_type("key4") == JSONJay::BaseDataType::BOOL);
                REQUIRE(obj.get_type("key5") == JSONJay::BaseDataType::LIST);
                REQUIRE(obj.get_type("key6") == JSONJay::BaseDataType::OBJECT);
            }
        }
    }

    SECTION("Iterating over elements") {
        GIVEN("An object with elements") {
            JSONJay::Object obj;
            obj.set("key1", 1);
            obj.set("key2", 3);
            obj.set("key3", 3);
            obj.set("key8", 7);

            WHEN("Iterating over the elements") {
                std::vector<std::string> keys;
                std::vector<int> values;
                for ( auto [key, value, type] : obj ) {
                    REQUIRE(type == JSONJay::BaseDataType::INT);
                    keys.push_back(key);
                    values.push_back(std::get<int>(value));
                }

                THEN("The keys should be correct") {
                    REQUIRE(keys.size() == 4);
                    REQUIRE(keys[0] == "key1");
                    REQUIRE(keys[1] == "key2");
                    REQUIRE(keys[2] == "key3");
                    REQUIRE(keys[3] == "key8");
                }
                AND_THEN("The values should be correct") {
                    REQUIRE(values.size() == 4);
                    REQUIRE(values[0] == 1);
                    REQUIRE(values[1] == 3);
                    REQUIRE(values[2] == 3);
                    REQUIRE(values[3] == 7);
                }
            }
        }
    }
}