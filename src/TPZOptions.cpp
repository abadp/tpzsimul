//
//   Copyright (C) 1998-2011 by Galerna Project, the University of
//   Cantabria, Spain.
//
//   This file is part of the TOPAZ network simulator, originallty developed
//   at the Unviersity of Cantabria
//
//   TOPAZ shares a large proportion of code with SICOSYS which was 
//   developed by V.Puente and J.M.Prellezo
//
//   TOPAZ has been developed by P.Abad, L.G.Menezo, P.Prieto and
//   V.Puente
// 
//  --------------------------------------------------------------------
//
//  If your use of this software contributes to a published paper, we
//  request that you (1) cite our summary paper that appears on our
//  website (http://www.atc.unican.es/topaz/) and (2) e-mail a citation
//  for your published paper to topaz@atc.unican.es
//  
//  If you redistribute derivatives of this software, we request that
//  you notify us and either (1) ask people to register with us at our
//  website (http://www.atc.unican.es/topaz/) or (2) collect registration
//  information and periodically send it to us.
//  
//   --------------------------------------------------------------------
//
//   TOPAZ is free software; you can redistribute it and/or
//   modify it under the terms of version 2 of the GNU General Public
//   License as published by the Free Software Foundation.
//
//   TOPAZ is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//   General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with the TOPAZ simulator; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
//   02111-1307, USA
//
//   The GNU General Public License is contained in the file LICENSE.
//
//     
//*************************************************************************
//:
//    File: TPZOptions.cpp
//
//    Class:  TPZOptions
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



//*************************************************************************

#include <TPZOptions.hpp>

//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZOptions);


//*************************************************************************
//:
//  f: TPZOptions (int argc, char * argv []);
//
//  d:
//:
//*************************************************************************

TPZOptions :: TPZOptions(int argc, char* argv[])
{
   for( int i=1; i<argc; i++ )
   {
      m_OptionsLine += TPZString(argv[i]) + TPZString(" ");
   }
}


//*************************************************************************
//:
//  f: TPZOptions (TPZString const & line);
//
//  d:
//:
//*************************************************************************

TPZOptions :: TPZOptions( const TPZString& line )
            : m_OptionsLine(line) 
{

}


//*************************************************************************
//:
//  f: TPZString getValueForOption (char opt) const;
//
//  d:
//:
//*************************************************************************

TPZString TPZOptions :: getValueForOption(char opt) const
{
   TPZString option = TPZString("-") + TPZString(opt);
   
   unsigned index = m_OptionsLine.indexOf(option);
   if( !index ) return "";
   if( (index>1) && (m_OptionsLine[index-1]!=' ') ) return "";
   
   index += 2;
   
   if( index > m_OptionsLine.size() ) return "";
      
   TPZString line = m_OptionsLine.subString(index);
   line = TPZString::stripBlanks(line);
   
   return line.word(1);
}

//*************************************************************************
//:
//  f: int isOptionActive (char opt) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZOptions :: isOptionActive(char opt) const
{
   TPZString option = TPZString("-") + TPZString(opt);   
   unsigned index = m_OptionsLine.indexOf(option);
   return (index==0) ? false : true;
}


//*************************************************************************

// end of file
