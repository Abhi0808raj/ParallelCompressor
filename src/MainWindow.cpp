#include "MainWindow.h"
#include "CompressionWorker.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QThread>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Central widget and layout
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // File selection
    QFormLayout *formLayout = new QFormLayout;
    sourceEdit = new QLineEdit;
    destEdit = new QLineEdit;
    QPushButton *browseSource = new QPushButton("Browse...");
    QPushButton *browseDest = new QPushButton("Browse...");

    QHBoxLayout *srcLayout = new QHBoxLayout;
    srcLayout->addWidget(sourceEdit);
    srcLayout->addWidget(browseSource);

    QHBoxLayout *dstLayout = new QHBoxLayout;
    dstLayout->addWidget(destEdit);
    dstLayout->addWidget(browseDest);

    formLayout->addRow("Source File:", srcLayout);
    formLayout->addRow("Destination File:", dstLayout);

    // Compression level and thread count 
    compressionLevelEdit = new QLineEdit("3");
    threadCountEdit = new QLineEdit(QString::number(QThread::idealThreadCount()));
    formLayout->addRow("Compression Level (1-22):", compressionLevelEdit);
    formLayout->addRow("Threads:", threadCountEdit);

    // Progress bar
    progressBar = new QProgressBar;
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    // Start button
    QPushButton *startButton = new QPushButton("Start Compression");

    // Assemble layout
    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(startButton);
    central->setLayout(mainLayout);
    setCentralWidget(central);
    setWindowTitle("Parallel File Compressor");

    // Connections
    connect(browseSource, &QPushButton::clicked, this, [this] {
        QString file = QFileDialog::getOpenFileName(this, "Select File to Compress");
        if (!file.isEmpty())
            sourceEdit->setText(file);
    });
    connect(browseDest, &QPushButton::clicked, this, [this] {
        QString file = QFileDialog::getSaveFileName(this, "Select Output File");
        if (!file.isEmpty())
            destEdit->setText(file);
    });
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startCompression);
}

void MainWindow::startCompression() {
    QString source = sourceEdit->text();
    QString dest = destEdit->text();
    int level = compressionLevelEdit->text().toInt();
    int threads = threadCountEdit->text().toInt();

    if (source.isEmpty() || dest.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please specify both source and destination files.");
        return;
    }
    if (level < 1 || level > 22) {
        QMessageBox::warning(this, "Input Error", "Compression level must be between 1 and 22.");
        return;
    }
    if (threads < 1) {
        QMessageBox::warning(this, "Input Error", "Thread count must be at least 1.");
        return;
    }

    // Disable UI during compression
    sourceEdit->setEnabled(false);
    destEdit->setEnabled(false);
    compressionLevelEdit->setEnabled(false);
    threadCountEdit->setEnabled(false);

    // Set up worker and thread
    CompressionWorker *worker = new CompressionWorker;
    worker->setParams(source, dest, level, threads);

    QThread *thread = new QThread;
    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &CompressionWorker::process);
    connect(worker, &CompressionWorker::progress, progressBar, &QProgressBar::setValue);
    connect(worker, &CompressionWorker::error, this, [this, worker, thread](const QString &msg) {
        QMessageBox::critical(this, "Compression Error", msg);
        progressBar->setValue(0);
        sourceEdit->setEnabled(true);
        destEdit->setEnabled(true);
        compressionLevelEdit->setEnabled(true);
        threadCountEdit->setEnabled(true);
        thread->quit();
    });
    connect(worker, &CompressionWorker::finished, this, [this, worker, thread]() {
        QMessageBox::information(this, "Done", "Compression finished.");
        progressBar->setValue(100);
        sourceEdit->setEnabled(true);
        destEdit->setEnabled(true);
        compressionLevelEdit->setEnabled(true);
        threadCountEdit->setEnabled(true);
        thread->quit();
    });
    connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);

    thread->start();
}
