#ifndef MAXRECTS_HPP
#define MAXRECTS_HPP

#include "Packer.hpp"

namespace packer {

class MaxRects
{
public:
    MaxRects();
    QList<MaxRectsNode> F;
    QList<QRect> R;
    QList<MaxRectsNode *> FR;
    QPoint insertNode(SpritePtr& node);
    int heuristic, w, h, rotation;
    bool leftToRight;
    Border *border;
};

}

#endif // MAXRECTS_HPP
