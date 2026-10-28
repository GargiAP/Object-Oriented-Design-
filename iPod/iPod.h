#include <bits/stdc++.h>
using namespace std;

enum class iPodType
{
    Apple1,
    Apple2,
    Apple3
};

class iPod
{
private:
    int id;
    iPodType type;
    int price;
    int quantity;

public:
    iPod(int number, iPodType itype, int iprice, int iquantity) : id(number), type(itype), price(iprice), quantity(iquantity)
    {
    }

    iPodType gettype() const
    {
        return type;
    }
    int getId() const
    {
        return id;
    }

    int getquant() const
    {
        return quantity;
    }

    int getprice() const
    {
        return price;
    }

    bool ispurchased(int quant)
    {
        if (quant <= 0 || quantity < quant)
            return false;
        quantity -= quant;
        return true;
    }
};

class Inventory
{
private:
    vector<iPod> details;
    int totalRevenue = 0;
    string name;

public:
    Inventory(string n = "") : name(n) {}

    string getName() const { return name; }

    bool addIPod(const iPod &product)
    {
        for (auto &prod : details)
        {
            if (prod.getId() == product.getId())
            {
                return false;
            }
        }
        details.push_back(product);
        return true;
    }

    vector<iPod> getInfo() const
    {
        return details;
    }

    bool purchaseStatus(int id, int quant)
    {
        if (quant <= 0 || quant % 10 != 0)
            return false;
        for (auto &product : details)
        {
            if (product.getId() == id && product.ispurchased(quant))
            {
                totalRevenue += 1LL * product.getprice() * quant;
                return true;
            }
        }
        return false;
    }

    bool validID(int custid)
    {
        for (auto &product : details)
        {
            if (product.getId() == custid)
            {
                return true;
            }
        }

        return false;
    }

    void displayInventory() const
    {
        if (details.empty())
        {
            cout << "Inventory is empty.\n";
            return;
        }

        for (const auto &product : details)
        {
            cout << "ID: " << product.getId()
                 << ", Quantity: " << product.getquant()
                 << '\n';
        }
    }

    void displaySalesReport() const
    {
        int totalUnits = 0;
        long long totalValue = 0;

        for (const auto &product : details)
        {
            totalUnits += product.getquant();
            totalValue += 1LL * product.getprice() * product.getquant();
        }

        cout << "Distinct products: " << details.size() << '\n';
        cout << "Total units in stock: " << totalUnits << '\n';
        cout << "Total inventory value: " << totalValue << '\n';
    }

    int getStock(int id) const
    {
        for (const auto &p : details)
            if (p.getId() == id)
                return p.getquant();
        return 0;
    }

    int getPrice(int id) const
    {
        for (const auto &p : details)
            if (p.getId() == id)
                return p.getprice();
        return -1;
    }
};

class OrderProcessor
{
private:
    vector<Inventory *> inventories;

public:
    OrderProcessor(vector<Inventory *> invs) : inventories(invs) {}

    long long calculateCost(Inventory *inv, int id, int qty)
    {
        int price = inv->getPrice(id);
        if (price < 0 || qty < 0)
            return -1;

        return 1LL * price * qty;
    }

    bool placeOrder(int id, int qty, string primary)
    {
        if (qty <= 0 || qty % 10 != 0)
            return false;

        Inventory *first = nullptr;

        for (auto *inv : inventories)
        {
            if (inv->getName() == primary)
            {
                first = inv;
                break;
            }
        }

        if (!first)
            return false;

        int totalStock = 0;
        for (auto *inv : inventories)
            totalStock += inv->getStock(id);

        if (totalStock < qty)
            return false;

        int remaining = qty;
        long long totalCost = 0;

        // Use the primary inventory first.
        int take = min(remaining, first->getStock(id));

        if (take > 0)
        {
            totalCost += calculateCost(first, id, take);
            if (!first->purchaseStatus(id, take))
                return false;

            remaining -= take;
        }

        // Use other inventories for the shortage.
        for (auto *inv : inventories)
        {
            if (inv == first || remaining == 0)
                continue;

            take = min(remaining, inv->getStock(id));

            if (take > 0)
            {
                totalCost += calculateCost(inv, id, take);
                totalCost += 400LL * (take / 10);
                if (!inv->purchaseStatus(id, take))
                    return false;

                remaining -= take;
            }
        }

        cout << "Total cost: $" << totalCost << '\n';
        return remaining == 0;
    }
};
class Customer
{
private:
    string name;
    OrderProcessor &op;

public:
    Customer(string n, OrderProcessor &opv) : name(n), op(opv)

    {
    }

    bool purchase(int id, int quant, string primary)
    {
        return op.placeOrder(id, quant, primary);
    }
};


// bool placeOrder(int id, int qty, string primary)
// {
//     if (qty <= 0 || qty % 10 != 0 || inventories.size() != 2)
//         return false;

//     Inventory* first = nullptr;
//     Inventory* second = nullptr;

//     for (auto* inv : inventories)
//     {
//         if (inv->getName() == primary)
//             first = inv;
//         else
//             second = inv;
//     }

//     if (!first || !second)
//         return false;

//     int s1 = first->getStock(id);
//     int s2 = second->getStock(id);

//     if (s1 + s2 < qty)
//         return false;

//     // Plan 1: use the primary warehouse first.
//     int from1 = min(qty, s1);
//     int from2 = qty - from1;

//     long long cost1 =
//         1LL * from1 * first->getPrice(id) +
//         1LL * from2 * second->getPrice(id);

//     if (from2 > 0)
//         cost1 += 400LL * (from2 / 10);

//     // Plan 2: use the other warehouse first.
//     int altFrom2 = min(qty, s2);
//     int altFrom1 = qty - altFrom2;

//     long long cost2 =
//         1LL * altFrom1 * first->getPrice(id) +
//         1LL * altFrom2 * second->getPrice(id);

//     if (altFrom1 > 0)
//         cost2 += 400LL * (altFrom1 / 10);

//     // Select the cheaper valid plan.
//     int use1, use2;
//     long long bestCost;

//     if (cost1 <= cost2)
//     {
//         use1 = from1;
//         use2 = from2;
//         bestCost = cost1;
//     }
//     else
//     {
//         use1 = altFrom1;
//         use2 = altFrom2;
//         bestCost = cost2;
//     }

//     // Deduct stock only after selecting the plan.
//     if (use1 > 0 && !first->purchaseStatus(id, use1))
//         return false;

//     if (use2 > 0 && !second->purchaseStatus(id, use2))
//         return false;

//     cout << "Total cost: $" << bestCost << '\n';
//     return true;
// }