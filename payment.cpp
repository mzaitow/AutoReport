#include "date.h"
#include "payment.h"

Payment::Payment(const int ID_, const int offerID_, const int invoiceID_, const int payment_, const Date date_, const int paymentTerm_)
  : ID(ID_),
    offerID(offerID_),
    invoiceID(invoiceID_),
    payment(payment_),
    date(date_),
    paymentTerm(paymentTerm_),
    termDate(date_),
    penalty(0) {}

Payment::Payment(const Payment &other)
  : ID(other.ID),
    offerID(other.offerID),
    invoiceID(other.invoiceID),
    payment(other.payment),
    date(other.date),
    paymentTerm(other.paymentTerm),
    termDate(other.termDate),
    penalty(other.penalty) {}

Payment &Payment::operator=(const Payment &other) {
  if(&other != this) {
    ID = other.ID;
    offerID = other.offerID;
    invoiceID = other.invoiceID;
    payment = other.payment;
    date = other.date;
    paymentTerm = other.paymentTerm;
    termDate = other.termDate;
    penalty = other.penalty;
  }

  return *this;
}
