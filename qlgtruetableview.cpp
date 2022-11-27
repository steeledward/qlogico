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
#include <qsplitter.h>
#include <qmessagebox.h>
#include <qstringlist.h>

#include "qlgmainwindow.h"
#include "qlgtruetableview.h"
#include "qlgtruetable.h"
#include "qlgfunctionstable.h"
#include "qlgquine.h"
#include "qlgfsmview.h"
#include "qlgcircuitview.h"
#include "qlgcircuitwindow.h"
#include "qlgtransition.h"
#include "qlgfsmmachine.h"
#include "qlgfsmoptions.h"

QlgTrueTableView::QlgTrueTableView(  QWidget *parent , QlgMainWindow *mw  )
	: QWidget( parent ), mainWindow( mw ), sView(0), fsmView(0)
{   
    QGridLayout *l = new QGridLayout( this );    
    split = new QSplitter( this );
    split->setOrientation( QSplitter::Vertical );
    trueTable = new QlgTrueTable( split  );
    functionsTable = new QlgFunctionsTable( split );    		       
    l->addWidget( split, 0, 0 );  
        
    connect( trueTable, SIGNAL( clicked(int,int,int,const QPoint&) ), this, SLOT( trueTableClicked(int,int,int,const QPoint&) ) );
    connect( trueTable->horizontalHeader(), SIGNAL( clicked(int) ), this, SLOT( trueTableHeaderClicked(int) ) );
    connect( this, SIGNAL(numVariablesChanged(int)), trueTable, SLOT(setNumVariables(int))  );    
    connect( this, SIGNAL(numOutputsChanged(int)), trueTable, SLOT(setNumOutputs(int))  );
    connect( this, SIGNAL(numOutputsChanged(int)), functionsTable, SLOT(setNumOutputs(int))  );
    
    setNumOutputs( 1 );
    setNumVariables( 2 );

}

void QlgTrueTableView::setNumVariables( int n )
{
    numVariables = n;
    emit numVariablesChanged( n );
}

void QlgTrueTableView::setNumOutputs( int n )
{
    numOutputs = n;
    emit numOutputsChanged( n );
}

void QlgTrueTableView::updateCircuitView( QlgCircuitView * sViewNew )
{   
    sView = sViewNew;
}

void QlgTrueTableView::updateFSMView( QlgFSMView * fsmViewNew )
{
    fsmView = fsmViewNew;
}

/**********************************************************************
  Function that creates the original and reduced boolean 
  functions from the minterms selected in the true table view
***********************************************************************/

void QlgTrueTableView::buildFunctions()
{
    QString oper = "";
    QString minTerms = "";
    QString dontCare = "";
    QString oFunction = "";
    int i = 0;
    
    for( int col = trueTable->numCols() - numOutputs; col < trueTable->numCols(); col++ ) {
	oper = "";
	minTerms = "";
	dontCare = "";
	oFunction = "";
	
	for( int row = 0; row < trueTable->numRows(); row++ ) {
	    if( trueTable->text( row, col ) == "1") {
		oFunction = oFunction + oper;
		oFunction = oFunction + getMinTerm( row );
		minTerms = minTerms + QString::number( row ) + " ";
		oper = '+';
	    }
	    else if( trueTable->text( row, col ) == "X")
		dontCare = dontCare + QString::number( row ) + " ";
	}
	
	functionsTable->setText( i, 0, oFunction );
	oFunction = QString( ReducirEcuacion(numVariables, (char *)minTerms.latin1(),(char *)dontCare.latin1() ) );
	oFunction = replaceVars( oFunction );
	functionsTable->setText( i, 1, oFunction  );
	i++;
    }
    
    functionsTable->adjustColumn( 0 );
    functionsTable->adjustColumn( 1 );   
}

/************************************************************
  Function that get a @ function that depend only the variables A, B, ... J
  and replace that for the variables that appears in the true table view
************************************************************/

QString QlgTrueTableView::replaceVars( QString function )
{
    for( int i = 0; i < 10; i++ )
	function = function.replace( (char)('A' + i), trueTable->horizontalHeader()->label( i ) );
    
    return function;
}

/*********************************************************************
  Functions that get the number of a row in the true table
 and then return  the min-term for that row
**********************************************************************/

QString QlgTrueTableView::getMinTerm( int Row )
{
    QString minTerm = "";
    QString oper = "";
    
    for(int Column=0; Column<numVariables; Column++){
	minTerm =  minTerm + oper;
	minTerm =  minTerm + trueTable->horizontalHeader()->label( Column );
	if( trueTable->text( Row, Column ) == "0" )
	    minTerm = minTerm + "'";
	oper = "*";
    }
    return minTerm;
}

/********************************************************************
  Show the vector containig the original functions desired
*********************************************************************/

void QlgTrueTableView::showFunctions( QlgBoolFunctionList oFuncs )
{
    setNumOutputs( oFuncs.count() );
    int j = 0;
    QlgBoolFunction *it;
    for ( it = oFuncs.first(); it; it = oFuncs.next() ) {
	functionsTable->setText( j, 0, it->text() );
	j++;
    }
    
    functionsTable->adjustColumn( 0 );
}

/***********************************************************************
  Function that redraw the cell clicked to show the value
  of a minterm that could be 0, 1 or X
************************************************************************/  

void QlgTrueTableView::trueTableClicked( int Row, int Col, int Button, const QPoint& )
{
    if( ( Button == LeftButton ) && ( Col>=numVariables ) ) {	
	if( trueTable->text( Row, Col) == "0")
	    trueTable->setText(Row, Col, "1" );
	else if( trueTable->text( Row, Col) == "1")
	    trueTable->setText( Row, Col, "X" );		
	else 
	    trueTable->setText( Row, Col, "0" );    
    }
}

/***********************************************************************
  Function that redraw the table of bool functions when a 
  header of the true table is clicked.
************************************************************************/  

void QlgTrueTableView::trueTableHeaderClicked(int Column )
{
    functionsTable->clearSelection(TRUE);
    QTableSelection tableSelection;
    tableSelection.init( Column-numVariables, 0 );
    tableSelection.expandTo( Column-numVariables, 1 );
    functionsTable->addSelection(tableSelection);   
}

/*********************************************************************
  Function that obtain the true table for the current boolean 
  functions that appears in the true table view
**********************************************************************/

void QlgTrueTableView::buildTTable()
{   
    originalFunctions();
    
    QString value = "";
        
    QStringList tVars = getTotalVariables( vOFunctions );

    setNumVariables( tVars.size() );
    setNumOutputs( vOFunctions.count() );
    
    for( int i = 0; i < numVariables; i++)
	trueTable->horizontalHeader()->setLabel( i, tVars[i] );
    
    int j = 0;
    QlgBoolFunction *fcn;
    for(int Row=0; Row<trueTable->numRows(); Row++) {
	for(int Column=0; Column<numVariables; Column++)
	    value = value + trueTable->text(Row, Column);
	for( fcn = vOFunctions.first(); fcn; fcn = vOFunctions.next() ) {
	    trueTable->setText(Row, numVariables+j, QString::number( fcn->evaluate( value ) ) );
	    j++;
	}
	value = "";
	j = 0;
    }
}

/***********************************************************************
  Function that return a vector containing the independiente 
  variables availables in a set of boolean functions
************************************************************************/  

QStringList QlgTrueTableView::getTotalVariables( QlgBoolFunctionList vF )
{
    QStringList::Iterator it, value;
    QStringList vVars;
    QStringList vLastVars;
    QlgBoolFunction *fcn;
    
    for( fcn = vF.first(); fcn; fcn = vF.next() ) {
	vVars = fcn->variables();
	for( value = vVars.begin(); value != vVars.end(); ++value ) {
	    it = vLastVars.find( *value );
	    if(  it == vLastVars.end() )
		vLastVars.append( *value );
	}    
    }
    return vLastVars;
}

/***********************************************************************
  Function that read the original boolean functions that
  appears in the true table view, validate and save them
************************************************************************/  

QlgBoolFunctionList QlgTrueTableView::originalFunctions()
{
    vOFunctions.clear();
    
    for( int row = 0; row < functionsTable->numRows(); row++ )
	vOFunctions.append( new QlgBoolFunction( functionsTable->verticalHeader()->label( row ) + "=" +  functionsTable->text( row, 0 ) ) );
    
    return vOFunctions;
}

/***********************************************************************
  Function that read the reduced boolean functions that
  appears in the true table view, validate and save them
************************************************************************/  

QlgBoolFunctionList QlgTrueTableView::reducedFunctions()
{   
    vRFunctions.clear();
    
    for( int row = 0; row < functionsTable->numRows(); row++ )
	vRFunctions.append( new QlgBoolFunction( functionsTable->verticalHeader()->label( row ) + "=" +  functionsTable->text( row, 1 ) ) );
    
    return vRFunctions;
}

/******************************************************************
  Function that build the finite State Machine True Table 
  for the active finite state machine  view
******************************************************************/

void QlgTrueTableView::buildFSMTable()
{
    if( !fsmView ) {
	QMessageBox::information( this, "Qlogico", tr("Construya primero una maquina de estados finitos") );
	return;
    }
    setNumVariables( fsmView->fsmMachine->values()->numMooreOutputs() + fsmView->fsmMachine->values()->numMealyInputs() ); 
    setNumOutputs( fsmView->fsmMachine->values()->numMooreOutputs() + ( fsmView->fsmMachine->values()->numMooreOutputs() * 2 ) +  fsmView->fsmMachine->values()->numMealyOutputs() );
    labelFSMTable();
    int fila = 0;
    bool ok;
    QString m_output = "";
    
    for( QlgTransition *trans = fsmView->fsmMachine->transitions().first(); trans; trans = fsmView->fsmMachine->transitions().next() ) {
	fila = ( trans->actual()->value() + trans->input() ).toInt( &ok, 2 );
	m_output = trans->next()->value() +  getJKValues( trans->actual()->value(), trans->next()->value() ) + trans->output();
	fillEndRow( fila, m_output );
    }    
    
    for( int i = 0; i < trueTable->numCols(); i++)
	trueTable->adjustColumn( i );
    
    for( int i = 0; i < fsmView->fsmMachine->values()->numMooreOutputs(); i++ )
	functionsTable->removeRow( 0 );
    
    numOutputs =  numOutputs -  fsmView->fsmMachine->values()->numMooreOutputs();
    buildFunctions();
    mainWindow->newCircuitWindow()->view()->addCircuitJK( reducedFunctions(), fsmView->fsmMachine->values()->numMooreOutputs(),  fsmView->fsmMachine->values()->numMealyOutputs() );
}

/***************************************************************
  Function that set the label for the true table using the value of the FSM
***************************************************************/

void QlgTrueTableView::labelFSMTable()
{
    int hd = 0;
    
    for( int i = fsmView->fsmMachine->values()->numMooreOutputs()-1; i > -1; i-- ) {
	trueTable->horizontalHeader()->setLabel( hd, fsmView->fsmMachine->values()->mooreNames() + QString::number(i) );
	hd++;
    }
    for( int i = fsmView->fsmMachine->values()->numMealyInputs()-1; i > -1; i-- ) {
	 trueTable->horizontalHeader()->setLabel( hd, fsmView->fsmMachine->values()->mealyInputNames()  + QString::number(i) );
	hd++;
    }	 
    for( int i = fsmView->fsmMachine->values()->numMooreOutputs()-1; i > -1; i-- ) {
	trueTable->horizontalHeader()->setLabel( hd, fsmView->fsmMachine->values()->mooreNames()  + QString::number(i) );
	hd++;
    }	 
    for( int i = fsmView->fsmMachine->values()->numMooreOutputs()-1; i > -1; i-- ) {
	trueTable->horizontalHeader()->setLabel( hd, "J"  + QString::number(i) );
	trueTable->horizontalHeader()->setLabel( hd+1, "K"  + QString::number(i) );
	hd+=2;
    }
    for( int i = fsmView->fsmMachine->values()->numMealyOutputs()-1; i > -1; i-- ) {
	trueTable->horizontalHeader()->setLabel( hd, fsmView->fsmMachine->values()->mealyOutputNames() + QString::number(i) );
	hd++;
    }    
    int row = 0;
    for( int col = trueTable->numCols() - numOutputs; col < trueTable->numCols(); col++) {
	functionsTable->verticalHeader()->setLabel(  row, trueTable->horizontalHeader()->label( col ) );
	row++;
    }
}

/***********************************************************
  Function that returns a string with the JK Flip-Flop values for the
  @ actual and @ next values
***********************************************************/  

QString QlgTrueTableView::getJKValues( QString actual, QString next )
{
    QString jkValue = "";
    
    for( uint i = 0; i < actual.length(); i++ ) {
	if( actual[i] == '0' && next[i] == '0' )
	    jkValue += "0X";
	else if( actual[i] == '0' && next[i] == '1' )
	    jkValue += "1X";	
	else if( actual[i] == '1' && next[i] == '0' )
	    jkValue += "X1";
	else if( actual[i] == '1' && next[i] == '1' )
	    jkValue += "X0";
    }
    return jkValue;
}

/*******************************************************************
  Function that fills the end of a @ row with a @ value
*******************************************************************/

void QlgTrueTableView::fillEndRow( int row, QString value )
{
    int i = 0;
    for( int col = numVariables; col < trueTable->numCols(); col++ ) {
	trueTable->setText( row, col, QString( value[i] ) );
	i++;	
    }
}

/*************************************************************
  Create a new true table with the label for each @ inputs and @ outputs
*************************************************************/

void QlgTrueTableView::setTrueTable( QString inputs, QString outputs, int **&bitList )
{
    
    QStringList ins = QStringList::split( ",", inputs );
    QStringList outs = QStringList::split( ",", outputs );
    QStringList headers = ins + outs;
    QString mterm = "";
    
    setNumVariables( ins.count() );
    setNumOutputs( outs.count() );
          
    for( uint i = 0; i < headers.count(); i++ )
	trueTable->horizontalHeader()->setLabel( i, headers[i] );	

    for( int bit = 0; bit < pow(2, numVariables); bit++ ) {
	mterm = "";
	for( uint var = 0; var <  numVariables; var++ ) 
	    mterm = mterm + QString::number( bitList[var][bit] );
	    
	bool ok;
	int row = mterm.toInt( &ok, 2 );
		
	for( int col = numVariables; col < (numVariables+numOutputs); col++ )
	    trueTable->setText( row, col, QString::number( bitList[col][bit] ) );
    }    
    
}

	
