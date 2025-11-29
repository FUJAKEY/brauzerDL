#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QUrl>

class QWebEngineView;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent = nullptr);

    QWebEngineView* currentView() const;
    void createTab(const QUrl &url);
    void closeTab(int index);
    void loadUrl(const QUrl &url);
    QString currentTitle() const;

public slots:
    void back();
    void forward();
    void reload();
    void stop();

signals:
    void urlChanged(const QUrl &url);
    void titleChanged(const QString &title);
    void loadProgress(int progress);

private slots:
    void handleCurrentChanged(int index);
    void handleWebViewUrlChanged(const QUrl &url);
    void handleWebViewTitleChanged(const QString &title);
    void handleWebViewLoadProgress(int progress);

private:
    QWebEngineView* createWebView();
};

#endif // TABWIDGET_H
