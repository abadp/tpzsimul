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
//    File: TPZNetwork.cpp
//
//    Class:  TPZNetwork
//
//    Inherited from:  TPZComponent, TPZObserver
// :
//*************************************************************************
//end of header

#include <TPZNetwork.hpp>

#ifndef __TPZConnectionDelay_HPP__
#include <TPZConnectionDelay.hpp>
#endif

#ifndef __TPZComponentId_HPP__
#include <TPZComponentId.hpp>
#endif
#ifndef __TPZPortInput_HPP__
#include <TPZPortInput.hpp>
#endif

#ifndef __TPZPortOutput_HPP__
#include <TPZPortOutput.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZGlobalData_HPP__
#include <TPZGlobalData.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

#include <cmath>
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZNetwork, TPZComponent);

//*************************************************************************
//:
//  f: inline void setReportMsg utime (unsigned long numberMsg)
//  d: load dinamic evaluation RSIM
//
//  f: inline getCurrentTime utime () const;
//  d:
//
//  f: getMessagesTx inline unsigned long () const;
//  d:
//
//  f: getMessagesRx inline unsigned long () const;
//  d:
//
//  f: getMessagesToTx inline unsigned long () const;
//  d:
//
//  f: inline unsigned getSizeX () const;
//  d:
//
//  f: inline unsigned getSizeY () const;
//  d:
//
//  f: inline unsigned getSizeZ () const;
//  d:
//
//  f: inline getConnectionDelay utime () const;
//  d:
//
//  f: inline void setConnectionDelay (utime delay);
//  d:
//
//  f: inline double getTotalMessagesDelay () const;
//  d:
//
//  f: getTotalDistance inline unsigned long () const;
//  d:
//
//  f: inline getMaximLatency utime () const;
//  d:
//
//  f: inline void incrementMessagesTx ();
//  d:
//
//  f: inline void incrementMessagesRx ();
//  d:
//
//  f: inline void incrementMessagesDelay (utime delay);
//  d:
//
//  f: inline void incrementMessagesDistance (unsigned long dst);
//  d:
//
//  f: inline void setMaximLatency (utime time);
//  d:
//
//  f: inline TPZString getRouterId () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZNetwork (const TPZComponentId & id,
//                 const TPZString & RouterOS,
//                 unsigned x,
//                 unsigned and
//                 unsigned z);
//
//  d:
//:
//*************************************************************************

TPZNetwork::TPZNetwork(const TPZComponentId& id, const TPZString& routerId,
        unsigned x, unsigned y, unsigned z) :
            TPZComponent(id), m_RouterId(routerId), m_SizeX(x), m_SizeY(y), m_SizeZ(z),
            m_CurrentTime((uTIME)0.0), m_MessagesTx(0), m_MessagesRx(0), m_MessagesEscape(0),
            m_PacketsTx(0), m_PacketsRx(0), m_FlitsTx(0), m_FlitsRx(0),
            m_MessageDelayTotal((uTIME)0.0), m_MessageDelayNetwork((uTIME)0.0),
            m_MessageDelayBuffer((uTIME)0.0), m_PacketDelayTotal((uTIME)0.0),
            m_PacketDelayNetwork((uTIME)0.0), m_PacketDelayBuffer((uTIME)0.0),
            m_AverageDistance(0), m_MaxPacketLatency(0), m_MaxMessageLatency(0),
            m_ConnectionDelay(0), m_RouterMatrix(0), m_numberMsg(~0), m_pckRcvLast(0),
            m_FlitsRcvLast(0), m_LastTime(0), m_AverageDistanceold(0),
            m_buffPrint(TPZString("")), m_LastTotalLatency(0),
            m_clockRestartStats(0), m_numberCyclesToReport(~0),
            m_dumpMapInjectors(~0), 
            m_ProtocolMessagesTx(0), m_ProtocolMessagesInj(0),
            m_BufferWrite(0), m_BufferRead(0), m_VCArbitration(0),
            m_SWArbitration(0), m_SWTraversal(0), m_LinkTraversal(0), 
	    m_RouterBypass(0), m_IStageTraversal(0), m_OStageTraversal(0),
	    m_MPTraversal(0),
            m_ProtocolMessagesRx(0), m_ProtocolAverageDistance(0),
            m_ProtocolMessagesDelayTotal(0), m_ProtocolMessagesDelayNetwork(0),
            m_ProtocolMessagesDelayBuffer(0), m_ProtocolMaxMessagesLatency(0),
            m_inicial(0)
{
    m_RouterMatrix = new TPZRouterMatrix(m_SizeX, m_SizeY, m_SizeZ);
    m_RouterMatrix->initialize(0);
    m_InjectedFlitsMap = new TPZUnsignedMatrix(m_SizeX, m_SizeY, m_SizeZ);
    m_ReceivedFlitsMap = new TPZUnsignedMatrix(m_SizeX, m_SizeY, m_SizeZ);
    m_InjectedFlitsMap->initialize(0);
    m_ReceivedFlitsMap->initialize(0);
#ifdef PTOPAZ 
    m_firstTime = true;
    m_kill=false;
    pthread_mutex_init(&m_MessagesKey, NULL);
#endif
}

//*************************************************************************
//:
//  f: virtual ~ TPZNetwork ();
//
//  d:
//:
//*************************************************************************

TPZNetwork::~TPZNetwork() {
    delete [] m_Histogram;
    delete m_InjectedFlitsMap;
    delete m_ReceivedFlitsMap;
#ifdef PTOPAZ    
    pthread_mutex_unlock(&m_MessagesKey);
    pthread_mutex_destroy(&m_MessagesKey);
#endif
}

//*************************************************************************
//:
//  f: virtual void postInitialize ();
//
//  d:
//:
//*************************************************************************

void TPZNetwork::postInitialize() {
    int loop_end = 1;
#ifdef PTOPAZ
    m_numThreads = ((TPZSimulation*)getSimulation())-> getNumberThreads();
    m_threadsTable.setSize(m_numThreads);
    m_ConnectionCursor.setSize(m_numThreads);
    m_process.setSize(m_numThreads);
    loop_end=m_numThreads;

    //Ceil is important with large number of threads. Tha light thread is the last one
    unsigned int routersPerThread = (unsigned int)std::ceil(1.0*m_SizeX*m_SizeY*m_SizeZ/m_numThreads); 
    if ( routersPerThread * (m_numThreads - 1 ) > m_SizeX*m_SizeY*m_SizeZ)
    {
        TPZString err;
        err.sprintf(" Awkaward combination of Threads and Nodes. Last thread will be too loaded... exiting\n");
        EXIT_PROGRAM(err);
    }
    if ( m_SizeX*m_SizeY*m_SizeZ < m_numThreads   ) 
    {
        TPZString err;
        err.sprintf(" The number of threads is too high\n");
        EXIT_PROGRAM(err);
    }
#ifndef NO_POOL
    m_messagesToRelease.setSize(m_numThreads);
#endif
#endif

    m_ProtocolMessagesTx.setSize(loop_end);
    m_ProtocolMessagesInj.setSize(loop_end);
    m_ProtocolMessagesRx.setSize(loop_end);
    m_ProtocolAverageDistance.setSize(loop_end);
    m_ProtocolMessagesDelayTotal.setSize(loop_end);
    m_ProtocolMessagesDelayNetwork.setSize(loop_end);
    m_ProtocolMessagesDelayBuffer.setSize(loop_end);
    m_ProtocolMaxMessagesLatency.setSize(loop_end);


    unsigned protocolNumber = ((TPZSimulation*)getSimulation())->getProtocolMessTypes();
    for (int i=0; i<loop_end; i++) {
       m_ProtocolMessagesTx[i].setSize(protocolNumber+1);
       m_ProtocolMessagesInj[i].setSize(protocolNumber+1);
       m_ProtocolMessagesRx[i].setSize(protocolNumber+1);
       m_ProtocolAverageDistance[i].setSize(protocolNumber+1);
       m_ProtocolMessagesDelayTotal[i].setSize(protocolNumber+1);
       m_ProtocolMessagesDelayNetwork[i].setSize(protocolNumber+1);
       m_ProtocolMessagesDelayBuffer[i].setSize(protocolNumber+1);
       m_ProtocolMaxMessagesLatency[i].setSize(protocolNumber+1);
       for (int j=0; j<=protocolNumber; j++)
       {
          m_ProtocolMessagesTx[i][j]=0;
          m_ProtocolMessagesInj[i][j]=0;
          m_ProtocolMessagesRx[i][j]=0;
          m_ProtocolAverageDistance[i][j]=0;
          m_ProtocolMessagesDelayTotal[i][j]=0;
          m_ProtocolMessagesDelayNetwork[i][j]=0;
          m_ProtocolMessagesDelayBuffer[i][j]=0;
          m_ProtocolMaxMessagesLatency[i][j]=0;
       }
    }
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)

            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                TPZPosition pos=TPZPosition(i, j, k);
                TPZRouter* routerXp = getRouterAt(positionOf(_Xplus_, pos) );
                router->setNeighbourXplus(routerXp);
                TPZRouter* routerXm = getRouterAt(positionOf(_Xminus_, pos) );
                router->setNeighbourXminus(routerXm);
                TPZRouter* routerYp = getRouterAt(positionOf(_Yplus_, pos) );
                router->setNeighbourYplus(routerYp);
                TPZRouter* routerYm = getRouterAt(positionOf(_Yminus_, pos) );
                router->setNeighbourYminus(routerYm);
		TPZRouter* routerZp = getRouterAt(positionOf(_Zplus_, pos) );
                router->setNeighbourZplus(routerZp);
                TPZRouter* routerZm = getRouterAt(positionOf(_Zminus_, pos) );
                router->setNeighbourZminus(routerZm);
		generateDORMasks(router);
		#ifdef PTOPAZ
                int linearindex=i+j*m_SizeX+k*m_SizeX*m_SizeY;
		router->setThreadNumber(linearindex/routersPerThread);
                if (router->getThreadNumber() >= m_numThreads   ) 
                {
                   TPZString err;
                   err.sprintf(" The number of threads is not comptible with the number of routers");
                   EXIT_PROGRAM(err);
                }
		TPZWRITE2LOG( TPZString("Router belongs to thread ") + TPZString((int)router->getThreadNumber()));
		#endif

            }
#ifdef PTOPAZ
   assert(m_numThreads>0);
   TPZConnectionSet::Cursor connectionCursor(m_ConnectionSet);
   //Asign connections to each slave thread   
   forCursor(connectionCursor)
   {
      TPZConnection *Elemento = connectionCursor.element();
      int outputId=((TPZRouter*)Elemento->getOutputPort()->getOwner()->getOwner()->getRouter())->getThreadNumber();
      m_ConnectionCursor[outputId].addAsLast(Elemento); //All conections outgoing the router
   }
  
  // Each threads with its own statistics in order to avoid collisions
   
   m_MessagesEscape.setSize(m_numThreads);
   m_MessagesRx.setSize(m_numThreads);
   m_PacketsRx.setSize(m_numThreads);
   m_FlitsRx.setSize(m_numThreads);
   m_MessageDelayTotal.setSize(m_numThreads);
   m_MessageDelayNetwork.setSize(m_numThreads);
   m_MessageDelayBuffer.setSize(m_numThreads);
   m_PacketDelayTotal.setSize(m_numThreads);
   m_PacketDelayNetwork.setSize(m_numThreads);
   m_PacketDelayBuffer.setSize(m_numThreads);
   m_AverageDistance.setSize(m_numThreads);

   pthread_barrier_init(&m_barrierRouters,NULL,m_numThreads+1); 
   //Slaves does not need father for this (so, this barrier does not involves master thread)
   pthread_barrier_init(&m_barrierConnections,NULL,m_numThreads); 
 
   for (int n=0; n<m_numThreads; n++)
   {   
      m_process[n] = new TPZThread;
      if (m_process[n] -> Start(n, m_SizeX, m_SizeY, m_SizeZ, this, m_numThreads))
         perror("Error when creating thread\n");
      m_process[n] -> setConnectionThread(m_ConnectionCursor[n]);
      m_MessagesEscape[n]=0;
      m_MessagesRx[n]=0;
      m_PacketsRx[n]=0;
      m_FlitsRx[n]=0;
      m_MessageDelayTotal[n]=0;
      m_MessageDelayNetwork[n]=0;
      m_MessageDelayBuffer[n]=0;
      m_PacketDelayTotal[n]=0;
      m_PacketDelayNetwork[n]=0;
      m_PacketDelayBuffer[n]=0;
      m_AverageDistance[n]=0;
   }
      
#else
   m_MessagesEscape=0;
   m_MessagesRx=0;
   m_PacketsRx=0;
   m_FlitsRx=0;
   m_MessageDelayTotal=0;
   m_MessageDelayNetwork=0;
   m_MessageDelayBuffer=0;
   m_PacketDelayTotal=0;
   m_PacketDelayNetwork=0;
   m_PacketDelayBuffer=0;
   m_AverageDistance=0;
#endif
   m_MessagesTx=0;
   m_PacketsTx=0;
   m_FlitsTx=0;
}

//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*dtermina************************************************************************

void TPZNetwork::terminate() {
#ifdef PTOPAZ  
    int rc;
    pthread_barrier_wait(&m_barrierRouters);
    for (int n=0; n<m_numThreads; n++)
    {   
       m_process[n]->terminate();
    }
    if ((rc = pthread_barrier_destroy(&m_barrierRouters))!=0)
    {
      perror("Barrier Routers");
      return;
    }
    if ((rc = pthread_barrier_destroy(&m_barrierConnections))!=0)
    {
      perror("Barrier Connections");
      return ;
    }
    for (int n=0; n<m_numThreads; n++)
    {   
      delete m_process[n];
    }
#endif
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {

                TPZRouter* router = getRouterAt(i, j, k);
                router->terminate();
                delete router;
            }

    delete m_RouterMatrix;
    m_RouterMatrix = 0;

    Inhereited::terminate();
}

//*************************************************************************
//:
//  f: void addRoute (TPZPosition const & pos, TPZRouter * router);
//
//  d:
//: 
//*************************************************************************

void TPZNetwork::addRouter(TPZRouter* router) {
    unsigned x = router->getPosition().valueForCoordinate(TPZPosition::X);
    unsigned y = router->getPosition().valueForCoordinate(TPZPosition::Y);
    unsigned z = router->getPosition().valueForCoordinate(TPZPosition::Z);
    TPZRouter* tempRouter = 0;
    m_RouterMatrix -> valueAt(x, y, z, tempRouter);
    if (tempRouter) {
        TPZString text;
        text.sprintf(ERR_TPZNET_001, (char*)TPZPosition(x,y,z).asString() );
        EXIT_PROGRAM(text);
    }
    m_RouterMatrix -> setValueAt(x, y, z, router);
    router->setOwner(this);
}

//*************************************************************************
//:
//  f: TPZRouter * getRouterAt (TPZPosition const & position) const;
//
//  d:
//:
//*************************************************************************

TPZRouter* TPZNetwork::getRouterAt(const TPZPosition& position) const {
    unsigned x = position.valueForCoordinate(TPZPosition::X);
    unsigned y = position.valueForCoordinate(TPZPosition::Y);
    unsigned z = position.valueForCoordinate(TPZPosition::Z);
    return getRouterAt(x, y, z);
}

//*************************************************************************
//:
//  f: getRouterAt TPZRouter * (unsigned x, unsigned, and unsigned z) const;
//
//  d:
//:
//*************************************************************************

TPZRouter* TPZNetwork::getRouterAt(unsigned x, unsigned y, unsigned z) const {
    TPZRouter* router;
    if (m_RouterMatrix->valueAt(x, y, z, router) )
        return router;
    return 0;
}

#ifdef PTOPAZ

//*************************************************************************
//:
//  f: void setThreadID (int, int);
//
//  d:
//:
//*************************************************************************
void TPZNetwork::setThreadID(long pid, int n)
{
  m_threadsTable[n]=pid;
}

//*************************************************************************
//:
//  f: int getThreadID (int);
//
//  d:
//:
//*************************************************************************

int TPZNetwork::getThreadID(long id)
{
  for (int i=0; i<m_numThreads; i++)
      if (m_threadsTable[i]==id)
         return i;
  return 0;
}

//*************************************************************************
//:
//  f: int messageRelease (int);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::messageRelease (TPZMessage* msg)
{
    unsigned n=getThreadID(pthread_self()); 
    m_messagesToRelease[n].enqueue(msg);
}

#endif


//*************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::run(uTIME runTime) {
#ifdef PTOPAZ
      TPZMessage* toRelease;
      int rc;
      m_CurrentTime = runTime;
      if((rc = pthread_barrier_wait(&m_barrierRouters))>0)
      {
         perror("Master thread is broken at Routers Barrier");
         return;
      }
      for (register int i=0; i<m_numThreads; i++)
      {  
         m_process[i] -> clockEdge(runTime);
      }

#ifndef NO_POOL
      //Avoids mutex at shared Pool. The master thread should release Spool items.
      //when static pool is used. If Pool is disabled, each thread will deallocate
      //each TPZMessage without problems.

      for(register int i=0; i<m_messagesToRelease.size();i++)
      {
         while(m_messagesToRelease[i].numberOfElements()!=0)
         {
            m_messagesToRelease[i].dequeue(toRelease);
            ((TPZSimulation *) getSimulation())->getMessagePool(toRelease->getPoolIndex()).release(toRelease);
          }
       }
#endif
        
#else
    m_CurrentTime = runTime;
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                router->preRun(runTime);
            }
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                router->run(runTime);
            }
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                router->postRun(runTime);
            }
    TPZConnectionSet::Cursor connectionCursor(m_ConnectionSet);
    forCursor(connectionCursor) {
        connectionCursor.element()->run(runTime);
    }
    
#endif
}

//*************************************************************************
//:
//  f: virtual void setSimulation (TPZComponent * simulation);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::setSimulation(TPZComponent* simulation) {
    if ( !isInitializated() ) {
        Inhereited::setSimulation(simulation);
        return;
    }
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                router->setSimulation(simulation);
            }
    TPZRouter* router = getRouterAt(0, 0, 0);
}

//*************************************************************************
//:
//  f: virtual void setBufferSize (unsigned size);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::setBufferSize(unsigned size) {
    if (size!=0) {
        TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData(this);
        gData.routerSetBufferSize(size);

        for (register int i=0; i<m_SizeX; i++)
            for (register int j=0; j<m_SizeY; j++)
                for (register int k=0; k<m_SizeZ; k++) {
                    TPZRouter* router = getRouterAt(i, j, k);
                    router->setBufferSize(size);
                }

    }
    //In any case you know the final value of the buffers
    //then I can inject without problems.
    TPZRouter* router = getRouterAt(0, 0, 0);
    router->setPoolSize(m_SizeX*m_SizeY*m_SizeZ);
    
}

//*************************************************************************
//:
//  f: void sendMessage (const TPZMessage & msg);
//
//  d: 
//:
//*************************************************************************

Boolean TPZNetwork::sendMessage(const TPZMessage& msg) {
    return sendMessage(new TPZMessage(msg));
}

//*************************************************************************
//:
//  f: void sendMessage (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************
Boolean TPZNetwork::sendMessage(TPZMessage* msg) {
    TPZPosition source = msg->source();
    TPZPosition destin = msg->destiny();

    TPZRouter* router = getRouterAt(source);
    TPZRouter* routerDest = getRouterAt(destin);

    if (!router) {
        TPZString text;
        text.sprintf(ERR_TPZNET_002, (char*)source.asString());
        EXIT_PROGRAM(text);
    }

    if (!routerDest) {
        TPZString text;
        text.sprintf(ERR_TPZNET_002, (char*)destin.asString());
        EXIT_PROGRAM(text);
    }

    if (msg->getExternalInfo()==0) 
    {
       msg->setGenerationTime(getCurrentTime());
    }
#ifndef NO_TRAZA 
   
#ifdef PTOPAZ
   pthread_mutex_lock(&m_MessagesKey);  //To avoid multiple messages with same ID. May be avoidable but some routing uses this ids for work
#endif	

    TPZMessage::assignUniqueId(msg);
    
#ifdef PTOPAZ
   pthread_mutex_unlock(&m_MessagesKey);
#endif	

#endif

    unsigned indice= msg->getVnet();
    incrProtocolMessagesTx(unsigned(indice));

    router->sendMessage(msg);

    incrementTx(Message);
    incrementTx(Packet, msg->messageSize());
    incrementTx(Flit, msg->messageSize()*msg->packetSize());
    
    
    return true;
    
}

//*************************************************************************
//:
//  f: TPZPosition CreatePosition (int num);
//
//  d:
//:
//*************************************************************************
TPZPosition TPZNetwork::CreatePosition(int num) {
    unsigned numi=unsigned(num);

    if (m_SizeZ==1) {
        unsigned x = numi%unsigned(m_SizeX);
        unsigned y = numi/unsigned(m_SizeX);
        return TPZPosition(x, y);
    } else {
        unsigned z=numi/(m_SizeX*m_SizeY);
        unsigned zproyectada=numi-z*(m_SizeX*m_SizeY);
        unsigned x=zproyectada%m_SizeX;
        unsigned y=zproyectada/m_SizeX;

        return TPZPosition(x, y, z);
    }
}
//*************************************************************************
//:
//  f: int Number_of_nodes () const
//
//  d:
//:
//*************************************************************************
int TPZNetwork::Number_of_nodes() {
    return m_SizeX*m_SizeY*m_SizeZ;
}

//*************************************************************************
//:
//  f: void * getExternalInfoAt (unsigned x, unsigned, and unsigned z = 0);
//
//  d:
//:
//*************************************************************************

void* TPZNetwork::getExternalInfoAt(unsigned x, unsigned y, unsigned z) {
    TPZRouter* router = getRouterAt(x, y, z);
    if ( !router) {
        TPZString text;
        text.sprintf(ERR_TPZNET_003, x, y);
        EXIT_PROGRAM(text);
    }
    return router->getExternalInfo();

}
//*************************************************************************
//:
//  f: void * getExternalInfoAt (TPZPosition pos);
//
//  d:
//:
//*************************************************************************

void* TPZNetwork::getExternalInfoAt(TPZPosition pos) {

    TPZRouter* router = getRouterAt(pos);
    if ( !router) {
        TPZString text;
        EXIT_PROGRAM(text);
    }
    return router->getExternalInfo();

}
//*************************************************************************
//:
//  f: void clearExternalInfoAt (TPZPosition pos);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::clearExternalInfoAt(TPZPosition pos) {

    TPZRouter* router = getRouterAt(pos);
    if ( !router) {
        TPZString text;
        EXIT_PROGRAM(text);
    }
    router->setExternalInfo(0);

}
//*************************************************************************
//:
//  f: void initializeHistogram (int a);
//
//  d: Generates a histogram of distances
//:
//*************************************************************************
void TPZNetwork::initializeHistogram(unsigned diametro) {
    //The latter uses it to packages that are a thousand laps
    //the network by a dynamic fault ... for example.

    m_Histogram=new unsigned long[diametro+1];
    m_diametro=diametro;
    for (int i=0; i<diametro+1; i++)
        m_Histogram[i]=0;
}
//*************************************************************************
//:
//  f: void destroyHistogram ();
//
//  d: Generates a histogram of distances
//:
//*************************************************************************
void TPZNetwork::destroyHistogram() {
    delete m_Histogram;
}
//*************************************************************************
//:
//  f: TPZString printHistogram ();
//
//  d: Generates a histogram of distances
//:
//*****************************************************************
TPZString TPZNetwork::printHistogram() {
    TPZString temp;
    double suma=0;
    int i;
    TPZString percent;
    for (i=0; i<m_diametro; i++) {
        suma=m_Histogram[i]+suma;
    }

    TPZString buffer="**********************************************************\n";
    buffer=buffer+"\tHops\t Message Count \t Percent\n";
    buffer=buffer+"**********************************************************\n";
    for (i=0; i<m_diametro; i++) {
        if (m_Histogram[i]!=0) {
            temp.sprintf("\t%d \t %d        \t %1.3f\n", i, m_Histogram[i], double(m_Histogram[i]/suma*100.0));
            buffer+=temp;
        }
    }
    if (m_Histogram[m_diametro]!=0) {
        temp.sprintf(
                " Packets traveling larger distance than expected diameter  due to dinamic fault injection\t %d\n",
                m_Histogram[m_diametro]);
        buffer+=temp;
    }
    return buffer;
}
//*************************************************************************
//:
//  f: void initializeStats ();
//
//  d: Initializes STATISTICS. By passing the parameter in the sgml resetStats
//    the simulation, the statistics of clean cold start
//:
//*****************************************************************
void TPZNetwork::initializeStats(uTIME timeRestart) {
#ifdef PTOPAZ
for (int n=0; n<m_numThreads; n++)
{ 
   m_MessagesEscape[n]=0;
   m_MessagesRx[n]=0;
   m_PacketsRx[n]=0;
   m_FlitsRx[n]=0;
   m_AverageDistance[n]=0;
   m_MessageDelayTotal[n]=0;
   m_MessageDelayNetwork[n]=0;
   m_MessageDelayBuffer[n]=0;
   m_PacketDelayTotal[n]=0;
   m_PacketDelayNetwork[n]=0;
   m_PacketDelayBuffer[n]=0;
}

#else
    m_MessagesEscape=0;
    m_MessagesRx=0;
    m_PacketsRx=0;
    m_FlitsRx=0;
    m_AverageDistance=0;
    m_MessageDelayTotal=0;
    m_MessageDelayNetwork=0;
    m_MessageDelayBuffer=0;
    m_PacketDelayTotal=0;
    m_PacketDelayNetwork=0;
    m_PacketDelayBuffer=0;
#endif

    m_FlitsTx=0;
    m_PacketsTx=0;
    m_MessagesTx=0;
    m_clockRestartStats=timeRestart;

    //clean injection map
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                router->clearSentFlits();
                clearInjectedFlitsAt(i, j, k);
            }

    unsigned protocolNumber = ((TPZSimulation*)getSimulation())->getProtocolMessTypes();

}

//*************************************************************************
//:
//  f: getMessagesToTx unsigned long () const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::getMessagesToTx() const {
    unsigned long totalMssg = 0;
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                totalMssg += router->getMessagesToSend();
            }
    return totalMssg;
}

//*************************************************************************
//:
//  f: void writeComponentStatus (ostream & os) const;
//
//  d:
//:
//*************************************************************************

void TPZNetwork::writeComponentStatus(ostream& os) const {
    os << "TIME=" << getCurrentTime() << endl << endl;
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                router->writeComponentStatus(os);
            }
}

//*************************************************************************
//:
//  f: void writeBufferInformation (ostream & os);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::writeBufferInformation(ostream& os) {
    unsigned short xSize = getSizeX();
    unsigned short ySize = getSizeY();
    unsigned short zSize = getSizeZ();

    os.write((char*)&xSize, sizeof(unsigned short));
    os.write((char*)&ySize, sizeof(unsigned short));
    os.write((char*)&zSize, sizeof(unsigned short));

    TPZRouter* router = getRouterAt(0, 0, 0);
    if (router)
        router-> writeBufferInformation(os);
}

//*************************************************************************
//:
//  f: void writeBufferStatus (ostream & os);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::writeBufferStatus(ostream& os, Boolean binario) {
    if (binario) {
        os.write((char*)&m_CurrentTime, sizeof(uTIME));

        for (register int i=0; i<m_SizeX; i++)
            for (register int j=0; j<m_SizeY; j++)
                for (register int k=0; k<m_SizeZ; k++) {
                    TPZRouter* router = getRouterAt(i, j, k);
                    router->writeBufferStatus(os);
                }
    } else {

        TPZString buffer=TPZString(m_CurrentTime)+"\t\t";
        TPZString linea;
        unsigned numeroBuffers=0;
        TPZRouter* router = getRouterAt(0, 0, 0);
        //Header Color
        if (getCurrentTime()==100) {
            os<<"              **AVERAGE BUFFER OCCUPATION**"<<endl;
            os<<"TIME\t"<<router->writeBuffersName()<<endl;
        }
        linea=router->writeBufferStatus();
        linea=TPZString::stripBlanks(linea);
        while (linea!=TPZString("")) {
            linea.removeFirstWord();
            numeroBuffers++;
        }
        unsigned *contadores=new unsigned[numeroBuffers];
        for (int ixx=0; ixx< numeroBuffers; ixx++)
            contadores[ixx]=0;

        for (register int i=0; i<m_SizeX; i++)
            for (register int j=0; j<m_SizeY; j++)
                for (register int k=0; k<m_SizeZ; k++) {
                    router = getRouterAt(i, j, k);
                    linea=router->writeBufferStatus();
                    linea=TPZString::stripBlanks(linea);
                    unsigned numeroBuffer=0;
                    while (linea!=TPZString("")) {
                        contadores[numeroBuffer]=contadores[numeroBuffer]+ (linea.firstWord()).asInteger();
                        linea.removeFirstWord();
                        numeroBuffer++;
                    }
                }
        for (int ix=0; ix< numeroBuffers; ix++) {
            double average=contadores[ix]/Number_of_nodes();
            buffer+=TPZString(average)+"\t\t";
        }
        buffer+="\n";
        os<<buffer;;
        delete[] contadores;
    }
}

//*************************************************************************
//:
//  f: void onHeaderPacketReceived (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::onHeaderPacketReceived(const TPZMessage* msg) {

    //incrementMessagesDistance (msg.getDistance ());
}

//*************************************************************************
//:
//  f: void onPacketReceived (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************
void TPZNetwork::onPacketReceived(const TPZMessage* msg) {
    
    uTIME t1 = msg->generationTime();
    uTIME t2 = msg->packetInjectionTime();
    uTIME t3 = getCurrentTime();
    uTIME totalTime = t3 - t1;

    TPZPosition destiny = msg->destiny();
    //WARNING: some kind of router constructions have multiple
    //buffer traversals per router, therefore the distance values
    //are not correct in these cases
    unsigned distancia=msg->getDistance()-1;
    if (distancia !=0) //Distances are counted only if
    {
        if (distancia>m_diametro) {
	    m_Histogram[m_diametro]=m_Histogram[m_diametro]+1;
        } else {
            m_Histogram[distancia-1]=m_Histogram[distancia-1]+1;
        }
    }
    incrementRx(Packet);
    unsigned acumTemp=0;
    m_ReceivedFlitsMap->valueAt(destiny.valueForCoordinate(TPZPosition::X),
            destiny.valueForCoordinate(TPZPosition::Y),
            destiny.valueForCoordinate(TPZPosition::Z), acumTemp);
    m_ReceivedFlitsMap->setValueAt(destiny.valueForCoordinate(TPZPosition::X),
            destiny.valueForCoordinate(TPZPosition::Y),
            destiny.valueForCoordinate(TPZPosition::Z), acumTemp+(msg->packetSize()));
    incrementRx(Flit, msg->packetSize());

    incrementDelayTotal(Packet, totalTime);
    incrementDelayNetwork(Packet, t3-t2);
    incrementDelayBuffer(Packet, t2-t1);

    if (totalTime > getMaximLatency(Packet) )
        setMaximLatency(Packet, totalTime);
    
  
    if (((getPacketsRx()-m_pckRcvLast)>=m_numberMsg) || (getCurrentTime()-m_LastTime)>=m_numberCyclesToReport) {
#ifdef PTOPAZ   
        if(getThreadID(pthread_self())!=0) return;
#endif  

        if (m_LastTime==0) {
            m_buffPrint
                    = "**********************************************************\n";
            m_buffPrint+="Time\tf/c/r\tf/c\tTotLat\tNetLat\tAverDist\n";
            m_buffPrint
                    +="**********************************************************\n";

        }
        if (m_clockRestartStats==m_CurrentTime) {
            m_buffPrint+=TPZString("Reset Global Stats here\n");
        } else {
            uTIME Time_interval=m_CurrentTime-m_LastTime;

            TPZString temp;

            m_buffPrint+=TPZString(m_CurrentTime)+TPZString("\t");

            temp.sprintf("%2.3f\t", double(getFlitsRx()-m_FlitsRcvLast)/double(Time_interval*m_SizeX*m_SizeY*m_SizeZ));
            m_buffPrint+=temp;

            temp.sprintf("%5.3f\t", double(getFlitsRx()-m_FlitsRcvLast)/double(Time_interval));
            m_buffPrint+=temp;

            temp.sprintf("%6.3f\t", (getTotalDelay(TPZNetwork::Packet))/(getPacketsRx()-m_pckRcvLast));
            m_buffPrint+=temp;

            temp.sprintf("%6.3f\t", (getNetworkDelay(TPZNetwork::Packet))/(getPacketsRx()-m_pckRcvLast));
            m_buffPrint+=temp;

            temp.sprintf("%6.3f\n", double(getTotalDistance()-m_AverageDistanceold)/double(getPacketsRx()-m_pckRcvLast));
            m_buffPrint+=temp;
            
        }

        //Reset Stats
        m_FlitsRcvLast=getFlitsRx();
        m_pckRcvLast=getPacketsRx();
        m_LastTime=m_CurrentTime;
        m_LastTotalLatency=getTotalDelay(TPZNetwork::Packet);
        resetPacketDelayTotal();
        resetPacketDelayNetwork();
        m_AverageDistanceold=getTotalDistance();

    }

    
}

//*************************************************************************
//:
//  f: void onMessageReceived (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::onMessageReceived(const TPZMessage* msg) {
   
    uTIME t1 = msg->generationTime();
    uTIME t2 = msg->packetInjectionTime();
    uTIME t3 = getCurrentTime();
    uTIME totalTime = t3 - t1;

    incrementRx(Message);
    if (msg->isOnScape()==true) m_MessagesEscape++;
    
    incrProtocolMessagesRx(msg->getVnet());
    incrProtocolMessagesDelayNetwork(msg->getVnet(), t3-t2);
    incrProtocolMessagesDelayTotal(msg->getVnet(), totalTime);
    incrProtocolMessagesDelayBuffer(msg->getVnet(), t2-t1);
    setProtocolMaxMessagesLatency(msg->getVnet(), totalTime);

    incrementDelayTotal(Message, totalTime);
    incrementDelayNetwork(Message, t3-t2);
    incrementDelayBuffer(Message, t2-t1);

    unsigned distancia=msg->getDistance()-1;
    incrProtocolAverageDistance(msg->getVnet(), distancia);
    incrementMessagesDistance(distancia);
    if (totalTime > getMaximLatency(Message) )
        setMaximLatency(Message, totalTime);

}

//*************************************************************************
//:
//  f: void setMaximLatency (int type, utime time);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::setMaximLatency(int type, uTIME time) {
    if (type == TPZNetwork::Packet) {
        m_MaxPacketLatency = time;
    } else if (type == TPZNetwork::Message) {
        m_MaxMessageLatency = time;
    }
}

//*************************************************************************
//:
//  f: utime getMaximLatency (int type) const;
//
//  d:
//:
//*************************************************************************

uTIME TPZNetwork::getMaximLatency(int type) const {
    uTIME rc = 0;

    if (type == TPZNetwork::Packet) {
        rc = m_MaxPacketLatency;
    } else if (type == TPZNetwork::Message) {
        rc = m_MaxMessageLatency;
    }

    return rc;
}

//*************************************************************************
//:
//  f: void incrementDelayTotal (int type, utime time);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::incrementDelayTotal(int type, uTIME time) {
#ifdef PTOPAZ
   int n=getThreadID(pthread_self());
   if( type == TPZNetwork::Packet ) {
      m_PacketDelayTotal[n]+=double(time);
   }
   else if( type == TPZNetwork::Message ) {
      m_MessageDelayTotal[n]+=double(time);
   }
#else
    if (type == TPZNetwork::Packet) {
        m_PacketDelayTotal += double(time);
    } else if (type == TPZNetwork::Message) {
        m_MessageDelayTotal += double(time);
    }
#endif
}

//*************************************************************************
//:
//  f: void incrementDelayNetwork (int type, utime time);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::incrementDelayNetwork(int type, uTIME time) {
#ifdef PTOPAZ
   int n=getThreadID(pthread_self());
   if( type == TPZNetwork::Packet ) {
      m_PacketDelayNetwork[n]+=double(time);
   }
   else if( type == TPZNetwork::Message ) {
      m_MessageDelayNetwork[n]+=double(time);
   }
#else
    if (type == TPZNetwork::Packet) {
        m_PacketDelayNetwork += time;
    } else if (type == TPZNetwork::Message) {
        m_MessageDelayNetwork += time;
    }
#endif
}

//*************************************************************************
//:
//  f: void incrementDelayBuffer (int type, utime time);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::incrementDelayBuffer(int type, uTIME time) {
#ifdef PTOPAZ
   int n=getThreadID(pthread_self());
   if( type == TPZNetwork::Packet ) {
      m_PacketDelayBuffer[n]+=double(time);
   }
   else if( type == TPZNetwork::Message ) {
      m_MessageDelayBuffer[n]+=double(time);
   }
#else
    if (type == TPZNetwork::Packet) {
        m_PacketDelayBuffer += time;
    } else if (type == TPZNetwork::Message) {
        m_MessageDelayBuffer += time;
    }
#endif
}

//*************************************************************************
//:
//  f: void incrementRx (int type, unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::incrementRx(int type, unsigned number) {
#ifdef PTOPAZ
   int n=getThreadID(pthread_self());
   if (type == TPZNetwork::Flit) {
      m_FlitsRx[n]+=number;
    } else if (type == TPZNetwork::Packet) {
      m_PacketsRx[n]+=number;        
    } else if (type == TPZNetwork::Message) {
      m_MessagesRx[n]+=number;       
    }
#else
    if (type == TPZNetwork::Flit) {
        m_FlitsRx += number;
    } else if (type == TPZNetwork::Packet) {
        m_PacketsRx += number;
    } else if (type == TPZNetwork::Message) {
        m_MessagesRx += number;
    }
#endif
}

//*************************************************************************
//:
//  f: void incrementTx (int type, unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::incrementTx(int type, unsigned number) {
    if (type == TPZNetwork::Flit) {
        m_FlitsTx += number;
    } else if (type == TPZNetwork::Packet) {
        m_PacketsTx += number;
    } else if (type == TPZNetwork::Message) {
        m_MessagesTx += number;
    }
}

//*************************************************************************
//:
//  f: double getTotalDelay (Message type = int) const;
//
//  d:
//:
//*************************************************************************

double TPZNetwork::getTotalDelay(int type) const {
    double rc = 0.0;
    if (type == TPZNetwork::Packet) {
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_PacketDelayTotal[i];
    }
#else
        rc = m_PacketDelayTotal;
#endif
    } else if (type == TPZNetwork::Message) {
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_MessageDelayTotal[i];
    }
#else
        rc = m_MessageDelayTotal;
#endif
    }

    return rc;
}

//*************************************************************************
//:
//  f: void resetPacketDelayTotal ();
//
//  d:
//:
//*************************************************************************

void TPZNetwork::resetPacketDelayTotal()  {
#ifdef PTOPAZ
   for (int i = 0; i<m_numThreads; i++)
   {
      m_PacketDelayTotal[i]= 0.0;
   }
#else
   m_PacketDelayTotal=0;
#endif
}

//*************************************************************************
//:
//  f: void resetPacketDelayNetwork ();
//
//  d:
//:
//*************************************************************************

void TPZNetwork::resetPacketDelayNetwork()  {
#ifdef PTOPAZ
   for (int i = 0; i<m_numThreads; i++)
   {
      m_PacketDelayNetwork[i]= 0.0;
   }
#else
   m_PacketDelayNetwork=0;
#endif
}

//*************************************************************************
//:
//  f: double getNetworkDelay (Message type = int) const;
//
//  d:
//:
//*************************************************************************

double TPZNetwork::getNetworkDelay(int type) const {
    double rc = 0.0;
    if (type == TPZNetwork::Packet) {
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_PacketDelayNetwork[i];
    }
#else
        rc = m_PacketDelayNetwork;
#endif
    } else if (type == TPZNetwork::Message) {
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_MessageDelayNetwork[i];
    }
#else
        rc = m_MessageDelayNetwork;
#endif	
    }

    return rc;
}

//*************************************************************************
//:
//  f: double getBufferDelay (Message type = int) const;
//
//  d:
//:
//*************************************************************************

double TPZNetwork::getBufferDelay(int type) const {
    double rc = 0.0;
    if (type == TPZNetwork::Packet) {
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_PacketDelayBuffer[i];
    }
#else
        rc = m_PacketDelayBuffer;
#endif
    } else if (type == TPZNetwork::Message) {
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_MessageDelayBuffer[i];
    }
#else
        rc = m_MessageDelayBuffer;
#endif
    }

    return rc;
}


//*************************************************************************
//:
//  f: unsigned long getMessagesRx() const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::getMessagesRx() const {
    unsigned long rc = 0;
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_MessagesRx[i];
    }
#else
        rc = m_MessagesRx;
#endif

    return rc;
}

//*************************************************************************
//:
//  f: unsigned long getMessagesEscape() const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::getMessagesEscape() const {
    unsigned long rc = 0;
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_MessagesEscape[i];
    }
#else
        rc = m_MessagesEscape;
#endif

    return rc;
}
//*************************************************************************
//:
//  f: unsigned long getPacketsRx() const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::getPacketsRx() const {
    unsigned long rc = 0;
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_PacketsRx[i];
    }
#else
        rc = m_PacketsRx;
#endif

    return rc;
}

//*************************************************************************
//:
//  f: unsigned long getFlitsRx() const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::getFlitsRx() const {
    unsigned long rc = 0;
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_FlitsRx[i];
    }
#else
        rc = m_FlitsRx;
#endif

    return rc;
}

//*************************************************************************
//:
//  f: unsigned long getTotalDistance() const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::getTotalDistance() const {
    unsigned long rc = 0;
#ifdef PTOPAZ
    for (int i = 0; i<m_numThreads; i++)
    {
       rc+=m_AverageDistance[i];
    }
#else
        rc = m_AverageDistance;
#endif

    return rc;
}

#ifdef PTOPAZ
//*************************************************************************
//:
//  f: void incrementMessagesDistance(unsigned long dst)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::incrementMessagesDistance(unsigned long dst)
{
   int n=getThreadID(pthread_self());
   m_AverageDistance[n]+=dst;
}
#endif

//*************************************************************************
//:
//  f: void initializeConnectionsFor (const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************

void TPZNetwork::initializeConnectionsFor(const TPZPosition& pos) {

    TPZRouter* router = getRouterAt(pos);
    TPZRouter* routerXp = getRouterAt(positionOf(_Xplus_, pos) );
    TPZRouter* routerXm = getRouterAt(positionOf(_Xminus_, pos) );
    TPZRouter* routerYp = getRouterAt(positionOf(_Yplus_, pos) );
    TPZRouter* routerYm = getRouterAt(positionOf(_Yminus_, pos) );
    TPZRouter* routerZp = getRouterAt(positionOf(_Zplus_, pos) );
    TPZRouter* routerZm = getRouterAt(positionOf(_Zminus_, pos) );

    unsigned iXp = router->getInputWithType(_Xplus_);
    unsigned iXm = router->getInputWithType(_Xminus_);
    unsigned iYp = router->getInputWithType(_Yplus_);
    unsigned iYm = router->getInputWithType(_Yminus_);
    unsigned iZp = router->getInputWithType(_Zplus_);
    unsigned iZm = router->getInputWithType(_Zminus_);

    unsigned oXp = router->getInputWithType(_Xplus_);
    unsigned oXm = router->getInputWithType(_Xminus_);
    unsigned oYp = router->getInputWithType(_Yplus_);
    unsigned oYm = router->getInputWithType(_Yminus_);
    unsigned oZp = router->getInputWithType(_Zplus_);
    unsigned oZm = router->getInputWithType(_Zminus_);
    
        if (m_channelOperationMode == TPZString("HALFDUPLEX"))  
        {
           if( iXp && oXp )
           {
               
           TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oXp),
                                           routerXp->getInputInterfaz(iXp),
                                           getConnectionDelay() );
               TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oXp+4),
                                           routerXp->getInputInterfaz(iXp+4),
                                           getConnectionDelay() );
            }

            if( iXm && oXm )
            {
               
           TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oXm),
                                           routerXm->getInputInterfaz(iXm),
                                           getConnectionDelay() );
               TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oXm+4),
                                           routerXm->getInputInterfaz(iXm+4),
                                           getConnectionDelay() );
            }
            if( iYp && oYp )
            {
               
           TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oYp),
                                           routerYp->getInputInterfaz(iYp),
                                           getConnectionDelay() );
               TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oYp+4),
                                           routerYp->getInputInterfaz(iYp+4),
                                           getConnectionDelay() );
            }
            if( iYm && oYm )
            {
               
           TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oYm),
                                           routerYm->getInputInterfaz(iYm),
                                           getConnectionDelay() );
               TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oYm+4),
                                           routerYm->getInputInterfaz(iYm+4),
                                           getConnectionDelay() );
            }
            if( iZp && oZp )
            {
               cerr << "THERE IS NOT SUPPORT FOR HALFDUPLEX 3D";
            }
            if( iZm && oZm )
               TPZConnection::connectInterfaces( this,
                                           router->getOutputInterfaz(oZm),
                                           routerZm->getInputInterfaz(iZm),
                                           getConnectionDelay() );
        }
    else
    {
        if (iXp && oXp)
        TPZConnection::connectInterfaces( this, router->getOutputInterfaz(oXp),
                routerXp->getInputInterfaz(iXp), getConnectionDelay() );

       if (iXm && oXm)
        TPZConnection::connectInterfaces( this, router->getOutputInterfaz(oXm),
                routerXm->getInputInterfaz(iXm), getConnectionDelay() );

       if (iYp && oYp)
        TPZConnection::connectInterfaces( this, router->getOutputInterfaz(oYp),
                routerYp->getInputInterfaz(iYp), getConnectionDelay() );

       if (iYm && oYm)
        TPZConnection::connectInterfaces( this, router->getOutputInterfaz(oYm),
                routerYm->getInputInterfaz(iYm), getConnectionDelay() );

       if (iZp && oZp)
        TPZConnection::connectInterfaces( this, router->getOutputInterfaz(oZp),
                routerZp->getInputInterfaz(iZp), getConnectionDelay() );

       if (iZm && oZm)
        TPZConnection::connectInterfaces( this, router->getOutputInterfaz(oZm),
                routerZm->getInputInterfaz(iZm), getConnectionDelay() );
        }
}

//*************************************************************************
//:
//  f: TPZPosition positionof (TPZROUTINGTYPE dir, const TPZPosition & pos);
//
//  d:
//:
//*************************************************************************

TPZPosition TPZNetwork::positionOf(TPZROUTINGTYPE dir, const TPZPosition& pos) {

    TPZPosition rp = pos;
    switch (dir) {
    case _Xplus_:
        rp = rp + TPZPosition(1, 0, 0);
        break;

    case _Xminus_:
        rp = rp - TPZPosition(1, 0, 0);
        break;

    case _Yplus_:
        rp = rp + TPZPosition(0, 1, 0);
        break;

    case _Yminus_:
        rp = rp - TPZPosition(0, 1, 0);
        break;

    case _Zplus_:
        rp = rp + TPZPosition(0, 0, 1);
        break;

    case _Zminus_:
        rp = rp - TPZPosition(0, 0, 1);
        break;
    }

    if (rp.valueForCoordinate(TPZPosition::X) < 0)
        rp.setValueAt(TPZPosition::X, m_SizeX-1);

    if (rp.valueForCoordinate(TPZPosition::X) >= m_SizeX)
        rp.setValueAt(TPZPosition::X, 0);

    if (rp.valueForCoordinate(TPZPosition::Y) < 0)
        rp.setValueAt(TPZPosition::Y, m_SizeY-1);

    if (rp.valueForCoordinate(TPZPosition::Y) >= m_SizeY)
        rp.setValueAt(TPZPosition::Y, 0);

    if (rp.valueForCoordinate(TPZPosition::Z) < 0)
        rp.setValueAt(TPZPosition::Z, m_SizeZ-1);

    if (rp.valueForCoordinate(TPZPosition::Z) >= m_SizeZ)
        rp.setValueAt(TPZPosition::Z, 0);

    return rp;

}

//*************************************************************************
//:
//  f: lowerFlitsSent unsigned long () const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::lowerFlitsSent() const {
    unsigned long min =~0;
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                if (router->getSentFlits() < min) {
                    min=router->getSentFlits();
                }
            }
    return min;
}
//*************************************************************************
//:
//  f: upperFlitsSent unsigned long () const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::upperFlitsSent() const {
    unsigned long max =0;
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                if (router->getSentFlits() > max) {
                    max=router->getSentFlits();
                }
            }
    return max;
}
//*************************************************************************
//:
//  f: flitSentsAt unsigned long () const;
//
//  d:
//:
//*************************************************************************

unsigned long TPZNetwork::flitSentsAt(TPZPosition pos) const {
    TPZRouter* router = getRouterAt(pos);
    return router->getSentFlits();
}
//*************************************************************************
//:
//  f: TPZString printMapInjectors () const;
//
//  d:
//:
//*************************************************************************      
TPZString TPZNetwork::printMapInjectors(unsigned long clock) const {
    TPZString format;
    TPZString
            buffer=
                    "\n***************************************************************************\n";
    buffer+="Injector Sent Flits Map\n X\tY\tZ\tAverage(f/c/r)";
    buffer
            +="\n***************************************************************************\n";
    unsigned long value;
    for (register int i=0; i<m_SizeX; i++)
        for (register int j=0; j<m_SizeY; j++)
            for (register int k=0; k<m_SizeZ; k++) {
                TPZRouter* router = getRouterAt(i, j, k);
                value=router->getSentFlits();
                format.sprintf("%d\t%d\t%d\t%1.5f\t\n", i, j, k, (double)value
                        /(double)clock);
                buffer+=format;
            }
    return buffer;

}

//*************************************************************************
//:
//  f: void incrProtocolMessagesTx (unsigned index)
//
//  d:
//:
//*************************************************************************    
void TPZNetwork::incrProtocolMessagesTx(unsigned index) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesTx[n][index]++;
    
#else
    m_ProtocolMessagesTx[0][index]++;
#endif
}

//*************************************************************************
//:
//  f: getProtocolMessagesTx unsigned long (unsigned index)
//
//  d:
//:
//************************************************************************* 
unsigned long TPZNetwork::getProtocolMessagesTx(unsigned index) {
    unsigned long data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMessagesTx[i][index]++;
    }
#else
    data=m_ProtocolMessagesTx[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: void cleanProtocolMessagesTx (unsigned index)
//
//  d:
//:
//*************************************************************************    
void TPZNetwork::cleanProtocolMessagesTx(unsigned index) {
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolMessagesTx[i][index]=0;
    }
#else
    m_ProtocolMessagesTx[0][index]=0;
#endif
}

//*************************************************************************
//:
//  f: void incrProtocolMessagesRx (unsigned index)
//
//  d:
//:
//*************************************************************************    
void TPZNetwork::incrProtocolMessagesRx(unsigned index) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesRx[n][index]++;
    
#else
    m_ProtocolMessagesRx[0][index]++;
#endif

}

//*************************************************************************
//:
//  f: getProtocolMessagesRx unsigned long (unsigned index)
//
//  d:
//:
//************************************************************************* 
unsigned long TPZNetwork::getProtocolMessagesRx(unsigned index) {
    unsigned long data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMessagesRx[i][index];
    }
#else
    data=m_ProtocolMessagesRx[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: void cleanProtocolMessagesRx (unsigned index)
//
//  d:
//:
//*************************************************************************    
void TPZNetwork::cleanProtocolMessagesRx(unsigned index) {
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolMessagesRx[i][index]=0;
    }
#else
    m_ProtocolMessagesRx[0][index]=0;
#endif
}

//*************************************************************************
//:
//  f: void incrProtocolMessagesInj (unsigned index)
//
//  d:
//:
//*************************************************************************    
void TPZNetwork::incrProtocolMessagesInj(unsigned index) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesInj[n][index]++;
    
#else
    m_ProtocolMessagesInj[0][index]++;
#endif
}

//*************************************************************************
//:
//  f: void decrProtocolMessagesInj (unsigned index)
//
//  d:
//:
//*************************************************************************    
void TPZNetwork::decrProtocolMessagesInj(unsigned index) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesInj[n][index]--;
    
#else
    m_ProtocolMessagesInj[0][index]--;
#endif
}

//*************************************************************************
//:
//  f: getProtocolMessagesInj unsigned long (unsigned index)
//
//  d:
//:
//************************************************************************* 
unsigned long TPZNetwork::getProtocolMessagesInj(unsigned index) {
    unsigned long data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMessagesInj[i][index];
    }
#else
    data=m_ProtocolMessagesInj[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: void incrProtocolAverageDistance (unsigned index, unsigned long dist)
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::incrProtocolAverageDistance(unsigned index, unsigned long dist) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolAverageDistance[n][index]+=dist;
    
#else
    m_ProtocolAverageDistance[0][index]+=dist;
#endif    
}

//*************************************************************************
//:
//  f: getProtocolAverageDistance unsigned long (unsigned index)
//
//  d:
//:
//************************************************************************* 
unsigned long TPZNetwork::getProtocolAverageDistance(unsigned index) {
    unsigned long data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolAverageDistance[i][index];
    }
#else
    data=m_ProtocolAverageDistance[0][index];
#endif 
    return data;
}

//*************************************************************************
//:
//  f: void cleanProtocolAverageDistance (unsigned index, unsigned long dist)
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::cleanProtocolAverageDistance(unsigned index) {
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolAverageDistance[i][index]=0;
    }
#else
    m_ProtocolAverageDistance[0][index]=0;
#endif
}

//*************************************************************************
//:
//  f: void incrProtocolMessagesDelayNetwork (unsigned index, utime delay)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::incrProtocolMessagesDelayNetwork(unsigned index, uTIME delay) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesDelayNetwork[n][index]+=delay;
    
#else
    m_ProtocolMessagesDelayNetwork[0][index]+=delay;
#endif
}

//*************************************************************************
//:
//  f: double getProtocolMessagesDelayNetwork (unsigned index)
//
//  d:
//:
//*************************************************************************
double TPZNetwork::getProtocolMessagesDelayNetwork(unsigned index) {
    double data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMessagesDelayNetwork[i][index];
    }
#else
    data=m_ProtocolMessagesDelayNetwork[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: voidcleanProtocolMessagesDelayNetwork (unsigned index, utime delay)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::cleanProtocolMessagesDelayNetwork(unsigned index) {
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolMessagesDelayNetwork[i][index]=0;
    }
#else
    m_ProtocolMessagesDelayNetwork[0][index]=0;
#endif
}

//*************************************************************************
//:
//  f: void incrProtocolMessagesDelayTotal (unsigned index, utime delay)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::incrProtocolMessagesDelayTotal(unsigned index, uTIME delay) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesDelayTotal[n][index]+=double(delay);
    
#else
    m_ProtocolMessagesDelayTotal[0][index]+=double(delay);
#endif
}

//*************************************************************************
//:
//  f: double getProtocolMessagesDelayTotal (unsigned index)
//
//  d:
//:
//*************************************************************************
double TPZNetwork::getProtocolMessagesDelayTotal(unsigned index) {
    double data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMessagesDelayTotal[i][index];
    }
#else
    data=m_ProtocolMessagesDelayTotal[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: void cleanProtocolMessagesDelayTotal (unsigned index, utime delay)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::cleanProtocolMessagesDelayTotal(unsigned index) {
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolMessagesDelayTotal[i][index]=0;
    }
#else
    m_ProtocolMessagesDelayTotal[0][index]=0;
#endif
}

//*************************************************************************
//:
//  f: void incrProtocolMessagesDelayBuffer (unsigned index, utime delay)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::incrProtocolMessagesDelayBuffer(unsigned index, uTIME delay) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    m_ProtocolMessagesDelayBuffer[n][index]+=double(delay);
    
#else
    m_ProtocolMessagesDelayBuffer[0][index]+=double(delay);
#endif
}

//*************************************************************************
//:
//  f: double getProtocolMessagesDelayBuffer (unsigned index)
//
//  d:
//:
//*************************************************************************
double TPZNetwork::getProtocolMessagesDelayBuffer(unsigned index) {
    double data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMessagesDelayBuffer[i][index];
    }
#else
    data=m_ProtocolMessagesDelayBuffer[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: void cleanProtocolMessagesDelayBuffer (unsigned index, utime delay)
//
//  d:
//:
//*************************************************************************
void TPZNetwork::cleanProtocolMessagesDelayBuffer(unsigned index) {
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolMessagesDelayBuffer[i][index]=0;
    }
#else
    m_ProtocolMessagesDelayBuffer[0][index]=0;
#endif
}

//*************************************************************************
//:
//  f: void setProtocolMaxMessageLatency (unsigned index, utime delay)
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::setProtocolMaxMessagesLatency(unsigned index, uTIME delay) {
#ifdef PTOPAZ
    int n=getThreadID(pthread_self());
    if(m_ProtocolMaxMessagesLatency[n][index] < delay)
       m_ProtocolMaxMessagesLatency[n][index] = delay;
#else
    if(m_ProtocolMaxMessagesLatency[0][index] < delay)
       m_ProtocolMaxMessagesLatency[0][index] = delay;
#endif
}

//*************************************************************************
//:
//  f: utime getProtocolMaxMessagesLatency (unsigned index)
//
//  d:
//:
//*************************************************************************
uTIME TPZNetwork::getProtocolMaxMessagesLatency(unsigned index) {
    uTIME data=0;
#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       data+=m_ProtocolMaxMessagesLatency[i][index];
    }
#else
    data=m_ProtocolMaxMessagesLatency[0][index];
#endif
    return data;
}

//*************************************************************************
//:
//  f: void setProtocolMaxMessageLatency (unsigned index, utime delay)
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::cleanProtocolMaxMessagesLatency(unsigned index) {

#ifdef PTOPAZ
    for (int i=0; i< m_numThreads; i++)
    {
       m_ProtocolMaxMessagesLatency[i][index]=0;
    }
#else
    m_ProtocolMaxMessagesLatency[0][index]=0;
#endif
}
//*************************************************************************
//:
//  f: getInjectedFlitsAt (x, y, z)
//
//  d:
//:
//************************************************************************* 
unsigned TPZNetwork::getInjectedFlitsAt(unsigned x, unsigned y, unsigned z) {
    unsigned valueTemp;
    m_InjectedFlitsMap->valueAt(x, y, z, valueTemp);
    return valueTemp;
}

//*************************************************************************
//:
//  f: clearInjectedFlitsAt (x, y, z)
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::clearInjectedFlitsAt(unsigned x, unsigned y, unsigned z) {
    unsigned valueTemp=0;
    m_InjectedFlitsMap->setValueAt(x, y, z, valueTemp);

}
//*************************************************************************
//:
//  f: incrInjectedFlitsAt (x, y, z)
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::incrInjectedFlitsAt(unsigned x, unsigned y, unsigned z,
        unsigned number) {
    unsigned valueTemp;
    m_InjectedFlitsMap->valueAt(x, y, z, valueTemp);
    m_InjectedFlitsMap->setValueAt(x, y, z, valueTemp+number);

}

//*************************************************************************
//:
//  f: getReceivedFlitsAt (x, y, z)
//
//  d:
//:
//************************************************************************* 
unsigned TPZNetwork::getReceivedFlitsAt(unsigned x, unsigned y, unsigned z) {
    unsigned valueTemp;
    m_ReceivedFlitsMap->valueAt(x, y, z, valueTemp);
    return valueTemp;
}

//*************************************************************************
//:
//  f: getLinkUtilizationAt (x, y, z)
//
//  d:
//:
//************************************************************************* 
unsigned long TPZNetwork::getLinkUtilizationAt(unsigned x, unsigned y, unsigned z) {
    unsigned long valueTemp;
    TPZRouter* router = getRouterAt(x,y,z);
    valueTemp=router->getLinkUtilization();
    return valueTemp;
}

//*************************************************************************
//:
//  f: getLinkChangesDirectionAt (x, y, z)
//
//  d:
//:
//************************************************************************* 
unsigned long TPZNetwork::getLinkChangesDirectionAt(unsigned x, unsigned y, unsigned z) {
    unsigned long valueTemp;
    TPZRouter* router = getRouterAt(x,y,z);
    valueTemp=router->getLinkChangesDirection();
    return valueTemp;
}

//*************************************************************************


//*************************************************************************
//:
//  f: multicastPorts
//
//  d:
//:
//************************************************************************* 
TPZQueue<TPZROUTINGTYPE> TPZNetwork::multiCastPorts(
        unsigned long long maskXplus, unsigned long long maskXminus,
        unsigned long long maskYplus, unsigned long long maskYminus,
        unsigned long long maskLocalNode, unsigned long long msgMask) {
    TPZQueue<TPZROUTINGTYPE> listReturn;

    if ( (msgMask & maskYminus) !=0) {
        listReturn.enqueue(_Yminus_);
#ifndef NO_TRAZA
        TPZWRITE2LOG(TPZString(maskYminus));
        TPZWRITE2LOG("_Yminus_");
#endif
    }
    if ( (msgMask & maskYplus) !=0) {
        listReturn.enqueue(_Yplus_);
#ifndef NO_TRAZA
        TPZWRITE2LOG(TPZString(maskYplus));
        TPZWRITE2LOG("_Yplus_");
#endif
    }
    if ( (msgMask & maskXminus) !=0) {
        listReturn.enqueue(_Xminus_);
#ifndef NO_TRAZA
        TPZWRITE2LOG(TPZString(maskXminus));
        TPZWRITE2LOG("_Xminus_");
#endif
    }
    if ( (msgMask & maskXplus) !=0) {
        listReturn.enqueue(_Xplus_);
#ifndef NO_TRAZA
        TPZWRITE2LOG(TPZString(maskXplus));
        TPZWRITE2LOG("_Xplus_");
#endif
    }
    if ( (msgMask & maskLocalNode) !=0) {
        listReturn.enqueue(_LocalNode_);
#ifndef NO_TRAZA
        TPZWRITE2LOG(TPZString(maskLocalNode));
        TPZWRITE2LOG("_LocalNode_");
#endif
    }
    return listReturn;
}
//*************************************************************************
//:
//  f: void generateDORMasks()
//
//  d:
//:
//************************************************************************* 
void TPZNetwork::generateDORMasks(TPZRouter* router) 
{
    TPZPosition pos=router->getPosition();
    
    unsigned sx=getSizeX();
    unsigned sy=getSizeY();
    
    unsigned posx=pos.valueForCoordinate(TPZPosition::X);
    unsigned posy=pos.valueForCoordinate(TPZPosition::Y);
    
    unsigned numNodes=sx*sy;

    unsigned nodeNumber= posx+posy*sx;
    unsigned long long maskXplus=0;
    unsigned long long maskXminus=0;
    unsigned long long maskYplus=0;
    unsigned long long maskYminus=0;
    unsigned long long maskLocalNode=0;
    
    unsigned long long maskprimamesh=1;
    
    maskLocalNode=(maskprimamesh << nodeNumber);
    
    for (int i=posy+1; i<sy; i++) 
    {
	unsigned long long maskaux=1;
	unsigned aux= (nodeNumber + sx*(i-posy));
	maskYplus= maskYplus + (maskaux << aux);
    }

    for (int j=0; j<posy; j++) 
    {
	unsigned long long maskaux2=1;
	unsigned aux= (nodeNumber - sx*(j+1));
	maskYminus= maskYminus + (maskaux2 << aux);
    }

    for (unsigned col=posx+1; col<sx; col++) 
    {
	unsigned long long Mascara=0;
	unsigned long long Mascara_aux2=0;
	unsigned long long Mascara_aux=1;
	for (unsigned fil=0; fil<sy; fil++) 
	{
	   unsigned Position=fil*sx+(col%sx);
	   Mascara_aux2=Mascara_aux<<Position;
           maskXplus=maskXplus|Mascara_aux2;
	}
    }

    for (unsigned col2=0; col2<posx; col2++) 
    {
	unsigned long long Mascara=0;
	unsigned long long Mascara_aux2=0;
	unsigned long long Mascara_aux=1;
	for (unsigned fil2=0; fil2<sy; fil2++) 
	{
	   unsigned Position=fil2*sx+(col2%sx);
	   Mascara_aux2=Mascara_aux<<Position;
	   maskXminus=maskXminus|Mascara_aux2;
	}
    }
         
    router->setMask(_Xplus_ , maskXplus);
    router->setMask(_Xminus_ , maskXminus);
    router->setMask(_Yplus_ , maskYplus);
    router->setMask(_Yminus_ , maskYminus);
    router->setMask(_LocalNode_ , maskLocalNode);
        
}

//*************************************************************************
//:
//  f: void incrEventCount (TPZTipoEvento event);
//
//  d:
//:
//************************************************************************* 
void TPZNetwork :: incrEventCount( TPZTipoEvento evento)
{
   switch (evento)
   {
      case BufferWrite:
         m_BufferWrite++;
     return;
      case BufferRead:
         m_BufferRead++;
     return;
      case VCArbitration:
         m_VCArbitration++;
     return;
      case SWArbitration:
         m_SWArbitration++;
     return;
      case SWTraversal:
         m_SWTraversal++;
     return;
      case LinkTraversal:
         m_LinkTraversal++;
     return;
      case RouterBypass:
         m_RouterBypass++; 
     return;
     case IStageTraversal:
         m_IStageTraversal++; 
     return;
     case OStageTraversal:
         m_OStageTraversal++; 
     return;
     case MPTraversal:
         m_MPTraversal++; 
     return;
   }
}

//*************************************************************************
//:
//  f: void incrEventCount (TPZTipoEvento event);
//
//  d:
//:
//************************************************************************* 
double TPZNetwork :: getEventCount( TPZTipoEvento evento)
{
   switch (evento)
   {
      case BufferWrite:
         return m_BufferWrite;
     
      case BufferRead:
         return m_BufferRead;
     
      case VCArbitration:
         return m_VCArbitration;
     
      case SWArbitration:
         return m_SWArbitration;
     
      case SWTraversal:
         return m_SWTraversal;
     
      case LinkTraversal:
         return m_LinkTraversal;
      
      case RouterBypass:
         return m_RouterBypass;
      
     case IStageTraversal:
         return m_IStageTraversal;
	 
      case OStageTraversal:
         return m_OStageTraversal;
      
      case MPTraversal:
         return m_MPTraversal;
   }
}
//*************************************************************************


// end of file
