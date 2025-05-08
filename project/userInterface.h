#pragma once

#include <string>
#include "user.h"

using namespace std;

class UserInterface {
public:
  // Загальні UI елементи
  static void clearScreen();
  static void showTitle(const string& title);
  static void showMessage(const string& message);
  static void waitForKeypress();

  // Початковий і вихідний екрани
  static void showWelcomeScreen();
  static void showExitMessage();

  // Менюшки
  static string showAuthMenu();
  static string showAdminMenu();
  static string showUserMenu();

  // Системи керування користувачами
  static void displayAllUsers();
  static void createUser();
  static void deleteUser();
  static void showUserProfile(User* user);

  // Допоміжна функція
  static string getHiddenInput(const string& prompt);
};