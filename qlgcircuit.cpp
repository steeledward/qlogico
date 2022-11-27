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

#include <math.h>
#include <qstring.h>

#include "qlgcircuitview.h"
#include "qlgcircuit.h"
#include "qlgcircuitcomponent.h"
#include "qlgcircuitline.h"

#define X_REL 60
#define Y_REL 80

/*************************************************************************************
Constructor. Creates a new circuit.
@ cir_canvas - to place the circuit
@ boolFunction - the bool function to draw
@ pos - where to place the circuit
@ X_REL - relative distance between element in the x axis
@ Y_REL - relative distance between element in the y axis
**************************************************************************************/

QlgCircuit::QlgCircuit( QlgCircuitView* c, QlgBoolFunction* boolFunction, QPoint pos, bool a )
    : cir_canvas( c->canvas() ), automatic( a ), cView( c )
{
    xa = xb = pos.x();
    ya = yb = pos.y();    
    
    if( boolFunction->text() != "" ) {
	fcn = boolFunction;
	xb = ( fcn->numBinTerms() * 10 ) + xa + X_REL;
	xic_rel = xb - 10;
	QStringList vFcns;
	vFcns.append( "=" );
	
	// Create the input components for the main circuit

	for( QStringList::Iterator variable = fcn->variables().begin(); variable != fcn->variables().end(); ++variable ) {
	    node = new QlgCircuitComponent( this, "entrada", 0, vFcns, *variable );
	    node->move( xa, ya );
	    vInputs.append( node );
	    ya = ya + Y_REL;
	}
	
	QlgCircuitComponent* input;
	QStringList fcns;
	QStringList binTerms;
	double bterm = 0;
	double res = 0;
	
	/// Loop to draw each minterm of the function
	for( int i =0; i < fcn->numMinTerms(); i++ ) {
	    binTerms = fcn->binTerms( i );
	    /// Loop to draw each term of the minterm
	    res = modf( binTerms.count()*0.5, &bterm );
	    if( res )
		bterm = binTerms.count()-1;
	    else
		bterm = binTerms.count();
	    for( int j = 0; j < bterm; j=j+2 ) {
		fcns.clear();
		fcns.append( "*" );
		gate = new QlgCircuitComponent( this, "and", 2, fcns, "" );
		gate->move( xb + X_REL, yb );
		gate->show();		
		input = getCircuitInput( binTerms[j] );
		if( input->type() == "not" ) {
		    input->move( xb, yb + gate->dYi() - input->dYo() );
		    input->show();		    
		    if( !( input->input(0)->isVisible() ) ) {
			input->input(0)->move( xa, input->y() + input->dYi() - input->input(0)->dYo()  );
			input->input(0)->show();
		    }
		    connectComponent( input );
		} else if( !( input->isVisible() ) ) {
		    input->move( xa, yb + gate->dYi() - input->dYo()  );
		    input->show();
		}
		yb = yb + Y_REL;
		gate->connect( input, 0, 0 );
		input = getCircuitInput( binTerms[j+1] );
		if( input->type() == "not" ) {
		    input->move( xb, yb );
		    input->show();		    
		    if( !( input->input(0)->isVisible() ) ) {
			input->input(0)->move( xa, input->y() + input->dYi() - input->input(0)->dYo()  );
			input->input(0)->show();
		    }
		    connectComponent( input );
		} else if( !( input->isVisible() ) ) {
		    input->move( xa, yb );
		    input->show();
		}
		yb = yb + Y_REL;
		gate->connect( input, 1, 0 );
		connectComponent( gate );
		vBinTerms.append( gate );		
		if( bterm > 2 && j > 0 )
		    connectBinTermAnd();
	    }
	    if( res && binTerms.count() > 1 ) {
		fcns.clear();
		fcns.append( "*" );
		gate = new QlgCircuitComponent( this, "and", 2, fcns, "" );
		gate->connect( vBinTerms.getLast(), 0, 0 );
		input = getCircuitInput( binTerms[binTerms.count()-1] );
		gate->connect( input, 1, 0 );				
		if( input->type() == "not" ) {
		    alignateMe( gate, 0 );
		    input->move( xb, yb + gate->dYi() - input->dYo() );
		    gate->show();		    
		    input->show();
		    if( !( input->input(0)->isVisible() ) ) {
			input->input(0)->move( xa, input->y() + input->dYi() - input->input(0)->dYo()  );
			input->input(0)->show();
		    }
		    connectComponent( input );
		}
		else {
		    gate->move( gate->input(0)->outputX() + X_REL, yb );
		    gate->show();
		    if( !( input->isVisible() ) ) {
			input->move( xa, yb + 2*gate->dYi() - input->dYo()  );
			input->show();
		    }
		}		
		connectComponent( gate );
		vBinTerms.append( gate );
		yb = yb + Y_REL;
	    } else if( res && binTerms.count() == 1 ) {
		input = getCircuitInput( binTerms[0] );
		if( input->type() == "entrada" ) {
		    fcns.clear();
		    fcns.append( "=" );
		    gate = new QlgCircuitComponent( this, "node", 1, fcns, "" );
		    gate->connect( input, 0, 0 );
		    input = gate;
		}		
		input->move( xb, yb );
		input->show();
		if( !( input->input(0)->isVisible() ) ) {
		    input->input(0)->move( xa, input->y() + input->dYi() - input->input(0)->dYo()  );
		    input->input(0)->show();
		}
		connectComponent( input );		
		vBinTerms.append( input );
		yb = yb + Y_REL;
	    }
	    if( fcn->numMinTerms() > 1 ) {
		vSumMinTerms.append( vBinTerms.getLast() );
		connectBinTermOr();
	    }
	}
	addOutput();	
    }
}

/**************************************************************
  Crates a new sequential circuit using JK Flip-Flops type
  @ c - place to draw the circuit
  @ vFunctions - A list with the flip-flop inputs and outputs
  @ numFF - The number of flip flops to use
  @ numOutputs - the number of outputs of the circuit
  @ pos - position where the complete circuit begin
  @ xr - relative distance between each element in the x axis
  @ yr - relative distance between each element in the y axis
***************************************************************/ 

QlgCircuit::QlgCircuit( QlgCircuitView *c, QlgBoolFunctionList vFunctions, int numFF, int numOutputs, QPoint pos, bool a )
    : cir_canvas( c->canvas() ), automatic( a ), cView( c )
{    
    xa = xb = pos.x();
    ya = yb = pos.y();

    QlgCircuit *circuit;
    double intg;
    for( int i = 0; i < 2*numFF; i++ ) {
	circuit = new QlgCircuit( c, vFunctions.at(i), QPoint(xa,yb), true );
	yb = circuit->y1();
	vCircuits.append( circuit );
	if( modf( (i+1)*0.5, &intg ) == 0 )
	    connectJKFF();
    }    
    cir_canvas->resize( xb, yb );
}

QlgCircuit::QlgCircuit( QlgCircuitView *c )
    : cir_canvas( c->canvas() ), automatic( false ), cView( c )
{ 
    
}
/**************************************************************
  Function that connects the outputs of the 2 last circuits in vCircuits
  using a JK Flip Flop
**************************************************************/  

void QlgCircuit::connectJKFF()
{
    if( vCircuits.count() > 1 ) {
	QStringList vFcns;
	vFcns.append( "ffjk0" );
	vFcns.append( "ffjk1" );
	gate = new QlgCircuitComponent( this, "jkff", 3, vFcns, "" );	
	gate->connect( vCircuits.at(vCircuits.count()-2)->outputVariables().getFirst(), 0, 0 );	
	gate->connect( vCircuits.getLast()->outputVariables().getFirst(), 2, 0 );	
	alignateMe( gate, 0 );
	connectComponent( gate );
	gate->show();
	vJKFF.append( gate );
	if( xb < gate->x() )
	    xb = int(gate->x()) + gate->width() + X_REL;
    }
}

/************************************************************
  Function that connect the last two components of the vector vBinTerms
  using an AND GATE component 
************************************************************/

void QlgCircuit::connectBinTermAnd()
{
    if( vBinTerms.count() > 1 ) {
	QStringList fcns;
	fcns.append( "*" );
	gate = new QlgCircuitComponent( this, "and", 2, fcns, "" );
	gate->connect( vBinTerms.at(vBinTerms.count()-2), 0, 0 );
	gate->connect( vBinTerms.getLast(), 1, 0 );
	alignateMe( gate, 0 );
	connectComponent( gate );
	gate->show();	
	vBinTerms.append( gate );
    }
}

/***************************************************************
  Function that connect the last two components of the vector vSumMinTerms
  using an OR GATE component 
***************************************************************/

void QlgCircuit::connectBinTermOr()
{
    if( vSumMinTerms.count() > 1 ) {
	QStringList fcns;
	fcns.append( "+" );
	gate = new QlgCircuitComponent( this, "or", 2, fcns, "" );
	gate->connect( vSumMinTerms.at(vSumMinTerms.count()-2), 0, 0 );
	gate->connect( vSumMinTerms.getLast(), 1, 0 );
	alignateMe( gate, 0 );
	connectComponent( gate );
	gate->show();	
	vSumMinTerms.append( gate );
    }
}

/************************************************************
  Function that alignate  the @ gate @ input with the @ output
************************************************************/

void QlgCircuit::alignateMe( QlgCircuitComponent* gate, int input )
{
    double x = 0;
    double y;
    
    for( int i = 0; i < gate->numInputs(); i++ ){
	if( gate->input(i) ) {
	    if( x < gate->input(i)->outputX() )
		x = gate->input(i)->outputX();
	}
    }
    x = x + X_REL;
    y = gate->input(input)->outputY( gate->posOutput(input)) -  gate->dYi();
    gate->move( x, y );
}


/*****************************************************************************
 Return a vector pointer to the circuit component for that input name
*****************************************************************************/

QlgCircuitComponent* QlgCircuit::getCircuitInput( QString term )
{    
    QString nterm;
    
    if( term.endsWith( "'" ) )
	nterm = term.left( term.length()-1);
    else
	nterm = term;
    
    vInputs.first();
    while ( vInputs.current()->label.text() != nterm )
	vInputs.next();
    
    if( term.endsWith( "'" ) ) {
	QStringList fcns;
	fcns.append( "'" );
	QlgCircuitComponent *input = new QlgCircuitComponent( this, "not", 1, fcns, "" );
	input->connect( vInputs.current(), 0, 0 );
	vInvTerms.append( input );
	return input;
    }
    
     return  vInputs.current();
}

/********************************************************************************************************
 Connect the desired circuit component with all his inputs using a cable
**********************************************************************************************************/   

void QlgCircuit::connectComponent( QlgCircuitComponent* circuit )	
{
    QlgCircuitLine* h_cable;
    QlgCircuitLine* v_cable;
    double px0, py0, pxm, px1, py1;
    QlgCircuitComponent *ci;   
    
    for( uint i = 0; i < circuit->inputs().count(); i++ ) {
	if( circuit->inputs().at(i) ) {
	    ci = circuit->inputs().at(i);
	    px0 = ci->outputX();
	    py0 = ci->outputY(circuit->posOutput(i));
	    px1 = circuit->inputX();
	    py1 = circuit->inputY(i);
	    
	    if( py0 == py1 ) // Are in the same coordinate in y
		h_cable = new QlgCircuitLine( this, px0, py0, px1,  py1 );
	    else {
		if( py0 < py1 && ci->type() == "entrada" ) {
		    px0 = xic_rel;
		    v_cable = new QlgCircuitLine( this, px0, py0, px0, py1 );
		    h_cable = new QlgCircuitLine( this, px0, py1, px1, py1 );
		    xic_rel = xic_rel - 10;
		}
		else {
		    pxm = px1 -10;
		    v_cable = new QlgCircuitLine( this, px0, py0, pxm, py0 );
		    h_cable = new QlgCircuitLine( this, pxm, py0, pxm, py1 );
		    v_cable = new QlgCircuitLine( this, pxm, py1, px1, py1 );
		}
	    }
	}
    }
}

/******************************************************************************************************
  Function that add an output to the circuit
*******************************************************************************************************/

void  QlgCircuit::addOutput()
{    
    QStringList fcns;
    fcns.append( "=" );
    
    node = new QlgCircuitComponent( this, "salida", 1, fcns,  fcn->fdx() );

    if( !vSumMinTerms.isEmpty() )
	node->connect( vSumMinTerms.getLast(), 0, 0 );
    else
	node->connect( vBinTerms.getLast(), 0, 0 );
    
    alignateMe( node, 0 );

    node->show(); 
    
    connectComponent( node );
    
    vOutputs.append( node );
    
    xb = int(node->x()) + node->width() + X_REL;
}

/***************************************************************
  Function that updates the input of the circuit for a @ value
***************************************************************/

void QlgCircuit::updateCircuitInputs( QString vars, QString value )
{
    QStringList variables = QStringList::split( ",", vars );
    int j =0;
    
    for( QlgCircuitComponent *cir = vInputs.first(); cir; cir = vInputs.next() ) {
	j = variables.findIndex(cir->label.text() );
	if( value[j] == '1' )
	    cir->setBitOutput( 0, 1 );
	else
	    cir->setBitOutput( 0, 0 );
	cir->setOff( false );
    }
    turnOff( vInvTerms );
    turnOff( vBinTerms );
    turnOff( vSumMinTerms );
    turnOff( vOutputs );

    for( uint i = 0; i < vOutputs.count(); i++)
	updateCircuit( vOutputs.at(i), vOutputs.at(i)->posOutput(0) );
}

/************************************************************************************************
Recursive function that update the value of each circuit component 
 of the main circuit. The argument is the desired output
***********************************************************************************************/

int  QlgCircuit::updateCircuit( QlgCircuitComponent* circuit, int out )
{       
    if( !circuit )
	return 0;        
    
    int bit = 0;        
    
    if( circuit->isOff() ) {
	QString bitInputs = "";
	for( int count = 0; count < circuit->numInputs(); count++ )
	    bitInputs = bitInputs + QString::number( updateCircuit( circuit->input(count), circuit->posOutput(count)  ) );
		
	circuit->evaluate( bitInputs );
	bit = circuit->bitOutput(out);

    }
    else
	bit = circuit->bitOutput(out);
      
    return bit;
}

QlgCircuit::~QlgCircuit()
{
    
}

/*************************************************************
  set to off a subset of the main circuit contained in @ vComponents
*************************************************************/

void QlgCircuit::turnOff( QlgCircuitCompList vComponents )
{
    for( QlgCircuitComponent *cc = vComponents.first(); cc; cc = vComponents.next() )
	cc->setOff( true );
}

void QlgCircuit::addComponent( QString name, QPoint pos )
{
    int inputs = 2;
    QStringList vFcns;
    double x;
    double y;
	
    if( name == "entrada" || name == "salida" || name == "not" || name == "personalizable" )
	inputs = 1;
    if( name == "entrada" || name == "salida" || name == "personalizable" )
	vFcns.append( "=" );
    else if( name == "not" )
	vFcns.append( "'" );
    else if( name == "and" )
	vFcns.append( "*" );
    else if( name == "or" )
	vFcns.append( "+" );
    else if( name == "nand" )
	vFcns.append( "a'+b'" );
    else if( name == "nor" )
	vFcns.append( "a'*b'" );
    else if( name == "xor" )
	vFcns.append( "a*b'+a'*b" );
    else if( name == "xnor" )
	vFcns.append( "a*b+a'*b'" );
    
    modf( pos.x() * 0.1, &x );	
    modf( pos.y() * 0.1, &y );
    
    gate = new QlgCircuitComponent( this, name, inputs, vFcns, name );
    gate->move( x*10, y*10 );
    gate->show();
    cir_canvas->update();
    
    if( name == "entrada" )
	vInputs.append( gate );
    else if( name == "salida" )
	vOutputs.append( gate );
    else
	vBinTerms.append( gate );
}

void QlgCircuit::removeComponent( QlgCircuitComponent *cc )
{
    if( cc->type() == "entrada" ) {
	cc->hide();
	cc->label.hide();
	cc->resetInputs();
	vInputs.remove( cc );	
    }
}
