#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

struct Guest {
    string roomNumber, name, gender, contact, checkInDate, checkOutDate, roomType, paymentStatus;
    int nights;
    float billAmount;
};

// Global vector to store guest records
vector<Guest> guests;

// Function to color text (CLI styling)
void color(string code) {
    cout << "\033[" << code << "m";
}

// Clear screen (works on most OS)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Load data from file
void loadGuests() {
    ifstream file("hotel_records.txt");
    if (!file) {
        color("1;33");
        cout << "No existing hotel records found. Starting fresh...\n";
        color("0");
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Guest g;
        string nightsStr, billStr;
        getline(ss, g.roomNumber, ',');
        getline(ss, g.name, ',');
        getline(ss, g.gender, ',');
        getline(ss, g.contact, ',');
        getline(ss, g.checkInDate, ',');
        getline(ss, g.checkOutDate, ',');
        getline(ss, g.roomType, ',');
        getline(ss, nightsStr, ',');
        g.nights = stoi(nightsStr);
        getline(ss, billStr, ',');
        g.billAmount = stof(billStr);
        getline(ss, g.paymentStatus, ',');
        guests.push_back(g);
    }
    file.close();
}

// Save data to file
void saveGuests() {
    ofstream file("hotel_records.txt");
    for (auto &g : guests) {
        file << g.roomNumber << "," << g.name << "," << g.gender << ","
             << g.contact << "," << g.checkInDate << "," << g.checkOutDate << ","
             << g.roomType << "," << g.nights << "," << g.billAmount << ","
             << g.paymentStatus << "\n";
    }
    file.close();
}

// Display a single guest record
void displayGuest(Guest &g) {
    color("1;36");
    cout << "\n---------------------------------------------\n";
    color("0");
    cout << "Room Number: " << g.roomNumber
         << "\nGuest Name: " << g.name
         << "\nGender: " << g.gender
         << "\nContact: " << g.contact
         << "\nCheck-In Date: " << g.checkInDate
         << "\nCheck-Out Date: " << g.checkOutDate
         << "\nRoom Type: " << g.roomType
         << "\nNights Stayed: " << g.nights
         << "\nBill Amount: " << fixed << setprecision(2) << g.billAmount
         << "\nPayment Status: " << g.paymentStatus << "\n";
}

// Add new guest record
void addGuest() {
    clearScreen();
    Guest g;
    cout << "Enter Room Number: ";
    cin >> g.roomNumber;
    cin.ignore();
    cout << "Enter Guest Name: ";
    getline(cin, g.name);
    cout << "Enter Gender (M/F): ";
    getline(cin, g.gender);
    cout << "Enter Contact Number: ";
    getline(cin, g.contact);
    cout << "Enter Check-In Date (DD/MM/YYYY): ";
    getline(cin, g.checkInDate);
    cout << "Enter Check-Out Date (DD/MM/YYYY): ";
    getline(cin, g.checkOutDate);
    cout << "Enter Room Type (Single/Double/Deluxe): ";
    getline(cin, g.roomType);
    cout << "Enter Number of Nights Stayed: ";
    cin >> g.nights;
    cout << "Enter Bill Amount: ";
    cin >> g.billAmount;
    cin.ignore();
    cout << "Enter Payment Status (Paid/Pending): ";
    getline(cin, g.paymentStatus);

    guests.push_back(g);
    saveGuests();

    color("1;32");
    cout << "\nGuest record added successfully!\n";
    color("0");
}

// View all guest records
void viewAll() {
    clearScreen();
    if (guests.empty()) {
        cout << "No guest records available.\n";
        return;
    }
    for (auto &g : guests)
        displayGuest(g);
}

// Search guest by room number
void searchGuest() {
    clearScreen();
    string room;
    cout << "Enter Room Number to search: ";
    cin >> room;
    bool found = false;

    for (auto &g : guests) {
        if (g.roomNumber == room) {
            displayGuest(g);
            found = true;
        }
    }

    if (!found)
        cout << "No record found for Room Number " << room << endl;
}

// Delete guest record
void deleteGuest() {
    clearScreen();
    string room;
    cout << "Enter Room Number to delete record: ";
    cin >> room;
    auto it = remove_if(guests.begin(), guests.end(), [&](Guest &g) {
        return g.roomNumber == room;
    });
    if (it != guests.end()) {
        guests.erase(it, guests.end());
        saveGuests();
        color("1;31");
        cout << "Guest record deleted successfully!\n";
        color("0");
    } else
        cout << "Room not found!\n";
}

// Summary Report
void summaryReport() {
    clearScreen();
    int single = 0, dbl = 0, deluxe = 0, pending = 0, paid = 0;
    float totalRevenue = 0;

    for (auto &g : guests) {
        if (g.roomType == "Single" || g.roomType == "single") single++;
        if (g.roomType == "Double" || g.roomType == "double") dbl++;
        if (g.roomType == "Deluxe" || g.roomType == "deluxe") deluxe++;
        if (g.paymentStatus == "Paid" || g.paymentStatus == "paid") paid++;
        else pending++;
        totalRevenue += g.billAmount;
    }

    color("1;33");
    cout << "\n===== HOTEL SUMMARY REPORT =====\n";
    color("0");
    cout << "Total Guests: " << guests.size() << "\n";
    cout << "Single Rooms: " << single
         << "\nDouble Rooms: " << dbl
         << "\nDeluxe Rooms: " << deluxe << "\n";
    cout << "Payments Pending: " << pending
         << "\nPayments Completed: " << paid << "\n";
    cout << "Total Revenue (Approx.): " << fixed << setprecision(2) << totalRevenue << "\n";
}

// Main Menu
int main() {
    loadGuests();
    int choice;

    while (true) {
        color("1;34");
        cout << "\n===== HOTEL MANAGEMENT SYSTEM =====\n";
        color("0");
        cout << "1. Add New Guest\n2. View All Guests\n3. Search Guest\n";
        cout << "4. Delete Guest Record\n5. Summary Report\n6. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: addGuest(); break;
        case 2: viewAll(); break;
        case 3: searchGuest(); break;
        case 4: deleteGuest(); break;
        case 5: summaryReport(); break;
        case 6:
            color("1;33");
            cout << "Exiting... All data saved successfully!\n";
            color("0");
            saveGuests();
            return 0;
        default:
            cout << "Invalid choice. Try again!\n";
        }
    }
    return 0;
}
