#include "user.h"
#include <sstream>
#include <vector>

using namespace std;

User::User(
	const string& username,
	const string& password,
	const string& fullName,
	const string& position,
	double baseSalary,
	bool admin)
{
	this->username = username;
	this->password = password;
	this->fullName = fullName;
	this->position = position;
	this->baseSalary = baseSalary;
	this->admin = admin;
}


// Геттери

string User::getUsername() const {
	return username;
}

string User::getPassword() const {
	return password;
}

string User::getFullName() const {
	return fullName;
}

string User::getPosition() const {
	return position;
}

double User::getBaseSalary() const {
	return baseSalary;
}

bool User::isAdmin() const {
	return admin;
}

// Сеттери
void User::setPassword(string& newPassword) {
	password = newPassword;
}

void User::setFullName(string& newFullName) {
	fullName = newFullName;
}

void User::setPosition(string& newPosition) {
	position = newPosition;
}

void User::setBaseSalary(double newSalary) {
	baseSalary = newSalary;
}

// Utility methods
string User::toString() const {
	stringstream ss;
	ss << username << "|"
		<< password << "|"
		<< fullName << "|"
		<< position << "|"
		<< baseSalary << "|"
		<< (admin ? "1" : "0");
	return ss.str();
}

User* User::fromString(const string& str) {
	stringstream ss(str);
	string username, password, fullName, position, temp;
	double baseSalary;
	bool admin;

	// Parse using | as delimiter
	getline(ss, username, '|');
	getline(ss, password, '|');
	getline(ss, fullName, '|');
	getline(ss, position, '|');
	getline(ss, temp, '|');
	baseSalary = stod(temp);
	getline(ss, temp);
	admin = (temp == "1");

	return new User(username, password, fullName, position, baseSalary, admin);
}
