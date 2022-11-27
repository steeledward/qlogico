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

#ifndef QLGCALCULATORDLGIMP_H
#define QLGCALCULATORDLGIMP_H

#include <qobject.h>

#include "qlgcalculatordlg.h"

class QlgCalculatorDlgImpl : public QlgCalculatorDlg
{
    Q_OBJECT
    
public:        
    QlgCalculatorDlgImpl( QWidget* parent = 0, const char* name = 0, WFlags fl = WType_TopLevel );
    ~QlgCalculatorDlgImpl();
       
private:
    void conversion();
    void addition();
    void substraction();
    void product();
    void division();
    
public slots: 
    void evaluate();
};
#endif
