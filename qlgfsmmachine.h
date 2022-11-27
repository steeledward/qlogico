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

#ifndef QLGFSMMACHINE_H
#define QLGFSMMACHINE_H

#include <qstring.h>
#include <qcanvas.h>
#include <qptrlist.h>
#include <qstringlist.h>

class QlgFSMOptions;
class QlgState;
class QlgTransition;

typedef QPtrList<QlgState> QlgStatesList;
typedef QPtrList<QlgTransition> QlgTransitionsList;

class QlgFSMMachine
{
public:    
    QlgFSMMachine(QCanvas* c, QlgFSMOptions * opt ) ;
    ~QlgFSMMachine();
    /// Returns the oprtions of the machine
    QlgFSMOptions* values(){ return options; };
    void setValues( QlgFSMOptions *v );
    /// Returns the states in use
    QlgStatesList& usedStates(){ return vUsedStates; };
    /// Returns all the transition in the machine
    QlgTransitionsList& transitions(){ return vTransitions; };
    /// Return the posible state codes for the machine
    QStringList& stateCodes(){ return c_states; };
    /// Return the posible transition codes for the machine
    QStringList transitionCodes(){ return c_transitions; };
    /// Returns the current state selected
    QlgState* stateSelected(){ return c_stateSelected; };
    /// Returns the current transition selected
    QlgTransition* transitionSelected(){ return c_transitionSelected; };
    /// Returns the state active
    QlgState* state(){ return c_state; };
    /// Changes the new active state
    void setState( QlgState* s ){ c_state = s; };
    /// Returns the transition in use
    QlgTransition* transition(){ return c_transition; };
    /// Changes the new active transition
    void setTransition( QlgTransition* t ){ c_transition = t; };
    /// Set the transition selected
    void setTransitionSelected( QlgTransition* ts ){ c_transitionSelected = ts; };
    /// Set  the current state selected
   void setStateSelected( QlgState* ss ){ c_stateSelected = ss; };
    
public slots:
    void addState( QStringList values );
    void addTransition( QStringList values );
    
    void updateState( QStringList values );                
    void updateTransition( QStringList values );    
    void updateFSM( QStringList values );

private:
    /// Properties of the machine
    QlgFSMOptions *options;
    /// List of the states in use
    QlgStatesList vUsedStates;
    /// Transitions in use
    QlgTransitionsList vTransitions;
    /// The place to draw the machine
    QCanvas *canvas;
    /// The only permited state codes for the machine
    QStringList c_states;
    /// The only permited transition codes for the machine
    QStringList c_transitions;    
    /// State selected in the view
    QlgState* c_stateSelected;
    /// Transition selected in the view
    QlgTransition* c_transitionSelected;
    /// State in use
    QlgState* c_state;
    /// Transition in use
    QlgTransition* c_transition;
};
#endif
