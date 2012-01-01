/*
	Copyright 2006-2012 Xavier Guerrin
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
#include "templateview.h"
#include "templatevisualcell.h"
#include "gridlayoutanimation.h"
#include "helpercell.h"
#include "splittedhelpercell.h"
#include "templatecommands.h"
#include "templatecellsset.h"
#include "dimensionwidget.h"
#include "qetapp.h"
#define ROW_OFFSET 2
#define COL_OFFSET 1
#define DEFAULT_PREVIEW_WIDTH 600
#define DEFAULT_PREVIEW_HELPER_CELL_HEIGHT 15
#define DEFAULT_COLS_HELPER_CELLS_HEIGHT   15
#define DEFAULT_ROWS_HELPER_CELLS_WIDTH    50



/**
	Constructor
	@param parent Parent QWidget.
*/
TitleBlockTemplateView::TitleBlockTemplateView(QWidget *parent) :
	QGraphicsView(parent),
	tbtemplate_(0),
	tbgrid_(0),
	form_(0),
	preview_width_(DEFAULT_PREVIEW_WIDTH),
	apply_columns_widths_count_(0),
	apply_rows_heights_count_(0),
	first_activation_(true)
{
	init();
}

/**
	Constructor
	@param parent Parent QWidget.
*/
TitleBlockTemplateView::TitleBlockTemplateView(QGraphicsScene *scene, QWidget *parent) :
	QGraphicsView(scene, parent),
	tbtemplate_(0),
	tbgrid_(0),
	preview_width_(DEFAULT_PREVIEW_WIDTH),
	apply_columns_widths_count_(0),
	apply_rows_heights_count_(0),
	first_activation_(true)
{
	init();
}

/**
	Destructor
*/
TitleBlockTemplateView::~TitleBlockTemplateView() {
}

/**
	@param tbtemplate Title block template to be rendered by this view.
	If set to zero, the View will render nothing.
*/
void TitleBlockTemplateView::setTitleBlockTemplate(TitleBlockTemplate *tbtemplate) {
	loadTemplate(tbtemplate);
	zoomFit();
}

/**
	@return The title block template object rendered by this view.
*/
TitleBlockTemplate *TitleBlockTemplateView::titleBlockTemplate() const {
	return(tbtemplate_);
}

/**
	Emits the selectedCellsChanged() signal with the currently selected cells.
*/
void TitleBlockTemplateView::selectionChanged() {
	emit(selectedCellsChanged(selectedCells()));
}

/**
	Zoom in by zoomFactor().
	@see zoomFactor()
*/
void TitleBlockTemplateView::zoomIn() {
	scale(zoomFactor(), zoomFactor());
}

/**
	Zoom out by zoomFactor().
	@see zoomFactor()
*/
void TitleBlockTemplateView::zoomOut() {
	qreal zoom_factor = 1.0/zoomFactor();
	scale(zoom_factor, zoom_factor);
}

/**
	Fit the rendered title block template in this view.
*/
void TitleBlockTemplateView::zoomFit() {
	adjustSceneRect();
	fitInView(scene() -> sceneRect(), Qt::KeepAspectRatio);
}

/**
	Reset the zoom level.
*/
void TitleBlockTemplateView::zoomReset() {
	adjustSceneRect();
	resetMatrix();
}

/**
	Add a column right before the last index selected when calling the context
	menu.
*/
void TitleBlockTemplateView::addColumnBefore() {
	int index = lastContextMenuCellIndex();
	if (index == -1) return;
	requestGridModification(ModifyTemplateGridCommand::addColumn(tbtemplate_, index));
}

/**
	Add a row right before the last index selected when calling the context
	menu.
*/
void TitleBlockTemplateView::addRowBefore() {
	int index = lastContextMenuCellIndex();
	if (index == -1) return;
	requestGridModification(ModifyTemplateGridCommand::addRow(tbtemplate_, index));
}

/**
	Add a column right after the last index selected when calling the context
	menu.
*/
void TitleBlockTemplateView::addColumnAfter() {
	int index = lastContextMenuCellIndex();
	if (index == -1) return;
	requestGridModification(ModifyTemplateGridCommand::addColumn(tbtemplate_, index + 1));
}

/**
	Add a row right after the last index selected when calling the context
	menu.
*/
void TitleBlockTemplateView::addRowAfter() {
	int index = lastContextMenuCellIndex();
	if (index == -1) return;
	requestGridModification(ModifyTemplateGridCommand::addRow(tbtemplate_, index + 1));
}

/**
	Edit the width of a column.
	@param cell (optional) HelperCell of the column to be modified. If 0, this
	method uses the last index selected when calling the context menu.
*/
void TitleBlockTemplateView::editColumn(HelperCell *cell) {
	int index = cell ? cell -> index : lastContextMenuCellIndex();
	if (index == -1) return;
	
	TitleBlockDimension dimension_before = tbtemplate_ -> columnDimension(index);
	TitleBlockDimensionWidget dialog(true, this);
	dialog.setWindowTitle(tr("Changer la largeur de la colonne", "window title when changing a column with"));
	dialog.label() -> setText(tr("Largeur :", "text before the spinbox to change a column width"));
	dialog.setValue(dimension_before);
	if (dialog.exec() == QDialog::Accepted) {
		ModifyTemplateDimension *command = new ModifyTemplateDimension(tbtemplate_);
		command -> setType(false);
		command -> setIndex(index);
		command -> setDimensionBefore(dimension_before);
		command -> setDimensionAfter(dialog.value());
		requestGridModification(command);
	}
}

/**
	Edit the height of a row.
	@param cell (optional) HelperCell of the row to be modified. If 0, this
	method uses the last index selected when calling the context menu.
*/
void TitleBlockTemplateView::editRow(HelperCell *cell) {
	int index = cell ? cell -> index : lastContextMenuCellIndex();
	if (index == -1) return;
	
	TitleBlockDimension dimension_before = TitleBlockDimension(tbtemplate_ -> rowDimension(index));
	TitleBlockDimensionWidget dialog(false, this);
	dialog.setWindowTitle(tr("Changer la hauteur de la ligne", "window title when changing a row height"));
	dialog.label() -> setText(tr("Hauteur :", "text before the spinbox to change a row height"));
	dialog.setValue(dimension_before);
	if (dialog.exec() == QDialog::Accepted) {
		ModifyTemplateDimension *command = new ModifyTemplateDimension(tbtemplate_);
		command -> setType(true);
		command -> setIndex(index);
		command -> setDimensionBefore(dimension_before);
		command -> setDimensionAfter(dialog.value());
		requestGridModification(command);
	}
}

/**
	Remove the column at the last index selected when calling the context menu.
*/
void TitleBlockTemplateView::deleteColumn() {
	int index = lastContextMenuCellIndex();
	if (index == -1) return;
	requestGridModification(ModifyTemplateGridCommand::deleteColumn(tbtemplate_, index));
}

/**
	Remove the row at the last index selected when calling the context menu.
*/
void TitleBlockTemplateView::deleteRow() {
	int index = lastContextMenuCellIndex();
	if (index == -1) return;
	requestGridModification(ModifyTemplateGridCommand::deleteRow(tbtemplate_, index));
}

/**
	Merge the selected cells.
*/
void TitleBlockTemplateView::mergeSelectedCells() {
	// retrieve the selected cells
	TitleBlockTemplateCellsSet selected_cells = selectedCellsSet();
	
	// merging applies only to cells composing a rectangle
	if (!selected_cells.isRectangle()) {
		qDebug() << "selected cells are not composing a rectangle";
		return;
	}
	
	// the merge area may also be too small
	if (selected_cells.count() < 2) {
		qDebug() << "the merge area does not even contain 2 selected and mergeable cells";
		return;
	}
	
	qDebug() << Q_FUNC_INFO << "ok, ready for cells merge";
	MergeCellsCommand *merge_command = new MergeCellsCommand(selected_cells, tbtemplate_);
	if (merge_command -> isValid()) requestGridModification(merge_command);
}

/**
	Split the selected cell.
*/
void TitleBlockTemplateView::splitSelectedCell() {
	// retrieve the selected cells
	TitleBlockTemplateCellsSet selected_cells = selectedCellsSet();
	
	// we expect only one visual cell to be selected
	if (selected_cells.count() != 1) {
		qDebug() << "please select a single cell";
		return;
	}
	
	SplitCellsCommand *split_command = new SplitCellsCommand(selected_cells, tbtemplate_);
	if (split_command -> isValid()) requestGridModification(split_command);
}

/**
	Reimplement the way the background is drawn to render the title block
	template.
*/
void TitleBlockTemplateView::drawBackground(QPainter *painter, const QRectF &rect) {
	QGraphicsView::drawBackground(painter, rect);
	if (!tbtemplate_) return; // TODO shouldn't we draw a large uniform rect?
}

/**
	@return the selected logical cells, not including the spanned ones.
*/
QList<TitleBlockCell *> TitleBlockTemplateView::selectedCells() const {
	return(selectedCellsSet().cells(false).toList());
}

/**
	@return the selected visual cells.
*/
TitleBlockTemplateCellsSet TitleBlockTemplateView::selectedCellsSet() const {
	return(makeCellsSetFromGraphicsItems(scene() -> selectedItems()));
}

/**
	@return the visual cells contained in the \a rect
	@param rect Rectangle in the coordinates of the QGraphicsWidget
	representing the title block template.
*/
TitleBlockTemplateCellsSet TitleBlockTemplateView::cells(const QRectF &rect) const {
	QPolygonF mapped_rect(form_ -> mapToScene(rect));
	QList<QGraphicsItem *> items = scene() -> items(mapped_rect, Qt::IntersectsItemShape);
	return(makeCellsSetFromGraphicsItems(items));
}

/**
	@return the current size of the rendered title block template
*/
QSizeF TitleBlockTemplateView::templateSize() const {
	return(QSizeF(templateWidth(), templateHeight()));
}

/**
	@return the current width of the rendered title block template
*/
qreal TitleBlockTemplateView::templateWidth() const {
	if (!tbtemplate_) return(0);
	
	qreal width = DEFAULT_ROWS_HELPER_CELLS_WIDTH;
	// the rendered width may exceed the initially planned preview width
	width += qMax<int>(preview_width_, tbtemplate_ -> width(preview_width_));
	
	return(width);
}

/**
	@return the current height of the rendered title block template
*/
qreal TitleBlockTemplateView::templateHeight() const {
	if (!tbtemplate_) return(0);
	
	qreal height = DEFAULT_PREVIEW_HELPER_CELL_HEIGHT;
	height += DEFAULT_COLS_HELPER_CELLS_HEIGHT;
	height += tbtemplate_ -> height();
	
	return(height);
}

/**
	Handles mouse wheel-related actions
	@param e QWheelEvent describing the wheel event
*/
void TitleBlockTemplateView::wheelEvent(QWheelEvent *e) {
	// si la touche Ctrl est enfoncee, on zoome / dezoome
	if (e -> modifiers() & Qt::ControlModifier) {
		if (e -> delta() > 0) { 
			zoomIn();
		} else {
			zoomOut();
		}
	} else {
		QAbstractScrollArea::wheelEvent(e);
	}
}

/**
	@return the zoom factor used by zoomIn() and zoomOut().
*/
qreal TitleBlockTemplateView::zoomFactor() const {
	return(1.1);
}

/**
	Initialize this view (actions, signals/slots connections, etc.)
*/
void TitleBlockTemplateView::init() {
	add_column_before_    = new QAction(tr("Ajouter une colonne (avant)",              "context menu"), this);
	add_row_before_       = new QAction(tr("Ajouter une ligne (avant)",                "context menu"), this);
	add_column_after_     = new QAction(tr("Ajouter une colonne (apr\350s)",           "context menu"), this);
	add_row_after_        = new QAction(tr("Ajouter une ligne (apr\350s)",             "context menu"), this);
	edit_column_dim_      = new QAction(tr("Modifier les dimensions de cette colonne", "context menu"), this);
	edit_row_dim_         = new QAction(tr("Modifier les dimensions de cette ligne",   "context menu"), this);
	delete_column_        = new QAction(tr("Supprimer cette colonne",                  "context menu"), this);
	delete_row_           = new QAction(tr("Supprimer cette ligne",                    "context menu"), this);
	change_preview_width_ = new QAction(tr("Modifier la largeur de cet aper\347u",     "context menu"), this);
	
	connect(add_column_before_,    SIGNAL(triggered()), this, SLOT(addColumnBefore()));
	connect(add_row_before_,       SIGNAL(triggered()), this, SLOT(addRowBefore()));
	connect(add_column_after_,     SIGNAL(triggered()), this, SLOT(addColumnAfter()));
	connect(add_row_after_,        SIGNAL(triggered()), this, SLOT(addRowAfter()));
	connect(edit_column_dim_,      SIGNAL(triggered()), this, SLOT(editColumn()));
	connect(edit_row_dim_,         SIGNAL(triggered()), this, SLOT(editRow()));
	connect(delete_column_,        SIGNAL(triggered()), this, SLOT(deleteColumn()));
	connect(delete_row_,           SIGNAL(triggered()), this, SLOT(deleteRow()));
	connect(change_preview_width_, SIGNAL(triggered()), this, SLOT(changePreviewWidth()));
	
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setBackgroundBrush(QBrush(QColor(248, 255, 160)));
	
	connect(scene(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

/**
	Apply the columns widths currently specified by the edited title block
	template.
	@param animate true to animate the change, false otherwise.
*/
void TitleBlockTemplateView::applyColumnsWidths(bool animate) {
	// the first column is dedicated to helper cells showing the rows height
	tbgrid_ -> setColumnFixedWidth(0, DEFAULT_ROWS_HELPER_CELLS_WIDTH);
	tbgrid_ -> setColumnSpacing(0, 0);
	
	// we apply the other columns width based on the title block template data
	QList<int> widths = tbtemplate_ -> columnsWidth(preview_width_);
	int total_applied_width = 0;
	for (int i = 0 ; i < widths.count() ; ++ i) {
		int applied_width = qMax(0, widths.at(i));
		tbgrid_ -> setColumnSpacing(COL_OFFSET + i, 0);
		if (!animate) {
			// no animation on first call
			tbgrid_ -> setColumnFixedWidth(COL_OFFSET + i, widths.at(i));
		} else {
			GridLayoutAnimation *animation = new GridLayoutAnimation(tbgrid_, form_);
			animation -> setIndex(COL_OFFSET + i);
			animation -> setActsOnRows(false);
			animation -> setStartValue(QVariant(tbgrid_ -> columnMinimumWidth(COL_OFFSET + i)));
			animation -> setEndValue(QVariant(1.0 * applied_width));
			animation -> setDuration(500);
			connect(animation, SIGNAL(finished()), this, SLOT(updateColumnsHelperCells()));
			animation -> start(QAbstractAnimation::DeleteWhenStopped);
		}
		total_applied_width += applied_width;
	}
	if (!animate) updateColumnsHelperCells();
	++ apply_columns_widths_count_;
	
	// we systematically parameter some cells
	total_width_helper_cell_ -> split_size = 0;
	tbgrid_ -> addItem(total_width_helper_cell_, 0, COL_OFFSET, 1, widths.count());
	removeItem(extra_cells_width_helper_cell_);
	
	if (total_applied_width < preview_width_) {
		// preview_width is greater than the sum of cells widths
		// we add an extra column with a helper cell
		tbgrid_ -> addItem(extra_cells_width_helper_cell_, ROW_OFFSET - 1, COL_OFFSET + widths.count(), tbtemplate_ -> rowsCount() + 1, 1);
		tbgrid_ -> addItem(total_width_helper_cell_, 0, COL_OFFSET, 1, widths.count() + 1);
		tbgrid_ -> setColumnFixedWidth(COL_OFFSET + widths.count(), preview_width_ - total_applied_width);
		extra_cells_width_helper_cell_ -> label = QString(
			tr("[%1px]","content of the extra cell added when the total width of cells is less than the preview width")
		).arg(preview_width_ - total_applied_width);
	} else if (total_applied_width > preview_width_) {
		// preview width is smaller than the sum of cells widths
		// we draw an extra header within th "preview width" cell.
		tbgrid_ -> addItem(total_width_helper_cell_, 0, COL_OFFSET, 1, widths.count());
		total_width_helper_cell_ -> split_background_color = QColor(Qt::red);
		total_width_helper_cell_ -> split_foreground_color = QColor(Qt::black);
		total_width_helper_cell_ -> split_label = QString(
			tr("[%1px]", "content of the extra helper cell added when the total width of cells is greather than the preview width")
		).arg(total_applied_width - preview_width_);
		total_width_helper_cell_ -> split_size = total_applied_width - preview_width_;
	}
}

/**
	Apply the rows heights currently specified by the edited title block
	template.
	@param animate true to animate the change, false otherwise.
*/
void TitleBlockTemplateView::applyRowsHeights(bool animate) {
	// the first row is dedicated to a helper cell showing the total width
	tbgrid_ -> setRowFixedHeight(0, DEFAULT_PREVIEW_HELPER_CELL_HEIGHT);
	tbgrid_ -> setRowSpacing(0, 0);
	// the second row is dedicated to helper cells showing the columns width
	tbgrid_ -> setRowFixedHeight(1, DEFAULT_COLS_HELPER_CELLS_HEIGHT);
	tbgrid_ -> setRowSpacing(1, 0);
	
	QList<int> heights = tbtemplate_ -> rowsHeights();
	for (int i = 0 ; i < heights.count() ; ++ i) {
		tbgrid_ -> setRowSpacing(ROW_OFFSET + i, 0);
		if (!animate) {
			// no animation on first call
			tbgrid_ -> setRowFixedHeight(ROW_OFFSET + i, heights.at(i));
		} else {
			GridLayoutAnimation *animation = new GridLayoutAnimation(tbgrid_, form_);
			animation -> setIndex(ROW_OFFSET + i);
			animation -> setActsOnRows(true);
			animation -> setStartValue(QVariant(tbgrid_ -> rowMinimumHeight(ROW_OFFSET + i)));
			animation -> setEndValue(QVariant(1.0 * heights.at(i)));
			animation -> setDuration(500);
			connect(animation, SIGNAL(finished()), this, SLOT(updateRowsHelperCells()));
			animation -> start(QAbstractAnimation::DeleteWhenStopped);
		}
		
	}
	if (!animate) updateRowsHelperCells();
	++ apply_rows_heights_count_;
}

/**
	Update the content (type and value) of rows helper cells.
*/
void TitleBlockTemplateView::updateRowsHelperCells() {
	int row_count = tbtemplate_ -> rowsCount();
	QList<int> heights = tbtemplate_ -> rowsHeights();
	for (int i = 0 ; i < row_count ; ++ i) {
		HelperCell *current_row_cell = static_cast<HelperCell *>(tbgrid_ -> itemAt(ROW_OFFSET + i, 0));
		current_row_cell -> setType(QET::Absolute); // rows always have absolute heights
		current_row_cell -> label = QString(tr("%1px", "format displayed in rows helper cells")).arg(heights.at(i));
	}
}

/**
	Update the content (type and value) of columns helper cells.
*/
void TitleBlockTemplateView::updateColumnsHelperCells() {
	int col_count = tbtemplate_ -> columnsCount();
	for (int i = 0 ; i < col_count ; ++ i) {
		TitleBlockDimension current_col_dim = tbtemplate_ -> columnDimension(i);
		HelperCell *current_col_cell = static_cast<HelperCell *>(tbgrid_ -> itemAt(1, COL_OFFSET + i));
		current_col_cell -> setType(current_col_dim.type);
		current_col_cell -> label = current_col_dim.toString();
	}
}

/**
	Add the cells (both helper cells and regular visual cells) to the scene to
	get a visual representation of the edited title block template.
*/
void TitleBlockTemplateView::addCells() {
	int col_count = tbtemplate_ -> columnsCount();
	int row_count = tbtemplate_ -> rowsCount();
	if (row_count < 1 || col_count < 1) return;
	
	// we add a big cell to show the total width
	total_width_helper_cell_ = new SplittedHelperCell();
	total_width_helper_cell_ -> setType(QET::Absolute);
	updateTotalWidthLabel();
	total_width_helper_cell_ -> orientation = Qt::Horizontal;
	total_width_helper_cell_ -> setActions(QList<QAction *>() << change_preview_width_);
	connect(total_width_helper_cell_, SIGNAL(contextMenuTriggered(HelperCell *)), this, SLOT(updateLastContextMenuCell(HelperCell *)));
	connect(total_width_helper_cell_, SIGNAL(doubleClicked(HelperCell*)),         this, SLOT(changePreviewWidth()));
	tbgrid_ -> addItem(total_width_helper_cell_, 0, COL_OFFSET, 1, col_count);
	
	// we also initialize an extra helper cells that shows the preview width is
	// too long for the current cells widths
	extra_cells_width_helper_cell_ = new HelperCell();
	extra_cells_width_helper_cell_ -> background_color = QColor(Qt::red);
	
	// we add one cell per column to show their respective width
	for (int i = 0 ; i < col_count ; ++ i) {
		TitleBlockDimension current_col_dim = tbtemplate_ -> columnDimension(i);
		HelperCell *current_col_cell = new HelperCell();
		current_col_cell -> setType(current_col_dim.type);
		current_col_cell -> label = current_col_dim.toString();
		current_col_cell -> setActions(columnsActions());
		current_col_cell -> orientation = Qt::Horizontal;
		current_col_cell -> index = i;
		connect(current_col_cell, SIGNAL(contextMenuTriggered(HelperCell *)), this, SLOT(updateLastContextMenuCell(HelperCell *)));
		connect(current_col_cell, SIGNAL(doubleClicked(HelperCell*)),         this, SLOT(editColumn(HelperCell *)));
		tbgrid_ -> addItem(current_col_cell, 1, COL_OFFSET + i, 1, 1);
	}
	
	// we add one cell per row to show their respective height
	QList<int> heights = tbtemplate_ -> rowsHeights();
	for (int i = 0 ; i < row_count ; ++ i) {
		HelperCell *current_row_cell = new HelperCell();
		current_row_cell -> setType(QET::Absolute); // rows always have absolute heights
		current_row_cell -> label = QString(tr("%1px")).arg(heights.at(i));
		current_row_cell -> orientation = Qt::Vertical;
		current_row_cell -> index = i;
		current_row_cell -> setActions(rowsActions());
		connect(current_row_cell, SIGNAL(contextMenuTriggered(HelperCell *)), this, SLOT(updateLastContextMenuCell(HelperCell *)));
		connect(current_row_cell, SIGNAL(doubleClicked(HelperCell*)),         this, SLOT(editRow(HelperCell *)));
		tbgrid_ -> addItem(current_row_cell, ROW_OFFSET + i, 0, 1, 1);
	}
	
	// eventually we add the cells composing the titleblock template
	for (int i = 0 ; i < col_count ; ++ i) {
		for (int j = 0 ; j < row_count ; ++ j) {
			TitleBlockCell *cell = tbtemplate_ -> cell(j, i);
			if (cell -> spanner_cell) continue;
			TitleBlockTemplateVisualCell *cell_item = new TitleBlockTemplateVisualCell();
			cell_item -> setTemplateCell(tbtemplate_, cell);
			tbgrid_ -> addItem(cell_item, ROW_OFFSET + j, COL_OFFSET + i, cell -> row_span + 1, cell -> col_span + 1);
		}
	}
}

/**
	Refresh the regular cells.
*/
void TitleBlockTemplateView::refresh() {
	int col_count = tbtemplate_ -> columnsCount();
	int row_count = tbtemplate_ -> rowsCount();
	if (row_count < 1 || col_count < 1) return;
	
	for (int i = 0 ; i < col_count ; ++ i) {
		for (int j = 0 ; j < row_count ; ++ j) {
			if (QGraphicsLayoutItem *item = tbgrid_ -> itemAt(ROW_OFFSET + j, COL_OFFSET + i)) {
				if (QGraphicsItem *qgi = dynamic_cast<QGraphicsItem *>(item)) {
					qgi -> update();
				}
			}
		}
	}
}

/**
	Ask the user a new width for the preview
*/
void TitleBlockTemplateView::changePreviewWidth() {
	TitleBlockDimensionWidget dialog(false, this);
	dialog.setWindowTitle(tr("Changer la largeur de l'aper\347u"));
	dialog.label() -> setText(tr("Largeur de l'aper\347u :"));
	dialog.setValue(TitleBlockDimension(preview_width_));
	if (dialog.exec() == QDialog::Accepted) {
		setPreviewWidth(dialog.value().value);
	}
}

/**
	Fill the layout with empty cells where needed.
*/
void TitleBlockTemplateView::fillWithEmptyCells() {
	int col_count = tbtemplate_ -> columnsCount();
	int row_count = tbtemplate_ -> rowsCount();
	if (row_count < 1 || col_count < 1) return;
	
	for (int i = 0 ; i < col_count ; ++ i) {
		for (int j = 0 ; j < row_count ; ++ j) {
			if (tbgrid_ -> itemAt(ROW_OFFSET + j, COL_OFFSET + i)) continue;
			qDebug() << Q_FUNC_INFO << "looks like there is nothing there (" << j << "," << i << ")";
			TitleBlockTemplateVisualCell *cell_item = new TitleBlockTemplateVisualCell();
			if (TitleBlockCell *target_cell = tbtemplate_ -> cell(j, i)) {
				qDebug() << Q_FUNC_INFO << "target_cell" << target_cell;
				cell_item -> setTemplateCell(tbtemplate_, target_cell);
			}
			tbgrid_ -> addItem(cell_item, ROW_OFFSET + j, COL_OFFSET + i);
		}
	}
}

/**
	@param event Object describing the received event 
*/
bool TitleBlockTemplateView::event(QEvent *event) {
	if (first_activation_ && event -> type() == QEvent::WindowActivate) {
		QTimer::singleShot(250, this, SLOT(zoomFit()));
		first_activation_ = false;
	}
	return(QGraphicsView::event(event));
}

/**
	Load the \a tbt title block template.
	If a different template was previously loaded, it is deleted.
	
*/
void TitleBlockTemplateView::loadTemplate(TitleBlockTemplate *tbt) {
	if (tbgrid_) {
		scene() -> removeItem(form_);
		// also deletes TemplateCellPreview because, according to the
		// documentation, QGraphicsGridLayout takes ownership of the items.
		form_ -> deleteLater();
	}
	if (tbtemplate_ && tbtemplate_ != tbt) {
		delete tbtemplate_;
	}
	
	tbtemplate_ = tbt;
	
	// initialize a grid layout with no margin
	tbgrid_ = new QGraphicsGridLayout();
	tbgrid_ -> setContentsMargins(0, 0, 0, 0);
	// add cells defined by the title block template in this layout
	addCells();
	// fill potential holes in the grid with empty cells
	fillWithEmptyCells();
	// apply rows and columns dimensions
	applyColumnsWidths(false);
	applyRowsHeights(false);
	
	// assign the layout to a basic QGraphicsWidget
	form_ = new QGraphicsWidget();
	form_ -> setLayout(tbgrid_);
	scene() -> addItem(form_);
	adjustSceneRect();
}

/**
	@return the list of rows-specific actions.
*/
QList<QAction *> TitleBlockTemplateView::rowsActions() const {
	return QList<QAction *>() << add_row_before_<< edit_row_dim_ << add_row_after_ << delete_row_;
}

/**
	@return the list of columns-specific actions.
*/
QList<QAction *> TitleBlockTemplateView::columnsActions() const {
	return QList<QAction *>() << add_column_before_ << edit_column_dim_ << add_column_after_ << delete_column_;
}

/**
	Update the displayed layout. Call this function to refresh the display
	after the rendered title block template has been "deeply" modified, e.g.
	rows/columns have been added/modified or cells were merged/splitted.
*/
void TitleBlockTemplateView::updateLayout() {
	// TODO we should try to update the grid instead of deleting-and-reloading it
	loadTemplate(tbtemplate_);
}

/**
	Update the displayed layout. Call this function when the dimensions of
	rows changed.
*/
void TitleBlockTemplateView::rowsDimensionsChanged() {
	applyRowsHeights();
}

/**
	Update the displayed layout. Call this function when the dimensions of
	columns changed.
*/
void TitleBlockTemplateView::columnsDimensionsChanged() {
	applyColumnsWidths();
}

/**
	Set the new preview width to width
	@param width new preview width
*/
void TitleBlockTemplateView::setPreviewWidth(int width) {
	if (preview_width_ == width) return;
	preview_width_ = width;
	applyColumnsWidths();
	updateTotalWidthLabel();
	//adjustSceneRect();
	centerOn(form_);
	/// TODO center again the preview()
}

/**
	Update the label of the helper cell that indicates the preview width.
*/
void TitleBlockTemplateView::updateTotalWidthLabel() {
	if (!total_width_helper_cell_) return;
	total_width_helper_cell_ -> label = QString(
		tr(
			"Largeur totale pour cet aper\347u : %1px",
			"displayed at the top of the preview when editing a title block template"
		)
	).arg(preview_width_);
}

/**
	Emit the gridModificationRequested() signal with \a command after having set
	its view component.
	@see TitleBlockTemplateCommand::setView()
	@param command A command object modifying the rendered title block template.
*/
void TitleBlockTemplateView::requestGridModification(TitleBlockTemplateCommand *command) {
	if (!command) return;
	command -> setView(this);
	emit(gridModificationRequested(command));
}

/**
	@return the last index selected when triggering the context menu.
	@see updateLastContextMenuCell
*/
int TitleBlockTemplateView::lastContextMenuCellIndex() const {
	if (last_context_menu_cell_) {
		return(last_context_menu_cell_ -> index);
	}
	return(-1);
}

/**
	@param item an item supposed to be contained in the grid layout.
	@return the flat index if this item, or -1 if it could not be found.
*/
int TitleBlockTemplateView::indexOf(QGraphicsLayoutItem *item) {
	for (int i = 0 ; i < tbgrid_ -> count() ; ++i) {
		if (item == tbgrid_ -> itemAt(i)) return(i);
	}
	return(-1);
}

/**
	Removes an item from the grid layout
	@param item an item supposed to be contained in the grid layout.
*/
void TitleBlockTemplateView::removeItem(QGraphicsLayoutItem *item) {
	int index = indexOf(item);
	if (index != -1) {
		tbgrid_ -> removeAt(index);
		// trick: we also have to remove the item from the scene
		if (QGraphicsScene *current_scene = scene()) {
			if (QGraphicsItem *qgi = item -> graphicsItem()) {
				current_scene -> removeItem(qgi);
			}
		}
	}
}

/**
	@param a list of QGraphicsItem
	@return the corresponding TitleBlockTemplateCellsSet
*/
TitleBlockTemplateCellsSet TitleBlockTemplateView::makeCellsSetFromGraphicsItems(const QList<QGraphicsItem *> &items) const {
	TitleBlockTemplateCellsSet set(this);
	foreach (QGraphicsItem *item, items) {
		if (TitleBlockTemplateVisualCell *cell_view = dynamic_cast<TitleBlockTemplateVisualCell *>(item)) {
			if (cell_view -> cell() && cell_view -> cell() -> num_row != -1) {
				set << cell_view;
			}
		}
	}
	return(set);
}

/**
	Stores \a last_context_menu_cell as being the last helper cell the context
	menu was triggered on.
*/
void TitleBlockTemplateView::updateLastContextMenuCell(HelperCell *last_context_menu_cell) {
	last_context_menu_cell_ = last_context_menu_cell;
}

/**
	Adjusts the bounding rect of the scene.
*/
void TitleBlockTemplateView::adjustSceneRect() {
	QRectF old_scene_rect = scene() -> sceneRect();
	
	// rectangle including everything on the scene
	QRectF bounding_rect(QPointF(0, 0), templateSize());
	scene() -> setSceneRect(bounding_rect);
	
	// met a jour la scene
	scene() -> update(old_scene_rect.united(bounding_rect));
}

