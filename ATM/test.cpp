#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ATM.h"
TEST_CASE("Verify pin"){
    ATMmachine a(10000);
    Account ac(123456, 1000, 2000);   // pin, starting balance
    REQUIRE(ac.verifyPin(123456));
}

TEST_CASE("wrong pin"){
    Account ac(123457, 1000, 2000);   // pin, starting balance
    REQUIRE_FALSE(ac.verifyPin(123456));
}

TEST_CASE("attempt locker for the pin"){
    Account ac(123456, 1000, 2000);
    REQUIRE(ac.verifyPin(123456));
    REQUIRE_FALSE(ac.verifyPin(123457));
    REQUIRE_FALSE(ac.verifyPin(123458));
    REQUIRE_FALSE(ac.verifyPin(123459));
    REQUIRE_FALSE(ac.verifyPin(123456));
}

TEST_CASE("overdraft account can go negative up to its limit") {
OverdraftAccount ac(123456, 100, 2000, 500);
ATMmachine m(10000);
REQUIRE(m.withdraw(123456, 600, ac));
REQUIRE(ac.getBalance() == -500);
}

TEST_CASE("daily limit check"){
    ATMmachine a(10000);
    Account ac(123456, 5000, 2000);
    REQUIRE(a.withdraw(123456, 1000, ac));
    REQUIRE(a.withdraw(123456, 1000, ac));
    REQUIRE_FALSE(a.withdraw(123456, 600, ac));
}

TEST_CASE("transaction history") {
    ATMmachine a(10000);
    Account ac(123456, 5000, 2000);

    TransactionHistory h;

    REQUIRE(a.withdraw(123456, 1000, ac));
    REQUIRE(a.deposit(123456, 1000, ac));

    auto history = h.getHistory();
}