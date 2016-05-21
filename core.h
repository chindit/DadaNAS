#ifndef CORE_H
#define CORE_H

#include <QMainWindow>
#include "bash.h"

namespace Ui {
class Core;
}

class Core : public QMainWindow
{
    Q_OBJECT

public:
    explicit Core(QWidget *parent = 0);
    ~Core();

public slots:
    void checkServerState();

private:
    Ui::Core *ui;
    Bash *insBash;
};

#endif // CORE_H
