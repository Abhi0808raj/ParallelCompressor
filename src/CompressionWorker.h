#pragma once

#include <QObject>
#include <QString>

class CompressionWorker : public QObject {
    Q_OBJECT
public:
    explicit CompressionWorker(QObject *parent = nullptr);

    // Set parameters before starting compression
    void setParams(const QString &source, const QString &dest, int compressionLevel, int threadCount);

public slots:
    // Starts the compression process (to be run in a separate thread)
    void process();

signals:
    // Emitted to report progress percentage (0-100)
    void progress(int percentage);

    // Emitted when compression finishes (success or failure)
    void finished();

    // Emitted on error with descriptive message
    void error(const QString &message);

private:
    QString sourcePath;
    QString destPath;
    int compressionLevel;
    int threadCount;
};
