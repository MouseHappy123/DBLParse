#include "parser.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QDataStream>
#include <QTime>
#include <QDebug>
#include<QMap>
#include <QList>

char *Parser::s_data;
QList<QPair<QString,int> > Parser::authorStac;

Parser::Parser(QObject *parent)
    :QThread(parent)
{
    
}

void Parser::setFileName(const QString &fileName)
{
    m_fileName = fileName;
}

void Parser::run()
{
    parse();
}

void Parser::parse()
{
    m_timing.start();
    emit stateChanged(tr("Parsing start."));
    Q_ASSERT(!m_fileName.isEmpty());
    QFile file(m_fileName);
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    s_data = new char[static_cast<quint64>(file.size())];
    quint32 len = static_cast<quint32>(file.read(s_data, file.size()));
    StringRef ref(0, len);
    file.close();
    emit stateChanged(tr("XML file read successful."));
    QVector<StringRef> authorIndex;
    QVector<StringRef> titleIndex;
    quint32 x = 0;
    QMap<StringRef,int> s_authorStacTemp;
    while(x < len){
        if(ref[x] == '<'){
            if(ref.startsWith("author", x + 1)){
                StringRef author = readElementText(ref, x);
                if(!s_authorStacTemp.contains(author)){
                    s_authorStacTemp.insert(author,1);
                }
                else{
                    s_authorStacTemp.insert(author,s_authorStacTemp.find(author).value()+1);
//                    qDebug()<<s_authorStacTemp.find(author).key();
                }
                authorIndex.append(author);
//                qDebug() << author;
            }else if(ref.startsWith("title", x + 1)){
                StringRef title = readElementText(ref, x);
                titleIndex.append(title);
//                qDebug() << title;
            }
        }
        ++x;
    }


//    QList<Parser::StringRef> tempkeys=s_authorStacTemp.keys();
//    values=s_authorStacTemp.values();

    QList<QPair<Parser::StringRef,int> > temp;

    QMap<StringRef, int>::iterator it=s_authorStacTemp.begin();
    while(it!=s_authorStacTemp.end()){
        temp.append(qMakePair(it.key(),it.value()));
        it++;
    }

    s_authorStacTemp.clear();

    std::sort(temp.begin(),temp.end(),sortByDesc);

    authorStac.clear();

    for(qint32 t=0;t<temp.size();t++){
        authorStac.append(qMakePair(temp[t].first.toString(),temp[t].second));
    }

    emit stateChanged(tr("XML file parse successful."));
    std::sort(authorIndex.begin(), authorIndex.end());
    std::sort(titleIndex.begin(), titleIndex.end());
    emit stateChanged(tr("Index file generated."));
    delete[] s_data;
    file.setFileName("author.dat");
    QDataStream stream(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, authorIndex){
        stream << i.l << i.r;
    }
    file.close();
    file.setFileName("title.dat");
    stream.setDevice(&file);
    file.open(QFile::WriteOnly);
    Q_ASSERT(file.isOpen());
    foreach(auto i, titleIndex){
        stream << i.l << i.r;
    }
    file.close();
    emit stateChanged(tr("Index file saved."));
    m_costMsecs = m_timing.elapsed();
    emit stateChanged(tr("Parse done. Cost time: %1").arg(Util::formatTime(m_costMsecs)));
    emit done();
}

QString Parser::fileName() const
{
    return m_fileName;
}

Parser::StringRef Parser::readElementText(const Parser::StringRef &r, quint32 &from)
{
    StringRef s = r.mid(from);
    Q_ASSERT(s[0] == '<');
    quint32 i = 1;
    char name[30];
    name[0] = '<';
    name[1] = '/';
    while(s[i] != ' ' && s[i] != '>'){
        name[i + 1] = s[i];
        ++i;
    }
    from += i;
    name[i + 1] = '>';
    name[i + 2] = 0;
    // name = "</ele>"
    while(s[i] != '>') ++i;
    qint32 p = s.indexOf(name, i + 1);
    Q_ASSERT(p != -1);
    quint32 x = static_cast<quint32>(p);
    from += x + 1;
    return s.mid(i + 1, x - i - 1);
}

int Parser::costMsecs()
{
    return m_costMsecs;
}

void Parser::clearIndex()
{
    QFile("author.dat").remove();
    QFile("title.dat").remove();
}

char &Parser::StringRef::operator[](quint32 x) const
{
    Q_ASSERT(0 <= x && x < r - l);
    return s_data[l + x];
}

bool Parser::StringRef::operator<(const Parser::StringRef &s) const
{
    quint32 len = r - l;
    if(s.r - s.l < len) len = s.r - s.l;
    for(quint32 i = 0; i < len; ++i){
        if(s_data[l + i] != s_data[s.l + i]){
            return s_data[l + i] < s_data[s.l + i];
        }
    }
    if(len == s.r - s.l) return false;
    else return true;
}

Parser::StringRef Parser::StringRef::mid(quint32 pos) const
{
    Q_ASSERT(0 <= pos && pos < r - l);
    return StringRef(l + pos, r);
}

Parser::StringRef Parser::StringRef::mid(quint32 pos, quint32 len) const
{
    Q_ASSERT(0 <= pos && pos < r - l);
    Q_ASSERT(pos + len <= r - l);
    return StringRef(l + pos, l + pos + len);
}

bool Parser::StringRef::startsWith(const char *str, quint32 from) const
{
    quint32 x = l + from;
    while(*str != 0){
        if(*str != s_data[x]) return false;
        ++str;
        ++x;
        if(x >= r) return false;
    }
    return true;
}

qint32 Parser::StringRef::indexOf(const char *str, quint32 from) const
{
    for(quint32 t = l + from; t < r; ++t){
        bool flag = true;
        for(const char *x = str; *x != 0; ++x){
            if(*x != s_data[t + x - str]){
                flag = false;
                break;
            }
        }
        if(flag){
            return static_cast<qint32>(t - l);
        }
    }
    return -1;
}
QString Parser::StringRef::toString() const
{
     return QByteArray::fromRawData(s_data + l, static_cast<int>(r - l));
}

//bool Parser::sortByDesc(QList<StringRef> &a, QList<int> &s,int l,int r) const
//{
//        if (l < r){
//            int i = l, j = r, x = s[l];
//            StringRef y=a[l];
//            while (i < j)
//            {
//                while (i < j && s[j] < x)
//                    j--;
//                if (i < j){
//                    s[i] = s[j];
//                    a[i]=a[j];
//                }

//                while (i < j && s[i] >= x)
//                    i++;
//                if (i < j){
//                    s[j] = s[i];
//                    a[j]=a[i];
//                }
//            }
//            s[i] = x;
//            a[i]=y;
//            sortByDesc(a,s, l, i - 1);
//            sortByDesc(a,s, i + 1, r);
//        }
//        return true;
//}
