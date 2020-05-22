#pragma once

#include <QObject>
#include <QDebug>
#include <string>

class Util : public QObject
{
    Q_OBJECT
public:
    static const int BUF_SZ = 4000;
    static const QVector<QPair<QString, QByteArray>> availableLanguages;
    static QString formatTime(int ms);
    
    static QString readFile(const QString &fileName);
    
    static QString getXmlFileName();
    
    static QString formatUrl(const QString &url);
    
    static QString getLocale();
    
    static void clearIndexs();
    
    static void initIndexs();
    
    static bool parsed();
    
    static bool canLoad();
    
    static QString str(int v) {
        return QString::number(v);
    }
    
    static QString str(const QString &s) {
        return s;
    }
    
    template <typename A, typename B>
    static QString str(QPair<A, B> p) {
        return QString("(%1, %2)").arg(str(p.first)).arg(str(p.second));
    }
    
    template <typename A, typename B>
    static QString str(const QMap<A, B> &p) {
        QStringList res;
        for (auto i = p.begin(); i != p.end(); ++i) {
            res.append(str(i.key()) + ": " + str(i.value()));
        }
        return "{" + res.join(", ") + "}";
    }
    
    template <typename A>
    static QString str(A v) {
        QStringList res;
        for (const auto &x : v) {
            res.append(str(x));
        }
        return "{" + res.join(", ") + "}";
    }
};
