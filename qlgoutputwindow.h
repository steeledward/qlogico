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

#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <qtabwidget.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qlistview.h>

class QTextEdit;
class QGridLayout;

class QlgOutputWindow : public QTabWidget
{
    Q_OBJECT
    
public:    
    QlgOutputWindow( QWidget *parent );
    ~QlgOutputWindow();    

public slots:
    void showDescription( QString  );
    
private:    
    QTextEdit *textEdit;
    QWidget *tab;
    QGridLayout *tabLayout;
};
#endif
