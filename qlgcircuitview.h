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

#ifndef QLGCIRCUITVIEW_H
#define QLGCIRCUITVIEW_H

#include <qcanvas.h>
#include <qptrlist.h>

#include "qlgboolfunction.h"
#include "qlgcircuitcomponent.h"
#include "qlgcircuit.h"

class QlgCircuitLine;
class QPopupMenu;
class QlgCircuitWindow;
class QlgCircuitView;

typedef QPtrList<QlgCircuitComponent> QlgCircuitCompList;
typedef QPtrList<QlgCircuit> QlgCircuitList;
typedef QPtrList<QlgBoolFunction> QlgBoolFunctionList;

static const int imagePointRTTI = 984397;
class QlgCircuitView;

class QlgPoint: public QCanvasRectangle
{
public:    
    QlgPoint( QlgCircuitView* c, int x, int y );
    //void moveBy( double x, double y );
    
protected:    
    void drawShape( QPainter &p );
    
private:
    QlgCircuitView *circuitView;
};

class ThreadUpdateCircuit : public QThread 
{
public:
    ThreadUpdateCircuit( QlgCircuit* cir ) { threadCircuit = cir; };
    virtual void run();
    QString value;
    QString variables;
    QlgCircuit* circuit(){ return threadCircuit; };
    
private:    
    QlgCircuit* threadCircuit;    
};

class QlgCircuitView : public QCanvasView {
    Q_OBJECT
    
public:
    QlgCircuitView(QlgCircuitWindow* w, const QString name );
    
    void clear();
    void addCircuit( QlgBoolFunctionList vEqns );
    void addCircuitJK( QlgBoolFunctionList vEqns, int numFF, int numOuts );
    void addComponent( QString name );
    void clearMesh();
    void simulateCircuit( QString vars, QString value );   
    void changeName( QString n );
    void setDescription( QString d );
    void setZoom( double newZoom );
    void updateInputVariables( QlgCircuitCompList& );    
    void updateInputVariables();
    void setLineColor( QColor lc ){ lColor=lc; };
    void setMeshColor( QColor mc ){ mColor=mc; };
    
    double zoom(){ return zom; };
    
    bool mesh(){ return isMesh; };
    bool isLocked(){ return locked; };
    
    int numInputs(){ return vInputs.count(); };
    
    QlgCircuitCompList& inputVariables(){ return vInputs; };
    QlgBoolFunctionList& functions(){ return vFunctions; };
    QlgCircuit * circuit(){ return mainCircuit; };    
    QString name(){ return viewName; };
    QString description(){ return viewDescription; };
    QColor lineColor(){ return lColor; };
    QColor meshColor(){ return mColor; };    
    QlgCircuitWindow* window(){ return viewWindow; };
    QPtrList<ThreadUpdateCircuit>& circuits(){ return vThreadUC; };
    
public slots:
    void showProperties();
    void removeComponent();
    void releaseCursor();
    void addMesh();
    
private slots:
    void updateMesh();
    
protected:
    void contentsMousePressEvent( QMouseEvent* );
    void contentsMouseMoveEvent( QMouseEvent* );
    void contentsMouseReleaseEvent( QMouseEvent* );
    void contentsMouseDoubleClickEvent( QMouseEvent *);
    
private:
    QString viewName;
    QString viewDescription;
    QlgCircuit *mainCircuit;
    QlgBoolFunctionList vFunctions;
    QlgCircuitCompList vInputs;
    QlgCircuitCompList vOutputs;    
    QlgCircuitList vCircuits;
    QPtrList<ThreadUpdateCircuit> vThreadUC;    
    QPopupMenu *menu;
    QlgCircuitWindow* viewWindow;
    
    void updateCircuitsMode();
    
    int x0; // Absolut distance in X-Axis
    int y0; // Absolut distance in Y-Axis
    int lastWidth; // To know wich is the last width of the schematic editor  
    
    double zom;    
    
    bool isMesh;
    bool locked;
    
    QColor lColor;
    QColor mColor;
    
    QString newComponent;    
    QCanvasItem* moving;
    QPoint moving_start;    
    
    QlgCircuitComponent* circuitComponent;
    QlgCircuitComponent* ccSelected;
    QlgCircuitLine* lineSelected;
    QlgCircuitLine* line;
    QlgCircuitLine* newLine;
    QPtrList<QlgPoint> vMesh;
    
signals:
    void editCircuitComponent( QlgCircuitComponent* );
    void editView( QlgCircuitView* );
    void lockChanged( bool );
    void cursorReleased();
    void showDescription( QString );
    void nameChanged( QString );
};
#endif

