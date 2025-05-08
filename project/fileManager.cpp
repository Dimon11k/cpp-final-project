#include "fileManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

void FileManager::initializeFiles() {
  // Створюємо потрібні файли, якщо не існують
  vector<string> filenames = { "users.txt", "payroll.txt" };

  for (const auto& filename : filenames) {
    ifstream fileCheck(filename);
    if (!fileCheck) {
      // Створюємо файл, якщо не існує
      ofstream newFile(filename);

      // Якщо це файл users, створюємо акаунт адміна, якщо він пустий
      if (filename == "users.txt") {
        User admin("admin", "admin123", "Administrator", "System Admin", 5000.0, true);
        newFile << admin.toString() << endl;
      }

      newFile.close();
    }
    fileCheck.close();
  }
}

vector<User*> FileManager::getAllUsers() {
  vector<User*> users;
  vector<string> lines = readFile("users.txt");

  for (const auto& line : lines) {
    if (!line.empty()) {
      User* user = User::fromString(line);
      users.push_back(user);
    }
  }

  return users;
}

User* FileManager::findUserByUsername(const string& username) {
  vector<string> lines = readFile("users.txt");

  for (const auto& line : lines) {
    if (!line.empty()) {
      User* user = User::fromString(line);
      if (user->getUsername() == username) {
        return user;
      }
      delete user;
    }
  }

  return nullptr;
}

bool FileManager::addUser(const User& user) {
  // Перевіряємо чи користувач існує
  User* existingUser = findUserByUsername(user.getUsername());
  if (existingUser) {
    delete existingUser;
    return false; // Користувач існує
  }

  // В іншому випадку додаємо користувача до файлу
  bool result = appendToFile("users.txt", user.toString());
  return result;
}

bool FileManager::updateUser(const User& user) {
  vector<string> lines = readFile("users.txt");
  bool userFound = false;

  for (size_t i = 0; i < lines.size(); ++i) {
    if (!lines[i].empty()) {
      User* existingUser = User::fromString(lines[i]);
      if (existingUser->getUsername() == user.getUsername()) {
        lines[i] = user.toString();
        userFound = true;
      }
      delete existingUser;
    }
  }

  if (userFound) {
    return writeFile("users.txt", lines);
  }

  return false; // Користувач не знайдений
}

bool FileManager::deleteUser(const string& username) {
  vector<string> lines = readFile("users.txt");
  bool userFound = false;
  vector<string> newLines;

  for (const auto& line : lines) {
    if (!line.empty()) {
      User* existingUser = User::fromString(line);
      if (existingUser->getUsername() != username) {
        newLines.push_back(line);
      }
      else {
        userFound = true;
      }
      delete existingUser;
    }
  }

  if (userFound) {
    return writeFile("users.txt", newLines);
  }

  return false; // Користувач не знайдений
}

vector<string> FileManager::readFile(const string& filename) {
  vector<string> lines;
  ifstream file(filename);

  if (file.is_open()) {
    string line;
    while (getline(file, line)) {
      lines.push_back(line);
    }
    file.close();
  }

  return lines;
}

bool FileManager::writeFile(const string& filename, const vector<string>& lines) {
  ofstream file(filename);

  if (file.is_open()) {
    for (const auto& line : lines) {
      file << line << endl;
    }
    file.close();
    return true;
  }

  return false;
}

bool FileManager::appendToFile(const string& filename, const string& line) {
  ofstream file(filename, ios::app);

  if (file.is_open()) {
    file << line << endl;
    file.close();
    return true;
  }

  return false;
}