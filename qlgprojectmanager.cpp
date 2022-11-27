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

#include <qlayout.h>
#include <qwidget.h>
#include <qpixmap.h>

#include "qlgprojectmanager.h"
#include "qlgmainwindow.h"
#include "qlgcircuitwindow.h"

QlgProjectManager::QlgProjectManager(  QWidget *parent , QlgMainWindow *mw  )
	: QListView( parent, 0, WStyle_Customize | WStyle_NormalBorder | WStyle_Title |
		 WStyle_Tool | WStyle_MinMax | WStyle_SysMenu ), mainWindow( mw )
{    
    setResizePolicy( QScrollView::Manual );
    addColumn( tr( "Archivos" ) );
    setAllColumnsShowFocus( TRUE );
    setHScrollBarMode( AlwaysOff );
    setVScrollBarMode( QScrollView::Auto );
    viewport()->setAcceptDrops( TRUE );
    setAcceptDrops( TRUE );
    setColumnWidthMode( 1, Manual );
}

void QlgProjectManager::addFile( QWidget *window )
{
    QListViewItem *newItem = new QListViewItem( this );
    newItem->setText( 0, window->caption() );
    newItem->setPixmap( 0,  *(window->icon()) );
}

QlgProjectManager::~QlgProjectManager()
{
    
}
