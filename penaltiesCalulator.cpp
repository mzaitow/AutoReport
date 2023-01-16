#include "penaltiesCalulator.h"
#include "date.h"

float calculatePenalties(Date term, Date current, float penaltyPercent, int amount) {
  int period = current - term;
  float percent = amount / 100;
  float penalty = amount + percent * penaltyPercent;

  for(int i = 0; i < period; ++i) {
    percent = penalty / 100;
    penalty += percent * penaltyPercent;
  }

  return penalty - amount;
}
