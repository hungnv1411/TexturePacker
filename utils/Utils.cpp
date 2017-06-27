#include "Utils.hpp"

QStringList Utils::supportedImageFormats = {"png", "jpg", "jpeg", "gif"};
QStringList Utils::supportedImageFormatFilters = {"*.png", "*.jpg", "*.jpeg", "*.gif"};

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

    if (recursive) {
        QFileInfoList dirs = dir.entryInfoList(QDir::Filter::NoDotAndDotDot | QDir::Dirs);
        for (int i = 0; i < dirs.size(); i++) {
            QFileInfo info = dirs[i];
            QString path = info.absoluteFilePath();
            ret.append(enumerateContentInDirectory(path, recursive));
        }
    }
    return ret;
}

quint32 Utils::hash(quint32 crc, const uchar *buf, size_t len) {
    static quint32 table[256];
    static int have_table = 0;
    quint32 rem, octet;
    const uchar *p, *q;

    /* This check is not thread safe; there is no mutex. */
    if(have_table == 0)
    {
        /* Calculate CRC table. */
        for(int i = 0; i < 256; i++)
        {
            rem = i;  /* remainder from polynomial division */
            for(int j = 0; j < 8; j++)
            {
                if(rem & 1)
                {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                }
                else
                {
                    rem >>= 1;
                }
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;
    q = buf + len;
    for(p = buf; p < q; p++)
    {
        octet = *p;  /* Cast to unsigned octet. */
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}






