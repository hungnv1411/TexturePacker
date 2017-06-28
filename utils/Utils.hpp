#ifndef UTILS_HPP
#define UTILS_HPP

#include <QtCore>

class Utils
{
public:
    static bool isImage(const QString& filePath);
    static QString getFileName(const QString& filePath);
    static QString getContainingDirectory(const QString& filePath);
    static QString getFileExtension(const QString& filePath);
    static QStringList enumerateContentInDirectory(const QString& directory, bool recursive = false);

    static quint32 hash(quint32 crc, const uchar *buf, size_t len);

    static QRect crop(const QImage& img, int cropThreshold);

public:
    static QStringList supportedImageFormats;
    static QStringList supportedImageFormatFilters;
};

#endif // UTILS_HPP
