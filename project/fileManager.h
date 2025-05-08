#pragma once

#include <string>
#include <vector>
#include "user.h"

using namespace std;

class FileManager {
public:
  // Ініціалізація файлів
  static void initializeFiles();

  // Методи адміна
  static vector<User*> getAllUsers();
  static User* findUserByUsername(const string& username);
  static bool addUser(const User& user);
  static bool updateUser(const User& user);
  static bool deleteUser(const string& username);

  // Загальні файлові методи
  static vector<string> readFile(const string& filename);
  static bool writeFile(const string& filename, const vector<string>& lines);
  static bool appendToFile(const string& filename, const string& line);
};