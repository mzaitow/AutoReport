#ifndef PAYMENT_H
#define PAYMENT_H

#include "date.h"

class Payment {
public:
  int ID;
  int offerID;
  int invoiceID;
  int payment;
  Date date;

  Payment(const int ID_, const int offerID_, const int invoiceID_, const int payment_, const Date date_);
  Payment(const Payment &other);
  Payment &operator=(const Payment &other);
};

#endif // PAYMENT_H
