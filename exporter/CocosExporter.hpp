#ifndef COCOSEXPORTER_HPP
#define COCOSEXPORTER_HPP

#include "Exporter.hpp"

namespace exporter {

class CocosExporter : public Exporter
{
public:
    CocosExporter();

    ExporterErrorCode exportSpriteSheet(const packer::ImagePackerPtr& imagePacker, const ConfigurationsPtr& configs);
};

}
#endif // COCOSEXPORTER_HPP
