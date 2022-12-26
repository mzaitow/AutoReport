#ifndef REPORT_H
#define REPORT_H

enum class Report {
  NotSelected, // Вид отчёта не выбран
  Overdue,     // Просроченные
  NonExpired,  // Непросроченные
  Paid,        // Оплаченные
  All          // Все
};

#endif // REPORT_H
