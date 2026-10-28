#include <bits/stdc++.h>
using namespace std;

enum class CustomerType {
    Regular,
    Rewardee
};

struct HotelDetails {
    string name;
    int rating;
    int regularWeekday;
    int regularWeekend;
    int rewardWeekday;
    int rewardWeekend;
};

struct Room {
    int roomNumber;
    string roomType;
    double pricePerNight;
};

struct Booking {
    int bookingId;
    string customerName;
    int roomNumber;
    string checkIn;
    string checkOut;
    bool cancelled = false;
};

// Part A: Hotel price comparison
class HotelPriceComparator {
private:
    vector<HotelDetails> hotels;

public:
    HotelPriceComparator() {
        hotels = {
            {"X", 3, 100, 120, 90, 60},
            {"Y", 5, 130, 150, 100, 95},
            {"Z", 4, 195, 150, 120, 90}
        };
    }

    bool parseDate(const string& input, tm& date) const {
        istringstream ss(input);
        ss >> get_time(&date, "%d%b%Y");

        if (ss.fail())
            return false;

        date.tm_hour = 12;
        date.tm_min = 0;
        date.tm_sec = 0;
        date.tm_isdst = -1;

        time_t timestamp = mktime(&date);
        if (timestamp == -1)
            return false;

        tm* normalized = localtime(&timestamp);
        if (!normalized)
            return false;

        if (normalized->tm_mday != date.tm_mday ||
            normalized->tm_mon != date.tm_mon ||
            normalized->tm_year != date.tm_year)
            return false;

        date = *normalized;
        return true;
    }

    bool isWeekend(const tm& date) const {
        return date.tm_wday == 0 || date.tm_wday == 6;
    }

    int getRate(const HotelDetails& hotel,
                CustomerType type, bool weekend) const {
        if (type == CustomerType::Regular)
            return weekend ? hotel.regularWeekend
                           : hotel.regularWeekday;

        return weekend ? hotel.rewardWeekend
                       : hotel.rewardWeekday;
    }

    long long calculateCost(const HotelDetails& hotel,
                            CustomerType type,
                            const vector<tm>& dates) const {
        long long total = 0;

        for (const auto& date : dates)
            total += getRate(hotel, type, isWeekend(date));

        return total;
    }

    void findCheapestHotel(CustomerType type,
                           const vector<tm>& dates) const {
        if (dates.empty()) {
            cout << "No dates provided.\n";
            return;
        }

        const HotelDetails* bestHotel = nullptr;
        long long bestCost = LLONG_MAX;

        for (const auto& hotel : hotels) {
            long long cost = calculateCost(hotel, type, dates);

            if (cost < bestCost ||
                (cost == bestCost &&
                 (!bestHotel || hotel.rating > bestHotel->rating))) {
                bestCost = cost;
                bestHotel = &hotel;
            }
        }

        cout << "Cheapest hotel: " << bestHotel->name << '\n';
        cout << "Rating: " << bestHotel->rating << " stars\n";
        cout << "Total cost: $" << bestCost << '\n';
    }
};

// Part B: Room and booking management
class Hotel {
private:
    vector<Room> rooms;
    vector<Booking> bookings;
    int nextBookingId = 1;

    bool roomExists(int roomNumber) const {
        for (const auto& room : rooms) {
            if (room.roomNumber == roomNumber)
                return true;
        }
        return false;
    }

public:
    void addRoom(const Room& room) {
        if (roomExists(room.roomNumber)) {
            cout << "Room already exists.\n";
            return;
        }

        rooms.push_back(room);
    }

    bool isRoomAvailable(int roomNumber,
                         const string& checkIn,
                         const string& checkOut) const {
        if (checkIn >= checkOut || !roomExists(roomNumber))
            return false;

        for (const auto& booking : bookings) {
            if (!booking.cancelled &&
                booking.roomNumber == roomNumber &&
                checkIn < booking.checkOut &&
                checkOut > booking.checkIn) {
                return false;
            }
        }

        return true;
    }

    bool bookRoom(const string& customerName,
                  int roomNumber,
                  const string& checkIn,
                  const string& checkOut) {
        if (!isRoomAvailable(roomNumber, checkIn, checkOut)) {
            cout << "Room unavailable or invalid dates.\n";
            return false;
        }

        bookings.push_back({
            nextBookingId++,
            customerName,
            roomNumber,
            checkIn,
            checkOut,
            false
        });

        cout << "Booking successful. ID: "
             << bookings.back().bookingId << '\n';

        return true;
    }

    bool cancelBooking(int bookingId) {
        for (auto& booking : bookings) {
            if (booking.bookingId == bookingId &&
                !booking.cancelled) {
                booking.cancelled = true;
                cout << "Booking cancelled.\n";
                return true;
            }
        }

        cout << "Booking not found or already cancelled.\n";
        return false;
    }

    void showAvailableRooms(const string& checkIn,
                            const string& checkOut) const {
        bool found = false;

        for (const auto& room : rooms) {
            if (isRoomAvailable(room.roomNumber, checkIn, checkOut)) {
                cout << "Room " << room.roomNumber
                     << " | " << room.roomType
                     << " | Price per night: "
                     << room.pricePerNight << '\n';
                found = true;
            }
        }

        if (!found)
            cout << "No rooms available for these dates.\n";
    }

    void printBill(int bookingId) const {
        for (const auto& booking : bookings) {
            if (booking.bookingId != bookingId ||
                booking.cancelled) {
                continue;
            }

            for (const auto& room : rooms) {
                if (room.roomNumber == booking.roomNumber) {
                    // Dates use YYYY-MM-DD.
                    // For this basic version, count nights using
                    // the standard date format and validation externally.
                    tm start{}, finish{};
                    istringstream in1(booking.checkIn);
                    istringstream in2(booking.checkOut);

                    in1 >> get_time(&start, "%Y-%m-%d");
                    in2 >> get_time(&finish, "%Y-%m-%d");

                    start.tm_hour = finish.tm_hour = 12;
                    start.tm_isdst = finish.tm_isdst = -1;

                    time_t t1 = mktime(&start);
                    time_t t2 = mktime(&finish);

                    if (t1 == -1 || t2 == -1) {
                        cout << "Invalid booking dates.\n";
                        return;
                    }

                    long long nights =
                        static_cast<long long>(difftime(t2, t1) / 86400);

                    if (nights <= 0) {
                        cout << "Invalid booking dates.\n";
                        return;
                    }

                    cout << "\n--- Booking Bill ---\n";
                    cout << "Booking ID: " << booking.bookingId << '\n';
                    cout << "Customer: " << booking.customerName << '\n';
                    cout << "Room: " << room.roomNumber << '\n';
                    cout << "Check-in: " << booking.checkIn << '\n';
                    cout << "Check-out: " << booking.checkOut << '\n';
                    cout << "Nights: " << nights << '\n';
                    cout << "Total: "
                         << nights * room.pricePerNight << '\n';
                    return;
                }
            }
        }

        cout << "Active booking not found.\n";
    }
};

int main() {
    // Part A: Price comparison
    HotelPriceComparator comparator;

    string customerInput, datesInput;
    cout << "Enter customer type (Regular/Rewardee): ";
    cin >> customerInput;

    CustomerType type;
    if (customerInput == "Regular")
        type = CustomerType::Regular;
    else if (customerInput == "Rewardee")
        type = CustomerType::Rewardee;
    else {
        cout << "Invalid customer type.\n";
        return 0;
    }

    cout << "Enter comma-separated dates (e.g. 16Mar2010,19Mar2010): ";
    cin >> datesInput;

    vector<tm> dates;
    stringstream ss(datesInput);
    string token;

    while (getline(ss, token, ',')) {
        tm date{};
        if (token.empty() || !comparator.parseDate(token, date)) {
            cout << "Invalid date: " << token << '\n';
            return 0;
        }
        dates.push_back(date);
    }

    comparator.findCheapestHotel(type, dates);

    // Part B: Room booking demo
    Hotel hotel;

    hotel.addRoom({101, "Standard", 2000});
    hotel.addRoom({102, "Deluxe", 3500});
    hotel.addRoom({103, "Suite", 5000});

    cout << "\nAvailable rooms for Oct 20-22:\n";
    hotel.showAvailableRooms("2026-10-20", "2026-10-22");

    hotel.bookRoom("Gargi", 101, "2026-10-20", "2026-10-22");
    hotel.bookRoom("Amit", 101, "2026-10-21", "2026-10-23");

    hotel.printBill(1);

    hotel.cancelBooking(1);

    cout << "\nAvailable rooms after cancellation:\n";
    hotel.showAvailableRooms("2026-10-20", "2026-10-22");

    return 0;
}