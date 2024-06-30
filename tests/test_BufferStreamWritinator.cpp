#include "catch2/catch_test_macros.hpp"
#include "BufferStreamWritinator.hpp"

#include <iostream>


class TestSerializable {
private:
    int m_int;
    std::string m_string;
    std::vector<int> m_vector;
    std::map<int, std::string> m_map;
    std::unordered_map<int, std::string> m_unorderedMap;
public:

    TestSerializable() = default;

    TestSerializable(
        int i, std::string s, std::vector<int> v, std::map<int, std::string> m,
        std::unordered_map<int, std::string> um)
        : m_int(i), m_string(s), m_vector(v), m_map(m), m_unorderedMap(um) {}

    void serialize(JSONJay::StreamWritinator* writinator) const {
        writinator->writeRaw(m_int);
        writinator->writeString(m_string);
        writinator->writeVector(m_vector);
        writinator->writeMap(m_map);
        writinator->writeMap(m_unorderedMap);
    }

    static void serialize(JSONJay::StreamWritinator* writinator, const TestSerializable& testSerializable) {
        testSerializable.serialize(writinator);
    }

};

TEST_CASE("BufferStreamWritinator::writeData", "[BufferStreamWritinator]") {
    GIVEN("A BufferStreamWritinator object") {
        JSONJay::BufferStreamWritinator bufferStreamWritinator;
        WHEN("writeData is called") {
            std::vector<char> expected = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!' };
            bufferStreamWritinator.writeData(expected.data(), expected.size());

            THEN("The Position is updated") {
                REQUIRE(bufferStreamWritinator.getStreamPosition() == expected.size());
            }

            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }

        WHEN("writeBuffer is called with a Buffer and writeSize is true") {
            std::vector<char> data = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!' };
            std::vector<char> expected = {
                13, 0, 0, 0, 0, 0, 0, 0, // size of the buffer
                'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'
            };

            bufferStreamWritinator.writeBuffer(data, true);

            THEN("The Position is updated") {
                REQUIRE(bufferStreamWritinator.getStreamPosition() == expected.size());
            }

            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }

        WHEN("writeBuffer is called with a Buffer and writeSize is false") {
            std::vector<char> data = { 'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!' };
            std::vector<char> expected = {
                'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'
            };

            bufferStreamWritinator.writeBuffer(data, false);
            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }

        WHEN("writeRaw is called with an int") {
            int data = 42;
            std::vector<char> expected = { 42, 0, 0, 0 };

            bufferStreamWritinator.writeRaw(data);
            THEN("The data is written to the buffer") {
                REQUIRE(bufferStreamWritinator.getBuffer() == expected);
            }
        }

        WHEN("writeVector is called with a vector with writeSize true") {
            std::vector<int> data = { 1, 2, 3, 4, 5 };
            std::vector<char> expected = {
                5, 0, 0, 0,
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
                5, 0, 0, 0 
            };

            bufferStreamWritinator.writeVector(data);

            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }

        WHEN("writeVector is called with a vector with writeSize false") {
            std::vector<int> data = { 1, 2, 3, 4, 5 };
            std::vector<char> expected = {
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
                5, 0, 0, 0 
            };

            bufferStreamWritinator.writeVector(data, false);

            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }


        WHEN("writeString is called with a string and writeSize true") {
            std::string data = "Hello, World!";
            std::vector<char> expected = {
                13, 0, 0, 0, 0, 0, 0, 0, // size of the string
                'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'
            };

            bufferStreamWritinator.writeString(data);

            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }

        WHEN("writeData is called with a serializable object") {
            TestSerializable testSerializable(
                42,
                "Hello, World!",
                { 1, 2, 3, 4, 5 },
                { {1, "One"}, {2, "Two"}, {3, "Three"} },
                { {1, "One"}, {2, "Two"}, {3, "Three"} }
            );
            std::vector<char> expected = {
                42, 0, 0, 0,
                13, 0, 0, 0, 0, 0, 0, 0, // size of the string
                'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!',
                5, 0, 0, 0,              // size of the vector
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
                5, 0, 0, 0,
                3, 0, 0, 0,             // size of the map
                1, 0, 0, 0,
                3, 0, 0, 0, 0, 0, 0, 0, 'O', 'n', 'e',
                2, 0, 0, 0,
                3, 0, 0, 0, 0, 0, 0, 0, 'T', 'w', 'o',
                3, 0, 0, 0,
                5, 0, 0, 0, 0, 0, 0, 0, 'T', 'h', 'r', 'e', 'e',
                3, 0, 0, 0,             // size of the unordered_map
                3, 0, 0, 0,
                5, 0, 0, 0, 0, 0, 0, 0, 'T', 'h', 'r', 'e', 'e',
                2, 0, 0, 0,
                3, 0, 0, 0, 0, 0, 0, 0, 'T', 'w', 'o',
                1, 0, 0, 0,
                3, 0, 0, 0, 0, 0, 0, 0, 'O', 'n', 'e'
            };
            
            bufferStreamWritinator.writeSerializable(testSerializable);            

            THEN("The data is written to the buffer") {
                REQUIRE(std::equal(expected.begin(), expected.end(), bufferStreamWritinator.getBuffer().begin()));
            }
        }
    }
}