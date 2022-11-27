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

#ifndef QLGNUMBER_H
#define QLGNUMBER_H

#include <qstring.h>

class QlgNumber
{ 
public:    
    QlgNumber( QString value, int base );
    QlgNumber
	    operator+( QlgNumber& rv ) {
	return QlgNumber( QString::number(decimalValue + rv.decimal()), 10 );
    }
    QlgNumber
	    operator-( QlgNumber& rv ) {
	return QlgNumber( QString::number(decimalValue - rv.decimal()), 10 );
    }
    QlgNumber
	    operator*( QlgNumber& rv ) {
	return QlgNumber( QString::number(decimalValue * rv.decimal()), 10 );
    }
    QlgNumber
	    operator/( QlgNumber& rv ) {
	QString number;
	if( rv.decimal() == 0 )
	    number = "0";
	else
	    number = QString::number(decimalValue / rv.decimal() );
	return QlgNumber( number, 10 );
    }
    QString value(){ return ip+"."+fp; };
    QString message(){ return msg; };    
    void convert( int base, int precison  );
    double decimal(){ return decimalValue; };

private:
    double decimalValue;
    uint baseValue;    
    QString ip;
    QString fp;
    QString msg;        
    
    bool isValidFractionalPart( QString Number, QString* integerPart, QString* fractionalPart );    
    bool isNumberCorrect( QString Number, QString* invalidDigits, int r);
    bool isValidNumber( QString value, int base );
    bool makeConversion( QString originalNumber, int BaseR, int BaseS, int Precision, QString *valueBaseS, QString *Dr, QString *valueDecimal, QString *errorMessage );
    void convertToDecimal();
    void convertDecimalToBaseS( int BaseS, int Digits );
    QString showInvalidDigits( QString invalidIntegerPart, QString invalidFractionalPart );
    QString validDigits;
};
#endif
