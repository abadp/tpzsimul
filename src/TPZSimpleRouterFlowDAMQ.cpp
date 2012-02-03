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
//    File: TPZSimpleRouterFlowDAMQ.cpp
//
//    Class:  TPZSimpleRouterFlowDAMQ
//
//    Inherited from:  TPZSimpleRouterFlow
// :
//*************************************************************************
//end of header



#include <TPZSimpleRouterFlowDAMQ.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZSimpleRouter_HPP__
#include <TPZSimpleRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouterFlowDAMQ,TPZSimpleRouterFlow);

//*************************************************************************
//:
//  f: TPZSimpleRouterFlowDAMQ ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowDAMQ :: TPZSimpleRouterFlowDAMQ( TPZComponent& component)
                 : TPZSimpleRouterFlow(component)
{
}


//*************************************************************************
//:
//  f: ~ TPZSimpleRouterFlowDAMQ ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowDAMQ :: ~TPZSimpleRouterFlowDAMQ()
{
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowDAMQ :: outputWriting()
{
   unsigned inPort;
   unsigned vNet;
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   
      // Through all the buffers.
   for( inPort =1; inPort<=m_ports; inPort++)
   {
      unsigned acum=0;
      for( vNet=1; vNet<=m_vnets; vNet++)
      {
         acum+=m_fifos[(m_ports*(vNet-1))+inPort].numberOfElements();
      }
      if((m_bufferSize - acum) <= sizeForCT)
      {
         if (inPort==m_ports)inputInterfaz(inPort)->sendStopRightNow(1);
	 else	 
	 {
	    for( vNet=1; vNet<=m_vnets; vNet++)
            {
               inputInterfaz(inPort)->sendStopRightNow(vNet);  
            }
	 }
      }
      else
      {
         if (inPort==m_ports)inputInterfaz(inPort)->clearStopRightNow(1);
	 else	 
	 {
	    for( vNet=1; vNet<=m_vnets; vNet++)
            {
               inputInterfaz(inPort)->clearStopRightNow(vNet);  
            }
	 }
      }
   }
   return true;
}

//*************************************************************************
// end of file
