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

#ifndef QUINE_H
#define QUINE_H

#include <vector.h>

#define gbit(a,b)	(((a)>>(b))&1)
#define BIG		32000	/* short integer infinity	*/
#define NMAX		12
#define WORD		16
#define MIMPS		48
#define QTERMS		256
#define LIN		500	    /* longest input line size	*/
#define MTERMS		4096	/* maximum numbers of minterms  */

struct lista
{
  int term;
  int mom;		/* first ancestor */
  int dad;		/* second ancestor */
  int nocare;
  int match;
};     

 char* ReducirEcuacion( int , char *, char * );
  int PChart();
  int  BitCount( int,int );
  void KMAP(char *, char * );
  void PrimePath(int j, int imp, lista *);
  void Primes(int , int , lista *);
  void PairUp(int , int , lista *);
  void Quine(lista *);
  char * Reduction(int uncov);

#endif
  
  
