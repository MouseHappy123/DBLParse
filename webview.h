#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    WebView(QWidget *parent = nullptr);

protected:
    void contextMenuEvent(QContextMenuEvent *) override;
};

#endif // WEBVIEW_H