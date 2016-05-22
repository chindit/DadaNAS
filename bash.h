#ifndef BASH_H
#define BASH_H

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QDebug>
#include "libsudo.h"


class Bash : public QObject
{
    Q_OBJECT
public:
    explicit Bash(QWidget *parent = 0);
    bool getServerState(QString ip);
    bool sendStartRequest(QString mac);
    void openShares(QString user, QString pass, QString ip, QString filemanager);
    void mountShares(bool status, QString ip, QString user, QString password);

private:
    bool checkConfig(QString ip);
    QWidget *parent;

signals:

};

#endif // BASH_H
