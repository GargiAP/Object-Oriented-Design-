#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "stock.h"

TEST_CASE("adding a product to inventory"){
    Inventory i;
    

    REQUIRE(i.addProduct(101,"Phone", 10000, 45));
}

TEST_CASE("restock"){
    Inventory i;
    REQUIRE(i.addProduct(101,"Phone", 10000, 45));

    REQUIRE(i.restock(101, 45));
    REQUIRE_FALSE(i.restock(102, 45));
}

TEST_CASE("sell"){
    Inventory i;
    REQUIRE(i.addProduct(101,"Phone", 10000, 45));
    REQUIRE(i.getquantity(101) == 45);
    REQUIRE(i.sell(101, 5));
    REQUIRE(i.getquantity(101) == 40);
    REQUIRE_FALSE(i.sell(102, 6));
    REQUIRE_FALSE(i.sell(101, 0));
    REQUIRE_FALSE(i.sell(101, -5));
    REQUIRE(i.getquantity(101) == 40);
    REQUIRE_FALSE(i.sell(101, 50));


}

TEST_CASE("Reject duplicate product IDs") {
    Inventory i;

    REQUIRE(i.addProduct(101, "Phone", 10000, 45));
    REQUIRE_FALSE(i.addProduct(101, "Laptop", 50000, 10));
    REQUIRE(i.getquantity(101) == 45);
}

TEST_CASE("Restock increases quantity") {
    Inventory i;
    REQUIRE(i.addProduct(101, "Phone", 10000, 45));

    REQUIRE(i.restock(101, 45));
    REQUIRE(i.getquantity(101) == 90);

    REQUIRE_FALSE(i.restock(102, 10));
    REQUIRE_FALSE(i.restock(101, 0));
    REQUIRE_FALSE(i.restock(101, -5));
    REQUIRE(i.getquantity(101) == 90);
}