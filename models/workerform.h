#ifndef WORKERFORM_H
#define WORKERFORM_H

#include <QDialog>

class QSqlTableModel;
class QDataWidgetMapper;

namespace Ui {
    class WorkerDialog;
}

class WorkerForm : public QDialog
{
    Q_OBJECT

public:
    WorkerForm(QSqlTableModel *modelCome, int id, QWidget *parent = 0);
    void done(int result);

private slots:
    void addRecord();
    void deleteRecord();
    void updateButtons(int row);

private:
    Ui::WorkerDialog *ui;
    QDataWidgetMapper *mapper;
    QSqlTableModel *model;

};

#endif // WORKERFORM_H
