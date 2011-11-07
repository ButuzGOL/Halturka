#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void openModelCustomer();
    void openModelWorker();
    void openModelWork();
    void openModelPayment();
    void openModelStatistik();
    void about();
    void preferenceDialog();

private:
    Ui::MainWindow *ui;
    QMdiSubWindow *subWindowCustomer;
    QMdiSubWindow *subWindowWorker;
    QMdiSubWindow *subWindowWork;
    QMdiSubWindow *subWindowPayment;
    QMdiSubWindow *subWindowStatistik;
    void customize();
};

#endif // MAINWINDOW_H
