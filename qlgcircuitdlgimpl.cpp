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

#include "qlgcircuitview.h"
#include "qlgcircuitdlgimpl.h"

QlgCircuitDlgImpl::QlgCircuitDlgImpl( QWidget *parent )
	: QlgCircuitDlg( parent, "FSM Init Dialog", Qt::WType_Dialog )
{            
    circuitView = 0;
    connect( (QObject*)(pushButtonApply), SIGNAL(  clicked() ), this, SLOT( apply() ) );
    connect( (QObject*)(pushButtonOK), SIGNAL( clicked() ), this, SLOT( update() ) );
    connect( (QObject*)(pushButtonCancel), SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( (QObject*)(pushButtonColor), SIGNAL( clicked() ), this, SLOT( setColor() ) );
    connect( (QObject*)(pushButtonLineColor), SIGNAL( clicked() ), this, SLOT( setLineColor() ) );
    connect( (QObject*)(pushButtonMeshColor), SIGNAL( clicked() ), this, SLOT( setMeshColor() ) );
}

/************************************************************
  Show the dialog for update the fsm view
************************************************************/

void QlgCircuitDlgImpl::showDlg( QlgCircuitView *circuitViewNew )
{
    circuitView = circuitViewNew;
    
    lineEditName->setText( circuitView->name() );
    textEditDescription->setText( circuitView->description() );
    spinBoxWidth->setValue( circuitView->canvas()->width() );
    spinBoxHeight->setValue( circuitView->canvas()->height() );
    lb_colorpreview->setPaletteBackgroundColor( circuitView->canvas()->backgroundColor() );
    lb_linecolorpreview->setPaletteBackgroundColor( circuitView->lineColor() );
    lb_meshcolorpreview->setPaletteBackgroundColor( circuitView->meshColor() );
    
    if( circuitView->mesh() )
	checkBoxGrid->setChecked( true );
    else
	checkBoxGrid->setChecked( false );
    show();
}

void QlgCircuitDlgImpl::apply()
{    
    circuitView->setName( lineEditName->text() );
    circuitView->setDescription( textEditDescription->text() );
    circuitView->canvas()->resize( spinBoxWidth->value(), spinBoxHeight->value() );
    circuitView->canvas()->setBackgroundColor( lb_colorpreview->paletteBackgroundColor() );
    circuitView->setLineColor( lb_linecolorpreview->paletteBackgroundColor() );
    circuitView->setMeshColor( lb_meshcolorpreview->paletteBackgroundColor() );
    
    if( checkBoxGrid->isChecked() )
	circuitView->addMesh();
    else
	circuitView->clearMesh();
    circuitView->canvas()->setAllChanged();
    circuitView->canvas()->update();
}

void QlgCircuitDlgImpl::update()
{
    apply();
    hide();
}

/*********************************************************************
  Show the dialog for select a color for the background 
  of the view
*********************************************************************/

void QlgCircuitDlgImpl::setColor()
{
 QColor color = QColorDialog::getColor(
                             lb_colorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color de fondo" ) );
        if ( color.isValid() )
	    lb_colorpreview->setPaletteBackgroundColor( color );
}

void QlgCircuitDlgImpl::setLineColor()
{
 QColor color = QColorDialog::getColor(
                             lb_linecolorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color para cada linea" ) );
        if ( color.isValid() )
	    lb_linecolorpreview->setPaletteBackgroundColor( color );
}

void QlgCircuitDlgImpl::setMeshColor()
{
 QColor color = QColorDialog::getColor(
                             lb_meshcolorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color para la malla" ) );
        if ( color.isValid() )
	    lb_meshcolorpreview->setPaletteBackgroundColor( color );
}

QlgCircuitDlgImpl::~QlgCircuitDlgImpl()
{
    
}
