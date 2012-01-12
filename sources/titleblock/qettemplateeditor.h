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
#ifndef TITLEBLOCK_SLASH_QET_TEMPLATE_EDITOR_H
#define TITLEBLOCK_SLASH_QET_TEMPLATE_EDITOR_H
#include <QtGui>
#include "qet.h"
#include "templateview.h"
#include "templatelocation.h"
class ModifyTitleBlockCellCommand;
class TitleBlockTemplateCommand;
class TitleBlockTemplateCellWidget;
class TitleBlockTemplateLogoManager;
class QETProject;

/**
	This class implements the main window of QElectroTech's titleblock template
	editor. This editor aims at allowing users to easily create their own title
	block templates.
*/
class QETTitleBlockTemplateEditor : public QMainWindow {
	Q_OBJECT
	
	// constructor, destructor
	public:
	QETTitleBlockTemplateEditor(QWidget * = 0);
	virtual ~QETTitleBlockTemplateEditor();
	private:
	QETTitleBlockTemplateEditor(const QETTitleBlockTemplateEditor &);
	
	// attributes
	private:
	/// is the template read-only?
	bool read_only;
	/// menus TODO
	QMenu *file_menu_, *edit_menu_,/* *paste_from_menu_, */*display_menu_,/* *tools_menu_,*/ *config_menu_, *help_menu_;
	/// actions
	QAction *new_, *open_, *open_from_file_, *save_, *save_as_, *save_as_file_, *quit_, *configure_, *about_qt_, *about_qet_, *merge_cells_, *split_cell_;
	QAction *zoom_in_, *zoom_out_, *zoom_fit_, *zoom_reset_;
	/// Location of the currently edited template
	TitleBlockTemplateLocation location_;
	/// Filepath of the currently edited template, if opened from a file
	QString filepath_;
	/// Whether to consider the location or the filepath
	bool opened_from_file_;
	/// Template Object edited
	TitleBlockTemplate *tb_template_;
	/// Template preview
	QGraphicsScene *template_edition_area_scene_;
	TitleBlockTemplateView *template_edition_area_view_;
	/// Individual cell widget edition
	QDockWidget *template_cell_editor_dock_widget_;
	TitleBlockTemplateCellWidget *template_cell_editor_widget_;
	/// Logo manager widget
	TitleBlockTemplateLogoManager *logo_manager_;
	/// Undo interface
	QUndoStack *undo_stack_;
	QUndoView *undo_view_;
	QDockWidget *undo_dock_widget_;
	
	// methods
	public:
	TitleBlockTemplateLocation location() const;
	
	protected:
	bool canClose();
	void closeEvent(QCloseEvent *);
	
	private:
	void initActions();
	void initMenus();
	void initWidgets();
	void initLogoManager();
	
	public slots:
	void selectedCellsChanged(QList<TitleBlockCell *>);
	bool edit(const TitleBlockTemplateLocation &);
	bool edit(QETProject *, const QString &);
	bool edit(const QString &);
	bool editCopyOf(const TitleBlockTemplate *);
	bool edit(TitleBlockTemplate *);
	void editLogos();
	void newTemplate();
	void open();
	void openFromFile();
	bool save();
	bool saveAs();
	bool saveAsFile();
	void quit();
	
	private slots:
	TitleBlockTemplateLocation getTitleBlockTemplateLocationFromUser(const QString & = QString(), bool existing_only = true);
	void pushCellUndoCommand(ModifyTitleBlockCellCommand *);
	void pushGridUndoCommand(TitleBlockTemplateCommand *);
	void pushUndoCommand(QUndoCommand *);
	void updateEditorTitle();
	bool saveAs(const TitleBlockTemplateLocation &);
	bool saveAs(const QString &);
};

#endif
