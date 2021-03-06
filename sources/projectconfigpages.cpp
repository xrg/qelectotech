#include "projectconfigpages.h"
#include "qeticons.h"
#include "qetproject.h"
#include "borderpropertieswidget.h"
#include "conductorpropertieswidget.h"
#include "diagramcontextwidget.h"
#include "titleblockpropertieswidget.h"
#include <QtGui>

/**
	Constructor
	@param project Project this page is editing.
	@param parent Parent QWidget
*/
ProjectConfigPage::ProjectConfigPage(QETProject *project, QWidget *parent) :
	ConfigPage(parent),
	project_(project)
{
}

/**
	Destructor
*/
ProjectConfigPage::~ProjectConfigPage() {
}

/**
	@return the project being edited by this page
*/
QETProject *ProjectConfigPage::project() const {
	return(project_);
}

/**
	Set \a new_project as the project being edited by this page.
	@param read_values True to read values from the project into widgets before setting them read only accordingly, false otherwise. Defaults to true.
	@return the former project
*/
QETProject *ProjectConfigPage::setProject(QETProject *new_project, bool read_values) {
	if (new_project == project_) return(project_);
	
	QETProject *former_project = project_;
	project_ = new_project;
	if (project_ && read_values) {
		readValuesFromProject();
		adjustReadOnly();
	}
	return(former_project);
}

/**
	Apply the configuration after user input
*/
void ProjectConfigPage::applyConf() {
	if (!project_ || project_ -> isReadOnly()) return;
	applyProjectConf();
}

/**
	Initialize the page by calling initWidgets() and initLayout(). Also call
	readValuesFromProject() and adjustReadOnly() if a non-zero project has been
	set. Typically, you should call this function in your subclass constructor.
*/
void ProjectConfigPage::init() {
	initWidgets();
	initLayout();
	if (project_) {
		readValuesFromProject();
		adjustReadOnly();
	}
}

/**
	Constructor
	@param project Project this page is editing.
	@param parent Parent QWidget
*/
ProjectMainConfigPage::ProjectMainConfigPage(QETProject *project, QWidget *parent) :
	ProjectConfigPage(project, parent)
{
	init();
}

/**
	Destructor
*/
ProjectMainConfigPage::~ProjectMainConfigPage() {
}

/**
	@return the title for this page
*/
QString ProjectMainConfigPage::title() const {
	return(tr("G\351n\351ral", "configuration page title"));
}

/**
	@return the icon for this page
*/
QIcon ProjectMainConfigPage::icon() const {
	return(QET::Icons::Settings);
}

/**
	Apply the configuration after user input
*/
void ProjectMainConfigPage::applyProjectConf() {
	bool modified_project = false;
	
	QString new_title = title_value_ -> text();
	if (project_ -> title() != new_title) {
		project_ -> setTitle(new_title);
		modified_project = true;
	}
	
	DiagramContext new_properties = project_variables_ -> context();
	if (project_ -> projectProperties() != new_properties) {
		project_ -> setProjectProperties(new_properties);
		modified_project = true;
	}
	
	if (modified_project) {
		project_ -> setModified(true);
	}
}

/**
	@return the project title entered by the user
*/
QString ProjectMainConfigPage::projectTitle() const {
	return(title_value_ -> text());
}

/**
	Initialize widgets displayed by the page.
*/
void ProjectMainConfigPage::initWidgets() {
	title_label_ = new QLabel(tr("Titre du projet\240:", "label when configuring"));
	title_value_ = new QLineEdit();
	title_information_ = new QLabel(tr("Ce titre sera disponible pour tous les sch\351mas de ce projet en tant que %projecttitle.", "informative label"));
	project_variables_label_ = new QLabel(
		tr(
			"Vous pouvez d\351finir ci-dessous des propri\351t\351s personnalis\351es qui seront disponibles pour tous les sch\351mas de ce projet (typiquement pour les cartouches).",
			 "informative label"
		)
	);
	project_variables_label_ -> setWordWrap(true);
	project_variables_ = new DiagramContextWidget();
	project_variables_ -> setContext(DiagramContext());
}

/**
	Initialize the layout of this page.
*/
void ProjectMainConfigPage::initLayout() {
	QVBoxLayout *main_layout0 = new QVBoxLayout();
	QHBoxLayout *title_layout0 = new QHBoxLayout();
	title_layout0 -> addWidget(title_label_);
	title_layout0 -> addWidget(title_value_);
	main_layout0 -> addLayout(title_layout0);
	main_layout0 -> addWidget(title_information_);
	main_layout0 -> addSpacing(10);
	main_layout0 -> addWidget(project_variables_label_);
	main_layout0 -> addWidget(project_variables_);
	setLayout(main_layout0);
}

/**
	Read properties from the edited project then fill widgets with them.
*/
void ProjectMainConfigPage::readValuesFromProject() {
	title_value_ -> setText(project_ -> title());
	project_variables_ -> setContext(project_ -> projectProperties());
}

/**
	Set the content of this page read only if the project is read only,
	editable if the project is editable.
 */
void ProjectMainConfigPage::adjustReadOnly() {
	bool is_read_only = project_ -> isReadOnly();
	title_value_ -> setReadOnly(is_read_only);
}

/**
	Constructor
	@param project Project this page is editing.
	@param parent Parent QWidget
*/
ProjectNewDiagramConfigPage::ProjectNewDiagramConfigPage(QETProject *project, QWidget *parent) :
	ProjectConfigPage(project, parent)
{
	init();
}

/**
	Destructor
*/
ProjectNewDiagramConfigPage::~ProjectNewDiagramConfigPage() {
}

/**
	@return the title for this page
*/
QString ProjectNewDiagramConfigPage::title() const {
	return(tr("Nouveau sch\351ma", "project configuration page title"));
}

/**
	@return the icon for this page
*/
QIcon ProjectNewDiagramConfigPage::icon() const {
	return(QET::Icons::NewDiagram);
}

/**
	Apply the configuration after user input
*/
void ProjectNewDiagramConfigPage::applyProjectConf() {
	bool modified_project = false;
	
	BorderProperties new_border_prop = border_ -> borderProperties();
	if (project_ -> defaultBorderProperties() != new_border_prop) {
		project_ -> setDefaultBorderProperties(border_ -> borderProperties());
		modified_project = true;
	}
	
	TitleBlockProperties new_tbt_prop = titleblock_ -> titleBlockProperties();
	if (project_ -> defaultTitleBlockProperties() != new_tbt_prop) {
		project_ -> setDefaultTitleBlockProperties(titleblock_ -> titleBlockProperties());
		modified_project = true;
	}
	
	ConductorProperties new_conductor_prop = conductor_ -> conductorProperties();
	if (project_ -> defaultConductorProperties() != new_conductor_prop) {
		project_ -> setDefaultConductorProperties(conductor_ -> conductorProperties());
		modified_project = true;
	}
	
	if (modified_project) {
		project_ -> setModified(modified_project);
	}
}

/**
	Initialize widgets displayed by the page.
*/
void ProjectNewDiagramConfigPage::initWidgets() {
	informative_label_ = new QLabel(
		tr(
			"Propri\351t\351s \340 utiliser lors de l'ajout d'un nouveau sch\351ma au projet :",
			"explicative label"
		)
	);
	border_ = new BorderPropertiesWidget(BorderProperties()); 
	titleblock_ = new TitleBlockPropertiesWidget(TitleBlockProperties(), true);
	conductor_ = new ConductorPropertiesWidget();
	conductor_ -> setContentsMargins(0, 0, 0, 0);
}

/**
	Initialize the layout of this page.
*/
void ProjectNewDiagramConfigPage::initLayout() {
	// put border properties above title block properties
	QVBoxLayout *vlayout2 = new QVBoxLayout();
	vlayout2 -> addWidget(border_);
	vlayout2 -> addWidget(titleblock_);
	vlayout2 -> setSpacing(5);
	
	// add conductor properties on the right
	QHBoxLayout *hlayout1 = new QHBoxLayout();
	hlayout1 -> addLayout(vlayout2);
	hlayout1 -> addWidget(conductor_);
	hlayout1 -> setAlignment(conductor_, Qt::AlignTop);
	
	// add the informative label above previous widgets
	QVBoxLayout *vlayout1 = new QVBoxLayout();
	vlayout1 -> addWidget(informative_label_);
	vlayout1 -> addLayout(hlayout1);
	vlayout1 -> addStretch();
	setLayout(vlayout1);
}

/**
	Read properties from the edited project then fill widgets with them.
*/
void ProjectNewDiagramConfigPage::readValuesFromProject() {
	border_ -> setEditedBorder(project_ -> defaultBorderProperties());
	conductor_ -> setConductorProperties(project_ -> defaultConductorProperties());
	titleblock_ -> setTitleBlockProperties(project_ -> defaultTitleBlockProperties());
}

/**
	Set the content of this page read only if the project is read only,
	editable if the project is editable.
 */
void ProjectNewDiagramConfigPage::adjustReadOnly() {
	bool is_read_only = project_ -> isReadOnly();
	border_ -> setReadOnly(is_read_only);
	titleblock_ -> setReadOnly(is_read_only);
	conductor_ -> setReadOnly(is_read_only);
}
