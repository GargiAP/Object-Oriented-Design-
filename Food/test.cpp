#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "FoodOrdering.h"
#include "Customer.h"

// Restaurant management

TEST_CASE("Add restaurant successfully") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
}

TEST_CASE("Reject invalid restaurant details") {
    Restaurant r;
    REQUIRE_FALSE(r.addRestaurant(0, "Gargi", "Pune"));
    REQUIRE_FALSE(r.addRestaurant(-1, "Gargi", "Pune"));
    REQUIRE_FALSE(r.addRestaurant(102, "", "Pune"));
    REQUIRE_FALSE(r.addRestaurant(103, "Gargi", ""));
}

TEST_CASE("Reject duplicate restaurant ID") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE_FALSE(r.addRestaurant(101, "Another", "Mumbai"));
}

TEST_CASE("Cannot add a menu item to an unknown restaurant") {
    Restaurant r;
    REQUIRE_FALSE(r.addMenu(999, 1, "Dosa", Category::MainCourse,
                            80, AvailabilityStatus::Yes, Type::Veg));
}

// Menu management

TEST_CASE("Add menu items successfully") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));

    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE(r.addMenu(101, 2, "Idli", Category::Starter,
                      50, AvailabilityStatus::Yes, Type::Veg));
}

TEST_CASE("Reject duplicate item IDs within a restaurant") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));

    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE_FALSE(r.addMenu(101, 1, "Idli", Category::Starter,
                            50, AvailabilityStatus::Yes, Type::Veg));
}

TEST_CASE("Allow same item ID in different restaurants") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Restaurant A", "Pune"));
    REQUIRE(r.addRestaurant(102, "Restaurant B", "Mumbai"));

    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE(r.addMenu(102, 1, "Idli", Category::Starter,
                      50, AvailabilityStatus::Yes, Type::Veg));
}

TEST_CASE("Reject invalid menu item details") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));

    REQUIRE_FALSE(r.addMenu(101, 0, "Dosa", Category::MainCourse,
                            80, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE_FALSE(r.addMenu(101, 2, "", Category::Starter,
                            50, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE_FALSE(r.addMenu(101, 3, "Idli", Category::Starter,
                            0, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE_FALSE(r.addMenu(101, 4, "Pasta", Category::MainCourse,
                            -10, AvailabilityStatus::Yes, Type::NonVeg));
}

TEST_CASE("Remove an existing menu item") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    REQUIRE(r.removeMenu(101, 1));
    REQUIRE_FALSE(r.removeMenu(101, 1));
}

TEST_CASE("Reject removal for unknown restaurant or item") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));

    REQUIRE_FALSE(r.removeMenu(999, 1));
    REQUIRE_FALSE(r.removeMenu(101, 999));
}

// Price updates

TEST_CASE("Update menu item price") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    REQUIRE(r.updatePrice(101, 1, 100));
}

TEST_CASE("Reject invalid price updates") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    REQUIRE_FALSE(r.updatePrice(101, 1, 0));
    REQUIRE_FALSE(r.updatePrice(101, 1, -10));
    REQUIRE_FALSE(r.updatePrice(101, 999, 100));
    REQUIRE_FALSE(r.updatePrice(999, 1, 100));
}

// Availability updates

TEST_CASE("Update menu item availability") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    REQUIRE(r.updatestatus(101, 1, AvailabilityStatus::No));
    REQUIRE(r.updatestatus(101, 1, AvailabilityStatus::Yes));
}

TEST_CASE("Reject availability updates for unknown IDs") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    REQUIRE_FALSE(r.updatestatus(101, 999, AvailabilityStatus::No));
    REQUIRE_FALSE(r.updatestatus(999, 1, AvailabilityStatus::No));
}

// Display

TEST_CASE("Display menu for an existing restaurant") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    REQUIRE(r.displayMenu(101));
}

TEST_CASE("Cannot display menu for an unknown restaurant") {
    Restaurant r;
    REQUIRE_FALSE(r.displayMenu(999));
}


TEST_CASE("Add an available item to cart") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE(c.addItemToCart(r, 101, 1, "Dosa", 2));
    REQUIRE(c.subTotal() == 160);
}

TEST_CASE("Reject unavailable item") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::No, Type::Veg));

    Cart c;
    REQUIRE_FALSE(c.addItemToCart(r, 101, 1, "Dosa", 1));
    REQUIRE(c.subTotal() == 0);
}

TEST_CASE("Reject invalid quantity when adding an item") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE_FALSE(c.addItemToCart(r, 101, 1, "Dosa", 0));
    REQUIRE_FALSE(c.addItemToCart(r, 101, 1, "Dosa", -2));
    REQUIRE(c.subTotal() == 0);
}

TEST_CASE("Reject unknown restaurant or food item") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE_FALSE(c.addItemToCart(r, 999, 1, "Dosa", 1));
    REQUIRE_FALSE(c.addItemToCart(r, 101, 999, "Unknown Food", 1));
}

TEST_CASE("Update quantity of an item in cart") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE(c.addItemToCart(r, 101, 1, "Dosa", 2));
    REQUIRE(c.updateCartItem(1, 4));
    REQUIRE(c.subTotal() == 320);
}

TEST_CASE("Reject invalid quantity update") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE(c.addItemToCart(r, 101, 1, "Dosa", 2));

    REQUIRE_FALSE(c.updateCartItem(1, 0));
    REQUIRE_FALSE(c.updateCartItem(1, -1));
    REQUIRE(c.subTotal() == 160);
}

TEST_CASE("Reject update for an item not in cart") {
    Cart c;
    REQUIRE_FALSE(c.updateCartItem(999, 3));
}

TEST_CASE("Calculate subtotal for multiple items") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));
    REQUIRE(r.addMenu(101, 2, "Idli", Category::Starter,
                      50, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE(c.addItemToCart(r, 101, 1, "Dosa", 2));
    REQUIRE(c.addItemToCart(r, 101, 2, "Idli", 3));

    // (2 * 80) + (3 * 50) = 310
    REQUIRE(c.subTotal() == 310);
}

TEST_CASE("Clear cart after successful order") {
    Restaurant r;
    REQUIRE(r.addRestaurant(101, "Gargi", "Pune"));
    REQUIRE(r.addMenu(101, 1, "Dosa", Category::MainCourse,
                      80, AvailabilityStatus::Yes, Type::Veg));

    Cart c;
    REQUIRE(c.addItemToCart(r, 101, 1, "Dosa", 2));
    REQUIRE(c.subTotal() == 160);

    c.clearCart();

    REQUIRE(c.subTotal() == 0);
}

TEST_CASE("Clearing an empty cart is safe") {
    Cart c;
    c.clearCart();
    REQUIRE(c.subTotal() == 0);
}
