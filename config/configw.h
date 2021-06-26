#ifndef CONFIG_H
#define CONFIG_H

#include <QtGui/QDialog>

namespace Ui {
    class Config;
}

class Config : public QDialog
{
    Q_OBJECT

public:
    explicit Config(QWidget *parent = 0);
    ~Config();

private:
    Ui::Config *ui;
    void loadSettings();

private slots:
    void saveSettings();

};

#endif // CONFIG_H
