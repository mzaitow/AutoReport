#include "offer.h"
#include "date.h"

#include <iostream>

Offer::Offer(const int ID_, const std::string contractor_, const int amount_,
             const Date date_, const Date paymentDate_, const float penalty_,
             const State state_, const std::string supervisor_, const int depNumber,
             const DayType daytype)
  : ID(ID_),
    contractor(contractor_),
    amount(amount_),
    date(date_),
    paymentDate(paymentDate_),
    penaltyPercent(penalty_),
    state(state_),
    paid(0),
    penalty(0),
    penaltiesPaid(0),
    supervisor(supervisor_),
    departamentNumber(depNumber),
    dayType(daytype) {}
