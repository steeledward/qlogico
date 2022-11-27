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

#ifndef QLGFSMOPTIONS_H
#define QLGFSMOPTIONS_H

#include <qstring.h>

class QFont;
class QColor;

class QlgFSMOptions
{
public:    
    QlgFSMOptions( QString nam, int moore,  QString mooren, QFont moorenf, int mei, QString mein, QFont meinf, int meo, QString meon, QFont meonf, int w, int h, QColor c, bool mesh,QString des );
    ~QlgFSMOptions();
    /// Returns the number of moore variables ( state variables )
    int numMooreOutputs(){ return mooreOutputs; };
    /// Return the number of mealy inputs ( number of transition input variables )
    int numMealyInputs(){ return mealyInputs; };
    /// Return the number of mealy outputs ( number of transition output variables )    
    int numMealyOutputs(){ return mealyOutputs; };
    /// Returns the name of the fsm machine
    QString name(){ return m_name; };
    /// Returns the name of the moore names
    QString mooreNames(){ return moNames; };
    /// Returns the mealy input names
    QString mealyInputNames(){ return meInputNames; };
    /// Return the mealy output names
    QString mealyOutputNames(){ return meOutputNames; };
    /// Returns the description machine
    QString description(){ return m_description; };
    /// Returns the Type of Font for the moore variables
    QFont mooreOutsFont(){ return m_mooreOutsFont; };
    /// Returns the Type of Font for the mealy input variables
    QFont mealyInputsFont(){ return m_mealyInputsFont; };
    /// Returns the Type of Font for the mealy output variables
    QFont mealyOutputsFont(){ return m_mealyOutputsFont; };
    /// Returns the color of the machine
    QColor color(){ return m_color; };
    /// Returns if is mesh
    bool mesh(){ return m_mesh; };
    /// Returns the width of the machine
    int width(){ return m_width; };    
    /// Returns the height of the machine
    int height(){ return m_height; };
    void setName( QString newName );
    
private:
    /// Number of moore outputs ( number of state variables )
    int mooreOutputs;
    /// Number of mealy inputs ( transition variables )
    int mealyInputs;
    /// Number of mealy outputs ( transition variables )
    int mealyOutputs;
    /// Name of the machine
    QString m_name;
    /// Names for the moore variables ( state names )
    QString moNames;
    /// Type of font for the moore variables
    QFont m_mooreOutsFont;
    /// Names for the mealy inputs
    QString meInputNames;
    /// Type of font for the mealy input variables
    QFont m_mealyInputsFont;
    /// Names for the mealy outputs
    QString meOutputNames;
    /// Type of font for the mealy output variables
    QFont m_mealyOutputsFont;
    /// Description of the machine
    QString m_description;
    /// The width
    int m_width;
    /// The height
    int m_height;
    /// The color background
    QColor m_color;
    ///  To check the mesh
    bool m_mesh;
};
#endif
