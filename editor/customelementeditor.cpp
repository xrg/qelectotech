#include "customelementeditor.h"
#include "editorscene.h"
#include "customelementpart.h"
#include "newelementwizard.h"
#include "qetapp.h"

CustomElementEditor::CustomElementEditor(QWidget *parent) :
	QMainWindow(parent),
	read_only(false),
	min_title(tr("QElectroTech - \311diteur d'\351l\351ment")),
	_filename(QString())
{
	setMinimumWidth(700);
	setMinimumHeight(450);
	setWindowTitle(min_title);
	setWindowIcon(QIcon(":/ico/qet.png"));
	
	setupInterface();
	setupActions();
	setupMenus();
}

CustomElementEditor::~CustomElementEditor() {
	qDebug() << "~CustomElementEditor()";
}

void CustomElementEditor::setupActions() {
	new_element   = new QAction(QIcon(":/ico/new.png"),          tr("&Nouveau"),                    this);
	open          = new QAction(QIcon(":/ico/open.png"),         tr("&Ouvrir"),                     this);
	save          = new QAction(QIcon(":/ico/save.png"),         tr("&Enregistrer"),                this);
	save_as       = new QAction(QIcon(":/ico/saveas.png"),       tr("Enregistrer sous"),            this);
	quit          = new QAction(QIcon(":/ico/exit.png"),         tr("&Quitter"),                    this);
	selectall     = new QAction(                                 tr("Tout s\351lectionner"),        this);
	deselectall   = new QAction(                                 tr("D\351s\351lectionner tout"),   this);
	inv_select    = new QAction(                                 tr("Inverser la s\351lection"),    this);
	edit_delete   = new QAction(QIcon(":/ico/delete.png"),       tr("&Supprimer"),                  this);
	edit_size     = new QAction(                                 tr("Modifier la taille"),          this);
	edit_hotspot  = new QAction(                                 tr("Modifier le point de saisie"), this);
	edit_names    = new QAction(                                 tr("\311diter les noms"),          this);
	edit_ori      = new QAction(QIcon(":/ico/orientations.png"), tr("\311diter les orientations"),  this);
	move          = new QAction(QIcon(":/ico/select.png"),       tr("D\351placer un objet"),        this);
	add_line      = new QAction(QIcon(":/ico/line.png"),         tr("Ajouter une ligne"),           this);
	add_ellipse   = new QAction(QIcon(":/ico/ellipse.png"),      tr("Ajouter une ellipse"),         this);
	add_circle    = new QAction(QIcon(":/ico/circle.png"),       tr("Ajouter un cercle"),           this);
	add_polygon   = new QAction(QIcon(":/ico/polygon.png"),      tr("Ajouter un polygone"),         this);
	add_text      = new QAction(QIcon(":/ico/text.png"),         tr("Ajouter du texte"),            this);
	add_arc       = new QAction(QIcon(":/ico/arc.png"),          tr("Ajouter un arc de cercle"),    this);
	add_terminal  = new QAction(QIcon(":/ico/terminal.png"),     tr("Ajouter une borne"),           this);
	add_textfield = new QAction(QIcon(":/ico/textfield.png"),    tr("Ajouter un champ de texte"),   this);
	
	edit_delete  -> setEnabled(false);
	edit_size    -> setEnabled(false);
	edit_hotspot -> setEnabled(false);
	
	selectall         -> setShortcut(QKeySequence::SelectAll);
	deselectall       -> setShortcut(QKeySequence(tr("Ctrl+Shift+A")));
	inv_select        -> setShortcut(QKeySequence(tr("Ctrl+I")));
	edit_delete       -> setShortcut(QKeySequence(tr("Ctrl+Suppr")));
	
	connect(new_element,   SIGNAL(triggered()), this,     SLOT(slot_new()));
	connect(open,          SIGNAL(triggered()), this,     SLOT(slot_open()));
	connect(save,          SIGNAL(triggered()), this,     SLOT(slot_save()));
	connect(save_as,       SIGNAL(triggered()), this,     SLOT(slot_saveAs()));
	connect(quit,          SIGNAL(triggered()), this,     SLOT(slot_quit()));
	connect(selectall,     SIGNAL(triggered()), ce_scene, SLOT(slot_selectAll()));
	connect(deselectall,   SIGNAL(triggered()), ce_scene, SLOT(slot_deselectAll()));
	connect(inv_select,    SIGNAL(triggered()), ce_scene, SLOT(slot_invertSelection()));
	connect(edit_delete,   SIGNAL(triggered()), ce_scene, SLOT(slot_delete()));
	connect(edit_size,     SIGNAL(triggered()), ce_scene, SLOT(slot_editSize()));
	connect(edit_hotspot,  SIGNAL(triggered()), ce_scene, SLOT(slot_editHotSpot()));
	connect(edit_names,    SIGNAL(triggered()), ce_scene, SLOT(slot_editNames()));
	connect(edit_ori,      SIGNAL(triggered()), ce_scene, SLOT(slot_editOrientations()));
	connect(move,          SIGNAL(triggered()), ce_scene, SLOT(slot_move()));
	connect(add_line,      SIGNAL(triggered()), ce_scene, SLOT(slot_addLine()));
	connect(add_ellipse,   SIGNAL(triggered()), ce_scene, SLOT(slot_addEllipse()));
	connect(add_circle,    SIGNAL(triggered()), ce_scene, SLOT(slot_addCircle()));
	connect(add_polygon,   SIGNAL(triggered()), ce_scene, SLOT(slot_addPolygon()));
	connect(add_text,      SIGNAL(triggered()), ce_scene, SLOT(slot_addText()));
	connect(add_arc,       SIGNAL(triggered()), ce_scene, SLOT(slot_addArc()));
	connect(add_terminal,  SIGNAL(triggered()), ce_scene, SLOT(slot_addTerminal()));
	connect(add_textfield, SIGNAL(triggered()), ce_scene, SLOT(slot_addTextField()));
	
	connect(move,          SIGNAL(triggered()), this, SLOT(slot_setRubberBandToView()));
	connect(add_line,      SIGNAL(triggered()), this, SLOT(slot_setNoDragToView()));
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
	parts -> addAction(add_ellipse);
	parts -> addAction(add_circle);
	parts -> addAction(add_polygon);
	parts -> addAction(add_text);
	parts -> addAction(add_arc);
	parts -> addAction(add_textfield);
	parts -> addAction(add_terminal);
	parts -> setExclusive(true);
	
	parts_toolbar = new QToolBar(tr("Parties"), this);
	foreach (QAction *action, parts -> actions()) parts_toolbar -> addAction(action);
	move -> setChecked(true);
	parts_toolbar -> setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);
	
	QAction *xml_preview = new QAction(QIcon(":/ico/info.png"), tr("XML"), this);
	connect(xml_preview, SIGNAL(triggered()), this, SLOT(xmlPreview()));
	parts_toolbar -> addAction(xml_preview);
	
	addToolBar(Qt::LeftToolBarArea, parts_toolbar);
	
	connect(ce_scene, SIGNAL(selectionChanged()), this, SLOT(slot_updateInformations()));
	connect(ce_scene, SIGNAL(selectionChanged()), this, SLOT(slot_updateMenus()));
}

void CustomElementEditor::setupMenus() {
	file_menu    = new QMenu(tr("Fichier"));
	edit_menu    = new QMenu(tr("\311dition"));
	display_menu = new QMenu(tr("Affichage"));
	tools_menu   = new QMenu(tr("Outils"));
	help_menu    = new QMenu(tr("Aide"));
	
	file_menu    -> setTearOffEnabled(true);
	edit_menu    -> setTearOffEnabled(true);
	display_menu -> setTearOffEnabled(true);
	tools_menu   -> setTearOffEnabled(true);
	help_menu    -> setTearOffEnabled(true);
	
	file_menu    -> addAction(new_element);
	file_menu    -> addAction(open);
	file_menu    -> addAction(save);
	file_menu    -> addAction(save_as);
	file_menu    -> addSeparator();
	file_menu    -> addAction(quit);
	
	edit_menu -> addAction(selectall);
	edit_menu -> addAction(deselectall);
	edit_menu -> addAction(inv_select);
	edit_menu -> addSeparator();
	edit_menu -> addAction(edit_delete);
	edit_menu -> addSeparator();
	edit_menu -> addAction(edit_names);
	edit_menu -> addAction(edit_size);
	edit_menu -> addAction(edit_hotspot);
	edit_menu -> addAction(edit_ori);
	
	menuBar() -> addMenu(file_menu);
	menuBar() -> addMenu(edit_menu);
	menuBar() -> addMenu(display_menu);
	menuBar() -> addMenu(tools_menu);
	menuBar() -> addMenu(help_menu);
}

void CustomElementEditor::slot_updateMenus() {
	edit_delete -> setEnabled(!ce_scene -> selectedItems().isEmpty());
}

void CustomElementEditor::setupInterface() {
	// editeur
	ce_scene = new EditorScene(this);
	ce_scene -> slot_move();
	ce_view = new QGraphicsView(ce_scene, this);
	ce_view -> setInteractive(true);
	ce_view -> setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	//ce_view -> setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	ce_view -> setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	//ce_view -> setSceneRect(QRectF(0.0, 0.0, 50.0, 200.0));
	ce_view -> scale(4.0, 4.0);
	slot_setRubberBandToView();
	setCentralWidget(ce_view);
	
	// widget par defaut dans le QDockWidget
	default_informations = new QLabel();
	
	// panel sur le cote
	tools_dock = new QDockWidget(tr("Informations"), this);
	tools_dock -> setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	tools_dock -> setFeatures(QDockWidget::AllDockWidgetFeatures);
	tools_dock -> setMinimumWidth(285);
	addDockWidget(Qt::RightDockWidgetArea, tools_dock);
	QWidget *info_widget = new QWidget();
	info_widget -> setLayout(new QVBoxLayout(info_widget));
	tools_dock -> setWidget(info_widget);
	slot_updateInformations();
	
	// barre d'etat
	statusBar() -> showMessage(tr("\311diteur d'\351l\351ments"));
}

void CustomElementEditor::slot_setRubberBandToView() {
	ce_view -> setDragMode(QGraphicsView::RubberBandDrag);
}

void CustomElementEditor::slot_setNoDragToView() {
	ce_view -> setDragMode(QGraphicsView::NoDrag);
}

void CustomElementEditor::slot_setNormalMode() {
	if (!move -> isChecked()) move -> setChecked(true);
	ce_view -> setDragMode(QGraphicsView::RubberBandDrag);
	ce_scene -> slot_move();
}

void CustomElementEditor::slot_updateInformations() {
	QList<QGraphicsItem *> selected_qgis = ce_scene -> selectedItems();
	QList<CustomElementPart *> selected_parts;
	foreach(QGraphicsItem *qgi, selected_qgis) {
		if (CustomElementPart *cep = dynamic_cast<CustomElementPart *>(qgi)) {
			selected_parts.append(cep);
		}
	}
	
	// recupere le layout
	QLayout *layout = tools_dock -> widget() -> layout();
	
	// enleve les widgets deja presents
	QLayoutItem *qli;
	while ((qli = layout -> takeAt(0)) != 0) {
		if (qli -> widget()) {
			layout -> removeWidget(qli -> widget());
			qli -> widget() -> setParent(0);
		}
	}
	if (selected_parts.size() == 1) {
		// recupere le premier CustomElementPart et en ajoute le widget d'edition
		layout -> addWidget(selected_parts.first() -> elementInformations());
	} else {
		default_informations -> setText(
			selected_parts.size() ?
			QString("%1").arg(selected_parts.size()) + tr(" parties s\351lectionn\351es.") :
			tr("Aucune partie s\351lectionn\351e.")
		);
		layout -> addWidget(default_informations);
	}
}

void CustomElementEditor::xmlPreview() {
	QMessageBox::information(
		this,
		"Export XML",
		ce_scene -> toXml().toString(4)
	);
}

void CustomElementEditor::fromFile(const QString &filepath) {
	bool state = true;
	QString error_message;
	
	// le fichier doit exister
	QFileInfo infos_file(filepath);
	if (!infos_file.exists() || !infos_file.isFile()) {
		state = false;
		error_message = tr("Le fichier ") + filepath + tr(" n'existe pas.");
	}
	
	// le fichier doit etre lisible
	QFile file(filepath);
	if (state) {
		if (!file.open(QIODevice::ReadOnly)) {
			state = false;
			error_message = tr("Impossible d'ouvrir le fichier ") + filepath + ".";
		}
	}
	
	// le fichier doit etre un document XML
	QDomDocument document_xml;
	if (state) {
		if (!document_xml.setContent(&file)) {
			state = false;
			error_message = tr("Ce fichier n'est pas un document XML valide");
		}
		file.close();
	}
	
	if (!state) {
		QMessageBox::critical(this, tr("Erreur"), error_message);
		return;
	}
	
	// chargement de l'element
	ce_scene -> fromXml(document_xml);
	
	// gestion de la lecture seule
	if (!infos_file.isWritable()) {
		QMessageBox::warning(
			this,
			tr("\311dition en lecture seule"),
			tr("Vous n'avez pas les privil\350ges n\351cessaires pour modifier cet \351lement. Il sera donc ouvert en lecture seule.")
		);
		setReadOnly(true);
	}
	
	// memorise le fichier
	_filename = filepath;
	
	// modifie le titre de la fenetre
	QString new_title = min_title + " - " + ce_scene -> names().name();
	if (isReadOnly()) new_title += tr(" [lecture seule]");
	setWindowTitle(new_title);
}

bool CustomElementEditor::toFile(const QString &fn) {
	QFile file(fn);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("Erreur"), tr("Impossible d'ecrire dans ce fichier"));
		return(false);
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << ce_scene -> toXml().toString(4);
	file.close();
	return(true);
}

/**
	specifie si l'editeur d'element doit etre en mode lecture seule
	@param ro true pour activer le mode lecture seule, false pour le desactiver
*/
void CustomElementEditor::setReadOnly(bool ro) {
	read_only = ro;
	// active / desactive les actions
	foreach (QAction *action, parts -> actions()) action -> setEnabled(!ro);
	
	// active / desactive les interactions avec la scene
	ce_view -> setInteractive(!ro);
	
	// active / desactive l'edition de la taille, du hotspot, des noms et des orientations
	edit_size    -> setEnabled(!ro);
	edit_hotspot -> setEnabled(!ro);
	edit_names   -> setEnabled(!ro);
	edit_ori     -> setEnabled(!ro);
}

/**
	@return true si l'editeur d'element est en mode lecture seule
*/
bool CustomElementEditor::isReadOnly() const {
	return(read_only);
}

void CustomElementEditor::slot_new() {
	NewElementWizard new_element_wizard;
	new_element_wizard.exec();
}

void CustomElementEditor::slot_open() {
	// demande un nom de fichier a ouvrir a l'utilisateur
	QString user_filename = QFileDialog::getOpenFileName(
		this,
		tr("Ouvrir un fichier"),
		QETApp::customElementsDir(),
		tr("\311l\351ments QElectroTech (*.elmt);;Fichiers XML (*.xml);;Tous les fichiers (*)")
	);
	if (user_filename == "") return;
	CustomElementEditor *cee = new CustomElementEditor();
	cee -> fromFile(user_filename);
	cee -> show();
}

bool CustomElementEditor::slot_save() {
	// si on ne connait pas le nom du fichier en cours, enregistrer revient a enregistrer sous
	if (_filename == QString()) return(slot_saveAs());
	// sinon on enregistre dans le nom de fichier connu
	return(toFile(_filename));
}

bool CustomElementEditor::slot_saveAs() {
	// demande un nom de fichier a l'utilisateur pour enregistrer l'element
	QString fn = QFileDialog::getSaveFileName(
		this,
		tr("Enregistrer sous"),
		QETApp::customElementsDir(),
		tr("\311l\351ments QElectroTech (*.elmt)")
	);
	// si aucun nom n'est entre, renvoie faux.
	if (fn == "") return(false);
	// si le nom ne se termine pas par l'extension .elmt, celle-ci est ajoutee
	if (!fn.endsWith(".elmt", Qt::CaseInsensitive)) fn += ".elmt";
	// tente d'enregistrer le fichier
	bool result_save = toFile(fn);
	// si l'enregistrement reussit, le nom du fichier est conserve
	if (result_save) setFileName(fn);
	// retourne un booleen representatif de la reussite de l'enregistrement
	return(result_save);
}

void CustomElementEditor::slot_quit(QCloseEvent *event) {
	if (close()) {
		if (event != NULL) event -> accept();
		delete(this);
	} else if (event != NULL) event -> ignore();
}

bool CustomElementEditor::close() {
	// demande d'abord a l'utilisateur s'il veut enregistrer l'element en cours
	QMessageBox::StandardButton answer = QMessageBox::question(
		this,
		tr("Enregistrer l'\351l\351ment en cours ?"),
		tr("Voulez-vous enregistrer l'\351l\351ment ") + ce_scene -> names().name() + tr(" ?"),
		QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
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
	Permet de quitter l'editeur lors de la fermeture de la fenetre principale
	@param qce Le QCloseEvent correspondant a l'evenement de fermeture
*/
void CustomElementEditor::closeEvent(QCloseEvent *qce) {
	slot_quit(qce);
}