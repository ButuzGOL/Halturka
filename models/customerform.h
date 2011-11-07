#ifndef CUSTOMERFORM_H
#define CUSTOMERFORM_H

#include <QDialog>

class QSqlTableModel;
class QDataWidgetMapper;

namespace Ui {
    class CustomerDialog;
}

class CustomerForm : public QDialog
{
    Q_OBJECT

public:
    CustomerForm(QSqlTableModel *modelCome, int id, QWidget *parent = 0);
    void done(int result);

private slots:
    void addRecord();
    void deleteRecord();
    void updateButtons(int row);

private:
    Ui::CustomerDialog *ui;
    QDataWidgetMapper *mapper;
    QSqlTableModel *model;

};

#endif // CUSTOMERFORM_H
