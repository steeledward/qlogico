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

#include "qlgfsmview.h"
#include "qlgfsmdlgimpl.h"
#include "qlgfsmmachine.h"

QlgFSMDlgImpl::QlgFSMDlgImpl( QWidget *parent )
	: QlgFSMDlg( parent, "FSM Init Dialog", Qt::WType_Dialog )
{            
    fsmView = 0;
    connect( (QObject*)(pushButtonApply), SIGNAL(  clicked() ), this, SLOT( apply() ) );    
    connect( (QObject*)(pushButtonOK), SIGNAL( clicked() ), this, SLOT( newFSMUpdate() ) );
    connect( (QObject*)(pushButtonCancel), SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( (QObject*)(pushButtonMooreOutsFont), SIGNAL( clicked() ), this, SLOT( changeMOF() ) );
    connect( (QObject*)(pushButtonMealyInputsFont), SIGNAL( clicked() ), this, SLOT( changeMeIF() ) );
    connect( (QObject*)(pushButtonMealyOutputsFont), SIGNAL( clicked() ), this, SLOT( changeMeOF() ) );
    connect( (QObject*)(pushButtonColor), SIGNAL( clicked() ), this, SLOT( setColor() ) );
}

void QlgFSMDlgImpl::newFSMUpdate()
{
    if( fsmView != 0 ) {
	update();
	fsmView = 0;
    }
    else {
	getValues();
	hide();
	emit newFSM( values );
    }
}

void  QlgFSMDlgImpl::getValues()
{
    values = new QlgFSMOptions( lineEditName->text(),
				spinBoxMooreOuts->value(),
				lineEditMooreOuts->text(),
				pushButtonMooreOutsFont->font(),
				spinBoxMealyInputs->value(), 
				lineEditMealyInputs->text(),
				pushButtonMealyInputsFont->font(),
				spinBoxMealyOutputs->value(), 
				lineEditMealyOutputs->text(), 
				pushButtonMealyOutputsFont->font(),
				spinBoxWidth->value(),
				spinBoxHeight->value(),
				lb_colorpreview->paletteBackgroundColor(),    
				checkBoxGrid->isChecked(),
				textEditDescription->text() );    
}

/************************************************************
  Show the dialog for update the fsm view
************************************************************/

void QlgFSMDlgImpl::showDlg( QlgFSMView *fsmViewNew )
{
    fsmView = fsmViewNew;
    spinBoxMooreOuts->setEnabled( false );
    spinBoxMealyInputs->setEnabled( false );
    spinBoxMealyOutputs->setEnabled( false );    
    pushButtonApply->setEnabled( true );
    
    lineEditName->setText( fsmView->fsmMachine->values()->name() );
    
    spinBoxMooreOuts->setValue( fsmView->fsmMachine->values()->numMooreOutputs() );
    lineEditMooreOuts->setText( fsmView->fsmMachine->values()->mooreNames() );
    pushButtonMooreOutsFont->setFont( fsmView->fsmMachine->values()->mooreOutsFont() );
	    
    spinBoxMealyInputs->setValue( fsmView->fsmMachine->values()->numMealyInputs() );    
    lineEditMealyInputs->setText( fsmView->fsmMachine->values()->mealyInputNames() );
    pushButtonMealyInputsFont->setFont( fsmView->fsmMachine->values()->mealyInputsFont() );

    spinBoxMealyOutputs->setValue( fsmView->fsmMachine->values()->numMealyOutputs() );
    lineEditMealyOutputs->setText( fsmView->fsmMachine->values()->mealyOutputNames() );
    pushButtonMealyOutputsFont->setFont( fsmView->fsmMachine->values()->mealyOutputsFont() );
	    
    spinBoxWidth->setValue( fsmView->canvas()->width() );
    spinBoxHeight->setValue( fsmView->canvas()->height() );	    
    lb_colorpreview->setPaletteBackgroundColor( fsmView->canvas()->backgroundColor() );
    if( fsmView->mesh() )
	checkBoxGrid->setChecked( true );
    else
	checkBoxGrid->setChecked( false );
    show();
}

void QlgFSMDlgImpl::apply()
{    
    getValues();
    fsmView->setOptions( values );  
}

void QlgFSMDlgImpl::update()
{
    apply();
    hide();
    spinBoxMooreOuts->setEnabled( true );
    spinBoxMealyInputs->setEnabled( true );
    spinBoxMealyOutputs->setEnabled( true );
    pushButtonApply->setEnabled( false );
}

/*********************************************************
  Change the font for the Moore Variables
*********************************************************/

void QlgFSMDlgImpl::changeMOF()
{
    pushButtonMooreOutsFont->setFont( QFontDialog::getFont( 0, pushButtonMooreOutsFont->font() ) );
    lineEditMooreOuts->setFont( pushButtonMooreOutsFont->font() );
}

/*********************************************************
  Change the font for the Mealy Input Variables
*********************************************************/

void QlgFSMDlgImpl::changeMeIF()
{
    pushButtonMealyInputsFont->setFont( QFontDialog::getFont( 0, pushButtonMealyInputsFont->font() ) );
    lineEditMealyInputs->setFont( pushButtonMealyInputsFont->font() );
} 

/*********************************************************
  Change the font for the Mealy Output Variables
*********************************************************/

void QlgFSMDlgImpl::changeMeOF()
{
    pushButtonMealyOutputsFont->setFont( QFontDialog::getFont( 0, pushButtonMealyOutputsFont->font() ) );
    lineEditMealyOutputs->setFont( pushButtonMealyOutputsFont->font() );
} 

/*********************************************************************
  Show the dialog for select a color for the background 
  of the view
*********************************************************************/

void QlgFSMDlgImpl::setColor()
{
 QColor color = QColorDialog::getColor(
                             lb_colorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color de fondo" ) );
        if ( color.isValid() )
	    lb_colorpreview->setPaletteBackgroundColor( color );
}

QlgFSMDlgImpl::~QlgFSMDlgImpl()
{
    
}
