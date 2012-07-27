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
//    File: TPZPosition.cpp
//
//    Class:  TPZPosition
//
//    Inherited from: 
// :
//*************************************************************************
//end of header




#include <TPZPosition.hpp>


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZPosition,TPZBase);

//*************************************************************************
//:
//  f: TPZPosition (int x = 0, int y = 0, int z = 0);
//
//  d:
//:
//*************************************************************************

TPZPosition :: TPZPosition(int x, int y, int z)
{
   m_Values[0] = x;
   m_Values[1] = y;
    m_Values[2] = z;       
}


//*************************************************************************
//:
//  f: TPZPosition (const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************

TPZPosition :: TPZPosition(const TPZPosition& pos)
{
   m_Values[0] = pos.m_Values[0];
   m_Values[1] = pos.m_Values[1];
   m_Values[2] = pos.m_Values[2];
}


//*************************************************************************
//:
//  f: TPZPosition & operator = (const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************
      
TPZPosition& TPZPosition :: operator=(const TPZPosition& pos)
{
   m_Values[0] = pos.m_Values[0];
   m_Values[1] = pos.m_Values[1];
   m_Values[2] = pos.m_Values[2];
   return *this;
}


//*************************************************************************
//:
//  f: Boolean operator == (const TPZPosition & pos) const;
//
//  d:
//:
//*************************************************************************
      
Boolean TPZPosition :: operator==(const TPZPosition& pos) const
{
   if( ( m_Values[0] == pos.m_Values[0] ) &&
       ( m_Values[1] == pos.m_Values[1] ) &&
       ( m_Values[2] == pos.m_Values[2] ) )
   {
       return true;
   }
   return false;
}


//*************************************************************************
//:
//  f: Boolean operator! = (const TPZPosition & pos) const;
//
//  d:
//:
//*************************************************************************
      
Boolean TPZPosition :: operator!=(const TPZPosition& pos) const
{
   if( ( m_Values[0] != pos.m_Values[0] ) ||
       ( m_Values[1] != pos.m_Values[1] ) ||
       ( m_Values[2] != pos.m_Values[2] ) )
   {
       return true;
   }
   return false;
}

//*************************************************************************
//:
//  f: TPZPosition operator + (const TPZPosition & pos) const;
//
//  d:
//:
//*************************************************************************
      
TPZPosition TPZPosition :: operator+(const TPZPosition& pos) const
{
   return TPZPosition( m_Values[0] + pos.m_Values[0],
                       m_Values[1] + pos.m_Values[1],
                       m_Values[2] + pos.m_Values[2] );
}


//*************************************************************************
//:
//  f: TPZPosition operator-(const TPZPosition & pos) const;
//
//  d:
//:
//*************************************************************************
      
TPZPosition TPZPosition :: operator-(const TPZPosition& pos) const
{
   return TPZPosition( m_Values[0] - pos.m_Values[0],
                       m_Values[1] - pos.m_Values[1],
                       m_Values[2] - pos.m_Values[2] );
}

//*************************************************************************
//:
//  f: virtual int valueForCoordinate (TPZCoordinate coordinate) const;
//
//  d:
//:
//*************************************************************************
      
int TPZPosition :: valueForCoordinate( TPZCoordinate coordinate) const
{
   switch(coordinate)
   {
      case TPZPosition::X : return m_Values[0];
      case TPZPosition::Y : return m_Values[1];
      case TPZPosition::Z : return m_Values[2];
   }
   
   return 0;
}


//*************************************************************************
//:
//  f: virtual void setValueAt (TPZCoordinate Coordinate, int value);
//
//  d:
//:
//*************************************************************************
      
void TPZPosition :: setValueAt( TPZCoordinate coordinate, int value)
{
   switch(coordinate)
   {
      case TPZPosition::X : m_Values[0] = value;
                            break;
      case TPZPosition::Y : m_Values[1] = value;
                            break;
      case TPZPosition::Z : m_Values[2] = value;
                            break;
   }
   
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************
      
TPZString TPZPosition :: asString() const
{
      TPZString rc = "(";
      rc += TPZString(m_Values[0]) + "," +
            TPZString(m_Values[1]) + "," +
            TPZString(m_Values[2]) + ")";
      return rc;
}

//*************************************************************************

// end of file
