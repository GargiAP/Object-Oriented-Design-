#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <ctime>
#include "fee.h"
using namespace std;

class Vehicle
{
public:
    int id;
    string vehicleType;
    Vehicle(string type, int vid)
    {
        vehicleType = type;
        id = vid;
    }
};

class Ticket
{
public:
    bool valid;
    bool active;
    int ticketId;
    int floorNumber;
    int slot;
    int entrytime;
    int vehicleId;
    Ticket(bool v, int id, bool a = false,
           int floor = 0, int sslot = 0,
           int eentrytime = 0, int vid = -1)
        : valid(v), active(a), ticketId(id),
          floorNumber(floor), slot(sslot),
          entrytime(eentrytime), vehicleId(vid) {}
    bool isValid()
    {
        return valid;
    }
};

class Parking
{
public:
    vector<vector<int>> slotsCar; // slotsCar[floor][slot]
    vector<vector<int>> slotsBike;
    vector<Ticket> activeTickets;
    Fee fee;
    vector<Vehicle> details;

    Parking(vector<pair<int, int>> floors)
    {
        for (int f = 0; f < floors.size(); f++)
        {
            slotsCar.push_back(vector<int>(floors[f].first, 0));
            slotsBike.push_back(vector<int>(floors[f].second, 0));
        }
    }
    bool leave(int ticketId, string vehicleType, int id, Payment &payment)
    {
        if (ticketId <= 0 || id <= 0 ||
            (vehicleType != "car" && vehicleType != "bike"))
            return false;

        auto ticketIt = activeTickets.begin();

        for (; ticketIt != activeTickets.end(); ++ticketIt)
        {
            if (ticketIt->ticketId == ticketId && ticketIt->active)
                break;
        }

        if (ticketIt == activeTickets.end())
            return false;

        // Ensure this ticket belongs to the supplied vehicle.
        if (ticketIt->vehicleId != id)
            return false;

        vector<vector<int>> &slots =
            (vehicleType == "car") ? slotsCar : slotsBike;

        int f = ticketIt->floorNumber;
        int s = ticketIt->slot;

        if (f < 0 || f >= slots.size() ||
            s < 0 || s >= slots[f].size() ||
            slots[f][s] != ticketId)
            return false;

        auto vehicleIt = details.begin();

        for (; vehicleIt != details.end(); ++vehicleIt)
        {
            if (vehicleIt->id == id &&
                vehicleIt->vehicleType == vehicleType)
                break;
        }

        if (vehicleIt == details.end())
            return false;
        int time = getCurrentTime() - ticketIt->entrytime;
        int n = fee.getFees(time);
        if (!payment.pay(n))
        {
            return false; // Keep the vehicle parked
        }

        slots[f][s] = 0;
        details.erase(vehicleIt);
        activeTickets.erase(ticketIt);

        return true;
    }
    int getCurrentTime()
    {
        return static_cast<int>(time(nullptr));
    }
    Ticket park(Vehicle v, int ticketId)
    {
        if (v.vehicleType != "car" && v.vehicleType != "bike")
        {
            return Ticket(false, -1);
        }

        if (ticketId <= 0)
            return Ticket(false, -1);
        if (v.id <= 0)
            return Ticket(false, -1);
        for (auto &d : details)
        {
            if (d.id == v.id)
            {

                return Ticket(false, -1);
            }
        }

        for (auto &t : activeTickets)
        {
            if (t.ticketId == ticketId)
            {

                return Ticket(false, -1);
            }
        }

        vector<vector<int>> &slots = (v.vehicleType == "car") ? slotsCar : slotsBike;
        int entrytime;
        for (int f = 0; f < slots.size(); f++)
        {
            for (int i = 0; i < slots[f].size(); i++)
            {
                if (slots[f][i] == ticketId)
                    return Ticket(false, -1);

                if (slots[f][i] == 0)
                {
                    slots[f][i] = ticketId;
                    entrytime = getCurrentTime();
                    details.push_back(v);
                    Ticket t(true, ticketId, true, f, i, entrytime, v.id);
                    activeTickets.push_back(t);

                    return t;
                }
            }
        }
        return Ticket(false, -1);
    }

    void displayAvailability()
    {
        int totalCarFree = 0, totalBikeFree = 0;
        int totalCarSlots = 0, totalBikeSlots = 0;

        for (int f = 0; f < slotsCar.size(); f++)
        {
            int freeCars = 0, occupiedCars = 0;
            int freeBikes = 0, occupiedBikes = 0;

            for (int slot : slotsCar[f])
            {
                if (slot == 0)
                    freeCars++;
                else
                    occupiedCars++;
            }

            for (int slot : slotsBike[f])
            {
                if (slot == 0)
                    freeBikes++;
                else
                    occupiedBikes++;
            }

            cout << "\nFloor " << f + 1 << ":\n";

            cout << "Cars  - Free: " << freeCars
                 << ", Occupied: " << occupiedCars << '\n';

            cout << "Bikes - Free: " << freeBikes
                 << ", Occupied: " << occupiedBikes << '\n';

            totalCarFree += freeCars;
            totalBikeFree += freeBikes;
            totalCarSlots += slotsCar[f].size();
            totalBikeSlots += slotsBike[f].size();
        }

        cout << "\nOverall availability:\n";

        cout << "Cars: "
             << (totalCarFree == 0 ? "FULL" : "AVAILABLE")
             << " (" << totalCarFree << " free out of "
             << totalCarSlots << ")\n";

        cout << "Bikes: "
             << (totalBikeFree == 0 ? "FULL" : "AVAILABLE")
             << " (" << totalBikeFree << " free out of "
             << totalBikeSlots << ")\n";

        if (totalCarFree == 0 && totalBikeFree == 0)
            cout << "The entire parking lot is FULL.\n";
    }
};