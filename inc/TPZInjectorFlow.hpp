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
//    File: TPZInjectorFlow.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZInjectorFlow_HPP__
#define __TPZInjectorFlow_HPP__

//*************************************************************************

   #include <TPZFlow.hpp>

   #ifndef __TPZQueue_HPP__
   #include <TPZQueue.hpp>
   #endif
   
   #ifndef __TPZMessage_HPP__
   #include <TPZMessage.hpp>
   #endif

   #ifndef __TPZPool_HPP__
   #include <TPZPool.hpp>
   #endif
   
   #ifndef __TPZThread_HPP__
   #include <pthread.h>
   #endif

//*************************************************************************

   class TPZComponent;
   class TPZInterfaz;
   typedef TPZQueue<TPZMessage*> TPZMessageQueue;
   //typedef TPZPool<TPZMessage> TPZMessagePool;
      
//*************************************************************************


   class TPZInjectorFlow : public TPZFlow
   {
      typedef TPZFlow Inhereited;
            
   public:
      TPZInjectorFlow( TPZComponent& component);
      virtual ~TPZInjectorFlow();

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      virtual Boolean outputWriting();
      
      virtual Boolean controlAlgoritm(Boolean info=false, int delta=0);
      
      virtual void sendMessage(TPZMessage* msg);
      unsigned getMessagesToSend() const;
      unsigned getSentFlits() const;
      void     clearSentFlits() {m_flitsSent=0;};
      unsigned getAvaliableMessages();
      
      TPZMessageQueue& getMessageQueue() const
      { return (TPZMessageQueue&)m_MessageQueue; }

      void getQueueKey();
      
      void releaseQueueKey();
      
      // Run time information
      DEFINE_RTTI(TPZInjectorFlow);

      void setMissRouting(int short a)
      {m_MissRouting=a;}

      
      void makeMissRouting(TPZMessage msg);
      void makeMissRouting(TPZMessage msg,int short dim);
      
   protected:
      Boolean             isAnyMessageToSend() const;
      TPZMessage*         getNextFlitToSend();
      unsigned long       m_flitsSent;
      TPZMessageQueue     m_MessageQueue;
      pthread_mutex_t     m_QueueKey;
      
      void setPacketInjectionTime(uTIME time)
         { m_PacketInjectionTime = time; }
      
      uTIME getPacketInjectionTime() const
         { return m_PacketInjectionTime; }
      
                     
private: 
      int short           m_MissRouting;              
      
      TPZMessage*         m_OutputData;
      Boolean             m_DataToSend;
      uTIME               m_PacketInjectionTime;            
};

#endif


// end of file
