#include "parttext.h"
#include "texteditor.h"

PartText::PartText(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsTextItem(parent, scene), CustomElementPart(), can_check_changes(true) {
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setPlainText(tr("T"));
	infos = new TextEditor(this);
}

PartText::~PartText() {
	qDebug() << "~PartText()";
	delete infos;
}

void PartText::fromXml(const QDomElement &xml_element) {
	bool ok;
	int font_size = xml_element.attribute("size").toInt(&ok);
	if (!ok || font_size < 1) font_size = 20;
	
	setFont(QFont(QString("Sans Serif"), font_size));
	setPlainText(xml_element.attribute("text"));
	setPos(
		xml_element.attribute("x").toDouble(),
		xml_element.attribute("y").toDouble()
	);
}

const QDomElement PartText::toXml(QDomDocument &xml_document) const {
	QDomElement xml_element = xml_document.createElement("text");
	xml_element.setAttribute("x", QString("%1").arg(pos().x()));
	xml_element.setAttribute("y", QString("%1").arg(pos().y()));
	xml_element.setAttribute("text", toPlainText());
	xml_element.setAttribute("size", font().pointSize());
	return(xml_element);
}

QWidget *PartText::elementInformations() {
	return(infos);
}

/**
	Retourne la position du texte, l'origine etant le point en bas a gauche du
	texte (et pas du cadre)
	@return la position du texte
*/
QPointF PartText::pos() const {
	return(QGraphicsTextItem::pos() + margin());
}

void PartText::setPos(const QPointF &left_corner_pos) {
	QGraphicsTextItem::setPos(left_corner_pos - margin());
}

void PartText::setPos(qreal x, qreal y) {
	QGraphicsTextItem::setPos(QPointF(x, y) - margin());
}

/**
	@return Les coordonnees du point situe en bas a gauche du texte.
*/
QPointF PartText::margin() const {
	QFont used_font = font();
	QFontMetrics qfm(used_font);
	QPointF margin(
		(boundingRect().width () - qfm.width(toPlainText())) / 2.0,
		((boundingRect().height() - used_font.pointSizeF()) / 3.0) + used_font.pointSizeF()
	);
	return(margin);
}

/**
	Permet a l'element texte de redevenir deplacable a la fin de l'edition de texte
	@param e Le QFocusEvent decrivant la perte de focus
*/
void PartText::focusOutEvent(QFocusEvent *e) {
	QGraphicsTextItem::focusOutEvent(e);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

/**
	Permet a l'element texte de devenir editable lorsqu'on double-clique dessus
	@param e Le QGraphicsSceneMouseEvent qui decrit le double-clic
*/
void PartText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e) {
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
	setTextInteractionFlags(Qt::TextEditorInteraction);
	QGraphicsTextItem::mouseDoubleClickEvent(e);
	setFocus(Qt::MouseFocusReason);
}

QVariant PartText::itemChange(GraphicsItemChange change, const QVariant &value) {
	if (scene() && can_check_changes) {
		if (change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemSelectedChange) {
			infos -> updateForm();
		}
	}
	return(QGraphicsTextItem::itemChange(change, value));
}

QRectF PartText::boundingRect() const {
	QRectF r = QGraphicsTextItem::boundingRect();
	r.adjust(0.0, -2.0, 0.0, 0.0);
	return(r);
}