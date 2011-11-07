#include <QtGui>
#include <QtSql>

#include "customerform.h"
#include "customermodel.h"

CustomerModel::CustomerModel()
{
    setWindowIcon(QPixmap(":/images/customer.png"));
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Customers"));

    initializeModel();
    createView();
    createButtons();

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(view);
    box->addWidget(buttonBox);
    setLayout(box);
}

QSize CustomerModel::sizeHint() const
{
    return QSize(72 * fontMetrics().width('x'),
                 25 * fontMetrics().lineSpacing());
}

void CustomerModel::initializeModel()
{
    model = new QSqlTableModel(this);

    model->setTable("customer");
    model->select();

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setSort(Customer_Name, Qt::AscendingOrder);
    model->setHeaderData(Customer_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(Customer_Surname, Qt::Horizontal, tr("Surname"));
    model->setHeaderData(Customer_Email, Qt::Horizontal, tr("Email"));
    model->setHeaderData(Customer_Address, Qt::Horizontal, tr("Address"));
    model->setHeaderData(Customer_Phone, Qt::Horizontal, tr("Phone"));
}

void CustomerModel::createButtons()
{
    deleteButton = new QPushButton(tr("&Delete"));
    editButton = new QPushButton(tr("&Edit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRecord()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editRecords()));
}

void CustomerModel::createView()
{
    view = new QTableView;
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setColumnHidden(Customer_Customer_id, true);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setSortingEnabled(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void CustomerModel::deleteRecord()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = model->record(index.row());

    int r = QMessageBox::warning(this, tr("Delete Record"),
                                tr("Delete Customer %1 %2 ?")
                                .arg(record.value(Customer_Name).toString(),
                                    record.value(Customer_Surname).toString()),
                                QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
        QSqlDatabase::database().rollback();
        return;
    }

    model->removeRow(index.row());
    model->submitAll();
    QSqlDatabase::database().commit();
}

void CustomerModel::editRecords()
{
    int id = -1;
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = model->record(index.row());
        id = record.value(Customer_Customer_id).toInt();
    }

    CustomerForm form(model, id, this);
    form.exec();
    model->submitAll();
}


