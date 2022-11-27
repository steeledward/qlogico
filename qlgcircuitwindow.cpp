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

#include <qscrollview.h>
#include <qsize.h>
#include <qcanvas.h>
#include <qprogressdialog.h>
#include <qlabel.h>
#include <qstatusbar.h>
#include <qaction.h>

#include "qlgmainwindow.h"
#include "qlgcircuitwindow.h"
#include "qlgcircuitview.h"
#include "qlgboolfunction.h"

QlgCircuitWindow::QlgCircuitWindow( QWidget* parent, QlgMainWindow* mw, const QString name )
    : QMainWindow( parent, "circuit" , WDestructiveClose), mainWindow( mw )
{
    mainCanvas = new QCanvas(500,400);
    circuitView = new QlgCircuitView( this, name );
    
    setCaption( tr("Esquemático") + " - " + name );

    setFocusProxy( circuitView );
    setCentralWidget( circuitView );   
        
    connect( circuitView, SIGNAL( lockChanged( bool ) ), this, SLOT( setLocked( bool ) ) );
    connect( circuitView, SIGNAL( cursorReleased() ), this, SLOT( releaseCursor() ) );
    connect( circuitView, SIGNAL( nameChanged(QString) ), this, SLOT( changeName(QString) ) );
    
    show();
}

void QlgCircuitWindow::setLocked( bool lock )
{
    if( lock )
	labelLocked->setPixmap( QPixmap::fromMimeSource("locked") );
    else
	labelLocked->setPixmap( QPixmap::fromMimeSource("unlocked") );
}

void QlgCircuitWindow::releaseCursor()
{
    mainWindow->actionPointer->toggle();
}

void QlgCircuitWindow::changeName( QString newName )
{
    if( newName == "" )
	newName = "Sin nombre";
    
    setCaption( tr("Esquemático") + " - " + newName );
}

QlgCircuitWindow::~QlgCircuitWindow()
{
    circuitView->clear();
}
