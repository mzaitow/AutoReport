#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csvparser.h"
#include "report.h"
#include <QErrorMessage>
#include <QFileDialog>
#include <QPushButton>
#include <iostream>
#include "date.h"
#include "penaltiesCalulator.h"

using namespace std;

MainWindow::~MainWindow() {
  delete ui;
}

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    filter(m_offers, m_invoices, m_payments),
    former(m_offers, m_invoices, m_payments),
    ui(new Ui::MainWindow) {
  QLocale::setDefault(QLocale::Russian);
  ui->setupUi(this);
}

Date parseDate(const std::string &dateString) {
  int rawDate[3] { 0, 0, 0 };
  int i = 0;

  string::const_iterator pos = dateString.begin();
  string::const_iterator prevPos = pos;

  do {
    ++pos;

    if(*pos == '.' || *pos == 0) {
      rawDate[i++] = stoi(dateString.substr(static_cast<size_t>(distance(dateString.begin(), prevPos)),
                                            static_cast<size_t>(distance(prevPos, pos))));
      prevPos = pos + 1;
    }
  } while(dateString.end() != pos || i < 3);

  return Date(rawDate[0], rawDate[1], rawDate[2]);
}

State parseState(const std::string &state) {
  if(state == "дебитор")
    return State::Debitor;
  else if(state == "кредитор")
    return State::Creditor;

  throw std::runtime_error(std::string("Unexpected company state ") + state);
}

DayType parseDayType(const std::string &daytype) {
    if(daytype == "банковские")
      return DayType::Bank;
    else if(daytype == "календарные")
      return DayType::Calendar;

    throw std::runtime_error(std::string("Unexpected day type ") + daytype);
}

void MainWindow::readOffers() {
  CSVParser parser(csvOffersFilename.c_str());

  parser.nextRow();

  while(!parser.getCellContent().empty()) {
    const int id = stoi(parser.getCellContent());

    const string companyName = parser.getForwardCellContent();
    const int amount = stoi(parser.getForwardCellContent());
    const Date date = parseDate(parser.getForwardCellContent());
    const Date paymentDate = parseDate(parser.getForwardCellContent());
    const int percent = stoi(parser.getForwardCellContent());
    const State state = parseState(parser.getForwardCellContent());
    const string supervisor = parser.getForwardCellContent();
    const int depNumber = stoi(parser.getForwardCellContent());
    const DayType daytype = parseDayType(parser.getForwardCellContent());

    m_offers.insert({ id,
                      Offer(id, companyName, amount,
                            date, paymentDate, percent,
                            state, supervisor, depNumber,
                            daytype) });

    parser.nextRow();
  }
}

void MainWindow::calcDeadlines() {
    for(auto &offer : m_offers) {
        calcOfferDeadlines(offer.second);
    }
}

void MainWindow::calcOfferDeadlines(Offer &offer) {
    for(auto &invoice : m_invoices[offer.ID]) {
        if(this->filter.date < invoice.second.termDate)
            continue;

        int paid = 0;

        for(auto &offerPayments : m_payments[offer.ID]) {
           for(auto &payment : offerPayments.second) {
              paid += payment.payment;
           }
        }

        if(paid < invoice.second.amount) {
            invoice.second.penalty =
                    calculatePenalties(invoice.second.date,
                                       filter.date,
                                       offer.penaltyPercent,
                                       invoice.second.amount);
            offer.overdueInvoices.insert({ invoice.second.ID, invoice.second });
        }
    }
}

void MainWindow::calcTermDays() {
    for(auto &offer : m_offers) {
        auto &offerInvoices = m_invoices[offer.second.ID];
        auto &offerPayments = m_payments[offer.second.ID];

        switch(offer.second.dayType) {
        case DayType::Bank:
            for(auto &invoice : offerInvoices) {
                invoice.second.termDate.addWorkDays(invoice.second.paymentTerm, 5);
            }

            for(auto &invoicePayments : offerPayments) {
                for(auto &payment : invoicePayments.second) {
                    payment.termDate.addWorkDays(payment.paymentTerm, 5);
                }
            }

            break;
        case DayType::Calendar:
            for(auto &invoice : offerInvoices) {
                invoice.second.termDate.addDays(invoice.second.paymentTerm);
            }

            for(auto &invoicePayments : offerPayments) {
                for(auto &payment : invoicePayments.second) {
                    payment.termDate.addDays(payment.paymentTerm);
                }
            }

            break;
        }
    }
}

void MainWindow::sumOffersPenalties() {
    for(auto &offer : m_offers) {
        for(auto &invoice : offer.second.overdueInvoices) {
            offer.second.penalty += invoice.second.penalty;
        }
    }
}

void MainWindow::calcStatus() {
    for(auto &offer : m_offers) {
        if(offer.second.amount + offer.second.penalty <= offer.second.paid)
          offer.second.status = Status::Paid;
        else if(offer.second.overdueInvoices.size() != 0 ||
                (offer.second.paid < offer.second.amount &&
                 offer.second.paymentDate < filter.date))
            offer.second.status = Status::Overdue;
        else
            offer.second.status = Status::NonExpired;
    }
}

void MainWindow::addPayment(const int ID, const int offerID, const int invoiceID,
                            const int payment, const Date date, const int paymentTerm) {
    if(m_payments.find(offerID) == m_payments.end()) {
        m_payments.insert({
            offerID,
            unordered_map<int, vector<Payment>>({ {
                invoiceID,
                vector<Payment>({ Payment(ID, offerID, invoiceID, payment, date, paymentTerm) })
            } })
        });
    } else {
        auto invoiceOffers = m_payments[offerID].find(invoiceID);

        if(invoiceOffers != m_payments[offerID].end()) {
            invoiceOffers->second.push_back(Payment(ID, offerID, invoiceID, payment, date, paymentTerm));
        } else {
            m_payments[offerID].insert({ {
                invoiceID,
                vector<Payment>({ Payment(ID, offerID, invoiceID, payment, date, paymentTerm) })
            } });
        }
    }
}

void MainWindow::addInvoice(const int ID, const int offerID, const int amount,
                            const Date date, const int paymentTerm) {
    if(m_invoices.find(offerID) == m_invoices.end()) {
        m_invoices.insert({
            offerID,
            unordered_map<int, Invoice>({ { ID, Invoice(ID, offerID, amount, date, paymentTerm) } })
        });
    } else {
        m_invoices[offerID].insert({ ID, Invoice(ID, offerID, amount, date, paymentTerm) });
    }
}

void MainWindow::readInvoices() {
  CSVParser parser(csvInvoicesFilename.c_str());

  parser.nextRow();

  while(!parser.getCellContent().empty()) {
    const int ID = stoi(parser.getCellContent());
    const int offerID = stoi(parser.getForwardCellContent());
    int paymentID = 0;

    if(!parser.getForwardCellContent().empty()) {
        paymentID = stoi(parser.getCellContent());
    }

    const int invoice = stoi(parser.getForwardCellContent());
    const Date date = parseDate(parser.getForwardCellContent());
    const int paymentTerm = stoi(parser.getForwardCellContent());

    auto offer = m_offers.find(offerID);

    if(offer != m_offers.end() && offer->second.state == State::Debitor) {
        addInvoice(ID, offerID, invoice, date, paymentTerm);
    } else {
        addPayment(ID, offerID, paymentID, invoice, date, paymentTerm);
    }

    parser.nextRow();
  }
}

void MainWindow::readPayments() {
  CSVParser parser(csvPaymentsFilename.c_str());

  parser.nextRow();

  while(!parser.getCellContent().empty()) {
    const int ID = stoi(parser.getCellContent());
    const int offerID = stoi(parser.getForwardCellContent());
    const int invoiceID = stoi(parser.getForwardCellContent());
    const int payment = stoi(parser.getForwardCellContent());
    const Date date = parseDate(parser.getForwardCellContent());
    const int term = stoi(parser.getForwardCellContent());

    auto offer = m_offers.find(offerID);

    if(offer != m_offers.end() && offer->second.state == State::Debitor) {
        addPayment(ID, offerID, invoiceID, payment, date, term);
    } else {
        addInvoice(ID, offerID, payment, date, term);
    }

    parser.nextRow();
  }
}

void MainWindow::calcPaymentUnderOffers() {
  for(auto &offer : m_offers) {
    for(auto &invoicePayments : m_payments[offer.first]) {
      for(auto &payment : invoicePayments.second) {
        offer.second.paid += payment.payment;
      }
    }
  }
}

void MainWindow::calcPenaltiesPaid() {
    for(auto &offerInvoices : m_invoices) {
        for(auto &invoice : offerInvoices.second) {
            int paid = 0;

            for(auto &payment : m_payments[invoice.second.offerID][invoice.second.ID]) {
                paid += payment.payment;
            }

            if(paid > invoice.second.amount) {
                m_offers.find(invoice.second.offerID)->second.penaltiesPaid +=
                        paid - invoice.second.amount;
            }
        }
    }
}

void MainWindow::on_radioButton_2_clicked() {
 filter.filterSettings.state = State::Debitor;
}


void MainWindow::on_radioButton_3_clicked() {
  filter.filterSettings.state = State::Creditor;
}


void MainWindow::on_radioButton_clicked() {
  filter.filterSettings.report = Report::NonExpired;
}


void MainWindow::on_radioButton_4_clicked() {
  filter.filterSettings.report = Report::Overdue;
}


void MainWindow::on_radioButton_5_clicked() {
  filter.filterSettings.report = Report::Paid;
}


void MainWindow::on_radioButton_6_clicked() {
  filter.filterSettings.report = Report::All;
}


void MainWindow::on_pushButton_3_clicked() {
  QErrorMessage msg;

  if(filter.filterSettings.state == State::NotSelected) {
    msg.showMessage("Не выбрана роль компании");
    msg.exec();

    return;
  }

  if(filter.filterSettings.report == Report::NotSelected) {
    msg.showMessage("Не выбран вид отчёта");
    msg.exec();

    return;
  }

  readOffers();
  readInvoices();
  readPayments();

  calcPaymentUnderOffers();
  calcTermDays();
  calcDeadlines();
  calcPenaltiesPaid();
  sumOffersPenalties();
  calcStatus();
  filter.filter();

  QString filename = QFileDialog::getSaveFileName(this, tr("Итоговая таблица"), "~/", tr("Text table files (*.csv)"));

  former.outputFilename = filename.toStdString();
  former.reportForm();
}

void MainWindow::on_pushButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Таблица с договорами"), "~/", tr("Text table files (*.csv)"));
  csvOffersFilename = filename.toStdString();
  findChild<QLabel*>("offer_label")->setText(csvOffersFilename.c_str());
}

void MainWindow::on_pushButton_2_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Таблица с счетами-фактурами"), "~/", tr("Text table files (*.csv)"));
  csvInvoicesFilename = filename.toStdString();
  findChild<QLabel*>("invoice_label")->setText(csvInvoicesFilename.c_str());
}

void MainWindow::on_pushButton_4_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Таблица с квитанциями об оплате"), "~/", tr("Text table files (*.csv)"));
  csvPaymentsFilename = filename.toStdString();
  findChild<QLabel*>("payment_label")->setText(csvPaymentsFilename.c_str());
}

void MainWindow::on_radioButton_7_clicked() {
  former.purpose = Purpose::Supervisor;
}

void MainWindow::on_radioButton_8_clicked() {
  former.purpose = Purpose::DepartamentHead;
}

void MainWindow::on_radioButton_9_clicked() {
  former.purpose = Purpose::Director;
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date) {
    filter.date = Date(date.day(), date.month(), date.year());
}

void MainWindow::on_dep_number_field_textChanged(const QString &arg1) {
  if(!arg1.contains(QRegExp("[^0-9]"))) {
    former.departamentNumber = arg1.toInt();
  } else {
      QLineEdit *line = static_cast<QLineEdit*>(QObject::sender());
      string newText(line->text().toStdString());

      newText.pop_back();
      line->setText(newText.c_str());
  }
}

void MainWindow::on_supervisor_field_textChanged(const QString &arg1){
  former.supervisor = arg1.toStdString();
}

