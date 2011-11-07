#include <QtGui>
#include <QtSql>

#include "../ui_paymentworkerform.h"
#include "paymentworkerform.h"
#include "paymentmodel.h"

PaymentWorkerForm::PaymentWorkerForm(QSqlRelationalTableModel *modelCome,
                                     int id, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::PaymentWorkerDialog)
{
    ui->setupUi(this);

    model = modelCome;

    QSqlTableModel *relModel = model->relationModel(Payment_Worker_id - 1);
    ui->workerComboBox->setModel(relModel);
    ui->workerComboBox->setModelColumn(relModel->fieldIndex("surname"));

    relModel = model->relationModel(Payment_Work_id - 1);
    ui->workComboBox->setModel(relModel);
    ui->workComboBox->setModelColumn(relModel->fieldIndex("name"));

    ui->priceEdit->setValidator(new QIntValidator(0, 999999, this));

    ui->dateEdit->setDate(QDate::currentDate());

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui->workerComboBox, Payment_Worker_id - 1);
    mapper->addMapping(ui->workComboBox, Payment_Work_id - 1);
    mapper->addMapping(ui->dateEdit, Payment_Date - 1);
    mapper->addMapping(ui->priceEdit, Payment_Price - 1);
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(this));

    if (id != -1) {
        for (int row = 0; row < model->rowCount(); ++row) {
            QSqlRecord record = model->record(row);
            if (record.value(Payment_Payment_id).toInt() == id) {
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
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(mapper, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateButtons(int)));

    if (id == -1) {
        ui->nextButton->setFocus();
    } else {
        ui->priceEdit->setFocus();
    }
    int row = mapper->currentIndex();
    updateButtons(row);

}

void PaymentWorkerForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}


void PaymentWorkerForm::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
    ui->firstButton->setEnabled(row > 0);
    ui->lastButton->setEnabled(row < model->rowCount() - 1);
}
