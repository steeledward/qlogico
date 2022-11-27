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

#ifndef QLGBOOLFUNCTION_H
#define QLGBOOLFUNCTION_H

#include <qstringlist.h> 
#include <qvaluelist.h> 
#include <qstring.h>
#include <qpoint.h>

class QlgBoolFunction : public QString
{
public:    
    QlgBoolFunction( QString s = 0 );
    ~QlgBoolFunction();
    int evaluate( QString value );
    
    /// Returns the text for the bool function
    QString text(){ return function; };
    /// Returns the Output variable fdx
    QString fdx(){ return f_fdx; };
    /// Returns the number of variables of the function
    int numVariables(){ return vVariables.size(); };
    /// Returns the pointer to the variables of the function
    QStringList& variables(){ return vVariables; };
    /// Returns the pointer to the minterms of the function    
    QStringList& minTerms(){ return vMinTerms; };
    /// Returns the vector containing the binterms of the minterm
    QStringList binTerms( int index );
    // Returns the number of minterms
    int numMinTerms() { return vMinTerms.size(); };
    /// Returns the number of binterms
    int numBinTerms(){ return vBinTerms.size(); };
    
private:
    /// Bool function in text form
    QString function;
    /// Output of the function fdx = x+y
    QString f_fdx;
    /// The permited letters to use in a bool function
    QString validVariables;
    /// Vector containning  the variables of the function
    QStringList vVariables;
    /// Vector containning  the minterms of the function    
    QStringList vMinTerms;
    /// Vector containning  the binterms of the function    
    QStringList vBinTerms;    
    /// Vector containning  the position of the binterms of the function        
    QValueList<QPoint> vPosBinTerms;
    
    void getMinTerms();  
    void getBinTerms();
    void getVariables();    
    int evaluateTerm( int index, QString value );
    int  findVarValue( QString var, QString value );    
};
#endif
