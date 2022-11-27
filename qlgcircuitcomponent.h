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

#ifndef QLGCIRCUITCOMPONENT_H
#define QLGCIRCUITCOMPONENT_H

#include <qcanvas.h>
#include <qimage.h>
#include <qptrlist.h> 
#include <qvaluelist.h> 
#include <qstringlist.h>
#include <qvaluevector.h>
#include <qobject.h>

class QPixmap;
class QlgSchematicLine;
class QlgCircuit;
class QTimer;

static const int circuitComponentRTTI = 984376;

class QlgCircuitComponent :  public QObject, public QCanvasRectangle
{    
    Q_OBJECT
    
public:
    QlgCircuitComponent( QlgCircuit *c, QString n_image, int numInputs, QStringList vFcns, QString label = 0  );
    ~QlgCircuitComponent();
    
    void connect( QlgCircuitComponent *input, int numInput, int numOutput );
    void disconnect( int numInput );
    void moveBy(double dx, double dy);
    void evaluate( QString bits );
    void setNumInputs( int );
    void setFunctions( QStringList );
    void setShowState( bool );          
    void setImage( const QPixmap* );
    void setBitOutput( int, int );
    void setOff( bool o ){ isoff = o; };
    void resetInputs();
    
    int numOutputs(){ return vFunctions.count(); };
    int numInputs(){ return num_Inputs; };
    int dYi(){ return dyi; };
    int dYo(){ return dyo; };
    int posOutput( int i ){ return vPosOutputs[i]; };
    int bitOutput( int j ){ return vBitOutputs[j]; };    
    //int posInput( int j ){ return vPosInputs[j]; };
    int rtti () const { return circuitComponentRTTI; }
    
    double inputX();
    double inputY( int );
    double outputX();
    double outputY( int );    
    
    bool showState(){ return showSt; };
    bool isOff(){ return isoff; };
    
    QPtrList<QlgCircuitComponent> inputs(){ return vInputs; };
    QlgCircuitComponent* input( int );
    QlgCircuit *circuit(){ return mainCircuit; };
    QPixmap pixmap(){ return pix; };
    /// The type of component
    QString type(){ return ctype; };
    QString function( int );
    /// The label for the component
    QCanvasText label;
    
private slots:
    void sendValue();
    
private:
    /// To kwno in what cirtcuit i am
    QlgCircuit *mainCircuit;
    /// Delay timer
    QTimer *delay;
    /// To know if the component is already activated
    bool isoff;
    /// The type of image for most environments
    QImage img;
    /// The type of image for  the embedded
    QPixmap pix;    
    /// The bit value for each input
    int *vBitInputs;
    /// The bit value for each output
    int *vBitOutputs;
    /// To know with what component  i am connected
    QPtrList<QlgCircuitComponent> vInputs;
    /// To know in what of the output of the component i am connected
    int *vPosOutputs;
    /// Vector with the boolean function for each output
    QStringList vFunctions;   
    /// Number of inputs for the circuit component
    int num_Inputs;
    /// To know if draw the state for the output values
    bool showSt;
    /// What type of component is
    QString ctype;    
    /// Value to evaluate the component
    QString bits;
    /// Relative distance between inputs
    int dyi;
    /// Relative distance between outputs
    int dyo;
    /// Time of delay
    int t_delay;
    
    QlgSchematicLine *lineCollided;
    
    virtual void resize();
    
protected:
    void drawShape( QPainter & );    
};
#endif
