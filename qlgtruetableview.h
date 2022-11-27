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

#ifndef QLGTRUETABLEVIEW_H
#define QLGTRUETABLEVIEW_H

#include <qwidget.h>
#include <qptrlist.h>
#include <qstringlist.h>
#include <vector.h>

#include "qlgboolfunction.h"
#include "qlgfsmview.h"
#include "qlgcircuitview.h"

class QlgMainWindow;
class QlgTrueTable;
class QlgFunctionsTable;
class QSplitter;
class QlgFSMView;
class QlgCircuitView;

typedef QPtrList<QlgBoolFunction> QlgBoolFunctionList;
	
class QlgTrueTableView : public QWidget
{
    Q_OBJECT
    
public:    
    QlgTrueTableView( QWidget *parent , QlgMainWindow *mw );
    QlgBoolFunctionList originalFunctions();
    QlgBoolFunctionList reducedFunctions();
    QSplitter *split;

    void updateCircuitView( QlgCircuitView * sV );
    void updateFSMView( QlgFSMView * sV );
    
public slots:
    void trueTableClicked( int Row, int Col, int Button, const QPoint& );
    void trueTableHeaderClicked(int Column );
    void buildFunctions();
    void showFunctions( QlgBoolFunctionList );
    void buildTTable();
    void buildFSMTable();
    void setNumVariables(int);
    void setNumOutputs(int);
    void setTrueTable( QString inputs, QString outputs, int **& );
    
private:
    QlgMainWindow *mainWindow;  
    QlgTrueTable *trueTable;
    QlgCircuitView * sView;
    QlgFSMView * fsmView;
    QlgFunctionsTable *functionsTable;
    void fillEndRow( int row, QString value );
    void labelFSMTable();
    QString getMinTerm( int Row );
    QString getJKValues( QString actual, QString next );
    QString replaceVars( QString function );
    
    QStringList getTotalVariables( QlgBoolFunctionList vF );
    QlgBoolFunctionList vOFunctions;
    QlgBoolFunctionList vRFunctions;
    
    int numVariables;
    int numOutputs;
    
signals:
    void numVariablesChanged(int);
    void numOutputsChanged(int);
};
#endif
