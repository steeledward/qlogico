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

#ifndef QLGFSMDGLIMPL_H
#define QLGFSMDGLIMPL_H

#include "qlgfsmdlg.h"
#include "qlgfsmoptions.h"

class QlgFSMView;
class QlgFSMMachine;
class QCheckBox;
class QLabel;

class QlgFSMDlgImpl : public QlgFSMDlg
{
    Q_OBJECT
    
public:    
    QlgFSMDlgImpl( QWidget *parent = 0 );
    ~QlgFSMDlgImpl();
    
public slots:    
    void showDlg( QlgFSMView *fsmView );

private slots:
    void newFSMUpdate();
    void changeMOF();
    void changeMeIF();
    void changeMeOF();
    void apply();
    void setColor();
    
signals:
    void newFSM( QlgFSMOptions *opt );
	    
private:    
    void update();
    void getValues();
    QlgFSMOptions *values;
    QlgFSMView *fsmView;
};
#endif
