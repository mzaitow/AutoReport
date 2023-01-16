#include "filter.h"
#include <unordered_map>
#include "offer.h"
#include "invoice.h"
#include "payment.h"
#include "report.h"

using namespace std;

FilterSettings::FilterSettings()
  : state(State::NotSelected), report(Report::NotSelected) {}

Filter::Filter(unordered_map<int, Offer> &offers,
               unordered_map<int, unordered_map<int, Invoice>> &invoices,
               std::unordered_map<int, std::unordered_map<int, std::vector<Payment>>> &payments)
    : m_offers(offers), m_invoices(invoices), m_payments(payments), date(1, 1, 2000) {}

void Filter::filter() {
  switch (filterSettings.state) {
  case State::Debitor:
    filterDebitor();
    break;
  case State::Creditor:
    filterCreditor();
    break;
  case State::NotSelected:
      break;
  }

  switch (filterSettings.report) {
  case Report::NonExpired:
    filterNonExpired();
    break;
  case Report::Overdue:
    filterOverdue();
    break;
  case Report::Paid:
    filterPaid();
    break;
  case Report::NotSelected:
  case Report::All:
    break;
  }
}

void Filter::filterBy(function<bool(const Offer &offer)> by) {
  vector<int> IDs;
  IDs.reserve(m_offers.size() / 2);

  for(auto &i : m_offers) {
    if(!by(i.second))
      IDs.push_back(i.first);
  }

  for(auto i : IDs) {
    m_invoices.erase(i);
    m_payments.erase(i);
    m_offers.erase(i);
  }
}

void Filter::filterOverdue() {
    filterBy([](const Offer &offer) -> bool {
        return offer.status == Status::Overdue;
    });
}

void Filter::filterNonExpired() {
    filterBy([](const Offer &offer) -> bool {
        return offer.status == Status::NonExpired;
    });
}

void Filter::filterDebitor() {
  filterBy([](const Offer &offer) -> bool {
    return offer.state == State::Debitor;
  });
}

void Filter::filterCreditor() {
  filterBy([](const Offer &offer) -> bool {
    return offer.state == State::Creditor;
  });
}

void Filter::filterPaid() {
  filterBy([](const Offer &offer) -> bool {
    return offer.status == Status::Paid;
  });
}

