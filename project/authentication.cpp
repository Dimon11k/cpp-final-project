#include "authentication.h"
#include "fileManager.h"
#include "userInterface.h"
#include <iostream>
#include <regex>
using namespace std;

User* Authentication::login() {
	UserInterface::showTitle("Login");
	string username, password;

	// Введення username
	cout << "Enter username: ";
	getline(cin, username);
	if (username.empty()) {
		UserInterface::showMessage("Username cannot be empty.");
		return nullptr;
	}

	// Введення пароля (приховане від користувача)
	password = UserInterface::getHiddenInput("Enter password: ");
	if (password.empty()) {
		UserInterface::showMessage("Password cannot be empty.");
		return nullptr;
	}

	// Пошук користувача в базі даних (файлі)
	User* user = FileManager::findUserByUsername(username);
	if (!user) {
		UserInterface::showMessage("User not found.");
		return nullptr;
	}

	// Перевірка пароля
	if (user->getPassword() != password) {
		UserInterface::showMessage("Incorrect password.");
		delete user;
		return nullptr;
	}

	UserInterface::showMessage("Login successful!");
	return user;
}

bool Authentication::registerUser() {
	UserInterface::showTitle("Register New User");
	string username, password, confirmPassword;
	string fullName, position;
	double salary = 0.0;

	// Введення username
	cout << "Create username: ";
	getline(cin, username);
	if (!validateUsername(username)) {
		UserInterface::showMessage("Username must be at least 3 characters and contain only letters, numbers, and underscores.");
		return false;
	}

	// Перевірка чи username вільний
	User* existingUser = FileManager::findUserByUsername(username);
	if (existingUser) {
		delete existingUser;
		UserInterface::showMessage("Username already exists.");
		return false;
	}

	// Введення пароля
	password = UserInterface::getHiddenInput("Create password: ");
	if (!validatePassword(password)) {
		UserInterface::showMessage("Password must be at least 6 characters long.");
		return false;
	}

	// Підтвердження пароля
	confirmPassword = UserInterface::getHiddenInput("Confirm password: ");
	if (password != confirmPassword) {
		UserInterface::showMessage("Passwords do not match.");
		return false;
	}

	// Введення повного ім'я
	cout << "Enter your full name: ";
	getline(cin, fullName);
	if (fullName.empty()) {
		UserInterface::showMessage("Full name cannot be empty.");
		return false;
	}

	// Введення посади (наприклад, менеджер)
	cout << "Enter your position: ";
	getline(cin, position);
	if (position.empty()) {
		UserInterface::showMessage("Position cannot be empty.");
		return false;
	}

	// Введення зарплати
	string salaryStr;
	cout << "Enter your base salary: $";
	getline(cin, salaryStr);
	try {
		salary = stod(salaryStr);
		if (salary < 0) {
			UserInterface::showMessage("Salary cannot be negative.");
			return false;
		}
	}
	catch (...) {
		UserInterface::showMessage("Invalid salary amount.");
		return false;
	}

	// Створення користувача (перший параметр false означає що це звичайний користувач)
	User newUser(username, password, fullName, position, salary, false);
	if (FileManager::addUser(newUser)) {
		UserInterface::showMessage("Registration successful! You can now log in.");
		return true;
	}
	else {
		UserInterface::showMessage("Error creating user account.");
		return false;
	}
}

bool Authentication::validateUsername(const string& username) {
	// Username повинен мати мінімум 3 символи і містити лише букви, цифри, і знак підресленнян '_'
	if (username.length() < 3) {
		return false;
	}
	regex usernamePattern("^[a-zA-Z0-9_]+$");
	return regex_match(username, usernamePattern);
}

bool Authentication::validatePassword(const string& password) {
	// Пароль повинен мати мінімум 6 символів
	return password.length() >= 6;
}