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
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <iostream.h>
#include "qlgquine.h"



 vector<int> impcnt;
 vector<int> impext;
 vector<int> essprm;
 vector<int> pricare;  	/* valores no importa para implicantes primos	*/
 vector<int> priterm;  	/* terminos para implicantes primos */
 vector<int> minterm;
 vector<int> noterm;   	/* no importa en mapa de Karnaugh		*/
 vector<char> vname;
 int **impchart;   
 int nterms = 1; 	   	/* numeros de terminos en caso		*/
 int nwords;		/* nterms/WORD + 1			*/
 int pptr = 0;		/* numero de actual nterms		*/
 int nvars;
 int imps = 0;		/* numero de  primos implicantes		*/

//---------------------------------------------------------------------------

int PChart()
{

    int i, j, k;
    int uncov;
    int temp;
    char echar;

    /* determinar coverage de miniterminos */

    for ( i=0; i<nwords; i++ ) {
	for ( j=0; j<imps; j++ )
	    impcnt[i] |= impchart[i][j];
    }

    /* determinar multiple coverage de miniterminos */

    for ( i=0; i<nterms; i++ ) {
	temp = 0;
	for ( j=0; j<imps; j++ )
	    temp += (gbit(impchart[i/WORD][j],i%WORD));
	if ( temp >= 2 ) impext[i/WORD] |= (1<<(i%WORD));
    }

    /* excluir terminos no-importa */

    for ( i=0; i<nwords; i++ ) {
	/* eliminar casos no-importa */
	impcnt[i] &= ~noterm[i];
	impext[i] &= ~noterm[i];
	/* checar por implicantes primos */
	for ( j=0; j<WORD; j++ ) {
	    if ( (gbit(impcnt[i],j) == 1) && (gbit(impext[i],j)==0) ) {
		k = 0;
		while ( gbit(impchart[i][k],j) == 0 ) k++;
		essprm[k] = 1;
	    }
	}
    }

    /* Determine coverage of essential prime implicants and */

    for ( i=0; i<imps; i++ ) {
	if ( essprm[i] == 1 ) {
	    echar = '*';
	    for ( j=0; j<nwords; j++ )
		impcnt[j] &= ~impchart[j][i];
	} 
	else 
	    echar = ' ';
    }

    uncov = 0;
    for ( i=0; i<nwords; i++ ) {
	uncov += BitCount(WORD,impcnt[i]);
    }
    return(uncov);
}

//---------------------------------------------------------------------------


int BitCount(int len,int term)
{
    int i;
    int count = 0;

    for ( i=0; i< len; i++ )
	count+=(term>>i)&1;
    return(count);
}

//---------------------------------------------------------------------------

void KMAP(char *MinTerminos, char *NoImporta)
{
    char entry[WORD];
    int i=0;
    int j;
    int num;

    /* Determinar numero maximo de miniterminos */

    nterms = int(pow(2, nvars));
    nwords = ( ( nterms + ( WORD - 1 ) ) / WORD );

    for (int i=0; i<QTERMS; i++ ) {
	for ( j=0; j<QTERMS; j++ )
	    impchart[i][j] = 0;
    }

    for(int i=0; i<nvars; i++)
	vname.push_back( (char)('A' + i) );

    /* Procesar miniterminos */

    i = 0;
    while (sscanf(&MinTerminos[i],"%s",entry) != EOF  ){
	j = 0;
	while ( entry[j++] != 0  )
	    i += j;
	sscanf(entry,"%d", &num );
	minterm[num/WORD] |= (1<<(num%WORD));
    }

    /* Procesar terminos no importa */

    i = 0;
    while (sscanf(&NoImporta[i],"%s",entry) != EOF  ) {
	j = 0;
	while ( entry[j++] != 0 );
	i += j;
	sscanf(entry,"%d", &num );
	noterm[num/WORD] |= (1<<(num%WORD));
    }
}

//---------------------------------------------------------------------------

void PrimePath(int j, int imp, lista *plista)
{
    if ( j < pptr ) {
	// Llegada de los terminos originales
	impchart[plista[j].term/WORD][imp] |= (1<<(plista[j].term%WORD));
    } 
    else {
	PrimePath(plista[j].mom,imp, plista);
	PrimePath(plista[j].dad,imp, plista);
    }
}

//---------------------------------------------------------------------------

void Primes(int first, int last, lista *plista)
{
    int i, j;
    int flag;
    int rep=0;

    /* Salida de implicantes primos */

    for ( j=first; j<last; j++ ) {
	if ( plista[j].match == 0 ) {
	    flag = 0;
	    for ( i=0; i<imps; i++ ) {
		// Prueba si existe un candidato como un subconjunto largo
		if (BitCount(nvars,plista[j].nocare) <= BitCount(nvars,pricare[i]) ){
		    if (((plista[j].nocare|pricare[i]) == (pricare[i])) &&
			(((~pricare[i])&priterm[i])==((~pricare[i])&plista[j].term))&&
			((plista[j].term|priterm[i]|pricare[i]) ==
			 (priterm[i]|pricare[i]))) {
			flag = 1;
		    }
		    // Prueba si el candidato puede ser remplazado por uno menor
		} 
		else if
			(BitCount(nvars,plista[j].nocare)>BitCount(nvars,pricare[i])) {
		    if (((pricare[i]|plista[j].nocare)==(plista[j].nocare)) &&
			(((~plista[j].nocare)&plista[j].term) ==
			 ((~plista[j].nocare)&priterm[i])) &&
			((priterm[i]|plista[j].term|plista[j].nocare) ==
			 (plista[j].term|plista[j].nocare))) {
			flag = 2;
			rep = i;
		    }
		}
	    }

	    if (flag == 0) {
		PrimePath(j,imps, plista);
		priterm[imps] = plista[j].term;
		pricare[imps] = plista[j].nocare;
		imps++;
		if ( imps >= MIMPS ) {
		    cout << "Overflow chart";
		    imps--;
		}

	    } 
	    else if (flag == 2) {
		PrimePath(j,rep, plista);
		priterm[rep] = plista[j].term;
		pricare[rep] = plista[j].nocare;
	    }
	}
    }
}

//---------------------------------------------------------------------------

void PairUp(int first, int last, lista *plista)
{
    int match = 0;		/* indicates a pairing was found		*/
    int submatch = 0;	/* pairing found on one pass			*/
    int diff, diffx = 0;		/* nocare term variables			*/
    int fterm=0;
    int dterm=0;		/* first term in loop parameters		*/
    int next;		/* pointer to next available plist location	*/
    int jstart, second;	/* pointers within the level			*/
    int j2;
    int j, k;

    jstart = first;
    second = first;
    next = last;		/* initialize loop controls			*/

    j = jstart;
    j2 = jstart;

    while ( jstart< last-1 ) {
	while ( j2 < (last-1) ) {
	    for ( k=second+1; k<last; k++ ) {
		// Prueba completa del metodo Quine-McCluskey
		if ((plista[j].nocare == plista[k].nocare ) && (BitCount(nvars,plista[j].term) == (BitCount(nvars,plista[k].term)-1)) && (BitCount(nvars,diff=(plista[k].term^plista[j].term))==1) ) {
		    if ((diffx==0)||((((plista[j].term-fterm)%dterm) == 0) &&
				     (diff==diffx))) {
			match = 1;
			submatch = 1;
			if ( diffx == 0 ) {
			    dterm = plista[k].term-plista[j].term;
			    fterm = plista[j].term;
			}
			plista[j].match = 1;
			plista[k].match = 1;
			plista[next].match = 0;
			plista[next].nocare = plista[j].nocare|diff;
			plista[next].term = plista[j].term;
			plista[next].mom = j;
			plista[next++].dad = k;
			second = k;
			diffx = diff;
			j = ++k;
		    }
		}
	    }

	    if ( submatch == 1 ) {
		second += 2;
		j = second;
		submatch = 0;
	    } 
	    else {
		j = ++j;
		j2 = j;
		second = j;
	    }
	}
	if ( match == 1 ) {
	    PairUp(last,next, plista);
	    j2 = plista[last].mom;
	    j = j2;
	    second = plista[last].dad;
	    next = last;
	    match = 0;
	    diffx = 0;
	} 
	else {
	    jstart++;
	    second = jstart;
	    j = jstart;
	}
    }
    // Process el  primo implicante */
    Primes(first,last, plista);
}

//---------------------------------------------------------------------------

void Quine(lista *plista)
{
    int i;
    int j;

    /* Inicia la cuenta de implicantes primos */

    for ( j=0; j<nwords; j++ )
    {
	impcnt[j] = 0;
	impext[j] = 0;
    }

    for ( j=0; j<MIMPS; j++ ) 
	essprm[j] = 0;
    /* Activa lista de paridades */

    for ( i=0; i<nterms; i++ ) {
	if (gbit((minterm[i/WORD]|noterm[i/WORD]),i%WORD) == 1 ) {
	    plista[pptr].nocare = 0;
	    plista[pptr].match = 0;
	    plista[pptr].mom = 1;
	    plista[pptr].dad = 0;
	    plista[pptr++].term = i;
	    if ( pptr >= MTERMS ) {
		i = nterms;
		pptr = 0;	/* nullify process */
	    }
	}
    }

    /* Procesa paridades */

    PairUp(0, pptr, plista);
}

//---------------------------------------------------------------------------

char * Reduction(int uncov)
{
    int i,j;
    int nonemps;		// Numero de terminos no-esenciales
    int terms, lits;		// Minimizacion de factores
    int nons[MIMPS];	/* index into impchart of non-essential impl.*/
    int termlist[QTERMS];	/* temporary location of covered term count  */
    int fail;			/* new candidate flag				*/
    long limit, li;		/* power set bits				*/
    char oper=0;		/* sum of products separator			*/

    struct current {
	int terms;
	int lits;
	int list[MIMPS];
    } curbest;

    if ( uncov != 0 ) {
	// No existe expression unica;
	// Lista de no esenciales implicantes
	j = 0;
	for ( i=0; i<imps; i++ )
	    if (essprm[i] == 0) nons[j++] = i;
	nonemps = j;

	if ( nonemps > 2*WORD ) {
	    fprintf(stderr,"Warning! Only %d prime implicants can be",2*WORD);
	    fprintf(stderr,"considered for coverage\n of all terms (in addition");
	    fprintf(stderr,"to essential primes). %d implicants not checked\n",
		    nonemps-(2*WORD));
	    nonemps = 2*WORD;
	}

	limit = 1;
	for ( i=0; i<nonemps; i++ ) limit *= 2;

	curbest.terms = BIG;
	curbest.lits = BIG;
	curbest.list[0] = -1;

	for ( li=1L; li<limit; li++ ) {
	    terms = BitCount(2*WORD,li);
	    if ( terms <= curbest.terms ) {
		lits = 0;

		for ( i=0; i<nwords; i++ )
		    termlist[i] = impcnt[i];

		for ( i=0; i<nonemps; i++ ) {
		    if (((li>>i)&1L) == 1L) {
			for ( j=0; j<nterms; j++ ) {
			    if (gbit(impchart[j/WORD][nons[i]],j%WORD)==1) {
				termlist[j/WORD] &= ~(1<<(j%WORD));
			    }
			}
		    }
		    lits += (nvars - BitCount(nvars,pricare[nons[i]]));
		}

		fail = 0;
		for ( i=0; i<nwords; i++ ) {
		    if ( termlist[i]!=0 ) fail = 1;
		}

		if ((fail==0) && ((terms<curbest.terms) || (lits<curbest.lits))) {
		    // we have a new candidate
		    curbest.terms = terms;
		    curbest.lits = lits;
		    j = 0;
		    for ( i=0; i<nonemps; i++ ) {
			if (((li>>i)&1L)==1L) {
			    curbest.list[j++] = nons[i];
			}
		    }
		    curbest.list[j] = -1;
		}
	    }
	}

	j = 0;
	while ( curbest.list[j] >= 0 ) {
	    essprm[curbest.list[j]] = 1;
	    j++;
	}
    }

/* Imprimr expresion minima */
    
    char ecuacion[256] = "";
    char temp[256] = "";
    char *pecuacion = &ecuacion[0];
	    
    for ( i=0; i<imps; i++ ) {
	if ( essprm[i] == 1 ) {
	    sprintf(temp, "%c", oper);
	    strcat( ecuacion, temp );
	    oper = 0;
	    for ( j=0; j<nvars; j++ ) {
		if (((pricare[i]>>(nvars-1-j))&1) == 0) {
		    sprintf(temp, "%c", oper);
		    strcat( ecuacion, temp );
		    sprintf(temp, "%c", vname[j]);
		    strcat( ecuacion, temp );
		    if (((priterm[i]>>(nvars-1-j))&1) == 0) 
			strcat ( ecuacion, "'" );
		    oper = '*';
		}		
	    }
	    oper = '+';
	}
    }

    return( pecuacion );
}
//---------------------------------------------------------------------------

char * ReducirEcuacion(int NVariables, char *MinTerminos, char *NoImporta)
{

    impchart = new int*[QTERMS];	
    for (int j = 0; j < QTERMS; j++)
	impchart[j] = new int[QTERMS];

    impcnt.assign(MTERMS, 0);
    impext.assign(MTERMS, 0);
    essprm.assign(MIMPS, 0);
    pricare.assign(MIMPS, 0);
    priterm.assign(MIMPS, 0);
    noterm.assign(MTERMS, 0);
    minterm.assign(MTERMS, 0);
    vname.clear();

    lista *plista1 = new lista[MTERMS*2];

    nterms = 1;		
    nwords = 0;		
    pptr = 0;		
    imps = 0;	

    int i;
    nvars = NVariables;

    KMAP(MinTerminos, NoImporta);
    Quine(plista1);
    i=PChart();
    char *Ecuacion = Reduction(i);
    delete [] plista1;

    for (int j = 0; j < MIMPS; j++)
	delete[] impchart[j];

    minterm.clear();

    return ( Ecuacion );
}
