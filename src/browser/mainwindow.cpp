#include "mainwindow.h"
#include "tabwidget.h"
#include "historymanager.h"
#include "downloadmanager.h"
#include <QToolBar>
#include <QBoxLayout>
#include <QAction>
#include <QIcon>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_historyManager = new HistoryManager(this);
    connect(m_historyManager, &HistoryManager::requestOpenUrl, [this](const QUrl &url){
        m_tabWidget->createTab(url);
    });

    m_downloadManager = new DownloadManager(this);

    setupUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    // Central Widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    createToolbar();

    m_progressBar = new QProgressBar(this);
    m_progressBar->setMaximumHeight(3);
    m_progressBar->setTextVisible(false);
    m_progressBar->setStyleSheet("QProgressBar { border: none; background: transparent; } QProgressBar::chunk { background: #cba6f7; }");
    layout->addWidget(m_progressBar);

    m_tabWidget = new TabWidget(this);
    connect(m_tabWidget, &TabWidget::urlChanged, this, &MainWindow::handleUrlChanged);
    connect(m_tabWidget, &TabWidget::titleChanged, this, &MainWindow::handleTitleChanged);
    connect(m_tabWidget, &TabWidget::loadProgress, this, &MainWindow::handleLoadProgress);

    layout->addWidget(m_tabWidget);
    setCentralWidget(centralWidget);

    createMenus();

    // Initial Tab
    handleNewTab();
}

void MainWindow::createToolbar()
{
    QToolBar *toolbar = addToolBar("Navigation");
    toolbar->setMovable(false);

    m_backAction = toolbar->addAction("Back");
    connect(m_backAction, &QAction::triggered, m_tabWidget, &TabWidget::back);

    m_forwardAction = toolbar->addAction("Forward");
    connect(m_forwardAction, &QAction::triggered, m_tabWidget, &TabWidget::forward);

    m_reloadAction = toolbar->addAction("Reload");
    connect(m_reloadAction, &QAction::triggered, m_tabWidget, &TabWidget::reload);

    m_urlBar = new QLineEdit(this);
    m_urlBar->setPlaceholderText("Search or enter address...");
    connect(m_urlBar, &QLineEdit::returnPressed, this, &MainWindow::navigateToUrl);
    toolbar->addWidget(m_urlBar);

    QAction *historyAction = toolbar->addAction("History");
    connect(historyAction, &QAction::triggered, this, &MainWindow::showHistory);

    QAction *downloadsAction = toolbar->addAction("Downloads");
    connect(downloadsAction, &QAction::triggered, this, &MainWindow::showDownloads);
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New Tab", this, &MainWindow::handleNewTab, QKeySequence::AddTab);
    fileMenu->addAction("Close Tab", [this](){ handleTabClose(m_tabWidget->currentIndex()); }, QKeySequence::Close);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);
}

void MainWindow::handleNewTab()
{
    m_tabWidget->createTab(QUrl("https://www.google.com"));
    m_urlBar->setFocus();
}

void MainWindow::handleTabClose(int index)
{
    m_tabWidget->closeTab(index);
    if (m_tabWidget->count() == 0) {
        close();
    }
}

void MainWindow::handleUrlChanged(const QUrl &url)
{
    m_urlBar->setText(url.toString());
    m_historyManager->addHistoryItem(m_tabWidget->currentTitle(), url);
}

void MainWindow::handleTitleChanged(const QString &title)
{
    setWindowTitle(title + " - CandyBrowser");
}

void MainWindow::handleLoadProgress(int progress)
{
    m_progressBar->setValue(progress);
    if (progress == 100) {
        m_progressBar->hide();
    } else {
        m_progressBar->show();
    }
}

void MainWindow::navigateToUrl()
{
    QString text = m_urlBar->text();
    if (text.isEmpty()) return;

    QUrl url = QUrl::fromUserInput(text);
    if (!url.isValid()) return;

    m_tabWidget->loadUrl(url);
}

void MainWindow::updateNavigationButtons()
{
    // Enable/disable back/forward buttons based on webview state
    // To be implemented fully via TabWidget signals
}

void MainWindow::showHistory()
{
    m_historyManager->showHistoryDialog();
}

void MainWindow::showDownloads()
{
    m_downloadManager->showDownloadsDialog();
}
