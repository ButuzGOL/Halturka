#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>

class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QListWidget;
class QStackedLayout;

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    PreferenceDialog(QWidget *parent = 0);

private slots:
    void savePreferences();

private:
    void createDataBasePage();
    void writeXml(const QString &fileName);
    void readXml(const QString &fileName);
    void loadPreferences();

    QStackedLayout *stackedLayout;
    QListWidget *listWidget;
    QWidget *dataBasePage;
    QDialogButtonBox *buttonBox;

    QGroupBox *dataBaseGroupBox;
    QLabel *hostNameLabel;
    QLineEdit *hostNameLineEdit;
    QLabel *dataBaseNameLabel;
    QLineEdit *dataBaseNameLineEdit;
    QLabel *userNameLabel;
    QLineEdit *userNameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
};

#endif // PREFERENCEDIALOG_H
