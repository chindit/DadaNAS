#include "toolsdialog.h"
#include "ui_toolsdialog.h"

ToolsDialog::ToolsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ToolsDialog){
    names = new QString[5];
    settings = new QVariant[5];

    names[IP] = "IP";
    names[MAC] = "MAC";
    names[User] = "User";
    names[Pass] = "Password";
    names[FileManager] = "FileManager";

    QSettings options("DadaNAS", "dadanas");
    settings[IP] = options.value(names[IP], "0.0.0.0");
    settings[MAC] = options.value(names[MAC], "00:00:00:00:00:00");
    settings[User] = options.value(names[User], "");
    settings[Pass] = options.value(names[Pass], "");
    settings[FileManager] = options.value(names[FileManager], "dolphin");

    ui->setupUi(this);

    ui->lineEditIP->setText(settings[IP].toString());
    ui->lineEditMAC->setText(settings[MAC].toString());
    ui->lineEditUser->setText(settings[User].toString());
    ui->lineEditPass->setText(settings[Pass].toString());
    ui->lineEditFileManager->setText(settings[FileManager].toString());
}

ToolsDialog::~ToolsDialog()
{
    delete ui;
}

/*void ToolsDialog::show(){
    //Setting values


    this->show();
    return;
}*/

void ToolsDialog::accept(){
    settings[IP] = ui->lineEditIP->text();
    settings[MAC] = ui->lineEditMAC->text();
    settings[User] = ui->lineEditUser->text();
    settings[Pass] = ui->lineEditPass->text();
    settings[FileManager] = ui->lineEditFileManager->text();

    QSettings options("DadaNAS", "dadanas");
    options.setValue(names[IP], settings[IP]);
    options.setValue(names[MAC], settings[MAC]);
    options.setValue(names[User], settings[User]);
    options.setValue(names[Pass], settings[Pass]);
    options.setValue(names[FileManager], settings[FileManager]);

    this->hide();
}

QString ToolsDialog::getSetting(Settings name){
    return settings[name].toString();
}
