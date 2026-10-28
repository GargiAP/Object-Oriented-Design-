#pragma once
#include <bits/stdc++.h>
using namespace std;

enum class PaymentStatus {
    Pending, Paid, Failed, Refunded
};

class Payment {
public:
    virtual bool processPayment(double amount) = 0;
    virtual string getPaymentMethod() const = 0;

    virtual ~Payment() = default;
};

class CardPayment : public Payment {
public:
    bool processPayment(double amount) override {
        if (amount <= 0) return false;
        cout << "Processing card payment...\n";
        return true; // Simulated success
    }

    string getPaymentMethod() const override {
        return "Card";
    }
};

class UPIPayment : public Payment {
public:
    bool processPayment(double amount) override {
        if (amount <= 0) return false;
        cout << "Processing UPI payment...\n";
        return true; // Simulated success
    }

    string getPaymentMethod() const override {
        return "UPI";
    }
};

class CashOnDelivery : public Payment {
public:
    bool processPayment(double amount) override {
        if (amount <= 0) return false;
        cout << "Cash on Delivery selected.\n";
        return true; // Accepts the COD order; cash is collected later
    }

    string getPaymentMethod() const override {
        return "Cash on Delivery";
    }
};