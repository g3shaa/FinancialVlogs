#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

const int MAX_INVESTORS = 50;
const int MAX_DEPOSITS = 3;
const int MAX_STRING_LENGTH = 100;

// Struct for representing a deposit.
struct Deposit {
    string currency;  // Currency of the deposit (e.g., BGN, USD, EURO)
    double amount;    // Amount of the deposit
    int year;         // Year of deposit creation
};

// Struct for representing an investor.
struct Investor {
    int accountNumber;   // Investor's account number
    string name;         // Investor's name
    Deposit deposits[MAX_DEPOSITS]; // Array of deposits (up to 3)
    int openYear;        // Year of account opening
};

Investor* investors[MAX_INVESTORS]; // Array of pointers to investors
int investorCount = 0; // Variable to track the current number of investors

// Функция за запис на данни в текстов файл
void saveInvestorsText(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening text file for writing.\n";
        return;
    }

    for (int i = 0; i < investorCount; ++i) {
        file << "Account Number: " << investors[i]->accountNumber << endl;
        file << "Name: " << investors[i]->name << endl;
        file << "Year of Account Opening: " << investors[i]->openYear << endl;
        file << "Deposits:\n";
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investors[i]->deposits[j]);
            if (!deposit->currency.empty()) {
                file << "  Currency: " << deposit->currency << endl;
                file << "  Amount: " << deposit->amount << endl;
                file << "  Year of Deposit Creation: " << deposit->year << endl;
            }
        }
        file << "----------------------------\n";
    }

    file.close();
    cout << "Investors saved to text file successfully.\n";
}

void saveInvestors(const string& filename) {
    ofstream file(filename, ios::out | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    for (int i = 0; i < investorCount; ++i) {
        file.write(reinterpret_cast<const char*>(investors[i]), sizeof(Investor));
    }

    file.close();
    cout << "Investors saved to file successfully.\n";
}

void loadInvestors(const string& filename) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for reading.\n";
        return;
    }

    investorCount = 0;

    while (file.peek() != EOF) {
        investors[investorCount] = new Investor();
        file.read(reinterpret_cast<char*>(investors[investorCount]), sizeof(Investor));
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
        Investor* investor = investors[i];
        cout << "*Account Number: " << investor->accountNumber << endl;
        cout << "*Name: " << investor->name << endl;
        cout << "*Year of Account Opening: " << investor->openYear << endl;
        cout << "*Deposits:\n";
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investor->deposits[j]);
            if (!deposit->currency.empty()) {
                cout << "  *Currency: " << deposit->currency << endl;
                cout << "  *Amount: " << deposit->amount << endl;
                cout << "  *Year of Deposit Creation: " << deposit->year << endl;
            }
        }
        cout << "----------------------------\n";
    }
}

void listBGNInvestors() {
    cout << "Investors with BGN deposits:\n";
    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investor->deposits[j]);
            if (!deposit->currency.empty() && deposit->currency == "BGN") {
                cout << "*Account Number: " << investor->accountNumber << endl;
                cout << "*Name: " << investor->name << endl;
                cout << "*Year of Account Opening: " << investor->openYear << endl;
                cout << "*Deposit:\n";
                cout << "  *Currency: " << deposit->currency << endl;
                cout << "  *Amount: " << deposit->amount << endl;
                cout << "  *Year of Deposit Creation: " << deposit->year << endl;
                cout << "----------------------------\n";
                break;
            }
        }
    }
}

void listMaxEuroInvestors() {
    double maxEuroDeposit = 0.0;
    int maxEuroInvestorIndex = -1;

    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investor->deposits[j]);
            if (!deposit->currency.empty() && deposit->currency == "EURO" && deposit->amount > maxEuroDeposit) {
                maxEuroDeposit = deposit->amount;
                maxEuroInvestorIndex = i;
            }
        }
    }

    if (maxEuroInvestorIndex != -1) {
        Investor* investor = investors[maxEuroInvestorIndex];
        cout << "Investor with the highest EURO deposit:\n";
        cout << "*Account Number: " << investor->accountNumber << endl;
        cout << "*Name: " << investor->name << endl;
        cout << "*Year of Account Opening: " << investor->openYear << endl;
        cout << "*Deposit:\n";
        cout << "  *Currency: EURO\n";
        cout << "  *Amount: " << maxEuroDeposit << endl;
        cout << "  *Year of Deposit Creation: ";
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investor->deposits[j]);
            if (!deposit->currency.empty() && deposit->currency == "EURO" && deposit->amount == maxEuroDeposit) {
                cout << deposit->year << endl;
                break;
            }
        }
    }
    else {
        cout << "No investors with EURO deposits found.\n";
    }
}

void listByOpenYear(int year) {
    cout << "Investors with an account opening year of " << year << ", sorted by account number:\n";
    bool found = false;

    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        if (investor->openYear == year) {
            found = true;
            cout << "*Account Number: " << investor->accountNumber << endl;
            cout << "*Name: " << investor->name << endl;
            cout << "*Year of Account Opening: " << investor->openYear << endl;
            cout << "----------------------------\n";
        }
    }

    if (!found) {
        cout << "No investors found with an account opening year of " << year << ".\n";
    }
}

void listWithSingleDeposit() {
    cout << "Investors with a single deposit, sorted by the amount of the deposit:\n";
    bool found = false;

    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        int numDeposits = 0;

        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investor->deposits[j]);
            if (!deposit->currency.empty()) {
                numDeposits++;
            }
        }

        if (numDeposits == 1) {
            found = true;
            cout << "*Account Number: " << investor->accountNumber << endl;
            cout << "*Name: " << investor->name << endl;
            cout << "*Deposit Currency: " << investor->deposits[0].currency << endl;
            cout << "*Deposit Amount: " << investor->deposits[0].amount << endl;
            cout << "*Year of Deposit Creation: " << investor->deposits[0].year << endl;
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

    int accountNumber;
    cout << "Enter the account number: ";
    cin >> accountNumber;

    // Проверка дали вложителят с този номер вече съществува
    bool existingInvestor = false;
    for (int i = 0; i < investorCount; ++i) {
        if (investors[i]->accountNumber == accountNumber) {
            existingInvestor = true;
            cout << "Investor with the given account number already exists.\n";
            break;
        }
    }

    if (existingInvestor) {
        return;
    }

    Investor* investor = new Investor();
    investor->accountNumber = accountNumber;

    cin.ignore();
    cout << "Enter the investor's name: ";
    getline(cin, investor->name);
    cout << "Enter the year of account opening: ";
    cin >> investor->openYear;

    // Check the number of existing deposits for this investor.
    int existingDeposits = 0;
    for (int i = 0; i < MAX_DEPOSITS; ++i) {
        if (!investor->deposits[i].currency.empty()) {
            existingDeposits++;
        }
    }

    // Проверка дали вложителят има 3 влога
    if (existingDeposits >= 3) {
        cout << "Investor already has 3 deposits, cannot add another deposit.\n";
        delete investor;
        return;
    }

    cout << "Enter the currency of the new deposit (BGN/USD/EURO): ";
    cin >> investor->deposits[existingDeposits].currency;
    cout << "Enter the amount of the new deposit: ";
    cin >> investor->deposits[existingDeposits].amount;
    cout << "Enter the year of deposit creation: ";
    cin >> investor->deposits[existingDeposits].year;

    investors[investorCount] = investor;
    investorCount++;
    cout << "Investor added successfully.\n";
}


// Функция за изтегляне на влог от определен вложител и влог
void withdrawDeposit() {
    int accountNumber;
    cout << "Enter the account number of the investor: ";
    cin >> accountNumber;

    // Търсене на вложителя по номер
    int investorIndex = -1;
    for (int i = 0; i < investorCount; ++i) {
        if (investors[i]->accountNumber == accountNumber) {
            investorIndex = i;
            break;
        }
    }

    if (investorIndex == -1) {
        cout << "Investor with the given account number does not exist.\n";
        return;
    }

    int depositNumber;
    cout << "Enter the deposit number from which you want to withdraw (1, 2, or 3): ";
    cin >> depositNumber;
    depositNumber--; // Тъй като индексите са 0-базирани

    if (depositNumber < 0 || depositNumber >= MAX_DEPOSITS) {
        cout << "Invalid deposit number.\n";
        return;
    }

    double amountToWithdraw;
    cout << "Enter the amount to withdraw from the deposit: ";
    cin >> amountToWithdraw;

    Investor* investor = investors[investorIndex];
    Deposit* deposit = &(investor->deposits[depositNumber]);

    if (deposit->currency.empty() || deposit->amount < amountToWithdraw) {
        cout << "Invalid withdrawal. The deposit does not exist or the amount is insufficient.\n";
        return;
    }

    // Изтегляне на сумата от влога
    deposit->amount -= amountToWithdraw;
    cout << "Withdrawal successful. Remaining balance in the deposit: " << deposit->amount << endl;
}

// Функция за затваряне на профил на вложител
void closeInvestorProfile() {
    int accountNumber;
    cout << "Enter the account number of the investor to close the profile: ";
    cin >> accountNumber;

    int investorIndex = -1;
    for (int i = 0; i < investorCount; ++i) {
        if (investors[i]->accountNumber == accountNumber) {
            investorIndex = i;
            break;
        }
    }

    if (investorIndex == -1) {
        cout << "Investor with the given account number does not exist.\n";
        return;
    }

    // Изтриване на вложителя и освобождаване на паметта
    delete investors[investorIndex];
    for (int i = investorIndex; i < investorCount - 1; ++i) {
        investors[i] = investors[i + 1];
    }
    investorCount--;
    cout << "Investor profile closed successfully.\n";
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
        cout << "9. Withdraw deposit\n";
        cout << "10. Close investor profile\n";
        cout << "10. Exit\n";
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
            listBGNInvestors();
            break;
        case 4:
            listMaxEuroInvestors();
            break;
        case 5:
            saveInvestors("investors.dat");
            break;
        case 6:
            loadInvestors("investors.dat");
            break;
        case 7:
            int yearToExtract;
            cout << "Enter the year of account opening to extract and sort investors: ";
            cin >> yearToExtract;
            listByOpenYear(yearToExtract);
            break;
        case 8:
            listWithSingleDeposit();
            break;
        case 9:
            withdrawDeposit();
            break;
        case 10:
            closeInvestorProfile();
            break;
        case 11:
            cout << "Goodbye!\n";
            saveInvestorsText("investors.txt");
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}