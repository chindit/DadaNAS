#ifndef BASH_H
#define BASH_H

#include <QProcess>
#include <QStringList>
#include <QDebug>


class Bash : public QObject
{
    Q_OBJECT
public:
    explicit Bash(QObject *parent = 0);
    bool getServerState();
    bool sendStartRequest();

signals:

};

#endif // BASH_H
