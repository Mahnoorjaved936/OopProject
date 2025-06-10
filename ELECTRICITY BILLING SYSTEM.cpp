#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

// ===== Appliance Class =====
class Appliance {
public:
    string name;
    int quantity;
    int hoursPerDay;
    int powerWatts;

    Appliance(string n, int q, int h, int p) : name(n), quantity(q), hoursPerDay(h), powerWatts(p) {}

    double monthlyUnits() const {
        return quantity * (hoursPerDay * powerWatts * 30) / 1000.0; // kWh
    }
};

// ===== BillCalculator Class =====
class BillCalculator {
public:
    static double calculateBill(double units, int userType) {
        const double slab1_rate = 10.0;
        const double slab2_rate = 15.0;
        const double slab3_rate = 20.0;

        const double commercial_slab1_rate = 12.0;
        const double commercial_slab2_rate = 18.0;
        const double commercial_slab3_rate = 25.0;

        double bill = 0.0;

        if (userType == 1) {
            if (units <= 100)
                bill = units * slab1_rate;
            else if (units <= 300)
                bill = 100 * slab1_rate + (units - 100) * slab2_rate;
            else
                bill = 100 * slab1_rate + 200 * slab2_rate + (units - 300) * slab3_rate;
        } else {
            if (units <= 100)
                bill = units * commercial_slab1_rate;
            else if (units <= 300)
                bill = 100 * commercial_slab1_rate + (units - 100) * commercial_slab2_rate;
            else
                bill = 100 * commercial_slab1_rate + 200 * commercial_slab2_rate + (units - 300) * commercial_slab3_rate;
        }

        return bill;
    }
};

// ===== SuggestionEngine Class =====
class SuggestionEngine {
public:
    static void suggestReduction(const vector<Appliance>& appliances, double currentUnits, double targetReduction) {
        cout << "\n--- Suggestions to Reduce Your Bill ---\n";
        double reduced = 0;

        for (int i = 0; i < (int)appliances.size(); ++i) {
            double usage = appliances[i].monthlyUnits();

            if (usage > 5 && reduced < targetReduction) {
                double saved = (appliances[i].quantity * appliances[i].powerWatts * 30) / 1000.0;
                cout << "- Use " << appliances[i].name << " for 1 hour less/day. Saves ~" << saved << " units/month.\n";
                reduced += saved;
            }
        }

        if (reduced < targetReduction) {
            cout << "- Also consider using energy-efficient models (e.g., inverter AC, LED lights).\n";
        }
    }
};

// ===== SolarUser Class =====
class SolarUser {
private:
    bool hasSolarPanel;
    double SolarGeneration;
    double totalConsumption;
    const double ratePerKWh = 12.0;

public:
    void userDetails() {
        string input;
        cout << "Do You Have Solar Panels Installed? (yes / no): ";
        cin >> input;
        for (int i = 0; i < (int)input.length(); ++i) {
            input[i] = tolower(input[i]);
        }

        hasSolarPanel = (input == "yes");

        cout << "Enter Your Total Monthly Energy Consumption (kWh): ";
        cin >> totalConsumption;

        if (hasSolarPanel) {
            cout << "Enter Your Average Monthly Solar Energy Generation (kWh): ";
            cin >> SolarGeneration;
        } else {
            SolarGeneration = 0;
        }
    }

    double getNetUnits() {
        return totalConsumption - SolarGeneration;
    }

    void displaySolarSummary() {
        cout << fixed << setprecision(2);
        cout << "\n---- Solar Energy Summary ----\n";
        cout << "Total Consumption: " << totalConsumption << " kWh\n";
        cout << "Solar Generation: " << SolarGeneration << " kWh\n";

        if (SolarGeneration > totalConsumption) {
            double profitKWh = SolarGeneration - totalConsumption;
            double profit = profitKWh * ratePerKWh;
            cout << "Surplus Energy: " << profitKWh << " kWh\n";
            cout << "Profit Earned: Rs. " << profit << "\n";

            double desiredProfit;
            cout << "Enter your desired monthly profit in PKR: ";
            cin >> desiredProfit;

            double requiredKWh = desiredProfit / ratePerKWh;
            double additionalKWh = requiredKWh - profitKWh;

            if (additionalKWh <= 0) {
                cout << "You are already exceeding your profit goal.\n";
            } else {
                cout << "To reach your target, generate " << additionalKWh << " more kWh/month.\n";
            }

        } else {
            double netBillable = totalConsumption - SolarGeneration;
            cout << "Net Billable Units: " << netBillable << " kWh\n";
            cout << "Estimated Bill: Rs. " << (netBillable * ratePerKWh) << "\n";
        }

        suggestOptimization();
    }

    void suggestOptimization() const {
        cout << "\n--- Optimization Tips ---\n";
        if (!hasSolarPanel)
            cout << "Consider installing solar panels to reduce bills.\n";
        else if (SolarGeneration < totalConsumption)
            cout << "Try increasing solar panel capacity or reducing usage.\n";
        else
            cout << "Excellent usage! You're energy positive.\n";
    }

    double getAdjustedUnits() {
        return max(0.0, getNetUnits());
    }
};

// ===== Residential Class =====
class Residential {
private:
    SolarUser solarUser;
    vector<Appliance> appliances;

public:
    void run() {
        cout << "\n--- Residential User ---\n";
        solarUser.userDetails();
        solarUser.displaySolarSummary();

        cout << "\n--- Appliance Entry ---\n";
        int count;
        cout << "How many types of appliances do you want to enter? ";
        cin >> count;
        cin.ignore();

        for (int i = 0; i < count; ++i) {
            string name;
            int qty, hours, watts;

            cout << "Appliance #" << i + 1 << " name: ";
            getline(cin, name);
            cout << "Quantity of " << name << ": ";
            cin >> qty;
            cout << "Hours per day used: ";
            cin >> hours;
            cout << "Power rating (Watts): ";
            cin >> watts;
            cin.ignore();
            appliances.push_back(Appliance(name, qty, hours, watts));
        }

        double totalUnits = 0;
        for (int i = 0; i < (int)appliances.size(); ++i) {
            totalUnits += appliances[i].monthlyUnits();
        }

        cout << "\n--- Usage Summary ---\n";
        cout << "Total Units Used by Appliances: " << totalUnits << " kWh\n";

        double adjustedUnits = solarUser.getAdjustedUnits();
        double bill = BillCalculator::calculateBill(adjustedUnits, 1);
        cout << "Estimated Bill after Solar Adjustment: Rs. " << bill << "\n";

        double target;
        cout << "\nHow many units would you like to reduce monthly? ";
        cin >> target;
        SuggestionEngine::suggestReduction(appliances, totalUnits, target);
    }
};

// ===== Commercial Class =====
class Commercial {
private:
    SolarUser solarUser;
    vector<Appliance> appliances;

public:
    void run() {
        cout << "\n--- Commercial User ---\n";
        solarUser.userDetails();
        solarUser.displaySolarSummary();

        cout << "\n--- Appliance Entry ---\n";
        int count;
        cout << "How many types of appliances do you want to enter? ";
        cin >> count;
        cin.ignore();

        for (int i = 0; i < count; ++i) {
            string name;
            int qty, hours, watts;

            cout << "Appliance #" << i + 1 << " name: ";
            getline(cin, name);
            cout << "Quantity of " << name << ": ";
            cin >> qty;
            cout << "Hours per day used: ";
            cin >> hours;
            cout << "Power rating (Watts): ";
            cin >> watts;
            cin.ignore();

            appliances.push_back(Appliance(name, qty, hours, watts));
        }

        double totalUnits = 0;
        for (int i = 0; i < (int)appliances.size(); ++i) {
            totalUnits += appliances[i].monthlyUnits();
        }

        cout << "\n--- Usage Summary ---\n";
        cout << "Total Units Used by Appliances: " << totalUnits << " kWh\n";

        double adjustedUnits = solarUser.getAdjustedUnits();
        double bill = BillCalculator::calculateBill(adjustedUnits, 2);
        cout << "Estimated Bill after Solar Adjustment: Rs. " << bill << "\n";

        double target;
        cout << "\nHow many units would you like to reduce monthly? ";
        cin >> target;
        SuggestionEngine::suggestReduction(appliances, totalUnits, target);
    }
};

// ===== Main Function =====
int main() {
    cout << fixed << setprecision(2);
    string again;

    do {
        cout << "\n--- Electricity Bill Management System ---\n";
        cout << "1. Exit\n";
        cout << "2. Residential User\n";
        cout << "3. Commercial User\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Exiting... Goodbye!\n";
            break;
        }

        switch (choice) {
            case 2: {
                Residential r;
                r.run();
                break;
            }
            case 3: {
                Commercial c;
                c.run();
                break;
            }
            default:
                cout << "Invalid option!\n";
        }

        cout << "\nWould you like to run again? (yes / no): ";
        cin >> again;
        for (int i = 0; i < (int)again.length(); ++i) {
            again[i] = tolower(again[i]);
        }
    } while (again == "yes");

    cout << "Thanks for using the program!\n";
    return 0;
}

