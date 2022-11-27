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
#include <qpixmap.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcolordialog.h>
#include <qpushbutton.h>

#include "qlgupdatetransitiondlgimpl.h"
#include "qlgtransition.h"

QlgUpdateTransitionDlgImpl::QlgUpdateTransitionDlgImpl( QWidget* parent )
	: QlgUpdateTransitionDlg( parent, "Update State Dialog", Qt::WType_Dialog | Qt::WStyle_Customize | Qt::WStyle_NormalBorder  )
{
    transition = 0;
    
    comboBoxTransitionStyle->insertItem( QPixmap::fromMimeSource( "tipo1" ), "", -1 );
    comboBoxTransitionStyle->insertItem( QPixmap::fromMimeSource( "tipo2" ), "", -1 );
    comboBoxTransitionStyle->insertItem( QPixmap::fromMimeSource( "tipo3" ), "", -1 );
    comboBoxTransitionStyle->insertItem( QPixmap::fromMimeSource( "tipo4" ), "", -1 );
    comboBoxTransitionStyle->insertItem( QPixmap::fromMimeSource( "tipo5" ), "", -1 );
    
    connect(  (QObject*)(pushButtonCancel), SIGNAL(  clicked() ), this, SLOT(  hide()  ) );
    connect(  (QObject*)(pushButtonUpdate), SIGNAL(  clicked() ), this, SLOT(  update()  ) );
    connect(  (QObject*)(pushButtonApplyUpdate), SIGNAL(  clicked() ), this, SLOT(  apply()  ) );
    connect(  (QObject*)(pushButtonColor), SIGNAL(  clicked() ), this, SLOT(  setColor()  ) );
}

void QlgUpdateTransitionDlgImpl::showDlg( QlgTransition *trans )
{
    transition = trans;
    spinBoxWidth->setValue( transition->width() );
    lb_colorpreview->setPaletteBackgroundColor( transition->color() );
    show();
}

void QlgUpdateTransitionDlgImpl::apply()
{
    transition->setWidth( spinBoxWidth->value() );
    transition->setColor( lb_colorpreview->paletteBackgroundColor() );
    transition->setStyle( comboBoxTransitionStyle->currentItem() );
    transition->canvas()->setAllChanged();
    transition->canvas()->update();
}

void QlgUpdateTransitionDlgImpl::update()
{
    apply();
    hide();
}

void QlgUpdateTransitionDlgImpl::setColor()
{
    QColor color = QColorDialog::getColor(
	    lb_colorpreview->paletteBackgroundColor(),
	    this, tr( "Seleccionar color de estado" ) );
    if ( color.isValid() )
	lb_colorpreview->setPaletteBackgroundColor( color );    
}

QlgUpdateTransitionDlgImpl::~QlgUpdateTransitionDlgImpl()
{
    
}
