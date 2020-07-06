#include "statusloadingwindow.hpp"
#include "ui_statusloadingwindow.h"

StatusLoadingWindow::StatusLoadingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusLoadingWindow)
{
    ui->setupUi(this);
}

StatusLoadingWindow::~StatusLoadingWindow() {
    delete ui;
}

bool StatusLoadingWindow::addLineInTable(const QString& filename,
                                         const QStringList& status)
{
    int newRowIndex{ ui->table->rowCount() };

    QString statusLine;
    for (auto item : status)
        statusLine.append(item);

    QTableWidgetItem* statusItem = new QTableWidgetItem(statusLine);

    if (statusLine.size() < 4 /* Lenght of "OK." */)
        statusItem->setBackground(QBrush(QColor{20, 200, 30}));
    else
        statusItem->setBackground(QBrush(QColor{200, 30, 50}));

    ui->table->insertRow(newRowIndex);
    ui->table->setItem(newRowIndex, 0, new QTableWidgetItem(filename));
    ui->table->setItem(newRowIndex, 1, statusItem);

    return true;
}

void StatusLoadingWindow::closeEvent(QCloseEvent* event) {
    clearTable();
    ui->OKButton->setEnabled(false);
    QWidget::closeEvent(event);
}

void StatusLoadingWindow::on_OKButton_clicked() {
    this->close();
}

void StatusLoadingWindow::onEmitedShowWindow() {
    this->show();
}

void StatusLoadingWindow::sendedCountOfFiles(int count) {
    if (count <= 0)
        ui->progress->setRange(0, 0);

    ui->progress->setRange(0, count);
}

void StatusLoadingWindow::sendedFileAndStatus(const QString& filename,
                                              const QStringList& status)
{
    addLineInTable(filename, status);

    ui->progress->setValue(ui->progress->value() + 1);
    if (ui->progress->value() == ui->progress->maximum())
        ui->OKButton->setEnabled(true);
}

void StatusLoadingWindow::clearTable() {
    for (int rowId{ ui->table->rowCount() }; rowId >= 0; --rowId)
        ui->table->removeRow(rowId);
}
