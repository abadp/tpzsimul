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
//    File: TPZThread.cpp
//
//    Class:  TPZThread
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#include <TPZThread.hpp>

#ifndef __TPZSimulator_HPP__
#include <TPZSimulator.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZBaseCollection_H__
#include <TPZBaseCollection.hpp>
#endif

#ifndef __TPZTrafficPattern_H__
#include <TPZTrafficPattern.hpp>
#endif

#include <cmath>

//*************************************************************************


TPZThread::TPZThread():
           m_MyNet(0),
           m_ThreadId(0),
	   m_SizeX(0),
           m_SizeY(0),
           m_SizeZ(0),
           m_kill(false),
           m_runTime (0)
 {}

TPZThread::~TPZThread()
{
   int rc;

   pthread_join(m_thread, NULL);
   pthread_cond_destroy(&m_exitFlag);
   pthread_mutex_destroy(&m_exitKey);
   

   
}


//*************************************************************************
//:
//  f: int Start (int id, unsigned x, unsigned, and unsigned z)
//  d:
//:
//*************************************************************************

int TPZThread::Start(int id, unsigned x, unsigned y, unsigned z, TPZNetwork* pthis, int numThreads)
{ 
   int rc;
   m_MyNet = pthis;
   m_ThreadId=id;
   m_SizeX=x;
   m_SizeY=y;
   m_SizeZ=z;
   m_num=numThreads;

   pthread_mutex_init(&m_exitKey, NULL);
   pthread_cond_init(&m_exitFlag, NULL);

   pthread_attr_t tattr;
   pthread_attr_init(&tattr);
   pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_JOINABLE);

   return pthread_create(&m_thread, &tattr, EntryPoint, this);     
}

//*************************************************************************
//:
//  f: void * EntryPoint (void * pthis)
//  d: input function, which must be static to be generic to all
//     class. Since we do not belong to the object, you must receive as
//     parameter the pointer to it, so that we can execute its members
//:
//*************************************************************************

void * TPZThread::EntryPoint(void * pthis)
{
   TPZThread * pt = (TPZThread*)pthis;
   pt -> ThreadFunction();
}

void TPZThread::clockEdge(uTIME time)
{
   pthread_mutex_lock(&m_exitKey);
   m_runTime = time; 
   pthread_cond_signal(&m_exitFlag); 
   pthread_mutex_unlock(&m_exitKey);
}

void TPZThread::terminate()           
{
   pthread_mutex_lock(&m_exitKey);
   m_kill = true;
   pthread_cond_signal(&m_exitFlag); 
   pthread_mutex_unlock(&m_exitKey);
}

//*************************************************************************
//:
//  f: void ThreadFunction ();
//
//  d: Function main thread running the routers run
//:
//*************************************************************************

void TPZThread :: ThreadFunction()
{
#ifdef PTOPAZ
   int jump;
   int max_XYZ, min_XYZ;
   int rc;
   register unsigned int i;
   
   //Distribute routers among threads per XY dimension. Lower unbalance in 
   //with awkward number of threads. Any change in this requieres to modify 
   //router->thread assigment in TPZNetwork::postinitialize

   jump = (unsigned int)std::ceil(1.0*m_SizeX*m_SizeY*m_SizeZ/m_num); // number of routers per thread
   max_XYZ = (m_ThreadId+1)*jump-1; // last row to evaluate
   min_XYZ = m_ThreadId*jump; 
   if (m_ThreadId == m_num-1)
   {
      max_XYZ = m_SizeX*m_SizeY*m_SizeZ-1;
   }
  
   m_MyNet->setThreadID(pthread_self(), m_ThreadId);
   double loadProb = ((TPZSimulation*)m_MyNet->getSimulation())->getLoad();

   pthread_barrier_t* barrierConnections=m_MyNet->getBarrierConnections();
   pthread_barrier_t* barrierRouters=m_MyNet->getBarrierRouters();
   pthread_mutex_lock(&m_exitKey);
   while(1)
   {
       //Cant be done in the same interation
       for(i=min_XYZ; i<=max_XYZ; i++ )
            {  
               int Z=i/(m_SizeX*m_SizeY);
               int Y=(i-Z*m_SizeX*m_SizeY)/m_SizeX;
               int X=(i-Z*m_SizeX*m_SizeY)%m_SizeX;
               TPZRouter* router = m_MyNet -> getRouterAt(X,Y,Z);
               router->preRun(m_runTime);
            }
       for(i=min_XYZ; i<=max_XYZ; i++ )
            {  
               int Z=i/(m_SizeX*m_SizeY);
               int Y=(i-Z*m_SizeX*m_SizeY)/m_SizeX;
               int X=(i-Z*m_SizeX*m_SizeY)%m_SizeX;
               TPZRouter* router = m_MyNet -> getRouterAt(X,Y,Z);
               router->run(m_runTime);
            }    
       for(i=min_XYZ; i<=max_XYZ; i++ )
            {  
               int Z=i/(m_SizeX*m_SizeY);
               int Y=(i-Z*m_SizeX*m_SizeY)/m_SizeX;
               int X=(i-Z*m_SizeX*m_SizeY)%m_SizeX;
               TPZRouter* router = m_MyNet -> getRouterAt(X,Y,Z);
               router->postRun(m_runTime);
            }      

      //Only slaves are synchorized before to run the connections
      if((rc = pthread_barrier_wait(barrierConnections))>0) 
      {
          perror("Connection Barrier broken at slaves");
          pthread_exit(NULL);
      }

      TPZConnectionSet::Cursor connectionCursor(m_ConnectionThread);
      forCursor(connectionCursor)
      {
         connectionCursor.element()->run(m_runTime);
      }  

      //Slaves and master thread (at TPZNetwork::run) are synchr. at the begining
      //of the thread.
      if((rc = pthread_barrier_wait(barrierRouters))>0) 
      {
          perror("Router Barrier broken at slaves");
          pthread_exit(NULL);
      }

      pthread_cond_wait(&m_exitFlag, &m_exitKey);
      
      //Generate Messages for myself. If GEMS this does nothing becasue traffic is always EMPTY
      ((TPZSimulation*)m_MyNet->getSimulation())->getTrafficPattern()->generateMessagesXsegment(loadProb, min_XYZ, max_XYZ);
        
      if(m_kill) pthread_exit(NULL);
   }
    

#endif
}
      

//*************************************************************************
// end of file
