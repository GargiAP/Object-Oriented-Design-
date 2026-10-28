#pragma once
#include <bits/stdc++.h>
#include "FoodOrdering.h"
#include "order.h"
#include "payment.h"
using namespace std;

struct CartItem
{
    int id;
    string name;
    int quantity;
    double price;
};

class Cart
{
private:
    vector<CartItem> i;

public:
    Cart() = default;
    Cart(vector<CartItem> ci) : i(ci)
    {
    }

    bool addItemToCart(Restaurant &p, int rid, int cid, string name, int quant)
    {
        if (rid <= 0 || cid <= 0 || quant <= 0 || empty(name))
            return false;
        for (auto &j : i)
        {
            if (j.id == cid)
            {
                return false;
            }
        }
        double n = p.getfoodPrice(rid, name, quant);
        if (n <= 0)
            return false;

        i.push_back({cid, name, quant, n});
        return true;
    }

    bool updateCartItem(int cid, int newQuant)
    {
        if (cid <= 0 || newQuant <= 0)
            return false;

        for (auto &j : i)
        {
            if (j.id == cid)
            {
                j.quantity = newQuant;
                return true;
            }
        }
        return false;
    }

    int subTotal()
    {
        int tot = 0;
        for (auto &j : i)
        {
            tot += j.price * j.quantity;
        }
        return tot;
    }

    void clearCart()
    {
        i.clear();
    }

    const vector<CartItem> &getItems() const
    {
        return i;
    }
};

class Customer
{
private:
    int id;
    Cart &c;
    string deliveryAddress;
    vector<Order> orders;

    inline static int nextOrderId = 1;

public:
    Customer(int i, Cart &ci, string address)
        : id(i), c(ci), deliveryAddress(address)
    {
    }

    bool addItems(Restaurant &p, int id, int cid,
                  string name, int quant)
    {
        if (quant <= 0 || id <= 0 || cid <= 0 || empty(name))
            return false;

        return c.addItemToCart(p, id, cid, name, quant);
    }

    bool updateItem(Restaurant &p, int cid, int newQuant)
    {
        if (cid <= 0 || newQuant <= 0)
            return false;

        return c.updateCartItem(cid, newQuant);
    }

    bool checkout(int restaurantId)
    {
        if (id <= 0 || restaurantId <= 0 ||
            deliveryAddress.empty() || c.getItems().empty())
            return false;

        vector<OrderedItem> orderedItems;
        double subtotal = 0;

        for (const auto &item : c.getItems())
        {
            if (item.quantity <= 0 || item.price <= 0)
                return false;

            orderedItems.push_back({item.id,
                                    item.name,
                                    item.quantity,
                                    item.price});

            subtotal += item.price * item.quantity;
        }

        double taxes = subtotal * 0.05;
        double deliveryFee = 40.0;
        double finalTotal = subtotal + taxes + deliveryFee;

        Order newOrder(
            nextOrderId,
            id,
            restaurantId,
            orderedItems,
            deliveryAddress,
            subtotal,
            taxes,
            deliveryFee,
            finalTotal);

        orders.push_back(newOrder);
        ++nextOrderId;

        c.clearCart();
        return true;
    }

    const vector<Order> &getOrders() const
    {
        return orders;
    }

    bool payForOrder(int orderId, Payment &payment)
    {
        for (auto &order : orders)
        {
            if (order.getOrderId() != orderId)
                continue;

            // Prevent processing payment twice.
            if (order.getPaymentStatus() != PaymentStatus::Pending)
                return false;

            string method = payment.getPaymentMethod();

            // COD is payable upon delivery, not now.
            if (method == "Cash on Delivery")
            {
                order.setPaymentPending(method);
                return true;
            }

            bool success = payment.processPayment(order.getFinalTotal());
            order.setPaymentResult(success, method);

            if (!success)
            {
                cout << "Payment failed. Order is not paid.\n";
                return false;
            }

            cout << "Payment successful!\n";
            return true;
        }

        return false; // Order ID not found
    }
    bool cancelMyOrder(int orderId)
    {
        for (auto &order : orders)
        {
            if (order.getOrderId() == orderId)
                return order.cancelOrder();
        }
        return false;
    }

    bool viewOrderStatus(int orderId) const
    {
        for (const auto &order : orders)
        {
            if (order.getOrderId() == orderId)
            {
                cout << "Order #" << orderId << " status: "
                     << static_cast<int>(order.getOrderStatus()) << '\n';
                return true;
            }
        }
        return false;
    }
};