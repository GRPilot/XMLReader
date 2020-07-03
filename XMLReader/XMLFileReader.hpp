#pragma once

#include <QXmlStreamReader>
#include <fstream>

#include "TextEditor.hpp"

class XMLFileReader {
public:
    enum class Error_t {
        NO_ERRORS,

        E_FileWasNotOpened,
        E_InvalidFileExtension,

        E_NoClosingTag,
        E_DifferentOpeningAndClosingTags,

        E_UnknownError,
    };
    struct ErrorData {
        QStringList    ErrorStrings;
        QList<Error_t> ErrorCodes;

        void setError(const QString &eStr, const QString& additionalInfo = "") {

            ErrorStrings.push_back(eStr + additionalInfo);

            if (eStr == "OK") {
                ErrorCodes.push_back(Error_t::NO_ERRORS);
                return;
            }

            if (eStr == "Expected character data.") {
                ErrorStrings.back().append(" There is no closing tag! ");
                ErrorCodes.push_back(Error_t::E_NoClosingTag);
                return;
            }

            if (eStr == "Opening and ending tag mismatch.") {
                ErrorStrings.back().append(" There is different names of "
                                           "opening teg and closing teg! ");
                ErrorCodes.push_back(Error_t::E_DifferentOpeningAndClosingTags);
                return;
            }

            if (eStr == "Invalid tag name.") {
                ErrorCodes.push_back(Error_t::E_DifferentOpeningAndClosingTags);
                return;
            }

            if (eStr == "File was not opened.") {
                ErrorCodes.push_back(Error_t::E_FileWasNotOpened);
                return;
            }
            if (eStr == "Invalid file extension.") {
                ErrorCodes.push_back(Error_t::E_InvalidFileExtension);
                return;
            }

            ErrorCodes.push_back(Error_t::E_UnknownError);
        }
    };

    XMLFileReader();

    std::pair<TextEditor, ErrorData> loadFromFile(const QString &filePath) const;

private:

    std::pair<TextEditor, ErrorData> readXML(const QString &data)              const;
    std::pair<QString, ErrorData>    readDataFromFile(const QString &filePath) const;
};

