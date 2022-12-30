#ifndef EXPORTER_HPP
#define EXPORTER_HPP

#include <QtCore>
#include <QImage>

#include "../packer/ImagePacker.hpp"

namespace exporter {

struct Configurations {
    QString dataFilePath;
    QString outputFormat;
};

enum ExporterErrorCode {
    Export_OK = 0,
    Export_Fail,
    Export_Cannot_Create_File,
};

typedef QSharedPointer<Configurations> ConfigurationsPtr;

class Exporter
{
public:
    Exporter();

    virtual ExporterErrorCode exportSpriteSheet(const packer::ImagePackerPtr& imagePacker, const ConfigurationsPtr& configs) = 0;
};

typedef QSharedPointer<Exporter> ExporterPtr;

}

#endif // EXPORTER_HPP
