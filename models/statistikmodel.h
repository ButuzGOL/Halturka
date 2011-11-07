#ifndef STATISTIKMODEL_H
#define STATISTIKMODEL_H

#include <QWidget>

class QLabel;
class QComboBox;
class QPushButton;
class QDateEdit;
class QDialog;
class QSqlQueryModel;
class QTableView;
class QVBoxLayout;

namespace Ui {
    class StatistikModel;
}

class StatistikModel : public QWidget
{
    Q_OBJECT

public:
    StatistikModel();

private slots:
    void emailCustomerAction();
    void datePaymentAction();
    void worksCountAction();
    void workCountAction();
    void workAllTrueAction();
    void workPaymentDateSameAction();
    void unionCustomerWorkerAction();
    void updateWorkTrueAction();

private:
    QLabel *emailLabel;
    QComboBox *emailComboBox;
    QPushButton *emailButton;
    QDateEdit *dateStartEdit;
    QDateEdit *dateFinishEdit;
    QLabel *dateLabel;
    QPushButton *dateButton;
    Ui::StatistikModel *ui;
    QDialog *dialog;
    QSqlQueryModel *model;
    QTableView *view;
    QVBoxLayout *layout;
};

#endif // STATISTIKMODEL_H
