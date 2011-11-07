#ifndef PAYMENTMODEL_H
#define PAYMENTMODEL_H

#include <QWidget>

class QSqlRelationalTableModel;
class QPushButton;
class QTableView;
class QDialogButtonBox;
class QSplitter;
class QLabel;

enum {
    Payment_Payment_id = 0,
    Payment_Customer_id = 1,
    Payment_Worker_id = 2,
    Payment_Work_id = 3,
    Payment_Date = 4,
    Payment_Price = 5
};

class PaymentModel : public QWidget
{
    Q_OBJECT

public:
    PaymentModel();

private slots:
    void deleteRecord();
    void editRecords();
    void updateWorkerView();
    void workerEditRecords();

private:
    QSize sizeHint() const;
    QPushButton *deleteButton;
    QPushButton *editButton;
    QPushButton *workerEditButton;
    QTableView *view;
    QTableView *workerView;
    QSqlRelationalTableModel *model;
    QSqlRelationalTableModel *workerModel;
    QDialogButtonBox *buttonBox;
    QDialogButtonBox *workerButtonBox;
    QWidget *panel;
    QWidget *workerPanel;
    QSplitter *splitter;
    QLabel *workerLabel;
    void initializeModel();
    void createView();
    void createButtons();
};

#endif // PAYMENTMODEL_H
