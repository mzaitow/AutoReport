#ifndef FILTER_H
#define FILTER_H

#include <unordered_map>
#include "offer.h"
#include "reportformer.h"
#include "report.h"
#include <functional>
#include "date.h"

class FilterSettings {
public:
  State state;
  Report report;

  FilterSettings();
};

class Filter {
private:
  std::unordered_map<int, Offer> &m_offers;
  std::unordered_map<int, std::unordered_map<int, Invoice>> &m_invoices;
  std::unordered_map<int, std::unordered_map<int, Payment>> &m_payments;

  void filterBy(std::function<bool(const Offer &offer)> by);

  void filterOverdue();
  void filterNonExpired();
  void filterDebitor();
  void filterCreditor();
  void filterPaid();

public:
  FilterSettings filterSettings;
  Date date;

  Filter(std::unordered_map<int, Offer> &offers,
         std::unordered_map<int, std::unordered_map<int, Invoice>> &invoices,
         std::unordered_map<int, std::unordered_map<int, Payment>> &payments);

  void filter();
};

#endif // FILTER_H
