#ifndef PAYMENTFORM_H
#define PAYMENTFORM_H

#include <QDialog>

class QSqlRelationalTableModel;
class QDataWidgetMapper;

namespace Ui {
    class PaymentDialog;
}

class PaymentForm : public QDialog
{
    Q_OBJECT

public:
    PaymentForm(QSqlRelationalTableModel *modelCome, int id,
                QWidget *parent = 0);
    void done(int result);

private slots:
    void addRecord();
    void deleteRecord();
    void updateButtons(int row);
    void updateFields(int row);

private:
    Ui::PaymentDialog *ui;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *model;

};

#endif // PAYMENTFORM_H
