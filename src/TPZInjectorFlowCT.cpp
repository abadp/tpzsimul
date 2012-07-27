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
//    File: TPZInjectorFlowCT.cpp
//
//    Class:  TPZInjectorFlowCT
//
//    Inherited from:  TPZInjectorFlow
// :
//*************************************************************************
//end of header



#include <TPZInjectorFlowCT.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZInjector_HPP__
#include <TPZInjector.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZInjectorFlowCT,TPZInjectorFlow);

//*************************************************************************
//:
//  f: inline isHeaderSent Boolean () const;
//  d:
//
//  f: inline void setHeaderSent (Boolean value);
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: TPZInjectorFlowCT (TPZcomponent & component);
//
//  d:
//:
//*************************************************************************

TPZInjectorFlowCT :: TPZInjectorFlowCT(TPZComponent& component)
                   : TPZInjectorFlow(component),
                     m_HeaderSent(false)
{

}


//*************************************************************************
//:
//  f: virtual ~ TPZInjectorFlowCT ();
//
//  d:
//:
//*************************************************************************

TPZInjectorFlowCT :: ~TPZInjectorFlowCT()
{

}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//: 
//*************************************************************************

Boolean TPZInjectorFlowCT :: outputWriting()
{
   outputInterfaz() -> clearData();

   if( isAnyMessageToSend())
   {
      if( (!outputInterfaz()->isStopActive())  || isHeaderSent() )
      {
         TPZMessage* msg = getNextFlitToSend();
         m_flitsSent++;
         if(!msg) return true;
  #ifdef PTOPAZ      
         TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
         unsigned n = net->getThreadID(pthread_self());
         msg -> setPoolIndex(n);
#endif  
    
         if( msg->isHeader() || msg->isHeadTail() )
     {
        setHeaderSent(true);
        TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
        TPZPosition source = msg->source();
        net->incrInjectedFlitsAt(source.valueForCoordinate(TPZPosition::X),source.valueForCoordinate(TPZPosition::Y),source.valueForCoordinate(TPZPosition::Z),msg->packetSize());
        if (msg->isHeadTail()) setHeaderSent(false);
     }
         if(msg->isTail() ) setHeaderSent(false);
         
	 outputInterfaz()->sendData(msg);
	 #ifndef NO_TRAZA
            uTIME delayTime = getOwnerRouter().getCurrentTime() ;
            TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
            texto += TPZString(delayTime) + " # " + msg->asString();
            if( msg->isHeader() || msg->isHeadTail() )
            {
               texto += TPZString("  HEADER: ") + 
                        msg->source().asString() + " => " + 
                        msg->destiny().asString();
            }
            TPZWRITE2LOG( texto );
         #endif
     }
   }
   return true;   
}


//*************************************************************************


// end of file
