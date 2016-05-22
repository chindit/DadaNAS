#include "bash.h"

Bash::Bash(QObject *parent) : QObject(parent){
    //Nothing to do
}

bool Bash::getServerState(){
    QProcess pingProcess;
    QString exec = "ping";
    QStringList params;
    params << "-c" << "1" << "192.168.1.111";
    pingProcess.start(exec,params,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = pingProcess.readAllStandardOutput();

    return (!p_stdout.contains("100% packet loss"));
}

bool Bash::sendStartRequest(){
    QProcess wolProcess;
    QString exec = "wol";
    QStringList params;
    params << "BC:5F:F4:07:8C:9C";

    wolProcess.start(exec,params,QIODevice::ReadOnly);
    wolProcess.waitForFinished(-1);

    if(wolProcess.exitCode() != QProcess::NormalExit){
        return false;
    }
    return true;
}
