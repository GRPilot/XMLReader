#pragma once

#include <QMainWindow>

#include "MainWindowController.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadXmlButton_clicked();
    void on_SaveToDBButton_clicked();
    void on_LoadFromDBButton_clicked();
    void on_insertButton_clicked();
    void on_removeButton_clicked();
    void on_clearTableButton_clicked();

private:
    Ui::MainWindow       *ui;
    MainWindowController *controller;
};
