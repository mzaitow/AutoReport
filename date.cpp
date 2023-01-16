#include "date.h"
#include <ctime>

using namespace std;

// Инициализируем Си-структуру tm
struct tm initializeDateStruct(int d, int m, int y) {
  struct tm ds = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

  ds.tm_mday = d;
  ds.tm_mon = m;
  ds.tm_year = y - 1900;

  return ds;
}

Date::Date(int d, int m, int y)
  : m_DateStruct(initializeDateStruct(d, m, y)),
    day(d),
    month(m),
    year(y) {}

Date::Date(const Date &date)
  : m_DateStruct(date.m_DateStruct),
    day(date.day),
    month(date.month),
    year(date.year) {}

int Date::getDifferenceInDays(Date &date) {
  double diff = difftime(mktime(&m_DateStruct), mktime(&date.m_DateStruct)) / (60*60*24);

  return static_cast<int>(diff);
}

// Оператор возвращает разницу в днях
int Date::operator-(Date &date) {
  double diff = difftime(mktime(&m_DateStruct), mktime(&date.m_DateStruct)) / (60*60*24);

  return static_cast<int>(diff);
}

Date &Date::operator=(const Date &date) {
  if(&date != this) {
    day = date.day;
    month = date.month;
    year = date.year;
    m_DateStruct = date.m_DateStruct;
  }

  return *this;
}

bool Date::operator<(const Date &date) {
  Date tmpDate = date;

  return *this - tmpDate < 0;
}

void Date::addWorkDays(const int days, const int daysInWeek) {
    time_t unixTime = mktime(&m_DateStruct);
    int weekend = 7 - daysInWeek;

    for(int i = 0; i < days; ++i) {
        unixTime += std::localtime(&unixTime)->tm_wday > daysInWeek ? 86400 * weekend : 86400;
    }

    m_DateStruct = *std::localtime(&unixTime);
    day = m_DateStruct.tm_mday;
    month = m_DateStruct.tm_mon;
    year = m_DateStruct.tm_year;
}

void Date::addDays(const int days) {
    time_t unixTime = mktime(&m_DateStruct);
    unixTime += days * 86400;

    m_DateStruct = *std::localtime(&unixTime);
    day = m_DateStruct.tm_mday;
    month = m_DateStruct.tm_mon;
    year = m_DateStruct.tm_year;
}

namespace std {
  string to_string(Date &date) {
    return to_string(date.day) + '.' + to_string(date.month) + '.' + to_string(date.year);
  }
}
