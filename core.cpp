#include "core.h"
#include "ui_core.h"

Core::Core(QWidget *parent) : QMainWindow(parent), ui(new Ui::Core){
    ui->setupUi(this);
    insBash = new Bash();

    connect(ui->pushButtonCheckState, SIGNAL(clicked()), this, SLOT(checkServerState()));
}

Core::~Core(){
    delete ui;
}

void Core::checkServerState(){
    bool isActive = insBash->getServerState();
    if(isActive){
        ui->labelEtat->setText("Serveur actif!");
    }
    else{
        ui->labelEtat->setText("Serveur inactif!");
    }
    return;
}
