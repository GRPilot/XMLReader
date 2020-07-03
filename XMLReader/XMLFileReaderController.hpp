#pragma once

#include <QFileDialog>

#include "statusloadingwindow.hpp"
#include "XMLFileReader.hpp"
#include "TextEditor.hpp"

class XMLFileReaderController : public QObject {
    Q_OBJECT;

    using TextEditors = QList<TextEditor>;
public:
    XMLFileReaderController(QObject *parent = nullptr);
    ~XMLFileReaderController();

signals:
    void loadComplete();
    void componentLoaded(const TextEditor &component);

    void showStatusWindow();
    void sendCountOfFiles(int count);
    void sendFileAndStatus(const QString &filename, const QStringList &status);

public slots:
    void load(const QString &dir);

private:
    XMLFileReader       *mReader;

};

