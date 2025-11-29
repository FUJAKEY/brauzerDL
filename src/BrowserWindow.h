#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QToolBar>
#include <QProgressBar>
#include "HistoryManager.h"
#include "DownloadManager.h"

class WebView;

class BrowserWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit BrowserWindow(QWidget *parent = nullptr);
    ~BrowserWindow();

private slots:
    void handleNewTabRequested(WebView *view = nullptr);
    void closeTab(int index);
    void handleTabChanged(int index);
    void loadUrl();
    void updateUrlBar(const QUrl &url);
    void updateTitle(const QString &title);
    void handleLoadProgress(int progress);
    void handleLoadFinished(bool success);

private:
    QTabWidget *m_tabWidget;
    QLineEdit *m_urlBar;
    QProgressBar *m_progressBar;
    QToolBar *m_toolBar;

    HistoryManager *m_historyManager;
    DownloadManager *m_downloadManager;

    QAction *m_backAction;
    QAction *m_forwardAction;
    QAction *m_reloadAction;
    QAction *m_stopAction;
    QAction *m_settingsAction;
    QAction *m_newTabAction;

    void setupUi();
    void retranslateUi();
    WebView* currentWebView();
};
