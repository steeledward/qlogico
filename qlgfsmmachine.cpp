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

/*****************************************************************
  File where is implemented the functionality of the FSM view
******************************************************************/  
#include <qmessagebox.h>
#include <qobject.h>

#include "qlgfsmmachine.h"
#include "qlgtransition.h"
#include "qlgtitle.h"
#include "qlgstate.h"
#include "qlgfsmoptions.h"

QlgFSMMachine::QlgFSMMachine(QCanvas* c, QlgFSMOptions * opt ) 
	: options( opt ),  canvas( c )
{
    QString value;
        
    for( int i=0; i<pow( 2, options->numMooreOutputs() ); i++) {
	value = value.setNum( i, 2 );
	value = value.rightJustify( options->numMooreOutputs(), '0' );  //Pre-append with the 0 character
	c_states.push_back( value );
    }    

    c_stateSelected = 0;
    c_transitionSelected = 0;
}

QlgFSMMachine::~QlgFSMMachine()
{
    
}

/**********************************************************
 Add a new c_state to the FSM
*********************************************************/

void QlgFSMMachine::addState( QStringList values )
{
    c_state = new QlgState( canvas, 
			  values[0],  // name
			  values[1],  // code
			  values[2].toInt(), // ratio
			  values[3].toInt(), // line width
			  QColor( values[4] ), // color
			  values[5] ); // description
    
    c_state->font = options->mooreOutsFont();
		    
    if( c_stateSelected )
	c_stateSelected->setSelected( false );
    c_stateSelected = c_state;
    
    canvas->setAllChanged();
    canvas->update(); 
    
    vUsedStates.append( c_state );
    QStringList::iterator it = c_states.begin();
    while(  *it != values[1] )
	++it;
    c_states.erase( it );
}

void QlgFSMMachine::addTransition( QStringList values )
{  
    QlgState* actual = 0;
    QlgState* next = 0;
    QString label = "";
    bool isOcupated = false;
    
    QString a = values[0];
    QString n = values[1];
    
    vUsedStates.first();
    
    while( vUsedStates.current()->value() != a )
	vUsedStates.next();
    actual =  vUsedStates.current();
    
    vUsedStates.first();
    
    while( vUsedStates.current()->value() != n )
	vUsedStates.next();
    next =  vUsedStates.current(); 
    
    label = values[2] + " / " + values[3];
    vTransitions.first();
    uint i = 0;
    while( ( i < vTransitions.count() ) && ( !isOcupated ) ) {
	if( ( vTransitions.current()->actual() == actual ) && ( vTransitions.current()->next() == next ) && ( vTransitions.current()->label() == label )  )
	    isOcupated = true;
	vTransitions.next();
	i++;	
    }
    
    if( isOcupated )
	QMessageBox::warning( 0, QObject::tr( "Mensaje de Error" ), QObject::tr( "La transicion solicitada ya esta asignada" ) );
    else {
	
	c_transition = new QlgTransition( canvas, actual, next, 
					values[2], values[3] );
	
	c_transition->inputFont = options->mealyInputsFont();
	c_transition->outputFont = options->mealyOutputsFont();
	
	if( c_transitionSelected )
	    c_transitionSelected->setSelected( false );
	c_transitionSelected = c_transition;
	
	vTransitions.append( c_transition );
	
	canvas->setAllChanged();
	canvas->update();
    }
}

void QlgFSMMachine::setValues( QlgFSMOptions *v )
{ 
    options = v;

    for( uint i = 0; i < vUsedStates.count(); i++ )
	vUsedStates.at(i)->font = options->mooreOutsFont();

    for( uint i = 0; i < vTransitions.count(); i++ ) {
	vTransitions.at(i)->inputFont = options->mealyInputsFont();
	vTransitions.at(i)->outputFont = options->mealyOutputsFont();
    }

    canvas->setAllChanged();
    canvas->update();
}

