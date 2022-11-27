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
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

#include "qlgaddtransitiondlgimpl.h"
#include "qlgfsmview.h"
#include "qlgstate.h"
#include "qlgtransition.h"
#include "qlgfsmmachine.h"
#include "qlgfsmoptions.h"

QlgAddTransitionDlgImpl::QlgAddTransitionDlgImpl( QWidget* parent )
	: QlgAddTransitionDlg( parent, "Add Transition Dialog" )
{
    fsmView = 0;
    connect(  (QObject*)(pushButtonCancel), SIGNAL(  clicked() ), this, SLOT(  hide()  ) );
    connect(  (QObject*)(pushButtonAddTransition), SIGNAL(  clicked() ), this, SLOT(  addTransition()  ) );
}

void QlgAddTransitionDlgImpl::showDlg()
{
    if( fsmView ) {
	update();    
	show();
    }
}

void QlgAddTransitionDlgImpl::update( QlgFSMView *fsmViewNew )
{    
    if( fsmView != fsmViewNew )
	fsmView = fsmViewNew;
}

// Fill the combo boxes with the posibles inputs / outputs

void QlgAddTransitionDlgImpl::update()
{   
    QString value;
    
    comboBoxInput->clear();
    comboBoxOutput->clear();
    comboBoxActualState->clear();
    comboBoxNextState->clear();    
    
    for( int i=0; i<pow( 2, fsmView->fsmMachine->values()->numMealyInputs() ); i++) {
	value = value.setNum( i, 2 );
	value = value.rightJustify( fsmView->fsmMachine->values()->numMealyInputs(), '0' );  //Pre-push_back with the 0 character	
	comboBoxInput->insertItem( value, -1 );
    }
    
    for( int i=0; i<pow( 2, fsmView->fsmMachine->values()->numMealyOutputs() ); i++) {
	value = value.setNum( i, 2 );
	value = value.rightJustify( fsmView->fsmMachine->values()->numMealyOutputs(), '0' );  //Pre-push_back with the 0 character	
	comboBoxOutput->insertItem( value );
    }

    for( QlgState* st = fsmView->fsmMachine->usedStates().first(); st; st = fsmView->fsmMachine->usedStates().next() ) {
	comboBoxActualState->insertItem( st->value() ); 
	comboBoxNextState->insertItem( st->value() );
    }
}

void QlgAddTransitionDlgImpl::addTransition()
{
    values.clear();
    values.push_back( comboBoxActualState->currentText() );
    values.push_back( comboBoxNextState->currentText() );
    values.push_back( comboBoxInput->currentText() );
    values.push_back( comboBoxOutput->currentText() );
    fsmView->fsmMachine->addTransition( values );
    hide();
}

QlgAddTransitionDlgImpl::~QlgAddTransitionDlgImpl()
{
    
}
