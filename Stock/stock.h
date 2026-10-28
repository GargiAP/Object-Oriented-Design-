#include <bits/stdc++.h>
using namespace std;

struct Product
{
    int id;
    string name;
    int price;
    int quantity;
};

class Inventory
{
private:
    vector<Product> products;

public:
    bool addProduct(int pid, string pname, int pprice, int pquantity)
    {
        if (pid <= 0 || pprice < 0 || pquantity < 0)
            return false;

        for (const auto &p : products)
        {
            if (p.id == pid)
                return false;
        }

        products.push_back({pid, pname, pprice, pquantity});
        return true;
    }

    bool restock(int id, int quant)
    {
        if (quant <= 0)
        {
            return false;
        }
        for (auto &p : products)
        {
            if (p.id == id)
            {
                p.quantity += quant;
                return true;
            }
        }
        return false;
    }

    bool sell(int id, int quant)
    {
        if (quant <= 0)
        {
            return false;
        }

        for (auto &p : products)
        {
            if (p.id == id && p.quantity >= quant)
            {
                p.quantity -= quant;
                return true;
            }
        }
        return false;
    }

    int getquantity(int id)
    {
        for (auto &p : products)
        {
            if (p.id == id)
            {
                return p.quantity;
            }
        }
        return -1;
    }

    Product *getProduct(int id)
    {
        for (auto &p : products)
        {
            if (p.id == id)
                return &p;
        }

        return nullptr;
    }

    void displayAllProducts() const
    {
        for (const auto &p : products)
        {
            cout << "ID: " << p.id
                 << " | Name: " << p.name
                 << " | Price: " << p.price
                 << " | Quantity: " << p.quantity << '\n';
        }
    }

    void displayLowStock(int threshold) const
    {
        for (const auto &p : products)
        {
            if (p.quantity < threshold)
            {
                cout << p.name << " (ID: " << p.id
                     << "), quantity: " << p.quantity << '\n';
            }
        }
    }
};