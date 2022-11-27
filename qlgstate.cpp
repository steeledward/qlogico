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

/**************************************************************************
  Implementation of the FSM components
****************************************************************************/

#include <qpainter.h>
#include <math.h>
#include <qwidget.h>
#include <qtooltip.h>

#include "qlgstate.h"
#include "qlgtransition.h"

QlgState::QlgState( QCanvas *canvas, QString Name, QString Value, int Ratio, int LineWidth, QColor Color, QString des )
           :  QCanvasRectangle(canvas)
{ 	           
    selected = true;
    rat = Ratio;
    lineW = LineWidth;
    setSize( rat, rat );
    s_name = Name;
    val = Value;
    col = Color;
    descrip = des;
    move( 0, 0);    
    setZ( 100 );
    show();    
}

void QlgState::moveBy(double dx, double dy)
{    
    QCanvasRectangle::moveBy( dx, dy );
}

void QlgState::drawShape( QPainter &p )
{
    if( rat != width() )
	setSize( rat, rat );
    
    QPen pen;
    pen.setWidth( lineW );
    p.setPen( pen );
	    
    if( selected ) {
	pen.setColor( Qt::red );
	p.setPen( pen );
	p.drawRect((int)x(), (int)y(), rat, rat );   
    }
    
    pen.setColor( Qt::black );
    p.setPen( pen );
    p.setBrush( col );
    p.setFont( font );
    p.drawEllipse( (int)x(), (int)y(), (int)width(), (int)height() );
    p.drawLine( (int)x(), (int)y()+(rat/2), (int)x()+rat, (int)y()+(rat/2) );
    p.drawText( (int)x()+10, (int)y()+(rat/2)-5, s_name, -1,QPainter::Auto );
    p.drawText( (int)x()+10, (int)y()+(rat/2)+15, val, -1,QPainter::Auto );
}

