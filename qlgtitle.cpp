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

#include "qlgtitle.h"

QlgTitle::QlgTitle( QCanvas* canvas, QString text )
	: QCanvasText( canvas )
{
    setText( text );
    move( 100, 10 );
    show();
    canvas->update();
 
}

void QlgTitle::drawShape( QPainter& p )
{
    p.setFont( QFont( "Helvetica", 18, QFont::Bold, true ) );
    p.setPen( Qt::red );
    p.drawText( (int)x(), (int)y(), text(), -1, QPainter::Auto  );
}

void QlgTitle::moveBy( double x, double y )
{
    QCanvasText::moveBy( x, y );         
}

