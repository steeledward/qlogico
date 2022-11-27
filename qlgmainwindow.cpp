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

#include <qaction.h>
#include <qmainwindow.h>
#include <qworkspace.h> 
#include <qwidget.h>
#include <qpixmap.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qvbox.h>
#include <qtooltip.h>
#include <qwhatsthis.h> 
#include <qaccel.h> 
#include <qcombobox.h>
#include <qlayout.h>
#include <qsplitter.h>
#include <qstatusbar.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qtoolbox.h>
#include <qprocess.h>

#include "qlgmainwindow.h"
#include "qlgoutputwindow.h"
#include "qlgcalculatordlgimpl.h"
#include "qlgprojectmanager.h"
#include "qcompletionedit.h"
#include "qlgtruetableview.h"
#include "qlgcircuitwindow.h"
#include "qlgcircuitview.h"
#include "qlgsignalgeneratordlgimpl.h"
#include "qlgtimechartwindow.h"
#include "qlgfsmdlgimpl.h"
#include "qlgaddstatedlgimpl.h"
#include "qlgaddtransitiondlgimpl.h"
#include "qlgupdatetransitiondlgimpl.h"
#include "qlgfsmsimulationdlgimpl.h"
#include "qlgfsmwindow.h"
#include "qlgfsmview.h"
#include "qlgboolfunction.h"
#include "qlgfsmoptions.h"
#include "qlgcircuitdlgimpl.h"
#include "qlgcircuitcomponentdlgimpl.h"
#include "qlgoutp.h"

extern void set_splash_status( const QString &txt );

const QString toolbarHelp = "<p>Toolbars contain a number of buttons to "
"provide quick access to often used functions.%1"
"<br>Click on the toolbar handle to hide the toolbar, "
"or drag and place the toolbar to a different location.</p>";

static const char * whatsthis_image[] = {
    "16 16 3 1",
    "	c None",
    "o	c #000000",
    "a	c #000080",
    "o        aaaaa  ",
    "oo      aaa aaa ",
    "ooo    aaa   aaa",
    "oooo   aa     aa",
    "ooooo  aa     aa",
    "oooooo  a    aaa",
    "ooooooo     aaa ",
    "oooooooo   aaa  ",
    "ooooooooo aaa   ",
    "ooooo     aaa   ",
    "oo ooo          ",
    "o  ooo    aaa   ",
    "    ooo   aaa   ",
    "    ooo         ",
    "     ooo        ",
    "     ooo        "};

/*******************************************************************
 Constructor a Main Window which is a child of 'parent', with the 
 name 'name' and widget flags set to 'f'.
*******************************************************************/

QlgMainWindow::QlgMainWindow()
       : QMainWindow( 0, "qlogico_mainwindow", WType_TopLevel | WDestructiveClose | WGroupLeader )
{ 

#if defined(Q_WS_X11)
    port = new QlgOutp();
#endif       
    
#if !defined(Q_WS_X11)
    port = 0;
#endif           

    windowMenu = 0;
    fsmView = 0;
    circuitView = 0;
    
    set_splash_status(  tr("Iniciando") );      
    
    setCaption("QLogico - " + tr("Simulador de Circuitos Digitales") );
    
    setIcon( QPixmap::fromMimeSource("qlogico") );    
    
    // setup the multiple display interface
    
    set_splash_status(  tr("Creando Area de Trabajo") );    
    
    setupMDI();        
    
    // setup the menu bar
    
    setupMenuBar();
    
    // Setup the components section
    
    set_splash_status(  tr("Creando Componentes") );    
    
    setupComponentsView();
    
    // setup the different actions
    
    setupFileActions();
    //setupEditActions();  is not yet ready
    //setupProjectActions();    is not yet ready
    
    // Setup the  Project Manager Window
    
    set_splash_status(  tr("Creando Administrador de Proyectos") );
	
    //setupProjectManagerWindow();   not yet ready
        
    // Setup the  True Table section
    
    set_splash_status(  tr("Creando Tabla de Verdad") );
    
    setupTrueTableWindow();
    
    // Setup the Output Window View
    
    set_splash_status(  tr("Creando Ventana de Mensajes") );
    
    setupOutputWindow();
    
    // Setup the  circuit actions
    
     set_splash_status(  tr("Creando Sección de Esquematico") );
     
     setupCircuitActions();
         
     // Setup the fsm actions
     
     set_splash_status(  tr("Creando Sección de  M.E.F.") );
	
     setupFSMActions();    
     
     // create another actions
     
     set_splash_status(  tr("Creando Herramientas") );
     
     setupToolsActions();
	  
     set_splash_status(  tr("Creando Acciones de Ventanas") );
     
     setupWindowActions();
          
     set_splash_status(  tr("Creando Ayuda") );
	  
     setupHelpActions();            

     resetActions();    
    
    connect( ws, SIGNAL( windowActivated(QWidget*) ), this, SLOT( updateActions(QWidget*) ) );

    set_splash_status(  tr("Listo") );    

}

/*****************************************************************************
 Destructor of the main window view
*****************************************************************************/

QlgMainWindow::~QlgMainWindow()
{
    // no need to delete child widgets, Qt does it all for us
}

void QlgMainWindow::setupMenuBar()
{
        menubar = menuBar();
}

/*****************************************************************************
 Creates the multiple display interface
*****************************************************************************/

void QlgMainWindow::setupMDI()
{
    QVBox *vbox = new QVBox( this );
    vbox->setFrameStyle( QFrame::StyledPanel | QFrame::Sunken );
    ws = new QWorkspace( vbox );
    ws->setPaletteBackgroundPixmap( QPixmap::fromMimeSource( "bground" ) );
    ws->setScrollBarsEnabled( TRUE );
    setCentralWidget( vbox );   
}

/*****************************************************************************
 Create the Output Window View
*****************************************************************************/

void QlgMainWindow::setupOutputWindow()
{
    QDockWindow *dw = new QDockWindow( QDockWindow::InDock, this );
    dw->setResizeEnabled( TRUE );
    dw->setCloseMode( QDockWindow::Always );
    addDockWindow( dw, Qt::DockLeft );
    oWindow = new QlgOutputWindow( dw );
    dw->setWidget( oWindow );
    dw->setFixedExtentHeight( 160 );
    dw->setFixedExtentWidth( 160 );
    dw->setCaption( tr( "Descripción" ) );
    dw->hide();
}

/*****************************************************************************
 Create the Project Window View
*****************************************************************************/

void QlgMainWindow::setupProjectManagerWindow()
{
    QDockWindow *dw = new QDockWindow( QDockWindow::InDock, this );
    dw->setResizeEnabled( TRUE );
    dw->setCloseMode( QDockWindow::Always );
    QVBox *vbox = new QVBox( dw );
    QCompletionEdit *edit = new QCompletionEdit( vbox );
    QToolTip::add( edit, tr( "Escriba el texto deseado, para buscar (ALT+B)" ) );
    QAccel *a = new QAccel( this );
    a->connectItem( a->insertItem( ALT + Key_B ), edit, SLOT( setFocus() ) );
    pmWindow = new QlgProjectManager( vbox, this );
    addDockWindow( dw, Qt::DockLeft );
    dw->setWidget( vbox );

    dw->setCaption( tr( "Vista del Proyecto" ) );
    QWhatsThis::add( pmWindow, tr("<b><font color=#355e00>Ventana de la Vista del Proyecto</font></b>"
				"<p>La Ventana de la Vista del Proyecto muestra proyecto "
				"actual, incluyendo todos los archivos.</p>"
				"<p>Usar el campo de busqueda para cambiar rapidamente"
				"entre elementos del proyecto.</p>"));
    dw->setFixedExtentHeight( 160 );
    
}

/*****************************************************************************
 Create the Table of States and Transitions Window View
*****************************************************************************/

void QlgMainWindow::setupFSMActions()
{
    // Create the dialog for the New FS Machine
    
    fsmDlg = new QlgFSMDlgImpl( this );
   
    // Create the add state dialog 
   
    addStateDlg = new QlgAddStateDlgImpl( this );
   
    // Create the add transition dialog 
   
    addTransitionDlg = new QlgAddTransitionDlgImpl( this );
        
    // Create the update transition dialog 
   
    updateTransitionDlg = new QlgUpdateTransitionDlgImpl( this );
      
    // Create the FSM Simulation Dialog
    
    fsmSimulationDlg = new QlgFSMSimulationDlgImpl(  this );   
    
    fsmMenu = new QPopupMenu( this, "Finite State Machine" );
    menubar->insertItem( tr( "&MEF" ), fsmMenu );
    
    
    fsmToolBar = new QToolBar( tr("Maquina de Estados Finitos"), 0, this, FALSE, "FSM" );
    fsmToolBar->setCloseMode( QDockWindow::Undocked );
    addToolBar( fsmToolBar, tr( "Maquina de Estados Finitos" ) );
    
    actionNewFSM = new QAction( tr( "Nueva" ), QPixmap::fromMimeSource( "newmachine" ), tr("&Nueva" ), 0, this, 0 );
    actionNewFSM->setAccel( ALT + Key_M );
    actionNewFSM->setStatusTip( tr("Nueva máquina de estados finitos") );
    actionNewFSM->setWhatsThis( tr("Crea un documento donde construir una <b><font color=#355e00>Máquina de Estados Finitos</font></b>") );
    connect( actionNewFSM, SIGNAL( activated() ), fsmDlg, SLOT( show() ) );    
    actionNewFSM->addTo( fsmToolBar );
    actionNewFSM->addTo( fsmMenu );
            
    actionShowFSMSimulator = new QAction( tr( "Simulador" ), QPixmap::fromMimeSource( "machinesimulate" ), tr("Sim&ulator" ), 0, this, 0 );
    actionShowFSMSimulator->setAccel( ALT + Key_U );
    actionShowFSMSimulator->setStatusTip( tr("Simulador MEF") );
    actionShowFSMSimulator->setWhatsThis( tr("Muestra una ventana que permite simular la <b><font color=#355e00>Máquina de Estados Finitos</font></b> visible") );
    connect( actionShowFSMSimulator, SIGNAL ( activated() ), fsmSimulationDlg, SLOT( showDlg() ) );
    actionShowFSMSimulator->addTo( fsmToolBar );
    actionShowFSMSimulator->addTo( fsmMenu );

    actionFSMToJK = new QAction( tr( "JK Flip-Flop" ), QPixmap::fromMimeSource( "getfsmtable" ), tr("&JK Flip-Flop" ), 0, this, 0 );
    actionFSMToJK->setAccel( ALT + Key_J );
    actionFSMToJK->setStatusTip( tr("Convertir a JK Flip-Flop") );
    actionFSMToJK->setWhatsThis( tr("Dibuja el <b><font color=#355e00>Esquematico</font></b> ( diagrama eléctrico )  de la <b><font color=#355e00>Máquina de Estados Finitos</b></font> visible usando flip-flops del tipo JK") );
    connect( actionFSMToJK, SIGNAL ( activated() ), trueTableView, SLOT( buildFSMTable() ) );
    //actionFSMToJK->addTo( fsmToolBar );
    //actionFSMToJK->addTo( fsmMenu );

    connect( fsmDlg, SIGNAL( newFSM( QlgFSMOptions * ) ), this, SLOT( newFSMWindow( QlgFSMOptions * ) ) );
}

/*****************************************************************************
 Create the True Table and Boolean expresions Window View
*****************************************************************************/

void QlgMainWindow::setupTrueTableWindow()
{
    QDockWindow *dw = new QDockWindow( QDockWindow::InDock, this );
    dw->setResizeEnabled( TRUE );
    dw->setCloseMode( QDockWindow::Always );
    QVBox *vbox = new QVBox( dw );
    trueTableView = new QlgTrueTableView( vbox, this );
    addDockWindow( dw, Qt::DockLeft );
    dw->setWidget( vbox );
    dw->setCaption( tr( "Tabla de Verdad - Funciones Booleanas" ) );        
    dw->setFixedExtentHeight( 160 );
    dw->show();
 
    trueTableMenu = new QPopupMenu( this, "True Table" );
    menubar->insertItem( tr( "Tabla de &Verdad" ), trueTableMenu );
    
    trueTableToolBar  = new QToolBar( tr("Tabla de Verdad"), 0, this, FALSE, "TrueTable" );
    trueTableToolBar->setCloseMode( QDockWindow::Undocked );   
    addToolBar( trueTableToolBar, tr( "Tabla de Verdad" ) );
    
    QActionGroup *ag;
    ag = new QActionGroup( this, 0 );
    ag->setText( tr( "Variables" ) );
    ag->setMenuText( tr( "Variables" ) );
    ag->setExclusive( TRUE );
    ag->setUsesDropDown( TRUE );
    connect( ag, SIGNAL( selected( QAction * ) ), this, SLOT( updateNumVariablesTrueTable( QAction * ) ) );

    QAction *a;
    for( int i = 2; i < 11; i++ ) {
	a = new QAction( QString::number(i) + " " + tr( "Variables" ), QString::number(i) + " " + tr( "Variables" ), 0, ag, 0, TRUE );
	a->setToolTip( tr("Número de variables de la tabla de verdad"));
	a->setStatusTip( tr("Número de variables de la tabla de verdad"));
	a->setWhatsThis( tr("Muestra el número de variables de entrada disponibles en la <b><font color=#355e00>Tabla de Verdad</font></b> ( numero de variables independientes para construir una función booleana )" ));
    }
    
    ag->addTo( trueTableMenu );
    ag->addTo( trueTableToolBar );
    
    ag = new QActionGroup( this, 0 );
    ag->setText( tr( "Salidas" ) );
    ag->setMenuText( tr( "Salidas" ) );
    ag->setExclusive( TRUE );
    ag->setUsesDropDown( TRUE );
    connect( ag, SIGNAL( selected( QAction * ) ), this, SLOT( updateNumOutputsTrueTable( QAction * ) ) );
    
    for( int i = 1; i < 11; i++ ) {
	a = new QAction( QString::number(i) + " " + tr( "Salidas" ), QString::number(i) + " " + tr( "Salidas" ), 0, ag, 0, TRUE );
	a->setToolTip( tr("Número de salidas de la tabla de verdad"));
	a->setStatusTip( tr("Número de salidas de la tabla de verdad"));
	a->setWhatsThis( tr("Muestra el número de salidas disponibles en la <b><font color=#355e00>Tabla de verdad</font></b> ( número de funciones booleanas a construir )"  ));
    }
    
    ag->addTo( trueTableMenu );
    ag->addTo( trueTableToolBar );
    
    trueTableToolBar->addSeparator();
    trueTableMenu->insertSeparator();
    
    actionTrueTableFunctions = new QAction( tr( "Funciones Booleanas" ), QPixmap::fromMimeSource( "functions" ), tr("Funciones &Booleanas" ), 0, this, 0 );
    actionTrueTableFunctions->setAccel( ALT + Key_F );
    actionTrueTableFunctions->setStatusTip( tr("Obtener funciones booleanas") );
    actionTrueTableFunctions->setWhatsThis( tr("Obtiene las <b><font color=#355e00>Funciones Originales</font></b> y <b><font color=#355e00>Funciones Reducidas</font></b> a partir de los minterminos seleccionados en la <b><font color=#355e00>Tabla de verdad</font></b>") );
    connect( actionTrueTableFunctions, SIGNAL( activated() ), trueTableView,  SLOT( buildFunctions() ) );
    actionTrueTableFunctions->addTo( trueTableMenu );
    actionTrueTableFunctions->addTo( trueTableToolBar );
    
    actionTrueTableOCircuit = new QAction( tr( "Esquematico original" ), QPixmap::fromMimeSource( "ocircuit" ), tr( "Esquematico &Original" ), 0, this, 0 );
    actionTrueTableOCircuit->setAccel( ALT + Key_O );
    actionTrueTableOCircuit->setStatusTip( tr("Dibujar esquematico original") );
    actionTrueTableOCircuit->setWhatsThis( tr("Dibuja el esquematico ( diagrama eléctrico ) para cada una de las <b><font color=#355e00>Funciones Originales</b></font> que aparecen en la <b><font color=#355e00>Tabla de Funciones Booleanas</font></b>") );
    connect( actionTrueTableOCircuit, SIGNAL( activated() ), this,  SLOT( buildOriginalCircuit() ) );
    actionTrueTableOCircuit->addTo( trueTableMenu );
    actionTrueTableOCircuit->addTo( trueTableToolBar );
    
    actionTrueTableRCircuit = new QAction( tr( "Esquematico reducido" ), QPixmap::fromMimeSource( "rcircuit" ), tr( "Esquematico &Reducido" ), 0, this, 0 );
    actionTrueTableRCircuit->setAccel( ALT + Key_R );
    actionTrueTableRCircuit->setStatusTip( tr("Dibujar esquematico reducido") );
    actionTrueTableRCircuit->setWhatsThis( tr("Dibuja el esquematico ( diagrama eléctrico ) para cada una de las <b><font color=#355e00>Funciones Reducidas</b></font> que aparecen en la <b><font color=#355e00>Tabla de Funciones Booleanas</font></b>") );
    connect( actionTrueTableRCircuit, SIGNAL( activated() ), this,  SLOT( buildReducedCircuit() ) );
    actionTrueTableRCircuit->addTo( trueTableMenu );
    actionTrueTableRCircuit->addTo( trueTableToolBar );
    
    actionGetTrueTable = new QAction( tr( "Tabla de Verdad" ), QPixmap::fromMimeSource( "table" ), tr( "Table de &Verdad" ), 0, this, 0 );
    actionGetTrueTable->setAccel( ALT + Key_T );
    actionGetTrueTable->setStatusTip( tr("Obtener tabla de Verdad") );
    actionGetTrueTable->setWhatsThis( tr("Obtiene la <b><font color=#355e00>Tabla de verdad</b></font> para el conjunto de <b><font color=#355e00>Funciones Originales</font></b> que aparecen en la <b><font color=#355e00>Tabla de Funciones Booleanas</font></b>") );
    connect( actionGetTrueTable, SIGNAL( activated() ), trueTableView,  SLOT( buildTTable() ) );
    actionGetTrueTable->addTo( trueTableMenu );
    actionGetTrueTable->addTo( trueTableToolBar );
        
    trueTableToolBar->addSeparator();
    trueTableMenu->insertSeparator();

}

void QlgMainWindow::setupCircuitActions()
{
    circuitDlg = new QlgCircuitDlgImpl( this );
    
    circuitComponentDlg = new QlgCircuitComponentDlgImpl( this );
    
    signalGenerator = new QlgSignalGeneratorDlgImpl(  this );
       
    timeChartWindow = new QlgTimeChartWindow(  this, "Time Chart", Qt::WType_Dialog | Qt::WStyle_Customize | Qt::WStyle_NormalBorder );
       
    circuitMenu = new QPopupMenu( this, "Circuit" );
    menubar->insertItem( tr( "E&squematico" ), circuitMenu );
    
    circuitToolBar  = new QToolBar( tr("Esquemático"), 0, this, FALSE, "Circuit" );
    circuitToolBar->setCloseMode( QDockWindow::Undocked );
    addToolBar( circuitToolBar, tr( "Esquemático" ) );
    
    actionCircuitNew = new QAction( tr( "Nuevo" ), QPixmap::fromMimeSource( "newcircuit" ), tr( "&Nuevo" ), 0, this, 0 );
    actionCircuitNew->setAccel( ALT + Key_S );
    actionCircuitNew->setStatusTip( tr("Nuevo esquemático") );
    actionCircuitNew->setWhatsThis( tr("Crea una nuevo documento en donde construir un <b><font color=#355e00>Esquemático</font></b> de manera libre") );
    connect( actionCircuitNew, SIGNAL( activated() ), this,  SLOT( newCircuitWindow() ) );
    actionCircuitNew->addTo( circuitMenu );
    actionCircuitNew->addTo( circuitToolBar );
    
    actionCircuitSGenerator = new QAction( tr( "Generador de pulsos binarios" ), QPixmap::fromMimeSource( "signal" ), tr( "&Generador de pulsos binarios" ), 0, this, 0 );
    actionCircuitSGenerator->setAccel( ALT + Key_G );
    actionCircuitSGenerator->setStatusTip( tr("Muestra un generador de pulsos binarios") );
    actionCircuitSGenerator->setWhatsThis( tr("Muestra un <b><font color=#355e00>Generador de pulsos binarios</font></b> con la finalidad de simular el <b><font color=#355e00>Esquemático</font></b> visible") );
    connect( actionCircuitSGenerator, SIGNAL( activated() ), signalGenerator,  SLOT( showDlg() ) );
    actionCircuitSGenerator->addTo( circuitMenu );
    actionCircuitSGenerator->addTo( circuitToolBar );
    
    actionCircuitTChart = new QAction( tr( "Diagrama de tiempos" ), QPixmap::fromMimeSource( "time" ), tr( "&Diagrama de tiempos" ), 0, this, 0 );
    actionCircuitTChart->setAccel( ALT + Key_C );
    actionCircuitTChart->setStatusTip( tr("Muestra un diagrama de tiempos") );
    actionCircuitTChart->setWhatsThis( tr("Muestra un <b><font color=#355e00>Diagrama de tiempos</font></b> con la finalidad de simular el <b><font color=#355e00>Esquemático</font></b> visible") );
    connect( actionCircuitTChart, SIGNAL( activated() ), timeChartWindow,  SLOT( showDlg() ) );
    actionCircuitTChart->addTo( circuitMenu );
    actionCircuitTChart->addTo( circuitToolBar );    
    
    connect( (QObject*)(signalGenerator->spinBoxValue),  SIGNAL( valueChanged(int) ), timeChartWindow, SLOT( nextStep(int) ) );
}

/******************************************************************
 Update the number of variables for theTrue Table
*******************************************************************/  

void QlgMainWindow::updateNumVariablesTrueTable( QAction *act)
{
    QString value = act->text();
    trueTableView->setNumVariables( value.remove( " Variables" ).toInt() );   
}

/******************************************************************
 Update the number of outputs for theTrue Table
*******************************************************************/  

void QlgMainWindow::updateNumOutputsTrueTable( QAction * act )
{
    QString value = act->text();
    trueTableView->setNumOutputs( value.remove( QObject::tr( " Salidas" ) ).toInt() );
}

/******************************************************************
 Create a new Circuit Window and return it
*******************************************************************/  

QlgCircuitWindow* QlgMainWindow::newCircuitWindow()
{
    QlgCircuitWindow* w = new QlgCircuitWindow( ws, this, tr("Sin nombre") + QString::number( ws->windowList().count() ) );
    w->setIcon( QPixmap::fromMimeSource( "circuit" ) );
    connect( (QObject*)(w->view()), SIGNAL( editCircuitComponent( QlgCircuitComponent * ) ), circuitComponentDlg, SLOT( showDlg( QlgCircuitComponent * ) ) );
    connect( (QObject*)(w->view()), SIGNAL( editView( QlgCircuitView * ) ), circuitDlg, SLOT( showDlg( QlgCircuitView * ) ) );
    connect( (QObject*)(w->view()), SIGNAL( showDescription( QString ) ), oWindow, SLOT( showDescription( QString ) ) );
    statusBar()->message( tr( "Listo" ), 2000 );
    return w;
}

/**********************************************************
  Build the circuit of the original equations
**********************************************************/  

void QlgMainWindow::buildOriginalCircuit()
{
    newCircuitWindow()->view()->addCircuit( trueTableView->originalFunctions() );
}

/**********************************************************
  Build the circuit of the reduced equations
**********************************************************/  

void QlgMainWindow::buildReducedCircuit()
{
    newCircuitWindow()->view()->addCircuit( trueTableView->reducedFunctions() );
}

/**********************************************************
 Create the area where draw the time chart.  
************************************************************/

void QlgMainWindow::setupTimeChart()
{       

}

/******************************************************************
 Create a new FSM Window and return it
*******************************************************************/  

QlgFSMWindow* QlgMainWindow::newFSMWindow( QlgFSMOptions *values )
{
    QlgFSMWindow* w = new QlgFSMWindow( ws, this, values );    
    w->setIcon( QPixmap::fromMimeSource( "machine" ) );
    w->setCaption( tr("Sin nombre") + QString::number( ws->windowList().count() ) );
    w->show();
    connect( (QObject*)(w->sView), SIGNAL( editState( QlgState * ) ), addStateDlg, SLOT( showDlg( QlgState * ) ) );
    connect( (QObject*)(w->sView), SIGNAL( editTransition( QlgTransition * ) ), updateTransitionDlg, SLOT( showDlg( QlgTransition * ) ) );
    connect( (QObject*)(w->sView), SIGNAL( editFSM( QlgFSMView * ) ), fsmDlg, SLOT( showDlg( QlgFSMView * ) ) );
    connect( (QObject*)(w->sView), SIGNAL( showDescription( QString ) ), oWindow, SLOT( showDescription( QString ) ) );    
    //pmWindow->addFile( ws->activeWindow() );
    statusBar()->message( tr("Listo"), 2000 );
    return w;
}

/******************************************************************
 Update the tool bar in function of the selected window
 and then send the correct information to each component
*******************************************************************/  

void QlgMainWindow::updateActions(QWidget* sV)
{
    if( !sV ) {
	resetActions();
	return;
    }

    if( QString(sV->name()) == "circuit" ) {
	circuitView = ((QlgCircuitWindow*)(sV))->view();
	comboBoxZoom->setEditText( QString::number( circuitView->zoom() * 100 ) + " % " );
	signalGenerator->update( circuitView );
	trueTableView->updateCircuitView( circuitView );
	timeChartWindow->update( circuitView );
	addStateDlg->update( 0 );
	addTransitionDlg->update( 0 );
	fsmSimulationDlg->update( 0 );
	trueTableView->updateFSMView( 0 );
    }
    else if(  QString(sV->name()) == "fsm"  ) {
	fsmView = ((QlgFSMWindow*)(sV))->sView;
	comboBoxZoom->setEditText( QString::number( fsmView->zoom() * 100 ) + " % " );	
	addStateDlg->update( fsmView );
	addTransitionDlg->update( fsmView );
	fsmSimulationDlg->update( fsmView );
	trueTableView->updateFSMView( fsmView );
	signalGenerator->update( 0 );	
	trueTableView->updateCircuitView( 0 );
    }
    
}

/******************************************************************
 Change the zoom for the active window
*******************************************************************/  

void QlgMainWindow::zoom()
{
    if( ws->activeWindow() ) {
	double nzoom = ( comboBoxZoom->currentText().remove( " %" ).toInt() ) * 0.01;
	if( QString(ws->activeWindow()->name()) == "fsm" ) {
	    ((QlgFSMWindow*)(ws->activeWindow()))->sView->setZoom( nzoom );
	} else if( QString(ws->activeWindow()->name()) == "circuit" )
	    ((QlgCircuitWindow*)(ws->activeWindow()))->view()->setZoom( nzoom );
    }
}

/***************************************************************
  Create the common components availables for the use in the views
***************************************************************/

void QlgMainWindow::setupComponentsView()
{
    QDockWindow *dw = new QDockWindow( QDockWindow::InDock, this );
    dw->setResizeEnabled( TRUE );
    dw->setCloseMode( QDockWindow::Always );
    addToolBar( dw, Qt::DockRight );
    toolBox = new QToolBox( dw );
    addDockWindow( dw, Qt::DockRight );
    dw->setWidget( toolBox );
    dw->setFixedExtentWidth( 160 );
    dw->setCaption( tr( "Componentes" ) );
    dw->show();
    setDockEnabled( dw, Qt::DockTop, FALSE );
    setDockEnabled( dw, Qt::DockBottom, FALSE );    
    
    QWhatsThis::add( toolBox, tr("<b><font color=#355e00>Componentes</font></b>"
			      "<p>La sección de <b><font color=#355e00>Componentes</font></b> le muestra "
			      "los elementos disponibles para construir ya sea un <b><font color=#355e00>Esquemático</font></b> o una <b><font color=#355e00>Maquina de Estados Finitos</font></b>.</p>" ) );
}

/**************************************************************
  Reset the enabled buttons for the toolbars
**************************************************************/

void QlgMainWindow::resetActions()
{
    fsmView = 0;
    circuitView = 0;
    signalGenerator->update( 0 );
    trueTableView->updateCircuitView( 0 );
    addStateDlg->update( 0 );
    addTransitionDlg->update( 0 );
    fsmSimulationDlg->update( 0 );
    trueTableView->updateFSMView( 0 );
    oWindow->showDescription( "" );
}

void QlgMainWindow::setupFileActions()
{
    /*
    QToolBar* tb  = new QToolBar( this, "File" );
    tb->setCloseMode( QDockWindow::Undocked );    
    QWhatsThis::add( tb, tr( "<b>La barra de herramientas de archivo</b>%1").arg(tr(toolbarHelp).arg("")) );
    
    addToolBar( tb, tr( "Archivo" ) );
    */

    fileMenu = new QPopupMenu( this, "File" );
    menubar->insertItem( tr( "&Archivo" ), fileMenu );

    QAction *a = 0;
    
    /*
    a = new QAction( this, 0 );
    a->setText( tr( "Nuevo" ) );
    a->setToolTip( tr( "Nuevo vista" ) );
    a->setMenuText( tr( "&Nuevo..." ) );
    a->setIconSet( QPixmap::fromMimeSource("filenew") );
    a->setAccel( CTRL + Key_N );
    a->setStatusTip( tr( "Crear un nuevo proyecto." ) );
    a->setWhatsThis( "Crear un nuevo proyecto." );
    connect( a, SIGNAL( activated() ), this, SLOT( fileNew() ) );
    a->addTo( tb );
    a->addTo( fileMenu );

    a = new QAction( this, 0 );
    a->setText( tr( "Abrir" ) );
    a->setMenuText( tr( "&Abrir..." ) );
    a->setIconSet( QPixmap::fromMimeSource("fileopen") );
    a->setAccel( CTRL + Key_O );
    a->setStatusTip( tr( "Abrir proyecto existente") );
    a->setWhatsThis( "Abrir proyecto existente" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileOpen() ) );    
    a->addTo( tb );
    a->addTo( fileMenu );
    
    a = new QAction( this, 0 );
    a->setText( tr( "Cerrar" ) );
    a->setMenuText( tr( "&Cerrar" ) );
    a->setStatusTip( tr( "Cierra el proyecto actual" ) );
    a->setWhatsThis( "Cierra el proyecto actual" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileClose() ) );
    a->addTo( fileMenu );
    
    a = new QAction( this, 0 );    
    a->setText( tr( "Guardar" ) );
    a->setMenuText( tr( "&Guardar" ) );
    a->setIconSet( QPixmap::fromMimeSource("filesave") );
    a->setAccel( CTRL + Key_S );
    a->setStatusTip( tr( "Guarda el proyecto o documento actual" ) );
    a->setWhatsThis( "Guarda el proyecto o documento actual" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSave() ) );
    a->addTo( tb );
    a->addTo( fileMenu );

    a = new QAction( this, 0 );
    a->setText( tr( "Guardar Como" ) );
    a->setMenuText( tr( "Guardar &Como..." ) );        
    a->setStatusTip( tr( "Guarda como un archivo nuevo" ) );
    a->setWhatsThis( "Guarda como un archivo nuevo" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAs() ) );
    a->addTo( fileMenu );
    
    a = new QAction( this, 0 );
    a->setText( tr( "Guradar Todo" ) );
    a->setMenuText( tr( "Gu&ardar Todo" ) );
    a->setStatusTip( tr( "Guarda todos los documentos activos" ) );
    a->setWhatsThis( "Guarda todos los documentos activos" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileSaveAll() ) );
    a->addTo( fileMenu );
    */
    
    a = new QAction( this, 0 );
    a->setText( tr( "Salir" ) );
    a->setMenuText( tr( "S&alir" ) );
    a->setStatusTip( tr( "Quita la aplicación" ) );
    a->setWhatsThis( "Quita la aplicación" );
    connect( a, SIGNAL( activated() ), this, SLOT( fileExit() ) );
    a->addTo( fileMenu );
}

void QlgMainWindow::setupEditActions()
{
    actionEditUndo = new QAction( tr("Deshacer"), QPixmap::fromMimeSource( "undo" ),tr("&Deshacer: No disponible"), CTRL + Key_Z, this, 0 );
    actionEditUndo->setStatusTip( tr( "Deshace la ultima a acción" ) );
    actionEditUndo->setWhatsThis( "Deshace la ultima a acción" );
    connect( actionEditUndo, SIGNAL( activated() ), this, SLOT( editUndo() ) );
    actionEditUndo->setEnabled( FALSE );

    actionEditRedo = new QAction( tr( "Rehacer" ), QPixmap::fromMimeSource("redo"), tr( "&Rehacer: No disponible" ), CTRL + Key_Y, this, 0 );
    actionEditRedo->setStatusTip( tr( "Rehace la ultima operación") );
    actionEditRedo->setWhatsThis( "Rehace la ultima operación" );
    connect( actionEditRedo, SIGNAL( activated() ), this, SLOT( editRedo() ) );
    actionEditRedo->setEnabled( FALSE );

    actionEditCut = new QAction( tr( "Cortar" ), QPixmap::fromMimeSource("editcut"), tr( "Co&rtar" ), CTRL + Key_X, this, 0 );
    actionEditCut->setStatusTip( tr( "Corta el elemento selecionado" ) );
    actionEditCut->setWhatsThis( "Corta el elemento selecionado" );
    connect( actionEditCut, SIGNAL( activated() ), this, SLOT( editCut() ) );
    actionEditCut->setEnabled( FALSE );

    actionEditCopy = new QAction( tr( "Copiar" ), QPixmap::fromMimeSource("editcopy"), tr( "&Copiar" ), CTRL + Key_C, this, 0 );
    actionEditCopy->setStatusTip( tr( "Copia el elemento seleccionado" ) );
    actionEditCopy->setWhatsThis( "Copia el elemento seleccionado" );
    connect( actionEditCopy, SIGNAL( activated() ), this, SLOT( editCopy() ) );
    actionEditCopy->setEnabled( FALSE );

    actionEditPaste = new QAction( tr( "Pegar" ), QPixmap::fromMimeSource("editpaste"), tr( "&Pegar" ), CTRL + Key_V, this, 0 );
    actionEditPaste->setStatusTip( tr( "Pega el contenido antes selecionado" ) );
    actionEditPaste->setWhatsThis( "Pega el contenido antes selecionado" );
    connect( actionEditPaste, SIGNAL( activated() ), this, SLOT( editPaste() ) );
    actionEditPaste->setEnabled( FALSE );

    actionEditDelete = new QAction( tr( "Eliminar" ), QPixmap(), tr( "&Eliminar" ), Key_Delete, this, 0 );
    actionEditDelete->setStatusTip( tr( "Elimina el componente seleccionado" ) );
    actionEditDelete->setWhatsThis( "Elimina el componente seleccionado" );
    connect( actionEditDelete, SIGNAL( activated() ), this, SLOT( editDelete() ) );
    actionEditDelete->setEnabled( FALSE );
#ifdef Q_WS_MAC
    QAction *macDelete = new QAction( tr( "Eliminar" ), QPixmap(), tr( "&Eliminar" ), Key_Backspace, this, 0 );
    connect( macDelete, SIGNAL( activated() ), this, SLOT( editDelete() ) );
#endif

    actionEditSelectAll = new QAction( tr( "Seleccionar todo" ), QPixmap(), tr( "Seleccionar &todo" ), CTRL + Key_A, this, 0 );
    actionEditSelectAll->setStatusTip( tr( "Seleciona todos los componentes" ) );
    actionEditSelectAll->setWhatsThis( "Seleciona todos los componentes" );
    connect( actionEditSelectAll, SIGNAL( activated() ), this, SLOT( editSelectAll() ) );
    actionEditSelectAll->setEnabled( TRUE );

    QToolBar *tb = new QToolBar( tr("Edición"), 0, this, FALSE, "Edit" );
    tb->setCloseMode( QDockWindow::Undocked );
    QWhatsThis::add( tb, tr( "<b>La barra de herramientas de edición</b>%1").arg(tr(toolbarHelp).arg("")) );
    addToolBar( tb, tr( "Editar" ) );
    actionEditUndo->addTo( tb );
    actionEditRedo->addTo( tb );
    tb->addSeparator();
    actionEditCut->addTo( tb );
    actionEditCopy->addTo( tb );
    actionEditPaste->addTo( tb );

    QPopupMenu *menu = new QPopupMenu( this, "Editar" );
    connect( menu, SIGNAL( aboutToShow() ), this, SLOT( updateEditorUndoRedo() ) );
    menubar->insertItem( tr( "&Editar" ), menu );
    actionEditUndo->addTo( menu );
    actionEditRedo->addTo( menu );
    menu->insertSeparator();
    actionEditCut->addTo( menu );
    actionEditCopy->addTo( menu );
    actionEditPaste->addTo( menu );
    actionEditDelete->addTo( menu );
    actionEditSelectAll->addTo( menu );
    
}
    
void QlgMainWindow::setupProjectActions()
{
    projectMenu = new QPopupMenu( this, "Project" );
    menubar->insertItem( tr( "Pr&oyecto" ), projectMenu );

    QActionGroup *ag = new QActionGroup( this, 0 );
    ag->setText( tr( "Proyecto Activo" ) );
    ag->setMenuText( tr( "Proyecto Activo" ) );
    ag->setExclusive( TRUE );
    ag->setUsesDropDown( TRUE );
    
    QAction *a = new QAction( tr( "<Sin Proyecto>" ), tr( "<Sin Proyecto>" ), 0, ag, 0, TRUE );
    a->setOn( TRUE );
    ag->addTo( projectMenu );

    a = new QAction( tr( "Añadir archivo" ), QPixmap(), tr( "&Añadir Archivo..." ), 0, this, 0 );
    a->setStatusTip( tr("Añadir un archivo al actual proyecto") );
    a->setWhatsThis( "Añadir un archivo al actual proyecto" );
    connect( a, SIGNAL( activated() ), this, SLOT( projectInsertFile() ) );
    a->setEnabled( FALSE );

    a->addTo( projectMenu );
    
}
 
void QlgMainWindow::setupToolsActions()
{
    toolBarZoom = new QToolBar( tr("Acercamiento"), 0, this, FALSE, "Zoom" );
    toolBarZoom->setLabel( tr( "Acercamiento" ) );
    addToolBar( toolBarZoom, tr( "Acercamiento" ) );    
    
    comboBoxZoom = new QComboBox( TRUE, toolBarZoom, "cbZoom" );
    QToolTip::add( comboBoxZoom, tr("Acercamiento"));
    QWhatsThis::add( comboBoxZoom, tr("Muestra el porcentaje de acercamiento del documento actual, si desea cambiarlo solo escriba un nuevo valor o seleccione uno disponible"  ));
    
    QStringList zoom;
    zoom << "100 %" << "50 %"    << "25 %";
    comboBoxZoom->insertStringList( zoom );    
    connect( comboBoxZoom, SIGNAL( activated(int) ), this, SLOT( zoom() ) );

    basesDlg =  new QlgCalculatorDlgImpl; 
    
    actionBases = new QAction( tr( "Calculadora" ), QPixmap::fromMimeSource( "bases" ), tr( "&Calculadora" ), ALT + Key_K, this, 0 );
    actionBases->setStatusTip( tr( "Calculadora de base de números" ) );
    actionBases->setWhatsThis( "Muestra una <b><font color=#355e00>Calculadora</font></b> con la cual se pueden realizar distintas operaciones en diferentes bases de números" );
    connect( actionBases, SIGNAL( activated() ), basesDlg, SLOT( show() ) );    
     
    QToolBar *tb = new QToolBar( tr("Calculadora"), 0, this, FALSE, "Calculator" );
    tb->setCloseMode( QDockWindow::Undocked );
    
    addToolBar( tb, tr( "Calculadora" ) );
    
    toolsMenu = new QPopupMenu( this, "Tools" );
    menubar->insertItem( tr( "H&erramientas" ), toolsMenu );
    
    actionBases->addTo( tb );
    actionBases->addTo( toolsMenu );
    
    toolsMenu->insertSeparator();
    
    QToolBar *tb1 = new QToolBar( tr("Esquematico (componentes)"), 0, toolBox, FALSE, "Circuit" );
    tb1->setCloseMode( QDockWindow::Undocked );
    
    QToolBar *tb2 = new QToolBar( tr("Esquematico (componentes)"), 0, toolBox, FALSE, "Circuit" );
    tb2->setFrameStyle( QFrame::NoFrame );
    tb2->setOrientation( Qt::Vertical );
    tb2->setBackgroundMode( PaletteBase );
    toolBox->addItem( tb2, tr("Esquematico") );
    
    QActionGroup *agC;
    agC = new QActionGroup( this, 0 );
    agC->setText( tr( "Componentes" ) );
    agC->setMenuText( tr( "Componentes" ) );
    agC->setExclusive( TRUE );
    agC->setUsesDropDown( FALSE );
    connect( agC, SIGNAL( selected( QAction * ) ), this, SLOT( addCircuitComponent( QAction * ) ) );
    QAction *actionNewComponent;
	
    actionPointer = new QAction( tr("Apuntador"), QPixmap::fromMimeSource("pointer"),
				     tr("&Apuntador"),  Key_F2,
				     agC,
				     tr("Apuntador"), TRUE );
    actionPointer->setStatusTip( tr("Apuntador") );
    actionPointer->setWhatsThis( tr("Selecciona o deselecciona un componente de la sección de <b><font color=#355e00>Componentes</font></b>") );
    
    QToolBar *tb3 = new QToolBar( tr("Apuntador"), 0, toolBox, FALSE, "Apuntador" );
    tb3->setCloseMode( QDockWindow::Undocked );
    
    actionPointer->addTo( tb3 );
    addToolBar( tb3, tr("Apuntador") );
    
    QStringList vComponents;            
    vComponents << "and" << "or" <<  "not" << "nand" << "nor" << "xor" << "xnor" << "entrada" << "salida" << "personalizable" << "linea" << "jkff";
        
    for ( QStringList::Iterator it = vComponents.begin(); it != vComponents.end(); ++it ) {
	actionNewComponent = new QAction( *it, QPixmap::fromMimeSource( *it ), *it, 0, agC, 0 );
	actionNewComponent->setToggleAction( TRUE );
	actionNewComponent->setToolTip( tr("Añade un componente tipo: ") + *it );
	actionNewComponent->setStatusTip( tr("Añade un componente tipo: ") + *it );
	actionNewComponent->setWhatsThis( tr("Añade un componente tipo: ") + *it );
	actionNewComponent->addTo( tb1 );
	actionNewComponent->addTo( tb2 );
    }    
    
    QWidget *w;
    tb2->setStretchableWidget( ( w = new QWidget( tb2 ) ) );
    w->setBackgroundMode( tb2->backgroundMode() );
	    
    addToolBar( tb1, tr("Esquematico (componentes)") );
    tb1->hide();
    
    tb1 = new QToolBar( tr("Maquina de Estados Finitos (componentes)"), 0, toolBox, FALSE, "FSM" );
    tb1->setCloseMode( QDockWindow::Undocked );
    
    tb2 = new QToolBar( tr("Maquina de Estados Finitos (componentes)"), 0, toolBox, FALSE, "FSM" );
    tb2->setFrameStyle( QFrame::NoFrame );
    tb2->setOrientation( Qt::Vertical );
    tb2->setBackgroundMode( PaletteBase );
    toolBox->addItem( tb2, "Maquina Estados finitos" );

    actionNewState = new QAction ( QPixmap::fromMimeSource( "state" ), tr("Estado"), 0, this, "State" );
    actionNewState->setText( tr("Estado") );
    actionNewState->setToolTip( tr("Añade un estado finito") );
    actionNewState->setStatusTip( tr("Añade un estado finito") );
    actionNewState->setWhatsThis( tr("Añade un estado finito") );
    actionNewState->addTo( tb1 );
    actionNewState->addTo( tb2 );
    connect( actionNewState, SIGNAL( activated() ), addStateDlg, SLOT( showDlg() ) );
    
    actionNewTrans = new QAction ( QPixmap::fromMimeSource( "transition" ), tr("Transición"), 0, this, "Transition" );
    actionNewTrans->setText( tr("Transición") );
    actionNewTrans->setToolTip( tr("Añade una transición") );
    actionNewTrans->setStatusTip( tr("Añade una transición") );
    actionNewTrans->setWhatsThis( tr("Añade una transición") );
    connect( actionNewTrans, SIGNAL( activated() ), addTransitionDlg, SLOT( showDlg() ) );
    actionNewTrans->addTo( tb1 );
    actionNewTrans->addTo( tb2 );
    
    tb2->setStretchableWidget( ( w = new QWidget( tb2 ) ) );
    w->setBackgroundMode( tb2->backgroundMode() );
	    
    addToolBar( tb1, tr("Maquina de Estados Finitos (componentes)") );
    tb1->hide();
    
}

void QlgMainWindow::setupWindowActions()
{
    static bool windowActionsSetup = FALSE;
    if ( !windowActionsSetup ) {
	windowActionsSetup = TRUE;

	actionWindowTile = new QAction( tr( "Reja" ), tr( "&Reja" ), 0, this );
	actionWindowTile->setStatusTip( tr("Enreja las ventanas de manera que puedan todas ser visibles") );
	actionWindowTile->setWhatsThis( ( "Enreja las ventanas de manera que puedan todas ser visibles" ) );
	connect( actionWindowTile, SIGNAL( activated() ), ws, SLOT( tile() ) );
	actionWindowCascade = new QAction( tr( "Cascada" ), tr( "&Cascada" ), 0, this );
	actionWindowCascade->setStatusTip( tr("Pone en cascada todas las ventanas de manera que sus barras titulos sean visibles") );
	actionWindowCascade->setWhatsThis( ( "Pone en cascada todas las ventanas de manera que sus barras titulos sean visibles" ) );
	connect( actionWindowCascade, SIGNAL( activated() ), ws, SLOT( cascade() ) );

	actionWindowClose = new QAction( tr( "Cerrar" ), tr( "Ce&rrar" ), CTRL + Key_F4, this );
	actionWindowClose->setStatusTip( tr( "Cierra la ventana activa") );
	actionWindowClose->setWhatsThis( ( "Cierra la ventana activa" ) );
	connect( actionWindowClose, SIGNAL( activated() ), ws, SLOT( closeActiveWindow() ) );

	actionWindowCloseAll = new QAction( tr( "Cerrar Todo" ), tr( "Cerrar To&do" ), 0, this );
	actionWindowCloseAll->setStatusTip( tr( "Cierrar todas las ventanas") );
	actionWindowCloseAll->setWhatsThis( ( "Cierrar todas las ventanas" ) );
	connect( actionWindowCloseAll, SIGNAL( activated() ), ws, SLOT( closeAllWindows() ) );

	actionWindowNext = new QAction( tr( "Siguiente" ), tr( "Si&guiente" ), CTRL + Key_F6, this );
	actionWindowNext->setStatusTip( tr( "Activa la siguiente ventana" ) );
	actionWindowNext->setWhatsThis( ( "Activa la siguiente ventana" ) );
	connect( actionWindowNext, SIGNAL( activated() ), ws, SLOT( activateNextWindow() ) );

	actionWindowPrevious = new QAction( tr( "Previa" ), tr( "Pre&via" ), CTRL + SHIFT + Key_F6, this );
	actionWindowPrevious->setStatusTip( tr( "Activa la ventana previa" ) );
	actionWindowPrevious->setWhatsThis( ( "Activa la ventana previa" ) );
	connect( actionWindowPrevious, SIGNAL( activated() ), ws, SLOT( activatePreviousWindow() ) );
    }

    if ( !windowMenu ) {
	windowMenu = new QPopupMenu( this, "Window" );
	menubar->insertItem( tr( "&Ventana" ), windowMenu );
	connect( windowMenu, SIGNAL( aboutToShow() ),
		 this, SLOT( setupWindowActions() ) );
    } else
	windowMenu->clear();

    actionWindowClose->addTo( windowMenu );
    actionWindowCloseAll->addTo( windowMenu );
    windowMenu->insertSeparator();
    actionWindowNext->addTo( windowMenu );
    actionWindowPrevious->addTo( windowMenu );
    windowMenu->insertSeparator();
    actionWindowTile->addTo( windowMenu );
    actionWindowCascade->addTo( windowMenu );
    windowMenu->insertSeparator();
    windowMenu->insertItem( tr( "Vis&ta" ), createDockWindowMenu( NoToolBars ) );
    windowMenu->insertItem( tr( "Barra de &Herramientas" ), createDockWindowMenu( OnlyToolBars ) );    
}
 
void QlgMainWindow::setupHelpActions()
{
    processHelp = new QProcess( this );
    
    actionHelpContents = new QAction( tr( "Ayuda" ), tr( "&Ayuda" ), Key_F1, this, 0 );
    actionHelpContents->setStatusTip( tr("Abre la ayuda en linea") );
    actionHelpContents->setWhatsThis( tr( "Muestra documentación de ayuda para el usuario:<br><b><font color=red>Tutorial de Circuitos Digitales</font></b> y <b><font color=red>Manual de usuario</font></b>" ) );
    connect( actionHelpContents, SIGNAL( activated() ), this, SLOT( helpContents() ) );

    actionHelpAbout = new QAction( tr("Acerca"), QPixmap(), tr("&Acerca"), 0, this, 0 );
    actionHelpAbout->setStatusTip( tr("Muestra información acerca de Qlogico") );
    actionHelpAbout->setWhatsThis( ( "Muestra información acerca de Qlogico" ) );
    connect( actionHelpAbout, SIGNAL( activated() ), this, SLOT( helpAbout() ) );

    actionHelpAboutQt = new QAction( tr("Acerca de Qt"), QPixmap(), tr("Acerca de &Qt"), 0, this, 0 );
    actionHelpAboutQt->setStatusTip( tr("Muestra información acerca de Qt") );
    actionHelpAboutQt->setWhatsThis( ( "Muestra información acerca de Qt" ) );
    connect( actionHelpAboutQt, SIGNAL( activated() ), this, SLOT( helpAboutQt() ) );

    actionHelpWhatsThis = new QAction( tr("¿Qué es esto?"), QIconSet(  whatsthis_image, whatsthis_image  ),
				       tr("¿Qué es esto?"), SHIFT + Key_F1, this, 0 );
    actionHelpWhatsThis->setStatusTip( tr("\"¿Qué es esto?\" ayuda sensitiva") );
    actionHelpWhatsThis->setWhatsThis( ( "¿Qué es esto?" ) );
    connect( actionHelpWhatsThis, SIGNAL( activated() ), this, SLOT( whatsThis() ) );

    QToolBar *tb = new QToolBar( tr("Ayuda"), 0, this, FALSE, "Help" );
    tb->setCloseMode( QDockWindow::Undocked );
    QWhatsThis::add( tb, tr( "<b>La barra de herramientas de ayuda</b>%1" ).arg(tr(toolbarHelp).arg("") ));
    addToolBar( tb, tr( "Ayuda" ) );
    actionHelpWhatsThis->addTo( tb );

    QPopupMenu *menu = new QPopupMenu( this, "Help" );
    menubar->insertSeparator();
    menubar->insertItem( tr( "&Ayuda" ), menu );
    actionHelpContents->addTo( menu );
    menu->insertSeparator();
    actionHelpAbout->addTo( menu );
    actionHelpAboutQt->addTo( menu );

    menu->insertSeparator();
    actionHelpWhatsThis->addTo( menu );    
}

void QlgMainWindow::helpAboutQt()
{
    QMessageBox::aboutQt( this, "Qt Designer" );
}

void QlgMainWindow::projectInsertFile()
{
    
}

void QlgMainWindow::fileNew()
{
    qWarning( "QlgMainWindow::fileNew(): Not implemented yet" );
}

void QlgMainWindow::fileOpen()
{
    qWarning( "QlgMainWindow::fileOpen(): Not implemented yet" );
}

void QlgMainWindow::fileSave()
{
    qWarning( "QlgMainWindow::fileSave(): Not implemented yet" );
}

void QlgMainWindow::fileSaveAs()
{
    qWarning( "QlgMainWindow::fileSaveAs(): Not implemented yet" );
}

void QlgMainWindow::fileSaveAll()
{
    qWarning( "QlgMainWindow::fileSaveAll(): Not implemented yet" );
}

void QlgMainWindow::fileClose()
{
    qWarning( "QlgMainWindow::fileClose(): Not implemented yet" );
}

void QlgMainWindow::filePrint()
{
    qWarning( "QlgMainWindow::filePrint(): Not implemented yet" );
}

void QlgMainWindow::fileExit()
{
    close();
}

void QlgMainWindow::editUndo()
{
    qWarning( "QlgMainWindow::editUndo(): Not implemented yet" );
}

void QlgMainWindow::editRedo()
{
    qWarning( "QlgMainWindow::editRedo(): Not implemented yet" );
}

void QlgMainWindow::editCut()
{
    qWarning( "QlgMainWindow::editCut(): Not implemented yet" );
}

void QlgMainWindow::editCopy()
{
    qWarning( "QlgMainWindow::editCopy(): Not implemented yet" );
}

void QlgMainWindow::editPaste()
{
    qWarning( "QlgMainWindow::editPaste(): Not implemented yet" );
}

void QlgMainWindow::editFind()
{
    qWarning( "QlgMainWindow::editFind(): Not implemented yet" );
}

void QlgMainWindow::helpContents()
{    
    QDir d;
    
    processHelp->clearArguments();
    processHelp->addArgument( "assistant" );
    processHelp->addArgument( "-profile" );
    processHelp->addArgument(  d.path()  + "/help/help.adp" );
    
    if( !processHelp->start() )
	QMessageBox::warning( this, "QLogico", "Activar variable de entorno QLDIR\nImposible encontrar documentación" );
}

void QlgMainWindow::helpAbout()
{
    QMessageBox::about ( this, "Qlogico", "Steel Edward Vázquez George" );
}

void QlgMainWindow::editDelete()
{
    qWarning( "QlgMainWindow::editDelete(): Not implemented yet" );   
}

void QlgMainWindow::editSelectAll()
{
    qWarning( "QlgMainWindow::editSelectAll(): Not implemented yet" );
}

void  QlgMainWindow::updateEditorUndoRedo()
{
    qWarning( "QlgMainWindow::updateEditorUndoRedo(): Not implemented yet" );
}

void QlgMainWindow::addCircuitComponent( QAction *act )
{
    if( circuitView )
	circuitView->addComponent( act->text() );
}

void QlgMainWindow::addFSMComponent( QAction *act )
{ 
    qDebug( "Insert the component : " + act->text() );
    
    if( act->text() == "state" )
	addStateDlg->showDlg();
    else
	addTransitionDlg->showDlg();
    act->setOn( FALSE );
}
