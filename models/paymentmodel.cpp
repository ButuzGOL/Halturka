#include <QtGui>
#include <QtSql>

#include "paymentmodel.h"
#include "paymentform.h"
#include "paymentworkerform.h"

PaymentModel::PaymentModel()
{
    setWindowIcon(QPixmap(":/images/payment.png"));
    setWindowTitle(tr("Payments"));
    setAttribute(Qt::WA_DeleteOnClose);

    initializeModel();
    createButtons();
    createView();

    splitter = new QSplitter(Qt::Horizontal);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(panel);
    splitter->addWidget(workerPanel);

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(splitter);
    setLayout(box);

    view->setCurrentIndex(model->index(0, 0));
}

QSize PaymentModel::sizeHint() const
{
    return QSize(72 * fontMetrics().width('x'),
                 25 * fontMetrics().lineSpacing());
}

void PaymentModel::initializeModel()
{
    model = new QSqlRelationalTableModel(this);

    model->setTable("payment");

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setRelation(Payment_Customer_id,
                       QSqlRelation("customer", "customer_id", "surname"));

    model->setRelation(Payment_Work_id,
                       QSqlRelation("work", "work_id", "name"));
    model->select();
    model->setSort(Payment_Date, Qt::AscendingOrder);
    model->setHeaderData(Payment_Customer_id, Qt::Horizontal, tr("Customer"));
    model->setHeaderData(Payment_Work_id - 1, Qt::Horizontal, tr("Work"));
    model->setHeaderData(Payment_Date - 1, Qt::Horizontal, tr("Date"));
    model->setHeaderData(Payment_Price - 1, Qt::Horizontal, tr("Price"));
    model->removeColumn(Payment_Worker_id);

    workerModel = new QSqlRelationalTableModel(this);

    workerModel->setTable("payment");

    workerModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    workerModel->setRelation(Payment_Worker_id,
                       QSqlRelation("worker", "worker_id", "surname"));

    workerModel->setRelation(Payment_Work_id,
                       QSqlRelation("work", "work_id", "name"));
    workerModel->setFilter("work_id = -1");

    workerModel->select();

    workerModel->setSort(Payment_Date, Qt::AscendingOrder);
    workerModel->setHeaderData(Payment_Worker_id - 1, Qt::Horizontal,
                               tr("Worker"));
    workerModel->setHeaderData(Payment_Work_id - 1, Qt::Horizontal, tr("Work"));
    workerModel->setHeaderData(Payment_Date - 1, Qt::Horizontal, tr("Date"));
    workerModel->setHeaderData(Payment_Price - 1, Qt::Horizontal, tr("Pay"));
    workerModel->removeColumn(Payment_Customer_id);
}

void PaymentModel::createButtons()
{
    deleteButton = new QPushButton(tr("&Delete"));
    editButton = new QPushButton(tr("&Edit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRecord()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editRecords()));

    workerEditButton = new QPushButton(tr("E&dit workers payment"));
    workerEditButton->setEnabled(false);

    workerButtonBox = new QDialogButtonBox;
    workerButtonBox->addButton(workerEditButton, QDialogButtonBox::ActionRole);

    connect(workerEditButton, SIGNAL(clicked()), this, SLOT(workerEditRecords()));
}

void PaymentModel::createView()
{
    panel = new QWidget;

    view = new QTableView;
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setColumnHidden(Payment_Payment_id, true);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setSortingEnabled(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setItemDelegate(new QSqlRelationalDelegate(this));

    connect(view->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex &)),
            this, SLOT(updateWorkerView()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addWidget(buttonBox);
    panel->setLayout(layout);

    workerPanel = new QWidget;

    workerView = new QTableView;
    workerView->setModel(workerModel);
    workerView->setSelectionMode(QAbstractItemView::SingleSelection);

    workerView->setSelectionBehavior(QAbstractItemView::SelectRows);
    workerView->setColumnHidden(Payment_Payment_id, true);
    workerView->resizeColumnsToContents();
    workerView->horizontalHeader()->setStretchLastSection(true);
    workerView->setSortingEnabled(true);
    workerView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    workerView->setItemDelegate(new QSqlRelationalDelegate(this));

    workerLabel = new QLabel(tr("Workers payments"));
    workerLabel->setBuddy(workerView);

    layout = new QVBoxLayout;
    layout->addWidget(workerLabel);
    layout->addWidget(workerView);
    layout->addWidget(workerButtonBox);
    workerPanel->setLayout(layout);
}

void PaymentModel::deleteRecord()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = model->record(index.row());

    int r = QMessageBox::warning(this, tr("Delete Payment"),
                                tr("Delete Payment %1 ?")
                                .arg(record.value(Payment_Work_id - 1).
                                     toString()),
                                QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
        QSqlDatabase::database().rollback();
        return;
    }

    model->removeRow(index.row());
    model->submitAll();
    QSqlDatabase::database().commit();
    QSqlQuery query;
    QString id = record.value(0).toString();
    query.exec("DELETE FROM payment WHERE payment_id=" + id);
}

void PaymentModel::editRecords()
{
    int id = -1;
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = model->record(index.row());
        id = record.value(Payment_Payment_id).toInt();
    }

    PaymentForm form(model, id, this);
    form.exec();
    model->submitAll();
}

void PaymentModel::updateWorkerView()
{
    QModelIndex index = view->currentIndex();

    if (index.isValid()) {
        QSqlRecord record = model->record(index.row());
        QString id = record.value("payment_id").toString();

        QSqlQuery query;
        query.exec("SELECT work_id FROM payment WHERE customer_id > 0 AND "
                   "payment_id=" + id);

        while (query.next()) {
            id = query.value(0).toString();
            break;
        }
        query.exec("SELECT payment_id FROM payment WHERE worker_id > 0 AND "
                   "work_id=" + id);

        QString str = "";
        while (query.next()) {
            str += "payment_id=" + query.value(0).toString();
            str += " OR ";
        }

        if (str == "") {
            workerModel->setFilter("work_id = -1");
        }
        else {
            str.remove(str.length()-4, 4);
            workerModel->setFilter(str);
        }
        workerLabel->setText(tr("Workers payments in the %1 Work")
                               .arg(record.value(Payment_Work_id - 1).
                                    toString()));
    } else {
        workerModel->setFilter("work_id = -1");
        workerLabel->setText(tr("Workers payments"));
    }
    workerModel->select();
    if (workerModel->rowCount())
        workerEditButton->setEnabled(true);
    else
        workerEditButton->setEnabled(false);
}

void PaymentModel::workerEditRecords()
{
    int id = -1;
    QModelIndex index = workerView->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = workerModel->record(index.row());
        id = record.value(Payment_Payment_id).toInt();
    }

    PaymentWorkerForm workerForm(workerModel, id, this);
    workerForm.exec();
    workerModel->submitAll();
}

