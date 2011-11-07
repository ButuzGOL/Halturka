#include <QtGui>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "models/customermodel.h"
#include "models/workermodel.h"
#include "models/workmodel.h"
#include "models/paymentmodel.h"
#include "models/statistikmodel.h"

#include "preferencedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openModelWork();

    customize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::openModelCustomer()
{
    if (ui->mdiArea->subWindowList().indexOf(subWindowCustomer) == -1)
    {
        CustomerModel *modelCustomer = new CustomerModel;
        subWindowCustomer = ui->mdiArea->addSubWindow(modelCustomer);
        subWindowCustomer->show();
    }
    else {
        ui->mdiArea->setActiveSubWindow(subWindowCustomer);
    }
}

void MainWindow::openModelWorker()
{
    if (ui->mdiArea->subWindowList().indexOf(subWindowWorker) == -1)
    {
        WorkerModel *modelWorker = new WorkerModel;
        subWindowWorker = ui->mdiArea->addSubWindow(modelWorker);
        subWindowWorker->show();
    }
    else {
        ui->mdiArea->setActiveSubWindow(subWindowWorker);
    }
}

void MainWindow::openModelWork()
{
    if (ui->mdiArea->subWindowList().indexOf(subWindowWork) == -1)
    {
        WorkModel *modelWork = new WorkModel;
        subWindowWork = ui->mdiArea->addSubWindow(modelWork);
        subWindowWork->show();
    }
    else {
        ui->mdiArea->setActiveSubWindow(subWindowWork);
    }
}

void MainWindow::openModelPayment()
{
    if (ui->mdiArea->subWindowList().indexOf(subWindowPayment) == -1)
    {
        PaymentModel *modelPayment = new PaymentModel;
        subWindowPayment = ui->mdiArea->addSubWindow(modelPayment);
        subWindowPayment->show();
    }
    else {
        ui->mdiArea->setActiveSubWindow(subWindowPayment);
    }
}

void MainWindow::openModelStatistik()
{
    if (ui->mdiArea->subWindowList().indexOf(subWindowStatistik) == -1)
    {
        StatistikModel *modelStatistik = new StatistikModel;
        subWindowStatistik = ui->mdiArea->addSubWindow(modelStatistik);
        subWindowStatistik->show();
    }
    else {
        ui->mdiArea->setActiveSubWindow(subWindowPayment);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Halturka"),
            tr("<h2>Halturka v1.0</h2>"
               "<p>Copyright &copy; 2010 by ButuzGOL"
               "<p>Halturka is a small application that managed "
               " construction projects.</p>"));
}

void MainWindow::customize()
{
    connect(ui->actionCustomers, SIGNAL(triggered()), this,
                SLOT(openModelCustomer()));

    connect(ui->actionWorkers, SIGNAL(triggered()), this,
                SLOT(openModelWorker()));

    connect(ui->actionWorks, SIGNAL(triggered()), this,
                SLOT(openModelWork()));

    connect(ui->actionPayments, SIGNAL(triggered()), this,
                SLOT(openModelPayment()));

    connect(ui->actionStatistic, SIGNAL(triggered()), this,
                SLOT(openModelStatistik()));

    connect(ui->nextAction, SIGNAL(triggered()),
            ui->mdiArea, SLOT(activateNextSubWindow()));

    connect(ui->cascadeAction, SIGNAL(triggered()),
            ui->mdiArea, SLOT(cascadeSubWindows()));

    connect(ui->tileAction, SIGNAL(triggered()),
            ui->mdiArea, SLOT(tileSubWindows()));

    connect(ui->previousAction, SIGNAL(triggered()),
            ui->mdiArea, SLOT(activatePreviousSubWindow()));

    connect(ui->aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    connect(ui->aboutAction, SIGNAL(triggered()), this, SLOT(about()));

    ui->closeAction->setShortcut(QKeySequence::Close);

    connect(ui->closeAction, SIGNAL(triggered()),
            ui->mdiArea, SLOT(closeActiveSubWindow()));

    connect(ui->closeAllAction, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionPreferences, SIGNAL(triggered()),
            this, SLOT(preferenceDialog()));
}

void MainWindow::preferenceDialog()
{
    PreferenceDialog dialog;
    dialog.exec();
}
