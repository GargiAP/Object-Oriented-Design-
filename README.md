# Object-Oriented Design (LLD) in Modern C++

A comprehensive collection of **Object-Oriented Design (OOD)** and **Low-Level Design (LLD)** case studies implemented in Modern C++. 

Each system models real-world business domains adhering to **SOLID principles**, robust domain modeling, clean abstractions, and comprehensive unit testing using the [Catch2](https://github.com/catchorg/Catch2) test framework.

---

## 📑 Table of Contents

- [Overview](#-overview)
- [Design Principles & Patterns](#-design-principles--patterns)
- [Projects Catalog](#-projects-catalog)
  - [1. ATM System](#1-atm-system)
  - [2. Food Ordering System](#2-food-ordering-system)
  - [3. Hotel Reservation & Price Comparator](#3-hotel-reservation--price-comparator)
  - [4. iPod Inventory & Order Management](#4-ipod-inventory--order-management)
  - [5. Mars Rover](#5-mars-rover)
  - [6. Multi-Floor Parking Lot](#6-multi-floor-parking-lot)
  - [7. E-Commerce Shopping Cart & Checkout Engine](#7-e-commerce-shopping-cart--checkout-engine)
  - [8. Stock & Inventory Management](#8-stock--inventory-management)
  - [9. Vending Machine](#9-vending-machine)
- [Repository Structure](#-repository-structure)
- [Getting Started & How to Run](#-getting-started--how-to-run)
- [Unit Testing](#-unit-testing)

---

## 💡 Overview

This repository demonstrates practical implementations of common system design and interview-level object-oriented design problems. The focus is on:
- **Encapsulation & Information Hiding**: Guarding internal state invariants (e.g. balances, stock levels, daily limits).
- **Extensibility & Polymorphism**: Using abstract interfaces and inheritance hierarchies for flexible business rules (e.g. account types, discounts, payment methods, fee strategies).
- **Defensive Programming**: Validating boundaries, preventing over-allocation, and handling failure modes cleanly.
- **Test-Driven Design**: Validating behavioral requirements and edge cases with unit tests.

---

## 🏛️ Design Principles & Patterns

| Principle / Pattern | Application in Repository |
| :--- | :--- |
| **Single Responsibility (SRP)** | Dedicated classes for domain entities (`Account`, `TransactionHistory`, `Menu`, `Ticket`, `Inventory`). |
| **Open / Closed (OCP)** | Extensible abstractions such as `Discount` (`FixedDiscount`, `PercentageDiscount`) and `PaymentMethod`. |
| **Liskov Substitution (LSP)** | `OverdraftAccount` substituting `Account` with extended overdraft allowances without breaking debit invariants. |
| **Strategy Pattern** | Dynamic calculation of parking fees (`Fee`), discount strategies (`Discount`), and payment gateways (`PaymentMethod`). |
| **State / Command Pattern** | Rover orientation and motion commands (`Plateau` / `Rover`), Vending Machine state transactions. |

---

## 📦 Projects Catalog

### 1. ATM System
- **Directory**: [`ATM/`](ATM/)
- **Core Entities**: `Account`, `OverdraftAccount`, `TransactionHistory`, `ATMmachine`
- **Key Features**:
  - Secure PIN validation with automatic 3-strike lockout counter.
  - Polymorphic accounts supporting standard accounts and overdraft-protected accounts.
  - Daily debit thresholds and minimum allowed balance rules.
  - Transaction ledger history tracking deposits and withdrawals.
  - Multi-denomination cash dispensing safeguards.

### 2. Food Ordering System
- **Directory**: [`Food/`](Food/)
- **Core Entities**: `Restaurant`, `Menu`, `Order`, `Customer`, `Cart`, `PaymentMethod` (`Cash`, `Card`, `UPI`)
- **Key Features**:
  - Multi-restaurant catalog with categorized menus (Starters, Main Course, Desserts, Beverages).
  - Dietary classification (Veg / Non-Veg) and item availability status.
  - Customer cart operations, order creation, and status transitions (`Placed`, `Preparing`, `Delivered`, `Cancelled`).
  - Strategy-based polymorphic checkout with flexible payment processors.

### 3. Hotel Reservation & Price Comparator
- **Directory**: [`Hotel/`](Hotel/)
- **Core Entities**: `HotelPriceComparator`, `HotelDetails`, `Room`, `Booking`
- **Key Features**:
  - Multi-hotel price comparison across dynamic date intervals.
  - Differentiated pricing models: Weekday vs. Weekend rates, Regular vs. Loyalty (Rewardee) customers.
  - Robust date parsing and calendar validation using C++ standard time utilities.
  - Best-deal discovery algorithm with rating tie-breaker logic.

### 4. iPod Inventory & Order Management
- **Directory**: [`iPod/`](iPod/)
- **Core Entities**: `iPod`, `Inventory`
- **Key Features**:
  - Multi-variant device tracking across multiple hardware generations (`Apple1`, `Apple2`, `Apple3`).
  - Stock validation, purchase order fulfillment, and replenishment.
  - Automated revenue accounting and per-model inventory metrics.

### 5. Mars Rover
- **Directory**: [`MarsRover/`](MarsRover/)
- **Core Entities**: `Plateau`, `Rover`, `Direction` (`N`, `E`, `S`, `W`)
- **Key Features**:
  - Bounded Cartesian plateau grid validation.
  - Command interpreter supporting Left (`L`), Right (`R`), and Move (`M`) directives.
  - Exception handling for boundary overflow, invalid coordinates, or malformed input sequences.

### 6. Multi-Floor Parking Lot
- **Directory**: [`parkinglot/`](parkinglot/)
- **Core Entities**: `Parking`, `Vehicle`, `Ticket`, `Fee`
- **Key Features**:
  - Multi-level parking garage supporting heterogeneous vehicle types (Cars, Bikes).
  - Slot allocation algorithms across distinct floor capacities.
  - Timestamped ticket lifecycle (entry, active tracking, exit, settlement).
  - Dynamic fee engine calculating costs based on vehicle classification and duration.

### 7. E-Commerce Shopping Cart & Checkout Engine
- **Directory**: [`ShoppingCart/`](ShoppingCart/)
- **Core Entities**: `ShoppingCart`, `Product`, `CartItem`, `Discount` (`FixedDiscount`, `PercentageDiscount`)
- **Key Features**:
  - Hybrid product modeling (Physical goods with shipping fees vs. digital goods with instant delivery).
  - Open-Closed compliant discount pipeline allowing stacking or custom promo strategies.
  - Real-time subtotal, shipping fee, tax, and order total computations.

### 8. Stock & Inventory Management
- **Directory**: [`Stock/`](Stock/)
- **Core Entities**: `Inventory`, `Product`
- **Key Features**:
  - SKU / Product management with unique identifier validation.
  - Stock restocking and transactional selling operations with inventory bound enforcement.
  - Real-time product lookup and stock level inspection.

### 9. Vending Machine
- **Directory**: [`VendingMachine/`](VendingMachine/)
- **Core Entities**: `VendingMachine`, `Product`, `Payment`
- **Key Features**:
  - Product selection, stock quantity verification, and credit tracking.
  - Insufficient fund checks, change computation, and item dispensing.
  - Detailed result status enumeration (`Dispensed`, `InsufficientFunds`, `OutOfStock`, `PaymentFailed`).

---

## 🗂️ Repository Structure

```text
.
├── ATM/
│   ├── ATM.h               # ATM, Account, Transaction classes
│   └── test.cpp            # ATM Catch2 test suite
├── Food/
│   ├── Customer.h          # Customer profile & cart
│   ├── FoodOrdering.h      # Restaurant, Menu, & System classes
│   ├── order.h             # Order lifecycle management
│   ├── payment.h           # Payment strategy interfaces & classes
│   └── test.cpp            # Food ordering test suite
├── Hotel/
│   ├── hotel.h             # Hotel details, pricing comparator, bookings
│   └── test.cpp            # Hotel Catch2 test suite
├── iPod/
│   ├── iPod.h              # iPod model & inventory management
│   └── test.cpp            # iPod test suite
├── MarsRover/
│   └── mars.cpp            # Rover simulation, plateau grid, & parser
├── parkinglot/
│   ├── ParkingLot.h        # Parking allocation & ticket tracker
│   ├── fee.h               # Dynamic fee calculator
│   ├── main.cpp            # Standalone runner
│   └── test.cpp            # Parking lot test suite
├── ShoppingCart/
│   └── shopping.cpp        # Shopping cart, discount strategies, & assertions
├── Stock/
│   ├── stock.h             # Stock & inventory models
│   └── test.cpp            # Stock management test suite
├── VendingMachine/
│   ├── VendingMachine.h    # Vending machine engine & state handlers
│   ├── main.cpp            # Interactive runner
│   └── test1.cpp           # Vending machine test suite
└── .gitignore              # Ignores compiler output, binaries, and IDE caches
```

---

## 🚀 Getting Started & How to Run

### Prerequisites
- A modern C++ compiler supporting C++17 or higher (`g++`, `clang++`, or MSVC).

### Building and Running Tests

Each subfolder contains standalone test suites using Catch2 or native assertions. You can compile and run any module with standard GCC/Clang commands:

#### ATM System
```bash
g++ -std=c++17 ATM/test.cpp -o ATM_test
./ATM_test
```

#### Food Ordering System
```bash
g++ -std=c++17 Food/test.cpp -o Food_test
./Food_test
```

#### Mars Rover
```bash
g++ -std=c++17 MarsRover/mars.cpp -o mars_rover
./mars_rover
```

#### Parking Lot System
```bash
g++ -std=c++17 parkinglot/test.cpp -o parking_test
./parking_test
```

#### Shopping Cart
```bash
g++ -std=c++17 ShoppingCart/shopping.cpp -o shopping_test
./shopping_test
```

#### Vending Machine
```bash
g++ -std=c++17 VendingMachine/test1.cpp -o vending_test
./vending_test
```

---

## 🧪 Unit Testing

All systems are validated with unit tests asserting edge cases such as:
- Negative balances, overdraft ceilings, and bad PIN attempts.
- Empty carts, invalid voucher percentages, and duplicate item additions.
- Out-of-bounds rover movements and invalid command inputs.
- Slot exhaustion and invalid ticket redemptions in parking structures.

---

## 👤 Author

- **Gargi A. Pratapwar** ([@GargiAP](https://github.com/GargiAP))
