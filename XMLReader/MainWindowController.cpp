#include "MainWindowController.hpp"

#include <QDebug>

MainWindowController::MainWindowController(QObject *parent)
    : QObject{ parent },
      mDBController { new DatabaseController      },
      mXmlController{ new XMLFileReaderController },
      mModel        { new TableModel    },
      mStatusWindow { new StatusLoadingWindow }
{
    connectXmlController();
    connectDBController();
    connectTableModel();
}

MainWindowController::~MainWindowController() {
    delete mDBController;
    delete mXmlController;
    delete mModel;
    delete mStatusWindow;
}

/*** MODEL ***/

TableModel* MainWindowController::model() const {
    return mModel;
}

bool MainWindowController::insertLines(int count) {
    return mModel->insertRows(mModel->rowCount(), count);
}

bool MainWindowController::removeSelected(QItemSelectionModel *const selection) {
    if (!selection->hasSelection())
        return false;

    int row{ selection->selection().indexes().first().row() };
    int count{ selection->selection().indexes().last().row() - row };

    return mModel->removeRows(row, count);
}

bool MainWindowController::clearTable() {
    return mModel->clear();
}

/*** XML ***/

bool MainWindowController::loadDataFromXml() const {
    QString filePath{
        QFileDialog::getExistingDirectory(nullptr, "Select the directory of .xml files.", "*.xml")
    };

    if (filePath.isEmpty())
        return false;

    mModel->clear();
    emit loadFromXml(filePath);

    return true;
}

/*** DATABASE ***/

bool MainWindowController::loadFromDatabase() const {
    mModel->clear(false);

    emit loadFromDB();

    return true;
}

bool MainWindowController::saveAllToDatabase() const {
    auto editors{ mModel->textEditors() };

    if (editors.isEmpty())
        return false;

    emit saveAllToDB(editors);

    return true;
}

/*** SLOTS XML ***/

void MainWindowController::onXmlLoadComplete() const {
    // Message about a successfuly loading
    qDebug() << "XML files was successfuly loaded!";
}

void MainWindowController::onXmlLoaded(const TextEditor& component) const {
    mModel->addTextEditor(component);
}

/*** SLOTS DATABASE ***/

void MainWindowController::onDBLoadComplete() const {
    // Message about a successfuly loading
    qDebug() << "Database was successfuly loaded";
}

void MainWindowController::onDBLoaded(const TextEditor& component) const {
    mModel->addTextEditor(component, false);
}

void MainWindowController::onDBSaved() const {
    // Message about a successfuly saving
    qDebug() << "Saved successfuly";
}

/*** Connecting and setting XML controller and its thread ***/
void MainWindowController::connectXmlController() {

    QThread *mXmlControllerThread = new QThread;

    connect(this,                 &MainWindowController::destroyed,
            mXmlControllerThread, &QThread::deleteLater);


    connect(this,           &MainWindowController::loadFromXml,
            mXmlController, &XMLFileReaderController::load);

    connect(mXmlController, &XMLFileReaderController::loadComplete,
            this,           &MainWindowController::onXmlLoadComplete);

    connect(mXmlController, &XMLFileReaderController::componentLoaded,
            this,           &MainWindowController::onXmlLoaded);


    connect(mXmlController, &XMLFileReaderController::sendCountOfFiles,
            mStatusWindow,  &StatusLoadingWindow::sendedCountOfFiles);

    connect(mXmlController, &XMLFileReaderController::sendFileAndStatus,
            mStatusWindow,  &StatusLoadingWindow::sendedFileAndStatus);

    connect(mXmlController, &XMLFileReaderController::showStatusWindow,
            mStatusWindow,  &StatusLoadingWindow::onEmitedShowWindow);


    mXmlController->moveToThread(mXmlControllerThread);
    mXmlControllerThread->start();
}

/*** Connecting and setting Database controller and its thread ***/
void MainWindowController::connectDBController() {

    QThread *mDBControllerThread = new QThread;

    connect(this,                &MainWindowController::destroyed,
            mDBControllerThread, &QThread::deleteLater);


    connect(this,          &MainWindowController::loadFromDB,
            mDBController, &DatabaseController::loadTextEditors);

    connect(this,          &MainWindowController::insertToDB,
            mDBController, &DatabaseController::insert);

    connect(this,          &MainWindowController::removeFromDB,
            mDBController, &DatabaseController::remove);

    connect(this,          &MainWindowController::updateInDB,
            mDBController, &DatabaseController::update);

    connect(this,          &MainWindowController::saveAllToDB,
            mDBController, &DatabaseController::rewriteAllData);


    connect(mDBController, &DatabaseController::saved,
            this,          &MainWindowController::onDBSaved);

    connect(mDBController, &DatabaseController::loaded,
            this,          &MainWindowController::onDBLoadComplete);

    connect(mDBController, &DatabaseController::componentLoaded,
            this,          &MainWindowController::onDBLoaded);

    mDBController->moveToThread(mDBControllerThread);
    mDBControllerThread->start();
}

/*** Connecting and setting TableModel with Database ***/
void MainWindowController::connectTableModel() {

    connect(mModel, &TableModel::rewriteTable,
            this,   &MainWindowController::saveAllToDB);

    connect(mModel, &TableModel::itemWasInserted,
            this,   &MainWindowController::insertToDB);

    connect(mModel, &TableModel::itemWasRemoved,
            this,   &MainWindowController::removeFromDB);

    connect(mModel, &TableModel::itemWasUpdated,
            this,   &MainWindowController::updateInDB);
}

