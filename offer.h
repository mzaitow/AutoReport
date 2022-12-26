#ifndef OFFER_H
#define OFFER_H

#include <string>
#include <unordered_map>
#include "date.h"
#include "invoice.h"

enum class State {
  NotSelected,
  Debitor,
  Creditor
};

class Offer {
public:
  int ID;
  std::string contractor; // РСЃРїРѕР»РЅРёС‚РµР»СЊ
  int  amount;            // РЎСѓРјРјР°
  Date date;              // Р”Р°С‚Р° РґРѕРіРѕРІРѕСЂР°
  Date paymentDate;       // Р”Р°С‚Р° РѕРїР»Р°С‚С‹ РїРѕ РґРѕРіРѕРІРѕСЂСѓ 
  int  penaltyPercent;    // РќРµСѓСЃС‚РѕР№РєР° РІ РїСЂРѕС†РµРЅС‚Р°С… РІ РґРµРЅСЊ
  State state;            // РЎРѕСЃС‚РѕСЏРЅРёРµ: РґРµР±РёС‚РѕСЂ РёР»Рё РєСЂРµРґРёС‚РѕСЂ
  int paid;               // РЎСѓРјРјР°, СѓРїР»Р°С‡РµРЅРЅР°СЏ РїРѕ РґРѕРіРѕРІРѕСЂСѓ
  int penalty;            // РџРµРЅРё
  std::string supervisor; // РљСѓСЂР°С‚РѕСЂ РґРѕРіРѕРІРѕСЂР°
  int departamentNumber;  // РќРѕРјРµСЂ РѕС‚РґРµР»Р°
  std::unordered_map<int, Invoice> overdueInvoices;

  Offer(const int ID_, const std::string contractor_, const int amount_,
        const Date date_, const Date paymentDate_, const int penalty_,
        const State state_, const std::string supervisor_, const int depNumber);
};

#endif // OFFER_H
