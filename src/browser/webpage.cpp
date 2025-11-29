#include "webpage.h"
#include <QDesktopServices>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
}

QWebEnginePage *WebPage::createWindow(QWebEnginePage::WebWindowType type)
{
    // For now, let default handling apply, or route back to tab widget
    return QWebEnginePage::createWindow(type);
}
