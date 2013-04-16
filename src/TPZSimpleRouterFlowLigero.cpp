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
//    File: TPZSimpleRouterFlowLigero.cpp
//
//    Class:  TPZSimpleRouterFlowLigero
//
//    Inherited from:  TPZSimpleRouterFlow
// :
//*************************************************************************
//end of header



#include <TPZSimpleRouterFlowLigero.hpp>

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

#define CYC_TRAZA 100000

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZSimpleRouterFlowLigero,TPZSimpleRouterFlow);

//*************************************************************************
//:
//  f: TPZSimpleRouterFlowTorus ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowLigero :: TPZSimpleRouterFlowLigero( TPZComponent& component)
                 : TPZSimpleRouterFlow(component)
{
}


//*************************************************************************
//:
//  f: ~ TPZSimpleRouterFlow ();
//
//  d:
//:
//*************************************************************************

TPZSimpleRouterFlowLigero :: ~TPZSimpleRouterFlowLigero()
{
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlowLigero :: initialize()
{
   Inhereited :: initialize();
   TPZSimpleRouter& simpleRouter = (TPZSimpleRouter&)getComponent();
   m_vnets=simpleRouter.getVnets();
   m_ports=simpleRouter.numberOfInputs();
   unsigned ports=m_ports+1;
   
   if ( simpleRouter.getISSize()==0) m_ISSize=m_bufferSize;
   else m_ISSize=simpleRouter.getISSize();
   
   if ( simpleRouter.getOSSize()==0) m_OSSize=m_bufferSize;
   else m_OSSize=simpleRouter.getOSSize();
   
   if ( simpleRouter.getMPSize()==0) m_MPSize=m_bufferSize;
   else m_MPSize=simpleRouter.getMPSize();
   
   m_missLoops= simpleRouter.getMissLoops();
   m_missLimit= simpleRouter.getMissLimit();
   
   //Escape
   m_interfazOnEscape=new Boolean[ports];
   m_latch_escape=0;
   
   //Ordered
   m_interfazInOrder=new Boolean[ports];
   
   //Consumer
   m_consAvailable=true;
   m_clearConsConnection=false;
   m_latch_cons=new TPZMessage*[ports];
   m_fifos_cons=new TPZMessageQueue[ports];
   
   //Output Stage & injection
   m_linkAvailable=new Boolean[ports];
   m_clearConnection=new Boolean[ports];
   m_latch_out_byp=new TPZMessage*[ports];
   m_latch_out_turn=new TPZMessage*[ports];
   m_latch_out_inj=new TPZMessage*[ports];
   m_fifos_out_byp=new TPZMessageQueue[ports];
   m_fifos_out_inj=new TPZMessageQueue[ports];
   m_fifos_out_turn=new TPZMessageQueue[ports];
     
   //Multiport Buffers
   m_ringExitAvailable=new Boolean[ports];
   m_clearRingExit=new Boolean[ports];
   m_ringAdvanceAvailable=new Boolean[ports];
   m_clearRingAdvance=new Boolean[ports];
   m_mp_one_ctrl=new TPZMPStageState[ports];
   m_mp_two_ctrl=new TPZMPStageState[ports];
   m_latch_mp_one=new TPZMessage*[ports];
   m_latch_mp_two=new TPZMessage*[ports];
   m_fifos_mp_one=new TPZMessageQueue[ports];
   m_fifos_mp_two=new TPZMessageQueue[ports];
   
   //Input Stage
   m_istage_ctrl=new TPZIStageState[ports];
   //Queues are inhereited from parent class
   
   for(int i=0; i<ports; i++)
   {
      m_interfazOnEscape[i]=false;
      
      m_interfazInOrder[i]=false;
      m_latch_cons[i]=0;
      
      m_linkAvailable[i] = true;
      m_clearConnection[i] = false;
      m_latch_out_byp[i]=0;
      m_latch_out_turn[i]=0;
      m_latch_out_inj[i]=0;
      
      m_ringExitAvailable[i]=true;
      m_clearRingExit[i]=false;
      m_ringAdvanceAvailable[i]=true;
      m_clearRingAdvance[i]=false;
      m_mp_one_ctrl[i] = Advance;
      m_mp_two_ctrl[i] = Advance;
      m_latch_mp_one[i]=0;
      m_latch_mp_two[i]=0;
      
      m_istage_ctrl[i] = Bypass;   
   }
   
}


//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZSimpleRouterFlowLigero :: terminate()
{
   delete[] m_linkAvailable;
   
   delete[] m_latch_out_byp;
   delete[] m_latch_out_turn;
   delete[] m_latch_out_inj;
   
   delete[] m_latch_cons;
   
   delete[] m_latch_mp_one;
   delete[] m_latch_mp_two;
   
   delete[] m_fifos_cons;
   delete[] m_fifos_mp_one;
   delete[] m_fifos_out_byp;
   delete[] m_fifos_out_inj;
   delete[] m_fifos_out_turn;
   delete[] m_fifos_mp_two;
} 


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlowLigero :: inputReading()
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
            m_fifos_cons[0].enqueue(m_latch_escape);
	    m_escape_ctrl=_LocalNode_;
	    m_latch_escape=0;	    
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
               m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_one[inPort]);
	       m_mp_one_ctrl[inPort]=Leave;
	       m_ringExitAvailable[inPort]=false;
	       if(m_latch_mp_one[inPort]->isHeadTail()) m_clearRingExit[inPort]=true;
	       m_latch_mp_one[inPort]=0;
	       continue;
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
	       m_fifos_out_turn[(inPort%(m_ports-1))+1].enqueue(m_latch_mp_two[inPort]);
	       m_mp_two_ctrl[inPort]=Leave;
	       m_ringExitAvailable[inPort]=false;
	       if(m_latch_mp_two[inPort]->isHeadTail()) m_clearRingExit[inPort]=true;
	       m_latch_mp_two[inPort]=0;
	       continue;
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
               m_fifos_cons[inPort].enqueue(m_routing[inPort]);
	       m_istage_ctrl[inPort]= Consume;
	       m_routing[inPort]=0;
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
     if( m_routing[m_ports]->isHeader() || m_routing[m_ports]->isHeadTail() )
     {
        
	//********************************************************
	// BORRAR, INJECTION ON SCAPE ALWAYS
	//********************************************************
	//m_routing[m_ports]->setOrdered();
	//********************************************************
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
           m_fifos_out_inj[outPort].enqueue(m_routing[m_ports]);
	   m_injection_ctrl=outPort;
	   m_routing[m_ports]=0;
	}	
     }
     else
     {
        //********************************************************
	// BORRAR, INJECTION ON SCAPE ALWAYS
	//********************************************************
	//m_routing[m_ports]->setOrdered();
	//********************************************************
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

TPZROUTINGTYPE TPZSimpleRouterFlowLigero :: getInjectionDirection(TPZMessage* msg)
{
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
//  f: Boolean checkHeader (unsigned inPort, TPZMessage* msg)
//
//  d:
//:
//*************************************************************************
Boolean TPZSimpleRouterFlowLigero :: checkHeader (unsigned inPort, TPZMessage* msg)
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
      TPZPosition dest=msg->destiny();
      int posX=pos.valueForCoordinate(TPZPosition::X);
      int posY=pos.valueForCoordinate(TPZPosition::Y);
      int destX=dest.valueForCoordinate(TPZPosition::X);
      int destY=dest.valueForCoordinate(TPZPosition::Y);
      
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
   
   if (deltaX>1 && oPort==1) return true;
   if (deltaY>1 && oPort==2) return true;
   if (deltaX<-1 && oPort==3) return true;
   if (deltaY<-1 && oPort==4) return true;
   
   return false;
   
}

//*************************************************************************
//:
//  f: virtual Boolean updateMessageInfo (TPZMessage & msg, unsigned outPort);
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowLigero :: updateMessageInfo(TPZMessage* msg, unsigned outPort)
{  
   int deltaX = msg->delta(0);
   int deltaY = msg->delta(1);
      
   if (outPort==1)
   {
      msg->setRoutingPort(_Xplus_);
      msg->setDelta(deltaX-1,0);
   }
   else if (outPort==2)
   {
      msg->setRoutingPort(_Yplus_);
      msg->setDelta(deltaY-1,1);
   }
   else if (outPort==3)
   {
      msg->setRoutingPort(_Xminus_);
      msg->setDelta(deltaX+1,0);
   }
   else if (outPort==4)
   {
      msg->setRoutingPort(_Yminus_);
      msg->setDelta(deltaY+1,1);
   }         
   
   return true; // No change of direction
}

//*************************************************************************
//:
//  f: Boolean checkFlowControl (unsigned Port, TPZMessageQueue* queue, unsigned numBubble)
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowLigero :: checkFlowControl(unsigned Port, TPZMessageQueue* queue, unsigned numBubble, unsigned BufferSize) const
{
   unsigned bubble = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner())*numBubble;
   if(BufferSize - queue[Port].numberOfElements() < bubble) return false;
   
   return true;
}

//*************************************************************************
//:
//  f: Boolean checkEscapeFlowControl ()
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowLigero :: checkEscapeFlowControl() const
{
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned BufferSize = 3*sizeForCT;
   unsigned bubble = 2*sizeForCT;
   
   if(BufferSize - m_fifo_escape.numberOfElements() < bubble) return false;
   
   return true;
}
//*************************************************************************
//:
//  f: Boolean checkEscapePath (TPZROUTINGTYPE direction)
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowLigero :: checkEscapePath(TPZROUTINGTYPE direction)
{
   TPZRouter* router=getOwnerRouter().getNeighbour(direction);
   return router->getEscapePath();   
}

//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowLigero :: onReadyUp(unsigned interfaz, unsigned cv)
{  
   TPZMessage* msg;
   inputInterfaz(interfaz)->getData(&msg,cv); 
   
   msg->incDistance();
   msg->resetMultiportNumber();
   
   if (msg->isHeader() || msg->isHeadTail())
   {
      //If the message has been missrouted, deltas must be recalculated
      if (msg->getLastMissRouted()==true)
      {
         TPZNetwork* net = ((TPZSimulation*)(getComponent().getSimulation()))->getNetwork();
         int deltaX;
         int deltaY;
         int deltaZ;
         net->routingRecord(getOwnerRouter().getPosition(),msg->destiny(),deltaX,deltaY,deltaZ);
         msg->setDelta(deltaX,0);
         msg->setDelta(deltaY,1);
         msg->setDelta(deltaZ,2);
         msg->setLastMissRouted(false);
         msg->resetMultiportNumber();              
      }
      //if the message is on escape it must go through escape path
      if (msg->isOnScape()==true)
      {
         m_interfazOnEscape[interfaz]=true;
      }
   }
   
   if (m_interfazOnEscape[interfaz]==true)
   {
      //Body and Tail flits are also marked as onEscape
      if(msg->isOnScape()==false) msg->setOnScape();
      m_fifo_escape.enqueue(msg);
   }
   else
   {
      m_fifos[interfaz].enqueue(msg);
   }
   
   if (msg->isTail() || msg->isHeadTail()) m_interfazOnEscape[interfaz]=false;
   if (interfaz!=m_ports) ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::IStageTraversal);
   return true;         
}

//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZSimpleRouterFlowLigero :: outputWriting()
{
   unsigned inPort;
   unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());
   unsigned virtualChannel; 
  

   // Through all the buffers.
   for( inPort = 1; inPort <= m_ports*m_vnets; inPort++)
   {
      // If there is no room for another packet, send stop signal.
      //Only one injection virtual channel

      virtualChannel =(inPort-1)/m_ports+1;
      if( inPort%m_ports == 0 ) {virtualChannel=1; if(inPort>m_ports)continue;} 

      if((m_ISSize - m_fifos[inPort].numberOfElements()) <= sizeForCT)
      {
         inputInterfaz((inPort-1)%m_ports+1)->sendStopRightNow(virtualChannel);
      }
      else
      {
         inputInterfaz((inPort-1)%m_ports+1)->clearStopRightNow(virtualChannel); 
      }
   }
   
   //We also arbitrate here Escape Path
   if( (3*sizeForCT - m_fifo_escape.numberOfElements()) <= sizeForCT)
   {
      getOwnerRouter().setEscapePath(false);
   }
   else
   {
      getOwnerRouter().setEscapePath(true);
   }
   
   return true;
}
//*************************************************************************
//:
//  f: TPZROUTING getOurputPort (TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

TPZROUTINGTYPE  TPZSimpleRouterFlowLigero :: getOutputDirection(TPZMessage* msg)
{  
   if (msg->isOnScape())
   {
      TPZPosition pos=getOwnerRouter().getPosition();
      TPZPosition dest=msg->destiny();
      int posX=pos.valueForCoordinate(TPZPosition::X);
      int posY=pos.valueForCoordinate(TPZPosition::Y);
      int destX=dest.valueForCoordinate(TPZPosition::X);
      int destY=dest.valueForCoordinate(TPZPosition::Y);
      unsigned size_x= ((TPZNetwork*)(getOwnerRouter().getOwner()))->getSizeX();
      unsigned size_y= ((TPZNetwork*)(getOwnerRouter().getOwner()))->getSizeY();
      if(pos==dest) return _LocalNode_;
      
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
      int deltaX = msg->delta(0);
      int deltaY = msg->delta(1);
      if( msg->isHeader() || msg->isHeadTail() )
      {  
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
// end of file
