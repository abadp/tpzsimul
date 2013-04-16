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
//    File: TPZInjectorFlowUC.cpp
//
//    Class:  TPZInjectorFlowUC
//
//    Inherited from:  TPZInjectorFlow
// :
//*************************************************************************
//end of header



#include <TPZInjectorFlowUC.hpp>

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

IMPLEMENT_RTTI_DERIVED(TPZInjectorFlowUC,TPZInjectorFlow);

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

TPZInjectorFlowUC :: TPZInjectorFlowUC(TPZComponent& component)
                   : TPZInjectorFlow(component)                     
{

}


//*************************************************************************
//:
//  f: virtual ~ TPZInjectorFlowCT ();
//
//  d:
//:
//*************************************************************************

TPZInjectorFlowUC :: ~TPZInjectorFlowUC()
{

}


//*************************************************************************
//:
//  f: void sendMessage (const TPZMessage & amessage)
//
//  d:
//:
//*************************************************************************

void TPZInjectorFlowUC :: sendMessage(TPZMessage* msg)
{

   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
   unsigned radioX = net->getSizeX();
   unsigned radioY = net->getSizeY();
   unsigned radioZ = net->getSizeZ();
   
   if (msg->isMulticast())
   {
      unsigned nodes= radioX*radioY*radioZ;
      unsigned long long msgMask=msg->getMsgmask();
     
      for (int i=0; i<nodes; i++)
      {
         unsigned long long auxMask=1;
         auxMask=auxMask << i;
         if ((msgMask & auxMask)!=0)
         {
            TPZMessage* replica= new TPZMessage;
            *replica=*msg;
            unsigned xval= i%radioX;
            unsigned yval= (i/radioX)%radioY;
	    unsigned zval= i/(radioY*radioX);
            TPZPosition destiny = TPZPosition(xval, yval, zval);
            replica->setDestiny(destiny);
            replica->clearMulticast();
#ifdef PTOPAZ
            pthread_mutex_lock(&m_QueueKey);
#endif             
            m_MessageQueue.enqueue(replica);
#ifdef PTOPAZ
            pthread_mutex_unlock(&m_QueueKey);
#endif         
          }
      }

      delete msg;
   }
   
   else
   {        
      Inhereited::sendMessage(msg);
   }
}

//*************************************************************************


// end of file
