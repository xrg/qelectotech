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
#ifndef CONFIG_PAGES_H
#define CONFIG_PAGES_H
#include <QtGui>
#include "configpage.h"
class BorderPropertiesWidget;
class ConductorPropertiesWidget;
class TitleBlockPropertiesWidget;
class ExportPropertiesWidget;

/**
	This configuration page enables users to define the properties of new
	diagrams to come.
*/
class NewDiagramPage : public ConfigPage {
	Q_OBJECT
	// constructors, destructor
	public:
	NewDiagramPage(QWidget * = 0);
	virtual ~NewDiagramPage();
	private:
	NewDiagramPage(const NewDiagramPage &);
	
	// methods
	public:
	void applyConf();
	QString title() const;
	QIcon icon() const;
	
	// attributes
	public:
	BorderPropertiesWidget *bpw;     ///< Widget to edit default diagram dimensions
	TitleBlockPropertiesWidget *ipw; ///< Widget to edit default title block properties
	ConductorPropertiesWidget *cpw;  ///< Widget to edit default conductor properties
};

/**
	This configuration page enables users to specify various options,most of
	them applying to the whole application.
*/
class GeneralConfigurationPage : public ConfigPage {
	Q_OBJECT
	// constructors, destructor
	public:
	GeneralConfigurationPage(QWidget * = 0);
	virtual ~GeneralConfigurationPage();
	private:
	GeneralConfigurationPage(const GeneralConfigurationPage &);
	
	// methods
	public:
	void applyConf();
	QString title() const;
	QIcon icon() const;
	
	// attributes
	public:
	QLabel *title_label_;
	QFrame *horiz_line_;
	QGroupBox *appearance_;
	QCheckBox *use_system_colors_;
	QGroupBox *projects_view_mode_;
	QRadioButton *windowed_mode_;
	QRadioButton *tabbed_mode_;
	QLabel *warning_view_mode_;
	QGroupBox *elements_management_;
	QCheckBox *integrate_elements_;
	QCheckBox *highlight_integrated_elements_;
	QLabel    *default_element_infos_label_;
	QTextEdit *default_element_infos_textfield_;
};

/**
	This configuration page enables users to set default export options.
*/
class ExportConfigPage : public ConfigPage {
	Q_OBJECT
	// constructors, destructor
	public:
	ExportConfigPage(QWidget * = 0);
	virtual ~ExportConfigPage();
	private:
	ExportConfigPage(const ExportConfigPage &);
	
	// methods
	public:
	void applyConf();
	QString title() const;
	QIcon icon() const;
	
	// attributes
	public:
	ExportPropertiesWidget *epw;
};

/**
	This configuration page enables users to set default printing options.
*/
class PrintConfigPage : public ConfigPage {
	Q_OBJECT
	// constructors, destructor
	public:
	PrintConfigPage(QWidget * = 0);
	virtual ~PrintConfigPage();
	private:
	PrintConfigPage(const PrintConfigPage &);
	
	// methods
	public:
	void applyConf();
	QString title() const;
	QIcon icon() const;
	
	// attributes
	public:
	ExportPropertiesWidget *epw;
};
#endif
