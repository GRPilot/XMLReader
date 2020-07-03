#pragma once

#include <QAbstractTableModel>
#include "TextEditor.hpp"

class TableModel : public QAbstractTableModel {
    Q_OBJECT

    using TextEditors = QList<TextEditor>;
public:
    TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex{})    const override;
    int columnCount(const QModelIndex &parent = QModelIndex{}) const override;

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section,
                        Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index,
                 const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &index = QModelIndex{}) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &index = QModelIndex{}) override;

    bool setTextEditors(const TextEditors &other);
    bool addTextEditor(const TextEditor &other, bool sendToDB = true);

    TextEditors textEditors() const;
    bool clear(bool sendToDB = true);

signals:
    void itemWasInserted(const TextEditor &item);
    void itemWasRemoved(int position);
    void itemWasUpdated(int row, int column, const QVariant& data);

    void rewriteTable(const TextEditors &newItems);

private:
    TextEditors mData;

    void emitAllDataChanged();

};

