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
#include <qcolordialog.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qspinbox.h>
#include <qmessagebox.h>
#include <qwidget.h> 
#include <qtextedit.h>

#include "qlgaddstatedlgimpl.h"
#include "qlgfsmview.h"
#include "qlgfsmmachine.h"
#include "qlgstate.h"

QlgAddStateDlgImpl::QlgAddStateDlgImpl( QWidget* parent )
	: QlgAddStateDlg( parent, "Add State Dialog" )
{
    fsmView = 0;
    state = 0;
    connect( (QObject*)(pushButtonApply), SIGNAL( clicked() ), this, SLOT( apply() ) );
    connect(  (QObject*)(pushButtonCancel), SIGNAL(  clicked() ), this, SLOT(  hide()  ) );
    connect(  (QObject*)(pushButtonOK), SIGNAL(  clicked() ), this, SLOT(  addUpdate()  ) );
    connect(  (QObject*)(pushButtonColor), SIGNAL(  clicked() ), this, SLOT(  setColor()  ) );    
}

void QlgAddStateDlgImpl::update( QlgFSMView *fsmViewNew )
{    
    if( fsmView != fsmViewNew )
	fsmView = fsmViewNew;
}

void QlgAddStateDlgImpl::showDlg()
{
    if( fsmView ) {
	update();
	if( comboBoxState->count() > 0 )	
	    show();
	else
	    QMessageBox::warning( this, tr("Maquina de estados finitos"), tr("No existen mas estados disponibles.\nYa estan es uso todos los estados permitidos") );
    }
}

void QlgAddStateDlgImpl::showDlg( QlgState *st )
{
    state = st;
    lineEditName->setText( state->name() );
    spinBoxRatio->setValue( state->ratio() );
    spinBoxWidth->setValue( state->lineWidth() );
    lb_colorpreview->setPaletteBackgroundColor( state->color() );
    textDescription->setText( state->description() );
    comboBoxState->setEnabled( false );
    pushButtonApply->setEnabled( true );
    show();
}

/********************************************************************** 
 Update the dialog to add a state
**********************************************************************/

void QlgAddStateDlgImpl::update()
{
    pushButtonApply->setEnabled( false );
    comboBoxState->setEnabled( true );
    QString value;	        
    comboBoxState->clear();   
    
    lineEditName->setText( tr("Sin nombre") );
    
    for ( QStringList::Iterator it = fsmView->fsmMachine->stateCodes().begin(); it != fsmView->fsmMachine->stateCodes().end(); ++it ) {
        comboBoxState->insertItem( *it );
    }
}

void QlgAddStateDlgImpl::addUpdate()
{
    if( state !=  0 ) {
	apply();
	hide();
	state = 0;
    }
    else {
	if( comboBoxState->currentText() != "" ) {
	    values.clear();
	    values.push_back( lineEditName->text() );
	    values.push_back( comboBoxState->currentText() );
	    values.push_back( spinBoxRatio->text() );
	    values.push_back( spinBoxWidth->text() );
	    values.push_back( lb_colorpreview->paletteBackgroundColor().name() );
	    values.push_back( textDescription->text() );
	    hide();
	    fsmView->fsmMachine->addState( values );
	    update();
	}
	else
	    QMessageBox::about( this, tr("Datos Incorrectos"), tr("Seleccionar un estado permitido") );
    }    
}

void QlgAddStateDlgImpl::setColor()
{
 QColor color = QColorDialog::getColor(
                             lb_colorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color de estado" ) );
        if ( color.isValid() )
	    lb_colorpreview->setPaletteBackgroundColor( color );
}

void QlgAddStateDlgImpl::apply()
{
    state->name( lineEditName->text() );
    state->setRatio( spinBoxRatio->value() );
    state->setLineWidth( spinBoxWidth->value() );
    state->setColor( lb_colorpreview->paletteBackgroundColor() );
    state->setDescription( textDescription->text() );
    state->canvas()->setAllChanged();
    state->canvas()->update();
}

QlgAddStateDlgImpl::~QlgAddStateDlgImpl()
{
    
}
