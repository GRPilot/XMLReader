#include "DatabaseController.hpp"
#include <QDebug>

DatabaseController::DatabaseController(QObject *parent)
    : QObject{ parent },
      mDatabase{ new Database }
{}

/*** SLOTS ***/

void DatabaseController::rewriteAllData(const DatabaseController::
                                         TextEditors &data) const
{
    mDatabase->clear();

    for (auto item : data)
        mDatabase->insert(item);

    emit saved();
}

void DatabaseController::insert(const TextEditor &item) {
    if (mDatabase->insert(item)) {
        emit saved();
    }
}

void DatabaseController::remove(int position) {
    if (mDatabase->remove(position)) {
        emit saved();
    }
}

void DatabaseController::update(int row, int column, const QVariant &data) {
    if (mDatabase->update(row, column, data)) {
        emit saved();
    }
}

void DatabaseController::loadTextEditors() const {

    auto output{ mDatabase->load() };

    for (auto editor : output)
        emit componentLoaded(editor);

    emit loaded();
}


