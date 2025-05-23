#pragma once

#include <string>
#include <vector>
#include "user.h"

using namespace std;

struct PayrollRecord {
  string username;
  string date;
  double amount;
  string description;

  // Інші методи
  string toString() const;
  static PayrollRecord fromString(const string& str);
};

class PayrollManager {
public:
  // Методи адміна
  static void processSalaryPayment();
  static void viewAllPayrolls();
  static void viewUserPayrolls(const string& username);

  // Внутрішні методи (ті, які НЕ будуть використовуватися ззовні)
  static vector<PayrollRecord> getAllPayrolls();
  static vector<PayrollRecord> getUserPayrolls(const string& username);
  static bool addPayrollRecord(const PayrollRecord& record);
};