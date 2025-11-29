#include "tabwidget.h"
#include "webpage.h"
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QTabBar>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);

    connect(this, &QTabWidget::currentChanged, this, &TabWidget::handleCurrentChanged);
    connect(this, &QTabWidget::tabCloseRequested, this, &TabWidget::closeTab);
}

QWebEngineView* TabWidget::currentView() const
{
    return qobject_cast<QWebEngineView*>(currentWidget());
}

void TabWidget::createTab(const QUrl &url)
{
    QWebEngineView *view = createWebView();
    view->load(url);
    addTab(view, "New Tab");
    setCurrentWidget(view);
}

void TabWidget::closeTab(int index)
{
    QWidget *widget = widget(index);
    removeTab(index);
    delete widget;
}

void TabWidget::loadUrl(const QUrl &url)
{
    if (QWebEngineView *view = currentView()) {
        view->load(url);
    }
}

QString TabWidget::currentTitle() const
{
    if (QWebEngineView *view = currentView()) {
        return view->title();
    }
    return QString();
}

void TabWidget::back()
{
    if (QWebEngineView *view = currentView()) view->back();
}

void TabWidget::forward()
{
    if (QWebEngineView *view = currentView()) view->forward();
}

void TabWidget::reload()
{
    if (QWebEngineView *view = currentView()) view->reload();
}

void TabWidget::stop()
{
    if (QWebEngineView *view = currentView()) view->stop();
}

QWebEngineView* TabWidget::createWebView()
{
    QWebEngineView *view = new QWebEngineView(this);
    WebPage *page = new WebPage(QWebEngineProfile::defaultProfile(), view);
    view->setPage(page);

    // Connect signals
    connect(view, &QWebEngineView::urlChanged, this, &TabWidget::handleWebViewUrlChanged);
    connect(view, &QWebEngineView::titleChanged, this, &TabWidget::handleWebViewTitleChanged);
    connect(view, &QWebEngineView::loadProgress, this, &TabWidget::handleWebViewLoadProgress);

    // Create new window handling
    connect(view->page(), &QWebEnginePage::iconChanged, [this, view](const QIcon &icon){
        int index = indexOf(view);
        if (index != -1) setTabIcon(index, icon);
    });

    return view;
}

void TabWidget::handleCurrentChanged(int index)
{
    if (index != -1) {
        QWebEngineView *view = qobject_cast<QWebEngineView*>(widget(index));
        if (view) {
            emit urlChanged(view->url());
            emit titleChanged(view->title());
        }
    }
}

void TabWidget::handleWebViewUrlChanged(const QUrl &url)
{
    if (sender() == currentView()) {
        emit urlChanged(url);
    }
}

void TabWidget::handleWebViewTitleChanged(const QString &title)
{
    QWebEngineView *view = qobject_cast<QWebEngineView*>(sender());
    int index = indexOf(view);
    if (index != -1) {
        setTabText(index, title);
    }
    if (view == currentView()) {
        emit titleChanged(title);
    }
}

void TabWidget::handleWebViewLoadProgress(int progress)
{
    if (sender() == currentView()) {
        emit loadProgress(progress);
    }
}
