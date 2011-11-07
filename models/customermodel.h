#ifndef CUSTOMERMODEL_H
#define CUSTOMERMODEL_H

#include <QWidget>

class QSqlTableModel;
class QPushButton;
class QTableView;
class QDialogButtonBox;

enum {
    Customer_Customer_id = 0,
    Customer_Name = 1,
    Customer_Surname = 2,
    Customer_Email = 3,
    Customer_Address = 4,
    Customer_Phone = 5
};

class CustomerModel : public QWidget
{
    Q_OBJECT

public:
    CustomerModel();

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

#endif // CUSTOMERMODEL_H

