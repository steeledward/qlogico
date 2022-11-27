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
#include <qwhatsthis.h>

#include "qlgboolfunction.h"
#include "qlgmainwindow.h"
#include "qlgfunctionstable.h"

QlgFunctionsTable::QlgFunctionsTable(  QWidget *parent )
	: QTable( parent )
{    
    numOutputs = 0;
    
    setNumCols( 2 );
    setNumRows( 1 );
    
    horizontalHeader()->setLabel( 0, QObject::tr( "Funciones Originales" ) );
    horizontalHeader()->setLabel( 1, QObject::tr( "Funciones Reducidas" ) );
    verticalHeader()->setLabel( 0, QObject::tr( "S0" ) );
    
    adjustColumn( 0 );
    adjustColumn( 1 );
    
    setResizePolicy( QScrollView::Manual );    
    setHScrollBarMode( QScrollView::Auto );
    setVScrollBarMode( QScrollView::Auto );
    viewport()->setAcceptDrops( TRUE );
    setAcceptDrops( TRUE );
    
    QWhatsThis::add( this, tr("<b>Tabla de Funciones Booleanas</b>"
			      "<p>La <b>Tabla de Funciones Booleanas</b> le muestra"
			      "las <b>Funciones Originales</b> y las  <b>Funciones Reducidas</b>.</p>" 
			      "obtenidas a partir de la <b>Tabla de Verdad</b>" ) );
}

/**********************************************************************
  Update the true table view depending on the number of 
  outputs = numOuts 
***********************************************************************/  

void QlgFunctionsTable::setNumOutputs( int numOuts )
{
    if (  ( numOuts < 101 ) && ( numOuts > 0 )  &&  ( numOuts != numOutputs ) ) {
	numOutputs = numOuts;
	setNumRows( numOutputs );
	int row = 0;
	
	for( int col = numCols() - numOuts; col < numCols(); col++) {
	    verticalHeader()->setLabel(  row, "S" + QString::number(row) );
	    row++;
	}	
    }
}

void QlgFunctionsTable::dropRows( int numRow )
{
    for( int j = 0;  j<numRow; j++)
	removeRow( numRows()-1 );
}

QlgFunctionsTable::~QlgFunctionsTable()
{
    
}
