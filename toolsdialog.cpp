#include "toolsdialog.h"
#include "ui_toolsdialog.h"

ToolsDialog::ToolsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ToolsDialog){
    names = new QString[7];
    settings = new QVariant[7];

    names[IP] = "IP";
    names[MAC] = "MAC";
    names[User] = "User";
    names[Pass] = "Password";
    names[FileManager] = "FileManager";
    names[Partage] = "Partage";
    names[AutoCheck] = "AutoCheck";

    QSettings options("DadaNAS", "dadanas");
    settings[IP] = options.value(names[IP], "0.0.0.0");
    settings[MAC] = options.value(names[MAC], "00:00:00:00:00:00");
    settings[User] = options.value(names[User], "");
    settings[Pass] = options.value(names[Pass], "");
    settings[Partage] = options.value(names[Partage], "public");
    settings[AutoCheck] = options.value(names[AutoCheck], true);
#ifdef Q_OS_UNIX
    settings[FileManager] = options.value(names[FileManager], "dolphin");
#elif defined(Q_OS_WIN)
    settings[FileManager] = options.value(names[FileManager], "explorer.exe");
#else
    //Doing nothing
#endif

    ui->setupUi(this);

    ui->lineEditIP->setText(settings[IP].toString());
    ui->lineEditMAC->setText(settings[MAC].toString());
    ui->lineEditUser->setText(settings[User].toString());
    ui->lineEditPass->setText(settings[Pass].toString());
    ui->lineEditFileManager->setText(settings[FileManager].toString());
    ui->lineEditPartage->setText(settings[Partage].toString());
    ui->checkBoxAutoCheck->setChecked(settings[AutoCheck].toBool());

    connect(ui->pushButtonFileManager, SIGNAL(clicked()), this, SLOT(selectFileManager()));
}

ToolsDialog::~ToolsDialog()
{
    delete ui;
}

void ToolsDialog::selectFileManager(){
    QString extension = "FileManager (*";
#ifdef Q_OS_WIN
    extension += ".exe";
#endif
            extension += ")";
    QString fileManager = QFileDialog::getOpenFileName(this, "Explorateur de fichier", QDir::homePath(), extension);
    if(!fileManager.isEmpty()){
        ui->lineEditFileManager->setText(fileManager);
    }
}

void ToolsDialog::accept(){
    settings[IP] = ui->lineEditIP->text();
    settings[MAC] = ui->lineEditMAC->text();
    settings[User] = ui->lineEditUser->text();
    settings[Pass] = ui->lineEditPass->text();
    settings[FileManager] = ui->lineEditFileManager->text();
    settings[Partage] = ui->lineEditPartage->text();
    settings[AutoCheck] = ui->checkBoxAutoCheck->isChecked();

    QSettings options("DadaNAS", "dadanas");
    options.setValue(names[IP], settings[IP]);
    options.setValue(names[MAC], settings[MAC]);
    options.setValue(names[User], settings[User]);
    options.setValue(names[Pass], settings[Pass]);
    options.setValue(names[FileManager], settings[FileManager]);
    options.setValue(names[Partage], settings[Partage]);
    options.setValue(names[AutoCheck], settings[AutoCheck]);

    this->hide();
}

QString ToolsDialog::getSetting(Settings name){
    return settings[name].toString();
}
