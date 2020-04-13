#include "finder.h"
#include "record.h"
#include "webpage.h"
#include "webview.h"
#include "loader.h"
#include "detailview.h"

#include <QFile>
#include <QDataStream>
#include <QSettings>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrl>
#include <QIcon>
#include <QQueue>
#include <QBuffer>

StringRef *Finder::s_authorIndex = nullptr;
StringRef *Finder::s_titleIndex = nullptr;
quint32 Finder::s_authorIndexs = 0;
quint32 Finder::s_titleIndexs = 0;
QFile *Finder::s_file = nullptr;
QList<QPair<QString,int> >  Finder::s_authorStac;

Finder::Finder(QObject *parent) : QObject(parent)
{
    clearIndex();
}

void Finder::find(const QString &type, const QString &word)
{
    QJsonArray result;
    if (!parsed()) goto not_ready;
    if (type == "author") {
        if (!authorLoaded()) goto not_ready;
        auto list = indexOfAuthor(word);
        result = getJson(list);
    } else if (type == "title") {
        if (!titleLoaded()) goto not_ready;
        auto list = indexOfTitle(word);
        result = getJson(list);
    } else if (type == "coauthor") {
        if (!authorLoaded()) goto not_ready;
        auto list = indexOfAuthor(word);
        QSet<QString> coauthors;
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Record record(Util::findRecord(Util::getXmlFileName(), pos));
            foreach(auto author, record.attr("authors").toStringList()){
                coauthors.insert(author);
            }
        }
        coauthors.remove(word);
        result = QJsonArray::fromStringList(coauthors.toList());
    } else if(type == "cograph") {
        QJsonArray cograph;
        cograph=cographBFS(word);
        result = cograph;
    }
    m_lastResult = result;
    emit ready(QJsonDocument(result).toJson());
    return ;
not_ready:
    emit notReady();
    emit ready("not_ready");
    return ;
}

void Finder::handleRequest(QUrl url)
{
    int idx = url.path().remove(0, 1).toInt();
    DetailView *view = new DetailView;
    view->setWindowIcon(qobject_cast<QWidget*>(parent())->windowIcon());
    view->setAttribute(Qt::WA_DeleteOnClose);
    auto html = Util::readFile(":/resources/detail.html");
    auto data = QJsonDocument(m_lastResult[idx].toObject()).toJson();
//    qDebug() << data;
    html.replace("<!-- DATA_HOLDER -->", data);
    view->setHtml(html, QUrl("qrc:/resources/"));
    view->show();
}

bool Finder::parsed()
{
    return QFile("author.dat").exists() 
            && QFile("title.dat").exists()
            && QFile("authorStac.dat").exists();
}

void Finder::clearIndex()
{
    m_loaded = false;
    m_authorLoaded = false;
    m_titleLoaded = false;
    m_authorStacLoaded = false;
    
    s_authorIndexs = 0;
    if (s_authorIndex != nullptr) {
        delete s_authorIndex;
        s_authorIndex = nullptr;
    }
    s_titleIndexs = 0;
    if (s_titleIndex != nullptr) {
        delete s_titleIndex;
        s_titleIndex = nullptr;
    }
    
    s_authorStac.clear();
}

QList<quint32> Finder::indexOfAuthor(const QString &author) const
{
    QList<quint32> list;
    if(s_authorIndex == nullptr) return list;
    auto range = equalRange(s_authorIndex, s_authorIndex + s_authorIndexs, author);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}

QList<quint32> Finder::indexOfTitle(const QString &title) const
{
    QList<quint32> list;
    if(s_titleIndex == nullptr) return list;
    auto range = equalRange(s_titleIndex, s_titleIndex + s_titleIndexs, title);
    for(auto i = range.first; i != range.second; ++i){
        list.append(i->l);
    }
    return list;
}

bool Finder::authorStacLoaded() const
{
    return m_authorStacLoaded;
}

void Finder::setAuthorStacLoaded()
{
    m_authorStacLoaded = true;
}

bool Finder::authorLoaded() const
{
    return m_authorLoaded;
}

void Finder::setAuthorLoaded()
{
    m_authorLoaded = true;
}

bool Finder::titleLoaded() const
{
    return m_titleLoaded;
}

void Finder::setTitleLoaded()
{
    m_titleLoaded = true;
}

bool Finder::loaded() const
{
    return m_loaded;
}

void Finder::setLoaded()
{
    m_loaded = true;
}

QString Finder::readText(const StringRef &ref)
{
    Q_ASSERT(s_file != nullptr);
    Q_ASSERT(s_file->isOpen());
    s_file->seek(ref.l);
    return s_file->read(ref.r - ref.l);
}

QJsonArray Finder::getJson(const QList<quint32> &posList)
{
    QJsonArray array;
    for(int i = 0; i < posList.size(); ++i){
        auto pos = posList.at(i);
        Record record(Util::findRecord(Util::getXmlFileName(), pos));
        array.append(record.toJson());
    }
    auto ret = array;
    return ret;
}
void Finder::init()
{
    if(s_file != nullptr){
        if(s_file->isOpen()) s_file->close();
        delete s_file;
        s_file = nullptr;
    }
    s_file = new QFile(Util::getXmlFileName());
    s_file->open(QFile::ReadOnly);
}

QPair<const StringRef *, const StringRef *> 
Finder::equalRange(const StringRef *first, 
                   const StringRef *last, 
                   const QString &val)
{
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        quint32 half = len >> 1;
        const StringRef *mid = first + half;
        QString text = readText(*mid);
        int cmp = QString::compare(text, val);
        if(cmp < 0){
            first = mid;
            ++first;
            len = len - half - 1;
        }else if(cmp > 0){
            len = half;
        }else{
            const StringRef *left = lowerBound(first, mid, val);
            first += len;
            const StringRef *right = upperBound(++mid, first, val);
            return qMakePair(left, right);
        }
    }
    return qMakePair(first, first);
}

const StringRef *
Finder::lowerBound(const StringRef *first, 
                   const StringRef *last, 
                   const QString &val)
{
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        quint32 half = len >> 1;
        const StringRef *mid = first + half;
        QString text = readText(*mid);
        int ret = QString::compare(text, val);
        if(ret < 0){
            first = mid;
            ++first;
            len = len - half - 1;
        }else{
            len = half;
        }
    }
    return first;
}

const StringRef *
Finder::upperBound(const StringRef *first, 
                   const StringRef *last, 
                   const QString &val)
{
    quint32 len = static_cast<quint32>(last - first);
    while(len > 0){
        quint32 half = len >> 1;
        const StringRef *mid = first + half;
        QString text = readText(*mid);
        int ret = QString::compare(val, text);
        if(ret < 0){
            len = half;
        }else{
            first = mid;
            ++first;
            len = len - half - 1;
        }
    }
    return first;
}

QJsonArray Finder::cographBFS(QString node) {
    QQueue<QPair<QString,QString> > q;
    QJsonArray cograph;
    QPair<QString,QString> parent("",node);
    q.enqueue(parent);
    int num=0;
    while(num<2 && !q.empty()){
        QPair<QString,QString> t=q.head();
        q.dequeue();
        QJsonObject coNode;
//        qDebug()<<"q "<<t;
        auto list = indexOfAuthor(t.second);
        QSet<QString> coauthors;
        for(int i = 0; i < list.size(); ++i){
            auto pos = list.at(i);
            Record record(Util::findRecord(Util::getXmlFileName(), pos));
            foreach(auto author, record.attr("authors").toStringList()){
                coauthors.insert(author);
            }
        }
        coauthors.remove(t.second);

        if(!t.first.isEmpty()){
            coauthors.remove(t.first);
        }

        coNode.insert("parentNode",t.second);

        if(coauthors.size()>0){
            coNode.insert("childNodes",QJsonArray::fromStringList(coauthors.toList()));

            foreach (const QString &value, coauthors){
                QPair<QString,QString> temp(t.second,value);
//                qDebug()<<value;
                q.enqueue(temp);
            }
//            qDebug()<<222<<coNode;
            cograph.append(coNode);
            num++;
//            qDebug()<<num;
            coauthors.clear();
        }

    }
    q.clear();
    return cograph;
}

void Finder::image(const QString &img , const QString &filename)
{
    QPixmap image;    
    image.loadFromData(QByteArray::fromBase64(img.section(",", 1).toLocal8Bit()));
    bool isOk=image.save("./"+filename+"の合作关系图.png");
//    qDebug()<<isOk;
    emit saveImg(isOk);

}
