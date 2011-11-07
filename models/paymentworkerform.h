#ifndef PAYMENTWORKERFORM_H
#define PAYMENTWORKERFORM_H

#include <QDialog>

class QSqlRelationalTableModel;
class QDataWidgetMapper;

namespace Ui {
    class PaymentWorkerDialog;
}

class PaymentWorkerForm : public QDialog
{
    Q_OBJECT

public:
    PaymentWorkerForm(QSqlRelationalTableModel *modelCome, int id,
                QWidget *parent = 0);
    void done(int result);

private slots:
    void updateButtons(int row);

private:
    Ui::PaymentWorkerDialog *ui;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *model;

};

#endif // PAYMENTWORKERFORM_H
