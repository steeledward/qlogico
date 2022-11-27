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

#ifndef QLGTIMECHARTWINDOW_H
#define QLGTIMECHARTWINDOW_H

#include <qmainwindow.h>
	
class QCanvasView;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QAction;
class QActionGroup;
class QToolBar;
class QPopupMenu;
class QToolButton;
class QCanvasView;
class QCanvas;
class QlgCircuitView;
class QStringList;
class QComboBox; 
class QlgTimeChartOptionsDlgImpl;

class QlgTimeChartWindow : public QMainWindow
{
    Q_OBJECT
    
public:    
    QlgTimeChartWindow( QWidget *parent = 0, const char * name = 0, WFlags f = WType_TopLevel  );
    ~QlgTimeChartWindow();
    
    void update( QlgCircuitView* cv);
    void draw( QString var, int value );
    void setLineColor( QColor lc ){ lColor = lc; };
    void setMeshColor( QColor mc ){ mColor = mc; };
    void addMesh();
    void clearMesh();
    
    QCanvas* canvas(){ return timeChartCanvas; };
    QColor lineColor(){ return lColor; };
    QColor meshColor(){ return mColor; };
    
    bool mesh(){ return isMesh; };
    
public slots:
    void showDlg();
    void setColorBackground();
    void zoom();
    void init();
    void nextStep( int );
    void getTrueTable();
    void showOptions();
    
private:
    void setupToolsActions();
    void clearTimeLines();
    void clearCanvas();
    
    QlgTimeChartOptionsDlgImpl *timeChartOptions;
    QToolBar *toolBarTools;
    QComboBox *comboBoxZoom;
    QCanvasView *timeChartView;
    QCanvas *timeChartCanvas;
    QlgCircuitView *circuitView;
    QStringList variables;
    QStringList outputs;    
    QColor lColor;
    QColor mColor;
    
    int **bitListVariables;
    
    bool isMesh;
    
    double zom;
    
    int counter;
    int x0;
    int y0;
};
#endif
