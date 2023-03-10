#ifndef INVOICE_H
#define INVOICE_H

#include "date.h"

class Invoice {
public:
  int ID;                 // РРґРµРЅС‚РёС„РёРєР°С†РёРѕРЅРЅС‹Р№ РЅРѕРјРµСЂ СЃС‡С‘С‚Р°-С„Р°РєС‚СѓСЂС‹
  int offerID;            // РРґРµРЅС‚РёС„РёРєР°С†РёРѕРЅРЅС‹Р№ РЅРѕРјРµСЂ РґРѕРіРѕРІРѕСЂР°
  int amount;             // РЎС‡С‘С‚-С„Р°РєС‚СѓСЂР°
  Date date;              // Р”Р°С‚Р°
  int paymentTerm;
  int penalty;            // РџРµРЅРё
  Date termDate;

  Invoice(const int ID_, const int offerID_, const int amount_, const Date date_, const int paymentTerm_);
};

#endif // INVOICE_H
