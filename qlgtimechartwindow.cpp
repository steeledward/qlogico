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

#include <qcolordialog.h>
#include <qcanvas.h>
#include <qwmatrix.h> 
#include <qvariant.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qaction.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qtoolbar.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qstringlist.h>
#include <qcombobox.h>

#include <math.h>

#include "qlgtimechartoptionsdlgimpl.h"
#include "qlgtimechartwindow.h"
#include "qlgcircuitview.h"
#include "qlgmainwindow.h"
#include "qlgtruetableview.h"

QlgTimeChartWindow::QlgTimeChartWindow( QWidget *parent, const char * name, WFlags f  )
	: QMainWindow( parent, name, Qt::WType_Dialog  )
{
    lColor = Qt::magenta;
    mColor = Qt::lightGray;
    isMesh = true;
    bitListVariables = 0;
    variables.clear();    
    circuitView = 0;
    zom = 1;
    x0 = 40;
    y0 = 40;
    counter = 0;
    
    setCaption( "QLogico" + tr( "Diagrama de tiempos" ) );
    setupToolsActions();
    
    timeChartCanvas = new QCanvas(500,400);
    timeChartView = new QCanvasView( timeChartCanvas, this );

    //timeChartView->resize( 500, 400 );        
    
    setFocusProxy( timeChartView );
    setCentralWidget( timeChartView );    
    
    timeChartOptions = new QlgTimeChartOptionsDlgImpl( this );
}

void QlgTimeChartWindow::setupToolsActions()
{
    toolBarTools = new QToolBar( tr("Preferencias"), 0, this, FALSE, "Preferencias" );
    toolBarTools->setLabel( tr( "Preferencias" ) );
    addToolBar( toolBarTools, tr( "Preferencias" ) );    
    
    comboBoxZoom = new QComboBox( TRUE, toolBarTools, "cbZoom" );
    QToolTip::add( comboBoxZoom, tr("Acercamiento"));
    QWhatsThis::add( comboBoxZoom, tr("Muestra el porcentaje de acercamiento del diagrama de tiempos, si desea cambiarlo solo escriba un nuevo valor o seleccione uno disponible"  ));
    
    QStringList zoom;
    zoom << "100 %" << "50 %"    << "25 %";
    comboBoxZoom->insertStringList( zoom );
    connect( comboBoxZoom, SIGNAL( activated(int) ), this, SLOT( zoom() ) );
    
    QAction *actionViewOptions = new QAction( tr( "Configuración" ), QPixmap::fromMimeSource( "timeoptions" ), tr( "&Configuración" ), ALT + Key_C, this, 0 );
    actionViewOptions->setStatusTip( tr( "Configuración" ) );
    actionViewOptions->setWhatsThis( "Muestra un dialogo con las opciones para configurar el <b><font color=#355e00>Diagrama de tiempos</font></b>" );
    connect( actionViewOptions, SIGNAL( activated() ), this, SLOT( showOptions() ) );
             
    actionViewOptions->addTo( toolBarTools );
        
    QAction *actionGetTrueTable = new QAction( tr( "Tabla de verdad" ), QPixmap::fromMimeSource( "table" ), tr( "&Tabla de verdad" ), ALT + Key_T, this, 0 );
    actionGetTrueTable->setStatusTip( tr( "Obtener tabla de verdad" ) );
    actionGetTrueTable->setWhatsThis( "Obtiene la <b><font color=#355e00>Tabla de verdad</font></b> del actual <b><font color=#355e00>Diagrama de tiempos</font></b> " );
    connect( actionGetTrueTable, SIGNAL( activated() ), this, SLOT( getTrueTable() ) );    
     
    QToolBar *tb = new QToolBar( tr("Opciones"), 0, this, FALSE, "Opciones" );
    tb->setCloseMode( QDockWindow::Undocked );
    
    addToolBar( tb, tr( "Opciones" ) );
        
    actionGetTrueTable->addTo( tb );    
    
}

/*************************************************************
  When in the main application a new circuit window is selected
  this is the update for the time chart
*************************************************************/

void QlgTimeChartWindow::update( QlgCircuitView* cv)
{ 
    circuitView=cv;
    
    if( circuitView ) {
	setEnabled( true );	
	init();	
    }
    else
	setEnabled( false );
}

/**************************************************************
  Whe the time chart is called then...
**************************************************************/

void QlgTimeChartWindow::showDlg()
{    
    if( circuitView ) {
	init();
	show();
    }
    else
	QMessageBox::warning( this, "QLogico", tr("Seleccione antes el Esquemático que desea simular") );
}

/***************************************************************
 Initialize the time chart dialog.
 using the information from the circuit view
***************************************************************/

void QlgTimeChartWindow::init()
{
    x0 = 40;
    y0 = 40;    
    
    clearCanvas();
    variables.clear();
    outputs.clear();
    
    if( bitListVariables )
	delete[] bitListVariables;    
    
    //if( circuitView->circuit() && !circuitView->isLocked() )
    //circuitView->updateInputVariables( circuitView->circuit()->inputVariables() );
    
    circuitView->updateInputVariables();
    
    QCanvasText *var;
    
    for( QlgCircuitComponent *variable = circuitView->inputVariables().first(); variable; variable = circuitView->inputVariables().next() ) {
	variables.append( variable->label.text() );
	var =  new QCanvasText( variable->label.text(), timeChartCanvas );
	var->setColor( lColor );
	var->move( 10, y0 - var->boundingRect().bottom() );
	var->show();
	y0 = y0 + 40;
    }
  
    for( ThreadUpdateCircuit *cir = circuitView->circuits().first(); cir; cir = circuitView->circuits().next() ) {
	for( QlgCircuitComponent *comp = cir->circuit()->outputVariables().first();  comp; comp = cir->circuit()->outputVariables().next() ) {
	    outputs.append( comp->label.text() );
	    var =  new QCanvasText( comp->label.text(), timeChartCanvas );
	    var->setColor( lColor );
	    var->move( 10, y0  - var->boundingRect().bottom() );
	    var->show();
	    y0 = y0 + 40;
	}
    }
    
    bitListVariables = new int *[variables.count()+outputs.count()];
    for( uint i = 0; i < (variables.count()+outputs.count()); i++ )
	bitListVariables[i] = new int[(int)pow(2,variables.count())];

    for( uint i = 0; i < (variables.count()+outputs.count()); i++ ) {
	for( int j = 0; j <pow(2,variables.count()); j++ )
	    bitListVariables[i][j] = 0;
    }
			      
    timeChartCanvas->resize( ((int)pow( 2, variables.count() )*20) + 60,  (variables.count()+outputs.count()+1) * 40  );

    if( isMesh )
	addMesh();
    
    timeChartCanvas->update();
    
    y0 = 40;
    
}
/*************************************************************
  Functions that update the view for each new value to come
  don't pay attention to the @ n
*************************************************************/  
  
void QlgTimeChartWindow::nextStep( int n )
{
    y0 = 40;    
    x0 = x0 + 20;
    counter++;
}

/************************************************************
  The function that draw the @ value of a variable @ var
************************************************************/

void QlgTimeChartWindow::draw( QString var, int value )
{    
    if( !this->isVisible() )
	return;
    
    if( counter > ( pow(2, variables.count() ) -1 ))
	clearTimeLines();    
    
    QPen pd ( lColor, 3, Qt::SolidLine  );
    QCanvasLine* line;
    
    int y = variables.findIndex(var) + 1;
 
    if( !y )
	y = variables.count() + outputs.findIndex(var) + 1;
    
    int y_last = 0;
    
    if( counter )
	y_last = bitListVariables[y-1][counter-1];
	
    if( y_last != value ) {
	line = new QCanvasLine( timeChartCanvas );
	line->setZ( 1 );
	line->setPen( pd );
	line->setPoints( x0, ( y * 40 ) - y_last*20, x0, ( y * 40 ) - value*20 );
	line->show();
    }
    
    line = new QCanvasLine( timeChartCanvas );
    line->setZ( 1 );
    line->setPen( pd );
    line->setPoints( x0, ( y * 40 )- value*20, x0 + 20, ( y * 40 ) - value*20 );
    line->show();
   
    timeChartCanvas->update();
    bitListVariables[y-1][counter] = value;
}

/***********************************************************
  Obtain a true table in  function of the available data
***********************************************************/

void QlgTimeChartWindow::getTrueTable()
{
    if( !circuitView )
	return;
    
    QString vars = variables.join( "," );
    QString outs = outputs.join( "," );
    
    ((QlgMainWindow*)(parentWidget()))->trueTableView->setTrueTable( vars, outs, bitListVariables );
    
}

/*********************************************************
  Clear the time lines 
*********************************************************/

void QlgTimeChartWindow::clearTimeLines()
{
    QCanvasItemList list = timeChartCanvas->allItems();
    QCanvasItemList::Iterator it = list.begin();
  
    for (; it != list.end(); ++it) {
        if ( (*it)->rtti() == 7 && (*it)->z() == 1 )
            delete *it;
    }
    
    for( uint i = 0; i < (variables.count()+outputs.count()); i++ ) {
	for( int j = 0; j <pow(2,variables.count()); j++ )
	    bitListVariables[i][j] = 0;
    }
        
    x0 = 40;   
    counter = 0;
}

/*********************************************************
  Clear the view
*********************************************************/

void QlgTimeChartWindow::clearCanvas()
{
    QCanvasItemList list = timeChartCanvas->allItems();
    QCanvasItemList::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
        if ( *it )
            delete *it;
    }    
}

/***************************************************************
 Change the color of the background for the time chart
***************************************************************/

void QlgTimeChartWindow::setColorBackground()
{
 QColor color = QColorDialog::getColor(
                             timeChartCanvas->backgroundColor(),
                            this, "Seleccionar color de fondo" );
        if ( color.isValid() )
	    timeChartCanvas->setBackgroundColor( color );

}

/***************************************************************
 Zoom In the time chart
***************************************************************/

void QlgTimeChartWindow::zoom()
{
    double newZoom = ( comboBoxZoom->currentText().remove( " %" ).toInt() ) * 0.01;
    QWMatrix m = timeChartView->worldMatrix();
    double factor = newZoom / zom;
    zom = newZoom;
    m.scale( factor, factor );
    timeChartView->setWorldMatrix( m );        
}

void QlgTimeChartWindow::clearMesh()
{
    QCanvasItemList list = timeChartCanvas->allItems();
    QCanvasItemList::Iterator it = list.begin();
    
    for (; it != list.end(); ++it) {
	if ( (*it)->rtti() == 7 && (*it)->z() == 0 )
	    delete *it;
    }        
}

void QlgTimeChartWindow::addMesh()
{
    QCanvasLine* line;
    QPen pd ( mColor, 1, Qt::DotLine  );
    
    for( int i = 1; i < timeChartCanvas->height() / 20; i++ ) {
	line = new QCanvasLine( timeChartCanvas );
	line->setZ( 0 );
	line->setPen( pd );
	line->setPoints( 0, i*20, timeChartCanvas->width(), i*20 );
	line->show();
    }    
    
    for( int i = 1; i < timeChartCanvas->width() / 20; i++ ) {
	line = new QCanvasLine( timeChartCanvas );
	line->setZ( 0 );
	line->setPen( pd );
	line->setPoints( i*20, 0, i*20, timeChartCanvas->height() );
	line->show();
    }
    
}
void QlgTimeChartWindow::showOptions()
{
    timeChartOptions->showDlg( this );
}

QlgTimeChartWindow::~QlgTimeChartWindow()
{
    
}
