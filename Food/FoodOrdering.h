#pragma once
#include <bits/stdc++.h>
using namespace std;

enum class Status
{
    Open,
    close
};

enum class Category
{
    Starter,
    MainCourse,
    Dessert,
    Beverage
};

enum class AvailabilityStatus
{
    Yes,
    No
};

enum class Type
{
    Veg,
    NonVeg
};

struct items
{
    int id;
    string foodName;
    Category c;
    double price;
    AvailabilityStatus as;
    Type t;
};

class Menu
{
private:
    vector<items> foodItems;

public:
    Menu() = default;
    Menu(vector<items> fi) : foodItems(fi)
    {
    }

    bool addItem(int id, string name, Category c, double p, AvailabilityStatus a, Type t)
    {
        if (id <= 0 || p <= 0 || empty(name))
            return false;
        for (auto &foodi : foodItems)
        {
            if (foodi.id == id)
            {
                return false;
            }
        }
        foodItems.push_back({id, name, c, p, a, t});
        return true;
    }

    bool removeFoodItem(int id)
    {
        for (auto it = foodItems.begin(); it != foodItems.end(); ++it)
        {
            if (it->id == id)
            {
                foodItems.erase(it);
                return true;
            }
        }
        return false;
    }

    bool updateItemPrice(int id, double newPrice)
    {
        for (auto &p : foodItems)
        {
            if (p.id == id)
            {
                p.price = newPrice;
                return true;
            }
        }
        return false;
    }

    bool updateAvailabilityStatus(int id, AvailabilityStatus status)
    {
        for (auto &p : foodItems)
        {
            if (p.id == id)
            {
                p.as = status;
                return true;
            }
        }
        return false;
    }

    void display()
    {
        for (auto &p : foodItems)
        {
            cout << "Name: " << p.foodName
                 << " | Category: " << static_cast<int>(p.c)
                 << " | Price: " << p.price
                 << " | Availability: " << static_cast<int>(p.as)
                 << " | Type: " << static_cast<int>(p.t)
                 << '\n';
        }
    }

    double getPrice(string fname)
    {
        for (auto &p : foodItems)
        {
            if (p.foodName == fname)
            {
                return p.price;
            }
        }
        return 0;
    }

    bool isAvailable(string fname)
    {
        for (auto &p : foodItems)
        {
            if (p.foodName == fname)
            {
                return p.as == AvailabilityStatus::Yes;
            }
        }
        return false;
    }
};

struct Restruant
{
    int id;
    string name;
    string location;
    Status s;
    Menu m;
};

class Restaurant
{
private:
    vector<Restruant> details;

public:
    Restaurant() = default;
    Restaurant(vector<Restruant> d) : details(d)
    {
    }

    bool addRestaurant(int rid, string rname, string rlocation)
    {
        if (rid <= 0 || empty(rname) || empty(rlocation))
            return false;

        for (auto &r : details)
        {
            if (r.id == rid)
            {
                return false;
            }
        }
        details.push_back({rid, rname, rlocation});
        return true;
    }

    bool addMenu(int id, int fid, string name, Category c, double p, AvailabilityStatus a, Type t)
    {
        if (id <= 0 || empty(name) || p <= 0)
            return false;
        for (auto &r : details)
        {
            if (r.id == id)
            {
                return r.m.addItem(fid, name, c, p, a, t);
            }
        }
        return false;
    }

    bool removeMenu(int rid, int id)
    {
        if (id <= 0)
            return false;
        for (auto &r : details)
        {
            if (r.id == rid)
            {
                return r.m.removeFoodItem(id);
            }
        }
        return false;
    }

    bool updatePrice(int rid, int fid, double newPrice)
    {
        if (rid <= 0 || fid <= 0 || newPrice <= 0)
            return false;
        for (auto &r : details)
        {
            if (r.id == rid)
            {
                return r.m.updateItemPrice(fid, newPrice);
            }
        }
        return false;
    }

    bool updatestatus(int rid, int fid, AvailabilityStatus s)
    {
        if (rid <= 0 || fid <= 0)
            return false;
        for (auto &r : details)
        {
            if (r.id == rid)
            {
                return r.m.updateAvailabilityStatus(fid, s);
            }
        }
        return false;
    }

    bool displayMenu(int rid)
    {
        if (rid <= 0)
            return false;
        for (auto &d : details)
        {
            if (d.id == rid)
            {
                d.m.display();
                return true;
            }
        }
        return false;
    }

    int getfoodPrice(int rid, string fname, int quantity)
    {
        for (auto &p : details)
        {
            if (p.id == rid)
            {
                if (!p.m.isAvailable(fname))
                    return 0;

                return p.m.getPrice(fname);
            }
        }
        return 0;
    }

    void viewIncomingOrders(const vector<Order> &orders, int rid)
    {
        if (rid <= 0)
            return;

        for (const auto &order : orders)
        {
            if (order.getRestaurantId() == rid &&
                order.getOrderStatus() == OrderStatus::Placed)
            {
                order.displayOrder();
            }
        }
    }

    bool acceptOrder(vector<Order> &orders, int rid, int oid)
    {
        for (auto &order : orders)
        {
            if (order.getOrderId() == oid &&
                order.getRestaurantId() == rid)
            {
                return order.confirmOrder();
            }
        }
        return false;
    }

    bool rejectOrder(vector<Order> &orders, int rid, int oid)
    {
        for (auto &order : orders)
        {
            if (order.getOrderId() == oid &&
                order.getRestaurantId() == rid)
            {
                if (order.getOrderStatus() != OrderStatus::Placed)
                    return false;

                return order.cancelOrder();
            }
        }
        return false;
    }

    bool prepareOrder(vector<Order> &orders, int rid, int oid)
    {
        for (auto &order : orders)
        {
            if (order.getOrderId() == oid &&
                order.getRestaurantId() == rid)
            {
                return order.startPreparing();
            }
        }
        return false;
    }

    bool markOrderOutForDelivery(
        vector<Order> &orders, int rid, int oid)
    {

        for (auto &order : orders)
        {
            if (order.getOrderId() == oid &&
                order.getRestaurantId() == rid)
            {
                return order.markOutForDelivery();
            }
        }
        return false;
    }
};
