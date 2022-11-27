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

#include "qlgoutputwindow.h"

#include <qlistview.h>
#include <qtextedit.h>
#include <qapplication.h>
#include <qlayout.h> 

QlgOutputWindow::QlgOutputWindow( QWidget *parent )
    : QTabWidget( parent, "output_window" )
{
    tab = new QWidget( this, "tab" );
    
    tabLayout = new QGridLayout( tab, 1, 1, 11, 6, "tabLayout");
    textEdit = new QTextEdit( tab, "textEdit" );
    tabLayout->addWidget( textEdit, 0, 0 );    
    insertTab( tab, tr( "Descripción" ) );
}

void QlgOutputWindow::showDescription( QString d )
{
    textEdit->clear();
    textEdit->append( d );
}

QlgOutputWindow::~QlgOutputWindow()
{

}
