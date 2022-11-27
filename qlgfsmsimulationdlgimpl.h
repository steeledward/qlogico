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

#ifndef QLGFSMSIMULATORDLGIMPL_H
#define QLGFSMSIMULATORDLGIMPL_H

#include "qlgfsmsimulationdlg.h"

class QlgFSMView;
class QTimer;

class QlgFSMSimulationDlgImpl : public QlgFSMSimulationDlg
{
    Q_OBJECT
    
public:    
    QlgFSMSimulationDlgImpl( QWidget *parent = 0 );
    ~QlgFSMSimulationDlgImpl();
    void update( QlgFSMView *sV );
    
private:    
    QlgFSMView* fsmView;
    QTimer *timer;
    
public slots:
    void showDlg();
    
private slots:
    void updateOutput( int index );
    void updateInput( int index );
    void updateState( int index );
    void init();
};
#endif
