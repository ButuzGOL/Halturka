#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtGui>
#include <QtSql>

static void connectionParametrs(QString& host, int& port, QString& dataBaseName,
                  QString& userName, QString& password)
{
    QFile *file = new QFile("preferences.xml");
    if (!file->open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QXmlStreamReader xmlReader(file);
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    xmlReader.readNext();
    host = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    dataBaseName = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    userName = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    password = xmlReader.readElementText();
    xmlReader.readNext();
    xmlReader.readNext();
    port = xmlReader.readElementText().toInt();

    file->close();
}

static bool createConnection()
{

    QString host = "localhost", dataBaseName = "halturka",
            userName = "postgres", password = "1";

    int port = 5432;
    connectionParametrs(host, port, dataBaseName, userName, password);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setDatabaseName(dataBaseName);
    db.setUserName(userName);
    db.setPassword(password);
    db.setPort(port);

    if (!db.open()) {
        QMessageBox::warning(0, QObject::tr("Database Error"),
                             db.lastError().text());
        return false;
    }
    return true;

}

#endif // CONNECTION_H
