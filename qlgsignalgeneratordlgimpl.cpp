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
#include <qtimer.h>
#include <qcanvas.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlcdnumber.h>
#include <qmessagebox.h>

#include "qlgsignalgeneratordlgimpl.h"
#include "qlgcircuitwindow.h"
#include "qlgcircuitview.h"
#include "qlgcircuitcomponent.h"

QlgSignalGeneratorDlgImpl::QlgSignalGeneratorDlgImpl( QWidget *parent )
	: QlgSignalGeneratorDlg( parent, "Signal Generator", Qt::WType_Dialog ), circuitView( 0 )
{
    setFixedSize( size() );
    timer = new QTimer( this );
   
    connect( (QObject*)(timer), SIGNAL(timeout()), this, SLOT(nextStepAutomatic()) );
    connect( (QObject*)(spinBoxValue),  SIGNAL( valueChanged(int) ), this, SLOT( nextStep(int) ) );
    connect( (QObject*)(pushButtonStart),  SIGNAL( toggled( bool ) ), this, SLOT( stepAutomatic( bool ) ) );
    connect( (QObject*)(spinBoxFrecuency),  SIGNAL( valueChanged( int ) ), this, SLOT( stepAutomatic( int ) ) );
    connect( (QObject*)(pushButtonClose), SIGNAL( clicked() ), this, SLOT( hide() ) );       
}

/**********************************************************
 Initialize the properties of the signal generator
 in function of his inputs, in order to be used in the simulation
 of the circuit
**********************************************************/

void QlgSignalGeneratorDlgImpl::update( QlgCircuitView *circuitViewNew )
{
    circuitView = circuitViewNew;
    if( circuitView ) {
	setEnabled( true );
	init();	
    }
    else
	setEnabled( false );
}

void QlgSignalGeneratorDlgImpl::showDlg()
{
    if( circuitView ) {
	init();
	show();
    }
    else
	QMessageBox::warning( this, "QLogico", tr("Seleccione antes el Esquemático que desea simular") );
}

void QlgSignalGeneratorDlgImpl::init()
{
    QString value = "";
    QString txtVariables = "";
    QString separator = "";    

    //if( circuitView->circuit() && !circuitView->isLocked() )
    //circuitView->updateInputVariables( circuitView->circuit()->inputVariables() );
    circuitView->updateInputVariables();
    
    for( QlgCircuitComponent *variable = circuitView->inputVariables().first(); variable; variable = circuitView->inputVariables().next() ) {	
	txtVariables =  txtVariables + separator + variable->label.text();
	separator = ",";
	value = value + "0";
    }
    
    inputVariables->setText( txtVariables );
    inputNumbers->display( value );
    spinBoxValue->setMaxValue( (int)powl( 2, value.length() ) - 1 );
    spinBoxValue->setValue( 0 );
}

/***************************************************************
 Send a signal to the current circuit in the circuit view and to the time chart 
 view in order to see the graphical simulation
***************************************************************/

void QlgSignalGeneratorDlgImpl::nextStep( int newValue )
{	    
    QString value;
    
    value = value.setNum( newValue, 2 );    
    
    //  Pre-append with the 0 character    
    
    value = value.rightJustify( circuitView->numInputs(), '0' );
    
    inputNumbers->display( value );         

    //  Update the circuit with the value
    	
    circuitView->simulateCircuit( inputVariables->text(), value );
        
}

/***************************************************************
 Send a signal to the current circuit in the circuit view
 in order to see the graphical simulation
***************************************************************/

void QlgSignalGeneratorDlgImpl::nextStepAutomatic()
{
    int newValue = spinBoxValue->value() + 1;
    	           
    if( newValue > spinBoxValue->maxValue() )
	 newValue = 0;    

    spinBoxValue->setValue( newValue );
    
}

void QlgSignalGeneratorDlgImpl::stepAutomatic( bool state )
{
    if( state ) {
	spinBoxValue->setEnabled( FALSE );
	int T = div( 1000, spinBoxFrecuency->value() ).quot;
	pushButtonStart->setText( "Detener");
	timer->start( T, FALSE ); // T seconds single-shot timer   
    }
    else {
	spinBoxValue->setEnabled( TRUE );
	pushButtonStart->setText( "Iniciar");   	
	timer->stop();
    }
}

/***************************************************************
 Send automatic pulses to the circuit for a new value
***************************************************************/

void QlgSignalGeneratorDlgImpl::stepAutomatic( int newValue )
{
    if( ( newValue >0 ) && ( newValue<1001 ) )
	stepAutomatic( pushButtonStart->isOn() );
}

QlgSignalGeneratorDlgImpl::~QlgSignalGeneratorDlgImpl()
{
    
}
