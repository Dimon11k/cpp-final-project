#include <iostream>
#include <string>
#include "user.h"
#include "fileManager.h"
#include "payroll.h"
#include "userInterface.h"
#include "authentication.h"

using namespace std;

int main() {
  // Ініціалізація
  FileManager::initializeFiles();
  UserInterface::showWelcomeScreen();

  bool isRunning = true;
  User* currentUser = nullptr;

  while (isRunning) {
    if (!currentUser) {
      // Якщо користувач НЕ увійшов в систему - показати меню авторизації
      string authChoice = UserInterface::showAuthMenu();

      if (authChoice == "1") {
        // Увійти
        currentUser = Authentication::login();
      }
      else if (authChoice == "2") {
        // Зареєстуватися
        Authentication::registerUser();
      }
      else if (authChoice == "exit") {
        isRunning = false;
      }
    }
    else {
      // Користувач увійшов в систему - показати відповідне меню
      if (currentUser->isAdmin()) {
        string adminChoice = UserInterface::showAdminMenu();

        if (adminChoice == "1") {
          UserInterface::displayAllUsers();
        }
        else if (adminChoice == "2") {
          UserInterface::createUser();
        }
        else if (adminChoice == "3") {
          UserInterface::deleteUser();
        }
        else if (adminChoice == "4") {
          PayrollManager::viewAllPayrolls();
        }
        else if (adminChoice == "5") {
          PayrollManager::processSalaryPayment();
        }
        else if (adminChoice == "6") {
          delete currentUser;
          currentUser = nullptr;
          UserInterface::showMessage("Logged out successfully.");
        }
        else if (adminChoice == "exit") {
          isRunning = false;
        }
      }
      else {
        // Меню користувача (співробітника)
        string userChoice = UserInterface::showUserMenu();

        if (userChoice == "1") {
          // View profile
          UserInterface::showUserProfile(currentUser);
        }
        else if (userChoice == "2") {
          // Записи зарплат
          PayrollManager::viewUserPayrolls(currentUser->getUsername());
        }
        else if (userChoice == "3") {
          // Log out
          delete currentUser;
          currentUser = nullptr;
          UserInterface::showMessage("Logged out successfully.");
        }
        else if (userChoice == "exit") {
          isRunning = false;
        }
      }
    }
  }

  // Cleanup
  if (currentUser) {
    delete currentUser;
  }

  UserInterface::showExitMessage();
  return 0;
}