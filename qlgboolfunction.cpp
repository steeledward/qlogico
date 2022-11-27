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

#include "qlgboolfunction.h"

/***********************************************
  Creates a new bool function using a string
***********************************************/

QlgBoolFunction::QlgBoolFunction( QString fcn )
    : QString( fcn ), validVariables("abcdefghijklmnñopqrstuvwxyzABCDEFGHIJKLMNÑOPQRSTUVWXYZ0123456789+'*=")
{  
    uint i =0;

    if( fcn.find( "''" ) == -1 && fcn.find( "++" ) == -1 && fcn.find( "**" ) == -1 && fcn.contains('=') < 2 ) {
	while( ( validVariables.find( fcn[i] ) != -1 ) && ( i < fcn.length() ) )
	    i++;
    
	if( i < fcn.length() ) {
	    function = "";
	    f_fdx = "";
	}
	else {
	    if( fcn.contains('=') == 1 ) {
		QStringList list =  QStringList::split( '=', fcn );
		f_fdx = list[0];
		function = list[1];
	    }
	    else {
		f_fdx = "S";
		function = fcn;
	    }
	    getMinTerms();
	    getBinTerms();
	    getVariables();
	}
    }
    else {
	function = "";
	f_fdx = "";
    }
}

/********************************************************************
 Return a vector with each minterm of the equation
 Eliminate the character +
 example: "abc + bcd " --> [ "abc" "bcd" ]	
*********************************************************************/

void QlgBoolFunction::getMinTerms()
{    
    vMinTerms = QStringList::split( "+", function );
}

/*******************************************************************
  Function that create a notation for the function
  Example: 
  "a'*b*cd + c*d*f" --->  [ "'a"  "b"  "cd"  "c"  "d"  "f"  ]
  the new information is stored in the vector vBinTerms 
  and the position for each minterm in the vector 
  vPosBinTerms
********************************************************************/

void QlgBoolFunction::getBinTerms()
{    
    QPoint p;
    for( QStringList::Iterator  minterm = vMinTerms.begin(); minterm != vMinTerms.end(); ++minterm ) {
	p.setX( vBinTerms.size() );	
	vBinTerms = vBinTerms + QStringList::split( "*", *minterm );
	p.setY( vBinTerms.size() );
	vPosBinTerms.push_back( p );
    }
}

/**********************************************************************
 Function that obtain the independent variables of the function
**********************************************************************/

void QlgBoolFunction::getVariables()	
{
    QStringList::Iterator term, it;
    QString value = "";
    
    for( term = vBinTerms.begin();  term != vBinTerms.end(); ++term ) {
	if( (*term).endsWith( "'" ) )
	    value = (*term).left((*term).length()-1);
	else
	    value = *term;
	it = qFind( vVariables.begin(), vVariables.end(), value );
	if ( it == vVariables.end() ) 
	    vVariables.push_back( value );
    }
}

/***************************************************************************
  Function that evaluate the function for certain vector of values. Return 1  or 0
*****************************************************************************/

int  QlgBoolFunction::evaluate( QString value )
{
    int result = 0;
    int index = 0;
    
    while( ( result == 0 ) && ( index < numMinTerms() ) ) {
	result = evaluateTerm( index, value );
	index++;
    }

    return ( result );
}

/***************************************************************************
  Function that evaluate a term for certain value. Return 1  or 0
*****************************************************************************/

int  QlgBoolFunction::evaluateTerm( int index, QString value )
{
    int result = 1;
    
    int begin = vPosBinTerms[index].x();
    int end = vPosBinTerms[index].y();
    
    while( ( begin < end ) && (result == 1) ) {	
	result = findVarValue( vBinTerms[begin], value);
	begin++;
    }

    return result;
}

/***************************************************************************
  Function that find the value for a variable. Return 1 or 0
*****************************************************************************/

int  QlgBoolFunction::findVarValue( QString var, QString value )
{
    int i = 0;
    int result = 0;
    QStringList::Iterator first, last;
    first = vVariables.begin();
    last = vVariables.end();
    QString tvar = var;
    
    if( var.endsWith( "'" ) )
	tvar = var.left(var.length()-1);
    else
	tvar = var;	
	
    while (first != last && *first != tvar ) {
	++first;
	i++;
    }
    
    if( var.endsWith( "'" ) ) {
	if(  value[i] == '0' )
	    result = 1;
	else
	    result = 0;
    }
    else{
	if(  value[i] == '0' )
	    result = 0;
	else
	    result = 1;
    }
    
    return result;
}

/************************************************************
  Returns a vectorn containning the binterms of the desired minterm
************************************************************/

QStringList QlgBoolFunction::binTerms( int index )
{ 
    QStringList vc;
    
    int begin = vPosBinTerms[index].x();
    int end = vPosBinTerms[index].y();
		
    for( int i = begin ; i < end; i++ )
	vc.push_back( vBinTerms[i] );    
    
    return vc;
}

QlgBoolFunction::~QlgBoolFunction()
{
    
}
