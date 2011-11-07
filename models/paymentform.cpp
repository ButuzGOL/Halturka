#include <QtGui>
#include <QtSql>

#include "../ui_paymentform.h"
#include "paymentform.h"
#include "paymentmodel.h"

PaymentForm::PaymentForm(QSqlRelationalTableModel *modelCome, int id,
                         QWidget *parent)
    : QDialog(parent),
    ui(new Ui::PaymentDialog)
{
    ui->setupUi(this);

    model = modelCome;

    QSqlTableModel *relModel = model->relationModel(Payment_Customer_id);
    ui->customerComboBox->setModel(relModel);
    ui->customerComboBox->setModelColumn(relModel->fieldIndex("surname"));

    relModel = model->relationModel(Payment_Work_id - 1);
    ui->workComboBox->setModel(relModel);
    ui->workComboBox->setModelColumn(relModel->fieldIndex("name"));

    ui->priceEdit->setValidator(new QIntValidator(0, 999999, this));

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->addMapping(ui->customerComboBox, Payment_Customer_id);
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
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addRecord()));
    connect(ui->deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteRecord()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(mapper, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateButtons(int)));
    connect(ui->workComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateFields(int)));

    if (id == -1) {
        ui->nextButton->setFocus();
    } else {
        ui->customerComboBox->setFocus();
    }
    int row = mapper->currentIndex();
    updateButtons(row);

    if (row == -1) {
        addRecord();
    }
}

void PaymentForm::done(int result)
{
    mapper->submit();
    QDialog::done(result);
}

void PaymentForm::addRecord()
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

    ui->dateEdit->setDate(QDate::currentDate());
    ui->priceEdit->clear();

    ui->customerComboBox->setFocus();
}

void PaymentForm::deleteRecord()
{
    int row = mapper->currentIndex();
    model->removeRow(row);
    mapper->submit();
    mapper->setCurrentIndex(qMin(row, model->rowCount() - 1));
}

void PaymentForm::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
    ui->firstButton->setEnabled(row > 0);
    ui->lastButton->setEnabled(row < model->rowCount() - 1);
}

void PaymentForm::updateFields(int index)
{
    QSqlQuery query;
    query.exec("SELECT customer_id, price FROM work");

    QString price;
    int id, k = 0;
    while (query.next()) {
        if (k == index) {
            id = query.value(0).toInt();
            price = query.value(1).toString();
            break;
        }
        ++k;
    }

    query.exec("SELECT customer_id FROM customer");
    index = 0;
    while (query.next()) {
        if (query.value(0).toInt() == id) {
            break;
        }
        ++index;
    }

    ui->priceEdit->setText(price);
    ui->customerComboBox->setCurrentIndex(index);
}
