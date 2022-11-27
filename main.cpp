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

#include <stdlib.h>
#include <iostream.h>
#include <qtranslator.h>
#include <qapplication.h>
#include <qstring.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qtextstream.h>
#include <qlabel.h>
#include <qwindowsstyle.h>

#include "qlgmainwindow.h"
#include "qlgapplication.h"

QString loadLanguage(QTranslator* trans);

int main( int argc, char ** argv )
{
    QString lang;
    QlgApplication a( argc, argv );        
    
    QLabel *splash = a.showSplash();
    
    QTranslator trans(0);
    lang = loadLanguage(&trans);
    a.installTranslator(&trans);
  
    QlgMainWindow *mw = new QlgMainWindow;
    QApplication::setStyle( new QWindowsStyle() );
    a.setMainWidget(mw);
    mw->show();     
        
    delete splash;       
    
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
  
    return a.exec();
}

/**********************************************************
function that seek for the languaje to be used on the local configuration,
for each user the languaje could be different english, spanish, etc
***********************************************************/

QString loadLanguage(QTranslator* trans)
{
  QString qlpath;
  QDir dir = QDir::home();
  QDir qldir(dir.absPath()+"/.qlogico");
  if (!qldir.exists())
  {
    if (!dir.mkdir(".qlogico"))
      qDebug(".qlogico not created");
//    qDebug(dir.absPath());
  }

  QFile file(qldir.absPath()+"/language");
  QTextStream fin(&file);
  
//  qDebug(file.name());

  if (file.isOpen())
  {
    qDebug("language file already open");
    file.close();
  }
  if (!file.exists())
    qDebug("language file does not exist");
  if (!file.open(IO_ReadOnly))
  {
    qDebug("language file could not be opened.");
    return QString::null;
  }

  QString s;
  fin >> s;

  qlpath = getenv("QLDIR");
  QDir podir(qlpath);

  if (podir.cd("translations"))
    trans->load(s, podir.absPath());

  file.close();

  return s;
}
