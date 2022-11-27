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

#include <qstring.h>
#include <math.h>
#include <qobject.h> 
#include <qapplication.h> 

#include "qlgnumber.h"

/************************************************************************
  Constructor for the new number with the value 'val' and his
  base number 'b'
************************************************************************/

QlgNumber::QlgNumber( QString val, int b )
{
    validDigits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    msg = "";
    
    if( !isValidNumber( val.upper(), b ) ) {
	ip = "0";
	fp = "0";
	baseValue = 0;
    }
    else
	convertToDecimal();
}

/************************************************************************
  If the number is valid assign the correspondent values 
for each part of the number
*************************************************************************/

bool QlgNumber::isValidNumber( QString number, int baseS )
{
    bool ipValid;               // integer part valid
    bool fpValid;               // fractional part valid
    
    QString NOriginalPEntera;
    QString NOriginalPFraccional;
    QString DigitosNoPermitidos;
    QString invalidIntegerPart;
    QString invalidFractionalPart;

// Check for the existence of only one fractional point, 
// if this occur the integer and fractional parts are stored

    if( isValidFractionalPart( number, &NOriginalPEntera, &NOriginalPFraccional) ) {
	msg = msg + QObject::tr( "Existe mas de un punto fraccional" );
	return ( false )  ;
    }
    else {

	ipValid = isNumberCorrect(NOriginalPEntera,&invalidIntegerPart, baseS );         // Validamos los digitos
	fpValid = isNumberCorrect(NOriginalPFraccional, &invalidFractionalPart, baseS );   // y guardamos los digitos
    }

  // If the digitos are corrects continue else show the message 
 
    if( ipValid && fpValid ){
	ip = NOriginalPEntera;
	fp = NOriginalPFraccional;
	baseValue =  baseS;
    }
    else {
	msg = msg + showInvalidDigits(invalidIntegerPart, invalidFractionalPart );
	return ( false );
    }

    return ( true );
}

/********************************************************************
Check for the valid digits for the respective base
********************************************************************/

bool QlgNumber::isNumberCorrect( QString Number, QString* invalidDigits, int r)
{
    if(Number != "0") {
	int PosDigito = 0;
	for( unsigned i=0; i< Number.length(); i++) {
	    PosDigito = validDigits.find( Number[i] );	    
	    if( PosDigito >= r )
		*invalidDigits = *invalidDigits + validDigits[PosDigito] + ", ";
	}
	if( !(*invalidDigits).isEmpty() )
	    return false;
	else
	    return true;
    }
    else
	return true;
}

/****************************************************************************
  Convert a number to the new base and return the new number
*****************************************************************************/

void QlgNumber::convert( int base, int precision )
{
    convertDecimalToBaseS( base, precision );
}


/********************************************************************** 
Check for the existenceof only one  fractional point, 
if this is true obtain his integer and fractional  part
**********************************************************************/

bool QlgNumber::isValidFractionalPart( QString Number, QString* integerPart, QString* fractionalPart )
{
    if(  Number.find(".")  == Number.findRev(".")  ) {
	if( Number.find(".") != -1 ) {
	    *integerPart = Number.left( Number.find(".") );
	    *fractionalPart = Number.right( Number.length() - Number.find(".") - 1 );
	    return false;
	}
	else {
	    *integerPart = Number;
	    *fractionalPart = "0";
	    return false;
	}
    }
    else
	return true;
}


/*********************************************************************** 
Convert a number to his decimal form
************************************************************************/

void QlgNumber::convertToDecimal()
{
    int p = ip.length() - 1;  // Number de digitos del radix-1
    double decimal = 0;

    for( unsigned i=0; i<ip.length(); i++) {
	decimal = decimal + ( validDigits.find( ip[i] ) ) * powl(baseValue,p);
	p -= 1;
    } 
    
    ip = QString::number( decimal );
    decimalValue = decimal;
    
    decimal =0;
    
    for( unsigned i=0; i<fp.length(); i++) {
	decimal = decimal + ( validDigits.find( fp[i] ) ) * powl(baseValue,p);
	p -= 1;
    } 
    
    fp = QString::number( decimal );
    decimalValue += decimal;    

}

/*********************************************************************
Convertimos a decimal number  to another base
*********************************************************************/

void QlgNumber::convertDecimalToBaseS( int baseS, int precision )
{
    QString NumeroBaseS = "";
    double DecimalEntero = ip.toDouble();
    double DecimalFraccion =  fp.toDouble();
    double Residuo;

    while(DecimalEntero >=baseS) {
	Residuo = fmod(DecimalEntero, baseS);
	NumeroBaseS += validDigits[int(Residuo)];
	modf(DecimalEntero/baseS, &DecimalEntero);
    }
    
    NumeroBaseS += validDigits[int(DecimalEntero)];
    
    QString temp = "";

    for(int i=NumeroBaseS.length()-1; i>-1; i--)
	temp += NumeroBaseS[i];
 
    ip = temp;
    
    NumeroBaseS = "";
	       
    if( precision > 0) {	
	double tempF = 0;
	for( int j=0; j<precision; j++) {
	    tempF = DecimalFraccion*baseS;
	    modf(tempF, &DecimalEntero);
	    NumeroBaseS += validDigits[int(DecimalEntero)];
	    DecimalFraccion = modf(tempF, &DecimalEntero);
	}
	fp = NumeroBaseS;
    }
    else
	fp = "0";
}

/***************************************************************************
 Show the invalid digits for this base. 
****************************************************************************/

QString QlgNumber::showInvalidDigits( QString invalidIntegerPart, QString invalidFractionalPart )
{
    QString DigitosNoPermitidos = "";

    if( ( !invalidIntegerPart.isEmpty() ) || ( !invalidFractionalPart.isEmpty() ) ) {
	if( !invalidIntegerPart.isEmpty() ) {
	    DigitosNoPermitidos += "\n\nDigitos invalidos en la parte entera: \n";
	    DigitosNoPermitidos += invalidIntegerPart.upper();
	}
	if( !invalidFractionalPart.isEmpty() ) {
	    DigitosNoPermitidos += "\nDigitos invalidos en la parte fraccional: \n";
	    DigitosNoPermitidos += invalidFractionalPart.upper();
	}	
    }

    return DigitosNoPermitidos;
}
