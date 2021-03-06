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
#ifndef BORDERTITLEBLOCK_H
#define BORDERTITLEBLOCK_H
#include "diagramcontext.h"
#include "titleblockproperties.h"
#include "borderproperties.h"
#include <QObject>
#include <QRectF>
#include <QDate>
class QPainter;
class DiagramPosition;
class TitleBlockTemplate;
class TitleBlockTemplateRenderer;
/**
	This class represents the border and the titleblock which frame a
	particular electric diagram.
*/
class BorderTitleBlock : public QObject {
	Q_OBJECT
	
	// constructors, destructor
	public:
	BorderTitleBlock(QObject * = 0);
	virtual ~BorderTitleBlock();
	
	private:
	BorderTitleBlock(const BorderTitleBlock &);
	
	// methods
	public:
	static int   minNbColumns();
	static qreal minColumnsWidth();
	static int   minNbRows();
	static qreal minRowsHeight();
	
	void draw(QPainter *, qreal = 0.0, qreal = 0.0);
	
	// methods to get dimensions
	// columns
	/// @return the number of columns
	int   columnsCount() const { return(columns_count_); }
	/// @return the columns width, in pixels
	qreal columnsWidth() const { return(columns_width_); }
	/// @return the total width of all columns, headers excluded
	qreal columnsTotalWidth() const { return(columns_count_ * columns_width_); }
	/// @return the column headers height, in pixels
	qreal columnsHeaderHeight() const { return(columns_header_height_); }
	
	// rows
	/// @return the number of rows
	int rowsCount() const { return(rows_count_); }
	/// @return the rows height, in pixels
	qreal rowsHeight() const { return(rows_height_); }
	/// @return the total height of all rows, headers excluded
	qreal rowsTotalHeight() const { return(rows_count_ * rows_height_); }
	/// @return la rows header width, in pixels
	qreal rowsHeaderWidth() const { return(rows_header_width_); }
	
	// border - title block = diagram
	/// @return the diagram width, i.e. the width of the border without title block
	qreal diagramWidth() const { return(columnsTotalWidth() + rowsHeaderWidth()); }
	/// @return the diagram height, i.e. the height of the border without title block
	qreal diagramHeight() const { return(rowsTotalHeight() + columnsHeaderHeight()); }
	
	// title block
	/// @return the title block width
	qreal titleBlockWidth()  const { return(titleblock_width_); }
	qreal titleBlockHeight() const;
	
	// border + title block
	/// @return the border width
	qreal borderWidth()  const { return(diagramWidth()); }
	/// @return the border height
	qreal borderHeight() const { return(diagramHeight() + titleBlockHeight()); }
	
	// methods to get title block basic data
	/// @return the value of the title block "Author" field
	QString author() const { return(btb_author_); }
	/// @return the value of the title block "Date" field
	QDate date() const { return(btb_date_); }
	/// @return the value of the title block "Title" field
	QString title() const { return(btb_title_); }
	/// @return the value of the title block "Folio" field
	QString folio() const { return(btb_folio_); }
	/// @return the value of the title block "File" field
	QString fileName() const { return(btb_filename_); }
	
	// methods to get display options
	/// @return true si le cartouche est affiche, false sinon
	bool titleBlockIsDisplayed() const { return(display_titleblock_); }
	/// @return true si les entetes des colonnes sont affiches, false sinon
	bool columnsAreDisplayed() const { return(display_columns_); }
	/// @return true si les entetes des lignes sont affiches, false sinon
	bool rowsAreDisplayed() const { return(display_rows_); }
	/// @return true si la bordure est affichee, false sinon
	bool borderIsDisplayed() const { return(display_border_); }
	
	// methods to set dimensions
	void addColumn();
	void addRow();
	void removeColumn();
	void removeRow();
	void setColumnsCount(int);
	void setRowsCount(int);
	void setColumnsWidth(const qreal &);
	void setRowsHeight(const qreal &);
	void setColumnsHeaderHeight(const qreal &);
	void setRowsHeaderWidth(const qreal &);
	void setDiagramHeight(const qreal &);
	void setTitleBlockWidth(const qreal &);
	void adjustTitleBlockToColumns();
	
	DiagramPosition convertPosition(const QPointF &);
	
	// methods to set title block basic data
	/// @param author the new value of the "Author" field
	void setAuthor(const QString &author) { btb_author_ = author; }
	/// @param author the new value of the "Date" field
	void setDate(const QDate &date) { btb_date_ = date; }
	/// @param author the new value of the "Title" field
	void setTitle(const QString &title) {
		if (btb_title_ != title) {
			btb_title_ = title;
			emit(diagramTitleChanged(title));
		}
	}
	/// @param author the new value of the "Folio" field
	void setFolio(const QString &folio) { btb_folio_ = folio; }
	void setFolioData(int, int, const DiagramContext & = DiagramContext());
	/// @param author the new value of the "File" field
	void setFileName(const QString &filename) { btb_filename_ = filename; }
	
	void titleBlockToXml(QDomElement &);
	void titleBlockFromXml(const QDomElement &);
	void borderToXml(QDomElement &);
	void borderFromXml(const QDomElement &);
	
	TitleBlockProperties exportTitleBlock();
	void importTitleBlock(const TitleBlockProperties &);
	BorderProperties exportBorder();
	void importBorder(const BorderProperties &);
	
	const TitleBlockTemplate *titleBlockTemplate();
	void setTitleBlockTemplate(const TitleBlockTemplate *);
	QString titleBlockTemplateName() const;
	
	public slots:
	void titleBlockTemplateChanged(const QString &);
	void titleBlockTemplateRemoved(const QString &, const TitleBlockTemplate * = 0);
	
	// methods to set display options
	void displayTitleBlock(bool);
	void displayColumns(bool);
	void displayRows(bool);
	void displayBorder(bool);
	
	private:
	void updateRectangles();
	void updateDiagramContextForTitleBlock(const DiagramContext & = DiagramContext());
	QString incrementLetters(const QString &);
	
	signals:
	/**
		Signal emitted after the border has changed
		@param old_border Former border
		@param new_border New border
	*/
	void borderChanged(QRectF old_border, QRectF new_border);
	/**
		Signal emitted after display options have changed
	*/
	void displayChanged();
	
	/**
		Signal emitted after the title has changed
	*/
	void diagramTitleChanged(const QString &);
	
	/**
		Signal emitted when the title block requires its data to be updated in order
		to generate the folio field.
	*/
	void needFolioData();
	
	/**
		Signal emitted when this object needs to set a specific title block
		template. This object cannot handle the job since it does not know of
		its parent project.
	*/
	void needTitleBlockTemplate(const QString &);
	
	// attributes
	private:
	// titleblock basic data
	QString btb_author_;
	QDate   btb_date_;
	QString btb_title_;
	QString btb_folio_;
	QString btb_final_folio_;
	int folio_index_;
	int folio_total_;
	QString btb_filename_;
	DiagramContext additional_fields_;
	
	// border dimensions (rows and columns)
	// columns: number and dimensions
	int columns_count_;
	qreal columns_width_;
	qreal columns_header_height_;
	
	// rows: number and dimensions
	int rows_count_;
	qreal rows_height_;
	qreal rows_header_width_;
	
	// title block dimensions
	qreal titleblock_width_;
	qreal titleblock_height_;
	
	// rectangles used for drawing operations
	QRectF diagram_rect_;
	QRectF titleblock_rect_;
	
	// display options
	bool display_titleblock_;
	bool display_columns_;
	bool display_rows_;
	bool display_border_;
	TitleBlockTemplateRenderer *titleblock_template_renderer_;
};
#endif
