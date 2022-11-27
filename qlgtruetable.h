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

#ifndef QLGTRUETABLE_H
#define QLGTRUETABLE_H

#include <qtable.h>

class QlgMainWindow;

class QlgTrueTableItem : public QTableItem
{
public:
    QlgTrueTableItem( QTable *t, EditType et, const QString &txt ) : QTableItem( t, et, txt ) {}
    void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );
};

class QlgTrueTable : public QTable
{   
    Q_OBJECT
    
public:    
    QlgTrueTable( QWidget *parent );
    ~QlgTrueTable();
    
    void dropColumns( int numCol );
    
    int numVariables(){ return nVariables; };
    int numOutputs(){ return nOutputs; };
    
private:    
    int nVariables; 
    int nOutputs;
    
    void fillTable();
    void fillOutputTable();
    void fillColumn( QString number, int col );
    
public slots:    
    void setNumVariables( int );
    void setNumOutputs( int );
};
#endif
