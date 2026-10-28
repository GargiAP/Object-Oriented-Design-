
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

enum class ProductType
{
    PhysicalProduct,
    DigitalProduct
};

// Product information
struct Product
{
    int id;
    string name;
    double price;
    ProductType type;
    double shippingFee;
};

struct CartItem
{
    Product product;
    int quantity;
};

// Abstract Discount class
class Discount
{
public:
    virtual double calculate(double subtotal) const = 0;
    virtual ~Discount() = default;
};

// Fixed discount
class FixedDiscount : public Discount
{
private:
    double amount;

public:
    explicit FixedDiscount(double amount) : amount(amount) {}

    double calculate(double subtotal) const override
    {
        if (amount < 0 || subtotal < 0)
            return -1;

        return min(amount, subtotal);
    }
};

class PercentageDiscount : public Discount
{
private:
    double percentage;

public:
    explicit PercentageDiscount(double percentage)
        : percentage(percentage) {}

    double calculate(double subtotal) const override
    {
        if (percentage < 0 || percentage > 100 || subtotal < 0)
            return -1;

        return subtotal * percentage / 100.0;
    }
};

class ShoppingCart
{
private:
    vector<CartItem> items;

public:
    bool addProduct(const Product &product, int quantity)
    {
        if (product.id <= 0 ||
            product.price < 0 ||
            product.shippingFee < 0 ||
            quantity <= 0)
        {
            return false;
        }

        for (auto &item : items)
        {
            if (item.product.id == product.id)
            {
                // Same ID must represent the same product.
                if (item.product.name != product.name ||
                    item.product.price != product.price ||
                    item.product.type != product.type ||
                    item.product.shippingFee != product.shippingFee)
                {
                    return false;
                }

                if (quantity > INT_MAX - item.quantity)
                    return false;

                item.quantity += quantity;
                return true;
            }
        }

        items.push_back({product, quantity});
        return true;
    }

    bool removeProduct(int id)
    {
        auto it = find_if(items.begin(), items.end(),
                          [id](const CartItem &item)
                          {
                              return item.product.id == id;
                          });

        if (it == items.end())
            return false;

        items.erase(it);
        return true;
    }

    bool updateQuantity(int id, int newQuantity)
    {
        if (newQuantity <= 0)
            return false;

        for (auto &item : items)
        {
            if (item.product.id == id)
            {
                item.quantity = newQuantity;
                return true;
            }
        }

        return false;
    }

    double calculateSubtotal() const
    {
        double subtotal = 0;

        for (const auto &item : items)
        {
            subtotal += item.product.price * item.quantity;
        }

        return subtotal;
    }

    double calculateShipping() const
    {
        double shipping = 0;

        for (const auto &item : items)
        {
            if (item.product.type == ProductType::PhysicalProduct)
            {
                shipping += item.product.shippingFee * item.quantity;
            }
        }

        return shipping;
    }

    double applyDiscount(const Discount &discount) const
    {
        return discount.calculate(calculateSubtotal());
    }

    double totalPrice(const Discount &discount) const
    {
        double discountAmount = applyDiscount(discount);

        if (discountAmount < 0)
            return -1; // Invalid discount

        return calculateSubtotal() - discountAmount + calculateShipping();
    }

    void displayCart(const Discount &discount) const
    {
        for (const auto &item : items)
        {
            cout << item.product.name
                 << " x " << item.quantity
                 << " = " << item.product.price * item.quantity
                 << '\n';
        }

        double discountAmount = applyDiscount(discount);

        cout << "Subtotal: " << calculateSubtotal() << '\n';
        cout << "Shipping: " << calculateShipping() << '\n';

        if (discountAmount < 0)
        {
            cout << "Invalid discount\n";
            return;
        }

        cout << "Discount: " << discountAmount << '\n';
        cout << "Total: " << totalPrice(discount) << '\n';
    }

    void clearCart()
    {
        items.clear();
    }
};

int main()
{
    ShoppingCart cart;

    Product tshirt{
        1, "T-shirt", 500,
        ProductType::PhysicalProduct, 50};

    Product ebook{
        2, "E-book", 200,
        ProductType::DigitalProduct, 0};

    assert(cart.addProduct(tshirt, 2));
    assert(cart.addProduct(ebook, 1));

    cout << "--- Percentage discount: 10% ---\n";
    PercentageDiscount percentageDiscount(10);
    cart.displayCart(percentageDiscount);

    cout << "\n--- Fixed discount: 200 ---\n";
    FixedDiscount fixedDiscount(200);
    cart.displayCart(fixedDiscount);

    cout << "\n--- Updating quantity ---\n";
    assert(cart.updateQuantity(1, 3));
    cart.displayCart(percentageDiscount);

    assert(!cart.updateQuantity(99, 2));
    assert(!cart.updateQuantity(1, -1));

    cout << "\n--- Invalid discount test ---\n";
    PercentageDiscount invalidDiscount(120);
    assert(cart.totalPrice(invalidDiscount) == -1);

    return 0;
}
