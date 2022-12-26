#ifndef DATE_H
#define DATE_H

#include <ctime>
#include <string>

class Date {
  struct std::tm m_DateStruct;

public:
  int day;
  int month;
  int year;

  Date(int d, int m, int y);
  Date(const Date &date);
  int getDifferenceInDays(Date &date);
  // РћРїРµСЂР°С‚РѕСЂ РІРѕР·РІСЂР°С‰Р°РµС‚ СЂР°Р·РЅРёС†Сѓ РІ РґРЅСЏС…
  int operator-(Date &date);
  Date &operator=(const Date &date);
  bool operator<(const Date &date);
};

namespace std {
  string to_string(Date &date);
}

#endif // DATE_H
