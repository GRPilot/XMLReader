#include "XMLFileReader.hpp"

XMLFileReader::XMLFileReader() {

}

std::pair<TextEditor, XMLFileReader::ErrorData> XMLFileReader::loadFromFile(const QString &filePath) const {
    std::pair dataFromFile{ readDataFromFile(filePath) };
    auto data{ dataFromFile.first };
    auto error{ dataFromFile.second };

    if (error.ErrorCodes.back() != Error_t::NO_ERRORS)
        return std::make_pair(TextEditor{}, error);

    return readXML(data);
}

std::pair<TextEditor, XMLFileReader::ErrorData> XMLFileReader::readXML(const QString& data) const {

    QXmlStreamReader reader(data);
    ErrorData resultErrorData;
    bool hasError{ false };

    if (reader.hasError()) {
        resultErrorData.setError(reader.errorString());
        return std::make_pair(TextEditor{}, resultErrorData);
    }

    QString dataBetweenTags;
    TextEditor result;

    while (!reader.atEnd() && !hasError) {
        if (!reader.isStartElement() || reader.name() == "root") {
            reader.readNext();
            continue;
        }

        QString nameOfTag{ reader.name().toString() };

        while (!reader.isEndElement()) {
            dataBetweenTags = reader.readElementText();

            if (reader.hasError()) {
                QString errStr{ reader.errorString() };

                resultErrorData.setError(reader.errorString());
                hasError = true;
                break;
            }

            if (!result.setDataFromString(nameOfTag, dataBetweenTags)) {
                resultErrorData.setError("Invalid tag name.", " <" + nameOfTag + "> ");
                hasError = true;
            }
        }
    }

    if (!hasError)
        resultErrorData.setError("OK");

    return std::make_pair(result, resultErrorData);
}

std::pair<QString, XMLFileReader::ErrorData> XMLFileReader::readDataFromFile(const QString& filePath) const {
    if (filePath.isEmpty())
        return {};

    ErrorData errors;

    if (QString ext{ filePath.right(4) }; ext != ".xml") {
        errors.setError("Invalid file extension.");
        return std::make_pair(QString{}, errors);
    }

    std::ifstream file(filePath.toStdString());

    if (!file.is_open()) {
        errors.setError("File was not opened.");
        return std::make_pair(QString{}, errors);
    }


    QString outData;

    for (std::string buffer;
         std::getline(file, buffer, '\n');
         outData.append(QString::fromStdString(buffer))
        );

    file.close();

    errors.setError("OK");

    return std::make_pair(outData, errors);
}
