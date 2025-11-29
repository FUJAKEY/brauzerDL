#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QProgressBar>

class TabWidget;
class HistoryManager;
class DownloadManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleNewTab();
    void handleTabClose(int index);
    void handleUrlChanged(const QUrl &url);
    void handleTitleChanged(const QString &title);
    void handleLoadProgress(int progress);
    void navigateToUrl();
    void updateNavigationButtons();
    void showHistory();
    void showDownloads();

private:
    void setupUi();
    void createToolbar();
    void createMenus();

    TabWidget *m_tabWidget;
    QLineEdit *m_urlBar;
    QProgressBar *m_progressBar;
    QAction *m_backAction;
    QAction *m_forwardAction;
    QAction *m_stopAction;
    QAction *m_reloadAction;

    HistoryManager *m_historyManager;
    DownloadManager *m_downloadManager;
};

#endif // MAINWINDOW_H
