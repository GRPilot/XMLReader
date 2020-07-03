#pragma once

#include <QObject>

#include "TextEditor.hpp"
#include "Database.hpp"

class DatabaseController : public QObject {
    Q_OBJECT

    using TextEditors = QList<TextEditor>;
public:
    DatabaseController(QObject *parent = nullptr);

signals:
    void saved() const;
    void loaded() const;
    void componentLoaded(const TextEditor &component) const;

public slots:

    void rewriteAllData(const TextEditors &data) const;

    void insert(const TextEditor &item);
    void remove(int position);
    void update(int row, int column, const QVariant &data);

    void loadTextEditors() const;


private:
    Database *mDatabase;
};
