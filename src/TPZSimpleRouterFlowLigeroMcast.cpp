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
//    File: TPZSimpleRouterFlowLigeroMcast.cpp
//
//    Class:  TPZSimpleRouterFlowLigeroMcast
//
//    Inherited from:  TPZSimpleRouterFlowLigero
// :
//*************************************************************************
//end of header



#include <TPZSimpleRouterFlowLigeroMcast.hpp>

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

#define CYC_TRAZA 0

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouterFlowLigeroMcast,TPZSimpleRouterFlowLigero);

//*************************************************************************
//:
//  f: TPZSimpleRouterFlowTorus ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowLigeroMcast :: TPZSimpleRouterFlowLigeroMcast( TPZComponent& component)
                 : TPZSimpleRouterFlowLigero(component)
{
}


//*************************************************************************
//:
//  f: ~ TPZSimpleRouterFlow ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowLigeroMcast :: ~TPZSimpleRouterFlowLigeroMcast()
{
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlowLigeroMcast :: inputReading()
{  
   unsigned outPort;
   unsigned inPort;
   unsigned virtualChannel=1;
   cleanOutputInterfaces();
   TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();

#ifndef NO_TRAZA
   TPZString txt_comp= getComponent().asString() + TPZString(" TIME: ") + TPZString(getOwnerRouter().getCurrentTime());   
#endif
     
   //**********************************************************************************************************
   //**********************************************************************************************************
   // PART 0: ESCAPE PATH
   //**********************************************************************************************************
   //**********************************************************************************************************
   if( (! m_latch_escape ) && (m_fifo_escape.numberOfElements()!=0) )
   {
      m_fifo_escape.dequeue(m_latch_escape);
   }
   
   m_clearEscConnection=false; 
   
   if(m_latch_escape)
   {
      Boolean checkFlow=true;
      
      if (m_latch_escape->isHeader() || m_latch_escape->isHeadTail())
      {
         m_escape_ctrl= getOutputDirection(m_latch_escape);
	 outPort=extractOutputPortNumber(m_escape_ctrl);
         if ( (m_escape_ctrl==_LocalNode_) && (checkFlowControl(0, m_fifos_cons, 1, m_bufferSize)==false) )
         {
	    checkFlow=false;
	 }
	 if ( (m_escape_ctrl!=_LocalNode_) && ( (checkEscapePath(m_escape_ctrl)==false) || (m_linkAvailable[outPort]==false) ) )
	 {
	    checkFlow=false;
	 }
      }
      
      if (checkFlow==true)
      {
         if (m_escape_ctrl==_LocalNode_)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	       TPZString texto = txt_comp + " ESCAPE->CONS " + m_latch_escape->asString();
               TPZWRITE2LOG(texto);
	    }
#endif 
            //Replication to consume
	    if (m_latch_escape->isMulticast())
	    {
	       unsigned long long messageMask=m_latch_escape->getMsgmask();
	       unsigned long long nodeMask=getOwnerRouter().getMask(_LocalNode_);
	       if ( (messageMask & (~nodeMask))==0 )
	       {
	          m_latch_escape->setMsgmask(messageMask & nodeMask);
		  m_fifos_cons[0].enqueue(m_latch_escape);
	          m_escape_ctrl= _LocalNode_;
	          m_latch_escape=0;
	       }
	       else
	       {
		  TPZMessage* replica;
		  if((replica = getMessagePool().allocate())==0) 
	          {
	             TPZString err;
                     err.sprintf(ERR_TPZPOOL_001);
                     EXIT_PROGRAM(err);
                  }
		  *replica=*m_latch_escape;
		  replica->setMulticast();
		  replica->setMsgmask(messageMask & nodeMask);
#ifdef PTOPAZ	
                  unsigned pool_index=net->getThreadID(pthread_self());
                  replica->setPoolIndex(pool_index);
#endif 
                     // Statistics functions 
                  net->incrementTx(TPZNetwork::Message);
                  net->incrementTx(TPZNetwork::Packet,replica->messageSize());
                  net->incrementTx(TPZNetwork::Flit,replica->messageSize()*replica->packetSize());
	          m_latch_escape->setMsgmask(messageMask & (~nodeMask));
	          m_fifos_cons[0].enqueue(replica);		     
	       }		  
	    }
	    else
	    {
	       m_fifos_cons[0].enqueue(m_latch_escape);
	       m_escape_ctrl=_LocalNode_;
	       m_latch_escape=0;
	    }
	 }
	 else
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
               TPZString texto2 = txt_comp + " LINK TRAVERSAL (ESCAPE) " + m_latch_escape->asString();
               TPZWRITE2LOG(texto2);
            }
#endif	 
	    outPort=extractOutputPortNumber(m_escape_ctrl);
	    if (m_latch_escape->isHeader() || m_latch_escape->isHeadTail()) m_linkAvailable[outPort]=false;
	 
	    outputInterfaz(outPort)->sendData(m_latch_escape,virtualChannel);
            ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::OStageTraversal);
            getOwnerRouter().incrLinkUtilization();
            ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
	 
	    // Clear the latch and the connection if necessary
            if( m_latch_escape->isHeadTail() || m_latch_escape->isTail()) m_clearEscConnection=true;
            m_latch_escape=0;
	 }
      }
   }
   
   //**********************************************************************************************************
   //**********************************************************************************************************
   // PART 1: CONSUMPTION
   //**********************************************************************************************************
   //**********************************************************************************************************
   
   //in the case of consumption, for index starts with 0 because the zero position
   //is employed for escape path. the rest of for loops start with 1.
   for( inPort = 0; inPort < m_ports; inPort++)
   {
     // Injection buffers
      if( (! m_latch_cons[inPort] ) && (m_fifos_cons[inPort].numberOfElements()!=0) )
      {
         m_fifos_cons[inPort].dequeue(m_latch_cons[inPort]);
      }      
   }
   // Clear connection
   m_clearConsConnection=false; 
   
   for( inPort = 0; inPort < m_ports; inPort++)
   {
      if( ! m_latch_cons[inPort] ) continue;
      
      if( m_latch_cons[inPort]->isHeader() || m_latch_cons[inPort]->isHeadTail() )
      {
         //First we check if the link is in use
	 if(m_consAvailable==false) continue;
	 
	 if( outputInterfaz(m_ports)->isStopActive(virtualChannel) ) continue;
	 
	 //reserve the link for the remaining flits
	 m_consAvailable=false;
      }
      
      //send the message
      outputInterfaz(m_ports)->sendData(m_latch_cons[inPort],virtualChannel);
      	    
#ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
         TPZString texto = txt_comp + " TO CONSUMER " + m_latch_cons[inPort]->asString();
         TPZWRITE2LOG(texto);
      }
#endif
           
      // Clear the latch and the connection if necessary
      if( m_latch_cons[inPort]->isHeadTail() || m_latch_cons[inPort]->isTail() ) m_clearConsConnection=true;
      m_latch_cons[inPort]=0;
   }
   
   if (m_clearConsConnection==true) m_consAvailable=true;
   
   //**********************************************************************************************************
   //**********************************************************************************************************
   // PART 2: OUTPUT STAGE
   //**********************************************************************************************************
   //**********************************************************************************************************
   
   //**********************************************************************************
   // Part 2-a: from buffers to latches 
   //**********************************************************************************
   for( outPort = 1; outPort < m_ports; outPort++)
   {
      // Bypass buffers
      if( (! m_latch_out_byp[outPort] ) && (m_fifos_out_byp[outPort].numberOfElements()!=0) )
      {
         m_fifos_out_byp[outPort].dequeue(m_latch_out_byp[outPort]);
      }
      
      // From-Ring buffers
      if( (! m_latch_out_turn[outPort] ) && (m_fifos_out_turn[outPort].numberOfElements()!=0) )
      {
         m_fifos_out_turn[outPort].dequeue(m_latch_out_turn[outPort]);
      }
      
      // Injection buffers
      if( (! m_latch_out_inj[outPort] ) && (m_fifos_out_inj[outPort].numberOfElements()!=0) )
      {
         m_fifos_out_inj[outPort].dequeue(m_latch_out_inj[outPort]);
      }
      
      // Clear connection
      m_clearConnection[outPort]=false;
      
   }
   //**********************************************************************************
   // Part 2-b: Request the Output link for Bypass buffers
   //**********************************************************************************
   for( outPort = 1; outPort < m_ports; outPort++)
   {
      if( ! m_latch_out_byp[outPort] ) continue;
#ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto = txt_comp + " LATCH-OUT-BYPASS " + m_latch_out_byp[outPort]->asString();
      texto += TPZString(" outPort:") + TPZString(outPort);
      texto += TPZString(" Occupation:") + TPZString(m_fifos_out_byp[outPort].numberOfElements());
      TPZWRITE2LOG(texto);
      }
#endif      
      if( m_latch_out_byp[outPort]->isHeader() || m_latch_out_byp[outPort]->isHeadTail() )
      {
         //First we check if the link is in use
	 if(m_linkAvailable[outPort]==false) continue;
	 
	 //Check the bubble is not consumed in the escape path
	 //if(checkFlowControl(outPort, m_fifos_out_turn, 2, m_bufferSize)==false) continue;
	 
	 if( outputInterfaz(outPort)->isStopActive(virtualChannel) ) continue;
	 
	 //reserve the link for the remaining flits
	 m_linkAvailable[outPort]=false;
	 
      }
      
      //send the message
      updateMessageInfo(m_latch_out_byp[outPort], outPort);
      outputInterfaz(outPort)->sendData(m_latch_out_byp[outPort],virtualChannel);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::OStageTraversal);
            
      if ( ((TPZSimpleRouter&)getComponent()).getTypeForOutput(outPort) != _LocalNode_ )
      {
         getOwnerRouter().incrLinkUtilization();
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
      }
	    
#ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto2 = txt_comp + " LINK TRAVERSAL (BYPASS) " + m_latch_out_byp[outPort]->asString();
      TPZWRITE2LOG(texto2);
      }
#endif
           
      // Clear the latch and the connection if necessary
      if( m_latch_out_byp[outPort]->isHeadTail() || m_latch_out_byp[outPort]->isTail() )
      {
         m_clearConnection[outPort]=true;
	 
	 //If the message is ordered, clear the input port restriction
	 if(m_latch_out_byp[outPort]->isOrdered()) m_interfazInOrder[m_latch_out_byp[outPort]->getInputInterfaz()]=false;
      }      
      m_latch_out_byp[outPort]=0;
   }
   
   //**********************************************************************************
   // PART 2-c: Request the Output link for From-Ring buffers
   //**********************************************************************************
   for( outPort = 1; outPort < m_ports; outPort++)
   {
      if( ! m_latch_out_turn[outPort] ) continue;
 #ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto = txt_comp + " LATCH-OUT-TURN " + m_latch_out_turn[outPort]->asString();
      texto += TPZString(" outPort:") + TPZString(outPort);
      texto += TPZString(" Occupation:") + TPZString(m_fifos_out_turn[outPort].numberOfElements());
      TPZWRITE2LOG(texto);
      }
#endif     
      if( m_latch_out_turn[outPort]->isHeader() || m_latch_out_turn[outPort]->isHeadTail() )
      {
         //First we check if the link is in use
	 if(m_linkAvailable[outPort]==false) continue;
	 
	 if( outputInterfaz(outPort)->isStopActive(virtualChannel) ) continue;
	 
	 //If the msg is on Escape, bubble must be checked
	 if ( (m_latch_out_turn[outPort]->isOnScape()==true) && (checkEscapeFlowControl()==false) ) continue;
	  
	 //reserve the link for the remaining flits
	 m_linkAvailable[outPort]=false;
      }
      
      //send the message
      updateMessageInfo(m_latch_out_turn[outPort], outPort);
      outputInterfaz(outPort)->sendData(m_latch_out_turn[outPort],virtualChannel);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::OStageTraversal);
            
      if ( ((TPZSimpleRouter&)getComponent()).getTypeForOutput(outPort) != _LocalNode_ )
      {
         getOwnerRouter().incrLinkUtilization();
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
      }
	    
#ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto2 = txt_comp + " LINK TRAVERSAL (RING) " + m_latch_out_turn[outPort]->asString();
      TPZWRITE2LOG(texto2);
      }
#endif
           
      // Clear the latch and the connection if necessary
      if( m_latch_out_turn[outPort]->isHeadTail() || m_latch_out_turn[outPort]->isTail() ) 
      {
         m_clearConnection[outPort]=true;
	  
	 //If the message is ordered, clear the input port restriction
	 if(m_latch_out_turn[outPort]->isOrdered()) m_interfazInOrder[m_latch_out_turn[outPort]->getInputInterfaz()]=false;      
      }
      m_latch_out_turn[outPort]=0;
   }
   
   //**********************************************************************************
   // PART 2-d: Request the Output link for Injection buffers
   //**********************************************************************************
   for( outPort = 1; outPort <= m_ports; outPort++)
   {
      if( ! m_latch_out_inj[outPort] ) continue;
#ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto = txt_comp + " LATCH-OUT-INJ " + m_latch_out_inj[outPort]->asString();
      texto += TPZString(" outPort:") + TPZString(outPort);
      texto += TPZString(" Occupation:") + TPZString(m_fifos_out_inj[outPort].numberOfElements());
      TPZWRITE2LOG(texto);
      }
#endif      
      if( m_latch_out_inj[outPort]->isHeader() || m_latch_out_inj[outPort]->isHeadTail() )
      {
         //First we check if the link is in use
	 if(m_linkAvailable[outPort]==false) continue;
	 
	 //if(checkFlowControl(outPort, m_fifos_out_turn, 2, m_bufferSize)==false) continue;
	 
	 if( outputInterfaz(outPort)->isStopActive(virtualChannel) ) continue;
	 
	 //reserve the link for the remaining flits
	 m_linkAvailable[outPort]=false;
      }
      
      //send the message
      updateMessageInfo(m_latch_out_inj[outPort], outPort);
      outputInterfaz(outPort)->sendData(m_latch_out_inj[outPort],virtualChannel);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::OStageTraversal);
            
      if ( ((TPZSimpleRouter&)getComponent()).getTypeForOutput(outPort) != _LocalNode_ )
      {
         getOwnerRouter().incrLinkUtilization();
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
      }
	    
#ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto2 = txt_comp + " LINK TRAVERSAL (INJ) " + m_latch_out_inj[outPort]->asString();
      TPZWRITE2LOG(texto2);
      }
#endif
           
      // Clear the latch and the connection if necessary
      if( m_latch_out_inj[outPort]->isHeadTail() || m_latch_out_inj[outPort]->isTail() ) m_clearConnection[outPort]=true;
      m_latch_out_inj[outPort]=0;
   }
      
   //**********************************************************************************
   // PART 2-e: Clear all the links (tail flit) occupied this cycle 
   //**********************************************************************************
   for( outPort = 1; outPort <= m_ports; outPort++)
   {
      if (m_clearConnection[outPort]==true) m_linkAvailable[outPort]=true;
   }
   
   if (m_clearEscConnection==true)
   {
      outPort=extractOutputPortNumber(m_escape_ctrl);
      m_linkAvailable[outPort]=true;
   }
   
   //**********************************************************************************************************
   //**********************************************************************************************************
   // PART 3: MULTIPORT BUFFERS (INTERNAL RING)
   //**********************************************************************************************************
   //**********************************************************************************************************
   
   //**********************************************************************************
   // Part 3-a: from buffers to latches
   //**********************************************************************************
   for( inPort = 1; inPort < m_ports; inPort++)
   {
      if( (! m_latch_mp_one[inPort] ) && (m_fifos_mp_one[inPort].numberOfElements()!=0) )
      {
         m_fifos_mp_one[inPort].dequeue(m_latch_mp_one[inPort]);
      }
      if( (! m_latch_mp_two[inPort] ) && (m_fifos_mp_two[inPort].numberOfElements()!=0) )
      {
         m_fifos_mp_two[inPort].dequeue(m_latch_mp_two[inPort]);
      }
      
      m_clearRingExit[inPort]=false;
      m_clearRingAdvance[inPort]=false;
   }
   
   //**********************************************************************************
   // Part 3-b: Arbitrate buffer one (the one comming from input stage)
   //**********************************************************************************
   for( inPort = 1; inPort < m_ports; inPort++)
   {
      if( !m_latch_mp_one[inPort] ) continue;
      
      if( m_latch_mp_one[inPort]->isHeader() || m_latch_mp_one[inPort]->isHeadTail() )
      {
         //First we check if the message can leave the ring
	 if(m_ringExitAvailable[inPort]==true && checkFlowControl((inPort%(m_ports-1))+1, m_fifos_out_turn, 1, m_OSSize)==true)
	 {
	    Boolean checkHead=checkHeader(inPort, m_latch_mp_one[inPort]);
	    if ( checkHead==true )
	    {
#ifndef NO_TRAZA
               if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
               {
	       TPZString texto = txt_comp + " MP->OSTAGE " + m_latch_mp_one[inPort]->asString();
	       texto += TPZString(" OutPort= ") + TPZString((inPort%(m_ports-1))+1);
               TPZWRITE2LOG(texto);
	       }
#endif	    
               if( m_latch_mp_one[inPort]->isMulticast())
	       {
	          unsigned long long messageMask=m_latch_mp_one[inPort]->getMsgmask();
		  unsigned long long portMask;
		  unsigned outPort= (inPort%(m_ports-1))+1;
		  if (outPort==1) portMask=getOwnerRouter().getMask(_Xplus_);
		  else if (outPort==2) portMask=getOwnerRouter().getMask(_Yplus_);
		  else if (outPort==3) portMask=getOwnerRouter().getMask(_Xminus_);
		  else if (outPort==4) portMask=getOwnerRouter().getMask(_Yminus_);
		  //in this first case no replication is needed
		  if( (messageMask & (~portMask))== 0)
		  {
		     m_latch_mp_one[inPort]->setMsgmask(messageMask & portMask);
		     m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_one[inPort]);
	             m_mp_one_ctrl[inPort]=Leave;
	             m_ringExitAvailable[inPort]=false;
	             if(m_latch_mp_one[inPort]->isHeadTail()) m_clearRingExit[inPort]=true;
	             m_latch_mp_one[inPort]=0;
	             continue;
		  }
		  //Replication is needed
		  else
		  {
		     TPZMessage* mess;
		     if((mess = getMessagePool().allocate())==0) 
	             {
	                TPZString err;
                        err.sprintf(ERR_TPZPOOL_001);
                        EXIT_PROGRAM(err);
                     }
		     *mess=*m_latch_mp_one[inPort];
		     mess->setMulticast();
	             mess->setMsgmask(messageMask & portMask);
#ifdef PTOPAZ	
                     unsigned pool_index=net->getThreadID(pthread_self());
                     mess->setPoolIndex(pool_index);
#endif 
                     // Statistics functions 
                     net->incrementTx(TPZNetwork::Message);
                     net->incrementTx(TPZNetwork::Packet,mess->messageSize());
                     net->incrementTx(TPZNetwork::Flit,mess->messageSize()*mess->packetSize());
	             m_latch_mp_one[inPort]->setMsgmask(messageMask & (~portMask));
		     
		     m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(mess);
	             m_mp_one_ctrl[inPort]=Leave;
	             m_ringExitAvailable[inPort]=false;
	             if(mess->isHeadTail()) m_clearRingExit[inPort]=true;
		     //continue;
		  }
	       }
	       else
	       {
	          m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_one[inPort]);
	          m_mp_one_ctrl[inPort]=Leave;
	          m_ringExitAvailable[inPort]=false;
	          if(m_latch_mp_one[inPort]->isHeadTail()) m_clearRingExit[inPort]=true;
	          m_latch_mp_one[inPort]=0;
	          continue;
	       }
	    } 
	 }
	 
	 //If not, we request the next multiport. As we are entering the internal router ring
	 //two conditions: two holes on mp_two[inPort] and one hole on mp_two[(inPort%(m_ports-1))+1]
	 Boolean BubbleFlowControl=checkFlowControl(inPort, m_fifos_mp_two, 2, m_MPSize );
	 Boolean CTFlowControl=checkFlowControl((inPort%(m_ports-1))+1, m_fifos_mp_two, 1, m_MPSize);
	 
	 if(m_ringAdvanceAvailable[inPort]==true && BubbleFlowControl==true && CTFlowControl==true)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto = txt_comp + " MP->MP " + m_latch_mp_one[inPort]->asString();
	    texto += TPZString(" NextMP= ") + TPZString((inPort%(m_ports-1))+1);
            TPZWRITE2LOG(texto);
	    }
#endif	    
	    m_fifos_mp_two[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_one[inPort]);
	    m_mp_one_ctrl[inPort]=Advance;
	    m_ringAdvanceAvailable[inPort]=false;
	    if(m_latch_mp_one[inPort]->isHeadTail()) m_clearRingAdvance[inPort]=true;
	    m_latch_mp_one[inPort]=0;
	    continue; 
	 }
      }
      else
      {
         if ( m_mp_one_ctrl[inPort]==Leave)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
            TPZString texto = txt_comp + " MP->OSTAGE " + m_latch_mp_one[inPort]->asString();
	    texto += TPZString(" OutPort= ") + TPZString((inPort%(m_ports-1))+1);
            TPZWRITE2LOG(texto);
	    }
#endif	    
	    m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_one[inPort]);
	    if(m_latch_mp_one[inPort]->isTail()) m_clearRingExit[inPort]=true;
	    m_latch_mp_one[inPort]=0;
	 }
	 else
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
            TPZString texto = txt_comp + " MP->MP " + m_latch_mp_one[inPort]->asString();
	    texto += TPZString(" NextMP= ") + TPZString((inPort%(m_ports-1))+1);
            TPZWRITE2LOG(texto);
	    }
#endif	    
	    m_fifos_mp_two[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_one[inPort]);
	    if(m_latch_mp_one[inPort]->isTail()) m_clearRingAdvance[inPort]=true;
	    m_latch_mp_one[inPort]=0;
	 }
      }
   }
   //**********************************************************************************
   // Part 3-c: Arbitrate buffer two (the one forming the ring)
   //**********************************************************************************
   for( inPort = 1; inPort < m_ports; inPort++)
   {
      if( !m_latch_mp_two[inPort] ) continue;
      
      if( m_latch_mp_two[inPort]->isHeader() || m_latch_mp_two[inPort]->isHeadTail() )
      {
         //First we check if the message can leave the ring
	 if(m_ringExitAvailable[inPort]==true && checkFlowControl((inPort%(m_ports-1))+1, m_fifos_out_turn, 1, m_OSSize)==true)
	 {
	    Boolean checkHead=checkHeader(inPort, m_latch_mp_two[inPort]);
	    if ( checkHead==true )
	    {
#ifndef NO_TRAZA
               if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
               {
	       TPZString texto = txt_comp + " MP->OSTAGE " + m_latch_mp_two[inPort]->asString();
	       texto += TPZString(" OutPort= ") + TPZString((inPort%(m_ports-1))+1);
               TPZWRITE2LOG(texto);
	       }
#endif 	    
	       if( m_latch_mp_two[inPort]->isMulticast() && (m_latch_mp_two[inPort]->getLastMissRouted()==false) && (m_latch_mp_two[inPort]->isOnScape()==false) )
	       {
	          unsigned long long messageMask=m_latch_mp_two[inPort]->getMsgmask();
		  unsigned long long portMask;
		  unsigned outPort= (inPort%(m_ports-1))+1;
		  if (outPort==1) portMask=getOwnerRouter().getMask(_Xplus_);
		  else if (outPort==2) portMask=getOwnerRouter().getMask(_Yplus_);
		  else if (outPort==3) portMask=getOwnerRouter().getMask(_Xminus_);
		  else if (outPort==4) portMask=getOwnerRouter().getMask(_Yminus_);
		  //in this first case no replication is needed
		  if( (messageMask & (~portMask))== 0)
		  {
		     m_latch_mp_two[inPort]->setMsgmask(messageMask & portMask);
		     m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_two[inPort]);
	             m_mp_two_ctrl[inPort]=Leave;
	             m_ringExitAvailable[inPort]=false;
	             if(m_latch_mp_two[inPort]->isHeadTail()) m_clearRingExit[inPort]=true;
	             m_latch_mp_two[inPort]=0;
	             continue;
		  }
		  //Replication is needed
		  else
		  {
		     TPZMessage* mess;
		     if((mess = getMessagePool().allocate())==0) 
	             {
	                TPZString err;
                        err.sprintf(ERR_TPZPOOL_001);
                        EXIT_PROGRAM(err);
                     }
		     *mess=*m_latch_mp_two[inPort];
		     mess->setMulticast();
	             mess->setMsgmask(messageMask & portMask);
#ifdef PTOPAZ	
                     unsigned pool_index=net->getThreadID(pthread_self());
                     mess->setPoolIndex(pool_index);
#endif 
                     // Statistics functions 
                     net->incrementTx(TPZNetwork::Message);
                     net->incrementTx(TPZNetwork::Packet,mess->messageSize());
                     net->incrementTx(TPZNetwork::Flit,mess->messageSize()*mess->packetSize());
	             m_latch_mp_two[inPort]->setMsgmask(messageMask & (~portMask));
		     
		     m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(mess);
	             m_mp_two_ctrl[inPort]=Leave;
	             m_ringExitAvailable[inPort]=false;
	             if(mess->isHeadTail()) m_clearRingExit[inPort]=true;
		     //continue;
		  }
	       }
	       else
	       {
	          m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_two[inPort]);
	          m_mp_two_ctrl[inPort]=Leave;
	          m_ringExitAvailable[inPort]=false;
	          if(m_latch_mp_two[inPort]->isHeadTail()) m_clearRingExit[inPort]=true;
	          m_latch_mp_two[inPort]=0;
	          continue;
	       }
	    }
	 }
	 
	 //If not, we request the next multiport
	 if(m_ringAdvanceAvailable[inPort]==true && checkFlowControl((inPort%(m_ports-1))+1, m_fifos_mp_two, 1, m_MPSize)==true)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto = txt_comp + " MP->MP " + m_latch_mp_two[inPort]->asString();
	    texto += TPZString(" NextMP= ") + TPZString((inPort%(m_ports-1))+1);
            TPZWRITE2LOG(texto);
	    }
#endif	    
	    m_fifos_mp_two[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_two[inPort]);
	    m_mp_two_ctrl[inPort]=Advance;
	    m_ringAdvanceAvailable[inPort]=false;
	    if(m_latch_mp_two[inPort]->isHeadTail()) m_clearRingAdvance[inPort]=true;
	    
	    //Normal messages must check the number of multiports to move to missrtg or escape
	    if (!m_latch_mp_two[inPort]->isOrdered())
	    {
	       if (m_latch_mp_two[inPort]->getLastMissRouted()==false && m_latch_mp_two[inPort]->isOnScape()==false)
	       {
	          m_latch_mp_two[inPort]->incrMultiportNumber();
	          if (m_latch_mp_two[inPort]->getMultiportNumber() >= (m_ports-1)*m_missLoops)
                  {
                     m_latch_mp_two[inPort]->incrTimesMiss();
                     if (m_latch_mp_two[inPort]->getTimesMiss() >= m_missLimit) m_latch_mp_two[inPort]->setOnScape();
                     else m_latch_mp_two[inPort]->setLastMissRouted(true);
                  }
	       }
	    }
	    m_latch_mp_two[inPort]=0;
	    continue; 
	 }
      }
      else
      {
         if ( m_mp_two_ctrl[inPort]==Leave)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto = txt_comp + " MP->OSTAGE " + m_latch_mp_two[inPort]->asString();
	    texto += TPZString(" OutPort= ") + TPZString((inPort%(m_ports-1))+1);
            TPZWRITE2LOG(texto);
	    }
#endif	    
	    m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_two[inPort]);
	    if(m_latch_mp_two[inPort]->isTail()) m_clearRingExit[inPort]=true;
	    m_latch_mp_two[inPort]=0;
	 }
	 else
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto = txt_comp + " MP->MP " + m_latch_mp_two[inPort]->asString();
	    texto += TPZString(" NextMP= ") + TPZString((inPort%(m_ports-1))+1);
            TPZWRITE2LOG(texto);
	    }
#endif	    
	    m_fifos_mp_two[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_two[inPort]);
	    if(m_latch_mp_two[inPort]->isTail()) m_clearRingAdvance[inPort]=true;
	    m_latch_mp_two[inPort]=0;
	 }
      }
   }
   
   //**********************************************************************************
   // Part 3-d: Clear finished connections
   //**********************************************************************************
   for( inPort = 1; inPort < m_ports; inPort++)
   {
      if (m_clearRingExit[inPort]==true) m_ringExitAvailable[inPort]= true;
      if (m_clearRingAdvance[inPort]==true) m_ringAdvanceAvailable[inPort]= true;
   }
          
   //****************************************************************************************************************
   //****************************************************************************************************************
   // PART 4: INPUT STAGE
   //****************************************************************************************************************
   //****************************************************************************************************************
   
   //**********************************************************************************
   // Part 4-a: from buffers to latches
   //**********************************************************************************
   for(inPort = 1; inPort <= m_ports; inPort++) 
   {
      // If the routing is empty, Take the next message from the corresponding fifo
      if( (! m_routing[inPort] ) && (m_fifos[inPort].numberOfElements()!=0) )
      {
         m_fifos[inPort].dequeue(m_routing[inPort]);
      }
   }
   //**********************************************************************************
   // PART 4-b: Then arbitrate msg access to the next stage
   //**********************************************************************************
   for(inPort = 1; inPort < m_ports; inPort++) //The last port is arbitrated different, is injection
   {
      // If the routing is empty, go to the next
      if( ! m_routing[inPort] ) continue;
 #ifndef NO_TRAZA
      if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
      {
      TPZString texto = txt_comp + " LATCH-ISTAGE " + m_routing[inPort]->asString();
      texto += TPZString(" inPort: ") + TPZString(inPort);
      texto += TPZString(" Occupation: ") + TPZString(m_fifos[inPort].numberOfElements());
      TPZWRITE2LOG(texto);
      }
#endif     
      
      if( m_routing[inPort]->isHeader() || m_routing[inPort]->isHeadTail() )
      {
         TPZROUTINGTYPE direction= getOutputDirection(m_routing[inPort]);
	 outPort = extractOutputPortNumber(direction);
	 
	 // First we check if it must be consumed
	 if( outPort == m_ports )
	 {
	    if(checkFlowControl(inPort, m_fifos_cons, 1, m_bufferSize)==true)
	    {
#ifndef NO_TRAZA
               if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
               {
	       TPZString texto = txt_comp + " ISTAGE->CONS " + m_routing[inPort]->asString();
               TPZWRITE2LOG(texto);
	       }
#endif 
               //Replication to consume
	       if (m_routing[inPort]->isMulticast())
	       {
	          unsigned long long messageMask=m_routing[inPort]->getMsgmask();
	          unsigned long long nodeMask=getOwnerRouter().getMask(_LocalNode_);
		  if ( (messageMask & (~nodeMask))==0 )
		  {
		     m_routing[inPort]->setMsgmask(messageMask & nodeMask);
		     m_fifos_cons[inPort].enqueue(m_routing[inPort]);
	             m_istage_ctrl[inPort]= Consume;
	             m_routing[inPort]=0;
		  }
		  else
		  {
		     TPZMessage* replica;
		     if((replica = getMessagePool().allocate())==0) 
	             {
	                TPZString err;
                        err.sprintf(ERR_TPZPOOL_001);
                        EXIT_PROGRAM(err);
                     }
		     *replica=*m_routing[inPort];
		     replica->setMulticast();
		     replica->setMsgmask(messageMask & nodeMask);
#ifdef PTOPAZ	
                     unsigned pool_index=net->getThreadID(pthread_self());
                     replica->setPoolIndex(pool_index);
#endif 
                     // Statistics functions 
                     net->incrementTx(TPZNetwork::Message);
                     net->incrementTx(TPZNetwork::Packet,replica->messageSize());
                     net->incrementTx(TPZNetwork::Flit,replica->messageSize()*replica->packetSize());
	             m_routing[inPort]->setMsgmask(messageMask & (~nodeMask));
	             
		     m_fifos_cons[inPort].enqueue(replica);		     
		  }		  
	       }
	       else
	       {
	          m_fifos_cons[inPort].enqueue(m_routing[inPort]);
	          m_istage_ctrl[inPort]= Consume;
	          m_routing[inPort]=0;
	       }
	    }
	    continue;
	 }
	 //At this point we check inOrder restrictions
	 if( m_routing[inPort]->isOrdered() && m_interfazInOrder[inPort]==true)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto = txt_comp + " STOP-ORDERED " + m_routing[inPort]->asString();
            TPZWRITE2LOG(texto);
	    }
#endif 	 
	    continue;
	 }
	 
	 //Second we try the bypass path
	 if( (outPort == inPort) && (checkFlowControl(outPort, m_fifos_out_byp, 1, m_OSSize)==true) && !m_routing[inPort]->isOnScape() )
	 {
	    if (m_routing[inPort]->isMulticast())
	    {
	       unsigned long long messageMask=m_routing[inPort]->getMsgmask();
	       unsigned long long portMask=getOwnerRouter().getMask(direction); 
	       //all destinations are through bypass path
	       if ( (messageMask & (~portMask))== 0 )
	       {
#ifndef NO_TRAZA
                  if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
                  {
	             TPZString texto = txt_comp + " ISTAGE->OSTAGE " + m_routing[inPort]->asString();
                     TPZWRITE2LOG(texto);
	          }
#endif
                  m_fifos_out_byp[outPort].enqueue(m_routing[inPort]);
	          m_istage_ctrl[inPort]= Bypass;
	          if(m_routing[inPort]->isOrdered())
	          {
	             m_interfazInOrder[inPort]=true;
	             m_routing[inPort]->setInputInterfaz(inPort);
	          }
	          m_routing[inPort]=0;
	          continue;
	       }
	    }
	    else
	    {
#ifndef NO_TRAZA
               if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
               {
	       TPZString texto = txt_comp + " ISTAGE->OSTAGE " + m_routing[inPort]->asString();
               TPZWRITE2LOG(texto);
	       }
#endif
               m_fifos_out_byp[outPort].enqueue(m_routing[inPort]);
	       m_istage_ctrl[inPort]= Bypass;
	       if(m_routing[inPort]->isOrdered())
	       {
	          m_interfazInOrder[inPort]=true;
	          m_routing[inPort]->setInputInterfaz(inPort);
	       }
	       m_routing[inPort]=0;
	       continue;
	    }
	 }
	 
	 //Next we try to enter the internal router ring
	 if( checkFlowControl(inPort, m_fifos_mp_one, 1, m_MPSize)==true )
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto = txt_comp + " ISTAGE->MP " + m_routing[inPort]->asString();
            TPZWRITE2LOG(texto);
	    }
#endif  
            m_fifos_mp_one[inPort].enqueue(m_routing[inPort]);
	    m_istage_ctrl[inPort]= Turn;
	    if(m_routing[inPort]->isOrdered())
	    {
	       m_interfazInOrder[inPort]=true;
	       m_routing[inPort]->setInputInterfaz(inPort);
	    }
	    m_routing[inPort]=0;
	    continue;
	 }
	 
      }
      else
      {
         if(m_istage_ctrl[inPort]==Bypass)
         {           
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto1 = txt_comp + " ISTAGE->OSTAGE " + m_routing[inPort]->asString();
            TPZWRITE2LOG(texto1);
	    }
#endif	    
	    m_routing[inPort]->setInputInterfaz(inPort);
	    m_fifos_out_byp[inPort].enqueue(m_routing[inPort]);
	    m_routing[inPort]=0;
	 }
	 else if(m_istage_ctrl[inPort]==Turn)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto2 = txt_comp + " ISTAGE->MP " + m_routing[inPort]->asString();
            TPZWRITE2LOG(texto2);
	    }
#endif 	    
            m_routing[inPort]->setInputInterfaz(inPort);
	    m_fifos_mp_one[inPort].enqueue(m_routing[inPort]);
	    m_routing[inPort]=0;
	 }
	 else if (m_istage_ctrl[inPort]==Consume)
	 {
#ifndef NO_TRAZA
            if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
            {
	    TPZString texto3 = txt_comp + " ISTAGE->CONS " + m_routing[inPort]->asString();
            TPZWRITE2LOG(texto3);
	    }
#endif 	    
	    m_fifos_cons[inPort].enqueue(m_routing[inPort]);
	    m_routing[inPort]=0;
	 }
	 else
	 {
	    TPZString err;
            err.sprintf( "Wrong value of I-Stage control for Body flits");
            EXIT_PROGRAM(err);
	 }
      }
   }
   //****************************************************************************************************************
   //****************************************************************************************************************
   // PART 5: INJECTION
   //****************************************************************************************************************
   //****************************************************************************************************************
   
   if( m_routing[m_ports] !=0 )
   {
     //Error Checking, only performed here
     if(m_routing[m_ports]->isHeader() && m_routing[m_ports]->isMulticast() )
     {
        TPZString err;
        err.sprintf( "Multicast for Multi-flit messages not implemented");
        EXIT_PROGRAM(err);
     }
     if(m_routing[m_ports]->isMulticast() && m_routing[m_ports]->isOrdered() )
     {
        TPZString err;
        err.sprintf( "Multicast for Ordered messages not implemented");
        EXIT_PROGRAM(err);
     }
     
     if( m_routing[m_ports]->isHeader() || m_routing[m_ports]->isHeadTail() )
     {
        //ordered messages are routed through a mesh
	if( m_routing[m_ports]->isOrdered())
	{
	   int deltaX;
           int deltaY;
	   TPZPosition src = getOwnerRouter().getPosition();
	   TPZPosition dst = m_routing[m_ports]->destiny();
	   TPZPosition deltaPos = dst - src;
           deltaX = deltaPos.valueForCoordinate(TPZPosition::X);
           deltaY = deltaPos.valueForCoordinate(TPZPosition::Y);
           deltaX = (deltaX>=0) ? deltaX+1 : deltaX-1;
           deltaY = (deltaY>=0) ? deltaY+1 : deltaY-1;
           m_routing[m_ports]->setDelta(deltaX,0);
           m_routing[m_ports]->setDelta(deltaY,1);         
	}
	TPZROUTINGTYPE direction= getInjectionDirection(m_routing[m_ports]);
	outPort= extractOutputPortNumber(direction);
	if (checkFlowControl(outPort, m_fifos_out_inj, 1, m_bufferSize)==true)
	{
#ifndef NO_TRAZA
           if (getOwnerRouter().getCurrentTime()> CYC_TRAZA)
           {
	   TPZString texto = txt_comp + " INJECTION " + m_routing[inPort]->asString();
	   texto += TPZString(" O-PORT: ") + TPZString(outPort);
           TPZWRITE2LOG(texto);
	   }
#endif
           if (m_routing[m_ports]->isMulticast())
	   {
	      unsigned long long messageMask=m_routing[m_ports]->getMsgmask();
	      unsigned long long portMask=getOwnerRouter().getMask(direction);
	      if ( (messageMask & (~portMask))== 0)
	      {
	         m_routing[m_ports]->setMsgmask(messageMask & portMask);
		 m_fifos_out_inj[outPort].enqueue(m_routing[m_ports]);
	         m_injection_ctrl=outPort;
	         m_routing[m_ports]=0;
	      }
	      else
	      {
	         TPZMessage* mess;
		 if((mess = getMessagePool().allocate())==0) 
	         {
	            TPZString err;
                    err.sprintf(ERR_TPZPOOL_001);
                    EXIT_PROGRAM(err);
                 }
                 *mess=*m_routing[m_ports];
	         //New message mask
	         mess->setMulticast();
	         mess->setMsgmask(messageMask & portMask);
#ifdef PTOPAZ	
                 unsigned pool_index=net->getThreadID(pthread_self());
                 mess->setPoolIndex(pool_index);
#endif 
                 // Statistics functions 
                 net->incrementTx(TPZNetwork::Message);
                 net->incrementTx(TPZNetwork::Packet,mess->messageSize());
                 net->incrementTx(TPZNetwork::Flit,mess->messageSize()*mess->packetSize());
	         m_routing[m_ports]->setMsgmask(messageMask & (~portMask));
		 m_fifos_out_inj[outPort].enqueue(mess);
	         m_injection_ctrl=outPort;
	      }
	   }
	   else
	   {
	      m_fifos_out_inj[outPort].enqueue(m_routing[m_ports]);
	      m_injection_ctrl=outPort;
	      m_routing[m_ports]=0;
	   }
	}	
     }
     else
     {
        m_fifos_out_inj[m_injection_ctrl].enqueue(m_routing[m_ports]);
        m_routing[m_ports]=0; 
     } 
   }
   
   return true;   
}

//*************************************************************************
//:
//  f: TPZROUTINGTYPE getInjectionDirection(TPZMessage* msg);
//
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE TPZSimpleRouterFlowLigeroMcast :: getInjectionDirection(TPZMessage* msg)
{
   if (msg->isMulticast())
   {
      unsigned long long msg_mask = msg -> getMsgmask();
     
      unsigned long long maskXminus=getOwnerRouter().getMask(_Xminus_);
      unsigned long long maskXplus=getOwnerRouter().getMask(_Xplus_);
      unsigned long long maskYminus=getOwnerRouter().getMask(_Yminus_);
      unsigned long long maskYplus=getOwnerRouter().getMask(_Yplus_);
      
      if ( (msg_mask & maskXplus)!=0) return _Xplus_;
      if ( (msg_mask & maskXminus)!=0) return _Xminus_;
      if ( (msg_mask & maskYplus)!=0) return _Yplus_;
      if ( (msg_mask & maskYminus)!=0) return _Yminus_;
   }
   
   if (msg->isOrdered())
   {
      if ((msg->delta(0) >1) ) return _Xplus_;
      else if ((msg->delta(0) <-1) ) return _Xminus_;
      else if ((msg->delta(1) >1) ) return _Yplus_;
      else if ((msg->delta(1) <-1) ) return _Yminus_;   
   }
   else
   {
      //First we check if any adaptive input is free
      if ((msg->delta(0) >1) && (checkFlowControl(1, m_fifos_out_inj, 1, m_bufferSize)==true) ) return _Xplus_;
      if ((msg->delta(0) <-1) && (checkFlowControl(3, m_fifos_out_inj, 1, m_bufferSize)==true) ) return _Xminus_;
      if ((msg->delta(1) >1) && (checkFlowControl(2, m_fifos_out_inj, 1, m_bufferSize)==true) ) return _Yplus_;
      if ((msg->delta(1) <-1 && (checkFlowControl(4, m_fifos_out_inj, 1, m_bufferSize)==true)) ) return _Yminus_;
      
      //if not, DOR order
      if ((msg->delta(0) >1) ) return _Xplus_;
      if ((msg->delta(0) <-1) ) return _Xminus_;
      if ((msg->delta(1) >1) ) return _Yplus_;
      if ((msg->delta(1) <-1) ) return _Yminus_;
   }
}

//*************************************************************************
//:
//  f: TPZROUTING getOurputPort (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE  TPZSimpleRouterFlowLigeroMcast :: getOutputDirection(TPZMessage* msg)
{  
   if (msg->isOnScape())
   {
      TPZPosition pos=getOwnerRouter().getPosition();
      TPZPosition dest=msg->destiny();
      int posX=pos.valueForCoordinate(TPZPosition::X);
      int posY=pos.valueForCoordinate(TPZPosition::Y);
      unsigned size_x= ((TPZNetwork*)(getOwnerRouter().getOwner()))->getSizeX();
      unsigned size_y= ((TPZNetwork*)(getOwnerRouter().getOwner()))->getSizeY();
      
      if( msg->isMulticast())
      {
         unsigned long long msg_mask = msg -> getMsgmask();
	 unsigned long long maskNode=getOwnerRouter().getMask(_LocalNode_);
	 if ((msg_mask & maskNode)!=0) return _LocalNode_;
      }
      else
      {
         if(pos==dest) return _LocalNode_;
      }
      
      if(posX==0)
      {
         if (posY==0) return _Xplus_;
	 else return _Yminus_;
      }
      else if (posX==1)
      {
         if (posY%2==0) return _Xplus_;
	 else
	 {
	    if (posY==(size_y-1)) return _Xminus_;
	    else return _Yplus_;
	 }
      }
      else if (posX==(size_x-1))
      {
         if (posY%2==0) return _Yplus_;
	 else return _Xminus_;
      }
      else
      {
         if (posY%2==0) return _Xplus_;
	 else return _Xminus_;
      }
            
      return _Unknow_;
   }
   else
   {
      if (msg->isMulticast())
      {
         unsigned long long msg_mask = msg -> getMsgmask();
     
         unsigned long long maskXminus=getOwnerRouter().getMask(_Xminus_);
         unsigned long long maskXplus=getOwnerRouter().getMask(_Xplus_);
         unsigned long long maskYminus=getOwnerRouter().getMask(_Yminus_);
         unsigned long long maskYplus=getOwnerRouter().getMask(_Yplus_);
	 unsigned long long maskNode=getOwnerRouter().getMask(_LocalNode_);
	 if ( (msg_mask & maskNode)!=0) return _LocalNode_;
	 else if ( (msg_mask & maskXplus)!=0) return _Xplus_;
         else if ( (msg_mask & maskXminus)!=0) return _Xminus_;
         else if ( (msg_mask & maskYplus)!=0) return _Yplus_;
         else if ( (msg_mask & maskYminus)!=0) return _Yminus_;
	 else EXIT_PROGRAM("Zero Mask");	 
      }
      else
      {
         int deltaX = msg->delta(0);
         int deltaY = msg->delta(1);
        
         if( deltaX > 1 ) return _Xplus_;                 
         else if( deltaX < -1 ) return _Xminus_;                 
         else if( deltaY > +1 ) return _Yplus_;           
         else if(deltaY < -1) return  _Yminus_;                
         else return _LocalNode_;
      }
      return _Unknow_; //Change of direction
   }
}

//*************************************************************************
//:
//  f: Boolean checkHeader (unsigned inPort, TPZMessage* msg)
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlowLigeroMcast :: checkHeader (unsigned inPort, TPZMessage* msg)
{
   unsigned oPort=(inPort%(m_ports-1))+1;
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
   
   if(msg->isOrdered())
   {
      if (deltaX>1 && oPort==1) return true;
      if (deltaX<-1 && oPort==3) return true;
      if ((deltaX==1 || deltaX==-1) && deltaY>1 && oPort==2) return true;
      if ((deltaX==1 || deltaX==-1) && deltaY<-1 && oPort==4) return true;  
      
      return false;
   }
   
   if(msg->getLastMissRouted()==true) return true;
   
   if(msg->isOnScape())
   {
      TPZPosition pos=getOwnerRouter().getPosition();
      int posX=pos.valueForCoordinate(TPZPosition::X);
      int posY=pos.valueForCoordinate(TPZPosition::Y);
      
      unsigned size_x= ((TPZNetwork*)(getOwnerRouter().getOwner()))->getSizeX();
      unsigned size_y= ((TPZNetwork*)(getOwnerRouter().getOwner()))->getSizeY();
      
      unsigned followPort;
      
      if(posX==0)
      {
         if (posY==0) followPort=1;
	 else followPort=4;
      }
      else if (posX==1)
      {
         if (posY%2==0) followPort=1;
	 else
	 {
	    if (posY==(size_y-1)) followPort=3;
	    else followPort=2;
	 }
      }
      else if (posX==(size_x-1))
      {
         if (posY%2==0) followPort=2;
	 else followPort=3;
      }
      else
      {
         if (posY%2==0) followPort=1;
	 else followPort=3;
      }
            
      if (followPort==oPort) return true;
      else return false;
   }
   
   if(msg->isMulticast())
   {
      unsigned long long msg_mask = msg->getMsgmask();
      unsigned long long maskPort;
      if (oPort==1) maskPort=getOwnerRouter().getMask(_Xplus_);
      else if (oPort==2) maskPort=getOwnerRouter().getMask(_Yplus_);
      else if (oPort==3) maskPort=getOwnerRouter().getMask(_Xminus_);
      else if (oPort==4) maskPort=getOwnerRouter().getMask(_Yminus_);
      
      if ((msg_mask & maskPort)!=0) return true;
      else return false;
   }     
   
   if (deltaX>1 && oPort==1) return true;
   if (deltaY>1 && oPort==2) return true;
   if (deltaX<-1 && oPort==3) return true;
   if (deltaY<-1 && oPort==4) return true;
   
   return false;
   
}
//*************************************************************************
// end of file
