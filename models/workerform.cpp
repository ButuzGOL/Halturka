#include <QtGui>
#include <QtSql>

#include "../ui_workerform.h"
#include "workerform.h"
#include "workermodel.h"

WorkerForm::WorkerForm(QSqlTableModel *modelCome, int id, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::WorkerDialog)
{
    ui->setupUi(this);

    model = modelCome;

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui->nameEdit, Worker_Name);
    mapper->addMapping(ui->surnameEdit, Worker_Surname);
    mapper->addMapping(ui->emailEdit, Worker_Email);
    mapper->addMapping(ui->addressEdit, Worker_Address);
    mapper->addMapping(ui->phoneEdit, Worker_Phone);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    if (id != -1) {
        for (int row = 0; row < model->rowCount(); ++row) {
            QSqlRecord record = model->record(row);
            if (record.value(Worker_Worker_id).toInt() == id) {
                mapper->setCurrentIndex(row);
                break;
            }
        }
    } else {
        mapper->toFirst();
    }

    connect(ui->firstButton, SIGNAL(clicked()), mapper, SLOT(toFirst()));
    connect(ui->previousButton, SIGNAL(clicked()),
            mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    connect(ui->lastButton, SIGNAL(clicked()), mapper, SLOT(toLast()));
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addRecord()));
    connect(ui->deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteRecord()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(mapper, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateButtons(int)));

    if (id == -1) {
        ui->nextButton->setFocus();
    } else {
        ui->nameEdit->setFocus();
    }
    int row = mapper->currentIndex();
    updateButtons(row);

    if (row == -1) {
        addRecord();
    }
}

void WorkerForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

void WorkerForm::addRecord()
{
    int row = mapper->currentIndex();
    if (row != -1) {
        mapper->submit();
    }
    else {
        row = model->rowCount();
    }

    model->insertRow(row);
    mapper->setCurrentIndex(row);

    ui->nameEdit->clear();
    ui->surnameEdit->clear();
    ui->emailEdit->clear();
    ui->addressEdit->clear();
    ui->phoneEdit->clear();

    ui->nameEdit->setFocus();
}

void WorkerForm::deleteRecord()
{
    int row = mapper->currentIndex();
    model->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, model->rowCount() - 1));
}

void WorkerForm::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
    ui->firstButton->setEnabled(row > 0);
    ui->lastButton->setEnabled(row < model->rowCount() - 1);
}

