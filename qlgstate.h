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

#ifndef QLGSTATE_H
#define QLGSTATE_H

/**************************************************************************
  Declaration of the FSM components
****************************************************************************/
#include <qcanvas.h>
#include <qptrlist.h>
#include <qwidget.h>

static const int stateRTTI = 984378;
class QlgTransition;

class QlgState: public QCanvasRectangle
{   
public:   
    QlgState( QCanvas *canvas, QString name, QString value, int ratio, int lineWidth, QColor color, QString des );
    
    void moveBy(double dx, double dy);    
    /// Returns the identifier for the component
    int rtti () const { return stateRTTI; }
    /// Returns the vectorn containing the transition for this state
    QPtrList<QlgTransition>& transitions(){ return vTransitions; };
    /// Returns true if the state is selected
    bool isSelected(){ return selected; };
    /// Set the active mode for the state
    void setSelected( bool state ){ selected = state; };
    /// Returns the name of the state
    QString name(){ return s_name; };
    /// Returns the value code of the state
    QString value(){ return val; };
    /// Returns the ratio of the circle
    int ratio(){ return rat; };
    /// Returns the line width of the circunference
    int lineWidth(){ return lineW; };
    /// Returns the color of the state
    QColor color(){ return col; };
    // Returns the descriotion of the state
    QString description(){ return descrip; };
    // Changes the name of the state
    void name( QString n ){ s_name = n; };
    // Changes the ratio of the state
    void setRatio( int r ){ rat = r; };
    // Changes the line width of the state
    void setLineWidth( int lw ){ lineW = lw; };
    // Changes the color of the state
    void setColor( QColor c ){ col = c; };
    // Changes the description of the state
    void setDescription( QString d ){ descrip = d; };
    /// Font of the state
    QFont font;
    
protected:
    void drawShape( QPainter & );
    
private:
    /// Name of the state
    QString s_name;
    /// Value code for the state
    QString val;
    /// Color for the view of the state
    QColor col;
    /// Description of the state
    QString descrip;
    /// Ratio of the circle of the state
    int rat;
    /// Line width of the circunference 
    int lineW;
    /// To check if it is selected or not
    bool selected;
    // The transitions connected to this state
    QPtrList<QlgTransition> vTransitions;

};

#endif

