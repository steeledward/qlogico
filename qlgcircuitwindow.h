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

#ifndef QLGCIRCUITWINDOW_H
#define QLGCIRCUITWINDOW_H

#include <qmainwindow.h>
#include <qptrlist.h>

class QlgCircuitView;
class QCanvas;
class QlgMainWindow;
class QlgBoolFunction;
class QLabel;

class QlgCircuitWindow: public QMainWindow
{
    Q_OBJECT
public:
    QlgCircuitWindow( QWidget* parent, QlgMainWindow* mw, const QString );
    ~QlgCircuitWindow();
    
    QCanvas* canvas(){ return mainCanvas; };    
    QlgCircuitView* view(){ return circuitView; };
    QlgMainWindow* window(){ return mainWindow; };
    
private:
    QCanvas* mainCanvas;
    QlgCircuitView* circuitView;
    QlgMainWindow* mainWindow;
    QLabel *labelLocked;
    
public slots:
    void setLocked( bool );
    void releaseCursor();
    void changeName( QString );
};
#endif
