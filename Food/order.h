
#pragma once
#include <bits/stdc++.h>
using namespace std;

enum class PaymentStatus
{
    Pending,
    Paid,
    Failed,
    Refunded
};

enum class OrderStatus
{
    Placed,
    Confirmed,
    Preparing,
    OutForDelivery,
    Delivered,
    Cancelled
};

struct OrderedItem
{
    int id;
    string name;
    int quantity;
    double priceAtCheckout;
};

class Order
{
private:
    int orderId;
    int customerId;
    int restaurantId;
    vector<OrderedItem> items;
    PaymentStatus paymentStatus = PaymentStatus::Pending;
    string paymentMethod = "Not selected";
    string deliveryAddress;

    double subtotal;
    double taxes;
    double deliveryFee;
    double finalTotal;

    PaymentStatus paymentStatus;
    OrderStatus orderStatus;

public:
    Order(int oid, int cid, int rid,
          const vector<OrderedItem> &orderedItems,
          const string &address,
          double sub, double tax,
          double fee, double total)
        : orderId(oid),
          customerId(cid),
          restaurantId(rid),
          items(orderedItems),
          deliveryAddress(address),
          subtotal(sub),
          taxes(tax),
          deliveryFee(fee),
          finalTotal(total),
          paymentStatus(PaymentStatus::Pending),
          orderStatus(OrderStatus::Placed)
    {
    }

    int getOrderId() const
    {
        return orderId;
    }

    double getSubtotal() const
    {
        return subtotal;
    }

    double getTaxes() const
    {
        return taxes;
    }

    double getDeliveryFee() const
    {
        return deliveryFee;
    }

    double getFinalTotal() const
    {
        return finalTotal;
    }

    OrderStatus getStatus() const
    {
        return orderStatus;
    }

    void displayOrder() const
    {
        cout << "Order ID: " << orderId << '\n';
        cout << "Customer ID: " << customerId << '\n';
        cout << "Restaurant ID: " << restaurantId << '\n';
        cout << "Delivery address: " << deliveryAddress << '\n';

        cout << "\nOrdered items:\n";
        for (const auto &item : items)
        {
            cout << item.name
                 << " | Quantity: " << item.quantity
                 << " | Unit price at checkout: "
                 << item.priceAtCheckout
                 << " | Line total: "
                 << item.priceAtCheckout * item.quantity
                 << '\n';
        }

        cout << "Subtotal: " << subtotal << '\n';
        cout << "Taxes: " << taxes << '\n';
        cout << "Delivery fee: " << deliveryFee << '\n';
        cout << "Final total: " << finalTotal << '\n';
    }

    void setPaymentResult(bool success, const string &method)
    {
        paymentMethod = method;
        paymentStatus = success ? PaymentStatus::Paid
                                : PaymentStatus::Failed;
    }

    void setPaymentPending(const string &method)
    {
        paymentMethod = method;
        paymentStatus = PaymentStatus::Pending;
    }

    PaymentStatus getPaymentStatus() const
    {
        return paymentStatus;
    }

    string getPaymentMethod() const
    {
        return paymentMethod;
    }

    bool confirmOrder()
    {
        if (orderStatus != OrderStatus::Placed)
            return false;

        orderStatus = OrderStatus::Confirmed;
        return true;
    }

    bool startPreparing()
    {
        if (orderStatus != OrderStatus::Confirmed)
            return false;

        orderStatus = OrderStatus::Preparing;
        return true;
    }

    bool markOutForDelivery()
    {
        if (orderStatus != OrderStatus::Preparing)
            return false;

        orderStatus = OrderStatus::OutForDelivery;
        return true;
    }

    bool markDelivered()
    {
        if (orderStatus != OrderStatus::OutForDelivery)
            return false;

        orderStatus = OrderStatus::Delivered;
        return true;
    }

    OrderStatus getOrderStatus() const
    {
        return orderStatus;
    }

    bool cancelOrder()
    {
        // Cancellation is allowed only before preparation starts.
        if (orderStatus != OrderStatus::Placed &&
            orderStatus != OrderStatus::Confirmed)
        {
            return false;
        }

        orderStatus = OrderStatus::Cancelled;

        if (paymentStatus == PaymentStatus::Paid)
        {
            paymentStatus = PaymentStatus::Refunded;
            cout << "Refund initiated for order #" << orderId << '\n';
        }

        // Pending or failed payments are not refunded.
        // COD remains unpaid, so no cash refund is issued.
        return true;
    }

    int getRestaurantId() const
    {
        return restaurantId;
    }
};
