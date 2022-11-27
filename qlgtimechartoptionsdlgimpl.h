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

#ifndef QLGTIMECHARTOPTIONSDGLIMPL_H
#define QLGTIMECHARTOPTIONSDGLIMPL_H

#include "qlgtimechartoptionsdlg.h"

class QlgCircuitView;
class QCheckBox;
class QLabel;
class QlgTimeChartWindow;

class QlgTimeChartOptionsDlgImpl : public QlgTimeChartOptionsDlg
{
    Q_OBJECT
    
public:    
    QlgTimeChartOptionsDlgImpl( QWidget *parent = 0 );
    ~QlgTimeChartOptionsDlgImpl();
    
public slots:    
    void showDlg( QlgTimeChartWindow * );

private slots:
    void update();
    void apply();
    void setColor();
    void setLineColor();
    void setMeshColor();
    
private:
    QlgTimeChartWindow *timeWindow;
};
#endif
