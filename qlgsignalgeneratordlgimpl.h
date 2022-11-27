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

#ifndef QLGSIGNALGENERATORDLGIMPL_H
#define QLGSIGNALGENERATORDLGIMPL_H

#include "qlgsignalgeneratordlg.h"

class QTimer;
class QlgCircuitView;

class QlgSignalGeneratorDlgImpl : public QlgSignalGeneratorDlg
{
    Q_OBJECT
    
public:    
    QlgSignalGeneratorDlgImpl( QWidget *parent = 0 );
    ~QlgSignalGeneratorDlgImpl();
    void update( QlgCircuitView *sV );
    
public slots: 	
    virtual void showDlg();
    
private slots:    
    void nextStep( int newValue );
    void nextStepAutomatic();
    void stepAutomatic( bool );
    void stepAutomatic( int );
    void init();
    
private:
    QTimer *timer;
    QlgCircuitView *circuitView;
    
};
#endif
