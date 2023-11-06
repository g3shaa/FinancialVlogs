#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

const int MAX_INVESTORS = 50;
const int MAX_DEPOSITS = 3;
const int MAX_STRING_LENGTH = 100;

struct Deposit {
    string currency;
    double amount;
    int year;
};

struct Investor {
    int accountNumber;
    string name;
    Deposit deposits[MAX_DEPOSITS];
    int openYear;
};

Investor investors[MAX_INVESTORS];
int investorCount = 0;

void saveInvestorsToFile(const string& filename) {
    ofstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    file.write(reinterpret_cast<const char*>(investors), sizeof(Investor) * investorCount);

    file.close();
    cout << "Investors saved to file successfully.\n";
}

void loadInvestorsFromFile(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for reading.\n";
        return;
    }

    investorCount = 0;

    while (file.read(reinterpret_cast<char*>(&investors[investorCount]), sizeof(Investor))) {
        investorCount++;
    }

    file.close();
    cout << "Investors loaded from file successfully.\n";
}

void displayInvestors() {
    if (investorCount == 0) {
        cout << "No investors to display.\n";
        return;
    }

    cout << "List of Investors:\n";
    for (int i = 0; i < investorCount; ++i) {
        cout << "Account Number: " << investors[i].accountNumber << endl;
        cout << "Name: " << investors[i].name << endl;
        cout << "Year of Account Opening: " << investors[i].openYear << endl;
        cout << "Deposits:\n";
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            if (!investors[i].deposits[j].currency.empty()) {
                cout << "  Currency: " << investors[i].deposits[j].currency << endl;
                cout << "  Amount: " << investors[i].deposits[j].amount << endl;
                cout << "  Year of Deposit Creation: " << investors[i].deposits[j].year << endl;
            }
        }
        cout << "----------------------------\n";
    }
}

void displayInvestorsWithBGN() {
    cout << "Investors with BGN deposits:\n";
    for (int i = 0; i < investorCount; ++i) {
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            if (!investors[i].deposits[j].currency.empty() && investors[i].deposits[j].currency == "BGN") {
                cout << "Account Number: " << investors[i].accountNumber << endl;
                cout << "Name: " << investors[i].name << endl;
                cout << "Year of Account Opening: " << investors[i].openYear << endl;
                cout << "Deposit:\n";
                cout << "  Currency: " << investors[i].deposits[j].currency << endl;
                cout << "  Amount: " << investors[i].deposits[j].amount << endl;
                cout << "  Year of Deposit Creation: " << investors[i].deposits[j].year << endl;
                cout << "----------------------------\n";
                break;
            }
        }
    }
}

void displayInvestorsWithMaxEuroDeposit() {
    double maxEuroDeposit = 0.0;
    int maxEuroInvestorIndex = -1;

    for (int i = 0; i < investorCount; ++i) {
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            if (!investors[i].deposits[j].currency.empty() && investors[i].deposits[j].currency == "EURO" && investors[i].deposits[j].amount > maxEuroDeposit) {
                maxEuroDeposit = investors[i].deposits[j].amount;
                maxEuroInvestorIndex = i;
            }
        }
    }

    if (maxEuroInvestorIndex != -1) {
        cout << "Investor with the highest EURO deposit:\n";
        cout << "Account Number: " << investors[maxEuroInvestorIndex].accountNumber << endl;
        cout << "Name: " << investors[maxEuroInvestorIndex].name << endl;
        cout << "Year of Account Opening: " << investors[maxEuroInvestorIndex].openYear << endl;
        cout << "Deposit:\n";
        cout << "  Currency: EURO\n";
        cout << "  Amount: " << maxEuroDeposit << endl;
        cout << "  Year of Deposit Creation: ";
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            if (!investors[maxEuroInvestorIndex].deposits[j].currency.empty() && investors[maxEuroInvestorIndex].deposits[j].currency == "EURO" && investors[maxEuroInvestorIndex].deposits[j].amount == maxEuroDeposit) {
                cout << investors[maxEuroInvestorIndex].deposits[j].year << endl;
                break;
            }
        }
    }
    else {
        cout << "No investors with EURO deposits found.\n";
    }
}

void extractAndSortInvestorsByOpenYear(int year) {
    cout << "Investors with an account opening year of " << year << ", sorted by account number:\n";
    bool found = false;

    for (int i = 0; i < investorCount; ++i) {
        if (investors[i].openYear == year) {
            found = true;
            cout << "Account Number: " << investors[i].accountNumber << endl;
            cout << "Name: " << investors[i].name << endl;
            cout << "Year of Account Opening: " << investors[i].openYear << endl;
            cout << "----------------------------\n";
        }
    }

    if (!found) {
        cout << "No investors found with an account opening year of " << year << ".\n";
    }
}

void extractAndSortInvestorsWithSingleDeposit() {
    cout << "Investors with a single deposit, sorted by the amount of the deposit:\n";
    bool found = false;

    for (int i = 0; i < investorCount; ++i) {
        int numDeposits = 0;

        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            if (!investors[i].deposits[j].currency.empty()) {
                numDeposits++;
            }
        }

        if (numDeposits == 1) {
            found = true;
            cout << "Account Number: " << investors[i].accountNumber << endl;
            cout << "Name: " << investors[i].name << endl;
            cout << "Deposit Currency: " << investors[i].deposits[0].currency << endl;
            cout << "Deposit Amount: " << investors[i].deposits[0].amount << endl;
            cout << "Year of Deposit Creation: " << investors[i].deposits[0].year << endl;
            cout << "----------------------------\n";
        }
    }

    if (!found) {
        cout << "No investors found with a single deposit.\n";
    }
}

void addInvestor() {
    if (investorCount >= MAX_INVESTORS) {
        cout << "Maximum number of investors reached.\n";
        return;
    }

    Investor investor;
    cout << "Enter the account number: ";
    cin >> investor.accountNumber;
    cin.ignore(); // Clear the newline character after the number.
    cout << "Enter the investor's name: ";
    getline(cin, investor.name);
    cout << "Enter the year of account opening: ";
    cin >> investor.openYear;

    for (int i = 0; i < MAX_DEPOSITS; ++i) {
        cout << "Enter the currency of the deposit (BGN/USD/EURO) for deposit " << i + 1 << ": ";
        cin >> investor.deposits[i].currency;
        if (investor.deposits[i].currency.empty()) {
            break;
        }
        cout << "Enter the amount of the deposit for deposit " << i + 1 << ": ";
        cin >> investor.deposits[i].amount;
        cout << "Enter the year of deposit creation for deposit " << i + 1 << ": ";
        cin >> investor.deposits[i].year;
    }

    investors[investorCount] = investor;
    investorCount++;
    cout << "Investor added successfully.\n";
}

int main() {
    int choice;

    while (true) {
        cout << "Financial Deposits Information System\n";
        cout << "1. Add Investor\n";
        cout << "2. Display All Investors\n";
        cout << "3. Display Investors with BGN Deposits\n";
        cout << "4. Display Investor with Highest EURO Deposit\n";
        cout << "5. Save Investors to File\n";
        cout << "6. Load Investors from File\n";
        cout << "7. Extract and Sort Investors by Account Opening Year\n";
        cout << "8. Extract and Sort Investors with Single Deposits\n";
        cout << "9. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addInvestor();
            break;
        case 2:
            displayInvestors();
            break;
        case 3:
            displayInvestorsWithBGN();
            break;
        case 4:
            displayInvestorsWithMaxEuroDeposit();
            break;
        case 5:
            saveInvestorsToFile("investors.bin");
            break;
        case 6:
            loadInvestorsFromFile("investors.bin");
            break;
        case 7:
            int yearToExtract;
            cout << "Enter the year of account opening to extract and sort investors: ";
            cin >> yearToExtract;
            extractAndSortInvestorsByOpenYear(yearToExtract);
            break;
        case 8:
            extractAndSortInvestorsWithSingleDeposit();
            break;
        case 9:
            cout << "Goodbye!\n";
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}