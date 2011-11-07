#include <QtGui>
#include <QtSql>

#include "../ui_workform.h"
#include "workform.h"
#include "workmodel.h"

WorkForm::WorkForm(QSqlRelationalTableModel *modelCome, int id, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::WorkDialog)
{
    ui->setupUi(this);

    model = modelCome;

    QSqlTableModel *relModel = model->relationModel(Work_Customer_id);
    ui->customerComboBox->setModel(relModel);
    ui->customerComboBox->setModelColumn(relModel->fieldIndex("surname"));

    ui->priceEdit->setValidator(new QIntValidator(0, 999999, this));

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui->nameEdit, Work_Name);
    mapper->addMapping(ui->customerComboBox, Work_Customer_id);
    mapper->addMapping(ui->statusCheckBox, Work_Status);
    mapper->addMapping(ui->dateStartEdit, Work_Date_start);
    mapper->addMapping(ui->dateFinishEdit, Work_Date_finish);
    mapper->addMapping(ui->priceEdit, Work_Price);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    if (id != -1) {
        for (int row = 0; row < model->rowCount(); ++row) {
            QSqlRecord record = model->record(row);
            if (record.value(Work_Work_id).toInt() == id) {
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

void WorkForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

void WorkForm::addRecord()
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
    ui->statusCheckBox->setChecked(false);
    ui->dateStartEdit->setDate(QDate::currentDate());
    ui->dateFinishEdit->setDate(QDate::currentDate().addDays(5));
    ui->priceEdit->clear();

    ui->nameEdit->setFocus();
}

void WorkForm::deleteRecord()
{
    int row = mapper->currentIndex();
    model->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, model->rowCount() - 1));
}

void WorkForm::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
    ui->firstButton->setEnabled(row > 0);
    ui->lastButton->setEnabled(row < model->rowCount() - 1);
}


