#pragma once

#include "user.h"
#include <string>

using namespace std;

class Authentication {
public:
  // Аутентифікація користувача
  static User* login();
  static bool registerUser();

  // Валідація даних користувача
  static bool validateUsername(const string& username);
  static bool validatePassword(const string& password);
};