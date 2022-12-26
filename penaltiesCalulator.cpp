#include "penaltiesCalulator.h"
#include "date.h"

int calculatePenalties(Offer offer, Date penaltyPeriodEnd) {
  int period = penaltyPeriodEnd - offer.paymentDate;
  int percent = offer.amount / 100;
  int penalty = offer.amount + percent * offer.penaltyPercent;

  for(int i = 0; i < period; ++i) {
    percent = penalty / 100;
    penalty += percent * offer.penaltyPercent;
  }

  return penalty;
}
