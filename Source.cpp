#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::to_string;
using std::fixed;
using std::setprecision;

enum action_type {
	create=0,
	close=1,
	income=2,
	transfer=3,
	expense=4
};

struct history_str {
	float amount;
	action_type type;
	string details;
};

struct balances_str {
	string name;
	float balance;
};

vector<balances_str> balances;
vector<history_str> history;

string action(int balance_index, action_type action, float amount = 0, string details = "", int to_index = -1);
bool within_range(vector<balances_str> vec, int index);
string create_balance(string balance_name);
void print_balances(int exception = -1);
string type_enum_to_string(action_type type);

int main() {
	cout << fixed << setprecision(2);
	cout << "      << Wallet >>" << endl;
	while (true) {
		cout << endl << "Balances: ";
	
		for (int i = 0; i < balances.size(); i++)
		{
			cout << balances.at(i).name << ": " << balances.at(i).balance << "$";
			if (i != balances.size() - 1)
				cout << " | ";
		}
		cout << endl;

		cout << "0) Open a new balance." << endl;
		cout << "1) Close a balance." << endl;
		cout << "2) Add an income." << endl;
		cout << "3) Add an expense." << endl;
		cout << "4) Add a transfer." << endl;
		cout << "5) View the history." << endl;
		cout << ": ";
		int choice = -1;
		cin >> choice;
		if (choice == 0) {
			cout << "Type the balance name: ";
			string name = "";
			cin >> name;
			cout << create_balance(name) << endl;
		}
		else if (choice == 1) {
			cout << "Pick a balance to delete:" << endl;

			print_balances();
			
			int picked_balance = -1;
			cin >> picked_balance;
			cout << action(picked_balance, close) << endl;
		}
		else if (choice == 2) {
			cout << "Pick a balance to add income:" << endl;
			print_balances();
			int picked_balance = -1;
			cin >> picked_balance;
			if (!within_range(balances, picked_balance))
				cout << "Balance number does not exist." << endl;
			else {
				cout << "<" << balances.at(picked_balance).name << "> Enter the amount of income: ";
				float amount;
				cin >> amount;
				cout << "Enter details of the income: ";
				string details;
				cin >> details;
				cout << action(picked_balance,income, amount,details) << endl;
			}
		}
		else if (choice == 3) {
			cout << "Pick a balance to add an expense:" << endl;
			print_balances();
			int picked_balance = -1;
			cin >> picked_balance;
			if (!within_range(balances, picked_balance))
				cout << "Balance number does not exist." << endl;
			else {
				cout << "<" << balances.at(picked_balance).name << "> Enter the amount of expense: ";
				float amount;
				cin >> amount;
				cout << "Enter details of the expense: ";
				string details;
				cin >> details;
				cout << action(picked_balance, expense, amount, details) << endl;
			}
		}
		else if (choice == 4) {
			cout << "Pick a balance to transfer from:" << endl;
			print_balances();
			int picked_balance = -1;
			cin >> picked_balance;
			if (!within_range(balances, picked_balance)) {
				cout << "Balance number does not exist." << endl;
				continue;
			}

			cout << "Pick a balance to transfer to:" << endl;
			print_balances(picked_balance);
			int picked_balance2 = -1;
			cin >> picked_balance2;
			if (!within_range(balances, picked_balance)) {
				cout << "Balance number does not exist." << endl;
				continue;
			}
			if (picked_balance2 == picked_balance) {
				cout << "You can't transfer from " + balances.at(picked_balance).name + " to " + balances.at(picked_balance2).name << endl;
				continue;
			}
				cout << balances.at(picked_balance).name + ", Enter the amount to transfer: ";
				float amount;
				cin >> amount;
				cout << "Enter details of the transfer: ";
				string details;
				cin >> details;
				cout << action(picked_balance, transfer, amount, details,picked_balance2) << endl;
		}
		else if (choice == 5) {
			for (int i = 0; i < history.size(); i++)
			{
				cout << "Amount: " << history.at(i).amount << "$ | Type: " << type_enum_to_string(history.at(i).type) << " | Details: " << history.at(i).details << endl;
			}
		}
	}
	return 0;
}

string action(int balance_index, action_type action, float amount,string details, int to_index) {
	if (!within_range(balances, balance_index))
		return "Balance number does not exist.";
 if (action == close) {
	 if (balances.at(balance_index).balance >= 0.01)
		 return "Can't close a balance that is not 0.";
	 string balance = balances.at(balance_index).name;
	 balances.erase(balances.begin() + balance_index);
	 return "The balance <" + balance + "> has been closed.";
	}
 else if (action == income)
 {
	 balances.at(balance_index).balance += amount;
	 history_str new_record;
	 new_record.amount = amount;
	 new_record.details = "To <"+balances.at(balance_index).name+">. " + details;
	 new_record.type = income;
	 history.push_back(new_record);
	 return "The income has been added to the record.";
 } else if(action == transfer){
	 if (!within_range(balances, to_index))
		 return "Transferred to balance number does not exist.";
	 if (amount > balances.at(balance_index).balance)
		 return "Amount " + to_string(amount) + "$ is bigger than the balance " + to_string(balances.at(balance_index).balance) + "$";
	 balances.at(balance_index).balance -= amount;
	 balances.at(to_index).balance += amount;
	 history_str new_record;
	 new_record.amount = amount;
	 new_record.details = "From <"+ balances.at(balance_index).name +"> To <"+ balances.at(to_index).name+ ">. " + details;
	 new_record.type = transfer;
	 history.push_back(new_record);
	 return "The transfer has been added to the record.";
 }
 else if (action == expense)
 {
	 if (amount > balances.at(balance_index).balance)
		 return "Expense " + to_string(amount) + "$ is bigger than the balance " + to_string(balances.at(balance_index).balance) + "$";
	 balances.at(balance_index).balance -= amount;
	 history_str new_record;
	 new_record.amount = -amount;
	 new_record.details = "From <" + balances.at(balance_index).name + ">. " + details;;
	 new_record.type = expense;
	 history.push_back(new_record);
	 return "The expense has been added to the record.";
 }
 return "Action has failed.";
}

string create_balance(string balance_name) {
	for (int i = 0; i < balances.size(); i++)
	{
		if (balances.at(i).name == balance_name)
			return "The balance <" + balance_name + "> already exists.";
	}
	balances_str new_balance;
	new_balance.name = balance_name;
	new_balance.balance = 0;
	balances.push_back(new_balance);
	return "Balance <" + balance_name + "> has been created.";
}

bool within_range(vector<balances_str> vec, int index){
	if (index < 0 || index >= vec.size())
		return false;
	else return true;
}

void print_balances(int exception) {
	for (int i = 0; i < balances.size(); i++)
	{
		if (i == exception)
			continue;
		cout << i << ") " << balances.at(i).name << ": " << balances.at(i).balance << "$" << endl;
	}
	cout << ": ";
}

string type_enum_to_string(action_type type) {
	if (type == create)
		return "Create";
	else if (type == close)
		return "Close";
	else if (type == income)
		return "Income";
	else if (type == transfer)
		return "Transfer";
	else if (type == expense)
		return "Expense";
	return "NULL";
}