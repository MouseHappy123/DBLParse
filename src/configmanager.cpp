#include "configmanager.h"

#include <QSettings>
#include <QStandardPaths>

#include "util.h"

ConfigManager::ConfigManager(QObject *parent) : QObject(parent)
{
    settings = new QSettings("DBLParse.ini" ,QSettings::IniFormat, this);
    init();
}

void ConfigManager::init()
{
    setDefault("lastOpenFileName", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    setDefault("language", "System");
}

QVariant ConfigManager::value(const QString &key) const
{
    return settings->value(key);
}

void ConfigManager::setValue(const QString &key, const QVariant &value)
{
    settings->setValue(key, value);
}

void ConfigManager::setDefault(const QString &key, const QVariant &value)
{
    if(contains(key)) return ;
    setValue(key, value);
}

bool ConfigManager::contains(const QString &key) const
{
    return settings->contains(key);
}
