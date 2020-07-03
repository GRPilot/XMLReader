#pragma once

#include <QWidget>

namespace Ui {
    class StatusLoadingWindow;
}

class StatusLoadingWindow : public QWidget
{
    Q_OBJECT

public:
    StatusLoadingWindow(QWidget *parent = nullptr);
    ~StatusLoadingWindow();

    bool addLineInTable(const QString &filename, const QStringList &status);
    void closeEvent(QCloseEvent *event) override;


public slots:
    void on_OKButton_clicked();

    void onEmitedShowWindow();
    void sendedCountOfFiles(int count);
    void sendedFileAndStatus(const QString &filename, const QStringList &status);

private:
    Ui::StatusLoadingWindow *ui;


    void clearTable();
};

