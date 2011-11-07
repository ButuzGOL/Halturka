#include <QtGui>
#include <QtSql>

#include "../ui_statistikmodel.h"
#include "statistikmodel.h"

StatistikModel::StatistikModel() : ui(new Ui::StatistikModel)
{
    ui->setupUi(this);

    dialog = new QDialog(this);
    model = new QSqlQueryModel(this);
    view = new QTableView;
    layout = new QVBoxLayout;

    ui->dateStartEdit->setDate(QDate::currentDate());
    ui->dateFinishEdit->setDate(QDate::currentDate());

    ui->workIdComboBox->setVisible(false);

    QSqlQuery query;
    query.exec("SELECT name, work_id FROM work");

    QString name, id;
    while (query.next()) {
        name = query.value(0).toString();
        id = query.value(1).toString();
        ui->workComboBox->addItem(name);
        ui->workIdComboBox->addItem(id);
    }

    connect(ui->emailButton, SIGNAL(clicked()), this,
            SLOT(emailCustomerAction()));

    connect(ui->dateButton, SIGNAL(clicked()), this,
            SLOT(datePaymentAction()));

    connect(ui->workCountButton, SIGNAL(clicked()), this,
            SLOT(worksCountAction()));

    connect(ui->worksCountButton, SIGNAL(clicked()), this,
            SLOT(workCountAction()));

    connect(ui->allButton, SIGNAL(clicked()), this,
            SLOT(workAllTrueAction()));

    connect(ui->immediatelyButton, SIGNAL(clicked()), this,
            SLOT(workPaymentDateSameAction()));

    connect(ui->unionButton, SIGNAL(clicked()), this,
            SLOT(unionCustomerWorkerAction()));

    connect(ui->updateButton, SIGNAL(clicked()), this,
            SLOT(updateWorkTrueAction()));

    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->resizeColumnsToContents();
    view->setSortingEnabled(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(view);

    dialog->setLayout(layout);
}

void StatistikModel::emailCustomerAction()
{
    QString email = ui->emailComboBox->currentText();

    dialog->setWindowTitle(tr("Select email"));

    model->setQuery("SELECT name AS Name, surname AS Surname FROM customer"
                    " WHERE email LIKE '%" + email + "'");

    dialog->show();
}

void StatistikModel::datePaymentAction()
{
    QString dateStart = ui->dateStartEdit->date().toString("yyyy-MM-dd");
    QString dateFinish = ui->dateFinishEdit->date().toString("yyyy-MM-dd");

    dialog->setWindowTitle(tr("Select date payments"));
    model->setQuery("SELECT name AS Name, surname AS Surname FROM customer"
                    " WHERE customer_id IN (SELECT customer_id FROM payment"
                    " WHERE date BETWEEN '" + dateStart + "'"
                    " AND '" + dateFinish + "')");
    dialog->show();
}

void StatistikModel::worksCountAction()
{
    QString id = ui->workIdComboBox->itemText(ui->workComboBox->currentIndex());

    dialog->setWindowTitle(tr("Select count workers on some work"));
    model->setQuery("SELECT count(*) FROM work_worker WHERE work_id=" + id);
    dialog->show();
}

void StatistikModel::workCountAction()
{
    dialog->setWindowTitle(tr("Select count workers on works"));
    model->setQuery("SELECT work.name AS Name, count(*) FROM work, work_worker"
                    " WHERE work.work_id=work_worker.work_id GROUP BY"
                    " work.name");
    dialog->show();
}

void StatistikModel::workAllTrueAction()
{
    dialog->setWindowTitle(tr("Select if all works have status true"));
    model->setQuery("SELECT count(*) FROM work WHERE"
                    " status=all(SELECT status FROM work) AND status=true");
    dialog->show();
}

void StatistikModel::workPaymentDateSameAction()
{
    dialog->setWindowTitle(tr("Select works that were not paid immediately"));
    model->setQuery("SELECT name AS Name FROM work WHERE work_id IN"
                    " (SELECT work_id FROM payment WHERE work_id=work.work_id"
                    " AND date<>work.date_start)");
    dialog->show();
}

void StatistikModel::unionCustomerWorkerAction()
{
    dialog->setWindowTitle(tr("Union customer and worker table"));
    model->setQuery("SELECT name AS Name, surname AS Surname, 'customer'"
                    " AS Type FROM customer UNION SELECT name AS Name,"
                    " surname AS Surname, 'worker' FROM worker");
    dialog->show();
}

void StatistikModel::updateWorkTrueAction()
{
    QSqlQuery query;
    query.exec("UPDATE work SET status=true WHERE work_id IN"
               " (SELECT work_id FROM payment WHERE worker_id > 0"
               " AND price > 0)");
}
