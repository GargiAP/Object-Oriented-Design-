// #define CATCH_CONFIG_MAIN
// #include "catch.hpp"
// #include "ParkingLot.h"

// // Basic validation and parking behavior tests.
// // Adjust "Parking.h" if your implementation file has a different name.

// TEST_CASE("Valid vehicles can park and receive ticket details") {
//     Parking parking(2, 2);

//     Vehicle car("car", 101);
//     Ticket ticket = parking.park(car, 1);

//     REQUIRE(ticket.isValid());
//     REQUIRE(ticket.active);
//     REQUIRE(ticket.ticketId == 1);
//     REQUIRE(ticket.vehicleId == 101);
//     REQUIRE(ticket.floorNumber == 0);
//     REQUIRE(ticket.slot == 0);
//     REQUIRE(parking.slotsCar[0][0] == 1);
//     REQUIRE(parking.activeTickets.size() == 1);
//     REQUIRE(parking.details.size() == 1);
// }

// TEST_CASE("Cars and bikes use their separate slot inventories") {
//     Parking parking(1, 1);

//     Ticket carTicket = parking.park(Vehicle("car", 101), 1);
//     Ticket bikeTicket = parking.park(Vehicle("bike", 202), 2);

//     REQUIRE(carTicket.isValid());
//     REQUIRE(bikeTicket.isValid());
//     REQUIRE(parking.slotsCar[0][0] == 1);
//     REQUIRE(parking.slotsBike[0][0] == 2);
// }

// TEST_CASE("Unsupported vehicle types are rejected") {
//     Parking parking(2, 2);

//     Ticket ticket = parking.park(Vehicle("truck", 101), 1);

//     REQUIRE_FALSE(ticket.isValid());
//     REQUIRE(parking.activeTickets.empty());
//     REQUIRE(parking.details.empty());
// }

// TEST_CASE("Invalid vehicle and ticket IDs are rejected") {
//     Parking parking(2, 2);

//     REQUIRE_FALSE(parking.park(Vehicle("car", 101), 0).isValid());
//     REQUIRE_FALSE(parking.park(Vehicle("car", 101), -1).isValid());
//     REQUIRE_FALSE(parking.park(Vehicle("car", 0), 2).isValid());
//     REQUIRE_FALSE(parking.park(Vehicle("bike", -5), 3).isValid());

//     REQUIRE(parking.activeTickets.empty());
//     REQUIRE(parking.details.empty());
// }

// TEST_CASE("Same vehicle cannot park twice at once") {
//     Parking parking(2, 2);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());

//     Ticket second = parking.park(Vehicle("bike", 101), 2);

//     REQUIRE_FALSE(second.isValid());
//     REQUIRE(parking.activeTickets.size() == 1);
//     REQUIRE(parking.details.size() == 1);
// }

// TEST_CASE("Duplicate active ticket IDs are rejected across vehicle types") {
//     Parking parking(2, 2);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());

//     Ticket duplicate = parking.park(Vehicle("bike", 202), 1);

//     REQUIRE_FALSE(duplicate.isValid());
//     REQUIRE(parking.activeTickets.size() == 1);
// }

// TEST_CASE("Parking rejects a vehicle when its category is full") {
//     Parking parking(1, 1);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());

//     Ticket secondCar = parking.park(Vehicle("car", 102), 2);
//     REQUIRE_FALSE(secondCar.isValid());

//     // Bike capacity is independent, so a bike can still park.
//     REQUIRE(parking.park(Vehicle("bike", 201), 3).isValid());
// }

// TEST_CASE("Successful exit frees the correct slot and removes active records") {
//     Parking parking(2, 2);
//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());
//     REQUIRE(parking.leave(1, "car", 101));

//     REQUIRE(parking.slotsCar[0][0] == 0);
//     REQUIRE(parking.activeTickets.empty());
//     REQUIRE(parking.details.empty());
// }

// TEST_CASE("Wrong vehicle ID cannot release another vehicle's slot") {
//     Parking parking(2, 2);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());

//     REQUIRE_FALSE(parking.leave(1, "car", 999));

//     REQUIRE(parking.slotsCar[0][0] == 1);
//     REQUIRE(parking.activeTickets.size() == 1);
//     REQUIRE(parking.details.size() == 1);
// }

// TEST_CASE("Wrong vehicle type cannot release a slot") {
//     Parking parking(2, 2);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());

//     REQUIRE_FALSE(parking.leave(1, "bike", 101));

//     REQUIRE(parking.slotsCar[0][0] == 1);
//     REQUIRE(parking.activeTickets.size() == 1);
//     REQUIRE(parking.details.size() == 1);
// }

// TEST_CASE("Unknown and repeated exits are rejected") {
//     Parking parking(2, 2);

//     REQUIRE_FALSE(parking.leave(999, "car", 101));

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());
//     REQUIRE(parking.leave(1, "car", 101));
//     REQUIRE_FALSE(parking.leave(1, "car", 101));

//     REQUIRE(parking.slotsCar[0][0] == 0);
//     REQUIRE(parking.activeTickets.empty());
//     REQUIRE(parking.details.empty());
// }

// TEST_CASE("Vehicle can park again after a successful exit") {
//     Parking parking(1, 1);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());
//     REQUIRE(parking.leave(1, "car", 101));

//     Ticket next = parking.park(Vehicle("car", 101), 2);

//     REQUIRE(next.isValid());
//     REQUIRE(next.ticketId == 2);
//     REQUIRE(parking.slotsCar[0][0] == 2);
// }

// TEST_CASE("Ticket stores the correct floor and slot on multiple floors") {
//     Parking parking(vector<pair<int, int>>{{1, 1}, {2, 2}});

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());
//     Ticket secondCar = parking.park(Vehicle("car", 102), 2);

//     REQUIRE(secondCar.isValid());
//     REQUIRE(secondCar.floorNumber == 1);
//     REQUIRE(secondCar.slot == 0);

//     REQUIRE(parking.leave(2, "car", 102));
//     REQUIRE(parking.slotsCar[1][0] == 0);
//     REQUIRE(parking.slotsCar[0][0] == 1);
// }

// TEST_CASE("Exit frees only the ticket's assigned slot") {
//     Parking parking(2, 1);

//     REQUIRE(parking.park(Vehicle("car", 101), 1).isValid());
//     REQUIRE(parking.park(Vehicle("car", 102), 2).isValid());

//     REQUIRE(parking.leave(2, "car", 102));

//     REQUIRE(parking.slotsCar[0][0] == 1);
//     REQUIRE(parking.slotsCar[0][1] == 0);
// }

