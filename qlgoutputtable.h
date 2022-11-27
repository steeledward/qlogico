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

#ifndef QLGOUTPUTTABLE_H
#define QLGOUTPUTTABLE_H

#include <qtable.h>

class QlgMainWindow;

class QlgOutputTable : public QTable
{
public:    
    QlgOutputTable( QWidget *parent , QlgMainWindow *mw );
    ~QlgOutputTable();
    
    int outputs() const { return numOutputs; }
    
public slots:    
	void setNumOutputs( int numOuts, int numVars );      

private:
    QlgMainWindow *mainWindow;    
    int numOutputs;    
    
signals:
       void numOutputsChanged( int numOuts, int numVars );        
};
#endif
