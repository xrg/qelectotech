/*
	Copyright 2006-2013 The QElectroTech Team
	This file is part of QElectroTech.
	
	QElectroTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.
	
	QElectroTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with QElectroTech.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef STYLE_EDITOR_H
#define STYLE_EDITOR_H
#include <QtGui>
#include "elementitemeditor.h"
class CustomElementGraphicPart;
/**
	This class provides a widget to edit styles (color, pen style and thickness,
	filling, antialiasing) common to most primitives within the element editor.
	Its appendWidget() method makes the insertion of another widget below it
	easier.
*/
class StyleEditor : public ElementItemEditor {
	Q_OBJECT
	// constructors, destructor
	public:
	StyleEditor(QETElementEditor *, CustomElementGraphicPart * = 0, QWidget * = 0);
	virtual ~StyleEditor();
	
	private:
	StyleEditor(const StyleEditor &);
	
	// attributes
	private:
	CustomElementGraphicPart *part;
	QVBoxLayout *main_layout;
	QButtonGroup *style, *weight;
	QRadioButton *black_color, *white_color,  *normal_style, *dashed_style, *dashdotted_style, *dotted_style, *green_color, *red_color, *blue_color;
	QRadioButton *none_weight, *thin_weight, *normal_weight, *no_filling;
	QRadioButton *black_filling, *white_filling, *green_filling, *red_filling, *blue_filling;
	QCheckBox *antialiasing;
	QComboBox *filling_color, *outline_color;
	
	// methods
	public:
	virtual bool setPart(CustomElementPart *);
	virtual CustomElementPart *currentPart() const;
	
	public slots:
	void updatePart();
	void updateForm();
	void updatePartAntialiasing();
	void updatePartColor();
	void updatePartLineStyle();
	void updatePartLineWeight();
	void updatePartFilling();
	
	private:
	void activeConnections(bool);
};
#endif
