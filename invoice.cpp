#include "invoice.h"
#include "date.h"

Invoice::Invoice(const int ID_, const int offerID_, const int amount_, const Date date_, const int paymentTerm_)
  : ID(ID_),
    offerID(offerID_),
    amount(amount_),
    date(date_),
    paymentTerm(paymentTerm_),
    penalty(0),
    termDate(date_) {}
