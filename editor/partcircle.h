#ifndef PART_CIRCLE_H
#define PART_CIRCLE_H
#include <QtGui>
#include "customelementgraphicpart.h"
class CircleEditor;
class PartCircle : public QGraphicsEllipseItem, public CustomElementGraphicPart {
	// constructeurs, destructeur
	public:
	PartCircle(QGraphicsItem * = 0, QGraphicsScene * = 0);
	virtual ~PartCircle() {
		qDebug() << "~PartCircle()";
	}
	
	private:
	PartCircle(const PartCircle &);
	
	// attributs
	private:
	CircleEditor *informations;
	
	// methodes
	public:
	virtual void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget * = 0);
	virtual const QDomElement toXml(QDomDocument &) const;
	virtual void fromXml(const QDomElement &);
	virtual QPointF sceneTopLeft() const;
	virtual QRectF boundingRect() const;
	QPointF sceneCenter() const;
	
	protected:
	QVariant itemChange(GraphicsItemChange, const QVariant &);
};
#endif