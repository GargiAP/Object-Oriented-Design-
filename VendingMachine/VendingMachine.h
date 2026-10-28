#pragma once

#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Product {
    int id;
    string name;
    int price;
    int quantity;
};

enum class Result {
    Dispensed,
    InsufficientFunds,
    OutOfStock,
    Added,
    NotFound,
    NotEnoughOfStock,
    InvalidSelection,
    PaymentFailed
};

// Forward declaration
class Payment;

class VendingMachine {
private:
    vector<Product> products;
    int credit = 0;
    int selectedProductId = -1;
    int selectedProductQuant = 0;

    int findProduct(int id) const {
        for (int i = 0; i < static_cast<int>(products.size()); i++) {
            if (products[i].id == id)
                return i;
        }
        return -1;
    }

public:
    bool addProduct(int id, const string& name,
                    int price, int quantity) {
        if (id <= 0 || name.empty() ||
            price <= 0 || quantity < 0 ||
            findProduct(id) != -1)
            return false;

        products.push_back({id, name, price, quantity});
        return true;
    }

    void insertMoney(int amount) {
        if (amount > 0)
            credit += amount;
    }

    Result select(int id, int quantity) {
        if (quantity <= 0)
            return Result::InvalidSelection;

        int idx = findProduct(id);

        if (idx == -1)
            return Result::NotFound;

        if (products[idx].quantity == 0)
            return Result::OutOfStock;

        if (products[idx].quantity < quantity)
            return Result::NotEnoughOfStock;

        selectedProductId = id;
        selectedProductQuant = quantity;
        return Result::Added;
    }

    int selectedTotal() const {
        int idx = findProduct(selectedProductId);

        if (idx == -1 || selectedProductQuant <= 0)
            return -1;

        return products[idx].price * selectedProductQuant;
    }

    // Called only after external payment succeeds.
    bool dispense() {
        if (selectedProductId == -1 || selectedProductQuant <= 0)
            return false;

        int idx = findProduct(selectedProductId);

        if (idx == -1 ||
            products[idx].quantity < selectedProductQuant)
            return false;

        products[idx].quantity -= selectedProductQuant;
        selectedProductId = -1;
        selectedProductQuant = 0;
        return true;
    }

    // Cash purchase: checks credit, deducts cost, returns change.
    bool buy(bool confirm, int& change) {
        change = 0;

        if (!confirm)
            return false;

        int total = selectedTotal();

        if (total < 0 || credit < total)
            return false;

        int idx = findProduct(selectedProductId);

        if (idx == -1 ||
            products[idx].quantity < selectedProductQuant)
            return false;

        change = credit - total;
        credit = 0;

        return dispense();
    }

    int cancel() {
        selectedProductId = -1;
        selectedProductQuant = 0;

        int refundAmount = credit;
        credit = 0;
        return refundAmount;
    }

    int getCredit() const {
        return credit;
    }

    int getQuantity(int id) const {
        int idx = findProduct(id);
        return idx == -1 ? 0 : products[idx].quantity;
    }
};

// Abstract payment interface
class Payment {
public:
    virtual bool pay(VendingMachine& vm) = 0;
    virtual int refund(VendingMachine& vm) = 0;
    virtual ~Payment() = default;
};

// Cash payment
class CashPayment : public Payment {
private:
    int& customerBalance;

public:
    explicit CashPayment(int& balance)
        : customerBalance(balance) {}

    bool insertCash(VendingMachine& vm, int amount) {
        const vector<int> denominations = {1, 2, 5, 10, 20, 50, 100};

        if (amount > customerBalance ||
            find(denominations.begin(), denominations.end(), amount)
                == denominations.end())
            return false;

        customerBalance -= amount;
        vm.insertMoney(amount);
        return true;
    }

    bool pay(VendingMachine& vm) override {
        int change = 0;

        if (!vm.buy(true, change))
            return false;

        customerBalance += change;
        return true;
    }

    int refund(VendingMachine& vm) override {
        int amount = vm.cancel();
        customerBalance += amount;
        return amount;
    }
};

// Card payment
class CardPayment : public Payment {
private:
    bool approved;

public:
    explicit CardPayment(bool isApproved = true)
        : approved(isApproved) {}

    bool pay(VendingMachine& vm) override {
        if (!approved)
            return false;

        int total = vm.selectedTotal();

        if (total < 0)
            return false;

        // Simulated card authorization succeeded.
        // Dispense only after authorization.
        return vm.dispense();
    }

    int refund(VendingMachine& vm) override {
        // Cancel the selection. No cash is refunded for card.
        vm.cancel();
        return 0;
    }
};

// Customer handles their balance and product selection,
// not the payment mechanics.
class Customer {
private:
    int balance;

public:
    explicit Customer(int initialBalance)
        : balance(initialBalance) {}

    int getBalance() const {
        return balance;
    }

    bool purchase(VendingMachine& vm, int id, int quantity) {
        return vm.select(id, quantity) == Result::Added;
    }

    CashPayment cashPayment() {
        return CashPayment(balance);
    }
};