#include "bash.h"

Bash::Bash(QWidget *parent) : QObject(parent){
    this->parent = parent;
}

Bash::~Bash(){
    delete this->parent;
}

bool Bash::getServerState(QString ip){
    if(!this->checkConfig(ip)){
        return false;
    }
    QProcess pingProcess;
    QString exec = "ping";
    QStringList params;
    params << "-c" << "1" << ip;
    pingProcess.start(exec,params,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = pingProcess.readAllStandardOutput();

    return (!p_stdout.contains("100% packet loss"));
}

bool Bash::sendStartRequest(QString mac){
    QProcess wolProcess;
    QString exec = "wol";
    QStringList params;
    params << mac;

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
    params << "smb://"+user+":"+pass+"@"+ip;
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

void Bash::mountShares(bool status, QString ip, QString user, QString password){
    QDir mountPoint("/tmp/nas");
    if(!mountPoint.exists()){
        mountPoint.mkdir(mountPoint.absolutePath());
    }
    if(status){
        QProcess mountProcess;
        QString exec = "pkexec";
        QStringList params;
        params << "mount" << "-t" << "cifs" << "//"+ip+"/public" << mountPoint.absolutePath() << "-o" << "user="+user+",password="+password;
        mountProcess.start(exec,params,QIODevice::ReadOnly);
        mountProcess.waitForFinished(-1);
    }
    else{
        QProcess umountProcess;
        QString exec = "pkexec";
        QStringList params;
        params << "umount" << mountPoint.absolutePath();
        umountProcess.start(exec,params,QIODevice::ReadOnly);
        umountProcess.waitForFinished(-1);
    }
    return;
}
