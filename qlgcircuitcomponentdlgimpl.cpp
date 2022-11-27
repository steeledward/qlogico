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

#include <qlineedit.h>
#include <qspinbox.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qfontdialog.h>
#include <qcolordialog.h>
#include <qtable.h>
#include <qgroupbox.h>

#include "qlgcircuitcomponentdlgimpl.h"
#include "qlgcircuitcomponent.h"

QlgCircuitComponentDlgImpl::QlgCircuitComponentDlgImpl( QWidget *parent )
	: QlgCircuitComponentDlg( parent, "Circuit Component Dialog", Qt::WType_Dialog )
{
    circuitComponent = 0;
    connect( (QObject*)(pushButtonApply), SIGNAL(  clicked() ), this, SLOT( apply() ) );    
    connect( (QObject*)(pushButtonOK), SIGNAL( clicked() ), this, SLOT( update() ) );
    connect( (QObject*)(pushButtonCancel), SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( (QObject*)(spinBoxOutputs), SIGNAL( valueChanged(int) ), tableOutputs, SLOT( setNumRows(int) ) );
}

/************************************************************
  Show the dialog for update the circuit component
************************************************************/

void QlgCircuitComponentDlgImpl::showDlg( QlgCircuitComponent *newCircuitComponent )
{
    circuitComponent = newCircuitComponent;
    lineEditName->setText( circuitComponent->label.text() );
    checkBoxVisible->setChecked( circuitComponent->label.isVisible() );
    spinBoxInputs->setValue( circuitComponent->numInputs() );
    spinBoxOutputs->setValue( circuitComponent->numOutputs() );
    checkBoxOutputsVisibles->setChecked( circuitComponent->showState() );
    tableOutputs->setNumRows( spinBoxOutputs->value() );
    for( int row = 0; row< spinBoxOutputs->value(); row++ )
	    tableOutputs->setText( row, 0, circuitComponent->function(row) );
    
    if( circuitComponent->type() == "entrada" ) {
	frameInputs->hide();
	groupBoxOutputs->hide();
    }
    else if( circuitComponent->type() == "salida" ) {	
	frameInputs->hide();
	groupBoxOutputs->hide();
    }
    else if( circuitComponent->type() == "personalizable" ) {
	frameInputs->show();
	groupBoxOutputs->show();
	spinBoxOutputs->show();
	tableOutputs->show();
	textLabelOutputs->show();
    }
    else {
	frameInputs->show();
	textLabelOutputs->hide();
	spinBoxOutputs->hide();
	tableOutputs->hide();
	groupBoxOutputs->show();
    }
    adjustSize();
    show();
}

void QlgCircuitComponentDlgImpl::apply()
{    
    circuitComponent->label.setText( lineEditName->text() );    
    circuitComponent->label.setVisible( checkBoxVisible->isChecked() );
    
    circuitComponent->setNumInputs( spinBoxInputs->value() );
    
    QStringList vFcns;
        
    for( int row = 0; row< tableOutputs->numRows(); row++ )
	vFcns << tableOutputs->text( row, 0 );
    
    circuitComponent->setFunctions( vFcns );
    circuitComponent->setShowState( checkBoxOutputsVisibles->isChecked() );
    circuitComponent->canvas()->setAllChanged();
    circuitComponent->canvas()->update();   
}

void QlgCircuitComponentDlgImpl::update()
{
    apply();
    hide();
}

QlgCircuitComponentDlgImpl::~QlgCircuitComponentDlgImpl()
{
    
}
