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

private:
    Ui::Core *ui;
    Bash *insBash;
    QTimer *startTimer;
};

#endif // CORE_H
