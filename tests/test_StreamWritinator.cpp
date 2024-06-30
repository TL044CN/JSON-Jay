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
    std::map<std::string, int> mMap;

public:
    TestSerializable(int i, double d, const std::string& s, const std::map<std::string, int>& m = {})
    : mInt(i), mDouble(d), mString(s), mMap(m) {}

    static void serialize(StreamWritinator* stream, const TestSerializable& obj) {
        stream->writeRaw(obj.mInt);
        stream->writeRaw(obj.mDouble);
        stream->writeString(obj.mString);
        stream->writeMap(obj.mMap);
    }

    size_t size() const {
        size_t size = sizeof(int) + sizeof(double) + sizeof(unsigned long) + mString.size();
        for (auto& [key, value] : mMap) {
            size += sizeof(unsigned long) + key.size() + sizeof(int);
        }
        return size;
    }
};

TEST_CASE("StreamWritinator", "[StreamWritinator]") {
    GIVEN("A StreamWritinator object") {
        StreamWritinatorTest stream;

        WHEN("Checking if the Stream is good") {
            THEN("The stream is good calling isStreamGood()") {
                REQUIRE(stream.isStreamGood());
            }

            AND_THEN("isStreamGood() is the same as the bool conversion operator"){
                REQUIRE(static_cast<bool>(stream) == stream.isStreamGood());
            }
        }

        WHEN("Getting the stream position") {
            THEN("The stream position is 0") {
                REQUIRE(stream.getStreamPosition() == 0);
            }
        }

        WHEN("Setting the stream position to 10") {
            stream.setStreamPosition(10);

            THEN("The stream position is 10") {
                REQUIRE(stream.getStreamPosition() == 10);
            }

            AND_THEN("The stream is no longer good") {
                REQUIRE_FALSE(stream.isStreamGood());
            }
        }

        AND_GIVEN("A Buffer"){
            std::vector<char> buffer = {'a', 'b', 'c', 'd'};

            WHEN("writeBuffer is called with the buffer and disabling writeSize") {
                stream.writeBuffer(buffer, false);

                THEN("the buffer is written to the stream") {
                    REQUIRE(stream.getString() == "abcd");
                }
            }

            WHEN("writeBuffer is called with the buffer and enabling writeSize") {
                stream.clear();
                REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

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

        AND_GIVEN("A String") {
            std::string str = "abcd";

            WHEN("writeString is called with the string") {
                stream.clear();
                REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

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

            WHEN("writeString is called with the string and writeSize disabled") {
                stream.clear();
                REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

                stream.writeString(str, false);

                THEN("the string is written to the stream") {
                    std::vector<uint8_t> expected = { 'a', 'b', 'c', 'd' };

                    auto result = stream.getBuffer();
                    REQUIRE(result.size() == str.size());
                    REQUIRE(result == expected);
                }
            }

            WHEN("writeString is called with a string_view and writeSize disabled") {
                stream.clear();
                REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

                std::string_view str = "abcd";
                stream.writeString(str, false);

                THEN("the string is written to the stream") {
                    std::vector<uint8_t> expected = { 'a', 'b', 'c', 'd' };

                    auto result = stream.getBuffer();
                    REQUIRE(result.size() == str.size());
                    REQUIRE(result == expected);
                }
            }
        }

        AND_GIVEN("A Raw Data") {
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

            WHEN("writeRaw is called with a 64 bit integer"){
                stream.clear();
                REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

                int64_t data = 1234567890123456789;
                stream.writeRaw(data);

                THEN("the 64 bit integer is written to the stream") {
                    auto expectedSize = sizeof(int64_t);
                    std::vector<uint8_t> expected = {
                        0x15, 0x81, 0xE9, 0x7D, 0xF4, 0x10, 0x22, 0x11
                    };

                    auto result = stream.getBuffer();

                    REQUIRE(result.size() == expectedSize);
                    REQUIRE(result == expected);
                }
            }
        }

#if 0   // TODO: Fix Serialization
        AND_GIVEN("A Serializable") {
            auto data = std::make_unique<TestSerializable>(1, 2.0, "abc");

            WHEN("writeSerializable is called with the Serializable") {
                stream.clear();
                REQUIRE((stream.getStreamPosition() == 0 && stream.isStreamGood()));

                stream.writeSerializable(*data);

                THEN("the Serializable is written to the stream") {
                    auto expectedSize = data->size();
                    const auto& result = stream.getBuffer();
                    REQUIRE(result.size() == expectedSize);

                    for(auto& byte : result){
                        std::cout << (int)byte << " ";
                    }
                }
            }

        }
#endif
    }        
}
