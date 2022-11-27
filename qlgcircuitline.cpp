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

#include "qlgcircuitline.h"
#include "qlgcircuitcomponent.h"
#include "qlgcircuit.h"
#include "qlgcircuitview.h"

#include <math.h>

QlgCircuitLine::QlgCircuitLine( QlgCircuit* c, double x0, double y0, double x1, double y1 )
	: QCanvasLine( c->canvas() ), mainCircuit( c )
{
    cOutput = 0;
    setPoints( int(x0), int(y0), int(x1), int(y1) );   
    reset();
    setSelected( false );    
    setZ( 1 );
    show();    
}

void QlgCircuitLine::reset()
{
    connectedAtEnd = false;
    connectedAtBegin = false;
    connectedEndExact = false;
    connectedBeginExact = false;    
    cInput = 0;
    nInput = 0;
    nOutput = 0;
}

void QlgCircuitLine::drawShape( QPainter & p )
{    
    if( isSelected() )
	p.setPen( QPen( Qt::magenta, 1 ) );
    else
	p.setPen( QPen( mainCircuit->view()->lineColor(), 1 ) );
    
    p.drawLine( startPoint(), endPoint() );
    
    if( mainCircuit->isAutomatic() )
	return;
    
    reset();

    QCanvasItemList l = collisions( TRUE );
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if( (*it)->rtti() == circuitLineRTTI  ) {
	    lineCollided = (QlgCircuitLine*)(*it);
	    if( !lineCollided->isHorizontal() && isHorizontal() ) {
		if( startPoint().x() == lineCollided->startPoint().x() ) {
		    connectedAtBegin = true;
		    if( ( startPoint().y() == lineCollided->startPoint().y() ) || ( startPoint().y() == lineCollided->endPoint().y() ) )
			connectedBeginExact = true;
		    if( lineCollided->circuitInput() ) {
			cInput = lineCollided->circuitInput();
			nOutput = lineCollided->numOutput();
		    }
		}
		else if( endPoint().x() == lineCollided->startPoint().x() ) {
		    connectedAtEnd = true;
		    if( ( endPoint().y() == lineCollided->startPoint().y() ) || ( endPoint().y() == lineCollided->endPoint().y() ) )
			connectedEndExact = true;
		    //lineCollided->setInput( cInput, nInput );
		    if( lineCollided->circuitInput() ) {
			cInput = lineCollided->circuitInput();
			nOutput = lineCollided->numOutput();
		    }
		}		
	    } else if( lineCollided->isHorizontal() && !isHorizontal() ) {
		if( startPoint().y() == lineCollided->startPoint().y() ) {
		    connectedAtBegin = true;
		    if( ( startPoint().x() == lineCollided->startPoint().x() ) || ( startPoint().x() == lineCollided->endPoint().x() ) )
			connectedBeginExact = true;
		    if( lineCollided->circuitInput() ) {
			cInput = lineCollided->circuitInput();
			nOutput = lineCollided->numOutput();
		    }
		}
		else if( endPoint().y() == lineCollided->startPoint().y() ) {
		    connectedAtEnd = true;
		    if( ( endPoint().x() == lineCollided->startPoint().x() ) || ( endPoint().x() == lineCollided->endPoint().x() ) )
			connectedEndExact = true;
		    if( lineCollided->circuitInput() ) {
			cInput = lineCollided->circuitInput();
			nOutput = lineCollided->numOutput();
		    }
		}
	    }
	}
	else if ( (*it)->rtti() == circuitComponentRTTI ) {
	    circuitCollided = (QlgCircuitComponent*)(*it);
	    if( isHorizontal() ) {
		if( startPoint().x() == circuitCollided->outputX()  ) {
		    int out = 0;
		    while( out < circuitCollided->numOutputs() && !connectedAtBegin ) {			
			if(  startPoint().y() == circuitCollided->outputY(out) ) {
			    connectedAtBegin = true;
			    connectedBeginExact = true;
			    cInput =  circuitCollided;
			    nOutput = out;
			}
			out++;
		    }
		}
		else if( endPoint().x() == circuitCollided->inputX() ) {
		    int in = 0;
		    while( in < circuitCollided->numInputs() && !connectedAtEnd ) {
			if( endPoint().y() == circuitCollided->inputY(in) ) {
			    connectedAtEnd = true;
			    connectedEndExact = true;
			    nInput = in;
			    if( cInput ) {
				circuitCollided->connect( cInput, nInput, nOutput );
				cOutput = circuitCollided;			    
			    }
			}
			in++;
		    }
		}
	    }
	}
    }
        
    if( cOutput && !connectedAtEnd ) {
	cOutput->disconnect( nInput );
	nInput = 0;
	cOutput = 0;
    }    
    
    if( connectedAtBegin && !connectedBeginExact ) {
	p.setBrush( Qt::SolidPattern );
	p.drawEllipse( startPoint().x() - 1, startPoint().y() - 1, 3, 3 );
    }
    else if( !connectedAtBegin ) {
	p.setBrush( Qt::NoBrush );
	p.drawRect( startPoint().x() - 2, startPoint().y() - 2, 5, 5 );
    }
    
    if( connectedAtEnd && !connectedEndExact ) {
	p.setBrush( Qt::SolidPattern );
	p.drawEllipse( endPoint().x() - 1, endPoint().y() - 1, 3, 3 );
    }
    else if( !connectedAtEnd ) {
	p.setBrush( Qt::NoBrush );
	p.drawRect( endPoint().x() - 2, endPoint().y() - 2, 5, 5 );
    }
}

void QlgCircuitLine::moveBy(double dx, double dy)
{   	    
    setPoints( startPoint().x() + int(dx), startPoint().y() + int(dy), endPoint().x() + int(dx), endPoint().y() + int(dy) );    
    
    QCanvasItemList l = collisions( true );
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
	if ( (*it)->rtti() == circuitLineRTTI  ) {
	    lineCollided = (QlgCircuitLine*)(*it);
	    if( !lineCollided->isHorizontal() && !isHorizontal() ) {
		if( lineCollided->startPoint().y() < startPoint().y() )
		    setPoints( lineCollided->startPoint().x(), lineCollided->startPoint().y(), lineCollided->endPoint().x(), endPoint().y() );
		else
		    setPoints( startPoint().x(), startPoint().y(), startPoint().x(), lineCollided->endPoint().y() );
		lineCollided->hide();
		delete lineCollided;
	    } else if( lineCollided->isHorizontal() && isHorizontal() ) {
		if( lineCollided->startPoint().x() < startPoint().x() )
		    setPoints( lineCollided->startPoint().x(), lineCollided->startPoint().y(), endPoint().x(), lineCollided->endPoint().y() );
		else
		    setPoints( startPoint().x(), startPoint().y(), lineCollided->endPoint().x(), endPoint().y() );
		lineCollided->hide();
		delete lineCollided;
	    }
	}
    }
}

void QlgCircuitLine::setInput( QlgCircuitComponent *input, int output )
{
    cInput = input;
    nOutput = output;
}

bool QlgCircuitLine::isHorizontal()
{
    if( startPoint().y() == endPoint().y() )
	horizontal = true;
    else
	horizontal = false;
    
    return horizontal;
}
