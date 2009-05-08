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
#ifndef QET_ICONS_H
#define QET_ICONS_H
#include <QIcon>
/**
	Ce namespace est reserve a la declaration des icones de l'application
	QElectroTech.
*/
namespace QET {
	namespace Icons {
		void initIcons();
		
		// l'utilisation du mot-cle extern permet l'acces aux objets statiques
		// depuis l'exterieur du namespace
		extern QIcon AddColumn;
		extern QIcon AddRow;
		extern QIcon Allowed;
		extern QIcon ApplicationExit;
		extern QIcon ArrowLeft;
		extern QIcon ArrowLeftDouble;
		extern QIcon ArrowRight;
		extern QIcon ArrowRightDouble;
		extern QIcon BringForward;
		extern QIcon Cancel;
		extern QIcon Conductor;
		extern QIcon ConductorSettings;
		extern QIcon Configure;
		extern QIcon ConfigureToolbars;
		extern QIcon CopyFile;
		extern QIcon DefaultConductor;
		extern QIcon DiagramAdd;
		extern QIcon Diagram;
		extern QIcon DiagramDelete;
		extern QIcon DialogCancel;
		extern QIcon DialogInformation;
		extern QIcon DialogOk;
		extern QIcon DocumentClose;
		extern QIcon DocumentExport;
		extern QIcon DocumentImport;
		extern QIcon DocumentNew;
		extern QIcon DocumentOpen;
		extern QIcon DocumentOpenRecent;
		extern QIcon DocumentPrint;
		extern QIcon DocumentPrintFrame;
		extern QIcon DocumentSave;
		extern QIcon DocumentSaveAll;
		extern QIcon DocumentSaveAs;
		extern QIcon East;
		extern QIcon EditClear;
		extern QIcon EditClearLocationBar;
		extern QIcon EditCopy;
		extern QIcon EditCut;
		extern QIcon EditDelete;
		extern QIcon EditPaste;
		extern QIcon EditRedo;
		extern QIcon EditSelectAll;
		extern QIcon EditUndo;
		extern QIcon ElementDelete;
		extern QIcon ElementEdit;
		extern QIcon ElementNew;
		extern QIcon EndLineCircle;
		extern QIcon EndLineDiamond;
		extern QIcon EndLineNone;
		extern QIcon EndLineSimple;
		extern QIcon EndLineTriangle;
		extern QIcon EPS;
		extern QIcon Folder;
		extern QIcon FolderDelete;
		extern QIcon FolderEdit;
		extern QIcon FolderNew;
		extern QIcon Forbidden;
		extern QIcon FullScreenEnter;
		extern QIcon FullScreenExit;
		extern QIcon Ground;
		extern QIcon Hide;
		extern QIcon Home;
		extern QIcon HotSpot;
		extern QIcon Lower;
		extern QIcon MoveFile;
		extern QIcon Names;
		extern QIcon Neutral;
		extern QIcon NewDiagram;
		extern QIcon NewDiagram;
		extern QIcon NewDiagram;
		extern QIcon North;
		extern QIcon ObjectLocked;
		extern QIcon ObjectRotateRight;
		extern QIcon ObjectUnlocked;
		extern QIcon Orientations;
		extern QIcon PartArc;
		extern QIcon PartCircle;
		extern QIcon PartEllipse;
		extern QIcon PartLine;
		extern QIcon PartPolygon;
		extern QIcon PartRectangle;
		extern QIcon PartSelect;
		extern QIcon PartText;
		extern QIcon PartTextField;
		extern QIcon PDF;
		extern QIcon Phase;
		extern QIcon PrintAllPages;
		extern QIcon Printer;
		extern QIcon PrintLandscape;
		extern QIcon PrintPortrait;
		extern QIcon PrintTwoPages;
		extern QIcon Project;
		extern QIcon QETIcon;
		extern QIcon QETLogo;
		extern QIcon QETLogo;
		extern QIcon QtLogo;
		extern QIcon Raise;
		extern QIcon RemoveColumn;
		extern QIcon RemoveRow;
		extern QIcon Restore;
		extern QIcon SendBackward;
		extern QIcon Settings;
		extern QIcon SinglePage;
		extern QIcon South;
		extern QIcon Start;
		extern QIcon Terminal;
		extern QIcon ViewFitWidth;
		extern QIcon ViewFitWindow;
		extern QIcon ViewMove;
		extern QIcon ViewRefresh;
		extern QIcon West;
		extern QIcon WindowNew;
		extern QIcon ZoomFitBest;
		extern QIcon ZoomIn;
		extern QIcon ZoomOriginal;
		extern QIcon ZoomOut;
	}
}
#endif