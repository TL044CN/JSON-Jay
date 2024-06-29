#include "catch2/catch_test_macros.hpp"
#include "StreamReadinator.hpp"

class ReadinatorTest : public JSONJay::StreamReadinator {
private:
    std::vector<uint8_t> mData;
    uint64_t mPosition;

public:

    ReadinatorTest() : mPosition(0) {}

    bool isStreamGood() const override {
        return mPosition < mData.size();
    }

    uint64_t getStreamPosition() override {
        return mPosition;
    }

    void setStreamPosition(uint64_t position) override {
        mPosition = position;
    }

    bool readData(char* data, uint64_t size) override {
        if (mPosition + size > mData.size()) {
            return false;
        }

        for (uint64_t i = 0; i < size; i++) {
            if(mPosition + i >= mData.size())
                return false;
            data[i] = mData[mPosition + i];
        }

        mPosition += size;
        return true;
    }

    void setData(const std::vector<uint8_t>& data) {
        mPosition = 0;
        mData = data;
    }

};

class TestDeserializable {
public:
    int mInt;
    std::string mString;
    double mDouble;

    TestDeserializable() : mInt(0), mString(""), mDouble(0) {}
    TestDeserializable(int i, const std::string& s, double d) : mInt(i), mString(s), mDouble(d) {}

    static void deserialize(JSONJay::StreamReadinator* readinator, TestDeserializable& deserializable) {
        readinator->readRaw<int>(deserializable.mInt);
        readinator->readString(deserializable.mString);
        readinator->readRaw<double>(deserializable.mDouble);
    }

};

TEST_CASE("Readinator") {
    GIVEN("A Readinator object") {
        ReadinatorTest readinator;
        std::vector<uint8_t> data = {42, 0, 0, 0};
        readinator.setData(data);

        THEN("The bool conversion operator checks if the Readinator is ok"){
            REQUIRE(readinator);
            readinator.setStreamPosition(4);
            REQUIRE_FALSE(readinator);
        }

        THEN("The stream position can be read") {
            REQUIRE(readinator.getStreamPosition() == 0);
        }

        THEN("The stream position can be set") {
            readinator.setStreamPosition(4);
            REQUIRE(readinator.getStreamPosition() == 4);
        }

        THEN("Data can be read") {
            int i;
            readinator.readRaw<int>(i);
            REQUIRE(i == 42);
            AND_THEN("The stream position changed"){
                REQUIRE(readinator.getStreamPosition() == 4);
            }
        }

    
    }
    GIVEN("A Readinator object containing a some test data and a TestDeserializable"){
        ReadinatorTest readinator;
        std::vector<uint8_t> data = {
            42, 0, 0, 0,
            13, 0, 0, 0, 0, 0, 0, 0,
            'H', 'e', 'l', 'l', 'o', ',',' ', 'W', 'o', 'r', 'l', 'd', '!',
            0xad, 0xfa, 0x5c, 0x6d, 0x45, 0x4a, 0x93, 0x40,
            42, 0, 0, 0,
            3 , 0, 0, 0, 0, 0, 0, 0,
            'a', 'b', 'c',
            0xad, 0xfa, 0x5c, 0x6d, 0x45, 0x4a, 0x93, 0x40
        };
        readinator.setData(data);

        THEN("The data can be read") {
            int i;
            readinator.readRaw<int>(i);
            REQUIRE(i == 42);

            CHECK(readinator.getStreamPosition() == sizeof(int));

            std::string s;
            readinator.readString(s);
            REQUIRE(s == "Hello, World!");

            CHECK(readinator.getStreamPosition() == sizeof(int) + sizeof(uint64_t) + 13);

            double d;
            readinator.readRaw<double>(d);
            REQUIRE(d == 1234.5678);

            CHECK(readinator.getStreamPosition() == sizeof(int) + sizeof(uint64_t) + 13 + sizeof(double));
        }

        AND_THEN("The stream position can be read") {
            readinator.setStreamPosition(sizeof(int) + sizeof(uint64_t) + 13 + sizeof(double));
            REQUIRE(readinator.getStreamPosition() == sizeof(int) + sizeof(uint64_t) + 13 + sizeof(double));
        }

        THEN("An object can be read") {
            readinator.setStreamPosition(sizeof(int) + sizeof(uint64_t) + 13 + sizeof(double));
            TestDeserializable deserializable;
            readinator.readDeserializable(deserializable);
            REQUIRE(deserializable.mInt == 42);
            REQUIRE(deserializable.mString == "abc");
            REQUIRE(deserializable.mDouble == 1234.5678);
        }

    }

    GIVEN("A Readinator object containing some test data and a map") {
        ReadinatorTest readinator;
        std::vector<uint8_t> data = {
            2, 0, 0, 0,
            42, 0, 0, 0,
            42, 0, 0, 0,
            13, 0, 0, 0, 0, 0, 0, 0,
            'H', 'e', 'l', 'l', 'o', ',',' ', 'W', 'o', 'r', 'l', 'd', '!',
            0xad, 0xfa, 0x5c, 0x6d, 0x45, 0x4a, 0x93, 0x40,
            3, 0, 0, 0,
            42, 0, 0, 0,
            3 , 0, 0, 0, 0, 0, 0, 0,
            'a', 'b', 'c',
            0xad, 0xfa, 0x5c, 0x6d, 0x45, 0x4a, 0x93, 0x40
        };
        readinator.setData(data);

        THEN("A map can be read") {
            std::map<int, TestDeserializable> map;
            readinator.readMap<int, TestDeserializable>(map);
            REQUIRE(map.size() == 2);
            REQUIRE(map[42].mInt == 42);
            REQUIRE(map[42].mString == "Hello, World!");
            REQUIRE(map[42].mDouble == 1234.5678);
            REQUIRE(map[3].mInt == 42);
            REQUIRE(map[3].mString == "abc");
            REQUIRE(map[3].mDouble == 1234.5678);
        }
    }
}


