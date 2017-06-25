#include "Utils.hpp"

QStringList Utils::supportedImageFormats = {"png", "jpg", "jpeg"};
QStringList Utils::supportedImageFormatFilters = {"*.png", "*.jpg", "*.jpeg"};

bool Utils::isImage(const QString &filePath) {
    QString ext = getFileExtension(filePath).toLower();
    return supportedImageFormats.contains(ext);
}

QString Utils::getFileName(const QString &filePath) {
    int idx = filePath.lastIndexOf(QDir::separator());
    if (idx != -1) {
        return filePath.right(filePath.length() - idx - 1);
    }
    return filePath;
}

QString Utils::getFileExtension(const QString &filePath) {
    int idx = filePath.lastIndexOf(".");
    if (idx != -1) {
        return filePath.right(filePath.length() - idx - 1);
    }
    return "";
}

QStringList Utils::enumerateContentInDirectory(const QString &directory, bool recursive) {
    QDir dir(directory);
    QStringList ret;

    QFileInfoList files = dir.entryInfoList(supportedImageFormatFilters, QDir::Files);
    for (int i = 0; i < files.size(); i++) {
        ret.append(QDir::toNativeSeparators(files[i].absoluteFilePath()));
    }

    QFileInfoList dirs = dir.entryInfoList(QDir::Filter::NoDotAndDotDot | QDir::Dirs);
    for (int i = 0; i < dirs.size(); i++) {
        QFileInfo info = dirs[i];
        QString path = info.absoluteFilePath();
        ret.append(enumerateContentInDirectory(path, recursive));
    }
    return ret;
}




