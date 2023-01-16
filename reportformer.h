#ifndef REPORTFORMER_H
#define REPORTFORMER_H

#include <unordered_map>
#include <vector>
#include "offer.h"
#include "invoice.h"
#include "payment.h"

enum class Purpose {
  NotSelected,     // Лицо для отчёта не выбрано
  Supervisor,      // Куратор договора
  DepartamentHead, // Начальник отдела
  Director         // Директор
};

class ReportFormer {
private:
  std::unordered_map<int, Offer> &m_offers;
  std::unordered_map<int, std::unordered_map<int, Invoice>> &m_invoices;
  std::unordered_map<int, std::unordered_map<int, std::vector<Payment>>> &m_payments;

  void formReportForSupervisor();
  void formReportForDepartamentHead();
  void formReportForDirector();

public:
  std::string outputFilename;
  std::string supervisor;
  int departamentNumber;

  Purpose purpose;

  ReportFormer(std::unordered_map<int, Offer> &offers,
         std::unordered_map<int, std::unordered_map<int, Invoice>> &invoices,
         std::unordered_map<int, std::unordered_map<int, std::vector<Payment>>> &payments);

  std::string getOfferStatusString(const Offer &offer);

  void reportForm();
};

#endif // REPORTFORMER_H
