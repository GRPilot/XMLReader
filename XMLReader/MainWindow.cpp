#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{ parent }
    , ui{ new Ui::MainWindow }
{
    controller = new MainWindowController{ this };

    ui->setupUi(this);
    ui->tableView->setModel(controller->model());
    controller->loadFromDatabase();
}

MainWindow::~MainWindow() {
    delete controller;
    delete ui;
}

void MainWindow::on_loadXmlButton_clicked() {
    controller->loadDataFromXml();
}

void MainWindow::on_SaveToDBButton_clicked() {
    controller->saveAllToDatabase();
}

void MainWindow::on_LoadFromDBButton_clicked() {
    controller->loadFromDatabase();
}

void MainWindow::on_insertButton_clicked() {
    controller->insertLines();
}

void MainWindow::on_removeButton_clicked() {
    controller->removeSelected(ui->tableView->selectionModel());
}

void MainWindow::on_clearTableButton_clicked() {
    controller->clearTable();
}
