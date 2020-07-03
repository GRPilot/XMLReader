#include "XMLFileReaderController.hpp"

XMLFileReaderController::XMLFileReaderController(QObject *parent)
    : QObject{ parent },
      mReader{ new XMLFileReader }
{ }

void XMLFileReaderController::load(const QString &dir) {
    QDir directory{ dir };

    if (!directory.exists()) {
        emit loadComplete();
        return;
    }

    directory.setFilter(QDir::Files);

    if (directory.entryList().isEmpty()) {
        emit loadComplete();
        return;
    }


    emit sendCountOfFiles(directory.entryList().size() - 1);
    emit showStatusWindow();

    for (const QString& fileName : directory.entryList()) {
        std::pair<TextEditor, XMLFileReader::ErrorData> dataAndErrors {
            mReader->loadFromFile(dir + "/" + fileName)
        };

        auto data{ dataAndErrors.first };
        auto errors{ dataAndErrors.second };

        emit sendFileAndStatus(fileName, errors.ErrorStrings);

        if (errors.ErrorCodes.back() == XMLFileReader::Error_t::NO_ERRORS)
            emit componentLoaded(data);
    }

    emit loadComplete();
}

XMLFileReaderController::~XMLFileReaderController() {
    delete mReader;
}
