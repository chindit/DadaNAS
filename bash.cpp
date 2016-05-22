#include "bash.h"

Bash::Bash(QWidget *parent) : QObject(parent){
    //Nothing to do
    this->parent = parent;
}

bool Bash::getServerState(QString ip){
    if(!this->checkConfig(ip)){
        return false;
    }
    QProcess pingProcess;
    QString exec = "ping";
    QStringList params;
#ifdef Q_OS_UNIX
    params << "-c" << "1" << ip;
#elif defined(Q_OS_WIN)
    params << "-n" << "1" << "-w" << "250" << ip;
#else
    //Nothing to do
#endif
    pingProcess.start(exec,params,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = pingProcess.readAllStandardOutput();
    QRegExp getPacketLoss("\((\\d+)%\)");
    getPacketLoss.indexIn(p_stdout);
    getPacketLoss.cap();
    return (!(getPacketLoss.cap() == "100%"));
}

bool Bash::sendStartRequest(QString mac){
    QProcess wolProcess;
    QString exec = "wol";
    QStringList params;
    params << mac;
#ifdef Q_OS_WIN
    exec = "WolCmd.exe";
    params << "192.168.1.1" << "255.255.255.0" << QString::number(7);
#endif
    wolProcess.start(exec,params,QIODevice::ReadOnly);
    wolProcess.waitForFinished(-1);

    if(wolProcess.exitCode() != QProcess::NormalExit){
        return false;
    }
    return true;
}

void Bash::openShares(QString user, QString pass, QString ip, QString filemanager){
    QProcess dolphinProcess;
    QString exec = filemanager;
    QStringList params;
#ifdef Q_OS_UNIX
    params << "smb://"+user+":"+pass+"@"+ip;
#elif defined(Q_OS_WIN)
    params << "\\\\"+ip;
#else
    //Doing nothing
#endif
    dolphinProcess.startDetached(exec, params);
    return;
}

bool Bash::checkConfig(QString ip){
    if(ip == "0.0.0.0"){
        QMessageBox::critical(this->parent, "Serveur non configuré", "Le serveur n'est pas configuré.  Veuillez le configurer via le menu Outils->Options");
        return false;
    }
    return true;
}

void Bash::mountShares(bool status, QString ip, QString user, QString password, QString partage){
#ifdef Q_OS_UNIX
    QDir mountPoint("/tmp/nas");
    if(!mountPoint.exists()){
        mountPoint.mkdir(mountPoint.absolutePath());
    }
#endif
    if(status){
        QProcess mountProcess;
        QString exec = "pkexec";
        QStringList params;
#ifdef Q_OS_UNIX
        params << "mount" << "-t" << "cifs" << "//"+ip+"/public" << mountPoint.absolutePath() << "-o" << "user="+user+",password="+password;
#elif defined(Q_OS_WIN)
        exec = "net";
        params << "use" << "X:" << "\\\\"+ip+"\\"+partage;
#else
        //Doing nothing
#endif
        mountProcess.start(exec,params,QIODevice::ReadOnly);
        mountProcess.waitForFinished(-1);
    }
    else{
        QProcess umountProcess;
        QString exec = "pkexec";
        QStringList params;
#ifdef Q_OS_UNIX
        params << "umount" << mountPoint.absolutePath();
#elif defined(Q_OS_WIN)
        exec = "net";
        params << "use" << "X:" << "/delete";
#else
        //Doing nothing
#endif
        umountProcess.start(exec,params,QIODevice::ReadOnly);
        umountProcess.waitForFinished(-1);
    }
    return;
}
