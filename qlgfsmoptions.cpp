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

#include <qfont.h>
#include <qcolor.h>

#include "qlgfsmoptions.h"

/****************************************************************
  Constructor for the options of a finite state machine
  @ nam - name of the machine
  @ moore - number of the moore variables
  @ mooren - names for the moore variables
  @ moorenf - font for the moore variables
  @ mei - number of mealy inputs
  @ mein - names for the mealy inputs
  @ meinf  - font for the mealy input variables
  @ meo - number of mealy outputs
  @ meon - names for the mealy outputs
  @ meonf - font for the mealy output variables
  @ des - description of the machine
****************************************************************/  

#include <qfont.h>

#include "qlgfsmoptions.h"

QlgFSMOptions::QlgFSMOptions( QString nam, int moore,  QString mooren, QFont moorenf, int mei, QString mein, QFont meinf, int meo, QString meon, QFont meonf, int w, int h, QColor c, bool 	mesh,QString des )
{
    m_name = nam;
    mooreOutputs = moore;
    mealyInputs = mei;
    mealyOutputs = meo;
    moNames = mooren;
    m_mooreOutsFont = moorenf;
    meInputNames = mein;
    m_mealyInputsFont = meinf;
    meOutputNames = meon;
    m_mealyOutputsFont = meonf;
    m_width = w;
    m_height = h;
    m_color = c;
    m_mesh = mesh;
    m_description = des;
}

/*********************************************************
  Changes the name of the machine
*********************************************************/

void QlgFSMOptions::setName( QString newName )
{
    m_name = newName;
}

QlgFSMOptions::~QlgFSMOptions()
{
    
}
