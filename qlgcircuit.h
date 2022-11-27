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

#ifndef QLGCIRCUIT_H
#define QLGCIRCUIT_H

#include <qobject.h> 
#include <qptrlist.h>
#include <qthread.h>

#include "qlgboolfunction.h"

class QlgCircuitComponent;
class QString;
class QCanvas;
class QPoint;
class QlgCircuit;
class QlgCircuitView;

typedef QPtrList<QlgCircuitComponent> QlgCircuitCompList;
typedef QPtrList<QlgCircuit> QlgCircuitList;
typedef QPtrList<QlgBoolFunction> QlgBoolFunctionList;

class QlgCircuit : public QObject
{
    Q_OBJECT
    
public:
    QlgCircuit( QlgCircuitView *c, QlgBoolFunction* boolFunction, QPoint pos, bool  );
    QlgCircuit( QlgCircuitView *c, QlgBoolFunctionList boolFunctionL, int, int, QPoint pos, bool );
    QlgCircuit( QlgCircuitView *c );
    ~QlgCircuit();
    
    /// Returns the bool function of the circuit
    QlgBoolFunction* function(){ return fcn; };
    /// Returns the vector of the inputs of the circuit
    QlgCircuitCompList& inputVariables(){ return vInputs; };
    /// Returns the vector of the outputs of the circuit    
    QlgCircuitCompList& outputVariables(){ return vOutputs; };
    /// Returns a specific output of the circuit        
    QlgCircuitComponent* outputVariables( int index ){ return vOutputs.at(index); };   
    QlgCircuitView* view(){ return cView; };
    /// Returns the cir_canvas of the circuit
    QCanvas *canvas(){ return cir_canvas; };
    
    void addComponent( QString name, QPoint pos );
    void removeComponent( QlgCircuitComponent * );
    void setAutomatic( bool a){ automatic=a; };
    
    bool isAutomatic(){ return automatic; };
    
    /// Returns x position of the circuit upper left
    int x0(){ return xa; };
    /// Returns y position of the circuit upper left
    int y0(){ return ya; };
    /// Returns x position of the circuit down right
    int x1(){ return xb; };
    /// Returns y position of the circuit down right
    int y1(){ return yb; };    
    /// Returns width of the circuit
    int width(){ return xb-xa; };
    int height(){ return yb-ya; };    
    
public slots:    
    void updateCircuitInputs( QString vars, QString value );
    
private:    
    /// Bool functions of the main circuit
    QlgBoolFunction* fcn;
    /// Vector containing each inverted term contained in the bool function    
    QlgCircuitCompList vInvTerms;
    /// Vector containing each term contained in the bool function    
    QlgCircuitCompList vBinTerms;
    /// Vector containing the OR components 
    QlgCircuitCompList vSumMinTerms;
    /// Vector containing the inputs of the main circuit
    QlgCircuitCompList vInputs;
    /// Vector containing the outputs of the main circuit    
    QlgCircuitCompList vOutputs;
    /// Vector containing the outputs of the main circuit
    QlgCircuitCompList vJKFF;
    /// Vector containning the subcircuits
    QlgCircuitList vCircuits;
    /// The three different type of components for construct the circuit
    QlgCircuitComponent* node;
    QlgCircuitComponent* gate;
    QlgCircuitComponent* inv;
    /// The place where the circuit is drawed
    QCanvas *cir_canvas;
    /// The x position where the first component circuit began    
    int xa;
    /// The y position where the first component circuit began        
    int ya;
    /// The x position where the last component circuit end
    int xb;
    /// The y position where the last component circuit end
    int yb;
    /// The relative x distance between the connection over the input
    int xic_rel;
private:
    void connectComponent( QlgCircuitComponent* circuit );
    void alignateMe( QlgCircuitComponent* gate, int input );
    void turnOff( QlgCircuitCompList vComponents );    
    void connectBinTermAnd();
    void connectBinTermOr();
    void addOutput();
    void connectJKFF();    
    
    int updateCircuit( QlgCircuitComponent* circuit, int out );    
    
    bool automatic;
	    
    QlgCircuitComponent* getCircuitInput( QString term );
    QlgCircuitView *cView;
};
#endif
