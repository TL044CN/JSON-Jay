#include "catch2/catch_test_macros.hpp"
#include "XMLObject.hpp"

using namespace JSONJay;

TEST_CASE("XMLObject construction", "[XMLObject]") {
    XMLObject obj;
    REQUIRE(obj.size() == 0); // Initially, the object should have no data
}

TEST_CASE("XMLObject data manipulation", "[XMLObject]") {
    XMLObject obj;

    // Add some data
    obj.push_back("tag1", "value1");
    obj[0].addAttribute("attr1", "attr1_value");

    XMLObject obj2;
    obj2.push_back("tag2", "value2");

    obj.push_back("tag3", obj2);
    obj[1].addAttribute("attr2", "attr2_value");
    obj[1].addAttribute("attr3", "attr3_value");

    // Check the data
    REQUIRE(obj.size() == 2);
    REQUIRE(obj.getTag(0).name == "tag1");
    REQUIRE(obj.getData(0).getDataType() == XMLObject::DataType::STRING);
    REQUIRE(std::get<std::string>(obj.getData(0).data) == "value1");
    REQUIRE(obj.getTag(0).attributes.size() == 1);
    REQUIRE(obj.getTag(0).attributes["attr1"] == "attr1_value");
    REQUIRE(obj[1].getDataType() == XMLObject::DataType::OBJECT);
    REQUIRE(std::get<std::unique_ptr<XMLObject>>(obj[1].data)->size() == 1);
    REQUIRE(std::get<std::unique_ptr<XMLObject>>(obj[1].data)->getTag(0).name == "tag2");
//    REQUIRE(*(std::get<std::unique_ptr<XMLObject>>(obj[1]).data)[0].getDataType() == XMLObject::DataType::STRING);
//    REQUIRE(*(std::get<std::unique_ptr<XMLObject>>(obj[1]).data)[0].data == "value2");


}