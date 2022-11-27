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

#include <qcanvas.h>

#include "qlgmainwindow.h"
#include "qlgfsmwindow.h"
#include "qlgfsmview.h"
#include "qlgfsmoptions.h"

QlgFSMWindow::QlgFSMWindow( QWidget* parent, QlgMainWindow* mw, QlgFSMOptions *values  )
    : QMainWindow( parent, "fsm", 0 ), mWindow( mw )
{
    if( values->name() == "" )
	values->setName( tr("Maquina sin nombre") );
    
    setCaption( values->name() );
    canvasFSM = new QCanvas( values->width(), values->height() );
    sView = new QlgFSMView( canvasFSM, this );
    sView->addMachine( values );
    
    setFocusProxy( sView );
    setCentralWidget( sView );
    resize( QSize(400,300) );
    connect( sView, SIGNAL(nameChanged(QString)), this, SLOT(changeName(QString)) );
}

void QlgFSMWindow::changeName( QString newName )
{
    if( newName == "" )
	newName = "Sin nombre";
    
    setCaption( tr("M.E.F.") + " - " + newName );
}

QlgFSMWindow::~QlgFSMWindow()
{

}

