#include "configw.h"
#include "ui_config.h"

#include <QSettings>

Config::Config(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Config)
{
    ui->setupUi(this);

    loadSettings();

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(saveSettings()));
}

Config::~Config()
{
    delete ui;
}

void Config::saveSettings()
{
    QSettings settings("energyXT2", "XT Software AS");

    settings.setValue("Audio-Inputs", ui->sp_ain->value());
    settings.setValue("Audio-Outputs", ui->sp_aout->value());
    settings.setValue("MIDI-Inputs", ui->sp_min->value());
    settings.setValue("MIDI-Outputs", ui->sp_mout->value());

    settings.setValue("Audio-Inputs-Connect", ui->ch_ain->isChecked());
    settings.setValue("Audio-Outputs-Connect", ui->ch_aout->isChecked());
    settings.setValue("MIDI-Inputs-Connect", ui->ch_min->isChecked());
    settings.setValue("MIDI-Outputs-Connect", ui->ch_mout->isChecked());
}

void Config::loadSettings()
{
    QSettings settings("energyXT2", "XT Software AS");

    ui->sp_ain->setValue(settings.value("Audio-Inputs", 2).toInt());
    ui->sp_aout->setValue(settings.value("Audio-Outputs", 2).toInt());
    ui->sp_min->setValue(settings.value("MIDI-Inputs", 1).toInt());
    ui->sp_mout->setValue(settings.value("MIDI-Outputs", 1).toInt());

    ui->ch_ain->setChecked(settings.value("Audio-Inputs-Connect", true).toBool());
    ui->ch_aout->setChecked(settings.value("Audio-Outputs-Connect", true).toBool());
    ui->ch_min->setChecked(settings.value("MIDI-Inputs-Connect", false).toBool());
    ui->ch_mout->setChecked(settings.value("MIDI-Outputs-Connect", false).toBool());
}
