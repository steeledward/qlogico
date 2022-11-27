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
#include <qobject.h>
#include <qnamespace.h> 
#include <qmessagebox.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qspinbox.h>

#include "qlgnumber.h"
#include "qlgcalculatordlgimpl.h"

QlgCalculatorDlgImpl::QlgCalculatorDlgImpl( QWidget* parent, const char* name, WFlags fl )
	: QlgCalculatorDlg( parent, name, fl )
{
    QObject::connect( (QObject *)pushButtonEvaluate, SIGNAL( clicked() ), SLOT( evaluate() )  ); 
}

QlgCalculatorDlgImpl::~QlgCalculatorDlgImpl()
{
    
}

void QlgCalculatorDlgImpl::evaluate()
{
    switch ( comboBoxOperation->currentItem()  ) {
    case(0):
	conversion();
	break;
    case(1):
	addition();
	break;
    case(2):
	substraction();
	break;	
    case(3):
	product();
	break;
    case(4):
	division();
	break;	
    }
}

void QlgCalculatorDlgImpl::conversion()
{
    if( editNumberBaseR->text() != "" ) {
	QlgNumber number( editNumberBaseR->text(), CSpinR->value() );
	number.convert( CSpinS->value(), CSpinN->value() );
	if( number.value() != "0.0" )
	    editNumberBaseS->setText( number.value() );
	else
	    QMessageBox::warning( this, tr("Calculadora"), tr("El numero : ") + editNumberBaseR->text() + tr(" no pertenece a la bases : ") + CSpinR->text() + number.message() );
    }
}

void QlgCalculatorDlgImpl::addition()
{
    if( editNumberBaseR->text() != "" && editNumberBaseS->text() != "" ) {
	QlgNumber A( editNumberBaseR->text(), CSpinR->value() );
	QlgNumber B( editNumberBaseS->text(), CSpinS->value() );
	QlgNumber C = A + B;
	editResult->setText( C.value() );
    }
}

void QlgCalculatorDlgImpl::substraction()
{
    if( editNumberBaseR->text() != "" && editNumberBaseS->text() != "" ) {
	QlgNumber A( editNumberBaseR->text(), CSpinR->value() );
	QlgNumber B( editNumberBaseS->text(), CSpinS->value() );
	QlgNumber C = A - B;
	editResult->setText( C.value() );
    }
}

void QlgCalculatorDlgImpl::product()
{
    if( editNumberBaseR->text() != "" && editNumberBaseS->text() != "" ) {
	QlgNumber A( editNumberBaseR->text(), CSpinR->value() );
	QlgNumber B( editNumberBaseS->text(), CSpinS->value() );
	QlgNumber C = A * B;
	editResult->setText( C.value() );
    }
}

void QlgCalculatorDlgImpl::division()
{
    if( editNumberBaseR->text() != "" && editNumberBaseS->text() != "" ) {
	QlgNumber A( editNumberBaseR->text(), CSpinR->value() );
	QlgNumber B( editNumberBaseS->text(), CSpinS->value() );
	QlgNumber C = A / B;
	editResult->setText( C.value() );
    }
}

