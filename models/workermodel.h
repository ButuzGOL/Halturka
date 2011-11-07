#ifndef WORKERMODEL_H
#define WORKERMODEL_H

#include <QWidget>

class QSqlTableModel;
class QPushButton;
class QTableView;
class QDialogButtonBox;

enum {
    Worker_Worker_id = 0,
    Worker_Name = 1,
    Worker_Surname = 2,
    Worker_Email = 3,
    Worker_Address = 4,
    Worker_Phone = 5,

    Work_Worker_Work_id = 1,
    Work_Worker_Worker_id = 2
};

class WorkerModel : public QWidget
{
    Q_OBJECT

public:
    WorkerModel();

private slots:
    void deleteRecord();
    void editRecords();

private:
    QSize sizeHint() const;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QTableView *view;
    QSqlTableModel *model;
    QDialogButtonBox *buttonBox;
    void initializeModel();
    void createView();
    void createButtons();
};

#endif // WORKERMODEL_H
