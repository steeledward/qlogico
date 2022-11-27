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

#ifndef QLGCIRCUITDGLIMPL_H
#define QLGCIRCUITDGLIMPL_H

#include "qlgcircuitdlg.h"

class QlgCircuitView;
class QCheckBox;
class QLabel;

class QlgCircuitDlgImpl : public QlgCircuitDlg
{
    Q_OBJECT
    
public:    
    QlgCircuitDlgImpl( QWidget *parent = 0 );
    ~QlgCircuitDlgImpl();
    
public slots:    
    void showDlg( QlgCircuitView *circuitView );

private slots:
    void update();
    void apply();
    void setColor();
    void setLineColor();
    void setMeshColor();
    
private:
    QlgCircuitView *circuitView;
};
#endif
