#include "userInterface.h"
#include "fileManager.h"
#include <iostream>
#include <iomanip>
#include <conio.h>

using namespace std;

void UserInterface::clearScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void UserInterface::showTitle(const string& title) {
	clearScreen();
	cout << "=========================================\n";
	cout << "   " << title << "\n";
	cout << "=========================================\n\n";
}

void UserInterface::showMessage(const string& message) {
	cout << "\n" << message << "\n";
	waitForKeypress();
}

void UserInterface::waitForKeypress() {
	cout << "\nPress Enter to continue...";
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void UserInterface::showWelcomeScreen() {
	clearScreen();
	cout << R"(
    ================================================
       ENTERPRISE SALARY PAYMENT SYSTEM
    ================================================
    
    Welcome to the salary payment management system.
    This system allows for tracking and processing 
    employee salary payments.
    
    ------------------------------------------------
    )";
	waitForKeypress();
}

void UserInterface::showExitMessage() {
	clearScreen();
	cout << R"(
    ================================================
       THANK YOU FOR USING THE SYSTEM
    ================================================
    
    All data has been saved.
    Have a great day!
    
    ------------------------------------------------
    )";
}

string UserInterface::showAuthMenu() {
	showTitle("Authentication");

	cout << "1. Login\n";
	cout << "2. Register\n";
	cout << "\nType 'exit' to quit the program\n\n";

	string choice;
	cout << "Enter your choice: ";
	getline(cin, choice);

	return choice;
}

string UserInterface::showAdminMenu() {
	showTitle("Admin Menu");

	cout << "1. View All Users\n";
	cout << "2. Create User\n";
	cout << "3. Delete User\n";
	cout << "4. View All Payrolls\n";
	cout << "5. Process Salary Payment\n";
	cout << "6. Log Out\n";
	cout << "\nType 'exit' to quit the program\n\n";

	string choice;
	cout << "Enter your choice: ";
	getline(cin, choice);

	return choice;
}

string UserInterface::showUserMenu() {
	showTitle("User Menu");

	cout << "1. View Profile\n";
	cout << "2. View Salary History\n";
	cout << "3. Log Out\n";
	cout << "\nType 'exit' to quit the program\n\n";

	string choice;
	cout << "Enter your choice: ";
	getline(cin, choice);

	return choice;
}

void UserInterface::displayAllUsers() {
	showTitle("All Users");

	vector<User*> users = FileManager::getAllUsers();

	if (users.empty()) {
		showMessage("No users found in the system.");
		return;
	}

	// Показуємо заголовок
	cout << setw(15) << left << "Username"
		<< setw(25) << left << "Full Name"
		<< setw(20) << left << "Position"
		<< setw(10) << right << "Salary"
		<< "  Role\n";
	cout << string(80, '-') << "\n";

	// Показуємо користувачів
	for (const auto& user : users) {
		cout << setw(15) << left << user->getUsername()
			<< setw(25) << left << user->getFullName()
			<< setw(20) << left << user->getPosition()
			<< "$" << setw(9) << right << fixed << setprecision(2) << user->getBaseSalary()
			<< "  " << (user->isAdmin() ? "Admin" : "User") << "\n";
	}

	cout << string(80, '-') << "\n";

	// Очищуємо
	for (User* user : users) {
		delete user;
	}

	waitForKeypress();
}

void UserInterface::createUser() {
	showTitle("Create New User");

	string username, password, fullName, position;
	double salary = 0.0;
	bool isAdmin = false;

	// Отримуємо username
	cout << "Enter username: ";
	getline(cin, username);

	if (username.empty()) {
		showMessage("Username cannot be empty.");
		return;
	}

	// Перевіряємо чи користувач існує
	User* existingUser = FileManager::findUserByUsername(username);
	if (existingUser) {
		delete existingUser;
		showMessage("Username already exists.");
		return;
	}

	// Оримуємо пароль
	password = getHiddenInput("Enter password: ");

	if (password.empty()) {
		showMessage("Password cannot be empty.");
		return;
	}

	// Оримуємо повне ім'я
	cout << "Enter full name: ";
	getline(cin, fullName);

	if (fullName.empty()) {
		showMessage("Full name cannot be empty.");
		return;
	}

	// Оримуємо позицію (в компанії, посада)
	cout << "Enter position: ";
	getline(cin, position);

	if (position.empty()) {
		showMessage("Position cannot be empty.");
		return;
	}

	// Оримуємо зарплату
	string salaryStr;
	cout << "Enter base salary: $";
	getline(cin, salaryStr);

	try {
		salary = stod(salaryStr);
		if (salary < 0) {
			showMessage("Salary cannot be negative.");
			return;
		}
	}
	catch (...) {
		showMessage("Invalid salary amount.");
		return;
	}

	// Перевіряємо чи адмін
	string adminChoice;
	cout << "Make this user an admin? (y/n): ";
	getline(cin, adminChoice);

	isAdmin = (adminChoice == "y" || adminChoice == "Y");

	// Створюємо і зберігаємо користувача
	User newUser(username, password, fullName, position, salary, isAdmin);

	if (FileManager::addUser(newUser)) {
		showMessage("User created successfully.");
	}
	else {
		showMessage("Error creating user.");
	}
}

void UserInterface::deleteUser() {
	showTitle("Delete User");

	// Показуємо перед видаленням всіх користувачів
	vector<User*> users = FileManager::getAllUsers();

	if (users.empty()) {
		showMessage("No users found in the system.");
		return;
	}

	// Показуємо заголовок
	cout << setw(15) << left << "Username"
		<< setw(25) << left << "Full Name"
		<< setw(20) << left << "Position"
		<< "  Role\n";
	cout << string(70, '-') << "\n";

	// Показуємо користувачів
	for (const auto& user : users) {
		// Don't show admin user in the deletion list
		if (user->getUsername() != "admin") {
			cout << setw(15) << left << user->getUsername()
				<< setw(25) << left << user->getFullName()
				<< setw(20) << left << user->getPosition()
				<< "  " << (user->isAdmin() ? "Admin" : "User") << "\n";
		}
	}

	cout << string(70, '-') << "\n\n";

	// Очищаємо
	for (User* user : users) {
		delete user;
	}

	// Отримуємо username
	string username;
	cout << "Enter username to delete (or 'back' to return): ";
	getline(cin, username);

	if (username == "back") {
		return;
	}

	if (username == "admin") {
		showMessage("Cannot delete the main admin account.");
		return;
	}

	// Підтверджуємо видалення
	string confirm;
	cout << "Are you sure you want to delete user '" << username << "'? (y/n): ";
	getline(cin, confirm);

	if (confirm == "y" || confirm == "Y") {
		if (FileManager::deleteUser(username)) {
			showMessage("User deleted successfully.");
		}
		else {
			showMessage("User not found or could not be deleted.");
		}
	}
	else {
		showMessage("Delete operation cancelled.");
	}
}

void UserInterface::showUserProfile(User* user) {
	if (!user) {
		showMessage("User not found.");
		return;
	}

	showTitle("User Profile");

	cout << "Username: " << user->getUsername() << "\n";
	cout << "Full Name: " << user->getFullName() << "\n";
	cout << "Position: " << user->getPosition() << "\n";
	cout << "Base Salary: $" << fixed << setprecision(2) << user->getBaseSalary() << "\n";
	cout << "Role: " << (user->isAdmin() ? "Administrator" : "Regular User") << "\n";

	waitForKeypress();
}

string UserInterface::getHiddenInput(const string& prompt) {
	string input;
	cout << prompt;

	char ch;
	while (ch = _getch()) {
		if (ch == '\r' || ch == '\n') {
			cout << endl;
			break;
		}
		else if (ch == '\b' && !input.empty()) { // Backspace
			input.pop_back();
			cout << "\b \b";
		}
		else if (ch >= 32 && ch <= 126) {  // Символи виводу
			input.push_back(ch);
			cout << '*';
		}
	}

	return input;
}