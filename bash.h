#ifndef BASH_H
#define BASH_H

#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QDebug>


class Bash : public QObject
{
    Q_OBJECT
public:
    explicit Bash(QWidget *parent = 0);
    ~Bash();

    bool getServerState(QString ip);
    bool sendStartRequest(QString mac);
    void openShares(QString user, QString pass, QString ip, QString filemanager);
    void mountShares(bool status, QString ip, QString user, QString password, QString partage);

private:
    bool checkConfig(QString ip);
    QWidget *parent;
    QTime *lastCheck;

signals:

};

#endif // BASH_H
