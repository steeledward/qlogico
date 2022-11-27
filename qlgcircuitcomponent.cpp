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
#include <qstring.h>
#include <qpopupmenu.h>
#include <qtimer.h>

#include "qlgtimechartwindow.h"
#include "qlgmainwindow.h"
#include "qlgcircuitwindow.h"
#include "qlgcircuitview.h"
#include "qlgcircuitcomponent.h"
#include "qlgboolfunction.h"
#include "qlgcircuitline.h"
#include "qlgcircuit.h"

/// The constant width size of the image for the component
#define IMG_WIDTH 60
/// Default space beetwen each input or input in component
#define DY 20
/// The space beetewen the draw
#define SPACE 5

/********************************************************************************
 constructor for a circuit component
  we need to build a new circuit component:
 QCanvas canvas: where place the circuit
 QString Type : to know what ctype of component is ( and, or, not, nand..)
 vector<CircuitComponent> vInputs :  a vector thaht contains the inputs for the new component
 QString Function : what boolean function have the component
QString label : the label that the user can see in the screen ( optional)
*********************************************************************************/

QlgCircuitComponent::QlgCircuitComponent( QlgCircuit *c, QString n_image, int numIn, QStringList vFcns, QString lab )
           :  QCanvasRectangle(c->canvas() ), label(lab, c->canvas() ), img(QPixmap::fromMimeSource(n_image).convertToImage())
{    
    vBitInputs = 0;
    vBitOutputs = 0;
    vPosOutputs = 0;
    
    delay= new QTimer( this);
    t_delay = 0;
	    
    mainCircuit = c;
    
    isoff = true;
    showSt = false;
    
    ctype = n_image;
    
    move(0,0);     
    if( label.text() == "" )
	label.setText( ctype );
    else
	label.show();
   
    vFunctions = vFcns;
    
    vBitOutputs = new int[vFunctions.size()];
               
     for( uint i = 0; i < vFunctions.size(); i++ )	 
	 vBitOutputs[i] = 0;               
    
    setNumInputs( numIn );
    
    setZ( 1 );
    
    QObject::connect( delay, SIGNAL(timeout()), this, SLOT(sendValue()) );
    
}

void QlgCircuitComponent::setNumInputs( int numIn )
{
    if( numIn == 0 )
	num_Inputs = 1;
    else
	num_Inputs = numIn;
    
    resize();
    
    if( vBitInputs )
	delete[] vBitInputs;
    
    vBitInputs = new int[num_Inputs];
    
    if( vPosOutputs )
	delete[] vPosOutputs;
    
    vPosOutputs = new int[num_Inputs];
    
    resetInputs();

}

void QlgCircuitComponent::setFunctions( QStringList vFcns )
{
    vFunctions = vFcns;    
    
    resize();
    
    if( vBitOutputs )
	delete[] vBitOutputs;
    
    vBitOutputs = new int[vFunctions.size()];
               
     for( uint i = 0; i < vFunctions.size(); i++ )	 
	 vBitOutputs[i] = 0;          
}

void QlgCircuitComponent::resize()
{
    if( uint(num_Inputs) == vFunctions.size() == 1 ) {
	dyi = dyo = DY / 2;
	img = img.smoothScale( IMG_WIDTH / 2, 2 * dyi );
    }
    else if( uint(num_Inputs) == vFunctions.size() ) {
	dyi = dyo = DY;
	img = img.smoothScale( IMG_WIDTH, ( num_Inputs + 1 ) * dyi );
    }
    else if( uint(num_Inputs) > vFunctions.size() ) {
	dyi = DY;		
	img = img.smoothScale( IMG_WIDTH, ( num_Inputs + 1 ) * dyi );	
	dyo = img.height() / ( vFunctions.count() + 1 );
    }
    else {
	dyo = DY;	
	img = img.smoothScale( IMG_WIDTH, ( vFunctions.count() + 1 ) * dyo );
	dyi = img.height() / ( num_Inputs + 1 );
    }
    
    setSize( img.width() + ( 2 * SPACE ), img.height() );
#if !defined(Q_WS_QWS)
    pix.convertFromImage(img, OrderedAlphaDither);
#endif
    label.move( x(), y() + height() + 5 );
    
}

void QlgCircuitComponent::moveBy(double dx, double dy )
{    
    QCanvasRectangle::moveBy( dx, dy );
    label.moveBy( dx, dy );
}

/*********************************************************
  Returns the position of the input of the component
********************************************************/

double QlgCircuitComponent::inputX()
{    
    return x();
}

double QlgCircuitComponent::inputY( int in )
{    
    return ( y() + dyi*(in+1) );
}
/*********************************************************
  Returns the position of the output of the component
********************************************************/

double QlgCircuitComponent::outputX()
{
    return(  x() + width() );
}

double QlgCircuitComponent::outputY( int out )
{
    return ( y() + dyo*(out+1) );
}

void QlgCircuitComponent::setImage( const QPixmap *p )
{
    img = *p;
    
#if !defined(Q_WS_QWS)
    pix.convertFromImage(img, OrderedAlphaDither);
#endif
}

void QlgCircuitComponent::drawShape( QPainter &p )
{
    // On Qt/Embedded, we can paint a QImage as fast as a QPixmap,
    // but on other platforms, we need to use a QPixmap.
   
#if defined(Q_WS_QWS)
    p.drawImage( int(x())+SPACE, int(y()), img, 0, 0, -1, -1, OrderedAlphaDither );
#else
    p.drawPixmap( int(x())+SPACE, int(y()), pix );
#endif
    
    // What color to use depending on if it is selected or not the component
    
    
    p.setPen( QPen( Qt::red, 1 ) );
    
    if( ctype != "entrada" ) {
	for( int i = 0; i < num_Inputs; i++ )
		p.drawLine( int(inputX())-SPACE, int(inputY(i)), int(x())+SPACE, int(inputY(i)) );
    }
    
    for( uint i = 0; i < vFunctions.size(); i++ ) {
	if( ctype != "salida" )
	    p.drawLine( int(outputX())-SPACE, int(outputY(i)), int(outputX())+SPACE, int(outputY(i)) );
	if( ctype == "entrada" || ctype == "salida" || showSt ) {
	    if( vBitOutputs[i] )
		p.setBrush( Qt::green );
	    else
		p.setBrush( Qt::red );
	    p.drawRect( int(outputX()) - 20, int(outputY(i)) - 2,  5, 5 );
	}
    }
    
    if( isSelected() ) {
	p.setBrush( Qt::NoBrush );
	p.setPen( QPen( Qt::magenta, 1, Qt::DotLine ) );
	label.setColor( Qt::magenta );
	p.drawRect( int(x()), int(y()) - 10, width(), height() + 20 );
    }
    else
	label.setColor( mainCircuit->view()->lineColor() );
    
}

void QlgCircuitComponent::resetInputs()
{
    vInputs.clear();

    for( int i = 0; i < num_Inputs; i++ ) {
	vInputs.append( 0 );
	vPosOutputs[i] = 0;
	vBitInputs[i] = 0;
    }
}

/*************************************************************
  Connect the actul circuit component with another circuit component 
  @ input - new circuit component
  @ numInput - the number of input of this circuit
  @ numInput - the output of the new circuit
*************************************************************/

void QlgCircuitComponent::connect( QlgCircuitComponent *input, int numInput, int numOutput )
{
    if( input ) {
	vInputs.replace( numInput, input );
	vPosOutputs[numInput] = numOutput;
    }
    else
	disconnect( numInput );
}

void QlgCircuitComponent::disconnect( int numInput )
{
    vInputs.replace( numInput, 0 );
    vPosOutputs[numInput] = 0;
}

/************************************************************
  Evaluate for an array of bit @ ba the function located on vFunctions at
  position @ out
************************************************************/

void QlgCircuitComponent::evaluate( QString b )
{
    bits = b;
    
    if( t_delay )
	delay->start( t_delay, TRUE );
    else
	sendValue();

}

void QlgCircuitComponent::sendValue()
{
    QString oper;
    int result = 0;
    
    for( uint j = 0; j < vFunctions.count(); j++ ) {
	oper = vFunctions[j];
	result = 0;
	if( oper.length() == 1 ) {
	    if( oper == "*" ){
		if( bits.find("0") == -1 )
		    result = 1;
		else
		    result = 0;
	    }
	    else if( oper == "+" ) {
		if( bits.find("1") == -1 )
		    result = 0;
		else
		    result = 1;
	    }
	    else if( oper == "'" ) {
		if( bits == "0" )
		    result = 1;
		else
		    result = 0;
	    }
	    else if( oper == "=" ) {
		result = bits.toInt();
	    }
	}
	else {
	    result = QlgBoolFunction( oper ).evaluate(bits);
	}
	setBitOutput( j, result );
    }
    isoff = false;
}


void QlgCircuitComponent::setBitOutput( int pos, int bit )
{   
    if( uint(pos) < sizeof(vBitOutputs ) )
	vBitOutputs[pos] = bit;
    
    if( ctype == "entrada" || ctype == "salida" )
	mainCircuit->view()->window()->window()->timeChart()->draw( label.text(), bit );
}

QlgCircuitComponent* QlgCircuitComponent::input( int i )
{
    if( uint(i) < vInputs.count() )
	return vInputs.at(i);
    else
	return 0;
}

QString QlgCircuitComponent::function( int i )
{
    if( uint(i) < vFunctions.count() )
	return vFunctions[i];
    else
	return "";
}

void QlgCircuitComponent::setShowState( bool st )
{
    showSt = st;
}

/*********************************************************
  Destructor
*********************************************************/

QlgCircuitComponent::~QlgCircuitComponent()
{
    delete[] vBitInputs;
    delete[] vBitOutputs;
    delete[] vPosOutputs;
}
