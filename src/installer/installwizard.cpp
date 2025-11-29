#include "installwizard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTimer>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QCoreApplication>

// Ideally we use a ZIP library (like libzip or karchive), but to keep dependencies minimal
// for this AI task, we will rely on system 'tar' (Windows 10+ has tar) or PowerShell.
// For the purpose of this demo, we assume a "payload.zip" exists next to the installer.

InstallWizard::InstallWizard(QWidget *parent) : QWizard(parent)
{
    setWindowTitle("Candy Browser Setup");
    setWizardStyle(QWizard::ModernStyle);

    addPage(new IntroPage);
    addPage(new LocationPage);
    addPage(new InstallPage); // This page will run the extract logic
}

void InstallWizard::accept()
{
    QWizard::accept();
}

// --- Intro Page ---

IntroPage::IntroPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Welcome to Candy Browser Setup");
    setSubTitle("This wizard will install Candy Browser on your computer.");

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel("Get ready for the sweetest browsing experience.");
    label->setWordWrap(true);
    layout->addWidget(label);
    setLayout(layout);
}

// --- Location Page ---

LocationPage::LocationPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Select Destination Location");
    setSubTitle("Where should Candy Browser be installed?");

    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *hLayout = new QHBoxLayout;
    m_pathEdit = new QLineEdit;

    // Default path: AppData/Local/Programs/CandyBrowser
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/CandyBrowser";
    m_pathEdit->setText(defaultPath);

    QPushButton *browseBtn = new QPushButton("Browse...");
    connect(browseBtn, &QPushButton::clicked, this, &LocationPage::browse);

    hLayout->addWidget(m_pathEdit);
    hLayout->addWidget(browseBtn);

    layout->addLayout(hLayout);
    setLayout(layout);

    registerField("installPath", m_pathEdit);
}

void LocationPage::browse()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory", m_pathEdit->text());
    if (!dir.isEmpty()) {
        m_pathEdit->setText(dir);
    }
}

QString LocationPage::installPath() const
{
    return m_pathEdit->text();
}

// --- Install Page ---

InstallPage::InstallPage(QWidget *parent) : QWizardPage(parent)
{
    setTitle("Installing");
    setSubTitle("Please wait while Candy Browser is being installed.");

    QVBoxLayout *layout = new QVBoxLayout;
    m_statusLabel = new QLabel("Ready to install...");
    m_progressBar = new QProgressBar;
    m_progressBar->setRange(0, 100);

    layout->addWidget(m_statusLabel);
    layout->addWidget(m_progressBar);
    setLayout(layout);
}

void InstallPage::initializePage()
{
    // Start installation automatically when this page is shown
    QTimer::singleShot(500, this, &InstallPage::startInstallation);
}

void InstallPage::startInstallation()
{
    QString targetDir = field("installPath").toString();
    QString payloadPath = QCoreApplication::applicationDirPath() + "/payload.zip";

    m_statusLabel->setText("Creating directories...");
    QDir dir(targetDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (!QFile::exists(payloadPath)) {
        m_statusLabel->setText("Error: payload.zip not found!");
        QMessageBox::critical(this, "Error", "Could not find payload.zip next to installer.");
        return;
    }

    m_statusLabel->setText("Extracting files...");
    m_progressBar->setValue(10);
    m_progressBar->setRange(0, 0); // Busy indicator

    // Use PowerShell to unzip (Native Windows 10+ feature)
    QString psCommand = QString("Expand-Archive -Path \"%1\" -DestinationPath \"%2\" -Force")
                            .arg(payloadPath)
                            .arg(targetDir);

    QProcess *process = new QProcess(this);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [this, process](int exitCode, QProcess::ExitStatus exitStatus){
        m_progressBar->setRange(0, 100);

        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            m_progressBar->setValue(100);
            m_statusLabel->setText("Installation Complete!");
            wizard()->button(QWizard::FinishButton)->setEnabled(true);
        } else {
            m_statusLabel->setText("Extraction failed.");
            QString error = process->readAllStandardError();
            if (error.isEmpty()) error = "Unknown error";
            QMessageBox::critical(this, "Error", "Extraction failed: " + error);
        }
        process->deleteLater();
        emit completeChanged();
    });

    process->start("powershell", QStringList() << "-Command" << psCommand);
}
