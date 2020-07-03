#pragma once

#include <QtSql>

#include "TextEditor.hpp"

class Database {
    using TextEditors = QList<TextEditor>;

public:
    Database(const QString& dbname = "editors.sqlite");

    bool insert(const TextEditor &item);
    bool update(int row, int column, const QVariant& data);
    bool remove(int position);
    bool clear();

    TextEditors load();

private:
    QSqlDatabase mDatabase;
    QString      mDBname;

    QString getColumnName(int column) const;

    bool createTableIfNoExists()    const;

    bool open();
    bool close();

    bool exec(const QString &queryStr);
};

