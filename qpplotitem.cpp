#include "qpplotitem.h"
#include "qpscaleengine.h"

QPPlotItem::QPPlotItem(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
}

QPPlotItem::~QPPlotItem()
{
}

QRectF QPPlotItem::coordinateBounds() const
{
    return QRectF();
}
