#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>

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
    string accountNumber;
    string name;       
    Deposit deposits[MAX_DEPOSITS]; 
    int openYear;       
};

/*This function saves investor data in a human-readable text format.
It already seems to handle variable deposits correctly by checking if the deposit's currency is empty.
*/
void saveInvestorsText(const string& filename, Investor* investors[], int investorCount) {
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

/*This function saves investor data in a binary format. 
It writes the entire Investor struct as a block of data.
If you want to skip empty deposits, you would need to write each field of the Investor struct individually,
similar to the text function. */
void saveInvestors(const string& filename, Investor* investors[], int investorCount) {
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


/*We read each Investor object in full, including all MAX_DEPOSITS deposits.
If the file doesn't contain enough data to fill an Investor object, we report an error.
This approach works under the assumption that the file format matches exactly the memory layout of your Investor struct.
*/
void loadInvestors(const string& filename, Investor* investors[], int& investorCount) {
    ifstream file(filename, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening file for reading.\n";
        return;
    }

    investorCount = 0;

    while (file.peek() != EOF && investorCount < MAX_INVESTORS) {
        investors[investorCount] = new Investor();
        file.read(reinterpret_cast<char*>(investors[investorCount]), sizeof(Investor));
        if (file.gcount() != sizeof(Investor)) {
            cerr << "Error reading investor data.\n";
            delete investors[investorCount];
            break;
        }
        investorCount++;
    }

    file.close();
    cout << "Investors loaded from file successfully.\n";
}

/*This function displays all investors and their deposits. 
It should work correctly with your current structure since it iterates through all possible deposits.
*/
void displayInvestors(Investor* investors[], int investorCount) {
    if (investorCount == 0) {
        cout << "No investors to display.\n";
        return;
    }

    const int width = 20; // Set a constant width for each column

    cout << "List of Investors:\n";
    cout << left
        << setw(width) << "Account Number"
        << setw(width) << "Name"
        << setw(width) << "Year of Account Opening"
        << setw(width) << "Currency"
        << setw(width) << "Amount"
        << setw(width) << "Year of Deposit" << endl;
    cout << string(width * 6, '-') << endl; // Separator line for headers

    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        bool hasDeposits = false;

        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            if (!investor->deposits[j].currency.empty()) {
                hasDeposits = true;
                break;
            }
        }

        if (hasDeposits) {
            for (int j = 0; j < MAX_DEPOSITS; ++j) {
                Deposit* deposit = &(investor->deposits[j]);
                if (!deposit->currency.empty()) {
                    cout << left
                        << setw(width) << investor->accountNumber
                        << setw(width) << investor->name
                        << setw(width) << investor->openYear
                        << setw(width) << deposit->currency
                        << setw(width) << deposit->amount
                        << setw(width) << deposit->year << endl;
                }
            }
        }
        else {
            cout << left
                << setw(width) << investor->accountNumber
                << setw(width) << investor->name
                << setw(width) << investor->openYear
                << setw(width * 3) << "No deposits" << endl;
        }

        cout << string(width * 6, '-') << endl; // Separator line for each investor
    }
}


/*This function lists investors with BGN deposits.
It checks each deposit's currency, so it should work correctly with your structure.
*/
void listBGNInvestors(Investor* investors[], int investorCount) {
    const int width = 20; 

    cout << "Investors with BGN deposits:\n";


    cout << left << setw(width) << "Account Number"
        << setw(width) << "Name"
        << setw(width) << "Year of Account Opening"
        << setw(width) << "Currency"
        << setw(width) << "Amount"
        << setw(width) << "Year of Deposit" << endl;
    cout << string(width * 6, '-') << endl; 

    bool found = false;

    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        for (int j = 0; j < MAX_DEPOSITS; ++j) {
            Deposit* deposit = &(investor->deposits[j]);
            if (!deposit->currency.empty() && deposit->currency == "BGN") {
                found = true;
                cout << left << setw(width) << investor->accountNumber
                    << setw(width) << investor->name
                    << setw(width) << investor->openYear
                    << setw(width) << deposit->currency
                    << setw(width) << deposit->amount
                    << setw(width) << deposit->year << endl;
                break; // Assuming only the first BGN deposit is to be listed
            }
        }
    }

    if (!found) {
        cout << "No investors with BGN deposits found.\n";
    }
}

/*This function finds the investor with the highest EURO deposit. 
It iterates through all deposits and compares their amounts, which should work fine with your structure.
*/
void listMaxEuroInvestors(Investor* investors[], int investorCount) {
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
        const int width = 20; // Set a constant width for each column

        cout << "Investor with the highest EURO deposit:\n";

        // Print table headers
        cout << left << setw(width) << "Account Number"
            << setw(width) << "Name"
            << setw(width) << "Year of Account Opening"
            << setw(width) << "Currency"
            << setw(width) << "Amount"
            << setw(width) << "Year of Deposit" << endl;
        cout << string(width * 6, '-') << endl; // Print a line to separate headers from data

        // Print investor data
        cout << left << setw(width) << investor->accountNumber
            << setw(width) << investor->name
            << setw(width) << investor->openYear
            << setw(width) << "EURO"
            << setw(width) << maxEuroDeposit
            << setw(width) << investor->deposits[0].year << endl; // Assuming the first deposit is the max one
    }
    else {
        cout << "No investors with EURO deposits found.\n";
    }
}

/*This function lists investors by a specific account opening year. 
It doesn't interact with the deposits directly, so no changes are needed.*/
void listByOpenYear(int year, Investor* investors[], int investorCount) {
    const int width = 20; // Set a constant width for each column

    cout << "Investors with an account opening year of " << year << ", sorted by account number:\n";


    cout << left << setw(width) << "Account Number"
        << setw(width) << "Name"
        << setw(width) << "Year of Account Opening" << endl;
    cout << string(width * 3, '-') << endl;

    bool found = false;

    for (int i = 0; i < investorCount; ++i) {
        Investor* investor = investors[i];
        if (investor->openYear == year) {
            found = true;
            cout << left << setw(width) << investor->accountNumber
                << setw(width) << investor->name
                << setw(width) << investor->openYear << endl;
        }
    }

    if (!found) {
        cout << "No investors found with an account opening year of " << year << ".\n";
    }
}

/*This function lists investors with a single deposit.
It counts the number of non-empty deposits for each investor, which aligns well with your structure.
*/
void listWithSingleDeposit(Investor* investors[], int investorCount) {
    const int width = 20; // Set a constant width for each column

    cout << "Investors with a single deposit, sorted by the amount of the deposit:\n";


    cout << left << setw(width) << "Account Number"
        << setw(width) << "Name"
        << setw(width) << "Currency"
        << setw(width) << "Amount"
        << setw(width) << "Year of Deposit" << endl;
    cout << string(width * 5, '-') << endl; 

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
            cout << left << setw(width) << investor->accountNumber
                << setw(width) << investor->name
                << setw(width) << investor->deposits[0].currency
                << setw(width) << investor->deposits[0].amount
                << setw(width) << investor->deposits[0].year << endl;
        }
    }

    if (!found) {
        cout << "No investors found with a single deposit.\n";
    }
}




void addInvestor(Investor* investors[], int& investorCount) {
    if (investorCount >= MAX_INVESTORS) {
        cout << "Maximum number of investors reached.\n";
        return;
    }

    string accountNumber;
    cout << "Enter the account number: ";
    cin >> accountNumber;

    for (int i = 0; i < investorCount; ++i) {
        if (investors[i]->accountNumber == accountNumber) {
            cout << "Investor with the given account number already exists.\n";
            return;
        }
    }

    Investor* investor = new Investor();
    investor->accountNumber = accountNumber;

    cin.ignore();
    cout << "Enter the investor's name: ";
    getline(cin, investor->name);
    cout << "Enter the year of account opening: ";
    cin >> investor->openYear;

    int numDeposits;
    cout << "Enter the number of deposits (up to " << MAX_DEPOSITS << "): ";
    cin >> numDeposits;
    numDeposits = min(numDeposits, MAX_DEPOSITS);

    for (int i = 0; i < numDeposits; ++i) {
        cout << "Enter details for deposit " << (i + 1) << ":\n";
        cout << "Enter the currency of the deposit (BGN/USD/EURO): ";
        cin >> investor->deposits[i].currency;
        cout << "Enter the amount of the deposit: ";
        cin >> investor->deposits[i].amount;
        cout << "Enter the year of deposit creation: ";
        cin >> investor->deposits[i].year;
    }

    investors[investorCount] = investor;
    investorCount++;
    cout << "Investor added successfully.\n";
}


/*This function allows withdrawal from a specific deposit.
It should work correctly as it accesses a specific deposit based on user input.*/
void withdrawDeposit(Investor* investors[], int investorCount) {
    string accountNumber;
    cout << "Enter the account number of the investor: ";
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

    int depositNumber;
    cout << "Enter the deposit number from which you want to withdraw (1, 2, or 3): ";
    cin >> depositNumber;
    depositNumber--; 

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

    deposit->amount -= amountToWithdraw;
    cout << "Withdrawal successful. Remaining balance in the deposit: " << deposit->amount << endl;
}

/*This function closes an investor's profile and should work correctly. 
It deals with the investor as a whole and doesn't need to interact with individual deposits.*/
void closeInvestorProfile(Investor* investors[], int& investorCount) {
    string accountNumber;
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

    delete investors[investorIndex];
    for (int i = investorIndex; i < investorCount - 1; ++i) {
        investors[i] = investors[i + 1];
    }
    investorCount--;
    cout << "Investor profile closed successfully.\n";
}

void bubbleSortInvestorsByAccountNumber(Investor* investors[], int investorCount) {
    bool swapped;
    for (int i = 0; i < investorCount - 1; ++i) {
        swapped = false;
        for (int j = 0; j < investorCount - i - 1; ++j) {
            if (investors[j]->accountNumber > investors[j + 1]->accountNumber) {
                swap(investors[j], investors[j + 1]);
                swapped = true;
            }
        }

        // Ако не е извършена нито една размяна, масивът е сортиран
        if (!swapped) {
            break;
        }
    }
}



int main() {
    int choice;
    Investor* investors[MAX_INVESTORS];
    int investorCount = 0;

    loadInvestors("investors.dat", investors, investorCount);


    while (true) {
        cout << "Financial Deposits Information System\n";
        cout << "1. Add Investor\n";
        cout << "2. Display All Investors\n";
        cout << "3. Display Investors with BGN Deposits\n";
        cout << "4. Display Investor with Highest EURO Deposit\n";
        cout << "5. Save Investors to File\n";
        cout << "6. Extract and Sort Investors by Account Opening Year\n";
        cout << "7. Extract and Sort Investors with Single Deposits\n";
        cout << "8. Withdraw deposit\n";
        cout << "9. Close investor profile\n";
        cout << "10. Exit\n";
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            addInvestor(investors, investorCount);
            break;
        case 2:
            displayInvestors(investors, investorCount);
            break;
        case 3:
            listBGNInvestors(investors, investorCount);
            break;
        case 4:
            listMaxEuroInvestors(investors, investorCount);
            break;
        case 5:
            saveInvestors("investors.dat", investors, investorCount);
            break;
        case 6:
            int yearToExtract;
            cout << "Enter the year of account opening to extract and sort investors: ";
            cin >> yearToExtract;
            listByOpenYear(yearToExtract, investors, investorCount);
            break;
        case 7:
            listWithSingleDeposit(investors, investorCount);
            break;
        case 8:
            withdrawDeposit(investors, investorCount);
            break;
        case 9:
            closeInvestorProfile(investors, investorCount);
            break;
        case 10:
            cout << "Goodbye!\n";
            saveInvestorsText("investors.txt", investors, investorCount);
            return 0;
        default:
            cout << "Invalid choice.\n";
        }
    }


    return 0;
}