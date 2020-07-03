#pragma once

#include <QItemSelectionModel>
#include <QString>
#include <QThread>
#include <QList>

#include "TextEditor.hpp"
#include "TableModel.hpp"
#include "DatabaseController.hpp"
#include "XMLFileReaderController.hpp"

class MainWindowController : public QObject {
    Q_OBJECT;

    using TextEditors = QList<TextEditor>;
public:

    MainWindowController(QObject *parent = nullptr);
    ~MainWindowController();

    TableModel* model() const;
    bool insertLines(int count = 1);
    bool removeSelected(QItemSelectionModel *const selection);
    bool clearTable();

    bool loadDataFromXml() const;

    bool loadFromDatabase() const;
    bool saveAllToDatabase() const;


signals:
    void loadFromXml(const QString &dir) const;

    void loadFromDB() const;

    void insertToDB(const TextEditor &item);
    void removeFromDB(int position);
    void updateInDB(int row, int column, const QVariant &data);

    void saveAllToDB(const TextEditors &data) const;

public slots:
    void onXmlLoadComplete() const;
    void onXmlLoaded(const TextEditor &component) const;

    void onDBLoadComplete() const;
    void onDBLoaded(const TextEditor &component) const;
    void onDBSaved() const;

private:
    DatabaseController      *mDBController;
    XMLFileReaderController *mXmlController;
    TableModel              *mModel;

    StatusLoadingWindow     *mStatusWindow;

    void connectXmlController();
    void connectDBController();
    void connectTableModel();
};
