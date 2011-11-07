#ifndef WORKFORM_H
#define WORKFORM_H

#include <QDialog>

class QSqlRelationalTableModel;
class QDataWidgetMapper;

namespace Ui {
    class WorkDialog;
}

class WorkForm : public QDialog
{
    Q_OBJECT

public:
    WorkForm(QSqlRelationalTableModel *modelCome, int id, QWidget *parent = 0);
    void done(int result);

private slots:
    void addRecord();
    void deleteRecord();
    void updateButtons(int row);

private:
    Ui::WorkDialog *ui;
    QDataWidgetMapper *mapper;
    QSqlRelationalTableModel *model;

};

#endif // WORKERFORM_H

