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

#ifndef QLGFSMVIEW_H
#define QLGFSMVIEW_H

#include <qcanvas.h>
#include <qptrlist.h>

class QlgState;
class QlgTransition;
class QlgFSMOptions;
class QlgFSMMachine;

class QlgFSMView : public QCanvasView 
{    
    Q_OBJECT

public:
    QlgFSMView(QCanvas* canvas, QWidget* parent );
    
    void addMachine( QlgFSMOptions * );
    void clear();    
    void clearMesh();
    void addMesh();
    void setZoom( double newZoom );
    void setOptions( QlgFSMOptions * );    
    void changeName( QString );
    
    /// Returns a value to know if the view is mesh
    bool mesh(){ return isMesh; };
    /// Returns the actual zoom
    double zoom(){ return zom; };
    QString name(){ return viewName; };
    /// To know which is the active machine
    QlgFSMMachine *fsmMachine;

protected:
    void contentsMousePressEvent(QMouseEvent*);
    void contentsMouseDoubleClickEvent ( QMouseEvent * e );
    void contentsMouseMoveEvent(QMouseEvent*);    
    
private:    
    /// Name of the view
    QString viewName;
    /// Description of the view
    QString viewDescription;
    /// Values for the FSM
    QlgFSMOptions *viewOptions;
    /// Title of the view
    QCanvasText title;
    ///  States in move
    QCanvasItem* moving;
    /// To know from where move
    QPoint moving_start;
    /// Actual zoom of the view    
    double zom;
    /// To know if the view is mesh
    bool isMesh;
    /// Points for draw the mesh
    QPtrList<QCanvasRectangle> vMesh;
    
signals:
    void editState( QlgState * );
    void editTransition( QlgTransition * );
    void editFSM( QlgFSMView * );
    void showDescription( QString );
    void nameChanged( QString );
};

#endif
