/**********************************************************************
** Copyright (C) 2003 Steel E. V. George.  All rights reserved.i
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

#include <math.h>
#include <qpainter.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>
#include <qpopupmenu.h>
#include <qcursor.h>

#include "qlgcircuitwindow.h"
#include "qlgcircuitview.h"
#include "qlgcircuitline.h"

QlgCircuitView::QlgCircuitView(QlgCircuitWindow* w, const QString name ) 
    : QCanvasView( w->canvas(), w), isMesh( false ), lColor( Qt::black ), mColor( Qt::lightGray )
{
    viewWindow = w;
    viewName = name;
    viewDescription = tr("Sin descripción");
    newLine = 0;
    mainCircuit = 0;
    vInputs.clear();
    lineSelected = 0;
    ccSelected = 0;
    zom = 1;
    lastWidth = canvas()->width();
    vCircuits.clear();
    emit lockChanged( false );
    locked = false;
    menu = new QPopupMenu( this, "menu" );
    menu->insertItem( QPixmap::fromMimeSource("pointer"), tr("Liberar cursor"), this, SLOT(releaseCursor()), Key_Escape, 0 );
    menu->insertItem( tr("Propiedades"), this, SLOT(showProperties()), ALT + Key_P, 1 );
    menu->insertItem( QPixmap::fromMimeSource("delete"), tr("Eliminar"), this, SLOT(removeComponent()), Key_Delete, 2 );
    menu->insertItem( QPixmap::fromMimeSource("grid"), tr("Mostrar malla"), this, SLOT(updateMesh()), ALT + Key_G, 3 );
}
	
/* *****************************************************************
  Function that create a circuit for each boolean function contained 
  in the vector equations
*******************************************************************/

void QlgCircuitView::addCircuit( QlgBoolFunctionList vEquations )
{
    vFunctions = vEquations;
        
    // Absolute coordinates

    x0 = 50;  
    y0 = 50;

#ifndef QT_NO_PROGRESSDIALOG
    QProgressDialog progress( tr( "Construyendo diagrama eléctrico..." ), tr( "Cancelar" ), vEquations.count(),
                          this, tr("progreso"), TRUE );
    progress.setMinimumDuration ( 500 );
    
#endif
    
    // loop to draw every equation, remember no more than 100
    int i = 0;
    QlgBoolFunction *it;
    for(  it = vFunctions.first();  it; it = vFunctions.next() ) {
	mainCircuit = new QlgCircuit( this,  it, QPoint(x0,y0), true );
	vCircuits.append( mainCircuit );
	y0 = mainCircuit->y1();
	if( lastWidth < mainCircuit->x1() )
	    lastWidth = mainCircuit->x1();
	//updateInputVariables( mainCircuit->inputVariables() );
	ThreadUpdateCircuit *threadUC = new ThreadUpdateCircuit( mainCircuit );
	vThreadUC.append( threadUC );
#ifndef QT_NO_PROGRESSDIALOG
	progress.setProgress( i );
	if ( progress.wasCancelled() )
	    break;
#endif	
	i++;
    }
    canvas()->resize( lastWidth, y0 );
    canvas()->update();
    updateCircuitsMode();
#ifndef QT_NO_PROGRESSDIALOG
    progress.setProgress( vFunctions.count() );
#endif
}

void QlgCircuitView::updateCircuitsMode()
{
    for( QlgCircuit *c = vCircuits.first(); c; c = vCircuits.next() )
	c->setAutomatic( false );
}
/****************************************************************
  Function that add a sequential circuit to the view using a JK Flip Flop
****************************************************************/ 

void QlgCircuitView::addCircuitJK( QlgBoolFunctionList vEquations, int numFF, int numOuts )
{
    QlgCircuit( this, vEquations, numFF, numOuts, QPoint(50,50), true );
}

/********************************************************************************************
  Update the values of the vInputs with the new values of the
  @ variables
********************************************************************************************/

void QlgCircuitView::updateInputVariables( QlgCircuitCompList& variables )
{
    if( vInputs.count() == 0 )
	vInputs = variables;	
    else {
	uint i = 0;
	for( QlgCircuitComponent *value = variables.first(); value; value = variables.next() ) {
	    vInputs.first();
	    while( ( i < vInputs.count() ) && ( vInputs.current()->label.text() != value->label.text() ) ) {
		vInputs.next();
		i++;
	    }
	    if( i == vInputs.count() )
		vInputs.append( value );
	    i = 0;
	}
    }
}

void QlgCircuitView::updateInputVariables()
{
    vInputs.clear();
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();
    QlgCircuitComponent *cc;
    for( ; it != list.end(); ++it ) {
	if( (*it)->rtti() == circuitComponentRTTI  ) {
	    cc = (QlgCircuitComponent*)(*it);
	    if( (cc->type() == "entrada") && cc->isVisible() ) {
		uint i = 0;
		vInputs.first();
		while( ( i < vInputs.count() ) && ( vInputs.current()->label.text() != cc->label.text() ) ) {
		    vInputs.next();
		    i++;
		}
		if( i == vInputs.count() )
		    vInputs.append( cc );
		i = 0;
	    }
	}
    }
}

void QlgCircuitView::contentsMousePressEvent(QMouseEvent* e)
{
    QPoint p = inverseWorldMatrix().map(e->pos());
    
    if( e->button() == Qt::RightButton ) {
	if( newComponent != "Apuntador" && newComponent != "" )
	    menu->setItemEnabled( 0, TRUE );
	else
	    menu->setItemEnabled( 0, FALSE );
	menu->setItemEnabled( 1, TRUE );
	if( moving ) {	    
	    menu->setItemEnabled( 2, TRUE );
	    menu->setItemEnabled( 3, FALSE );
	}
	else {
	    menu->setItemEnabled( 2, FALSE );
	    if( isMesh )
		menu->changeItem( 3, tr("Ocultar Malla") );
	    else
		menu->changeItem( 3, tr("Mostrar Malla") );
	    menu->setItemEnabled( 3, TRUE  );
	}	
	menu->exec( e->globalPos() );
	return;
    }
    
    QCanvasItemList l=canvas()->collisions(p);
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == circuitComponentRTTI ) {
	    circuitComponent = (QlgCircuitComponent*)(*it);
	    if( ccSelected !=  circuitComponent ) {
		if( ccSelected )
		    ccSelected->setSelected( false );
		circuitComponent->setSelected( true );
		ccSelected = circuitComponent;
		canvas()->setAllChanged();	    
		canvas()->update();
	    }
	    moving = *it;
	    moving_start = p;
	    return;
	}
	else if ( (*it)->rtti() == circuitLineRTTI ) {
	    line = (QlgCircuitLine*)(*it);
	    if( lineSelected != line ) {
		if( lineSelected )
		    lineSelected->setSelected( false );
		line->setSelected( true );
		lineSelected = line;
		canvas()->setAllChanged();	    
		canvas()->update();		
	    }
	    moving = *it;
	    moving_start = p;
	    return;
	}
    }

    if( ccSelected ) {
	ccSelected->setSelected( false );
	ccSelected = 0;
    }
    if( lineSelected ) {
	lineSelected->setSelected( false );
	lineSelected = 0;
    }

    moving = 0;
    
    if( newComponent == "linea" ) {
	double x;
	modf( p.x() * 0.1, &x );
	double y;
	modf( p.y() * 0.1, &y );
	if( !mainCircuit ) {
	    mainCircuit = new QlgCircuit( this );
	    ThreadUpdateCircuit *threadUC = new ThreadUpdateCircuit( mainCircuit );
	    vThreadUC.append( threadUC );
	}
	newLine = new QlgCircuitLine( mainCircuit, x*10, y*10, x*10, y*10 );	
    }
    else if( newComponent != "Apuntador" && newComponent != "" ) {
	if( !mainCircuit ) {	
	    mainCircuit = new QlgCircuit( this );
	    ThreadUpdateCircuit *threadUC = new ThreadUpdateCircuit( mainCircuit );
	    vThreadUC.append( threadUC );
	}
	mainCircuit->addComponent( newComponent, p );
    }    
    
    canvas()->setAllChanged();
    canvas()->update();
    
    emit showDescription( viewDescription );
}

void QlgCircuitView::contentsMouseMoveEvent( QMouseEvent* e )
{    
    if ( moving ) {	     
	QPoint p = inverseWorldMatrix().map(e->pos());	 
	moving->moveBy(p.x() - moving_start.x(),
		       p.y() - moving_start.y());
	moving_start = p;
	canvas()->setAllChanged();
	canvas()->update();	
    }
    else if( newLine ) {
	QPoint p = inverseWorldMatrix().map(e->pos());
	double dx = newLine->startPoint().x() < p.x();
	double dy = newLine->startPoint().y() < p.y();	
	double x;
	 modf( p.x() * 0.1, &x );
	 double y;
	 modf( p.y() * 0.1, &y );	
	if(  (ulong)dx < (ulong)dy )
	    newLine->setPoints( newLine->startPoint().x(), newLine->startPoint().y(), newLine->startPoint().x(), int(y)*10 );
	else
	    newLine->setPoints( newLine->startPoint().x(), newLine->startPoint().y(), int(x)*10,  newLine->startPoint().y() );
	canvas()->update();
    }
}

void QlgCircuitView::contentsMouseReleaseEvent(  QMouseEvent* e )
{
     if ( moving ) {
	 double ipx;
	 double ipy;
	 double dx;
	 double dy;
	 double x;
	 double y;
	 if( moving->rtti() == circuitComponentRTTI ) {
	     x = moving->x();
	     y = moving->y();
	 }
	 else {
	     x = ((QlgCircuitLine*)moving)->startPoint().x();
	     y = ((QlgCircuitLine*)moving)->startPoint().y();
	 }
	 dx = modf( x * 0.1, &ipx );
	 dy = modf( y * 0.1, &ipy );
	 moving->moveBy( -dx*10, -dy*10 );
	 canvas()->setAllChanged();
	 canvas()->update();
    }
     else if( newLine )
	 newLine = 0;
}

void QlgCircuitView::contentsMouseDoubleClickEvent ( QMouseEvent * e ) 
{
    QPoint p = inverseWorldMatrix().map(e->pos());   
    QCanvasItemList l=canvas()->collisions(p); 
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == circuitComponentRTTI ) {
	    emit editCircuitComponent( (QlgCircuitComponent*)(*it) );
	    return;
	}
    }
    emit editView( this );
}

// Clear the schematic area

void QlgCircuitView::clear()
{
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();
    for( ; it != list.end(); ++it )
	    (*it)->hide();        
}

/*************************************************************
  Add mesh to the view
*************************************************************/

void QlgCircuitView::addMesh()
{
    if( isMesh )
	clearMesh();
    
    int x0 = 10;
    int y0 = 10;
    int dx = 10;
    int dy = 10;
    QlgPoint* point;
    vMesh.clear();
    
    for( int i=0; i<canvas()->width()/dx; i++) {
	for( int j=0; j<canvas()->height()/dy; j++) {
	    point = new QlgPoint( this, x0, y0 );
	    vMesh.append( point );
	    y0 = y0 + dy;
	}
	y0 = dy;
	x0 = x0 + dx;
    }
    
    isMesh = true;
}
	
/**********************************************************
  Clear the mesh of the view
**********************************************************/

void QlgCircuitView::clearMesh()
{
    if( !isMesh)
	return;		    
	    
    for( QlgPoint *point = vMesh.first(); point; point = vMesh.next() ) {
	point->hide();
	delete point;
    }
    
    vMesh.clear();    
    
    isMesh = false;
}

void QlgCircuitView::setZoom( double newZoom )
{
    QWMatrix m = worldMatrix();
    double factor = newZoom / zom;
    zom = newZoom;
    m.scale( factor, factor );
    setWorldMatrix( m );
}

void QlgCircuitView::addComponent( QString name )
{
    newComponent = name;
	
     if( newComponent == "linea" )
	setCursor( QCursor(Qt::CrossCursor)  );
    else if( newComponent != "Apuntador" && newComponent != "" )
	setCursor( QCursor( QPixmap::fromMimeSource( newComponent ) )  );
    else
	setCursor( QCursor( Qt::ArrowCursor ) );
}


/********************************************************************
 Update the schematic view
*********************************************************************/

void QlgCircuitView::simulateCircuit( QString vars, QString value )
{
    ThreadUpdateCircuit *it;
    
    for (  it = vThreadUC.first(); it; it = vThreadUC.next() ) {
	it->variables = vars;
	it->value = value;
	it->start();
	it->wait();
    }

    canvas()->setAllChanged();
    canvas()->update();       
}

void QlgCircuitView::showProperties()
{
    if( moving ) {
	if ( (moving)->rtti() == circuitComponentRTTI ) {
	    emit editCircuitComponent( (QlgCircuitComponent*)(moving) );
	    return;
	}
    }
    else
	emit editView( this );        
}

void QlgCircuitView::removeComponent()
{
    if( moving ) {
	if ( moving->rtti() == circuitComponentRTTI ) {
	    QlgCircuitComponent *cc = (QlgCircuitComponent*)(moving);
	    mainCircuit->removeComponent( cc );	    
	}
	else if ( moving->rtti() == circuitLineRTTI ) {
	    QlgCircuitLine* line = (QlgCircuitLine*)(moving);
	    line->hide();
	    line->reset();
	    line = 0;
	}
	canvas()->setAllChanged();
	canvas()->update();
    }
}
 
void QlgCircuitView::releaseCursor()
 {
    setCursor( QCursor( Qt::ArrowCursor ) );
    newComponent = "";
    emit cursorReleased();
 }

void QlgCircuitView::updateMesh()
{    
    if( menu->text( 3 ).find( tr("Mostrar") ) != -1 )
	addMesh();
    else
	clearMesh();
    canvas()->setAllChanged();
    canvas()->update();
}

void QlgCircuitView::setDescription( QString d )
{ 
    viewDescription = d;
}

void QlgCircuitView::changeName( QString n )
{ 
    viewName = n; 
    emit nameChanged( viewName );
}

/************************************************************************************************
Thread that redraw the circuit for a specific input value
The argument is the desired output
***********************************************************************************************/

void ThreadUpdateCircuit::run()
{
    threadCircuit->updateCircuitInputs( variables, value );
}

QlgPoint::QlgPoint( QlgCircuitView* c, int x, int y )
	: QCanvasRectangle( x, y, 1, 1, c->canvas() ), circuitView( c )
{
    setZ( 0 );
    move( x, y );
    show();
 
}

void QlgPoint::drawShape( QPainter& p )
{
    p.setPen( QPen( circuitView->meshColor(), 1 ) );
    p.drawPoint( (int)x(), (int)y() );
}
