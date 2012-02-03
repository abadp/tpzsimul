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
//    File: TPZSimpleRouterFlow.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZSimpleRouterFlow_HPP__
#define __TPZSimpleRouterFlow_HPP__

//*************************************************************************

   #include <TPZFlow.hpp>

   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif

   #ifndef __TPZQueue_HPP__
   #include <TPZQueue.hpp>
   #endif

   #ifndef __TPZPair_HPP__
   #include <TPZPair.hpp>
   #endif
      
//*************************************************************************

   class TPZMessage;
   
//*************************************************************************
   
   typedef TPZQueue<TPZMessage*>      TPZMessageQueue;

//*************************************************************************

   class TPZSimpleRouterFlow : public TPZFlow
   {
      typedef TPZFlow Inhereited;
   
   public:
      TPZSimpleRouterFlow( TPZComponent& component);
      virtual ~TPZSimpleRouterFlow();

      virtual void initialize();
      virtual void terminate();
      
      virtual void run(uTIME runTime);      
      virtual void preRun(uTIME time);
      virtual void postRun(uTIME time);
      
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();

      virtual Boolean onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean onStopDown(unsigned interfaz, unsigned cv);
      virtual TPZString getStatus() const;
      
      // Run time information
      DEFINE_RTTI(TPZSimpleRouterFlow);

   protected:
      virtual  void    cleanOutputInterfaces();
      virtual  void    cleanInputInterfaces();
      virtual  Boolean propagateStop();
      virtual  Boolean updateMessageInfo(TPZMessage* msg);
      virtual  Boolean dispatchEvent(const TPZEvent& event);
      
      virtual  unsigned extractOutputPortNumber(TPZROUTINGTYPE dir);
      virtual  unsigned extractOutputPortNumber(TPZMessage* msg);
      
      unsigned extractOutputChannel(TPZMessage* msg) const;

   protected:
      Boolean cleanInterfaces() const
      { return m_CleanInterfaces; }
      void setCleanInterfaces(Boolean value)
      { m_CleanInterfaces = value; }
      TPZROUTINGTYPE  getOutputDirection(TPZMessage* msg);
      Boolean m_CleanInterfaces;
   protected:
      unsigned *m_connections;
      unsigned m_bufferSize;
      unsigned m_token;
      int* m_routingtime;
      TPZMessage** m_routing;
      TPZMessage** m_sync;
      TPZMessageQueue* m_fifos;
      unsigned* m_shortcut;
      unsigned m_ports;
      unsigned m_vnets;
                  
};

//*************************************************************************


#endif


// end of file
