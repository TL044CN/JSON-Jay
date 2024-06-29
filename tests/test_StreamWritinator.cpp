#include "catch2/catch_test_macros.hpp"
#include "StreamWritinator.hpp"

#include <iostream>

#include <cstdint>
#include <sstream>

using JSONJay::StreamWritinator;

class StreamWritinatorTest : public StreamWritinator {
private:
    std::vector<uint8_t> mBuffer;
    size_t mPosition;

public:
    StreamWritinatorTest() : mPosition(0) {}

    bool isStreamGood() const override {
        return mPosition < mBuffer.size() || mBuffer.empty() && mPosition == 0;
    }

    uint64_t getStreamPosition() override {
        return mPosition;
    }

    void setStreamPosition(uint64_t position) override {
        mPosition = position;
    }

    bool writeData(const char* data, uint64_t size) override {
        if (mPosition + size > mBuffer.size()) {
            mBuffer.resize(mPosition + size);
        }

        std::copy(data, data + size, mBuffer.begin() + mPosition);

        mPosition += size;
        return true;
    }

    void clear() {
        mBuffer.clear();
        mPosition = 0;
    }

    std::string getString() {
        return std::string(mBuffer.begin(), mBuffer.end());
    }

    std::vector<uint8_t> getBuffer() {
        return mBuffer;
    }
};

class TestSerializable {
private:
    int mInt;
    double mDouble;
    std::string mString;

public:
    TestSerializable(int i, double d, const std::string& s) : mInt(i), mDouble(d), mString(s) {}

    static void serialize(StreamWritinator* stream, const TestSerializable& obj) {
        stream->writeRaw(obj.mInt);
        stream->writeRaw(obj.mDouble);
        stream->writeString(obj.mString);
    }

    size_t size() const {
        return sizeof(int) + sizeof(double) + sizeof(unsigned long) + mString.size();
    }
};

TEST_CASE("StreamWritinator writeBuffer", "[StreamWritinator]") {
    GIVEN("A StreamWritinator object") {
        StreamWritinatorTest stream;

        WHEN("Checking if the Stream is good") {
            THEN("The stream is good") {
                REQUIRE(stream.isStreamGood());
            }

            AND_THEN("The Stream is good"){
                REQUIRE(static_cast<bool>(stream) == stream.isStreamGood());
            }
        }

        WHEN("Getting the stream position") {
            THEN("The stream position is 0") {
                REQUIRE(stream.getStreamPosition() == 0);
            }
        }

        WHEN("Setting the stream position") {
            stream.setStreamPosition(10);

            THEN("The stream position is 10") {
                REQUIRE(stream.getStreamPosition() == 10);
            }
        }


        WHEN("writeBuffer is called with a buffer and disabling writeSize") {
            std::vector<char> buffer = {'a', 'b', 'c', 'd'};
            stream.writeBuffer(buffer, false);

            THEN("the buffer is written to the stream") {
                REQUIRE(stream.getString() == "abcd");
            }
        }
            
        WHEN("writeBuffer is called with a buffer and enabling writeSize") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::vector<char> buffer = {'a', 'b', 'c', 'd'};
            stream.writeBuffer(buffer, true);

            THEN("the buffer is written to the stream") {
                auto expectedSize = sizeof(unsigned long) + buffer.size();
                std::vector<uint8_t> expected = { 4, 0, 0, 0, 0, 0, 0, 0, 'a', 'b', 'c', 'd' };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            };
        }

        WHEN("writeBuffer is called with an empty buffer and enabling writeSize") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::vector<char> buffer;
            stream.writeBuffer(buffer, true);

            THEN("the buffer is written to the stream") {
                auto expectedSize = sizeof(unsigned long);
                std::vector<uint8_t> expected = { 0, 0, 0, 0, 0, 0, 0, 0};

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            };
        }

        WHEN("writeBuffer is called with a buffer and enabling writeSize") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::vector<char> buffer = {'a', 'b', 'c', 'd'};
            stream.writeBuffer(buffer, true);

            THEN("the buffer is written to the stream") {
                auto expectedSize = sizeof(unsigned long) + buffer.size();
                std::vector<uint8_t> expected = { 4, 0, 0, 0, 0, 0, 0, 0, 'a', 'b', 'c', 'd' };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            };
        }

        WHEN("writeZero is called with a size of 4") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            stream.writeZero(4);

            THEN("4 zeros are written to the stream") {
                std::vector<uint8_t> expected = { 0, 0, 0, 0 };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == 4);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeString is called with a string") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::string str = "abcd";
            stream.writeString(str);

            THEN("the string is written to the stream") {
                auto expectedSize = sizeof(unsigned long) + str.size();
                std::vector<uint8_t> expected = { 4, 0, 0, 0, 0, 0, 0, 0, 'a', 'b', 'c', 'd' };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeString is called with a string_view") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::string_view str = "abcd";
            stream.writeString(str);

            THEN("the string is written to the stream") {
                auto expectedSize = sizeof(unsigned long) + str.size();
                std::vector<uint8_t> expected = { 4, 0, 0, 0, 0, 0, 0, 0, 'a', 'b', 'c', 'd' };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeRaw is called with an integer") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            int data = 1234;
            stream.writeRaw(data);

            THEN("the integer is written to the stream") {
                auto expectedSize = sizeof(int);
                std::vector<uint8_t> expected = { 0xd2, 0x04, 0x00, 0x00 };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeRaw is called with a double") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            double data = 1234.5678;
            stream.writeRaw(data);

            THEN("the double is written to the stream") {
                auto expectedSize = sizeof(double);
                std::vector<uint8_t> expected = {
                    0xad, 0xfa, 0x5c, 0x6d, 0x45, 0x4a, 0x93, 0x40
                };

                auto result = stream.getBuffer();

                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeMap is called with writeSize enabled"){
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::map<std::string, int> data = {{"a", 1}, {"b", 2}, {"c", 3}};
            stream.writeMap(data, true);

            THEN("the map is written to the stream") {
                auto expectedSize = sizeof(uint32_t) + 3 * (sizeof(unsigned long) + sizeof(char) + sizeof(int));
                std::vector<uint8_t> expected = {
                    3, 0, 0, 0, 
                    1, 0, 0, 0, 0, 0, 0, 0, 'a', 1, 0, 0, 0, 
                    1, 0, 0, 0, 0, 0, 0, 0, 'b', 2, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'c', 3, 0, 0, 0
                };
                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeMap is called with writeSize disabled"){
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::map<std::string, int> data = {{"a", 1}, {"b", 2}, {"c", 3}};
            stream.writeMap(data, false);

            THEN("the map is written to the stream") {
                auto expectedSize = 3 * (sizeof(unsigned long) + sizeof(char) + sizeof(int));
                std::vector<uint8_t> expected = { 
                    1, 0, 0, 0, 0, 0, 0, 0, 'a', 1, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'b', 2, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'c', 3, 0, 0, 0
                };

                auto result = stream.getBuffer();
                REQUIRE(result == expected);
                REQUIRE(result.size() == expectedSize);
            }
        }

        WHEN("writeMap is called with a Map with trivial key"){
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::map<int, int> data = {{1, 1}, {2, 3}, {5, 8}};
            stream.writeMap(data, true);

            THEN("the map is written to the stream") {
                auto expectedSize = sizeof(uint32_t) + 3 * (sizeof(int) + sizeof(int));
                std::vector<uint8_t> expected = {
                    3, 0, 0, 0, 
                    1, 0, 0, 0, 1, 0, 0, 0, 
                    2, 0, 0, 0, 3, 0, 0, 0, 
                    5, 0, 0, 0, 8, 0, 0, 0
                };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeMap is called with a unordered Map with writeSize enabled") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::unordered_map<std::string, int> data = {{"a", 1}, {"b", 2}, {"c", 3}};
            stream.writeMap(data, true);

            THEN("the unordered map is written to the stream") {
                auto expectedSize = sizeof(uint32_t) + 3 * (sizeof(unsigned long) + sizeof(char) + sizeof(int));
                std::vector<uint8_t> expected = {
                    3, 0, 0, 0, 
                    1, 0, 0, 0, 0, 0, 0, 0, 'c', 3, 0, 0, 0, 
                    1, 0, 0, 0, 0, 0, 0, 0, 'b', 2, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'a', 1, 0, 0, 0
                };
                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeMap is called with a unordered Map with writeSize disabled") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::unordered_map<std::string, int> data = {{"a", 1}, {"b", 2}, {"c", 3}};
            stream.writeMap(data, false);

            THEN("the unordered map is written to the stream") {
                auto expectedSize = 3 * (sizeof(unsigned long) + sizeof(char) + sizeof(int));
                std::vector<uint8_t> expected = { 
                    1, 0, 0, 0, 0, 0, 0, 0, 'c', 3, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'b', 2, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'a', 1, 0, 0, 0
                };
                auto result = stream.getBuffer();
                REQUIRE(result == expected);
                REQUIRE(result.size() == expectedSize);
            }
        }

        WHEN("writeMap is called with an unordered Map with a trivial key") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::unordered_map<int, int> data = {{1, 1}, {2, 3}, {5, 8}};
            stream.writeMap(data, true);

            THEN("the unordered map is written to the stream") {
                auto expectedSize = sizeof(uint32_t) + 3 * (sizeof(int) + sizeof(int));
                std::vector<uint8_t> expected = {
                    3, 0, 0, 0, 
                    5, 0, 0, 0, 8, 0, 0, 0,
                    2, 0, 0, 0, 3, 0, 0, 0, 
                    1, 0, 0, 0, 1, 0, 0, 0 
                };
                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeMap is called with a map with a string key") {
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::map<std::string, int> data = {{"a", 1}, {"b", 2}, {"c", 3}};
            stream.writeMap(data, true);

            THEN("the map is written to the stream") {
                auto expectedSize = sizeof(uint32_t) + 3 * (sizeof(unsigned long) + sizeof(char) + sizeof(int));
                std::vector<uint8_t> expected = {
                    3, 0, 0, 0, 
                    1, 0, 0, 0, 0, 0, 0, 0, 'a', 1, 0, 0, 0, 
                    1, 0, 0, 0, 0, 0, 0, 0, 'b', 2, 0, 0, 0,
                    1, 0, 0, 0, 0, 0, 0, 0, 'c', 3, 0, 0, 0
                };
                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeVector is called with writeSize enabled"){
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            std::vector<int> data = {1, 2, 3};
            stream.writeVector(data, true);

            THEN("the array is written to the stream") {
                auto expectedSize = sizeof(uint32_t) + 3 * sizeof(int);
                std::vector<uint8_t> expected = {
                    3, 0, 0, 0,
                    1, 0, 0, 0, 
                    2, 0, 0, 0, 
                    3, 0, 0, 0 
                };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }

        WHEN("writeSerializable is called"){
            stream.clear();
            REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

            TestSerializable data(1, 2.0, "abc");
            stream.writeSerializable(data);

            THEN("the serializable object is written to the stream") {
                auto expectedSize = data.size();
                std::vector<uint8_t> expected = {
                    1, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0x40, 
                    3, 0, 0, 0, 0, 0, 0, 0, 'a', 'b', 'c'
                };

                auto result = stream.getBuffer();
                REQUIRE(result.size() == expectedSize);
                REQUIRE(result == expected);
            }
        }
    }
}

