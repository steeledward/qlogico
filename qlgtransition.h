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

#ifndef QLGTRANSITION_H
#define QLGTRANSITION_H

#include <qcanvas.h>
#include <qpointarray.h>
 #include <math.h>

#include "qlgstate.h"

#define RAD2DEG 57.2958

static const int transtitionRTTI = 984379;

class QlgTransition: public QCanvasSpline
{
public:        
    QlgTransition( QCanvas* canvas, QlgState* a, QlgState* n, QString in, QString out );
    ~QlgTransition();
    void updateConections();
    void drawArrow( int sx, int sy, int ex, int ey );
    /// Run Time True Information
    int rtti () const { return transtitionRTTI; }
    /// Returns true if the transition is selected
    bool isSelected(){ return selected; };
    /// Set the active mode for the transition
    void setSelected( bool state ){ selected = state; };
    /// Returns the points of the transition
    QPointArray points(){ return pa; };
    int width(){ return lineWidth; };
    /// Returns the color of the transition
    QColor color(){ return col; };
    /// Changes the line width
    void setWidth( int w ){ lineWidth = w; };
    /// Changes the color of the transition
    void setColor( QColor c ){ col = c; };
    /// Changes the style of the line for the transition
    void setStyle( int s );
    /// Returns the labe of the transition
    QString label(){ return lab; };
    /// Returns the actual state
    QlgState* actual(){ return act; };
    /// Returns the next state
    QlgState* next(){ return nex; };
    /// Returns the input value of the transition
    QString input(){ return c_input; };
    /// Returns the output value of the transition
    QString output(){ return c_output; };
    /// Font for the input transition
    QFont inputFont;
    /// Font for the output transition
    QFont outputFont;
    
private:
    double computeAngle(int sx, int sy, int ex, int ey);  
    QPoint lastPoint( QPoint sPoint );
    /// To check if the transition is selected
    bool selected;    
    int lineWidth;
    PenStyle style;
    QColor col;	    
    QlgState* act;
    QlgState* nex;    
    /// The current input
    QString c_input;
    /// The current input
    QString c_output;        
    QString lab;
    /// Array of points to draw the transition
    QPointArray pa;
    QPointArray pts;
    QCanvasPolygon arrow;
	    
protected:    
    void drawShape( QPainter & );
    
};

#endif

