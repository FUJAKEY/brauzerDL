#pragma once
#include <QWebEngineView>
#include <QWebEnginePage>

class WebView : public QWebEngineView {
    Q_OBJECT

public:
    explicit WebView(QWidget *parent = nullptr);

protected:
    // This allows the browser to handle "Open in new tab" requests
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

signals:
    // Signal to notify the main window to create a new tab with this view
    void newTabRequested(WebView *view);
};
