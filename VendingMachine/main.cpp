#include <iostream>
#include "VendingMachine.h"

using namespace std;

int main() {
    VendingMachine vm;
    vm.addProduct(101, "Chocolate", 30, 5);

    Customer customer(100);

    // Cash purchase
    {
        CashPayment cash = customer.cashPayment();

        customer.purchase(vm, 101, 1);
        cash.insertCash(vm, 50);

        if (cash.pay(vm))
            cout << "Cash purchase successful\n";

        cout << "Balance: " << customer.getBalance() << '\n';
        // Balance is 70: 100 - 50 + 20 change.
    }

    // Card purchase
    CardPayment card(true);

    customer.purchase(vm, 101, 1);

    if (card.pay(vm))
        cout << "Card purchase successful\n";

    cout << "Remaining stock: " << vm.getQuantity(101) << '\n';

    return 0;
}