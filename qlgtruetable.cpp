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

#include <qmessagebox.h>
#include <qwhatsthis.h>
#include <math.h>

#include "qlgtruetable.h"
#include "qlgmainwindow.h"

QlgTrueTable::QlgTrueTable(  QWidget *parent )
    : QTable( parent )
{
    nVariables = 0;
    nOutputs = 0;
    
    setReadOnly( TRUE );
    setResizePolicy( QScrollView::Manual );    
    setHScrollBarMode( QScrollView::Auto );
    setVScrollBarMode( QScrollView::Auto );
    viewport()->setAcceptDrops( TRUE );
    setAcceptDrops( TRUE );    
    
    QWhatsThis::add( this, tr("<b>Tabla de Verdad</b>"
			      "<p>La <b>Tabla de Verdad</b> le permite"
			      "la creación  de un circuito combinacional a partir de sus minterminos, los cuales se seleccionan haciendo click sobre las <b>salidas</b> de la tabla <b>S0, S1, ... , S9</b>.</p>" 
			      "los valores puden ser <b>0</b>, <b>1</b> o <b>X</b>( no importa )" ) );
}

/***********************************************************************
  Function that updates the size of the true table based
  in the number of variables
************************************************************************/  

void QlgTrueTable::setNumVariables( int numVars )
{
    if (  ( numVars < 11 ) && ( numVars >1 ) && ( numVars != nVariables ) ) {	
	nVariables = numVars;	
	setNumCols( numVars + nOutputs );
	setNumRows( (int)powl( 2, numVars ) );
    
	for( int j=0; j<numVars; j++)
	    horizontalHeader()->setLabel( j, QString( QChar( 'A'+j ) ) );
        
	for( int j=0; j< numRows(); j++)	
	    verticalHeader()->setLabel( j, QString::number( j ) );    
    
	fillTable();
	fillOutputTable();			
    }
}

/***********************************************************************
  Fill the true table with binary numbers
************************************************************************/  

void QlgTrueTable::fillTable()
{
    int rows = numRows();
    int n = 1;
    int bit = 0;
    QString number = "";
    
    for( int column = nVariables-1; column > -1; column-- ) {
	for(  int j = 0; j<rows; j++ ) {
	    for( int digit = 0; digit < n; digit++ ) {
		number = number + QString::number( bit );
	    }
	    if( bit == 0 )
		bit = 1;
	    else
		bit = 0;	    	    
	}
	fillColumn( number, column );
	number = "";
	rows = rows / 2;
	n = n * 2;
    }
}

/*********************************************************************
  Function that fill a column @ col with a string @ number
*********************************************************************/

void QlgTrueTable::fillColumn( QString number, int col )
{
	    
    for( int j = 0; j < numRows(); j++ )
	setItem( j, col, new QlgTrueTableItem( this, QTableItem::WhenCurrent, QChar(number[j]) ) );

    adjustColumn( col );
}

/***********************************************************************
  Function that fill the output section of the true table
  the headers using the label S and setting all the value to 0
************************************************************************/

void QlgTrueTable::fillOutputTable()
{
    int  k = 0;    
    for( int j = nVariables; j < numCols(); j++) {
	horizontalHeader()->setLabel( j, "S" + QString::number( k ) );
	k++;
    }

    for( int col = nVariables; col < numCols(); col++ ) {
	for( int row = 0; row < numRows(); row++ )
	    setText( row, col, "0" );	
	adjustColumn( col );
    }    
}

void QlgTrueTable::dropColumns( int numCol )
{
    for( int j = 0;  j<numCol; j++)
	removeColumn( numCols()-1 );
}

/**********************************************************************
  Update the true table view depending on the number of 
  outputs = numOuts 
***********************************************************************/  

void QlgTrueTable::setNumOutputs( int numOuts )
{
    if (  ( numOuts < 101 ) && ( numOuts > 0 )  &&  ( numOuts != nOutputs ) ) {
	if( numOuts < nOutputs ) 
	    dropColumns( nOutputs - numOuts );
	else	    
	    insertColumns( nVariables + nOutputs, numOuts - nOutputs );
	fillOutputTable();  	
	nOutputs = numOuts;
    }
}

void QlgTrueTableItem::paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected )
{
    QColorGroup g( cg );
    // last row is the sum row - we want to make it more visible by
    // using a different background
    if ( row() < table()->numRows() && col() < ((QlgTrueTable*)(table()))->numVariables() )
	g.setColor( QColorGroup::Base, 15134100 );
    QTableItem::paint( p, g, cr, selected );
}

QlgTrueTable::~QlgTrueTable()
{
    
}
