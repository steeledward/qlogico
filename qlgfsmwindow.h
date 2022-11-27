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

#ifndef QLGFSMWINDOW_H
#define QLGFSMWINDOW_H

#include <qmainwindow.h>
#include <vector.h>

class QlgFSMView;
class QCanvas;
class QlgMainWindow;
class QlgFSMOptions;

class QlgFSMWindow: public QMainWindow
{
    Q_OBJECT
public:
    QlgFSMWindow( QWidget* parent, QlgMainWindow* mw, QlgFSMOptions *values  );
    ~QlgFSMWindow();
    QlgFSMView* sView;
    
public slots:    
    void changeName( QString );
    
private:
    QCanvas* canvasFSM;
    QlgMainWindow* mWindow;
};
#endif
