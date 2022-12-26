#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "csvparser.h"
#include "report.h"
#include <QErrorMessage>
#include <QFileDialog>
#include <iostream>
#include "date.h"

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

void MainWindow::readOffers() {
  CSVParser parser(csvOffersFilename.c_str());

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

    m_offers.insert({ id,
                      Offer(id, companyName, amount,
                            date, paymentDate, percent,
                            state, supervisor, depNumber) });

    parser.nextRow();
  }
}

void MainWindow::readInvoices() {
  CSVParser parser(csvInvoicesFilename.c_str());

  while(!parser.getCellContent().empty()) {
    const int ID = stoi(parser.getCellContent());
    const int offerID = stoi(parser.getForwardCellContent());
    const int invoice = stoi(parser.getForwardCellContent());
    const Date date = parseDate(parser.getForwardCellContent());
    const Date paymentDate = parseDate(parser.getForwardCellContent());

    if(m_invoices.find(offerID) == m_invoices.end()) {
      m_invoices.insert({
          offerID,
          unordered_map<int, Invoice>({ { ID, Invoice(ID, offerID, invoice, date, paymentDate) } })
        });
    } else {
      m_invoices[offerID].insert({ ID, Invoice(ID, offerID, invoice, date, paymentDate) });
    }

    parser.nextRow();
  }
}

void MainWindow::readPayments() {
  CSVParser parser(csvPaymentsFilename.c_str());

  while(!parser.getCellContent().empty()) {
    const int ID = stoi(parser.getCellContent());
    const int offerID = stoi(parser.getForwardCellContent());
    const int invoiceID = stoi(parser.getForwardCellContent());
    const int payment = stoi(parser.getForwardCellContent());
    const Date date = parseDate(parser.getForwardCellContent());

    if(m_payments.find(offerID) == m_payments.end()) {
      m_payments.insert({
          offerID,
          unordered_map<int, Payment>({ { ID, Payment(ID, offerID, invoiceID, payment, date) } })
        });
    } else {
      m_payments[offerID].insert({ ID, Payment(ID, offerID, invoiceID, payment, date) });
    }

    parser.nextRow();
  }
}

void MainWindow::calcPaymentUnderOffers() {
  for(auto &i : m_offers) {
    for(auto &j : m_payments[i.first]) {
      i.second.paid += j.second.payment;
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
  filter.filter();


  QString filename = QFileDialog::getSaveFileName(this, tr("Итоговая таблица"), "~/", tr("Text table files (*.csv)"));

  former.outputFilename = filename.toStdString();
  cout << "[ DBG ]: " << former.outputFilename << endl;
  former.reportForm();
}

void MainWindow::on_pushButton_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Таблица с договорами"), "~/", tr("Text table files (*.csv)"));
  csvOffersFilename = filename.toStdString();
  cout << "[ DBG ]: " << csvOffersFilename << endl;
}

void MainWindow::on_pushButton_2_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Таблица с счетами-фактурами"), "~/", tr("Text table files (*.csv)"));
  csvInvoicesFilename = filename.toStdString();
}

void MainWindow::on_pushButton_4_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, tr("Таблица с квитанциями об оплате"), "~/", tr("Text table files (*.csv)"));
  csvPaymentsFilename = filename.toStdString();
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

void MainWindow::on_lineEdit_4_textEdited(const QString &arg1) {
  former.departamentNumber = arg1.toInt();
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1) {
  former.supervisor = arg1.toStdString();
}

void MainWindow::on_dateEdit_userDateChanged(const QDate &date) {
    filter.date = Date(date.day(), date.month(), date.year());
}

