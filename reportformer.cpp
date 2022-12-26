#include "reportformer.h"
#include <fstream>
#include "csvbuilder.h"

using namespace std;

ReportFormer::ReportFormer(std::unordered_map<int, Offer> &offers,
       std::unordered_map<int, std::unordered_map<int, Invoice>> &invoices,
       std::unordered_map<int, std::unordered_map<int, Payment>> &payments)
  : m_offers(offers),
    m_invoices(invoices),
    m_payments(payments) {}

std::string ReportFormer::getOfferStatus(const Offer &offer) {
  if(offer.amount + offer.penalty <= offer.paid)
    return "Исполнен";

  return "default";
}

void ReportFormer::formReportForSupervisor() {
  CSVBuilder builder(outputFilename.c_str());

  std::string columns[] {
      "Номер договора", "Компания", "Сумма", "Дата", "Срок", "Процент пени", "Оплачено", "Куратор", "Номер отдела", "Статус" };

  for(auto &i : columns) {
    builder.setCellContent(i);
    builder.nextColumn();
  }

  builder.nextRow();

  for(auto &offer : m_offers) {
    if(offer.second.supervisor != supervisor)
      continue;

    builder.setCellContent(to_string(offer.second.ID)); builder.nextColumn();
    builder.setCellContent(offer.second.contractor); builder.nextColumn();
    builder.setCellContent(to_string(offer.second.amount)); builder.nextColumn();
    builder.setCellContent(to_string(offer.second.date)); builder.nextColumn();
    builder.setCellContent(to_string(offer.second.paymentDate)); builder.nextColumn();
    builder.setCellContent(to_string(offer.second.penaltyPercent)); builder.nextColumn();
    builder.setCellContent(to_string(offer.second.paid)); builder.nextColumn();
    builder.setCellContent(offer.second.supervisor); builder.nextColumn();
    builder.setCellContent(to_string(offer.second.departamentNumber)); builder.nextColumn();
    builder.setCellContent(getOfferStatus(offer.second)); builder.nextColumn();

    builder.nextRow();
  }
}

void ReportFormer::formReportForDepartamentHead() {}
void ReportFormer::formReportForDirector() {}

void ReportFormer::reportForm() {
  switch(purpose) {
  case Purpose::Supervisor:
    formReportForSupervisor();
    break;
  case Purpose::Director:
    formReportForDirector();
    break;
  case Purpose::DepartamentHead:
    formReportForDepartamentHead();
    break;
  case Purpose::NotSelected:
    break;
  }
}
