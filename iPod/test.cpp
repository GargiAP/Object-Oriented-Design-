#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "iPod.h"

TEST_CASE("storing different ipods") {
    Inventory inventory;

    iPod p1(101, iPodType::Apple1, 10000, 45);
    iPod p2(102, iPodType::Apple1, 15000, 30);

    inventory.addIPod(p1);
    inventory.addIPod(p2);

    vector<iPod> see;
    see = inventory.getInfo();

}

TEST_CASE("customer purchase"){
    Inventory inventory;

REQUIRE(inventory.addIPod(iPod(101, iPodType::Apple1, 10000, 45)));
REQUIRE_FALSE(inventory.addIPod(iPod(101, iPodType::Apple2, 12000, 20)));
}