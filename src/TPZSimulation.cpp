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
//    File: TPZSimulation.cpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header





#include "TPZSimulation.hpp"

#ifndef __TPZCONST_H__
#include "TPZConst.hpp"        
#endif

#ifndef __TPZSBLDR_HPP__
#include "TPZSimulationBuilder.hpp"      
#endif

#ifndef __TPZTRAFI_HPP__
#include "TPZTrafficPattern.hpp"      
#endif

#include <stdio.h>

//*************************************************************************

   #define SPACES " "
    
//*************************************************************************

// Run time information
IMPLEMENT_RTTI_DERIVED(TPZSimulation,TPZComponent);

//*************************************************************************
//:
//  f: inline void setNetwork (TPZNetwork * net);
//  d:
//
//  f: inline getNetwork TPZNetwork * () const;
//  d:
//
//  f: inline void setSimulationCycles (unsigned long cycles);
//  d:
//
//  f: inline void setStopSimulation (unsigned long cycles);
//  d:
//
//  f: inline void setsid (unsigned seed);
//  d:
//
//  f: inline void setMessageLength (unsigned length);
//  d:
//
//  f: inline unsigned getMessageLength () const;
//  d:
//
//  f: inline void setPacketLength (unsigned length);
//  d:
//
//  f: inline unsigned getPacketLength () const;
//  d:
//
//  f: inline void setRadius (unsigned radius);
//  d:
//
//  f: inline unsigned getRadius () const;
//  d:
//
//  f: inline void setDefaultLoadSupply (double value);
//  d:
//
//  f: inline void setShowState (Boolean value);
//  d:
//
//  f: inline void setTrafficPattern (TPZTrafficPattern * pattern);
//  d:
//
//  f: inline void setShowBufferState (Boolean value);
//  d:
//
//  f: inline TPZString buffersFileName () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZSimulation (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZSimulation :: TPZSimulation(const TPZComponentId& id)
               : TPZComponent(id),
                 m_Network(0),
                 m_TrafficPattern(0),
                 m_SimulationCycles(0),
                 m_StopSimulation(~0),
                 m_StopSimulMessages(0),
                 m_burstWidth(~0),
                 m_burstNumber(1),
                 m_Seed(113),
                 m_MessageLength(0),
                 m_PacketLength(0),
		 m_verbosity(0),
                 m_q(0.0),
                 m_defaultLoad(-1.0),
                 m_defaultProb(-1.0),
                 m_Radius(0),
                 m_ShowState(true),
                 m_StartHour(0),
                 m_Clock(0),
                 m_BufferOutput(0),
                 m_ShowBufferState(false),
                 m_mensajestraza(0),
                 m_simulatedClock(0),
                 m_clockTR(0),
                 m_resetStats(0),
                 m_binaryOutputFormat(true),
                 m_flitSize(0),
                 m_RouterProcesorRatio(1.0),
                 m_unifyNetworks(0),
                 m_overrideNetwork(TPZString("")),
                 m_ProtocolTypes(1),
                 m_terminatingMsgNumber(0),
                 m_isRafagaMode(false),
                 m_pipelineNormal(0),
                 m_discardTraffic(false),
                 m_pipelineBypass(0)
{
   setSimulation(this);
   disableNotification();
}


//*************************************************************************
//:
//  f: virtual ~ TPZSimulation ();
//
//  d:
//:
//*************************************************************************

TPZSimulation :: ~TPZSimulation()
{
   getNetwork()->terminate();
   
   if(m_BufferOutput)
   {
      m_BufferOutput -> close();
      delete m_BufferOutput;
   }
#ifdef PTOPAZ
   for(int n=0; n<m_numberThreads; n++)
   {
      if(m_MessagePool[n]) delete m_MessagePool[n];
   }
   m_MessagePool.clear();
#else
   if(m_MessagePool) delete m_MessagePool;
#endif
      
   delete m_TrafficPattern;
   delete m_Network;
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: initialize()
{
   if( isInitializated() ) return;
   m_StartHour = time(0);
   m_Radius = m_Network->getSizeX();

   if(m_Network) m_Network->initialize();
   Inhereited::initialize();   
}

//*************************************************************************
//:
//  f: Boolean injectRequestMessage ()
//
//  d:
//:
//*************************************************************************
Boolean TPZSimulation :: injectProtocolMessage(const TPZPosition& source, const TPZPosition& destiny, unsigned type)
{
   m_TrafficPattern->injectProtocolMessage(source, destiny, type);
   return true;
}

Boolean TPZSimulation :: injectProtocolRandomMessage(const TPZPosition& source, unsigned type)
{
   m_TrafficPattern->injectProtocolRandomMessage(source, type);
   return true;
}
//*************************************************************************
//:
//  f: void setLoadSupply (double A);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: setLoadSupply(double value)
{
   // Correction in some values when the traffic is
   // bimodal
   double realPacketLegth=m_PacketLength;
   
   if(getTrafficPattern()->isBimodal() && getTrafficPattern()->getMessagesBimodal()<1)
   {
      realPacketLegth=m_PacketLength*(1+getTrafficPattern()->getProbBimodal()*(getTrafficPattern()->getMessagesBimodal()-1));
   }
   m_q = 8.0*value/m_Radius/m_MessageLength/realPacketLegth;

   
}

//*************************************************************************
//:
//  f: void setLoadProb (double A);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: setLoadProb(double value)
{
   unsigned realPacketLegth=m_PacketLength;
   
   if(getTrafficPattern()->isBimodal() && getTrafficPattern()->getMessagesBimodal()<1)
   {
      realPacketLegth=(unsigned int)(m_PacketLength*(1+getTrafficPattern()->getProbBimodal()*(1-getTrafficPattern()->getMessagesBimodal())));
   }
   double probMCast=getTrafficPattern()->getProbMulticast();
   double lengthMCast=getTrafficPattern()->getLengthMulticast();
   m_q=value*(1-probMCast)/realPacketLegth+value*probMCast/(lengthMCast*realPacketLegth); //unicast + broadcast component

}

//*************************************************************************
//:
//  f: TPZMessagePool& getMessagePool() const;
//
//  d:
//:
//*************************************************************************
TPZMessagePool& TPZSimulation :: getMessagePool() const
{
#ifdef PTOPAZ
   int n = getNetwork()-> getThreadID(pthread_self());
   TPZMessagePool* pool = m_MessagePool[n];
   return *pool;
#else
   return *m_MessagePool;
#endif   
}

#ifdef PTOPAZ
//*************************************************************************
//:
//  f: TPZMessagePool& getMessagePool(unsigned n) const;
//
//  d:
//:
//*************************************************************************
TPZMessagePool& TPZSimulation :: getMessagePool(unsigned n) const
{
  TPZMessagePool* pool = m_MessagePool[n];
  return *pool;
}
#endif 

//*************************************************************************
//:
//  f: void initializePool(unsigned size);
//
//  d:
//:
//*************************************************************************
void TPZSimulation :: initializePool(unsigned size)
{
    //If in any occasion you are in trouble with the
    //allocation of new messages because pool size is
    //too small, it can be re-dimensioned here, multipying
    //current size by an unsigned value >1
    
    //The probability of finding such a situation is higher
    //for multithreaded simulations, because pool is
    //distributed among threads.
    
    assert(size>0);
#ifdef PTOPAZ
    assert(m_numberThreads>0);
    if(m_MessagePool.size()==0)
    {
      m_poolSize=(size)/m_numberThreads;
      m_MessagePool.setSize(m_numberThreads);
      for(int n=0; n<m_numberThreads; n++)
      {
         m_MessagePool[n] = new TPZMessagePool(m_poolSize);
      }
    }
#else
   m_poolSize=size;
   m_MessagePool = new TPZMessagePool(m_poolSize);   
#endif


}

//*************************************************************************
//:
//  f: virtual void run (utime time = 0);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: run(uTIME time)
{
   
   /*if((getNetwork()->getMessagesInNet() == 0))
   {
      if( getTrafficPattern()->isEmpty())  
      {
         m_Clock+=time;
         return;
      }  
   }*/

   if( time != 0)
   {
      
      //With this line we could emulate frecuency relations between
      // processor and network non divisible (float)
      //if (drand48 ()> 1.0/m_RouterProcesorRatio) return;

      // In this case, runs the simulator for "time" cycle,
      // and returns. We do not control the time when simulation
      // execution must finish, but we verify when we must
      // stop injecting.
      //Extern simulator runs thorugh this. Therefore multithreaded injection
      //is not active.

      for( uTIME tempClk=0; tempClk<time; tempClk++ )
      {
#ifndef PTOPAZ
         //In multihread simulation each slave thread generates the packets originated 
         //in his portion of the simulated network. See TPZThread::run

         if( m_Clock <= m_StopSimulation )
         {
            getTrafficPattern()->generateMessages(m_q);
         }
#endif   
         if(m_Clock==m_resetStats)
         {
            //Cleaning cycles of warmup start
            getNetwork()->initializeStats(m_resetStats);
         }
         getNetwork()->run(m_Clock++);
         writeCurrentData();
      }
      return;
   }

#ifndef NO_TRAZA
    cerr << "WARNING! Dumping Trace to file" << endl;
#endif

   if( m_ShowState )
   {
#ifndef NO_POOL
      cout<<"WARNING!: Simulator is using a Flit Static Pool. This may be non-optimal for "<<endl;
      cout<<"         large network or multithreaded simulations. You should consider to "<<endl;
      cout<<"         change NO_POOL preprocessor variable in Makefile "<<endl;
      cout<< "--> mbytes_resident: " << process_memory_resident() << endl;
      cout<< "--> mbytes_total: " << process_memory_total() << endl;  
#endif
      if( isDiscardTraffic() == false)
      {
          cout<<"WARNING!: Packets are not discarded at injection queues"<< endl;
          cout<<"          expect large memory footprint if simulation is"<< endl;
          cout<<"          long and network is saturated "<<endl;
          cout<<"          (use -D option to avoid this)"<< endl;
          cout<<" "<<endl;
      }   
#ifdef PTOPAZ
      cout<<endl<<" Multithreaded TOPAZ with " << m_numberThreads <<" threads "<<endl<<endl;
#endif
      cout << "Simulating.." << endl; 
   }  

   if( getTrafficPattern() -> isFromFile() )
   {
      
      for ( m_Clock=1; m_Clock<=m_SimulationCycles; m_Clock++ )
      {  
         getTrafficPattern()->injectMessage();
	 getNetwork()->run(m_Clock);
         writeCurrentData();

         m_Clock++;// Delay modeling processor speed is in the
                   //TPZTraffficPatternTrace, when initializing the trace
         
      }
   }
   else 
   {   
       for( m_Clock=1; m_Clock<=m_SimulationCycles; m_Clock++ )
       {
       
          //BurstMode Emulation.
          if( (m_Clock > m_StopSimulation || m_StopSimulMessages==0) && m_isRafagaMode)
          {
                //double check (much Heavier than previous)
                if(getNetwork()->getMessagesTx()-getNetwork()->getMessagesRx()
                +m_Network->getMessagesToTx()==0 )
                {
                   if( getBurstRepet()!=1 )
                   {
                      unsigned pending=getBurstRepet()-1;
                      setBurstRepet(pending);
                      setStopSimulation(m_Clock+getBurstWidth());
                   }
                   else
                   if(m_Clock>=100)
                   {
                      m_SimulationCycles=m_Clock;
                   } 
                }
         }//END BURST
                  
         if( m_Clock <= m_StopSimulation )
         {
#ifndef PTOPAZ
            //To avoid injector synch, if multithreade, each thread runs his own. See TPZThread. This change does not affect 
            //GEMS simulation becasue internal traffic is always set to EMPTY in such case. This small change increases 
            //significantly simulator speedup.
            getTrafficPattern()->generateMessages(m_q); 
#endif
         } 
         if(m_Clock==m_resetStats)
         {
            //Cleaning cycles of cold start
            getNetwork()->initializeStats(m_resetStats);
         }
         getNetwork()->run(m_Clock);   
         writeCurrentData();
         
      } // for
   }
   
   m_Clock--;
}

//*************************************************************************
//:
//  f: void writeCurrentData ();
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: writeCurrentData()
{
   unsigned long step;
   step= long(m_SimulationCycles/12);

   
   if( (m_Clock%100) == 0 )
   {
      if( showBufferState() && m_Clock%showResolutionBS()==0) writeBufferStatus();
      if( isEnableNotification() )
      {
         double clock = m_Clock;
         double latenciaMedia = 0.0;
         double msgTx = m_Network->getMessagesTx();
         double msgRx = m_Network->getMessagesRx();         
            
         if( msgRx )
         {
           latenciaMedia=double(  m_Network->getFlitsRx()/m_Clock);
         }
         TPZNotificationEvent event(EV_LATENCY,clock,latenciaMedia,msgTx,msgRx);
         notifyObservers(event);
      }
      
   }
      
   if( m_ShowState && ((m_Clock%step) == 0) )
   {
         cerr << "*";
         cerr.flush();
   }

}


//*************************************************************************
//:
//  f: void writeResults ();
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: writeResults()
{
   TPZString buffer=TPZString(writeSimulationStatus());
   
   if( ! isEnableNotification() )
   {
      cout << buffer;
   }
   else
   {
      TPZNotificationEvent event(EV_RESULT,buffer);
      notifyObservers(event);
   }
}


//*************************************************************************
//:
//  f: void writeSimulationStatus (ostream & os);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: writeSimulationStatus(ostream& os)
{                 
   os << writeSimulationStatus();
   os.flush();
}

//*************************************************************************
//:
//  f: char * writeSimulationStatus ();
//
//  d:
//:
//*************************************************************************

TPZString TPZSimulation :: writeSimulationStatus()
{     
   if(m_Clock<=m_resetStats)
   {
      m_resetStats=0;
   }
   
   double cargaAplicada = m_Network->getFlitsTx()*100.0/8.0/m_Radius/(m_Clock-m_resetStats);
   double cargaReal = m_Network->getFlitsRx()*100.0/8.0/m_Radius/(m_Clock-m_resetStats);

   double throughput = m_Network->getFlitsRx()/double((m_Clock-m_resetStats));
   if( getNetwork()->getSizeZ() > 1 )
   {
      cargaAplicada /= m_Radius;
      cargaReal /= m_Radius;   
   }
   
   unsigned index= getProtocolMessTypes();
   unsigned nodes=getNetwork()->getSizeX();
   if(getNetwork()->getSizeY()!=0)
      nodes=nodes*getNetwork()->getSizeY();
   if(getNetwork()->getSizeZ()!=0)
      nodes=nodes*getNetwork()->getSizeZ();
                            
   double latMediaMsgTotal = m_Network->getTotalDelay(TPZNetwork::Message) / double(m_Network->getMessagesRx());

   double latMediaMsgNetwork = m_Network->getNetworkDelay(TPZNetwork::Message) / double(m_Network->getMessagesRx());

   double latMediaMsgBuffer = m_Network->getBufferDelay(TPZNetwork::Message) / double(m_Network->getMessagesRx());

   double latMediaPaqTotal = m_Network->getTotalDelay(TPZNetwork::Packet) / double(m_Network->getPacketsRx());

   double latMediaPaqNetwork = m_Network->getNetworkDelay(TPZNetwork::Packet) / double(m_Network->getPacketsRx());

   double latMediaPaqBuffer = m_Network->getBufferDelay(TPZNetwork::Packet) / double(m_Network->getPacketsRx());

   double distanciaMedia = double(m_Network->getTotalDistance()) / double(m_Network->getMessagesRx());
   
   double cargaAplicadaFC  = (double)m_Network->getFlitsTx()/(double)(m_Clock-m_resetStats);
   
   double throughputNormalizado = throughput/nodes;
      
   double cargaAplicadaNormalizada= cargaAplicadaFC/nodes;
   
   double minimoInjectadoNormalizado =(double)m_Network->lowerFlitsSent()/double((m_Clock-m_resetStats));
   
   double maximoInjectadoNormalizado = (double)m_Network->upperFlitsSent()/double((m_Clock-m_resetStats));
         
   double minimoInjectado = minimoInjectadoNormalizado*nodes;
   
   double maximoInjectado =  maximoInjectadoNormalizado*nodes;
   
   time_t localHour2 = time(0);
   TPZString buffer;
   
   //****************************************************************************************************************************
   // VERBOSITY=0
   //****************************************************************************************************************************
   buffer += TPZString("\n******************* NET CONFIGURATION *********************") +
	     TPZString("\n Network        : ") + asString() +
             TPZString("\n Started at     : " ) + ctime(&m_StartHour) +
             TPZString(" Ended at       : " ) + ctime(&localHour2);
   buffer += TPZString("***********************************************************");

   buffer += TPZString("\n Simulation time         = ") + 
             convertSeconds(difftime(localHour2,m_StartHour))+ TPZString(" (")+TPZString(difftime(localHour2,m_StartHour)) + TPZString(" secs)")+
             TPZString("\n");
   buffer += TPZString(" Memory Footprint        = ") + TPZString(process_memory_total()) + TPZString(" MBytes)")+TPZString("\n");
   buffer += TPZString(" Traffic Pattern         = ") + m_TrafficPattern->asString() +
             TPZString("\n Seed                    = ") + TPZString(m_Seed) +                           
             TPZString("\n Cycles simulated        = ") + TPZString(m_Clock) +
             TPZString("\n Cycles deprecated       = ") + TPZString(m_resetStats) +
             TPZString("\n Buffers size            = ") + TPZString(globalData().routerBufferSize()) +
             TPZString("\n Messages length         = ") + TPZString(m_MessageLength) + " packet(s)" +
             TPZString("\n Packets length          = ") + TPZString(m_PacketLength) + " flits" +
	     TPZString("\n************************ PERFORMANCE ***********************") +
             TPZString("\n Supply Thr. Norm        = ") + TPZString(cargaAplicadaNormalizada) + " flits/cycle/router " +
             TPZString("\n Accept Thr. Norm        = ") + TPZString(throughputNormalizado) + " f/c/r (m: " 
                                                       + TPZString(minimoInjectadoNormalizado)+ ", M:" +
                                                       + TPZString(maximoInjectadoNormalizado) +")"+
             TPZString("\n Supply Thr.             = ") + TPZString(cargaAplicadaFC) + " f/c" +
             TPZString("\n Throughput              = ") + TPZString(throughput) + " f/c (m: " 
                                                       + TPZString(minimoInjectado)+ ", M:" +
                                                       + TPZString(maximoInjectado) +")"+
             TPZString("\n Average Distance        = ") + TPZString(distanciaMedia) +                       
             TPZString("\n Messages generated      = ") + TPZString(m_Network->getMessagesTx()) +
             TPZString("\n Messages received       = ") + TPZString(m_Network->getMessagesRx()) +
             TPZString("\n Messages to inject      = ") + TPZString(m_Network->getMessagesToTx()) +
	     TPZString("\n Messages on Escape      = ") + TPZString(m_Network->getMessagesEscape()) +
             TPZString("\n Total message latency   = ") + TPZString(latMediaMsgTotal) +
             TPZString("\n Network message latency = ") + TPZString(latMediaMsgNetwork) +
             TPZString("\n Buffer message latency  = ") + TPZString(latMediaMsgBuffer) +
             TPZString("\n Maximum message latency = ") + TPZString(m_Network->getMaximLatency(TPZNetwork::Message)) +
             TPZString("\n Last Message cycle      = ") + TPZString(m_LastMessage) + TPZString("\n");
             buffer += TPZString("***********************************************************\n");

             TPZString("\n\n******************** EVENT COUNTING **********************")+
             TPZString("\n Buffer Writes           = ") + TPZString(m_Network->getEventCount( TPZNetwork::BufferWrite))+
             TPZString("\n Buffer Reads            = ") + TPZString(m_Network->getEventCount( TPZNetwork::BufferRead))+
             TPZString("\n VC Arbitrations         = ") + TPZString(m_Network->getEventCount( TPZNetwork::VCArbitration))+
             TPZString("\n SW Arbitrations         = ") + TPZString(m_Network->getEventCount( TPZNetwork::SWArbitration))+
             TPZString("\n SW Traversal            = ") + TPZString(m_Network->getEventCount( TPZNetwork::SWTraversal))+
	     TPZString("\n Router Bypass           = ") + TPZString(m_Network->getEventCount( TPZNetwork::RouterBypass))+
             TPZString("\n Link Traversal          = ") + TPZString(m_Network->getEventCount( TPZNetwork::LinkTraversal))+
	     TPZString("\n\n**********************************************************")+
	     TPZString("\n");
             
	     //****************************************************************************************************************************
             // VERBOSITY=1
             //****************************************************************************************************************************
             if( m_verbosity>0 ) for (int i=1; i<=index; i++)
             {
                buffer += TPZString("\n****************** VIRTUAL NETWORK ") + TPZString(i) + TPZString(" *********************");
                double distanciaMediaProtocol = double(m_Network->getProtocolAverageDistance (i))/double(m_Network->getProtocolMessagesRx(i));
                buffer += TPZString("\nAverage_distance_vn_") + TPZString(i) + TPZString("        = ") + TPZString(distanciaMediaProtocol);
                buffer += TPZString("\nMessages_generated_vn_") + TPZString(i) + TPZString("      = ") + TPZString(m_Network->getProtocolMessagesTx(i));
                buffer += TPZString("\nMessages_received_vn_") + TPZString(i) + TPZString("       = ") + TPZString(m_Network->getProtocolMessagesRx(i));
                double latenciaMediaTotalProtocol = double(m_Network->getProtocolMessagesDelayTotal(i))/double(m_Network->getProtocolMessagesRx(i));
                buffer += TPZString("\nTotal_message_latency_vn_") + TPZString(i) + TPZString("   = ") + TPZString(latenciaMediaTotalProtocol);
                double latenciaMediaNetworkProtocol = double(m_Network->getProtocolMessagesDelayNetwork(i))/double(m_Network->getProtocolMessagesRx(i));
                buffer += TPZString("\nNetwork_message_latency_vn_") + TPZString(i) + TPZString(" = ") + TPZString(latenciaMediaNetworkProtocol);
                double latenciaMediaBufferProtocol = double(m_Network->getProtocolMessagesDelayBuffer(i))/double(m_Network->getProtocolMessagesRx(i));
                buffer += TPZString("\nBuffer_message_latency_vn_") + TPZString(i) + TPZString("  = ") + TPZString(latenciaMediaBufferProtocol);
                buffer += TPZString("\nMaximin_message_latency_vn_") + TPZString(i) + TPZString(" = ") + TPZString(m_Network->getProtocolMaxMessagesLatency(i));
                buffer += TPZString("\n**********************************************************");
                buffer += TPZString("\n");
             }
	     
	     //****************************************************************************************************************************
             // VERBOSITY=2
             //****************************************************************************************************************************
             if( m_verbosity>1 )
             {
	        buffer += TPZString("\n******************* INJECION MAP (flits/cycle) ***********");
                buffer += TPZString("\n");
                for (int l=0; l<m_Network->getSizeZ(); l++)
                {
                   buffer += TPZString("\n************************** Z=") + TPZString(l) +TPZString(" **************************");
		   buffer += TPZString("\n");
		   for (int k=0; k<m_Network->getSizeY(); k++)
                   {
                      for (int j=0; j<m_Network->getSizeX(); j++)
                      {
                         buffer += TPZString(double(m_Network->getInjectedFlitsAt(j,k,l))/double((m_Clock-m_resetStats)));
                         if (j==m_Network->getSizeX()-1) buffer += TPZString("\n");
                         else buffer += TPZString("\t");
                      }
                   }
                }
                buffer += TPZString("\n");
                buffer += TPZString("\n****************** CONSUMPTION MAP (flits/cycle) ********");
                buffer += TPZString("\n");
                double suma;
                for (int l=0; l<m_Network->getSizeZ(); l++)
                {
	           buffer += TPZString("\n************************** Z=") + TPZString(l) +TPZString(" **************************");
		   buffer += TPZString("\n");
		   for (int k=0; k<m_Network->getSizeY(); k++)
                   {
                      for (int j=0; j<m_Network->getSizeX(); j++)
                      {
                         buffer += TPZString(double(m_Network->getReceivedFlitsAt(j,k,l))/double((m_Clock-m_resetStats)));
                         if (j==m_Network->getSizeX()-1) buffer += TPZString("\n");
                         else buffer += TPZString("\t");
                      }
                   }
                }
	     }
	     //****************************************************************************************************************************
             // VERBOSITY=3
             //****************************************************************************************************************************
             if( m_verbosity>2 )
             {
                buffer += TPZString("\n");
                buffer += TPZString("\n****************** LINK USAGE MAP (%) *******************");
                buffer += TPZString("\n");
                for (int l=0; l<m_Network->getSizeZ(); l++)
                {
	           buffer += TPZString("\n************************** Z=") + TPZString(l) +TPZString(" **************************");
		   buffer += TPZString("\n");
		   for (int k=0; k<m_Network->getSizeY(); k++)
                   {
                      for (int j=0; j<m_Network->getSizeX(); j++)
                      {
                         buffer += TPZString(double(m_Network->getLinkUtilizationAt(j,k,l))/double(4*(m_Clock)));
                         if (j==m_Network->getSizeX()-1) buffer += TPZString("\n");
                          else buffer += TPZString("\t");
                      }
                   }
                }
             }
             
   if( getTrafficPattern()->isBimodal() || (m_MessageLength>1) )
   {
      buffer += TPZString("\nTotal packet latency    = ") + TPZString(latMediaPaqTotal) +
                TPZString("\nNetwork packet latency  = ") + TPZString(latMediaPaqNetwork) +
                TPZString("\nBuffer packet latency   = ") + TPZString(latMediaPaqBuffer) +
                TPZString("\nMaximum packet latency  = ") + 
                TPZString(m_Network->getMaximLatency(TPZNetwork::Packet));
   }
   
             

   
   buffer+=m_Network->getReport();
   if(m_Network->getReport()!=TPZString(""))
   {
      buffer+=m_Network->printHistogram();
   }
   if( m_Network->getDumpMapInjectors()==1)
   {
      buffer+=m_Network->printMapInjectors(m_Clock-m_resetStats);
   }        
  return buffer;

}
//*************************************************************************
//:
//  f: void resetStats ();
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: resetStats()
{
  cout<<endl<<"<TOPAZ> Clearing Stats at "<<m_Clock<<" </TOPAZ>"<<endl;     
  m_Network->initializeStats(m_Clock);
  m_resetStats=m_Clock;
}
//*************************************************************************
//:
//  f: TPZString convertSeconds (double secs);
//
//  d:
//:
//*************************************************************************

TPZString TPZSimulation :: convertSeconds(double secs)
{
   char buffer[10];
   unsigned long hours=0, minutes=0;
   unsigned long seconds = (unsigned long)secs;
   if( seconds > 3600 )
   {
      hours = (unsigned long)(seconds/3600);
      seconds = seconds%3600;
   }
   if( seconds > 60 )
   {
      minutes = (unsigned long)(seconds/60);
      seconds = seconds%60;
   }
   
   sprintf(buffer,"%02d:%02d:%02d\0",hours,minutes,seconds);
   return buffer;
}

//*************************************************************************
//:
//  f: void setBuffersFileName (TPZString const & name);
//
//  d:
//:
//*************************************************************************
void TPZSimulation ::setBuffersFileName(const TPZString& name)
{
   m_BuffersFileName = name;
      TPZString parameters = name;
   parameters.replace(',',' ');

   if(parameters.word(1)==TPZString("")) EXIT_PROGRAM("Output file required");
   m_BuffersFileName = parameters.word(1);
   if(parameters.word(2)==TPZString(""))
   {
       m_BufferStateResolution=100;
   }
   else
   {
       m_BufferStateResolution=unsigned((parameters.word(2)).asInteger());
       setAsciiOutputFormat();
   }
   
}

//*************************************************************************
//:
//  f: void writeBufferStatus ();
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: writeBufferStatus()
{
   if( ! m_BufferOutput )
   {
      m_BufferOutput = new ofstream(buffersFileName());     
   }
   
   m_Network -> writeBufferStatus(*m_BufferOutput,getAsciiOutputFormat());
}


//*************************************************************************
//:
//  f: void setTrafficPattern (TPZString const & name);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: setTrafficPattern(const TPZString& name)
{
   Boolean isBimodal = false;
   Boolean isReactive = false;
   Boolean isMulticast = false;
      
   double probBimodal;
   double msgBimodal;
   unsigned numMessTypes;
   double probMulticast;
   unsigned lengthMulticast;
   
   if( getTrafficPattern() )
   {
      if( getTrafficPattern()->isBimodal() ) isBimodal=true;
      else if ( getTrafficPattern()->isReactive() ) isReactive=true;
      else if (getTrafficPattern()->isMulticast() ) isMulticast=true;
      
      probBimodal = getTrafficPattern()->getProbBimodal();
      msgBimodal  = getTrafficPattern()->getMessagesBimodal();
      numMessTypes = getTrafficPattern()->getNumMessTypes();
      probMulticast = getTrafficPattern()->getProbMulticast();
      lengthMulticast = getTrafficPattern()->getLengthMulticast();
      delete getTrafficPattern();
   }
   
   TPZTag tag(TPZ_TAG_PATTERN,TPZ_TAG_MODAL);
   if (isBimodal) 
   {
      TPZTag tag1(TPZ_TAG_PATTERN,TPZ_TAG_BIMODAL);
      tag = tag1;
   }
   
   else if (isReactive)
   {
      TPZTag tag2(TPZ_TAG_PATTERN,TPZ_TAG_REACTIVE);
      tag = tag2;
   }
   
   else if (isMulticast)
   {
      TPZTag tag4(TPZ_TAG_PATTERN,TPZ_TAG_MULTICAST);
      tag = tag4;
   }
      
   if ( true )
   {
      tag.addAttribute(new TPZAttribute(TPZ_TAG_NUM_MESS_TYPES,numMessTypes));
   }
   
   if( isBimodal )
   {
      tag.addAttribute(new TPZAttribute(TPZ_TAG_PROBBIMODAL,probBimodal));
      tag.addAttribute(new TPZAttribute(TPZ_TAG_MSGBIMODAL,msgBimodal));   
   }
   
   if( isMulticast )
   {
      tag.addAttribute(new TPZAttribute(TPZ_TAG_NUM_MESS_TYPES,numMessTypes));
      tag.addAttribute(new TPZAttribute(TPZ_TAG_PROB_MULTICAST,probMulticast));
      tag.addAttribute(new TPZAttribute(TPZ_TAG_LENGTH_MULTICAST,lengthMulticast));   
   }
      
   // The traffic can come with 2 parameters
   TPZString tempName = name;
   tempName.replace(',',' ');
   
   if( tempName.word(1) != TPZString("") )
      tag.addAttribute(new TPZAttribute(TPZ_TAG_TYPE,tempName.word(1)));

   if( tempName.word(2) != TPZString("") )
      tag.addAttribute(new TPZAttribute(TPZ_TAG_PARAM1,tempName.word(2)));
   
   if( tempName.word(3) != TPZString("") )
      tag.addAttribute(new TPZAttribute(TPZ_TAG_PARAM2,tempName.word(3)));

   TPZTrafficPattern* pattern =
                      TPZTrafficPattern::createTrafficPattern(tag,*this);
		     
   setTrafficPattern(pattern);
}

//*************************************************************************
//:
//  f: void overrideNetwork (TPZString const & name);
//
//  d: overwrite the network from the command line
//:
//*************************************************************************
void TPZSimulation ::overrideNetwork(const TPZString& name )
{
   //destroyed the previously built in the sgml
   TPZNetwork* net=getNetwork();
   net->terminate();
   
   net= (TPZNetwork*)(TPZComponent::networkBuilder->
                     createComponentWithId(name,this));
   this->setNetwork(net);
   this->getTrafficPattern()->updateSizes();
}


//*************************************************************************
//:
//  f: void setBimodalTraffic (TPZString const & params);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: setBimodalTraffic(const TPZString& params)
{
   if( ! getTrafficPattern() )
   {
      setTrafficPattern( new TPZTrafficPatternRandom(*this) );
   }

   TPZString parameters = params;
   parameters.replace(',',' ');
   if( (parameters.word(1)==TPZString("")) || (parameters.word(2)==TPZString("")) )
   {
      EXIT_PROGRAM(ERR_TPZSIMUL_0012);
   }
   
   getTrafficPattern()->setBimodal();
   getTrafficPattern()->setMessagesBimodal(parameters.word(1).asInteger());
   getTrafficPattern()->setProbBimodal(parameters.word(2).asDouble());
}

//*************************************************************************
//:
//  f: void setTrafficPatternFile (TPZString const & name);
//
//  d:
//:
//*************************************************************************

void TPZSimulation :: setTrafficPatternFile(const TPZString& name)
{
   delete getTrafficPattern();
   TPZTrafficPattern* pattern = 
                      TPZTrafficPattern::createTrafficPattern(name,*this);
   setTrafficPattern(pattern);
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZSimulation :: asString() const
{
   return m_Network->asString();
}


//*************************************************************************
//:
//  f: static TPZSimulation * newFrom (TPZTag const * tag, index & index);
//
//  d:
//:
//*************************************************************************

TPZSimulation* TPZSimulation::newFrom(const TPZTag* tag, INDEX& index)
{   
   TPZComponentId idSimulation(*tag);
   TPZSimulation* simulation = new TPZSimulation(idSimulation);  
   simulation->parseTags(simulation,tag,index);
   return simulation;
} 
   
   
//*************************************************************************
//:
//  f: void parseTags (TPZSimulation * simulation, TPZTag const * tag,
//                                    INDEX & i);
//
//  d:
//:
//*************************************************************************
void TPZSimulation::parseTags(TPZSimulation *simulation, const TPZTag* tag, 
                                     INDEX& index)
{   
   TPZString endTag = TPZString("/") + tag->tagName();
   TPZString notClosed=tag->tagName();
   TPZTag* nextTag = TPZSimulation::simulationBuilder->
                     getTagWithIndex(++index);
   
   while( nextTag && nextTag->tagName()!=endTag )
   {
      TPZString tagName = nextTag->tagName();

      if(tagName==notClosed)
      {
         TPZString err=TPZString("Tag")+ notClosed+("Open on simulation sgml file\n");
         EXIT_PROGRAM(err);
      }
      if( tagName == TPZ_TAG_NETWORK )
      {
         // This network
         TPZString networkId = nextTag->tagId();
         TPZNetwork* net= (TPZNetwork*)(TPZComponent::networkBuilder->
                           createComponentWithId(networkId,simulation));
         simulation->setNetwork(net);
      }
      else if( tagName == TPZ_TAG_SIMULCYCLES)
      {
         TPZString ciclos = nextTag->tagId();
         simulation->setSimulationCycles(ciclos.asInteger());
      }
      else if( tagName == TPZ_TAG_STOPSIMUL )
      {
         TPZString stopSimulation = nextTag->tagId();
         simulation->setStopSimulation(stopSimulation.asInteger());      
      }
      else if( tagName == TPZ_TAG_STOPSIMUL_MSG )
      {
         TPZString stopSimulationMsg = nextTag->tagId();
         simulation->setStopSimulMessages(stopSimulationMsg.asInteger());
         m_isRafagaMode=true;
      }
      else if(tagName == TPZ_TAG_RESETSTATS)
      {
         TPZString resetStats = nextTag->tagId();
         simulation->setResetStats(resetStats.asInteger());    
      }
      else if( tagName == TPZ_TAG_MESSAGELENGTH )
      {
         TPZString messageLength = nextTag->tagId();
         simulation->setMessageLength(messageLength.asInteger());      
      }
      else if( tagName == TPZ_TAG_PACKETLENGTH )
      {
         TPZString packetLength = nextTag->tagId();
         simulation->setPacketLength(packetLength.asInteger());      
      }
      else if( tagName == TPZ_TAG_LOAD )
      {
         TPZString load = nextTag->tagId();
         simulation->setDefaultLoadSupply(load.asDouble());      
      }     
      else if( tagName == TPZ_TAG_PROB )
      {
         TPZString prob = nextTag->tagId();
         simulation->setDefaultLoadProb(prob.asDouble());      
      }     
      else if( tagName == TPZ_TAG_PATTERN )
      {
         TPZTrafficPattern* pattern = 
                            TPZTrafficPattern::createTrafficPattern(*nextTag,*simulation);
         simulation->setTrafficPattern(pattern);
      }
      else if( tagName == TPZ_TAG_SEED )
      {
         TPZString seed = nextTag->tagId();
         simulation->setSeed(seed.asInteger());      
      }
      else if( tagName == TPZ_TAG_RAFAGA)
      {
         TPZString anchura,number;
         nextTag->getAttributeValueWithName(TPZ_TAG_RAFAGA_WITDTH,anchura);
         nextTag->getAttributeValueWithName(TPZ_TAG_RAFAGA_NUMBER,number);
         simulation->setBurstWidth(anchura.asInteger());
         simulation->setBurstRepet(number.asInteger());
      }

      //Specific flags to work with ruby. 
      //   Link width in bytes
      //   Frecuency relation between processor and router
      //   virtual networks or fisical networks
      else if( tagName == TPZ_TAG_FLITSIZE )
      {
         TPZString flitsize = nextTag->tagId();
         simulation->setFlitSize(flitsize.asInteger());
      }
      else if( tagName == TPZ_TAG_NETWORKCLOCK )
      {
         TPZString routerProcesorRatio = nextTag->tagId();
         simulation->m_RouterProcesorRatio = routerProcesorRatio.asDouble();
      }
      else if( tagName == TPZ_TAG_UNIFYNETWORK )
      {
         TPZString unify = nextTag->tagId();
         simulation->m_unifyNetworks = unify.asInteger();
      }
      
      nextTag = TPZSimulation::simulationBuilder->getTagWithIndex(++index);      

   } // while

   simulation->setSimulation(simulation);
   simulation->initialize();

   
  // return simulation;
}

//*************************************************************************
// end of file
