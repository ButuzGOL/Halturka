#ifndef WORKMODEL_H
#define WORKMODEL_H

#include <QWidget>

class QSqlRelationalTableModel;
class QPushButton;
class QTableView;
class QDialogButtonBox;
class QSplitter;
class QLabel;
class QSqlTableModel;

enum {
    Work_Work_id = 0,
    Work_Customer_id = 1,
    Work_Name = 2,
    Work_Status = 3,
    Work_Date_start = 4,
    Work_Date_finish = 5,
    Work_Price = 6
};

class WorkModel : public QWidget
{
    Q_OBJECT

public:
    WorkModel();

private slots:
    void deleteRecord();
    void editRecords();
    void updateMaterialAndWorkerView();
    void materialAddRecord();
    void materialDeleteRecords();
    void workerAddRecord();
    void workerDeleteRecords();
    void updateWorkerPayment();

private:
    QSize sizeHint() const;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QTableView *view;
    QTableView *materialView;
    QTableView *workerView;
    QSqlRelationalTableModel *model;
    QSqlTableModel *materialModel;
    QSqlRelationalTableModel *workerModel;
    QDialogButtonBox *buttonBox;
    QSplitter *splitter;
    QSplitter *leftSplitter;
    QWidget *panel;
    QWidget *materialPanel;
    QWidget *workerPanel;
    QPushButton *materialDeleteButton;
    QPushButton *materialAddButton;
    QDialogButtonBox *materialButtonBox;
    QLabel *materialLabel;
    QPushButton *workerDeleteButton;
    QPushButton *workerAddButton;
    QDialogButtonBox *workerButtonBox;
    QLabel *workerLabel;
    void initializeModel();
    void createView();
    void createButtons();
};

#endif // WORKMODEL_H
