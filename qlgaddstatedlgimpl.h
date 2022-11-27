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

#ifndef QLGADDSTATEDLGIMPL_H
#define QLGADDSTATEDLGIMPL_H

#include <qstringlist.h>

#include "qlgaddstatedlg.h"

class QlgFSMView;
class QlgState;

class QlgAddStateDlgImpl : public QlgAddStateDlg
{
    Q_OBJECT
    
public:        
    QlgAddStateDlgImpl( QWidget* parent = 0 );
    ~QlgAddStateDlgImpl();
    
public slots:
    void showDlg();
    void showDlg( QlgState *st );
    void update( QlgFSMView *fsmViewNew );  
    void addUpdate();
    void setColor();
    void update();    
    void apply();
 
private:    
    /// The actual fsm view
    QlgFSMView* fsmView;
    QStringList values;
    /// Tha actual state to update or modify
    QlgState *state;        
};
#endif
