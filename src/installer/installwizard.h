#ifndef INSTALLWIZARD_H
#define INSTALLWIZARD_H

#include <QWidget>
#include <QWizard>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>

class InstallWizard : public QWizard
{
    Q_OBJECT

public:
    InstallWizard(QWidget *parent = nullptr);

    void accept() override;
};

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget *parent = nullptr);
};

class LocationPage : public QWizardPage
{
    Q_OBJECT
public:
    LocationPage(QWidget *parent = nullptr);
    QString installPath() const;

private slots:
    void browse();

private:
    QLineEdit *m_pathEdit;
};

class InstallPage : public QWizardPage
{
    Q_OBJECT
public:
    InstallPage(QWidget *parent = nullptr);
    void initializePage() override;

private slots:
    void startInstallation();

private:
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
};

#endif // INSTALLWIZARD_H
