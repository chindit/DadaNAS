#include "core.h"
#include "ui_core.h"

Core::Core(QWidget *parent) : QMainWindow(parent), ui(new Ui::Core){
    ui->setupUi(this);
    insBash = new Bash();

    connect(ui->pushButtonCheckState, SIGNAL(clicked()), this, SLOT(checkServerState()));
    connect(ui->pushButtonStart, SIGNAL(clicked()), this, SLOT(startServer()));
    connect(ui->pushButtonStopPolicy, SIGNAL(clicked(bool)), this, SLOT(changeStopPolicy(bool)));
}

Core::~Core(){
    delete ui;
    delete insBash;
}

void Core::checkServerState(){
    QMovie *gifLoader = new QMovie(":/pictures/Ressources/loader.gif");
    ui->labelEtat->setMovie(gifLoader);
    ui->labelEtat->movie()->start();
    ui->labelEtat->show();
    QApplication::processEvents();

    bool isActive = insBash->getServerState();
    if(isActive){
        ui->labelEtat->setText("Serveur actif!");
        ui->labelEtat->setStyleSheet("QLabel{ color: green; font-weight: bold;}");
    }
    else{
        ui->labelEtat->setText("Serveur inactif!");
        ui->labelEtat->setStyleSheet("QLabel{ color: red; font-weight: bold;}");
    }
    return;
}

void Core::startServer(){
    if(insBash->getServerState()){
        QMessageBox::information(this, "Serveur en cours d'exécution", "Le serveur est déjà allumé.  Il est donc inutile de lui envoyer un ordre de démarrage");
        return;
    }
    //Starting server
    bool result = insBash->sendStartRequest();
    if(result){
        QMessageBox::information(this, "Requête envoyée", "La demande de démarrage a bien été envoyée.  Le serveur sera allumé d'ici une ou deux minutes.");
    }
    else{
        startTimer = new QTimer(this);
        startTimer->setInterval(2*60*1000);
        startTimer->setSingleShot(true);
        connect(startTimer, SIGNAL(timeout()), this, SLOT(checkServerState()));
        startTimer->start();
        QMessageBox::warning(this, "Échec du démarrage", "La demande de démarrage n'a pas pu être envoyée.  Vérifiez que:\n1)La commande «wol» est bien disponible sur votre système (sinon, installez-la)"
                                                         "\n2)L'adresse MAC du serveur est correcte\n3)Vous êtes correctement connecté à Internet");
    }
    return;
}

void Core::changeStopPolicy(bool status){
    if(!insBash->getServerState())
        return;

    QString url = "http://192.168.1.111/nas.php?action=";
    url += ((status) ? "noshutdown" : "shutdown");
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(url);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();
    QString contenu_page(data);

    //Reading result
    QJsonDocument jsonAnswer = QJsonDocument::fromJson(data);
    QJsonObject jsonResponse = jsonAnswer.object();
    bool isShutdownPrevented = jsonResponse.value("result").toBool(false);

    if(isShutdownPrevented){
        ui->labelStopPolicy->setText("Arrêt interdit");
        ui->labelStopPolicy->setStyleSheet("QLabel { color: blue; font-weight: bold; }");
        ui->pushButtonStopPolicy->setText("Retirer l'interdiction d'arrêt");
    }
    else{
        ui->labelStopPolicy->setText("Pas d'ordre spécifique");
        ui->labelStopPolicy->setStyleSheet("QLabel { color: inherit; font-weight: inherit; }");
        ui->pushButtonStopPolicy->setText("Interdire l'arrêt");
    }
    return;
}
