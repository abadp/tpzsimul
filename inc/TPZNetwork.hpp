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
//    File: TPZNetwork.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZNetwork_HPP__
#define __TPZNetwork_HPP__

//*************************************************************************

   #ifndef __TPZComponent_HPP__
   #include <TPZComponent.hpp>
   #endif
   
   #ifndef __TPZMatrix_HPP__
   #include <TPZMatrix.hpp>
   #endif

   #ifndef __TPZRouter_HPP__
   #include <TPZRouter.hpp>
   #endif
   
   #ifndef __TPZGlobalData_HPP__
   #include <TPZGlobalData.hpp>
   #endif
   
   #ifndef __TPZArray_HPP__
   #include <TPZArray.hpp>
   #endif
   
   #ifndef __TPZThread_HPP__
   #include <TPZThread.hpp>
   #endif
   
   #ifndef __TPZVector_HPP__
   #include <TPZVector.hpp>
   #endif

   #include <vector>
   
//*************************************************************************

   typedef TPZVector< TPZVector<unsigned long> >  TPZMatrixMessages;
   typedef TPZVector<unsigned long>             TPZVectorMessages;
   
   typedef TPZVector< TPZVector<double> >       TPZMatrixMessagesDelay;
   typedef TPZVector<double>                    TPZVectorMessagesDelay;
   
   typedef TPZVector< TPZVector<uTIME> >        TPZMatrixProtocolMaxMessageLatency;
   
   typedef TPZVector<long>                      TPZVectorLongs;
   
   class TPZThread;
//*************************************************************************

   class TPZNetwork : public TPZComponent
   {
   public:
      typedef TPZComponent Inhereited;
      typedef TPZMatrix<TPZRouter*> TPZRouterMatrix;
      typedef TPZMatrix<unsigned> TPZUnsignedMatrix;
      typedef TPZVector<TPZConnectionSet> TPZVectorConnections;
      typedef TPZVector<TPZThread*> TPZVectorThreads;
      typedef TPZVector<TPZQueue <TPZMessage*> > TPZPendindMessages;

      enum { Flit, Packet, Message };
      typedef enum { BufferWrite, BufferRead, VCArbitration, SWArbitration, SWTraversal, IStageTraversal, 
                     OStageTraversal, MPTraversal, LinkTraversal, RouterBypass} TPZTipoEvento;      
      
      TPZNetwork( const TPZComponentId& id,
                  const TPZString& routerId,
                  unsigned x, 
                  unsigned y, 
                  unsigned z=1 );
      ~TPZNetwork();

      TPZRouter* getRouterAt(const TPZPosition& position) const;
      TPZRouter* getRouterAt(unsigned x, unsigned y, unsigned z) const;
      
      unsigned getInjectedFlitsAt(unsigned x, unsigned y, unsigned z);
      void incrInjectedFlitsAt(unsigned x, unsigned y, unsigned z, unsigned number);
      void clearInjectedFlitsAt(unsigned x, unsigned y, unsigned z);
      unsigned getReceivedFlitsAt(unsigned x, unsigned y, unsigned z); 
      unsigned long getLinkUtilizationAt(unsigned x, unsigned y, unsigned z);
      unsigned long getLinkChangesDirectionAt(unsigned x, unsigned y, unsigned z);
      virtual void run(uTIME runTime=0);
      virtual void terminate();
      virtual void setSimulation(TPZComponent* simulation);
      
      virtual unsigned distance(const TPZPosition& src, const TPZPosition& dst)=0;
      virtual void     routingRecord( const TPZPosition& src,
                                      const TPZPosition& dst,
                                      int&  deltaX,
                                      int&  deltaY,
                                      int&  deltaZ,
                                      Boolean ordered=false ) = 0;
               Boolean isDORDeadlockable()
                         { return true;}
      
      virtual void generateDORMasks(TPZRouter* router);    
      virtual unsigned long long setMulticastMask(const TPZPosition& current,
                                        const TPZROUTINGTYPE& direction)=0;
      
      TPZQueue<TPZROUTINGTYPE> multiCastPorts(
      					unsigned long long maskXplus,
					unsigned long long maskXminus,
					unsigned long long maskYplus,
					unsigned long long maskYminus,
					unsigned long long maskLocalNode,
					unsigned long long msgMask);
					      
      Boolean sendMessage(TPZMessage* msg);
      Boolean sendMessage(const TPZMessage& msg);
      
      //VPV-RSIM 3d and rectangular networks
      virtual TPZPosition CreatePosition(int num);
      
      //Dynamic reporting of network parameters for RSIM)
      void setReportMsg(unsigned long numberMsg)
            { m_numberMsg=numberMsg;}
      
      unsigned long getReportMsg()
            {return m_numberMsg;}
      
      TPZString getReport()
            {return m_buffPrint;}
      
      void setReportCycles(unsigned long numberCycles)
      { m_numberCyclesToReport=numberCycles;}
      //**********************************//
      
      virtual void postInitialize();
      
      void writeBufferInformation(ostream& os);
      void writeBufferStatus(ostream& os,Boolean binario=true);
      void writeComponentStatus(ostream& os) const;
      
      void onHeaderPacketReceived(const TPZMessage* msg);
      void onPacketReceived(const TPZMessage* msg);
      void onMessageReceived(const TPZMessage* msg);

      double getTotalDelay(int type=Message) const;
      double getNetworkDelay(int type=Message) const;
      double getBufferDelay(int type=Message) const;
      void*  getExternalInfoAt(unsigned x, unsigned y, unsigned z=0);
      void*  getExternalInfoAt(TPZPosition pos);
      void   clearExternalInfoAt(TPZPosition pos);
      
      void   setBufferSize(unsigned size);

      uTIME getCurrentTime() const
      { return m_CurrentTime; }
      
      Boolean endSimulation () const
      { return m_kill;} 
      
      void setCurrentTime(uTIME time)
      {
         m_CurrentTime=time;
      }
     // TPZConnectionSet getConnectionList () const
     // {
     //    m_ConnectionSet return;
     // }
      TPZConnectionSet::Cursor getConnectionCursor()const
      {
          TPZConnectionSet::Cursor connectionCursor(m_ConnectionSet);
          return connectionCursor;
      }
      
      unsigned long getMessagesTx() const
      { return m_MessagesTx; }

      unsigned long getMessagesRx() const;
      
      unsigned long getMessagesEscape() const;

      unsigned long getPacketsTx() const
      { return m_PacketsTx; }
      
      unsigned long getPacketsRx() const;

      unsigned long getFlitsTx() const
      { return m_FlitsTx; }
      
      unsigned long getFlitsRx() const;

      void setChannelOperationMode(TPZString operationMode)
      {
         m_channelOperationMode = operationMode;
      }
      
      TPZString getChannelOperationMode() const
      { return m_channelOperationMode; }
      
      unsigned long getMessagesToTx() const;
      
      void initializeHistogram(unsigned); 
      
      void destroyHistogram();
      
      TPZString printHistogram();
      
      TPZString printMapInjectors(unsigned long clock ) const;
      
      unsigned getSizeX() const
      { return m_SizeX; }

      unsigned getSizeY() const
      { return m_SizeY; }
      
      unsigned getSizeZ() const
      { return m_SizeZ; }

      int Number_of_nodes();

      uTIME getConnectionDelay() const
      { return m_ConnectionDelay; }

      void setConnectionDelay(uTIME delay)
      { m_ConnectionDelay=delay; }
      
      void resetPacketDelayNetwork ();
      void resetPacketDelayTotal ();

      unsigned long getTotalDistance() const;

      void setDumpMapInjectors(unsigned freq)
      {
         m_dumpMapInjectors=freq;
      }
      
      unsigned getDumpMapInjectors() const
      {
          return m_dumpMapInjectors;
      }
      
      //getDiameter virtual unsigned () const
      //{Return 1;}
      void initializeStats(uTIME timeRestart);
      
      uTIME getMaximLatency(int type) const;
      
      void incrementTx(int type, unsigned number=1);

      unsigned long lowerFlitsSent() const;

      unsigned long upperFlitsSent() const;
      
      unsigned long flitSentsAt(TPZPosition pos) const; 
      
      //*******************************************************************
      //ADDED FOR MULTIPLE TRAFFIC SOURCES SHARING THE ONLY VIRTUAL CHANNEL
      //*******************************************************************
      
      void incrProtocolAverageDistance (unsigned index, unsigned long dist);
      
      unsigned long getProtocolAverageDistance ( unsigned index);
      
      void cleanProtocolAverageDistance ( unsigned index);
      
      void incrProtocolMessagesTx ( unsigned index );
      
      unsigned long getProtocolMessagesTx ( unsigned index );
      
      void cleanProtocolMessagesTx ( unsigned index );
      
      void incrProtocolMessagesRx ( unsigned index );
      
      unsigned long getProtocolMessagesRx ( unsigned index );
      
      void cleanProtocolMessagesRx ( unsigned index );
      
      void incrProtocolMessagesInj ( unsigned index );
      
      void decrProtocolMessagesInj ( unsigned index );
      
      unsigned long getProtocolMessagesInj ( unsigned index );
      
      void incrProtocolMessagesDelayNetwork ( unsigned index, uTIME delay );
      
      double getProtocolMessagesDelayNetwork ( unsigned index );
      
      void cleanProtocolMessagesDelayNetwork ( unsigned index );
      
      void incrProtocolMessagesDelayTotal ( unsigned index, uTIME delay );
      
      double getProtocolMessagesDelayTotal ( unsigned index );
      
      void cleanProtocolMessagesDelayTotal ( unsigned index );
      
      void incrProtocolMessagesDelayBuffer ( unsigned index, uTIME delay);
      
      double getProtocolMessagesDelayBuffer ( unsigned index );
      
      void cleanProtocolMessagesDelayBuffer ( unsigned index);
      
      void setProtocolMaxMessagesLatency ( unsigned index, uTIME time );
      
      uTIME getProtocolMaxMessagesLatency ( unsigned index );
      
      void cleanProtocolMaxMessagesLatency ( unsigned index);
      
      //******************************************************************
      //END OF ADDITION
      //******************************************************************
      //******************************************************************
      // Added for event counting
      //******************************************************************
      void incrEventCount( TPZTipoEvento evento);
      double getEventCount( TPZTipoEvento evento);
      //******************************************************************
      //END OF ADDITION
      //******************************************************************
#ifdef PTOPAZ 
      void setThreadID(long, int);

      int getThreadID(long);

      void messageRelease (TPZMessage* msg);

      pthread_barrier_t* getBarrierRouters ()  
      {
           return   &m_barrierRouters; 
      }
      pthread_barrier_t* getBarrierConnections ()  
      {
           return   &m_barrierConnections; 
      }
#endif

      // Run time information
      DEFINE_RTTI(TPZNetwork);
            
   protected:
      void addRouter(TPZRouter* router);
      virtual void initializeConnectionsFor(const TPZPosition& pos);
      TPZPosition positionOf(TPZROUTINGTYPE dir, const TPZPosition& pos);
            
      void incrementRx(int type, unsigned number=1);

      void incrementDelayTotal(int type, uTIME delay);
      void incrementDelayNetwork(int type, uTIME delay);
      void incrementDelayBuffer(int type, uTIME delay);
      void setMaximLatency(int type, uTIME time);
#ifdef PTOPAZ            
      void incrementMessagesDistance(unsigned long dst);
#else
      void incrementMessagesDistance(unsigned long dst)
      { m_AverageDistance += dst; }
#endif      
      
   
      TPZString getRouterId() const
      { return m_RouterId; }
         
   private:
      TPZRouterMatrix* m_RouterMatrix;
      TPZUnsignedMatrix* m_InjectedFlitsMap;
      TPZUnsignedMatrix* m_ReceivedFlitsMap;
      unsigned         m_SizeX;
      unsigned         m_SizeY;
      unsigned         m_SizeZ;
      uTIME            m_CurrentTime;
      uTIME            m_clockRestartStats;
#ifdef PTOPAZ     
      TPZVectorMessages          m_MessagesEscape;
      unsigned long              m_MessagesTx;
      TPZVectorMessages          m_MessagesRx;
      unsigned long              m_PacketsTx;
      TPZVectorMessages          m_PacketsRx;
      unsigned long              m_FlitsTx;
      TPZVectorMessages          m_FlitsRx;
      
      TPZVectorMessages          m_AverageDistance;
      
      TPZVectorMessagesDelay     m_MessageDelayTotal;
      TPZVectorMessagesDelay     m_MessageDelayNetwork;
      TPZVectorMessagesDelay     m_MessageDelayBuffer;
      
      TPZVectorMessagesDelay     m_PacketDelayTotal;
      TPZVectorMessagesDelay     m_PacketDelayNetwork;
      TPZVectorMessagesDelay     m_PacketDelayBuffer;
      TPZPendindMessages         m_messagesToRelease;
#else
      unsigned long    m_MessagesEscape;
      unsigned long    m_MessagesTx;
      unsigned long    m_MessagesRx;
      unsigned long    m_PacketsTx;
      unsigned long    m_PacketsRx;
      unsigned long    m_FlitsTx;
      unsigned long    m_FlitsRx;
      
      unsigned long    m_AverageDistance;
      
      double           m_MessageDelayTotal;
      double           m_MessageDelayNetwork;
      double           m_MessageDelayBuffer;
      
      double           m_PacketDelayTotal;
      double           m_PacketDelayNetwork;
      double           m_PacketDelayBuffer;
#endif
      
      uTIME            m_MaxPacketLatency;
      uTIME            m_MaxMessageLatency;
      
      TPZString        m_RouterId;
      uTIME            m_ConnectionDelay;

      unsigned long    m_numberMsg;
      unsigned long    m_pckRcvLast;
      uTIME            m_LastTime;
      TPZString        m_buffPrint;
      unsigned long    m_FlitsRcvLast;
      double           m_LastTotalLatency;
      unsigned long    m_AverageDistanceold;
      
      unsigned long   *m_Histogram;
      unsigned long    m_diametro;
      unsigned long    m_numberCyclesToReport;
      unsigned         m_dumpMapInjectors;
      //PAUL insertion
      unsigned         m_inicial;
      
      Boolean          m_kill;
      
      //*******************************************************************
      // ADDED FOR EVENT COUNTING
      //*******************************************************************
      double  m_BufferWrite;
      double  m_BufferRead;
      double  m_VCArbitration;
      double  m_SWArbitration;
      double  m_SWTraversal;
      double  m_LinkTraversal;
      double  m_IStageTraversal;
      double  m_OStageTraversal;
      double  m_MPTraversal;
      double  m_RouterBypass;
      //******************************************************************
      //END OF ADDITION
      //******************************************************************
      //*******************************************************************
      //ADDED FOR MULTIPLE TRAFFIC SOURCES SHARING THE ONLY VIRTUAL CHANNEL
      //*******************************************************************
      
      TPZMatrixMessages m_ProtocolMessagesTx;
      TPZMatrixMessages m_ProtocolMessagesInj;
      TPZMatrixMessages m_ProtocolMessagesRx;
      TPZMatrixMessages m_ProtocolAverageDistance;
      
      TPZMatrixMessagesDelay m_ProtocolMessagesDelayTotal;
      TPZMatrixMessagesDelay m_ProtocolMessagesDelayNetwork;
      TPZMatrixMessagesDelay m_ProtocolMessagesDelayBuffer;
      
      TPZMatrixProtocolMaxMessageLatency m_ProtocolMaxMessagesLatency;
            
         
      //******************************************************************
      //END OF ADDITION
      //******************************************************************
      TPZString        m_channelOperationMode;
      //PRIETOP insertion
#ifdef PTOPAZ
      TPZVectorThreads           m_process;
      bool                       m_firstTime;
      pthread_mutex_t            m_MessagesKey;
      TPZVectorLongs             m_threadsTable;
      TPZVectorConnections       m_ConnectionCursor;
      TPZConnectionSet           m_ConnectionCriticalCursor;
      unsigned                   m_numThreads;
      int                        m_flagConnection;
      //VPV way
      pthread_barrier_t          m_barrierRouters;
      pthread_barrier_t          m_barrierConnections;
   
#endif
   };

//*************************************************************************


#endif


// end of file
