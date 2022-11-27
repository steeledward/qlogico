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

#ifndef QLGPROJECTMANAGERWINDOW_H
#define QLGPROJECTMANAGERWINDOW_H

#include <qlistview.h>

class QlgMainWindow;
class QlgCircuitWindow;

class QlgProjectManager : public QListView
{
    Q_OBJECT
    
public:    
    QlgProjectManager( QWidget *parent , QlgMainWindow *mw );
    ~QlgProjectManager();
    
    void addFile( QWidget* );
    
private:
    QlgMainWindow *mainWindow;
    QlgCircuitWindow *schWindow;
};
#endif
