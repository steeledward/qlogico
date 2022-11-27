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
#include <qcanvas.h>

#include "qlgtimechartwindow.h"
#include "qlgtimechartoptionsdlgimpl.h"

QlgTimeChartOptionsDlgImpl::QlgTimeChartOptionsDlgImpl( QWidget *parent )
	: QlgTimeChartOptionsDlg( parent, "TimeChart Options Dialog", Qt::WType_Dialog )
{
    timeWindow = 0;
    
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

void QlgTimeChartOptionsDlgImpl::showDlg( QlgTimeChartWindow *timeWindowNew )
{
    timeWindow = timeWindowNew;
    
    lb_colorpreview->setPaletteBackgroundColor( timeWindow->canvas()->backgroundColor() );
    lb_linecolorpreview->setPaletteBackgroundColor( timeWindow->lineColor() );
    lb_meshcolorpreview->setPaletteBackgroundColor( timeWindow->meshColor() );
    
    if( timeWindow->mesh() )
	checkBoxGrid->setChecked( true );
    else
	checkBoxGrid->setChecked( false );
    show();
}

void QlgTimeChartOptionsDlgImpl::apply()
{    
    timeWindow->canvas()->setBackgroundColor( lb_colorpreview->paletteBackgroundColor() );
    timeWindow->setLineColor( lb_linecolorpreview->paletteBackgroundColor() );
    timeWindow->setMeshColor( lb_meshcolorpreview->paletteBackgroundColor() );
    
    if( checkBoxGrid->isChecked() )
	timeWindow->addMesh();
    else
	timeWindow->clearMesh();
    timeWindow->canvas()->setAllChanged();
    timeWindow->canvas()->update();
}

void QlgTimeChartOptionsDlgImpl::update()
{
    apply();
    hide();
}

/*********************************************************************
  Show the dialog for select a color for the background 
  of the view
*********************************************************************/

void QlgTimeChartOptionsDlgImpl::setColor()
{
 QColor color = QColorDialog::getColor(
                             lb_colorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color de fondo" ) );
        if ( color.isValid() )
	    lb_colorpreview->setPaletteBackgroundColor( color );
}

void QlgTimeChartOptionsDlgImpl::setLineColor()
{
 QColor color = QColorDialog::getColor(
                             lb_linecolorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color para cada linea" ) );
        if ( color.isValid() )
	    lb_linecolorpreview->setPaletteBackgroundColor( color );
}

void QlgTimeChartOptionsDlgImpl::setMeshColor()
{
 QColor color = QColorDialog::getColor(
                             lb_meshcolorpreview->paletteBackgroundColor(),
                            this, tr( "Seleccionar color para la malla" ) );
        if ( color.isValid() )
	    lb_meshcolorpreview->setPaletteBackgroundColor( color );
}

QlgTimeChartOptionsDlgImpl::~QlgTimeChartOptionsDlgImpl()
{
    
}
