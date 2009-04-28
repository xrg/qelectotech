/*
	Copyright 2006-2009 Xavier Guerrin
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
#include "qetelementeditor.h"
#include "qetapp.h"
#include "elementscene.h"
#include "elementview.h"
#include "customelementpart.h"
#include "newelementwizard.h"
#include "elementitemeditor.h"
#include "elementdefinition.h"
#include "elementdialog.h"
#include "recentfiles.h"

/**
	Constructeur
	@param parent QWidget parent
*/
QETElementEditor::QETElementEditor(QWidget *parent) :
	QMainWindow(parent),
	read_only(false),
	min_title(tr("QElectroTech - \311diteur d'\351l\351ment", "window title")),
	opened_from_file(false)
{
	setWindowTitle(min_title);
	setWindowIcon(QIcon(":/ico/qet.png"));
	
	setupInterface();
	setupActions();
	setupMenus();
	
	// la fenetre est maximisee par defaut
	setMinimumSize(QSize(500, 350));
	setWindowState(Qt::WindowMaximized);
	
	// lecture des parametres
	readSettings();
	slot_updateMenus();
	
	// affichage
	show();
}

/// Destructeur
QETElementEditor::~QETElementEditor() {
	/*
		retire le widget d'edition de partie affiche par le dock
		cela evite qu'il ne soit supprime avant que la partie a laquelle il est
		rattache ne le supprime une fois de trop
	*/
	clearToolsDock();
}

/**
	Met en place les actions
*/
void QETElementEditor::setupActions() {
	new_element   = new QAction(QIcon(":/ico/document-new.png"),          tr("&Nouveau"),                    this);
	open          = new QAction(QIcon(":/ico/document-open.png"),         tr("&Ouvrir"),                     this);
	open_file     = new QAction(QIcon(":/ico/document-open.png"),         tr("&Ouvrir depuis un fichier"),   this);
	save          = new QAction(QIcon(":/ico/document-save.png"),         tr("&Enregistrer"),                this);
	save_as       = new QAction(QIcon(":/ico/document-save-as.png"),      tr("Enregistrer sous"),            this);
	save_as_file  = new QAction(QIcon(":/ico/document-save-as.png"),      tr("Enregistrer dans un fichier"), this);
	reload        = new QAction(QIcon(":/ico/view-refresh.png"),          tr("Recharger"),                   this);
	quit          = new QAction(QIcon(":/ico/application-exit.png"),      tr("&Quitter"),                    this);
	selectall     = new QAction(QIcon(":/ico/edit-select-all.png"),       tr("Tout s\351lectionner"),        this);
	deselectall   = new QAction(                                          tr("D\351s\351lectionner tout"),   this);
	cut           = new QAction(QIcon(":/ico/edit-cut.png"),              tr("Co&uper"),                     this);
	copy          = new QAction(QIcon(":/ico/edit-copy.png"),             tr("Cop&ier"),                     this);
	paste         = new QAction(QIcon(":/ico/edit-paste.png"),            tr("C&oller"),                     this);
	paste_in_area = new QAction(QIcon(":/ico/edit-paste.png"),            tr("C&oller dans la zone..."),     this);
	inv_select    = new QAction(                                          tr("Inverser la s\351lection"),    this);
	edit_delete   = new QAction(QIcon(":/ico/edit-delete.png"),           tr("&Supprimer"),                  this);
	zoom_in       = new QAction(QIcon(":/ico/zoom-in.png"),               tr("Zoom avant"),                  this);
	zoom_out      = new QAction(QIcon(":/ico/zoom-out.png"),              tr("Zoom arri\350re"),             this);
	zoom_fit      = new QAction(QIcon(":/ico/zoom-fit-best.png"),         tr("Zoom adapt\351"),              this);
	zoom_reset    = new QAction(QIcon(":/ico/zoom-original.png"),         tr("Pas de zoom"),                 this);
	edit_size_hs  = new QAction(QIcon(":/ico/hotspot.png"),               tr("\311diter la taille et le point de saisie"), this);
	edit_names    = new QAction(QIcon(":/ico/names.png"),                 tr("\311diter les noms"),          this);
	edit_ori      = new QAction(QIcon(":/ico/orientations.png"),          tr("\311diter les orientations"),  this);
	edit_raise    = new QAction(QIcon(":/ico/raise.png"),                 tr("Rapprocher"),                  this);
	edit_lower    = new QAction(QIcon(":/ico/lower.png"),                 tr("\311loigner"),                 this);
	edit_backward = new QAction(QIcon(":/ico/send_backward.png"),         tr("Envoyer au fond"),             this);
	edit_forward  = new QAction(QIcon(":/ico/bring_forward.png"),         tr("Amener au premier plan"),      this);
	move          = new QAction(QIcon(":/ico/select.png"),                tr("D\351placer un objet"),        this);
	add_line      = new QAction(QIcon(":/ico/line.png"),                  tr("Ajouter une ligne"),           this);
	add_rectangle = new QAction(QIcon(":/ico/rectangle.png"),             tr("Ajouter un rectangle"),        this);
	add_ellipse   = new QAction(QIcon(":/ico/ellipse.png"),               tr("Ajouter une ellipse"),         this);
	add_circle    = new QAction(QIcon(":/ico/circle.png"),                tr("Ajouter un cercle"),           this);
	add_polygon   = new QAction(QIcon(":/ico/polygon.png"),               tr("Ajouter un polygone"),         this);
	add_text      = new QAction(QIcon(":/ico/text.png"),                  tr("Ajouter du texte"),            this);
	add_arc       = new QAction(QIcon(":/ico/arc.png"),                   tr("Ajouter un arc de cercle"),    this);
	add_terminal  = new QAction(QIcon(":/ico/terminal.png"),              tr("Ajouter une borne"),           this);
	add_textfield = new QAction(QIcon(":/ico/textfield.png"),             tr("Ajouter un champ de texte"),   this);
	
	QString add_status_tip = tr("Maintenez la touche Shift enfonc\351e pour effectuer plusieurs ajouts d'affil\351e");
	add_line      -> setStatusTip(add_status_tip);
	add_rectangle -> setStatusTip(add_status_tip);
	add_ellipse   -> setStatusTip(add_status_tip);
	add_circle    -> setStatusTip(add_status_tip);
	add_text      -> setStatusTip(add_status_tip);
	add_arc       -> setStatusTip(add_status_tip);
	add_terminal  -> setStatusTip(add_status_tip);
	add_textfield -> setStatusTip(add_status_tip);
	add_polygon -> setStatusTip(tr("Utilisez le bouton droit de la souris pour poser le dernier point du polygone"));
	
	undo = ce_scene -> undoStack().createUndoAction(this, tr("Annuler"));
	redo = ce_scene -> undoStack().createRedoAction(this, tr("Refaire"));
	undo -> setIcon(QIcon(":/ico/edit-undo.png"));
	redo -> setIcon(QIcon(":/ico/edit-redo.png"));
	undo -> setShortcuts(QKeySequence::Undo);
	redo -> setShortcuts(QKeySequence::Redo);
	
	new_element       -> setShortcut(QKeySequence::New);
	open              -> setShortcut(QKeySequence::Open);
	open_file         -> setShortcut(tr("Ctrl+Shift+O"));
	save              -> setShortcut(QKeySequence::Save);
	save_as_file      -> setShortcut(tr("Ctrl+Shift+S"));
	reload            -> setShortcut(Qt::Key_F5);
	quit              -> setShortcut(QKeySequence(tr("Ctrl+Q")));
	selectall         -> setShortcut(QKeySequence::SelectAll);
	deselectall       -> setShortcut(QKeySequence(tr("Ctrl+Shift+A")));
	inv_select        -> setShortcut(QKeySequence(tr("Ctrl+I")));
	cut               -> setShortcut(QKeySequence::Cut);
	copy              -> setShortcut(QKeySequence::Copy);
	paste             -> setShortcut(QKeySequence::Paste);
	paste_in_area     -> setShortcut(tr("Ctrl+Shift+V"));
	edit_delete       -> setShortcut(QKeySequence(tr("Suppr")));
	
	zoom_in           -> setShortcut(QKeySequence::ZoomIn);
	zoom_out          -> setShortcut(QKeySequence::ZoomOut);
	zoom_fit          -> setShortcut(QKeySequence(tr("Ctrl+9")));
	zoom_reset        -> setShortcut(QKeySequence(tr("Ctrl+0")));
	
	edit_names        -> setShortcut(QKeySequence(tr("Ctrl+E")));
	edit_size_hs      -> setShortcut(QKeySequence(tr("Ctrl+R")));
	edit_ori          -> setShortcut(QKeySequence(tr("Ctrl+T")));
	
	edit_raise        -> setShortcut(QKeySequence(tr("Ctrl+Shift+Up")));
	edit_lower        -> setShortcut(QKeySequence(tr("Ctrl+Shift+Down")));
	edit_backward     -> setShortcut(QKeySequence(tr("Ctrl+Shift+End")));
	edit_forward      -> setShortcut(QKeySequence(tr("Ctrl+Shift+Home")));
	
	connect(new_element,   SIGNAL(triggered()), this,     SLOT(slot_new()));
	connect(open,          SIGNAL(triggered()), this,     SLOT(slot_open()));
	connect(open_file,     SIGNAL(triggered()), this,     SLOT(slot_openFile()));
	connect(save,          SIGNAL(triggered()), this,     SLOT(slot_save()));
	connect(save_as,       SIGNAL(triggered()), this,     SLOT(slot_saveAs()));
	connect(save_as_file,  SIGNAL(triggered()), this,     SLOT(slot_saveAsFile()));
	connect(reload,        SIGNAL(triggered()), this,     SLOT(slot_reload()));
	connect(quit,          SIGNAL(triggered()), this,     SLOT(close()));
	connect(selectall,     SIGNAL(triggered()), ce_scene, SLOT(slot_selectAll()));
	connect(deselectall,   SIGNAL(triggered()), ce_scene, SLOT(slot_deselectAll()));
	connect(inv_select,    SIGNAL(triggered()), ce_scene, SLOT(slot_invertSelection()));
	connect(cut,           SIGNAL(triggered()), ce_view,  SLOT(cut()));
	connect(copy,          SIGNAL(triggered()), ce_view,  SLOT(copy()));
	connect(paste,         SIGNAL(triggered()), ce_view,  SLOT(paste()));
	connect(paste_in_area, SIGNAL(triggered()), ce_view,  SLOT(pasteInArea()));
	connect(zoom_in,       SIGNAL(triggered()), ce_view,  SLOT(zoomIn()));
	connect(zoom_out,      SIGNAL(triggered()), ce_view,  SLOT(zoomOut()));
	connect(zoom_fit,      SIGNAL(triggered()), ce_view,  SLOT(zoomFit()));
	connect(zoom_reset,    SIGNAL(triggered()), ce_view,  SLOT(zoomReset()));
	connect(edit_delete,   SIGNAL(triggered()), ce_scene, SLOT(slot_delete()));
	connect(edit_size_hs,  SIGNAL(triggered()), ce_scene, SLOT(slot_editSizeHotSpot()));
	connect(edit_names,    SIGNAL(triggered()), ce_scene, SLOT(slot_editNames()));
	connect(edit_ori,      SIGNAL(triggered()), ce_scene, SLOT(slot_editOrientations()));
	connect(edit_forward,  SIGNAL(triggered()), ce_scene, SLOT(slot_bringForward()));
	connect(edit_raise,    SIGNAL(triggered()), ce_scene, SLOT(slot_raise()));
	connect(edit_lower,    SIGNAL(triggered()), ce_scene, SLOT(slot_lower()));
	connect(edit_backward, SIGNAL(triggered()), ce_scene, SLOT(slot_sendBackward()));
	connect(move,          SIGNAL(triggered()), ce_scene, SLOT(slot_move()));
	connect(add_line,      SIGNAL(triggered()), ce_scene, SLOT(slot_addLine()));
	connect(add_rectangle, SIGNAL(triggered()), ce_scene, SLOT(slot_addRectangle()));
	connect(add_ellipse,   SIGNAL(triggered()), ce_scene, SLOT(slot_addEllipse()));
	connect(add_circle,    SIGNAL(triggered()), ce_scene, SLOT(slot_addCircle()));
	connect(add_polygon,   SIGNAL(triggered()), ce_scene, SLOT(slot_addPolygon()));
	connect(add_text,      SIGNAL(triggered()), ce_scene, SLOT(slot_addText()));
	connect(add_arc,       SIGNAL(triggered()), ce_scene, SLOT(slot_addArc()));
	connect(add_terminal,  SIGNAL(triggered()), ce_scene, SLOT(slot_addTerminal()));
	connect(add_textfield, SIGNAL(triggered()), ce_scene, SLOT(slot_addTextField()));
	
	connect(move,          SIGNAL(triggered()), this, SLOT(slot_setRubberBandToView()));
	connect(add_line,      SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_rectangle, SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_ellipse,   SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_circle,    SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_polygon,   SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_text,      SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_arc,       SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_terminal,  SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	connect(add_textfield, SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
	
	connect(ce_scene,      SIGNAL(needNormalMode()), this, SLOT(slot_setNormalMode()));
	
	move          -> setCheckable(true);
	add_line      -> setCheckable(true);
	add_rectangle -> setCheckable(true);
	add_ellipse   -> setCheckable(true);
	add_circle    -> setCheckable(true);
	add_polygon   -> setCheckable(true);
	add_text      -> setCheckable(true);
	add_arc       -> setCheckable(true);
	add_terminal  -> setCheckable(true);
	add_textfield -> setCheckable(true);
	
	parts = new QActionGroup(this);
	parts -> addAction(move);
	parts -> addAction(add_line);
	parts -> addAction(add_rectangle);
	parts -> addAction(add_ellipse);
	parts -> addAction(add_circle);
	parts -> addAction(add_polygon);
	parts -> addAction(add_arc);
	parts -> addAction(add_text);
	parts -> addAction(add_textfield);
	parts -> addAction(add_terminal);
	parts -> setExclusive(true);
	
	parts_toolbar = new QToolBar(tr("Parties", "toolbar title"), this);
	parts_toolbar -> setObjectName("parts");
	foreach (QAction *action, parts -> actions()) parts_toolbar -> addAction(action);
	move -> setChecked(true);
	parts_toolbar -> setAllowedAreas(Qt::AllToolBarAreas);
	
	/*
	QAction *xml_preview = new QAction(QIcon(":/ico/dialog-information.png"), tr("XML"), this);
	connect(xml_preview, SIGNAL(triggered()), this, SLOT(xmlPreview()));
	parts_toolbar -> addAction(xml_preview);
	*/
	
	main_toolbar = new QToolBar(tr("Outils", "toolbar title"), this);
	main_toolbar -> setObjectName("main_toolbar");
	view_toolbar = new QToolBar(tr("Affichage", "toolbar title"), this);
	view_toolbar -> setObjectName("display");
	element_toolbar = new QToolBar(tr("\311l\351ment", "toolbar title"), this);
	element_toolbar -> setObjectName("element_toolbar");
	depth_toolbar = new QToolBar(tr("Profondeur", "toolbar title"), this);
	depth_toolbar -> setObjectName("depth_toolbar");
	
	main_toolbar -> addAction(new_element);
	main_toolbar -> addAction(open);
	main_toolbar -> addAction(save);
	main_toolbar -> addAction(save_as);
	main_toolbar -> addAction(reload);
	main_toolbar -> addSeparator();
	main_toolbar -> addAction(undo);
	main_toolbar -> addAction(redo);
	main_toolbar -> addSeparator();
	main_toolbar -> addAction(edit_delete);
	view_toolbar -> addAction(zoom_in);
	view_toolbar -> addAction(zoom_out);
	view_toolbar -> addAction(zoom_fit);
	view_toolbar -> addAction(zoom_reset);
	element_toolbar -> addAction(edit_size_hs);
	element_toolbar -> addAction(edit_names);
	element_toolbar -> addAction(edit_ori);
	depth_toolbar -> addAction(edit_forward);
	depth_toolbar -> addAction(edit_raise);
	depth_toolbar -> addAction(edit_lower);
	depth_toolbar -> addAction(edit_backward);
	
	addToolBar(Qt::TopToolBarArea, main_toolbar);
	addToolBar(Qt::TopToolBarArea, view_toolbar);
	addToolBar(Qt::TopToolBarArea, element_toolbar);
	addToolBar(Qt::TopToolBarArea, depth_toolbar);
	addToolBar(Qt::LeftToolBarArea, parts_toolbar);
	
	connect(ce_scene, SIGNAL(selectionChanged()), this, SLOT(slot_updateInformations()));
	connect(ce_scene, SIGNAL(selectionChanged()), this, SLOT(slot_updateMenus()));
	connect(QApplication::clipboard(),  SIGNAL(dataChanged()),      this, SLOT(slot_updateMenus()));
	connect(&(ce_scene -> undoStack()), SIGNAL(cleanChanged(bool)), this, SLOT(slot_updateMenus()));
	connect(&(ce_scene -> undoStack()), SIGNAL(cleanChanged(bool)), this, SLOT(slot_updateTitle()));
	connect(&(ce_scene -> undoStack()), SIGNAL(indexChanged(int)),  this, SLOT(slot_updatePartsList()));
}

/**
	Met en place les menus.
*/
void QETElementEditor::setupMenus() {
	file_menu    = new QMenu(tr("Fichier"),    this);
	edit_menu    = new QMenu(tr("\311dition"), this);
	display_menu = new QMenu(tr("Affichage"),  this);
	tools_menu   = new QMenu(tr("Outils"),     this);
	help_menu    = new QMenu(tr("Aide"),       this);
	
	file_menu    -> setTearOffEnabled(true);
	edit_menu    -> setTearOffEnabled(true);
	display_menu -> setTearOffEnabled(true);
	tools_menu   -> setTearOffEnabled(true);
	help_menu    -> setTearOffEnabled(true);
	
	file_menu    -> addAction(new_element);
	file_menu    -> addAction(open);
	file_menu    -> addAction(open_file);
	file_menu    -> addMenu(QETApp::elementsRecentFiles() -> menu());
	connect(QETApp::elementsRecentFiles(), SIGNAL(fileOpeningRequested(const QString &)), this, SLOT(openRecentFile(const QString &)));
	file_menu    -> addAction(save);
	file_menu    -> addAction(save_as);
	file_menu    -> addAction(save_as_file);
	file_menu    -> addSeparator();
	file_menu    -> addAction(reload);
	file_menu    -> addSeparator();
	file_menu    -> addAction(quit);
	
	edit_menu -> addAction(undo);
	edit_menu -> addAction(redo);
	edit_menu -> addSeparator();
	edit_menu -> addAction(selectall);
	edit_menu -> addAction(deselectall);
	edit_menu -> addAction(inv_select);
	edit_menu -> addSeparator();
	edit_menu -> addAction(cut);
	edit_menu -> addAction(copy);
	edit_menu -> addAction(paste);
	edit_menu -> addAction(paste_in_area);
	edit_menu -> addSeparator();
	edit_menu -> addAction(edit_delete);
	edit_menu -> addSeparator();
	edit_menu -> addAction(edit_names);
	edit_menu -> addAction(edit_size_hs);
	edit_menu -> addAction(edit_ori);
	edit_menu -> addSeparator();
	edit_menu -> addAction(edit_forward);
	edit_menu -> addAction(edit_raise);
	edit_menu -> addAction(edit_lower);
	edit_menu -> addAction(edit_backward);
	
	// menu Affichage > Afficher
	QMenu *display_toolbars = createPopupMenu();
	display_toolbars -> setTearOffEnabled(true);
	display_toolbars -> setTitle(tr("Afficher"));
	display_menu -> addMenu(display_toolbars);
	
	menuBar() -> addMenu(file_menu);
	menuBar() -> addMenu(edit_menu);
	menuBar() -> addMenu(display_menu);
	
	/*
	menuBar() -> addMenu(tools_menu);
	menuBar() -> addMenu(help_menu);
	*/
}

/**
	Met a jour les menus
*/
void QETElementEditor::slot_updateMenus() {
	bool selected_items = !ce_scene -> selectedItems().isEmpty();
	bool clipboard_elmt = ElementScene::clipboardMayContainElement();
	
	deselectall   -> setEnabled(selected_items);
	cut           -> setEnabled(selected_items);
	copy          -> setEnabled(selected_items);
	paste         -> setEnabled(clipboard_elmt);
	paste_in_area -> setEnabled(clipboard_elmt);
	edit_delete   -> setEnabled(selected_items);
	edit_forward  -> setEnabled(selected_items);
	edit_raise    -> setEnabled(selected_items);
	edit_lower    -> setEnabled(selected_items);
	edit_backward -> setEnabled(selected_items);
	save -> setEnabled(!ce_scene -> undoStack().isClean());
}

/**
	Met a jour le titre de la fenetre
*/
void QETElementEditor::slot_updateTitle() {
	QString title = min_title;
	title += " - " + ce_scene -> names().name() + " ";
	if (!filename_.isEmpty() || !location_.isNull()) {
		if (!ce_scene -> undoStack().isClean()) title += tr("[Modifi\351]", "window title tag");
	}
	if (isReadOnly()) title += tr(" [lecture seule]", "window title tag");
	setWindowTitle(title);
}

/**
	Met en place l'interface
*/
void QETElementEditor::setupInterface() {
	// editeur
	ce_scene = new ElementScene(this, this);
	ce_scene -> slot_move();
	ce_view = new ElementView(ce_scene, this);
	slot_setRubberBandToView();
	setCentralWidget(ce_view);
	
	// widget par defaut dans le QDockWidget
	default_informations = new QLabel();
	
	// ScrollArea pour accueillir un widget d'edition (change a la volee)
	tools_dock_scroll_area_ = new QScrollArea();
	
	// Pile de widgets pour accueillir les deux widgets precedents
	tools_dock_stack_ = new QStackedWidget();
	tools_dock_stack_ -> insertWidget(0, default_informations);
	tools_dock_stack_ -> insertWidget(1, tools_dock_scroll_area_);
	
	// panel sur le cote pour editer les parties
	tools_dock = new QDockWidget(tr("Informations", "dock title"), this);
	tools_dock -> setObjectName("informations");
	tools_dock -> setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	tools_dock -> setFeatures(QDockWidget::AllDockWidgetFeatures);
	tools_dock -> setMinimumWidth(380);
	addDockWidget(Qt::RightDockWidgetArea, tools_dock);
	tools_dock -> setWidget(tools_dock_stack_);
	
	// panel sur le cote pour les annulations
	undo_dock = new QDockWidget(tr("Annulations", "dock title"), this);
	undo_dock -> setObjectName("undo");
	undo_dock -> setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	undo_dock -> setFeatures(QDockWidget::AllDockWidgetFeatures);
	undo_dock -> setMinimumWidth(290);
	addDockWidget(Qt::RightDockWidgetArea, undo_dock);
	QUndoView* undo_view = new QUndoView(&(ce_scene -> undoStack()), this);
	undo_view -> setEmptyLabel(tr("Aucune modification"));
	undo_dock -> setWidget(undo_view);
	
	// panel sur le cote pour la liste des parties
	parts_list = new QListWidget(this);
	parts_list -> setSelectionMode(QAbstractItemView::ExtendedSelection);
	connect(ce_scene,   SIGNAL(partsAdded()),           this, SLOT(slot_createPartsList()));
	connect(ce_scene,   SIGNAL(partsRemoved()),         this, SLOT(slot_createPartsList()));
	connect(ce_scene,   SIGNAL(partsZValueChanged()),   this, SLOT(slot_createPartsList()));
	connect(ce_scene,   SIGNAL(selectionChanged()),     this, SLOT(slot_updatePartsList()));
	connect(parts_list, SIGNAL(itemSelectionChanged()), this, SLOT(slot_updateSelectionFromPartsList()));
	parts_dock = new QDockWidget(tr("Parties", "dock title"), this);
	parts_dock -> setObjectName("parts_list");
	parts_dock -> setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	parts_dock -> setFeatures(QDockWidget::AllDockWidgetFeatures);
	parts_dock -> setMinimumWidth(290);
	tabifyDockWidget(undo_dock, parts_dock);
	parts_dock -> setWidget(parts_list);
	
	slot_updateInformations();
	slot_createPartsList();
	
	// barre d'etat
	statusBar() -> showMessage(tr("\311diteur d'\351l\351ments", "status bar message"));
}

/**
	Passe l'editeur d'element en mode selection : le pointeur deplace les
	elements selectionnes et il est possible d'utiliser un rectangle de selection.
*/
void QETElementEditor::slot_setRubberBandToView() {
	ce_view -> setDragMode(QGraphicsView::RubberBandDrag);
}

/**
	Passe l'editeur d'element en mode immobile (utilise pour la lecture seule)
*/
void QETElementEditor::slot_setNoDragToView() {
	ce_view -> setDragMode(QGraphicsView::NoDrag);
}

/**
	Passe l'editeur en mode normal
*/
void QETElementEditor::slot_setNormalMode() {
	if (!move -> isChecked()) move -> setChecked(true);
	ce_view -> setDragMode(QGraphicsView::RubberBandDrag);
	ce_scene -> slot_move();
}

/**
	Met a jour la zone d'information et d'edition.
	Si plusieurs parties sont selectionnees, seul leur nombre est affiche.
	Sinon, le widget d'edition de la partie est insere.
	@see CustomElementPart::elementInformations()
*/
void QETElementEditor::slot_updateInformations() {
	QList<QGraphicsItem *> selected_qgis = ce_scene -> selectedItems();
	QList<CustomElementPart *> selected_parts;
	foreach(QGraphicsItem *qgi, selected_qgis) {
		if (CustomElementPart *cep = dynamic_cast<CustomElementPart *>(qgi)) {
			selected_parts.append(cep);
		}
	}
	
	clearToolsDock();
	
	if (selected_parts.size() == 1) {
		// recupere le premier CustomElementPart et en ajoute le widget d'edition
		QWidget *edit_widget = selected_parts.first() -> elementInformations();
		tools_dock_scroll_area_ -> setWidget(edit_widget);
		tools_dock_stack_ -> setCurrentIndex(1);
	} else {
		default_informations -> setText(
			tr(
				"%n partie(s) s\351lectionn\351e(s).",
				"",
				selected_parts.size()
			)
		);
		default_informations -> setAlignment(Qt::AlignHCenter | Qt::AlignTop);
		tools_dock_stack_ -> setCurrentIndex(0);
	}
}

/**
	Affiche le code XML correspondant a l'element dans son etat actuel dans
	une boite de dialogue.
*/
void QETElementEditor::xmlPreview() {
	QMessageBox::information(
		this,
		"Export XML",
		ce_scene -> toXml().toString(4)
	);
}

/**
	Verifie si l'ensemble des parties graphiques consituant l'element en cours
	d'edition est bien contenu dans le rectangle representant les limites de
	l'element. Si ce n'est pas le cas, l'utilisateur en est informe.
	@return true si la situation est ok, false sinon
*/
bool QETElementEditor::checkElementSize() {
	if (ce_scene -> borderContainsEveryParts()) {
		return(true);
	} else {
		QMessageBox::warning(
			this,
			tr("Dimensions de l'\351l\351ment", "messagebox title"),
			tr(
				"Attention : certaines parties graphiques (textes, cercles, "
				"lignes...) semblent d\351border du cadre de l'\351l\351ment. Cela"
				" risque de g\351n\351rer des bugs graphiques lors de leur "
				"manipulation sur un sch\351ma. Vous pouvez corriger cela soit "
				"en d\351pla\347ant ces parties, soit en vous rendant dans "
				"\311dition > \311diter la taille et le point de saisie."
				, "messagebox content"
			)
		);
		return(false);
	}
}

/**
	Charge un fichier
	@param filepath Chemin du fichier a charger
*/
void QETElementEditor::fromFile(const QString &filepath) {
	bool state = true;
	QString error_message;
	
	// le fichier doit exister
	QFileInfo infos_file(filepath);
	if (!infos_file.exists() || !infos_file.isFile()) {
		state = false;
		error_message = QString(tr("Le fichier %1 n'existe pas.", "message box content")).arg(filepath);
	}
	
	// le fichier doit etre lisible
	QFile file(filepath);
	if (state) {
		if (!file.open(QIODevice::ReadOnly)) {
			state = false;
			error_message = QString(tr("Impossible d'ouvrir le fichier %1.", "message box content")).arg(filepath);
		}
	}
	
	// le fichier doit etre un document XML
	QDomDocument document_xml;
	if (state) {
		if (!document_xml.setContent(&file)) {
			state = false;
			error_message = tr("Ce fichier n'est pas un document XML valide", "message box content");
		}
		file.close();
	}
	
	if (!state) {
		QMessageBox::critical(this, tr("Erreur", "toolbar title"), error_message);
		return;
	}
	
	// chargement de l'element
	ce_scene -> fromXml(document_xml);
	slot_createPartsList();
	
	// gestion de la lecture seule
	if (!infos_file.isWritable()) {
		QMessageBox::warning(
			this,
			tr("\311dition en lecture seule", "message box title"),
			tr("Vous n'avez pas les privil\350ges n\351cessaires pour modifier cet \351lement. Il sera donc ouvert en lecture seule.", "message box content")
		);
		setReadOnly(true);
	}
	
	// memorise le fichier
	setFileName(filepath);
	QETApp::elementsRecentFiles() -> fileWasOpened(filepath);
	slot_updateMenus();
}

/**
	Enregistre l'element vers un fichier
	@param fn Chemin du fichier a enregistrer
	@return true en cas de reussite, false sinon
*/
bool QETElementEditor::toFile(const QString &fn) {
	QFile file(fn);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("Erreur", "message box title"), tr("Impossible d'\351crire dans ce fichier", "message box content"));
		return(false);
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << ce_scene -> toXml().toString(4);
	file.close();
	return(true);
}

/**
	Enregistre l'element vers un emplacement
	@param location Emplacement de l'element a enregistrer
	@return true en cas de reussite, false sinon
*/
bool QETElementEditor::toLocation(const ElementsLocation &location) {
	ElementsCollectionItem *item = QETApp::collectionItem(location);
	ElementDefinition *element;
	if (item) {
		// l'element existe deja
		element = qobject_cast<ElementDefinition *>(item);
	} else {
		// l'element n'existe pas encore, on demande sa creation
		element = QETApp::createElement(location);
	}
	
	if (!element) {
		QMessageBox::critical(
			this,
			tr("Erreur", "message box title"),
			tr("Impossible d'atteindre l'\351l\351ment", "message box content")
		);
		return(false);
	}
	
	// enregistre l'element
	element -> setXml(ce_scene -> toXml().documentElement());
	if (!element -> write()) {
		QMessageBox::critical(
			this,
			tr("Erreur", "message box title"),
			tr("Impossible d'enregistrer l'\351l\351ment", "message box content")
		);
		return(false);
	}
	
	return(true);
}

/**
	specifie si l'editeur d'element doit etre en mode lecture seule
	@param ro true pour activer le mode lecture seule, false pour le desactiver
*/
void QETElementEditor::setReadOnly(bool ro) {
	read_only = ro;
	// active / desactive les actions
	foreach (QAction *action, parts -> actions()) action -> setEnabled(!ro);
	
	// active / desactive les interactions avec la scene
	ce_view -> setInteractive(!ro);
	
	// active / desactive l'edition de la taille, du hotspot, des noms et des orientations
	cut          -> setEnabled(!ro);
	copy         -> setEnabled(!ro);
	paste        -> setEnabled(!ro);
	selectall    -> setEnabled(!ro);
	deselectall  -> setEnabled(!ro);
	inv_select   -> setEnabled(!ro);
	undo         -> setEnabled(!ro);
	redo         -> setEnabled(!ro);
	edit_delete  -> setEnabled(!ro);
	edit_size_hs -> setEnabled(!ro);
	edit_names   -> setEnabled(!ro);
	edit_ori     -> setEnabled(!ro);
	parts_list   -> setEnabled(!ro);
}

/**
	@return true si l'editeur d'element est en mode lecture seule
*/
bool QETElementEditor::isReadOnly() const {
	return(read_only);
}

/**
	Lance l'assistant de creation d'un nouvel element.
*/
void QETElementEditor::slot_new() {
	NewElementWizard new_element_wizard(this);
	new_element_wizard.exec();
}

/**
	Ouvre un element
*/
void QETElementEditor::slot_open() {
	// demande le chemin virtuel de l'element a ouvrir a l'utilisateur
	ElementsLocation location = ElementDialog::getOpenElementLocation();
	if (location.isNull()) return;
	QETElementEditor *cee = new QETElementEditor();
	cee -> fromLocation(location);
	cee -> show();
}

/**
	Ouvre un fichier
	Demande un fichier a l'utilisateur et ouvre ce fichier
*/
void QETElementEditor::slot_openFile() {
	// demande un nom de fichier a ouvrir a l'utilisateur
	QString user_filename = QFileDialog::getOpenFileName(
		this,
		tr("Ouvrir un fichier", "dialog title"),
		filename_.isEmpty() ? QETApp::customElementsDir() : QDir(filename_).absolutePath(),
		tr(
			"\311l\351ments QElectroTech (*.elmt);;"
			"Fichiers XML (*.xml);;"
			"Tous les fichiers (*)",
			"filetypes allowed when opening an element file"
		)
	);
	openElement(user_filename);
}

/**
	Slot utilise pour ouvrir un fichier recent.
	Transfere filepath au slot openElement seulement si cet editeur est actif
	@param filepath Fichier a ouvrir
	@see openElement
*/
void QETElementEditor::openRecentFile(const QString &filepath) {
	if (qApp -> activeWindow() != this) return;
	openElement(filepath);
}

/**
	Ouvre un fichier element dans un nouvel editeur
	Cette methode ne controle pas si le fichier est deja ouvert
	@param filepath Fichier a ouvrir
	@see fromFile
*/
void QETElementEditor::openElement(const QString &filepath) {
	if (filepath.isEmpty()) return;
	QETElementEditor *cee = new QETElementEditor();
	cee -> fromFile(filepath);
	cee -> show();
}

/**
	Recharge l'element edite
*/
void QETElementEditor::slot_reload() {
	// s'il ya des modifications, on demande a l'utilisateur s'il est certain
	// de vouloir recharger
	if (!ce_scene -> undoStack().isClean()) {
		QMessageBox::StandardButton answer = QMessageBox::question(
			this,
			tr("Recharger l'\351l\351ment", "dialog title"),
			tr("Vous avez efffectu\351 des modifications sur cet \351l\351ment. Si vous le rechargez, ces modifications seront perdues. Voulez-vous vraiment recharger l'\351l\351ment ?", "dialog content"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
			QMessageBox::Cancel
		);
		if (answer != QMessageBox::Yes) return;
	}
	
	// recharge l'element
	if (opened_from_file) {
		// l'element a ete ouvert a partir d'un chemin de fichier
		ce_scene -> reset();
		fromFile(filename_);
	} else {
		// l'element a ete ouvert a partir d'un emplacement (ElementsLocation)
		// il peut s'agir aussi bien d'un fichier que d'un element XML
		if (ElementsCollectionItem *item = QETApp::collectionItem(location_)) {
			item -> reload();
			ce_scene -> reset();
			fromLocation(location_);
		}
	}
}

/**
	Enregistre l'element en cours d'edition.
	Si le nom du fichier en cours n'est pas connu, cette methode equivaut a
	l'action "Enregistrer sous"
	@see slot_saveAs()
*/
bool QETElementEditor::slot_save() {
	// verification avant d'enregistrer le fichier
	checkElementSize();
	
	// si on ne connait pas le nom du fichier en cours, enregistrer revient a enregistrer sous
	if (opened_from_file) {
		if (filename_.isEmpty()) return(slot_saveAsFile());
		// sinon on enregistre dans le nom de fichier connu
		bool result_save = toFile(filename_);
		if (result_save) ce_scene -> undoStack().setClean();
		return(result_save);
	} else {
		if (location_.isNull()) return(slot_saveAs());
		// sinon on enregistre a l'emplacement connu
		bool result_save = toLocation(location_);
		if (result_save) ce_scene -> undoStack().setClean();
		return(result_save);
	}
}

/**
	Demande une localisation a l'utilisateur et enregistre l'element
*/
bool QETElementEditor::slot_saveAs() {
	// demande une localisation a l'utilisateur
	ElementsLocation location = ElementDialog::getSaveElementLocation();
	if (location.isNull()) return(false);
	
	// tente l'enregistrement
	bool result_save = toLocation(location);
	if (result_save) {
		setLocation(location);
		ce_scene -> undoStack().setClean();
	}
	
	// retourne un booleen representatif de la reussite de l'enregistrement
	return(result_save);
}

/**
	Demande un nom de fichier a l'utilisateur et enregistre l'element
*/
bool QETElementEditor::slot_saveAsFile() {
	// demande un nom de fichier a l'utilisateur pour enregistrer l'element
	QString fn = QFileDialog::getSaveFileName(
		this,
		tr("Enregistrer sous", "dialog title"),
		filename_.isEmpty() ? QETApp::customElementsDir() : QDir(filename_).absolutePath(),
		tr(
			"\311l\351ments QElectroTech (*.elmt)",
			"filetypes allowed when saving an element file"
		)
	);
	// si aucun nom n'est entre, renvoie faux.
	if (fn.isEmpty()) return(false);
	// si le nom ne se termine pas par l'extension .elmt, celle-ci est ajoutee
	if (!fn.endsWith(".elmt", Qt::CaseInsensitive)) fn += ".elmt";
	// tente d'enregistrer le fichier
	bool result_save = toFile(fn);
	// si l'enregistrement reussit, le nom du fichier est conserve
	if (result_save) {
		setFileName(fn);
		QETApp::elementsRecentFiles() -> fileWasOpened(fn);
		ce_scene -> undoStack().setClean();
	}
	// retourne un booleen representatif de la reussite de l'enregistrement
	return(result_save);
}

/**
	@return true si l'element peut etre ferme.
	Un element peut etre ferme s'il ne comporte aucune modification.
	Si l'element comporte des modifications, la question est posee a
	l'utilisateur.
*/
bool QETElementEditor::canClose() {
	if (ce_scene -> undoStack().isClean()) return(true);
	// demande d'abord a l'utilisateur s'il veut enregistrer l'element en cours
	QMessageBox::StandardButton answer = QMessageBox::question(
		this,
		tr("Enregistrer l'\351l\351ment en cours ?", "dialog title"),
		QString(
			tr(
				"Voulez-vous enregistrer l'\351l\351ment %1 ?",
				"dialog content - %1 is an element name"
			)
		).arg(ce_scene -> names().name()),
		QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
		QMessageBox::Cancel
	);
	bool result;
	switch(answer) {
		case QMessageBox::Cancel: result = false;         break; // l'utilisateur annule : echec de la fermeture
		case QMessageBox::Yes:    result = slot_save();   break; // l'utilisateur dit oui : la reussite depend de l'enregistrement
		default:                  result = true;                 // l'utilisateur dit non ou ferme le dialogue: c'est reussi
	}
	return(result);
}

/**
	Enleve et cache le widget affiche par le dock permettant d'editer les
	parties.
	@return le widget enleve, ou 0 s'il n'y avait pas de widget a enlever
*/
QWidget *QETElementEditor::clearToolsDock() {
	if (QWidget *previous_widget = tools_dock_scroll_area_ -> takeWidget()) {
		previous_widget -> setParent(0);
		previous_widget -> hide();
		return(previous_widget);
	}
	return(0);
}

/**
	Permet de quitter l'editeur lors de la fermeture de la fenetre principale
	@param qce Le QCloseEvent correspondant a l'evenement de fermeture
*/
void QETElementEditor::closeEvent(QCloseEvent *qce) {
	if (canClose()) {
		writeSettings();
		setAttribute(Qt::WA_DeleteOnClose);
		qce -> accept();
	} else qce -> ignore();
}

/**
	Remplit la liste des parties
*/
void QETElementEditor::slot_createPartsList() {
	parts_list -> blockSignals(true);
	parts_list -> clear();
	QList<QGraphicsItem *> qgis = ce_scene -> zItems(true);
	for (int j = qgis.count() - 1 ; j >= 0 ; -- j) {
		QGraphicsItem *qgi = qgis[j];
		if (CustomElementPart *cep = dynamic_cast<CustomElementPart *>(qgi)) {
			QString part_desc = cep -> name();
			QListWidgetItem *qlwi = new QListWidgetItem(part_desc);
			QVariant v;
			v.setValue<QGraphicsItem *>(qgi);
			qlwi -> setData(42, v);
			parts_list -> addItem(qlwi);
			qlwi -> setSelected(qgi -> isSelected());
		}
	}
	parts_list -> blockSignals(false);
}

/**
	Met a jour la selection dans la liste des parties
*/
void QETElementEditor::slot_updatePartsList() {
	if (parts_list -> count() != ce_scene -> items().count()) {
		slot_createPartsList();
	} else {
		parts_list -> blockSignals(true);
		int i = 0;
		QList<QGraphicsItem *> items = ce_scene -> zItems(true);
		for (int j = items.count() - 1 ; j >= 0 ; -- j) {
			QGraphicsItem *qgi = items[j];
			QListWidgetItem *qlwi = parts_list -> item(i);
			if (qlwi) qlwi -> setSelected(qgi -> isSelected());
			++ i;
		}
		parts_list -> blockSignals(false);
	}
}

/**
	Met a jour la selection des parties de l'element a partir de la liste des
	parties
*/
void QETElementEditor::slot_updateSelectionFromPartsList() {
	ce_scene  -> blockSignals(true);
	parts_list -> blockSignals(true);
	for (int i = 0 ; i < parts_list -> count() ; ++ i) {
		QListWidgetItem *qlwi = parts_list -> item(i);
		QGraphicsItem *qgi = qlwi -> data(42).value<QGraphicsItem *>();
		if (qgi) {
			qgi -> setSelected(qlwi -> isSelected());
		}
	}
	parts_list -> blockSignals(false);
	ce_scene -> blockSignals(false);
	slot_updateInformations();
	slot_updateMenus();
}

/// Lit les parametres de l'editeur d'element
void QETElementEditor::readSettings() {
	QSettings &settings = QETApp::settings();
	
	// dimensions et position de la fenetre
	QVariant geometry = settings.value("elementeditor/geometry");
	if (geometry.isValid()) restoreGeometry(geometry.toByteArray());
	
	// etat de la fenetre (barres d'outils, docks...)
	QVariant state = settings.value("elementeditor/state");
	if (state.isValid()) restoreState(state.toByteArray());
}

/// Enregistre les parametres de l'editeur d'element
void QETElementEditor::writeSettings() {
	QSettings &settings = QETApp::settings();
	settings.setValue("elementeditor/geometry", saveGeometry());
	settings.setValue("elementeditor/state", saveState());
}

/**
	@return les decalages horizontaux et verticaux (sous la forme d'un point) a
	utiliser lors d'un copier/coller avec decalage.
*/
QPointF QETElementEditor::pasteOffset() {
	QPointF paste_offset(5.0, 0.0);
	return(paste_offset);
}

/**
	@return Le type de mouvement a effectuer lors d'un copier/coller avec
	decalage.
*/
QET::OrientedMovement QETElementEditor::pasteMovement() {
	return(QET::ToEast);
}

/**
	@param location Emplacement de l'element a editer
*/
void QETElementEditor::fromLocation(const ElementsLocation &location) {
	
	// l'element doit exister
	ElementsCollectionItem *item = QETApp::collectionItem(location);
	ElementDefinition *element = 0;
	if (!item) {
		QMessageBox::critical(
			this,
			tr("\311l\351ment inexistant.", "message box title"),
			tr("L'\351l\351ment n'existe pas.", "message box content")
		);
		return;
	}
	
	if (!item -> isElement() || !(element = qobject_cast<ElementDefinition *>(item)) || element -> isNull()) {
		QMessageBox::critical(
			this,
			tr("\311l\351ment inexistant.", "message box title"),
			tr("Le chemin virtuel choisi ne correspond pas \340 un \351l\351ment.", "message box content")
		);
		return;
	}
	
	// le fichier doit etre un document XML
	QDomDocument document_xml;
	QDomNode node = document_xml.importNode(element -> xml(), true);
	document_xml.appendChild(node);
	
	// chargement de l'element
	ce_scene -> fromXml(document_xml);
	slot_createPartsList();
	
	// gestion de la lecture seule
	if (!element -> isWritable()) {
		QMessageBox::warning(
			this,
			tr("\311dition en lecture seule", "message box title"),
			tr("Vous n'avez pas les privil\350ges n\351cessaires pour modifier cet \351lement. Il sera donc ouvert en lecture seule.", "message box content")
		);
		setReadOnly(true);
	}
	
	// memorise le fichier
	setLocation(location);
	slot_updateMenus();
}
