#include "CompressionWorker.h"
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <zstd.h>
#include <fstream>

CompressionWorker::CompressionWorker(QObject *parent)
    : QObject(parent), compressionLevel(3), threadCount(1) {}

void CompressionWorker::setParams(const QString &source, const QString &dest, int level, int threads) {
    sourcePath = source;
    destPath = dest;
    compressionLevel = level;
    threadCount = threads;
}

void CompressionWorker::process() {
    constexpr size_t CHUNK_SIZE = 4 * 1024 * 1024; // 4MB
    QFile inFile(sourcePath);
    if (!inFile.open(QIODevice::ReadOnly)) {
        emit error(QString("Failed to open input file: %1").arg(sourcePath));
        emit finished();
        return;
    }

    QFile outFile(destPath);
    if (!outFile.open(QIODevice::WriteOnly)) {
        emit error(QString("Failed to open output file: %1").arg(destPath));
        emit finished();
        return;
    }

    qint64 totalSize = inFile.size();
    int totalChunks = (totalSize + CHUNK_SIZE - 1) / CHUNK_SIZE;
    std::atomic<int> chunksDone{0};
    std::mutex writeMutex;

    // Read all chunks into memory (for demonstration; for huge files, use a queue)
    std::vector<QByteArray> inputChunks;
    while (!inFile.atEnd()) {
        QByteArray chunk = inFile.read(CHUNK_SIZE);
        inputChunks.push_back(chunk);
    }

    std::vector<QByteArray> compressedChunks(inputChunks.size());

    auto compressChunk = [&](int idx) {
        const QByteArray &chunk = inputChunks[idx];
        size_t maxDstSize = ZSTD_compressBound(chunk.size());
        QByteArray compChunk((int)maxDstSize, 0);
        size_t compSize = ZSTD_compress(compChunk.data(), maxDstSize,
                                        chunk.constData(), chunk.size(),
                                        compressionLevel);
        if (ZSTD_isError(compSize)) {
            emit error(QString("Compression error on chunk %1: %2")
                           .arg(idx)
                           .arg(ZSTD_getErrorName(compSize)));
            return;
        }
        compChunk.resize((int)compSize);
        compressedChunks[idx] = compChunk;
        int done = ++chunksDone;
        emit progress((done * 100) / totalChunks);
    };

    // Launch threads
    std::vector<std::thread> workers;
    for (int t = 0; t < threadCount; ++t) {
        workers.emplace_back([&, t]() {
            for (int i = t; i < inputChunks.size(); i += threadCount) {
                compressChunk(i);
            }
        });
    }
    for (auto &worker : workers) worker.join();

    // Write compressed chunks in order
    for (const auto &compChunk : compressedChunks) {
        outFile.write(compChunk);
    }

    emit progress(100);
    emit finished();
}
