#include "WebView.h"
#include <QWebEngineSettings>

WebView::WebView(QWidget *parent) : QWebEngineView(parent) {
    // Enable plugins and other features
    settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
    settings()->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type) {
    Q_UNUSED(type);
    // Create a new WebView for the new window
    WebView *newView = new WebView();
    // Emit signal so BrowserWindow takes ownership and puts it in a tab
    emit newTabRequested(newView);
    return newView;
}
