#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "VendingMachine.h"

TEST_CASE("Exact payment purchases successfully") {
    VendingMachine vm;
    Customer customer(100);
    int id = 101;

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 30));
    REQUIRE(customer.purchase(vm, 101, 1));
    REQUIRE(customer.confirmSelect(vm));

    REQUIRE(vm.getCredit() == 0);
    REQUIRE(vm.getQuantity(id) == 4);
    REQUIRE(customer.getBalance() == 70);
}

TEST_CASE("Overpayment returns correct change") {
    VendingMachine vm;
    Customer customer(100);
    int id = 101;

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 50));
    REQUIRE(customer.purchase(vm, 101, 1));
    REQUIRE(customer.confirmSelect(vm));

    REQUIRE(vm.getCredit() == 0);
    REQUIRE(vm.getQuantity(id) == 4);
    REQUIRE(customer.getBalance() == 70);
}

TEST_CASE("Insufficient funds preserve credit and selection") {
    VendingMachine vm;
    Customer customer(100);
    int id = 101;

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 20));
    REQUIRE(customer.purchase(vm, 101, 1));

    REQUIRE_FALSE(customer.confirmSelect(vm));
    REQUIRE(vm.getCredit() == 20);
    REQUIRE(vm.getQuantity(id) == 5);

    REQUIRE(customer.insertInto(vm, 10));
    REQUIRE(customer.confirmSelect(vm));

    REQUIRE(vm.getCredit() == 0);
    REQUIRE(vm.getQuantity(id) == 4);
    REQUIRE(customer.getBalance() == 70);
}

TEST_CASE("Multiple units calculate total and change correctly") {
    VendingMachine vm;
    Customer customer(150);
    int id = 101;

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 100));
    REQUIRE(customer.purchase(vm, 101, 2));
    REQUIRE(customer.confirmSelect(vm));

    REQUIRE(vm.getCredit() == 0);
    REQUIRE(vm.getQuantity(id) == 3);
    REQUIRE(customer.getBalance() == 90); // 150 - 100 + 40
}

TEST_CASE("Cannot select more units than available stock") {
    VendingMachine vm;
    Customer customer(100);

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 1));

    REQUIRE_FALSE(customer.purchase(vm, 101, 2));
    REQUIRE(vm.getQuantity(101) == 1);
}

TEST_CASE("Cancellation refunds all inserted money") {
    VendingMachine vm;
    Customer customer(100);
    int id = 101;

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 50));
    REQUIRE(customer.purchase(vm, 101, 1));

    customer.cancelAt(vm);

    REQUIRE(vm.getCredit() == 0);
    REQUIRE(vm.getQuantity(id) == 5);
    REQUIRE(customer.getBalance() == 100);
    REQUIRE_FALSE(customer.confirmSelect(vm));
}

TEST_CASE("Successful purchase cannot be confirmed twice") {
    VendingMachine vm;
    Customer customer(100);
    int id = 101;

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 50));
    REQUIRE(customer.purchase(vm, 101, 1));

    REQUIRE(customer.confirmSelect(vm));
    REQUIRE_FALSE(customer.confirmSelect(vm));

    REQUIRE(vm.getCredit() == 0);
    REQUIRE(vm.getQuantity(id) == 4);
}

TEST_CASE("Cannot confirm without a pending selection") {
    VendingMachine vm;
    Customer customer(100);

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));
    REQUIRE(customer.insertInto(vm, 50));

    REQUIRE_FALSE(customer.confirmSelect(vm));
    REQUIRE(vm.getCredit() == 50);
    REQUIRE(vm.getQuantity(101) == 5);
}

TEST_CASE("Invalid quantity is rejected") {
    VendingMachine vm;
    Customer customer(100);

    REQUIRE(vm.addProduct(101, "Chocolate", 30, 5));

    REQUIRE_FALSE(customer.purchase(vm, 101, 0));
    REQUIRE_FALSE(customer.purchase(vm, 101, -1));
    REQUIRE(vm.getQuantity(101) == 5);
}