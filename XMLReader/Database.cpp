#include "Database.hpp"
#include <QMessageBox>

Database::Database(const QString& dbname)
    : mDatabase{ QSqlDatabase::addDatabase("QSQLITE") },
      mDBname  { dbname }
{
    mDatabase.setDatabaseName(mDBname);

    if (!open()) {
        QMessageBox::critical(nullptr, "Error!", "Cannot create or open database!");
        return;
    }

    close();

    createTableIfNoExists();
}

bool Database::insert(const TextEditor& item) {

    QString queryStr{
        QString{
            "INSERT INTO TextEditors(texteditor, fileformats, encoding, hasintellisense, hasplugins, cancompile)"
            "VALUES ('%1', '%2', '%3', '%4', '%5', '%6');"
        }.arg(item.texteditor     ).arg(item.fileformats).arg(item.encoding  )
         .arg(item.hasintellisense).arg(item.hasplugins ).arg(item.cancompile)
    };

    return exec(queryStr);
}

bool Database::update(int row, int column, const QVariant& data) {
    QString queryStr{
        QString{"UPDATE TextEditors SET %1='%2' WHERE rowid=%3;"}
        .arg(getColumnName(column)).arg(data.toString()).arg(row + 1)
    };

    return exec(queryStr);
}

bool Database::remove(int position) {
    QString query{
        QString{"DELETE FROM TextEditors WHERE rowid=%1;"}.arg(position)
    };

    return exec(query);
}

bool Database::clear() {
    return exec("DELETE FROM TextEditors;");
}

Database::TextEditors Database::load() {
    QString queryStr{ "SELECT * FROM TextEditors;" };

    TextEditors editors;

    open();

    QSqlQuery query{ mDatabase };
    query.exec(queryStr);

    while (query.next()) {
        TextEditor editor{
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toString(),
            query.value(3).toBool(),
            query.value(4).toBool(),
            query.value(5).toBool()
        };

        editors.push_back(editor);
    }

    close();

    return editors;
}

QString Database::getColumnName(int column) const {
    switch (column) {
        case 0: return "texteditor";
        case 1: return "fileformats";
        case 2: return "encoding";
        case 3: return "hasintellisense";
        case 4: return "hasplugins";
        case 5: return "cancompile";
    }

    return {};
}

bool Database::open() {
    if (!mDatabase.isOpen())
        return mDatabase.open();

    return true;
}

bool Database::close() {
    if (mDatabase.isOpen())
        mDatabase.close();

    return true;
}

bool Database::exec(const QString &queryStr) {
    open();

    QSqlQuery query{mDatabase};
    bool status{ query.exec(queryStr) };

    close();

    return status;
}

bool Database::createTableIfNoExists() const {
    QSqlQuery query{mDatabase};

    QString queryStr{
        "CREATE TABLE if not exists TextEditors("
        "texteditor      STRING,"
        "fileformats     STRING,"
        "encoding        STRING,"
        "hasintellisense BOOLEAN NOT NULL DEFAULT (false),"
        "hasplugins      BOOLEAN NOT NULL DEFAULT (false),"
        "cancompile      BOOLEAN NOT NULL DEFAULT (false) "
        ");"
    };

    bool status{ query.exec(queryStr) };

    return status;
}
