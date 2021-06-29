#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Config; }
QT_END_NAMESPACE

class Config : public QDialog
{
    Q_OBJECT

public:
    Config(QWidget *parent = nullptr);
    ~Config();

private:
    Ui::Config *ui;
    void loadSettings();

private slots:
    void saveSettings();

};
#endif // CONFIG_H
