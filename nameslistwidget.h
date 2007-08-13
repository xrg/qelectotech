#ifndef NAMES_LIST_WIDGET_H
#define NAMES_LIST_WIDGET_H
#include <QtGui>
#include "nameslist.h"
/**
	Cette classe represente une interface permettant de saisir les noms des
	categories et elements.
*/
class NamesListWidget : public QWidget {
	Q_OBJECT
	
	// constructeurs, destructeur
	public:
	NamesListWidget(QWidget * = 0);
	virtual ~NamesListWidget();
	
	private:
	NamesListWidget(const NamesListWidget &);
	
	// attributs
	private:
	QTreeWidget *tree_names;
	QPushButton *button_add_line;
	NamesList hash_names;
	bool read_only;
	
	// methodes
	public:
	bool checkOneName();
	NamesList names();
	void setNames(const NamesList &);
	void setReadOnly(bool);
	bool isReadOnly() const;
	
	private:
	void clean();
	void updateHash();
	
	public slots:
	void addLine();
	void check();
	
	signals:
	void inputChecked();
};
#endif