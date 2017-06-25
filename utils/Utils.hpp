#ifndef UTILS_HPP
#define UTILS_HPP

#include <QtCore>

class Utils
{
public:
    static bool isImage(const QString& filePath);
    static QString getFileName(const QString& filePath);
    static QString getFileExtension(const QString& filePath);
    static QStringList enumerateContentInDirectory(const QString& directory, bool recursive = false);

public:
    static QStringList supportedImageFormats;
    static QStringList supportedImageFormatFilters;
};

#endif // UTILS_HPP
