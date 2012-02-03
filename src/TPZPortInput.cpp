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
//    File: TPZPortInput.cpp
//
//    Class:  TPZPortInput
//
//    Inherited from:  TPZPort
// :
//*************************************************************************
//end of header



#include <TPZPortInput.hpp>

#ifndef __TPZMessage_HPP__
#include <TPZMessage.hpp>
#endif

//************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZPortInput, TPZPort);

//************************************************************************
//:
//  f: TPZPortInput (const TPZComponentId & id);
//
//  d:
//:
//************************************************************************

TPZPortInput :: TPZPortInput( const TPZComponentId& id )
              : TPZPort(id)
{

}

//************************************************************************
//:
//  f: virtual ~ TPZPortInput ();
//
//  d:
//:
//************************************************************************

TPZPortInput :: ~TPZPortInput()
{

}


//************************************************************************
//:
//  f: virtual Boolean sendDataRightNow (const TPZMessage & msg);
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: sendDataRightNow(TPZMessage* msg)
{
   // A port of entry can not send data.
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean clearDataRightNow ();
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: clearDataRightNow()
{
   // A port of entry can not manipulate the input signals.
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean sendStopRightNow ();
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: sendStopRightNow()
{
   if( sendStop() )
   {
      TPZConnectionSet::Cursor cursor(m_ConnectionSet);
      forCursor(cursor)
      {
         cursor.element()->sendStop();
      }
      return true;
   }
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean clearStopRightNow ();
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: clearStopRightNow()
{
   if( clearStop() )
   {
      TPZConnectionSet::Cursor cursor(m_ConnectionSet);
      forCursor(cursor)
      {
         cursor.element()->clearStop();
      }
      return true;
   }
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean onDataUp (unsigned interface, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: onDataUp(unsigned interfaz, unsigned channel)
{
   return getOwner() -> onDataUp(getOwnerInterfazId(),getChannel());
}


//************************************************************************
//:
//  f: virtual Boolean onDataDown (unsigned interface, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: onDataDown(unsigned interfaz, unsigned channel)
{
   return getOwner()->onDataDown(getOwnerInterfazId(),getChannel());
}


//************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: onStopUp(unsigned interfaz, unsigned channel)
{
   // One input port receives stop sign.
   return false;
}


//************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned channel);
//
//  d:
//:
//************************************************************************

Boolean TPZPortInput :: onStopDown(unsigned interfaz, unsigned channel)
{
   // One input port receives no stop sign.
   return false;
}


//************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//************************************************************************

TPZString TPZPortInput :: asString() const
{
   TPZString texto ="IPort:";
   texto += getOwner()->asString();
   return texto;
}


//************************************************************************


// end of file
