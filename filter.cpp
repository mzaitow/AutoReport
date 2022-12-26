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
               unordered_map<int, unordered_map<int, Payment>> &payments)
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
    // filterNonExpired();
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
    /* filterBy([this](const Offer &offer) -> bool {
      for(auto &i : m_invoices[offer.ID]) {
          if(date < i.second.paymentTerm)
              continue;

          int payment = 0;

          for(auto &p : m_payments[offer.ID]) {
            if(p.second.invoiceID == i.second.ID)
                payment += p.second.payment;
          }

          if(payment < i.second.amount) {
              Invoice &inv = i.second;

              offer.overdueInvoices.insert({ i.second.ID, inv });

              return true;
          }

      }

      return false;
    }); */
}

void Filter::filterNonExpired() {
    filterBy([this](const Offer &offer) -> bool {
      for(auto &i : m_invoices[offer.ID]) {
          if(date < i.second.paymentTerm)
              continue;

          int payment = 0;

          for(auto &p : m_payments[offer.ID]) {
            if(p.second.invoiceID == i.second.ID)
                payment += p.second.payment;
          }

          if(payment < i.second.amount)
              return false;

      }

      return true;
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
  filterBy([this](const Offer &offer) -> bool {
    return
      (m_payments.find(offer.ID) != m_payments.end()) &&
      (offer.amount <= offer.paid);
  });
}

