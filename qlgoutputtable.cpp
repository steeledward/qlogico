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
#include <math.h>

#include "qlgoutputtable.h"
#include "qlgmainwindow.h"

QlgOutputTable::QlgOutputTable(  QWidget *parent , QlgMainWindow *mw  )
	: QTable( parent ), mainWindow( mw )
{   
    setNumOutputs( 1, 2 ); 
    
    setResizePolicy( QScrollView::Manual );    
    setHScrollBarMode( AlwaysOff );
    setVScrollBarMode( AlwaysOn );
    viewport()->setAcceptDrops( TRUE );
    setAcceptDrops( TRUE );
}

void QlgOutputTable::setNumOutputs( int numOuts, int numVars )
{
    if (  ( numOuts < 101 ) && ( numOuts > 0 ) ) {
	numOutputs = numOuts;
	emit numOutputsChanged( numOuts, numVars );
    }
	
}

void QlgOutputTable::numOutputsChanged( int numOuts, int numVars )
{
    setNumCols( numOuts );
    setNumRows( (int)powl( 2, numVars ) );
    
    for( int j=0; j<numOuts; j++)
	horizontalHeader()->setLabel( j, "S" + QString( QChar( j ) ) );
}

QlgOutputTable::~QlgOutputTable()
{
    
}
