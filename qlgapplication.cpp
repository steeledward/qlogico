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

#include "qlgapplication.h"
#include "qlgmainwindow.h"

#include <qobject.h> 
#include <qapplication.h> 
#include <qlabel.h>
#include <qpainter.h>
#include <qsettings.h>
#include <qpixmap.h>

#ifdef Q_WS_WIN
#include <qt_windows.h>
#include <process.h>
#endif

static QLabel *splash = 0;

void set_splash_status( const QString &txt )
{
    if ( !splash )
	return;
    splash->repaint( FALSE );
    QPainter p( splash );
    p.setPen( Qt::white );
    p.setFont( QFont( "Helvetica [Cronyx]", 8 ) );
    p.drawText( 5, splash->fontMetrics().height(), txt );
    QString s = QObject::tr( " Simulador de Circuitos Digitales" );
    p.drawText( 5, 2 * splash->fontMetrics().height() + 1, s );
    QApplication::flush();
}

QlgApplication::QlgApplication( int &argc, char **argv )
    : QApplication( argc, argv )
{
    //setFont( QFont( "Helvetica [Cronyx]", 12 ) );
}

QLabel *QlgApplication::showSplash()
{
    QRect screen = QApplication::desktop()->screenGeometry();
    QRect mainRect;
    QString keybase = settingsKey();
    QSettings config;
    config.insertSearchPath( QSettings::Windows, "/qlogico" );

    bool show = config.readBoolEntry( keybase + "SplashScreen", TRUE );
    mainRect.setX( config.readNumEntry( keybase + "Geometries/MainwindowX", 0 ) );
    mainRect.setY( config.readNumEntry( keybase + "Geometries/MainwindowY", 0 ) );
    mainRect.setWidth( config.readNumEntry( keybase + "Geometries/MainwindowWidth", 500 ) );
    mainRect.setHeight( config.readNumEntry( keybase + "Geometries/MainwindowHeight", 500 ) );
    screen = QApplication::desktop()->screenGeometry( QApplication::desktop()->screenNumber( mainRect.center() ) );

    if ( show ) {
	splash = new QLabel( 0, "splash", WDestructiveClose | WStyle_Customize | WStyle_NoBorder | WX11BypassWM | WStyle_StaysOnTop );
	splash->setFrameStyle( QFrame::WinPanel | QFrame::Raised );
	splash->setPixmap( QPixmap::fromMimeSource( "splash" ) );
	splash->adjustSize();
	splash->setCaption( "Qlogico" );
	splash->move( screen.center() - QPoint( splash->width() / 2, splash->height() / 2 ) );
	splash->show();
	splash->repaint( FALSE );
	QApplication::flush();
	set_splash_status( QObject::tr("Iniciando..." ) );
    }

    return splash;
}

void QlgApplication::closeSplash()
{
    splash->hide();
}
