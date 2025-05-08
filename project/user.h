#pragma once

#include <string>
using namespace std;

class User {
private:
	string username;
	string password;
	string fullName;
	string position;
	double baseSalary;
	bool admin;

public:
	// Конструктор
	User(const string& username,
		const string& password,
		const string& fullName,
		const string& position,
		double baseSalary,
		bool admin = false);


	// Геттери
	string getUsername() const;
	string getPassword() const;
	string getFullName() const;
	string getPosition() const;
	double getBaseSalary() const;
	bool isAdmin() const;

	// Сеттери
	void setPassword(string& newPassword);
	void setFullName(string& newFullName);
	void setPosition(string& newPosition);
	void setBaseSalary(double newSalary);

	// Інші методи
	string toString() const;
	static User* fromString(const string& str);
};