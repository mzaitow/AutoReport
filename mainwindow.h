#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <unordered_map>
#include <functional>
#include <vector>
#include "offer.h"
#include "invoice.h"
#include "payment.h"
#include "filter.h"
#include "reportformer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void on_radioButton_2_clicked();
  void on_radioButton_3_clicked();
  void on_radioButton_clicked();
  void on_radioButton_4_clicked();
  void on_radioButton_5_clicked();
  void on_radioButton_6_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_clicked();
  void on_pushButton_2_clicked();
  void on_pushButton_4_clicked();
  void on_radioButton_7_clicked();
  void on_radioButton_8_clicked();
  void on_radioButton_9_clicked();
  void on_dateEdit_userDateChanged(const QDate &date);
  void on_dep_number_field_textChanged(const QString &arg1);
  void on_supervisor_field_textChanged(const QString &arg1);

private:
  std::unordered_map<int, Offer> m_offers;
  std::unordered_map<int, std::unordered_map<int, Invoice>> m_invoices;
  std::unordered_map<int, std::unordered_map<int, std::vector<Payment>>> m_payments;

  Filter filter;
  ReportFormer former;

  std::string csvOffersFilename;
  std::string csvInvoicesFilename;
  std::string csvPaymentsFilename;

  void readOffers();
  void readInvoices();
  void readPayments();

  void addPayment(const int ID_, const int offerID_, const int invoiceID_,
                  const int payment_, const Date date_, const int paymentTerm_);
  void addInvoice(const int ID_, const int offerID_, const int amount_,
                  const Date date_, const int paymentTerm_);

  void calcPaymentUnderOffers();
  void calcDeadlines();
  void calcStatus();
  void sumOffersPenalties();
  void calcTermDays();
  void calcOfferDeadlines(Offer &offer);
  void calcPenaltiesPaid();

  void getFilteredOutstandingOffers();

  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
