#ifndef CORE_H
#define CORE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>

#include "bash.h"
#include "toolsdialog.h"

#define VERSION "0.1"

namespace Ui {
class Core;
}

class Core : public QMainWindow
{
    Q_OBJECT

public:
    explicit Core(QWidget *parent = 0);
    ~Core();

public slots:
    void checkServerState();
    void startServer();

private slots:
    void changeStopPolicy(bool status);
    void viewShares();
    void mountShares(bool status);
    void about();

private:
    Ui::Core *ui;
    Bash *insBash;
    ToolsDialog *insTools;
    QTimer *startTimer;
};

#endif // CORE_H
