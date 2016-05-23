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
#include <QTime>
#include <QTimer>

#include "bash.h"
#include "toolsdialog.h"

#define VERSION "0.1"
#define SECFORSTART 90 //Time before checking server for startup

namespace Ui {
class Core;
}

enum ServerStatus { UP, DOWN, UNKNOWN };

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
    void timerServerState();

private:
    Ui::Core *ui;
    Bash *insBash;
    ToolsDialog *insTools;
    QTime *lastCheck;
    QTimer *startTimer;
    long secForStart = 0;
    bool startupOrder = false;
    long serverState = UNKNOWN;
};

#endif // CORE_H
