#ifndef TOOLSDIALOG_H
#define TOOLSDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <QVariant>

namespace Ui {
class ToolsDialog;
}

enum Settings{ IP, MAC, User, Pass, FileManager, Partage, AutoCheck };

class ToolsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolsDialog(QWidget *parent = 0);
    ~ToolsDialog();
    QString getSetting(Settings name);

public slots:
    void selectFileManager();

private slots:
    void accept();

private:
    Ui::ToolsDialog *ui;
    QVariant *settings;
    QString *names;
};

#endif // TOOLSDIALOG_H
