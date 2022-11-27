/**********************************************************************
** Copyright (C) 2003 Steel E. V. George.  All rights reserved.
**
** This file is part of QLogico.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://qlogico.sourceforge.net
**
** Contact steel_edward.vazquez@aldea.iberopuebla.net  if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/

#ifndef QLGMAINWINDOW_H
#define QLGMAINWINDOW_H

#include <qmainwindow.h>
#include <qwidget.h>
#include <vector.h>

class QWorkspace;
class QlgOutputWindow;
class QToolBar;
class QToolButton;
class QlgCalculatorDlgImpl;
class QlgProjectManager;
class QlgTrueTableView;
class QComboBox;
class QlgCircuitWindow;
class QlgSignalGeneratorDlgImpl;
class QlgTimeChartWindow;
class QlgFSMDlgImpl;
class QlgAddStateDlgImpl;
class QlgAddTransitionDlgImpl;
class QlgFSMSimulationDlgImpl;
class QlgUpdateTransitionDlgImpl;
class QlgFSMWindow;
class QToolBox;
class QlgFSMOptions;
class QAction;
class QActionGroup;
class QlgFSMView;
class QlgCircuitView;
class QlgCircuitDlgImpl;
class QlgCircuitComponentDlgImpl;
class QProcess;
class QlgOutp;

class QlgMainWindow: public QMainWindow
{
    Q_OBJECT
        
public:        
    QlgMainWindow();
    ~QlgMainWindow(); 
    
    QlgCalculatorDlgImpl *basesDlg;
    
    QAction *actionPointer;
    
public slots:
    QlgCircuitWindow* newCircuitWindow();
    QlgFSMWindow* newFSMWindow(  QlgFSMOptions * );
    void updateNumVariablesTrueTable( QAction * );
    void updateNumOutputsTrueTable( QAction * );
    void buildOriginalCircuit();
    void buildReducedCircuit();
    void updateActions(QWidget*);
    void zoom();
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void fileSaveAll();
    void fileClose();
    void filePrint();
    void fileExit();
    void editUndo();
    void editRedo();
    void editCut();
    void editCopy();
    void editPaste();
    void editDelete();
    void editSelectAll();
    void  updateEditorUndoRedo();
    void editFind();
    void helpContents();
    void helpAbout();
    void projectInsertFile();
    void helpAboutQt();
    
    QlgTimeChartWindow* timeChart(){ return timeChartWindow; };
	    
private slots:
    void addCircuitComponent( QAction* );
    void addFSMComponent( QAction* );
    
private:
    void setupMDI();
    void setupMenuBar();
    void setupProjectManagerWindow();
    void setupOutputWindow();
    void setupCircuitActions();
    void setupFSMActions(); 
    void setupTrueTableWindow();
    void setupSignalGenerator();
    void setupTimeChart();
    void setupComponentsView();
    void resetActions();
    void setupFileActions();
    void setupEditActions();
    void setupProjectActions();
    void setupToolsActions();
    void setupHelpActions();
    
public:    
    QMenuBar *menubar;
    
    QPopupMenu *fileMenu;
    QPopupMenu *editMenu;
    QPopupMenu *projectMenu;
    QPopupMenu *trueTableMenu;
    QPopupMenu *circuitMenu;
    QPopupMenu *fsmMenu;
    QPopupMenu *toolsMenu;
    QPopupMenu *windowMenu;    
    QPopupMenu *helpMenu;
    
    QToolBar *projectToolBar;
    QToolBar *toolBar;
    QToolBar *toolBarHelp;
    QToolBar *fsmToolBar;
    QToolBar *trueTableToolBar;
    QToolBar *circuitToolBar;
    QToolBar *toolBarZoom;    
    
    QAction* fileNewAction;
    QAction* fileOpenAction;
    QAction* fileSaveAction;
    QAction* fileSaveAsAction;
    QAction* filePrintAction;
    QAction* fileExitAction;
    QAction* actionEditUndo;
    QAction* actionEditRedo;
    QAction* actionEditCut;
    QAction* actionEditCopy;
    QAction* actionEditPaste;
    QAction* actionEditDelete;
    QAction* actionEditSelectAll;

    QAction *actionBases;
    
    QAction *actionWindowTile;
    QAction *actionWindowCascade;
    QAction *actionWindowClose;
    QAction *actionWindowCloseAll;
    QAction *actionWindowNext;
    QAction *actionWindowPrevious;
    
    QAction *actionHelpContents;
    QAction *actionHelpAbout;
    QAction *actionHelpAboutQt;
    QAction *actionHelpWhatsThis;
    
    QAction *actionTrueTableFunctions;
    QAction *actionTrueTableOCircuit;
    QAction *actionTrueTableRCircuit;
    QAction *actionGetTrueTable;
    
    QAction *actionCircuitNew;
    QAction *actionCircuitSGenerator;    
    QAction *actionCircuitTChart;
    
    QAction *actionNewFSM;
    QAction *actionNewState;
    QAction *actionNewTrans;
    QAction *actionShowFSMSimulator;
    QAction *actionFSMToJK;
    
    QAction *actionNewAND;
    QAction *actionNewOR;
    QAction *actionNewNOT;
    QAction *actionNewNAND;
    QAction *actionNewNOR;
    QAction *actionNewCustom;
    QAction *actionNewLine;    
	    
    QWorkspace *ws;
    QlgOutputWindow *oWindow;
    QlgProjectManager *pmWindow;
    QlgTrueTableView *trueTableView;        

    QComboBox *comboBoxOutputs;
    QComboBox *comboBoxVariables;
    QComboBox *comboBoxZoom;
    QToolBox* toolBox;
    
    QlgAddStateDlgImpl *addStateDlg;
    QlgAddTransitionDlgImpl *addTransitionDlg;    
    
    QlgFSMDlgImpl *fsmDlg;
    QlgFSMSimulationDlgImpl *fsmSimulationDlg;
    QlgFSMView *fsmView;

    QlgSignalGeneratorDlgImpl *signalGenerator;
    QlgTimeChartWindow *timeChartWindow;
    QlgUpdateTransitionDlgImpl *updateTransitionDlg;
    QlgCircuitView *circuitView;
    QlgCircuitDlgImpl *circuitDlg;
    QlgCircuitComponentDlgImpl *circuitComponentDlg;
    
    QProcess *processHelp;
	    
    //QToolBar *commonComponentsToolBar;
    
    QlgOutp *port;
	    
protected slots:
    void setupWindowActions();
};
#endif

