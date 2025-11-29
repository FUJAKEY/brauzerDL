#include "BrowserWindow.h"
#include "WebView.h"
#include <QVBoxLayout>
#include <QToolButton>
#include <QStyle>
#include <QIcon>
#include <QUrl>
#include <QWebEngineProfile>

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_historyManager = new HistoryManager(this);
    m_downloadManager = new DownloadManager(this);

    // Global download handler
    QWebEngineProfile::defaultProfile()->setHttpCacheType(QWebEngineProfile::DiskHttpCache);
    connect(QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested,
            m_downloadManager, &DownloadManager::handleDownloadRequested);

    setupUi();
    // Start with one tab
    handleNewTabRequested();
}

BrowserWindow::~BrowserWindow() {}

void BrowserWindow::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Toolbar
    m_toolBar = new QToolBar(this);
    m_toolBar->setMovable(false);
    m_toolBar->setFloatable(false);
    addToolBar(Qt::TopToolBarArea, m_toolBar);

    // Navigation Actions
    m_backAction = m_toolBar->addAction(QIcon(":/icons/back.svg"), "Back");
    m_forwardAction = m_toolBar->addAction(QIcon(":/icons/forward.svg"), "Forward");
    m_reloadAction = m_toolBar->addAction(QIcon(":/icons/reload.svg"), "Reload");
    m_stopAction = m_toolBar->addAction(QIcon(":/icons/stop.svg"), "Stop");

    // URL Bar
    m_urlBar = new QLineEdit(this);
    m_urlBar->setPlaceholderText("Search or enter address");
    m_toolBar->addWidget(m_urlBar);

    // Progress Bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setMaximumWidth(100);
    m_progressBar->setVisible(false);
    m_toolBar->addWidget(m_progressBar);

    // New Tab Button
    QAction *newTabAction = m_toolBar->addAction(QIcon(":/icons/new_tab.svg"), "+");
    connect(newTabAction, &QAction::triggered, this, [this]() { handleNewTabRequested(); });

    // Tab Widget
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setDocumentMode(true);
    m_tabWidget->setTabsClosable(true);
    layout->addWidget(m_tabWidget);

    // Connections
    connect(m_urlBar, &QLineEdit::returnPressed, this, &BrowserWindow::loadUrl);
    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &BrowserWindow::closeTab);
    connect(m_tabWidget, &QTabWidget::currentChanged, this, &BrowserWindow::handleTabChanged);

    // Connect actions
    connect(m_backAction, &QAction::triggered, [this]() {
        if (auto view = currentWebView()) view->back();
    });
    connect(m_forwardAction, &QAction::triggered, [this]() {
        if (auto view = currentWebView()) view->forward();
    });
    connect(m_reloadAction, &QAction::triggered, [this]() {
        if (auto view = currentWebView()) view->reload();
    });
    connect(m_stopAction, &QAction::triggered, [this]() {
        if (auto view = currentWebView()) view->stop();
    });
}

void BrowserWindow::handleNewTabRequested(WebView *view) {
    if (!view) {
        view = new WebView(this);
    }

    // Connect WebView signals
    connect(view, &WebView::newTabRequested, this, &BrowserWindow::handleNewTabRequested);
    connect(view, &QWebEngineView::urlChanged, this, [this, view](const QUrl &url){
        if (currentWebView() == view) updateUrlBar(url);
    });
    connect(view, &QWebEngineView::titleChanged, this, [this, view](const QString &title){
        int index = m_tabWidget->indexOf(view);
        if (index != -1) {
            m_tabWidget->setTabText(index, title);
            if (currentWebView() == view) setWindowTitle(title + " - SweetBrowser");
        }
        // Save history on title change (usually means load complete)
        if (!title.isEmpty() && !view->url().isEmpty()) {
            m_historyManager->addHistoryItem(title, view->url().toString());
        }
    });
    connect(view, &QWebEngineView::loadProgress, this, [this, view](int progress){
        if (currentWebView() == view) handleLoadProgress(progress);
    });
    connect(view, &QWebEngineView::loadFinished, this, [this, view](bool success){
        if (currentWebView() == view) handleLoadFinished(success);
    });

    // Add to tab widget
    int index = m_tabWidget->addTab(view, "New Tab");
    m_tabWidget->setCurrentIndex(index);

    // If it's a fresh tab, focus the URL bar.
    if (view->url().isEmpty()) {
        m_urlBar->setFocus();
    }
}

void BrowserWindow::closeTab(int index) {
    if (m_tabWidget->count() > 1) {
        QWidget *widget = m_tabWidget->widget(index);
        m_tabWidget->removeTab(index);
        widget->deleteLater();
    } else {
        // If closing the last tab, maybe close the window or just create a new blank one
        // For now, close app
        close();
    }
}

void BrowserWindow::handleTabChanged(int index) {
    if (index < 0) return;
    WebView *view = qobject_cast<WebView*>(m_tabWidget->widget(index));
    if (view) {
        updateUrlBar(view->url());
        setWindowTitle(view->title().isEmpty() ? "SweetBrowser" : view->title() + " - SweetBrowser");

        // Update actions state
        m_backAction->setEnabled(view->history()->canGoBack());
        m_forwardAction->setEnabled(view->history()->canGoForward());
    }
}

WebView* BrowserWindow::currentWebView() {
    return qobject_cast<WebView*>(m_tabWidget->currentWidget());
}

void BrowserWindow::loadUrl() {
    WebView *view = currentWebView();
    if (view) {
        QString input = m_urlBar->text();
        QUrl url = QUrl::fromUserInput(input);

        // Basic search fallback
        if (!url.isValid() || (input.indexOf('.') == -1 && input.indexOf(':') == -1 && input.indexOf("localhost") == -1)) {
            url = QUrl("https://www.google.com/search?q=" + input);
        }

        view->setUrl(url);
    }
}

void BrowserWindow::updateUrlBar(const QUrl &url) {
    m_urlBar->setText(url.toString());
}

void BrowserWindow::handleLoadProgress(int progress) {
    m_progressBar->setValue(progress);
    if (progress < 100) {
        m_progressBar->setVisible(true);
    } else {
        m_progressBar->setVisible(false);
    }
}

void BrowserWindow::handleLoadFinished(bool success) {
    m_progressBar->setVisible(false);
    if (!success) {
        // Could show an error page here
    }
    // Refresh buttons state
    if (auto view = currentWebView()) {
        m_backAction->setEnabled(view->history()->canGoBack());
        m_forwardAction->setEnabled(view->history()->canGoForward());
    }
}
