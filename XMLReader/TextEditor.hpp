#pragma once

#include <QString>
#include <QVariant>

struct TextEditor {
    QString texteditor  { "<unknown>" };
    QString fileformats { "<none>" };
    QString encoding    { "<none>" };
    bool hasintellisense{ false };
    bool hasplugins     { false };
    bool cancompile     { false };

    static int size() { return 6; }

    QVariant at(int index) const {
        switch (index) {
            case 0: return texteditor;
            case 1: return fileformats;
            case 2: return encoding;
            case 3: return hasintellisense;
            case 4: return hasplugins;
            case 5: return cancompile;
        }
        return {};
    }

    bool setDataFromString(const QString &tagName, const QString &data) {
        if (tagName == "texteditor") {
            texteditor = data;
            return true;
        }

        if (tagName == "fileformats") {
            fileformats = data;
            return true;
        }

        if (tagName == "encoding") {
            encoding = data;
            return true;
        }

        if (tagName == "hasintellisense") {
            hasintellisense = data == "true";
            return true;
        }

        if (tagName == "hasplugins") {
            hasplugins = data == "true";
            return true;
        }

        if (tagName == "cancompile") {
            cancompile = data == "true";
            return true;
        }

        return false;
    }
};
