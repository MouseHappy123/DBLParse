#ifndef UTIL_H
#define UTIL_H

#include <QObject>

class Util : public QObject
{
    Q_OBJECT
public:
    static QString formatTime(int ms);
    static QString findRecord(const QString &fileName, quint32 pos);
};

#endif // UTIL_H
