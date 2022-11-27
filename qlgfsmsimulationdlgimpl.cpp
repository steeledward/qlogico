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

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qtimer.h>
#include <qlabel.h>
#include <qmessagebox.h>

#include "qlgfsmsimulationdlgimpl.h"
#include "qlgfsmview.h"
#include "qlgstate.h"
#include "qlgtransition.h"
#include "qlgfsmmachine.h"

QlgFSMSimulationDlgImpl::QlgFSMSimulationDlgImpl( QWidget *parent )
	: QlgFSMSimulationDlg( parent, "FSM Simulation", Qt::WType_Dialog  )
{
    fsmView = 0;
    
    setFixedSize( size() );  
    
    connect( (QObject*)(comboBoxState), SIGNAL( activated( int ) ), this, SLOT( updateState( int ) ) );
    connect( (QObject*)(comboBoxInput), SIGNAL( activated( int ) ), this, SLOT( updateInput( int ) ) );
    connect( (QObject*)(comboBoxOutput), SIGNAL( activated( int ) ), this, SLOT( updateOutput( int ) ) );
}

void QlgFSMSimulationDlgImpl::update( QlgFSMView *fsmViewNew )
{
    fsmView = fsmViewNew;
    if( fsmView )	
	init();    
}

/*************************************************************
  Initialize the fsm simulation dlg
*************************************************************/

void QlgFSMSimulationDlgImpl::showDlg()
{
    if( fsmView ) {
	init();
	show();
    }
    else
	QMessageBox::warning( this, "QLogico", tr("Seleccione antes la Maquina de Estados Finitos que desea simular") );
}

void QlgFSMSimulationDlgImpl::init()
{
    if( fsmView->fsmMachine->usedStates().count() > 0 ) {
	setEnabled( true );
	comboBoxState->clear();
    
	for( QlgState* st = fsmView->fsmMachine->usedStates().first(); st; st = fsmView->fsmMachine->usedStates().next() )
	    comboBoxState->insertItem( st->value() );
	updateState( 0 );    
    }
    else
	setEnabled( false );
}

/*************************************************************
  Get the index of the combo box that contain the state to be selected in the
  fsm view.
*************************************************************/

void QlgFSMSimulationDlgImpl::updateState( int index )
{
    if( index > -1 ) {
	fsmView->fsmMachine->setState( fsmView->fsmMachine->usedStates().at(index) );
	if( fsmView->fsmMachine->stateSelected() != fsmView->fsmMachine->state() ) {
	    fsmView->fsmMachine->stateSelected()->setSelected( false );
	    fsmView->fsmMachine->state()->setSelected( true );
	    fsmView->fsmMachine->setStateSelected( fsmView->fsmMachine->state() );
	    textLabelStateName->setText( fsmView->fsmMachine->stateSelected()->name() );
	    fsmView->canvas()->setAllChanged();
	    fsmView->canvas()->update();  
	    comboBoxInput->clear();
	    comboBoxOutput->clear();
	    for( uint i=0; i<fsmView->fsmMachine->transitions().count(); i++) {
		if( fsmView->fsmMachine->transitions().at(i)->actual() == fsmView->fsmMachine->stateSelected() ) {
		    comboBoxInput->insertItem( fsmView->fsmMachine->transitions().at(i)->input() );
		    comboBoxOutput->insertItem( fsmView->fsmMachine->transitions().at(i)->output() );
		}
	    }	
	}    
    }
}

/*************************************************************
  Get the index of the combo box that contain the input to be applied
  to the  fsm 
*************************************************************/

void QlgFSMSimulationDlgImpl::updateInput( int index )
{
    if( (index > -1) && ( comboBoxOutput->currentText() != "" ) ) {
	int st = -1;
	QString label = comboBoxInput->currentText() + " / " +  comboBoxOutput->currentText();
	for( uint i=0; i<fsmView->fsmMachine->transitions().count(); i++) {
	    if( (fsmView->fsmMachine->transitions().at(i)->label() == label ) && ( fsmView->fsmMachine->transitions().at(i)->actual() == fsmView->fsmMachine->stateSelected() ) ) {
		fsmView->fsmMachine->transitions().at(i)->setWidth( fsmView->fsmMachine->transitions().at(i)->width() * 3 );
		fsmView->canvas()->setAllChanged();	    
		fsmView->canvas()->update();
		QTimer twait;
		twait.start( 10000, true );
		for( uint j=0; j<fsmView->fsmMachine->usedStates().count(); j++) {
		    if( fsmView->fsmMachine->usedStates().at(j) == fsmView->fsmMachine->transitions().at(i)->next() ) {
			st = j;
			j = fsmView->fsmMachine->usedStates().count();
		    }
		}
		fsmView->fsmMachine->transitions().at(i)->setWidth( fsmView->fsmMachine->transitions().at(i)->width() / 3 );
		fsmView->canvas()->setAllChanged();	    
		fsmView->canvas()->update();  		
		i = fsmView->fsmMachine->transitions().count();
	    }
	}	
	if( st != -1 ) {
	    updateState( st );
	    comboBoxState->setCurrentItem( st );
	}
    }
}

/*************************************************************
  Get the index of the combo box that contain the output to be applied
  to the  fsm 
*************************************************************/

void QlgFSMSimulationDlgImpl::updateOutput( int index )
{
    if( (index > -1) && ( comboBoxInput->currentText() != "" ) ) {
	int st = -1;
	QString label = comboBoxInput->currentText() + " / " +  comboBoxOutput->currentText();
	for( uint i=0; i<fsmView->fsmMachine->transitions().count(); i++) {
	    if( (fsmView->fsmMachine->transitions().at(i)->label() == label ) && ( fsmView->fsmMachine->transitions().at(i)->actual() == fsmView->fsmMachine->stateSelected() ) ) {
		fsmView->fsmMachine->transitions().at(i)->setWidth( fsmView->fsmMachine->transitions().at(i)->width() * 3 );
		fsmView->canvas()->setAllChanged();	    
		fsmView->canvas()->update();  	
		QTimer twait;
		twait.start( 6000, true );		
		for( uint j=0; j<fsmView->fsmMachine->usedStates().count(); j++) {
		    if( fsmView->fsmMachine->usedStates().at(j) == fsmView->fsmMachine->transitions().at(i)->next() ) {
			st = j;
			j = fsmView->fsmMachine->usedStates().count();
		    }
		}
		fsmView->fsmMachine->transitions().at(i)->setWidth( fsmView->fsmMachine->transitions().at(i)->width() / 3 );
		fsmView->canvas()->setAllChanged();
		fsmView->canvas()->update();
		i = fsmView->fsmMachine->transitions().count();
	    }
	}	
	if( st != -1 ) {
	    updateState( st );
	    comboBoxState->setCurrentItem( st );
	}
    }
}

QlgFSMSimulationDlgImpl::~QlgFSMSimulationDlgImpl()
{
    
}
