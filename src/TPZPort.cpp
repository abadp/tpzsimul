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
//    File: TPZPort.cpp
//
//    Class:  TPZPort
//
//    Inherited from:  TPZComponent
// :
//*************************************************************************
//end of header


//Borja


#include <TPZPort.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

//************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZPort,TPZComponent);

//************************************************************************
//:
//  f: virtual Boolean sendDataInmediately (const TPZMessage & msg) = 0;
//  d:
//
//  f: virtual Boolean receiveData (const TPZMessage & msg) = 0;
//  d:
//
//  f: inline virtual void ClearData ();
//  d:
//
//  f: inline virtual void clearDataInmediately ();
//  d:
//
//  f: inline TPZMessage getData () const;
//  d:
//
//  f: inline isMessageValid Boolean () const;
//  d:
//
//:
//************************************************************************

//************************************************************************
//:
//  f: TPZPort (const TPZComponentId & id);
//
//  d:
//:
//************************************************************************

TPZPort :: TPZPort(const TPZComponentId& id)
         : TPZComponent(id),
           m_Ready(false),
           m_Stop(false),
           m_DataChanged(false),
           m_StopChanged(false),
           m_CurrentConnection(0)
{
   setAutoDeleteConnectionSet(false);
   m_MessageStored=0;
}

   
//************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//************************************************************************

void TPZPort :: run(uTIME runTime)
{
   // by default, does nothing.
}


//************************************************************************
//:
//  f: getOwnerInterfazId unsigned () const
//
//  d:
//:
//************************************************************************

unsigned TPZPort :: getOwnerInterfazId() const
{
   TPZInterfaz* interfaz = (TPZInterfaz*)getOwner();
   return interfaz->getNumberId();
}


//*************************************************************************
//:
//  f: Boolean establishConnection (TPZConnection * connection);
//
//  d:
//:
//*************************************************************************

Boolean TPZPort :: establishConnection(TPZConnection* connection)
{
   return m_ConnectionSet.add(connection);
}


//*************************************************************************
//:
//  f: Boolean sendData (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

Boolean TPZPort :: sendData(TPZMessage* msg)
{
   setDataChanged(((!m_Ready) ? true:false));
   m_Ready = true;
   setData(msg);
   return true;
}


//************************************************************************
//:
//  f: void setDataChanged (Boolean change);
//
//  d:
//:
//************************************************************************

void TPZPort :: setDataChanged(Boolean change)
{ 
   if ( m_DataChanged != change )
   {
      unsigned conns = m_ConnectionSet.numberOfElements();
      if( conns > 1 )
      {
         if( (++m_CurrentConnection) == conns ) 
         {
            m_DataChanged = change;
            m_CurrentConnection = 0;
         }
      }
      else
      {
         m_DataChanged = change;
      }
   } 
}

//************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//************************************************************************

TPZString TPZPort :: asString() const
{
   return "Port";
}

//************************************************************************


// end of file
