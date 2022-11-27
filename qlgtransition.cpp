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

#include <qpainter.h>
#include <qmessagebox.h>

#include "qlgtransition.h"

QlgTransition::QlgTransition( QCanvas* canvas, QlgState* a, QlgState* n, QString in, QString out ) 
	: QCanvasSpline( canvas ), pa(4), pts(3), arrow( canvas )
{  
    col = Qt::black;
    style = Qt::SolidLine;
       
    selected = true;
    
    lab = in + " / " + out ;
    lineWidth = 2;
    act = a;
    nex = n;
    
    c_input = in;
    c_output = out;
    
    // Update the array with the points to contruct the transition line
    
    updateConections();    
    setPoints( pa );
    setZ( 0 );
    show();
   
    // Draw an arrow to the end of the line
    
    drawArrow( pa[2].x(), pa[2].y(),  pa[3].x(), pa[3].y() );    
    arrow.setZ(500);    
    arrow.show(); 
                       
    act->transitions().append( this ); //Check
    nex->transitions().append( this ); // Check
    
}

/************************************************************
Function that update the points for draw the transition line when a state
is moved in the canvas view
************************************************************/

void QlgTransition::updateConections()
{
    int ratio = nex->width() / 2;
    
    pa[0] = QPoint( (int)act->x() + ratio, (int)act->y() + ratio );
    
    if( act != nex ) {
	
	pa[3] = QPoint( (int)nex->x()+ ratio, (int)nex->y()+ ratio );
        
	int dx, dy;
        
	dx = (int)( pa[3].x() - pa[0].x() ) / 3;
	dy = (int)( pa[3].y() - pa[0].y() ) / 3; 
	
	pa[1] = QPoint( pa[0].x() + dx, pa[0].y() + dy );
	pa[2] = QPoint( pa[1].x() + dx, pa[1].y() + dy );	
	
	pa[3] = lastPoint( pa[2] );
    }
    else {
	pa[1] = QPoint( pa[0].x(), pa[0].y() - act->ratio() );    
	pa[2] = QPoint( (int)(act->x() + (act->ratio())*0.7), pa[0].y() - act->ratio() );
	pa[3] = QPoint( (int)(act->x() + (act->ratio())*0.7), (int)act->y() );
    }
        
}	

void QlgTransition::drawShape(QPainter &p)
{	
    p.setPen( QPen( col, lineWidth, style ) );
    p.drawCubicBezier( pa );
    p.setFont( inputFont );
    p.drawText( ( pa[1].x() + pa[3].x() ) / 2, ( pa[1].y() + pa[3].y() ) / 2, lab, -1, QPainter::Auto  );
    
    /*
    if( selected ) {
	p.setBrush( Qt::green );
	p.drawRect( pa[1].x(), pa[1].y(), 5, 5 );
	p.drawRect( pa[2].x(), pa[2].y(), 5, 5 );
    }
    */
	
}

QlgTransition::~QlgTransition() 
{
    
}

/************************************************************
  Search for the end point of the transition line, this point is over the radius
  of each state  
 *************************************************************/

QPoint QlgTransition::lastPoint( QPoint sPoint)
{	
    double Xc, Yc, Xt, Yt, R, H, L;
    double r = nex->width() * 0.5;
    int xAxis =1;
    int yAxis =1;
    
    Xc = nex->x() + r;
    Yc = nex->y() + r;

    if( ( sPoint.x() <= ( nex->x() + r) ) && ( sPoint.y() <= ( nex->y() + r ) ) ) {
	xAxis = 1;
	yAxis = 1;
    }
    else if ( ( sPoint.x() > ( nex->x() + r) ) && ( sPoint.y() <= ( nex->y() + r ) ) ) {
	xAxis = -1;
	yAxis = 1;
    }
    else if ( ( sPoint.x() <= ( nex->x() + r) ) && ( sPoint.y() > ( nex->y() + r ) ) ) {
	xAxis = 1;
	yAxis = -1;
    }
    else if ( ( sPoint.x() > ( nex->x() + r) ) && ( sPoint.y() > ( nex->y() + r ) ) ) {
	xAxis = -1;
	yAxis = -1;
    }
    
    Xt = ( Xc - sPoint.x() ) * xAxis;
    Yt = ( Yc - sPoint.y() ) * yAxis;
    R = sqrt( Xt*Xt + Yt*Yt );
    L = ( r * ( Xt / R ) )* xAxis;
    H = ( r * ( Yt / R ) ) * yAxis;
       
    return QPoint( (int)(Xc - L), (int)(Yc - H) );
}

void QlgTransition::drawArrow( int sx, int sy, int ex, int ey )
{
    double angle = computeAngle( sx, sy, ex, ey );
    
    int x, y;
       
   QWMatrix m;
       
   m.rotate(angle);
   m.map(-10, -5, &x, &y);
   pts.setPoint(0, x, y);
   m.map(-10, 5, &x, &y);
   pts.setPoint(1, x, y);
   m.map(0, 0, &x, &y);
   pts.setPoint(2, x, y);   
   arrow.setPoints(pts);    
   arrow.setBrush( col );   
   arrow.move( pa[3].x(), pa[3].y() );   
    
}

double QlgTransition::computeAngle(int sx, int sy, int ex, int ey)
{
   double run = ex - sx;
   double rise = ey - sy;
  
   double angle;
   if (run == 0)
      if (rise < 0)
         angle = 270;
      else
         angle = 90;
   else
   {
      angle = RAD2DEG * atan(rise/run);
      if (run < 0)
         angle += 180;
      if (rise < 0)
         angle += 360;
   }
   return angle;           
}

/*************************************************************
  Function that changes the style of the transition in function of  a number
*************************************************************/

void QlgTransition::setStyle( int s )
{
switch( s ) {
    case 0:
	style = Qt::SolidLine;
        break;
    case 1:
	style = Qt::DashLine;
        break;
    case 2:
	style = Qt::DotLine;
        break;
    case 3:
	style = Qt::DashDotLine;
        break;
    case 4:
	style = Qt::DashDotDotLine;
        break;
    default:
	style = Qt::SolidLine;
        break;
    }
}
