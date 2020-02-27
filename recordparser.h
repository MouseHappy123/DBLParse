#ifndef RECORDPARSER_H
#define RECORDPARSER_H

#include <QObject>
#include <QDate>

class QXmlStreamReader;
class QFile;

class RecordParser : public QObject
{
    Q_OBJECT
public:
    RecordParser(QXmlStreamReader *r);
    QString name() const;
    QString key() const;
    QDate mdate() const;
    QStringList authors() const;
    void setAuthorIndex(QHash<QString, QVariant> *index);
    void parse();
    
    static RecordParser* fromFile(QFile *file, qint64 pos);
    static RecordParser* fromStr(QString str);
    
private:
    QXmlStreamReader *reader;
    QString name_;
    QString key_;
    QDate mdate_;
    QStringList authors_;
    QHash<QString,QVariant> *authorIndex_;
    
    void parseContent();
};

#endif // RECORDPARSER_H
