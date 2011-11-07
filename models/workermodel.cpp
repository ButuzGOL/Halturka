#include <QtGui>
#include <QtSql>

#include "workermodel.h"
#include "workerform.h"

WorkerModel::WorkerModel()
{
    setWindowIcon(QPixmap(":/images/worker.png"));
    setWindowTitle(tr("Workers"));
    setAttribute(Qt::WA_DeleteOnClose);

    initializeModel();
    createView();
    createButtons();

    QVBoxLayout *box = new QVBoxLayout;
    box->addWidget(view);
    box->addWidget(buttonBox);
    setLayout(box);
}

QSize WorkerModel::sizeHint() const
{
    return QSize(72 * fontMetrics().width('x'),
                 25 * fontMetrics().lineSpacing());
}

void WorkerModel::initializeModel()
{
    model = new QSqlTableModel(this);

    model->setTable("worker");
    model->select();

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    model->setSort(Worker_Name, Qt::AscendingOrder);
    model->setHeaderData(Worker_Name, Qt::Horizontal, tr("Name"));
    model->setHeaderData(Worker_Surname, Qt::Horizontal, tr("Surname"));
    model->setHeaderData(Worker_Email, Qt::Horizontal, tr("Email"));
    model->setHeaderData(Worker_Address, Qt::Horizontal, tr("Address"));
    model->setHeaderData(Worker_Phone, Qt::Horizontal, tr("Phone"));
}

void WorkerModel::createButtons()
{
    deleteButton = new QPushButton(tr("&Delete"));
    editButton = new QPushButton(tr("&Edit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(editButton, QDialogButtonBox::ActionRole);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteRecord()));
    connect(editButton, SIGNAL(clicked()), this, SLOT(editRecords()));
}

void WorkerModel::createView()
{
    view = new QTableView;
    view->setModel(model);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setColumnHidden(Worker_Worker_id, true);
    view->resizeColumnsToContents();
    view->horizontalHeader()->setStretchLastSection(true);
    view->setSortingEnabled(true);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void WorkerModel::deleteRecord()
{
    QModelIndex index = view->currentIndex();
    if (!index.isValid())
        return;

    QSqlDatabase::database().transaction();
    QSqlRecord record = model->record(index.row());

    int r = QMessageBox::warning(this, tr("Delete Record"),
                                tr("Delete Worker %1 %2 ?")
                                .arg(record.value(Worker_Name).toString(),
                                    record.value(Worker_Surname).toString()),
                                QMessageBox::Yes | QMessageBox::No);
    if (r == QMessageBox::No) {
        QSqlDatabase::database().rollback();
        return;
    }

    model->removeRow(index.row());
    model->submitAll();
    QSqlDatabase::database().commit();
}

void WorkerModel::editRecords()
{
    int id = -1;
    QModelIndex index = view->currentIndex();
    if (index.isValid()) {
        QSqlRecord record = model->record(index.row());
        id = record.value(Worker_Worker_id).toInt();
    }

    WorkerForm form(model, id, this);
    form.exec();
    model->submitAll();
}
