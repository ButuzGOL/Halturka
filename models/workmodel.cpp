#include <QtGui>
#include <QtSql>

#include "materialmodel.h"
#include "workermodel.h"

#include "workmodel.h"
#include "workform.h"

WorkModel::WorkModel()
{
    setWindowIcon(QPixmap(":/images/work.png"));
    setWindowTitle(tr("Works"));
    setAttribute(Qt::WA_DeleteOnClose);

    initializeModel();
    createButtons();
    createView();

    leftSplitter = new QSplitter(Qt::Horizontal);
    leftSplitter->addWidget(materialPanel);
    leftSplitter->addWidget(workerPanel);
    leftSplitter->setStretchFactor(2, 1);

    splitter = new QSplitter(Qt::Vertical);
    splitter->setFrameStyle(QFrame::StyledPanel);
    splitter->addWidget(panel);
    splitter->addWidget(leftSplitter);

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(splitter);
    setLayout(box);

    view->setCurrentIndex(model->index(0, 0));
}

QSize WorkModel::sizeHint() const
{
    return QSize(72 * fontMetrics().width('x'),
                 25 * fontMetrics().lineSpacing());
}

void WorkModel::initializeModel()
{
    model = new QSqlRelationalTableModel(this);

    model->setTable("work");

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setRelation(Work_Customer_id,
                       QSqlRelation("customer", "customer_id", "surname"));
    model->select();
    model->setSort(Work_Name, Qt::AscendingOrder);
    model->setHeaderData(Work_Customer_id, Qt::Horizontal, tr("Customer"));
    model->setHeaderData(Work_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(Work_Status, Qt::Horizontal, tr("Status"));
    model->setHeaderData(Work_Date_start, Qt::Horizontal, tr("Date start"));
    model->setHeaderData(Work_Date_finish, Qt::Horizontal, tr("Date finish"));
    model->setHeaderData(Work_Price, Qt::Horizontal, tr("Price"));

    materialModel = new QSqlTableModel(this);
    materialModel->setTable("material");
    materialModel->setSort(Material_Name, Qt::AscendingOrder);
    materialModel->setHeaderData(Material_Name, Qt::Horizontal, tr("Name"));
    materialModel->setHeaderData(Material_Count, Qt::Horizontal, tr("Count"));
    materialModel->setHeaderData(Material_Price, Qt::Horizontal, tr("Price"));

    workerModel = new QSqlRelationalTableModel(this);
    workerModel->setTable("work_worker");
    workerModel->setRelation(Work_Worker_Worker_id,
                       QSqlRelation("worker", "worker_id", "surname"));

    workerModel->setSort(Work_Worker_Worker_id, Qt::AscendingOrder);
    workerModel->setHeaderData(Work_Worker_Worker_id, Qt::Horizontal,
                               tr("Surname"));
}

void WorkModel::createButtons()
{
    deleteButton = new QPushButton(tr("&Delete"));
    editButton = new QPushButton(tr("&Edit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRecord()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editRecords()));

    materialAddButton = new QPushButton(tr("Add &material"));
    materialDeleteButton = new QPushButton(tr("Delete m&aterial"));

    materialButtonBox = new QDialogButtonBox;
    materialButtonBox->addButton(materialAddButton,
                                 QDialogButtonBox::ActionRole);
    materialButtonBox->addButton(materialDeleteButton,
                                 QDialogButtonBox::ActionRole);

    connect(materialAddButton, SIGNAL(clicked()), this,
            SLOT(materialAddRecord()));
    connect(materialDeleteButton, SIGNAL(clicked()), this,
            SLOT(materialDeleteRecords()));

    workerAddButton = new QPushButton(tr("Add &worker"));
    workerDeleteButton = new QPushButton(tr("Delete w&orker"));

    workerButtonBox = new QDialogButtonBox;
    workerButtonBox->addButton(workerAddButton,
                                 QDialogButtonBox::ActionRole);
    workerButtonBox->addButton(workerDeleteButton,
                                 QDialogButtonBox::ActionRole);

    connect(workerAddButton, SIGNAL(clicked()), this,
            SLOT(workerAddRecord()));
    connect(workerDeleteButton, SIGNAL(clicked()), this,
            SLOT(workerDeleteRecords()));
}

void WorkModel::createView()
{
    panel = new QWidget;

    view = new QTableView;
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setColumnHidden(Work_Work_id, true);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setSortingEnabled(true);
    view->setItemDelegate(new QSqlRelationalDelegate(this));
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(view->selectionModel(),
            SIGNAL(currentRowChanged(const QModelIndex &,
                                     const QModelIndex &)),
            this, SLOT(updateMaterialAndWorkerView()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(view);
    layout->addWidget(buttonBox);
    panel->setLayout(layout);

    materialPanel = new QWidget;

    materialView = new QTableView;
    materialView->setModel(materialModel);
    materialView->setSelectionMode(QAbstractItemView::SingleSelection);
    materialView->setSelectionBehavior(QAbstractItemView::SelectRows);
    materialView->horizontalHeader()->setStretchLastSection(true);
    materialView->setColumnHidden(Material_Material_id, true);
    materialView->setColumnHidden(Material_Work_id, true);

    materialLabel = new QLabel(tr("Materials"));
    materialLabel->setBuddy(materialView);

    layout = new QVBoxLayout;
    layout->addWidget(materialLabel);
    layout->addWidget(materialView);
    layout->addWidget(materialButtonBox);
    materialPanel->setLayout(layout);

    workerPanel = new QWidget;

    workerView = new QTableView;
    workerView->setModel(workerModel);
    workerView->setSelectionMode(QAbstractItemView::SingleSelection);
    workerView->setSelectionBehavior(QAbstractItemView::SelectRows);
    workerView->horizontalHeader()->setStretchLastSection(true);
    workerView->setColumnHidden(0, true);
    workerView->setColumnHidden(Work_Worker_Work_id, true);
    workerView->setItemDelegate(new QSqlRelationalDelegate(this));

    connect(workerView->selectionModel(),
            SIGNAL(currentColumnChanged(const QModelIndex &,
                                        const QModelIndex &)),
            this, SLOT(updateWorkerPayment()));

    workerLabel = new QLabel(tr("Workers"));
    workerLabel->setBuddy(workerView);

    layout = new QVBoxLayout;
    layout->addWidget(workerLabel);
    layout->addWidget(workerView);
    layout->addWidget(workerButtonBox);
    workerPanel->setLayout(layout);
}

void WorkModel::deleteRecord()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = model->record(index.row());

    int r = QMessageBox::warning(this, tr("Delete Record"),
                                tr("Delete Work %1 ?")
                                .arg(record.value(Work_Name).toString()),
                                QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
        QSqlDatabase::database().rollback();
        return;
    }

    model->removeRow(index.row());
    model->submitAll();
    QSqlDatabase::database().commit();
}

void WorkModel::editRecords()
{
    int id = -1;
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = model->record(index.row());
        id = record.value(Work_Work_id).toInt();
    }

    WorkForm form(model, id, this);
    form.exec();
    model->submitAll();
}

void WorkModel::updateMaterialAndWorkerView()
{
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = model->record(index.row());
        int id = record.value("work_id").toInt();
        materialModel->setFilter(QString("work_id = %1").arg(id));
        materialLabel->setText(tr("Materials in the %1 Work")
                               .arg(record.value("name").toString()));
        workerModel->setFilter(QString("work_id = %1").arg(id));
        workerLabel->setText(tr("Workers in the %1 Work")
                               .arg(record.value("name").toString()));
    } else {
        materialModel->setFilter("work_id = -1");
        materialLabel->setText(tr("Materials"));
        workerModel->setFilter("work_id = -1");
        workerLabel->setText(tr("Workers"));
    }
    materialModel->select();
    workerModel->select();
}

void WorkModel::materialAddRecord()
{
    QModelIndex index = view->currentIndex();
    QSqlRecord record = model->record(index.row());
    QVariant id = record.value("work_id");

    record = materialModel->record();
    record.setValue(Material_Work_id, id);

    materialModel->insertRecord(-1, record);
    int row = materialModel->rowCount();
    index = materialModel->index(row - 1, Material_Name);

    materialView->setCurrentIndex(index);
    materialView->edit(index);
}

void WorkModel::materialDeleteRecords()
{
    QModelIndex index = materialView->currentIndex();
    if (!index.isValid())
        return;

    materialModel->removeRow(index.row());
    materialModel->submitAll();
}

void WorkModel::workerAddRecord()
{
    QModelIndex index = view->currentIndex();
    QSqlRecord record = model->record(index.row());
    QVariant id = record.value("work_id");

    record = workerModel->record();
    record.setValue(Work_Worker_Work_id, id);

    workerModel->insertRecord(-1, record);
    int row = workerModel->rowCount();
    index = workerModel->index(row - 1, Work_Worker_Worker_id);

    workerView->setCurrentIndex(index);
    workerView->edit(index);
}

void WorkModel::workerDeleteRecords()
{
    QModelIndex index = workerView->currentIndex();
    if (!index.isValid())
        return;

    workerModel->removeRow(index.row());
    workerModel->submitAll();

    index = view->currentIndex();
    QSqlRecord record = model->record(index.row());
    QString id = record.value("work_id").toString();

    QSqlQuery query;
    query.exec("SELECT payment_id FROM payment WHERE work_id=" + id +
               " AND worker_id NOT IN (SELECT worker_id FROM work_worker"
               " WHERE work_id=" + id + ")");


    while (query.next()) {
        id = query.value(0).toString();
        query.exec("DELETE FROM payment WHERE payment_id=" + id);
    }
}

void WorkModel::updateWorkerPayment()
{
    QModelIndex index = view->currentIndex();
    QSqlRecord record = model->record(index.row());
    QString id = record.value("work_id").toString();

    QSqlQuery query;
    query.exec("SELECT worker_id FROM work_worker WHERE work_id=" + id);

    int a[query.size()], i = 0, idd, k, j = 0;
    for (i = 0; i < query.size(); i++)
        a[i] = 0;
    i = 0;
    QString ids;
    QSqlQuery query1;
    while (query.next()) {
        idd = query.value(0).toInt();
        ids = query.value(0).toString();

        for (j = 0, k = 0; j < i; j++) {

            if (idd == a[j]) {
                query1.exec("SELECT work_worker_id FROM work_worker WHERE "
                            "work_id=" + id + " AND worker_id=" + ids +
                            " LIMIT 1");
                while (query1.next()) {
                    ids = query1.value(0).toString();
                }

                query1.exec("DELETE FROM work_worker WHERE work_worker_id="
                            + ids);    
                k = 1;
                break;
            }
        }
        if (!k) {
            query1.exec("SELECT * FROM payment WHERE "
                        "work_id=" + id + " AND worker_id=" + ids);

            if (query1.size() == 0) {
                query1.exec("INSERT INTO payment (worker_id, work_id) VALUES "
                           "(" + ids + ", " + id + ")");

            }
            a[i] = idd;
            i++;
        }
    }
}
