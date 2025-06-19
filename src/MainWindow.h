#pragma once

#include <QMainWindow>

class QLineEdit;
class QProgressBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void startCompression();

private:
    QLineEdit *sourceEdit;
    QLineEdit *destEdit;
    QLineEdit *compressionLevelEdit;
    QLineEdit *threadCountEdit;
    QProgressBar *progressBar;
};
