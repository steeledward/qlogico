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

/*****************************************************************
  File where is implemented the functionality of the FSM view
******************************************************************/  
#include <qpainter.h>
#include <qimage.h>
#include <qprogressdialog.h>
#include <qmessagebox.h>
#include <qwmatrix.h> 
#include <qcolordialog.h>
#include <math.h>
#include <qtimer.h>
#include <qcolor.h>

#include "qlgtransition.h"
#include "qlgtitle.h"
#include "qlgfsmview.h"
#include "qlgfsmoptions.h"
#include "qlgfsmmachine.h"

QlgFSMView::QlgFSMView(QCanvas* canvas, QWidget* parent ) 
	: QCanvasView(canvas, parent), title( canvas )
{        
    zom = 1;
    viewDescription = tr("Sin descripción");
}

/***********************************************************
  Add aFSM Machine to the view
***********************************************************/

void QlgFSMView::addMachine(  QlgFSMOptions *values )
{
    viewOptions = values;
    viewName = values->name();
    
    resize( values->width(), values->height() );
    canvas()->resize( values->width(), values->height() );
    canvas()->setBackgroundColor( values->color() );
    
    if( values->mesh() )
	addMesh();
    else
	clearMesh();
    
    title.setText( viewName );
    title.move( 50, 50 );
    title.show();
    
    fsmMachine = new QlgFSMMachine( this->canvas(), values );
    canvas()->setAllChanged();
    canvas()->update();
}

void QlgFSMView::contentsMousePressEvent(QMouseEvent* e)
{
    QPoint p = inverseWorldMatrix().map(e->pos());   
    QCanvasItemList l=canvas()->collisions(p); 
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == stateRTTI ) {
	    fsmMachine->setState( (QlgState*)(*it) );
	    if( fsmMachine->stateSelected() != fsmMachine->state() ) {
		fsmMachine->stateSelected()->setSelected( false );
		fsmMachine->state()->setSelected( true );
		fsmMachine->setStateSelected( fsmMachine->state() );
		canvas()->setAllChanged();	    
		canvas()->update();
	    }
	    moving = *it;
	    moving_start = p;
	}
	else if ( (*it)->rtti() == transtitionRTTI ) {
	    fsmMachine->setTransition( (QlgTransition*)(*it) );
	    if( fsmMachine->transitionSelected() != fsmMachine->transition() ) {
		fsmMachine->transitionSelected()->setSelected( false );
		fsmMachine->transition()->setSelected( true );
		fsmMachine->setTransitionSelected( fsmMachine->transition() );
		canvas()->setAllChanged();	    
		canvas()->update();
	    }
	}	
	return;
    }
    moving = 0;
    emit showDescription( viewDescription );
}

void QlgFSMView::contentsMouseMoveEvent(QMouseEvent* e)
{
    if ( moving ) {
	QPoint p = inverseWorldMatrix().map(e->pos());
	moving->moveBy(p.x() - moving_start.x(),
		       p.y() - moving_start.y());
	moving_start = p;    
    	
	for( uint i=0; i<fsmMachine->state()->transitions().count(); i++) {
	    fsmMachine->state()->transitions().at(i)->updateConections();
	    fsmMachine->state()->transitions().at(i)->drawArrow( fsmMachine->state()->transitions().at(i)->points()[2].x(), fsmMachine->state()->transitions().at(i)->points()[2].y(),  fsmMachine->state()->transitions().at(i)->points()[3].x(), fsmMachine->state()->transitions().at(i)->points()[3].y() );
	    fsmMachine->state()->transitions().at(i)->setPoints( fsmMachine->state()->transitions().at(i)->points() );
	}	
	canvas()->setAllChanged();	
	canvas()->update();
    }
}

void QlgFSMView::contentsMouseDoubleClickEvent ( QMouseEvent * e ) 
{
    QPoint p = inverseWorldMatrix().map(e->pos());   
    QCanvasItemList l=canvas()->collisions(p); 
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == stateRTTI ) {
	    emit editState( (QlgState*)(*it) );
	    return;
	}
	else if ( (*it)->rtti() == transtitionRTTI ) {
	    emit editTransition( (QlgTransition*)(*it) );
	    return;
	}
    }
    emit editFSM( this );
}

void QlgFSMView::clear()
{
    for( QlgState* st = fsmMachine->usedStates().first(); st; st = fsmMachine->usedStates().next() ) 
	st->hide();            	 
    fsmMachine->usedStates().clear();        
    
    for( QlgTransition* tr = fsmMachine->transitions().first(); tr; tr = fsmMachine->transitions().next() ) 
	tr->hide();                    
    fsmMachine->transitions().clear();
    
    canvas()->update();
    
    fsmMachine->setStateSelected(0);
    fsmMachine->setTransitionSelected(0);    
}

/************************************************************
  Clear the mesh of the view
*************************************************************/

void QlgFSMView::clearMesh()
{
    for( QCanvasRectangle *point = vMesh.first(); point; point = vMesh.next() ) 
	point->hide();
    vMesh.clear();
}

/*************************************************************
  Add mesh to the view
*************************************************************/

void QlgFSMView::addMesh()
{
    int x0 = 10;
    int y0 = 10;  
    QCanvasRectangle* point;
    vMesh.clear();
    
    for( int i=0; i<canvas()->width()/10; i++) {
	for( int j=0; j<canvas()->height()/10; j++) {
	    point = new QCanvasRectangle( x0, y0, 2, 2, this->canvas() );
	    vMesh.append( point );
	    y0 = y0 + 10;
	}
	y0 = 10;
	x0 = x0 + 10;	
    }
    isMesh = true;
}

void QlgFSMView::setZoom( double newZoom )
{
    QWMatrix m = worldMatrix();
    double factor = newZoom / zom;
    zom = newZoom;
    m.scale( factor, factor );
    setWorldMatrix( m );    
}

/************************************************************
  Update the values of the FSM with the new values
************************************************************/

void QlgFSMView::setOptions( QlgFSMOptions *newOptions )
{
    viewOptions = newOptions;
    changeName( viewOptions->name() );    
    resize( viewOptions->width(), viewOptions->height() );
    canvas()->resize( viewOptions->width(), viewOptions->height() );
    canvas()->setBackgroundColor( viewOptions->color() );
    if( viewOptions->mesh() )
	addMesh();
    else
	clearMesh();
    fsmMachine->setValues( viewOptions );
}

void QlgFSMView::changeName( QString newName )
{
    if( newName == "" )
	newName = tr("Sin nombre" );

    title.setText( newName  );
    
    emit nameChanged( viewName );
}
