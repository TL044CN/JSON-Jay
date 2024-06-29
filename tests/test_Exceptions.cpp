#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_exception.hpp"
#include "Exceptions.hpp"

TEST_CASE("Exceptions", "[Exceptions]") {
    SECTION("JSONJay::Exception") {
        GIVEN("An exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::JSONJayException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::JSONJayException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::JSONJayException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::JSONJayException(), "JSONJay Exception: Unknown Exception");
            }
        }
        AND_GIVEN("An invalid format exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::InvalidFormatException("Test exception"), JSONJay::InvalidFormatException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidFormatException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidFormatException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidFormatException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidFormatException(), "JSONJay Exception: Invalid Format");
            }
        }
        AND_GIVEN("An invalid type exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::InvalidTypeException("Test exception"), JSONJay::InvalidTypeException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidTypeException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidTypeException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidTypeException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidTypeException(), "JSONJay Exception: Invalid Type");
            }
        }
        AND_GIVEN("An invalid key exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::InvalidKeyException("Test exception"), JSONJay::InvalidKeyException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidKeyException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidKeyException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidKeyException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidKeyException(), "JSONJay Exception: Invalid Key");
            }
        }
        AND_GIVEN("An invalid Vaule exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::InvalidValueException("Test exception"), JSONJay::InvalidValueException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidValueException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidValueException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidValueException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidValueException(), "JSONJay Exception: Invalid Value");
            }
        }
        AND_GIVEN("An invalid Index exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::InvalidIndexException("Test exception"), JSONJay::InvalidIndexException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidIndexException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::InvalidIndexException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidIndexException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::InvalidIndexException(), "JSONJay Exception: Invalid Index");
            }
        }
        AND_GIVEN("A MissingImplementation exception") {
            THEN("The exception type should be correct") {
                REQUIRE_THROWS_AS(throw JSONJay::MissingImplementationException("Test exception"), JSONJay::MissingImplementationException);
                REQUIRE_THROWS_AS(throw JSONJay::MissingImplementationException("Test exception"), JSONJay::JSONJayException);
                REQUIRE_THROWS_AS(throw JSONJay::MissingImplementationException("Test exception"), std::exception);
            }
            THEN("The exception message should be correct") {
                REQUIRE_THROWS_WITH(throw JSONJay::MissingImplementationException("Test exception"), "JSONJay Exception: Test exception");
                REQUIRE_THROWS_WITH(throw JSONJay::MissingImplementationException(), "JSONJay Exception: Missing Implementation");
            }
        }
    }
}