#include "core.h"
#include "ui_core.h"

/**
 * @brief Core::Core Constructor
 * @param parent
 */
Core::Core(QWidget *parent) : QMainWindow(parent), ui(new Ui::Core){
    ui->setupUi(this);
    insBash = new Bash();
    insTools = new ToolsDialog(this);
    lastCheck = new QTime();
    startTimer = new QTimer(this);

    //Setting statusBar
    ui->statusBar->setEnabled(true);
    ui->statusBar->showMessage("Bienvenue dans DadaNAS");

    /**
      Enable timer for auto-check if needed
    */
    if (insTools->getSetting(AutoCheck) == "true"){
        QTimer *autoCheckTimer = new QTimer(this);
        autoCheckTimer->setInterval(60000);
        connect(autoCheckTimer, SIGNAL(timeout()), this, SLOT(checkServerState()));
    }

    //Connecting Signals/Slots
    connect(ui->pushButtonCheckState, SIGNAL(clicked()), this, SLOT(checkServerState()));
    connect(ui->pushButtonStart, SIGNAL(clicked()), this, SLOT(startServer()));
    connect(ui->pushButtonStopPolicy, SIGNAL(clicked(bool)), this, SLOT(changeStopPolicy(bool)));
    connect(ui->pushButtonShare, SIGNAL(clicked()), this, SLOT(viewShares()));
    connect(ui->actionOptions, SIGNAL(triggered()), insTools, SLOT(show()));
    connect(ui->pushButtonMountShare, SIGNAL(clicked(bool)), this, SLOT(mountShares(bool)));
    connect(ui->action_propos_de_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->action_propos_de_DadaNAS, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(startTimer, SIGNAL(timeout()), this, SLOT(timerServerState()));
}

/**
 * @brief Core::~Core Destructor
 */
Core::~Core(){
    delete ui;
    delete insBash;
    delete insTools;
    delete startTimer;
    delete lastCheck;
}

/**
 * @brief Core::checkServerState Check if NAS is UP and Running
 */
void Core::checkServerState(){
    if(!this->lastCheck->isNull() && this->lastCheck->elapsed() < 60000){
        return;
    }
    this->lastCheck->restart();
    QMovie *gifLoader = new QMovie(":/pictures/Ressources/loader.gif");
    ui->labelEtat->setMovie(gifLoader);
    ui->labelEtat->movie()->start();
    ui->labelEtat->show();
    QApplication::processEvents();

    bool isActive = insBash->getServerState(insTools->getSetting(IP));
    if(isActive){
        this->serverState = UP;
        ui->labelEtat->setText("Serveur actif!");
        ui->labelEtat->setStyleSheet("QLabel{ color: green; font-weight: bold;}");
        //If server is active, we check shutdown policy
        //NOTE : first parameter is useless in this case
        this->changeStopPolicy(false, true);
    }
    else{
        this->serverState = DOWN;
        ui->labelEtat->setText("Serveur inactif!");
        ui->labelEtat->setStyleSheet("QLabel{ color: red; font-weight: bold;}");
    }
    return;
}

/**
 * @brief Core::getServerState Get current server state
 * @return
 */
bool Core::getServerState(){
    if(!this->lastCheck->isNull() && this->lastCheck->elapsed() < 60000){
        return this->serverState == UP;
    }

    return false;
}

/**
 * @brief Core::startServer Request NAS startup
 */
void Core::startServer(){
    if(this->startupOrder){
        QMessageBox::warning(this, "Ordre déjà envoyé", "La demande de démarrage a déjà été envoyée.  Veuillez patienter.  Merci");
        return;
    }
    if(this->serverState == UNKNOWN){
        checkServerState();
    }
    if(this->serverState == UP){
        if (!this->getServerState()){
            QMessageBox::information(this, "Serveur en cours d'exécution", "Le serveur est déjà allumé.  Il est donc inutile de lui envoyer un ordre de démarrage");
            return;
        } else {
            this->checkServerState();
            if (this->getServerState()) {
                return;
            }
        }
    }
    //Starting server
    bool result = insBash->sendStartRequest(insTools->getSetting(MAC));
    if(result){
        //Start timer first.  Else, timer will only start when user close dialog
        startTimer->setInterval(1000);
        startTimer->setSingleShot(false);
        startTimer->start();
        this->secForStart = SECFORSTART; //Timer set to full time
        ui->pushButtonCheckState->setEnabled(false);
        this->startupOrder = true;
        ui->labelEtat->setText(QString("Démarré dans %1 sec").arg(this->secForStart));
        ui->statusBar->showMessage("Ordre de démarrage envoyé", 5000);
    }
    else{
        QMessageBox::warning(this, "Échec du démarrage", "La demande de démarrage n'a pas pu être envoyée.  Vérifiez que:\n1)La commande «wol» est bien disponible sur votre système (sinon, installez-la)"
                                                         "\n2)L'adresse IP du serveur est correcte\n3)L'adresse MAC du serveur est correcte\n4)Vous êtes correctement connecté à Internet");
    }
    return;
}

/**
 * @brief Core::changeStopPolicy Change stop policy for NAS
 * @param status Prevent (true) or not (false) NAS automatic shutdown
 * @param check Just check (true) Shutdown policy but don't change it
 */
void Core::changeStopPolicy(bool status, bool check){
    if(this->serverState != UP){
        ui->pushButtonStopPolicy->setChecked(!status); //Prevent from changing state
        return;
    }

    //Creating status
    QString action = ((status) ? "noshutdown" : "shutdown");

    //If check -> no need for action
    if(check)
        action = "none";

    //Getting response
    QJsonObject jsonResponse = this->callServer(action);
    bool actionSuccessful = jsonResponse.value("result").toBool(false);
    bool isShutdownPrevented = jsonResponse.value("shutdown").toBool(false);

    //Checking for error
    if(!actionSuccessful && !check){
        //If «check» is true, «result» will not exists and is set to false by default -> preventing error from triggering
        QMessageBox::critical(this, "Une erreur est survenue", "Une erreur inattendue est survenue.  L'opération n'a pas pu être exécutée!");
        return;
    }

    //Changing status
    if(isShutdownPrevented){
        ui->labelStopPolicy->setText("Arrêt interdit");
        ui->labelStopPolicy->setStyleSheet("QLabel { color: blue; font-weight: bold; }");
        ui->pushButtonStopPolicy->setText("Retirer l'interdiction d'arrêt");
        ui->pushButtonStopPolicy->setChecked(true);
    }
    else{
        ui->labelStopPolicy->setText("Pas d'ordre spécifique");
        ui->labelStopPolicy->setStyleSheet("QLabel { color: inherit; font-weight: inherit; }");
        ui->pushButtonStopPolicy->setText("Interdire l'arrêt");
        ui->pushButtonStopPolicy->setChecked(false);
    }
    return;
}

/**
 * @brief Core::viewShares Open Samba shares in the given explorer
 */
void Core::viewShares(){
    if(this->serverState != UP){
        return;
    }
#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
    insBash->openShares(insTools->getSetting(User), insTools->getSetting(Pass), insTools->getSetting(IP), insTools->getSetting(FileManager));
#else
    QMessageBox::warning(this, "Système non compatible", "Malheureusement, cette fonctionnalité n'est pour l'instant compatible qu'avec les systèmes UNIX ou Windows");
#endif
}

/**
 * @brief Core::mountShares Mount and unmount Samba shares
 * @param status button status -> (un)mounting drives
 */
void Core::mountShares(bool status){
    if(this->serverState != UP){
        ui->pushButtonMountShare->setChecked(!status);
        return;
    }
#if defined(Q_OS_UNIX) || defined(Q_OS_WIN)
    insBash->mountShares(status, insTools->getSetting(IP), insTools->getSetting(User), insTools->getSetting(Pass), insTools->getSetting(Partage));
    if(status){
        ui->pushButtonMountShare->setText("Démonter les répertoires partagés");
        ui->pushButtonMountShare->setStyleSheet("QPushButton{ color: red; font-weight: bold; }");
    }
    else{
        ui->pushButtonMountShare->setText("Monter les répertoires partagés");
        ui->pushButtonMountShare->setStyleSheet("QPushButton{ color: inherit; font-weight: inherit; }");
    }
#else
    QMessageBox::warning(this, "Système non compatible", "Malheureusement, cette fonctionnalité n'est pour l'instant compatible qu'avec les systèmes UNIX ou Windows");
#endif
    return;
}

/**
 * @brief Core::timerServerState Update the timer shown for NAS startup
 */
void Core::timerServerState(){
    this->secForStart--;
    if(this->secForStart <= 0){
        this->startTimer->stop();
        this->checkServerState();
        ui->pushButtonCheckState->setEnabled(true);
        this->startupOrder = false;
        if(ui->labelEtat->text() == "Serveur actif!"){
            //Alert is useless -> showing in statusbar
            //QMessageBox::information(this, "Serveur démarré", "Le serveur a correctement démarré.  Bonne utilisation ;)");
            ui->statusBar->showMessage("Serveur démarré.  Bonne utilisation", 5000);
        }
    }
    else{
        ui->labelEtat->setText(QString("Démarré dans %1 sec").arg(this->secForStart));
    }
    return;
}

/**
 * @brief Core::about Shows the «About» box
 */
void Core::about(){
    QString string_about = ("<h2>À propos de DadaNAS</h2><br><b>Dévoloppé par</b> : David Lumaye<br><b>Version</b> : ")+QString(VERSION)+tr("<br><b>Courriel</b>:littletiger58.aro-base.gmail.com<br><b>Distribué sous license</b> : <a href='http://www.gnu.org/licenses/gpl-3.0.fr.html'>GPL 3</a>");
    QMessageBox::about(this, tr("À propos de DadaNAS"), string_about);
    return;
}

/**
 * @brief Core::callServer Interact with the NAS
 * @param action Action parameter to send to the script
 * @return QJsonObject : JSON response from the NAS
 */
QJsonObject Core::callServer(QString action){
    //No need to verify Action.  An empty string will also work, we just have shutdown status in this case
    QString url = "http://"+insTools->getSetting(IP)+"/nas.php?action="+action;
    QNetworkAccessManager nw_manager;
    QNetworkRequest request(url);
    QNetworkReply *reponse = nw_manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reponse, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    QByteArray data = reponse->readAll();

    //Reading result
    QJsonDocument jsonAnswer = QJsonDocument::fromJson(data);
    QJsonObject jsonResponse = jsonAnswer.object();

    //Returning response
    return jsonResponse;
}
