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

#ifndef QLGCIRCUITLINE_H
#define QLGCIRCUITLINE_H

#include <qcanvas.h>
#include <qpoint.h>
#include <qpainter.h>
#include <qpointarray.h> 
	
static const int circuitLineRTTI = 984379;
class QlgCircuitComponent;
class QlgCircuitLine;
class QlgCircuit;

class QlgCircuitLine: public QCanvasLine
{
    
public:
    QlgCircuitLine( QlgCircuit *c, double x0, double y0, double x1, double y1 );
    
    void moveBy(double dx, double dy);
    void setInput( QlgCircuitComponent *, int );
    void setNumInput( int i ){ nInput = i; };
    void reset();
    
    bool isHorizontal();
    bool connectedAtBegin;
    bool connectedAtEnd;    
    bool connectedEndExact;
    bool connectedBeginExact;
    
    int numInput(){ return nInput; };
    int numOutput(){ return nOutput; };
    int rtti () const { return circuitLineRTTI; }
    
    QlgCircuitComponent* circuitInput(){ return cInput; };
    QlgCircuit *circuit(){ return mainCircuit; };
    
private:    
    int state;
    int id;
    int nInput;
    int nOutput;
    bool horizontal;

    QlgCircuit *mainCircuit;
    QlgCircuitLine *lineCollided;
    QlgCircuitComponent *circuitCollided;
    QlgCircuitComponent *cInput;
    QlgCircuitComponent *cOutput;
    
protected:
    void drawShape( QPainter & p );    
    
};

#endif
