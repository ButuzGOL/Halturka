#include <QtGui>

#include "connection.h"
#include "preferencedialog.h"

PreferenceDialog::PreferenceDialog(QWidget *parent)
    : QDialog(parent)
{
    createDataBasePage();
    loadPreferences();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    listWidget = new QListWidget;
    listWidget->addItem(tr("Data Base"));

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(dataBasePage);
    connect(listWidget, SIGNAL(currentRowChanged(int)),
            stackedLayout, SLOT(setCurrentIndex(int)));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(1, 3);
    mainLayout->addWidget(listWidget, 0, 0);
    mainLayout->addLayout(stackedLayout, 0, 1);
    mainLayout->addWidget(buttonBox, 1, 0, 1, 2);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(savePreferences()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    setWindowTitle(tr("Preferences"));
    listWidget->setCurrentRow(0);
}

void PreferenceDialog::createDataBasePage()
{
    dataBasePage = new QWidget;

    dataBaseGroupBox = new QGroupBox(tr("Data Base connection"));
    hostNameLabel = new QLabel(tr("Host:"));
    hostNameLineEdit = new QLineEdit;
    dataBaseNameLabel = new QLabel(tr("Data Base name:"));
    dataBaseNameLineEdit = new QLineEdit;
    userNameLabel = new QLabel(tr("User name:"));
    userNameLineEdit = new QLineEdit;
    passwordLabel = new QLabel(tr("Password:"));
    passwordLineEdit = new QLineEdit;
    portLabel = new QLabel(tr("Port:"));
    portLineEdit = new QLineEdit;

    QGridLayout *dataBaseLayout = new QGridLayout;
    dataBaseLayout->addWidget(hostNameLabel, 0, 0);
    dataBaseLayout->addWidget(hostNameLineEdit, 0, 1);
    dataBaseLayout->addWidget(dataBaseNameLabel, 1, 0);
    dataBaseLayout->addWidget(dataBaseNameLineEdit, 1, 1);
    dataBaseLayout->addWidget(userNameLabel, 2, 0);
    dataBaseLayout->addWidget(userNameLineEdit, 2, 1);
    dataBaseLayout->addWidget(passwordLabel, 3, 0);
    dataBaseLayout->addWidget(passwordLineEdit, 3, 1);
    dataBaseLayout->addWidget(portLabel, 4, 0);
    dataBaseLayout->addWidget(portLineEdit, 4, 1);
    dataBaseGroupBox->setLayout(dataBaseLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout;
    pageLayout->setMargin(0);
    pageLayout->addWidget(dataBaseGroupBox);
    pageLayout->addStretch();
    dataBasePage->setLayout(pageLayout);
}

void PreferenceDialog::writeXml(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("database");
    xmlWriter.writeTextElement("host", hostNameLineEdit->text());
    xmlWriter.writeTextElement("databasename", dataBaseNameLineEdit->text());
    xmlWriter.writeTextElement("username", userNameLineEdit->text());
    xmlWriter.writeTextElement("password", passwordLineEdit->text());
    xmlWriter.writeTextElement("port", portLineEdit->text());
    xmlWriter.writeEndDocument();
    file.close();
}

void PreferenceDialog::readXml(const QString &fileName)
{
    QFile *file = new QFile(fileName);
    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QXmlStreamReader xmlReader(file);
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    hostNameLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    dataBaseNameLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    userNameLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    passwordLineEdit->setText(xmlReader.readElementText());
    xmlReader.readNext();
    xmlReader.readNext();
    portLineEdit->setText(xmlReader.readElementText());

    file->close();
}

void PreferenceDialog::savePreferences()
{
    writeXml("preferences.xml");
    if (createConnection())
        accept();
}

void PreferenceDialog::loadPreferences()
{
    readXml("preferences.xml");
}


