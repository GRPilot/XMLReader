#include "TableModel.hpp"

TableModel::TableModel(QObject* parent)
    : QAbstractTableModel{ parent }
{}

int TableModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return mData.size();
}
int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return TextEditor::size();
}

QVariant TableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return {};

    if (index.row() < 0 || index.row() >= rowCount())
        return {};

    switch (role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return mData.at(index.row()).at(index.column());

        default:
            return {};
    }

    return {};
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return "Text editor";
            case 1: return "File formats";
            case 2: return "Encoding";
            case 3: return "Intellisense";
            case 4: return "Plugins";
            case 5: return "Compiler";
        }
    }
    if (orientation == Qt::Vertical) {
        return section + 1;
    }

    return {};
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid())
        return false;

    int row{ index.row() };
    int column{ index.column() };

    if (row < 0 || row >= rowCount())
        return false;

    if (column < 0 || column >= columnCount())
        return false;

    if (role == Qt::EditRole) {
        TextEditor& editor{ mData[row] };

        switch (column) {
            case 0: editor.texteditor      = value.toString();    break;
            case 1: editor.fileformats     = value.toString();    break;
            case 2: editor.encoding        = value.toString();    break;
            case 3: editor.hasintellisense = value.toBool();      break;
            case 4: editor.hasplugins      = value.toBool();      break;
            case 5: editor.cancompile      = value.toBool();      break;
        }

        emit dataChanged(index, index);
        emit itemWasUpdated(row, column, value);

        return true;
    }
    return false;
}

bool TableModel::insertRows(int position, int rows, const QModelIndex& index) {
    Q_UNUSED(index);

    beginInsertRows({}, position, position + rows - 1);

    for (int row{ position }; row < position + rows; ++row) {
        mData.insert(position, TextEditor{});
        emit itemWasInserted(TextEditor{});
    }

    endInsertRows();

    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex& index) {
    Q_UNUSED(index);

    beginRemoveRows({}, position, position + rows);

    for (int row{ position + rows }; row >= position; --row) {
        mData.removeAt(row);
        emit itemWasRemoved(row + 1);
    }

    endRemoveRows();

    return true;
}

bool TableModel::setTextEditors(const TableModel::TextEditors& other) {
    beginResetModel();

    if (int size{ other.size() }; rowCount() < size) {
        insertRows(rowCount() - 1, size - rowCount());
    } else if (rowCount() > size) {
        removeRows(rowCount() - 1, size - rowCount());
    }

    mData = other;

    endResetModel();
    emitAllDataChanged();

    return true;
}

bool TableModel::addTextEditor(const TextEditor& other, bool sendToDB) {
    beginResetModel();

    mData.push_back(other);
    int row{ mData.size() };

    endResetModel();

    emit dataChanged(index(row, 0), index(row, columnCount() - 1));

    if (sendToDB)
        emit itemWasInserted(mData.back());

    return true;
}

TableModel::TextEditors TableModel::textEditors() const {
    return mData;
}

bool TableModel::clear(bool sendToDB) {
    beginRemoveRows({}, 0, rowCount() - 1);
    mData.clear();
    endRemoveRows();

    if (sendToDB) emit rewriteTable(mData);

    emitAllDataChanged();

    return true;
}

void TableModel::emitAllDataChanged() {
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}
