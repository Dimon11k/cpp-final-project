#include "payroll.h"
#include "fileManager.h"
#include "userInterface.h"
#include <sstream>
#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

string PayrollRecord::toString() const {
	stringstream ss;
	ss << username << "|"
		<< date << "|"
		<< amount << "|"
		<< description;
	return ss.str();
}

PayrollRecord PayrollRecord::fromString(const string& str) {
	stringstream ss(str);
	PayrollRecord record;
	string temp;

	getline(ss, record.username, '|');
	getline(ss, record.date, '|');
	getline(ss, temp, '|');
	record.amount = stod(temp);
	getline(ss, record.description);

	return record;
}

void PayrollManager::processSalaryPayment() {
	UserInterface::showTitle("Process Salary Payment");

	// Отримуємо список користувачів
	vector<User*> users = FileManager::getAllUsers();

	if (users.empty()) {
		UserInterface::showMessage("No users found in the system.");
		return;
	}

	// Показуємо список користувачів
	UserInterface::displayAllUsers();

	// Запитуємо аро ім'я користувача
	string username;
	cout << "Enter username to pay (or 'back' to return): ";
	getline(cin, username);

	if (username == "back") {
		return;
	}

	// Знаходимо користувача
	User* targetUser = nullptr;
	for (User* user : users) {
		if (user->getUsername() == username) {
			targetUser = user;
			break;
		}
	}

	if (!targetUser) {
		UserInterface::showMessage("User not found.");
		// Очищаємо
		for (User* user : users) {
			delete user;
		}
		return;
	}

	// Показуємо деталі
	cout << "\n==== User Details ====\n";
	cout << "Username: " << targetUser->getUsername() << "\n";
	cout << "Full Name: " << targetUser->getFullName() << "\n";
	cout << "Position: " << targetUser->getPosition() << "\n";
	cout << "Base Salary: $" << fixed << setprecision(2) << targetUser->getBaseSalary() << "\n\n";

	// Отримуємо кількість оплати, відповідно до звичайної зарплати
	double amount = targetUser->getBaseSalary();
	string amountStr;
	cout << "Enter payment amount [$" << amount << "]: ";
	getline(cin, amountStr);

	if (!amountStr.empty()) {
		try {
			amount = stod(amountStr);
		}
		catch (...) {
			UserInterface::showMessage("Invalid amount. Using base salary.");
		}
	}

	// Отримуємо опис
	string description;
	cout << "Enter payment description [Monthly Salary]: ";
	getline(cin, description);

	if (description.empty()) {
		description = "Monthly Salary";
	}

	// Отримуємо теперішню дату
	time_t now = time(0);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	char dateBuffer[20];
	strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &timeinfo);
	string dateStr = dateBuffer;

	// Створюємо і зберігаємо запис платежу
	PayrollRecord record;
	record.username = username;
	record.date = dateStr;
	record.amount = amount;
	record.description = description;

	if (addPayrollRecord(record)) {
		UserInterface::showMessage("Payment processed successfully.");
	}
	else {
		UserInterface::showMessage("Error processing payment.");
	}

	// Очищаємо
	for (User* user : users) {
		delete user;
	}
}

void PayrollManager::viewAllPayrolls() {
	UserInterface::showTitle("All Payroll Records");

	vector<PayrollRecord> records = getAllPayrolls();

	if (records.empty()) {
		UserInterface::showMessage("No payroll records found.");
		return;
	}

	// Показуємо заголовок
	cout << setw(15) << left << "Username"
		<< setw(12) << left << "Date"
		<< setw(10) << right << "($) Amount"
		<< "  Description\n";
	cout << string(100, '-') << "\n";

	// Показуємо записи
	for (const auto& record : records) {
		cout << setw(15) << left << record.username
			<< setw(12) << left << record.date
			<< setw(9) << right << fixed << setprecision(2) << record.amount
			<< "  " << record.description << "\n";
	}

	cout << string(100, '-') << "\n";
	UserInterface::waitForKeypress();
}

void PayrollManager::viewUserPayrolls(const string& username) {
	UserInterface::showTitle("Salary History for " + username);

	vector<PayrollRecord> records = getUserPayrolls(username);

	if (records.empty()) {
		UserInterface::showMessage("No payroll records found for this user.");
		return;
	}

	// Показуємо заголовок
	cout << setw(12) << left << "Date"
		<< setw(10) << right << "Amount"
		<< "  Description\n";
	cout << string(100, '-') << "\n";

	// Показуємо записи і вираховуємо загальну суму
	double total = 0.0;
	for (const auto& record : records) {
		cout << setw(12) << left << record.date
			<< "$" << setw(9) << right << fixed << setprecision(2) << record.amount
			<< "  " << record.description << "\n";
		total += record.amount;
	}

	cout << string(100, '-') << "\n";
	cout << "Total received: $" << fixed << setprecision(2) << total << "\n";
	UserInterface::waitForKeypress();
}

vector<PayrollRecord> PayrollManager::getAllPayrolls() {
	vector<PayrollRecord> records;
	vector<string> lines = FileManager::readFile("payroll.txt");

	for (const auto& line : lines) {
		if (!line.empty()) {
			records.push_back(PayrollRecord::fromString(line));
		}
	}

	return records;
}

vector<PayrollRecord> PayrollManager::getUserPayrolls(const string& username) {
	vector<PayrollRecord> allRecords = getAllPayrolls();
	vector<PayrollRecord> userRecords;

	for (const auto& record : allRecords) {
		if (record.username == username) {
			userRecords.push_back(record);
		}
	}

	return userRecords;
}

bool PayrollManager::addPayrollRecord(const PayrollRecord& record) {
	return FileManager::appendToFile("payroll.txt", record.toString());
}