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

#include <stdio.h>
#if defined(Q_WS_X11)
#include <unistd.h> /* needed for ioperm() */
#include <sys/io.h>
#endif

#include "qlgoutp.h"

#define PORT 0x378
#define STATUS PORT+1
#define CONTROL PORT+2

QlgOutp::QlgOutp() {  
#if defined(Q_WS_X11)
    if ( ioperm(PORT,3,1) ) {
	printf( "Imposible acceder al puerto paralelo\n" );
	printf("Se necesitan permisos de administrador ");
    }
    else
	outb( 0, PORT );
#endif    
}

void QlgOutp::sendData( int data )
{
#if defined(Q_WS_X11)    
    outb( data, PORT );
#endif    
}
