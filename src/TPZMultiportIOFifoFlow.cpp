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
//    File: TPZMultiportIOFifoFlow.cpp
//
//    Class:  TPZMultiportIOFifoFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZMultiportIOFifoFlow.hpp>

#ifndef __TPZMultiportIOFifo_HPP__
#include <TPZMultiportIOFifo.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZMultiportIOFifoFlow,TPZFlow);

//*************************************************************************
//:
//  f: inline unsigned bufferSize () const;
//  d:
//
//
//  f: inline unsigned bufferHoles () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZMultiportIOFifoFlow (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZMultiportIOFifoFlow :: TPZMultiportIOFifoFlow(TPZComponent& component)
                   : TPZFlow(component),
                     m_Size(0),
                     m_inputs(1),
                     m_outputs(1),
                     m_SendStop(false),
                     m_memory(0),
                     m_payload(0),
                     m_lpack(0),
            m_wideOfMemory(0),
            m_flitsStored(0)
                     
{
   
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZMultiportIOFifoFlow :: initialize()
{
   Inhereited::initialize();
   m_Size = ((TPZMultiportIOFifo&)getComponent()).getBufferSize(); 
   m_inputs= ((TPZMultiportIOFifo&)getComponent()).numberOfInputs();
   m_outputs= ((TPZMultiportIOFifo&)getComponent()).numberOfOutputs();   

   m_wideOfMemory= ((TPZMultiportIOFifo&)getComponent()).getWideOfMemory();   


   m_memory=new QueueFlits[m_wideOfMemory];

   m_InputIndex = new unsigned[m_inputs]; 
        //for each entry stores the queue in which to store the next flit
   int i;
   for( i=1;i<=m_inputs;i++)
   {
      m_InputIndex[i-1]=0;
   }

    m_payload= new unsigned[m_outputs];
   for(i=1;i<=m_outputs;i++)
   {
      m_payload[i-1]=0;
   }
   m_OutputIndex = new unsigned[m_outputs]; 
        //for each output keeps the queue in which will bring the next flit
   for(i=1;i<=m_outputs;i++)
   {
      m_OutputIndex[i-1]=0;
   }

   m_flitsStored=0;
           

}

TPZMultiportIOFifoFlow :: ~TPZMultiportIOFifoFlow()
{
   delete[] m_memory;
   delete[] m_InputIndex;
}




void TPZMultiportIOFifoFlow :: postInitialize()
{

 }

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d: Reading from multiple input ports
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: inputReading()
{ 
for(int i=1;i<m_inputs+1;i++)
    //here is to sequence the packet writing in different queues
   //rather than put everything in it (as to the MultiportFIfo)
{ 
   if( inputInterfaz(i)->isReadyActive() )
   {
      if( ! bufferHoles() )
      {
         TPZString err;
         err.sprintf(ERR_TPZFFLOW_001,(char*)getComponent().asString());
         err+="\n Superada la capacidad de la memoria";
         EXIT_PROGRAM(err);
      }
      
      TPZMessage* msg;
      inputInterfaz(i)->getData(&msg);    
      m_memory[m_InputIndex[i-1]].enqueue(msg);
      m_flitsStored++;
      
      #ifndef NO_TRAZA
         uTIME delayTime = getOwnerRouter().getCurrentTime() ;
         TPZString texto = getComponent().asString() + " Flit Rx (" +
                           TPZString(delayTime) + " # " + msg->asString() +
                           ") holes= " + TPZString(bufferHoles());
         TPZWRITE2LOG( texto );
         
         TPZWRITE2LOG(getComponent().asString() + " Enqueue:" + 
                 TPZString(m_InputIndex[i-1])+ " -> " + 
                 TPZString(m_memory[m_InputIndex[i-1]].numberOfElements()));
      #endif
           
        m_InputIndex[i-1]= (m_InputIndex[i-1]++)%(m_wideOfMemory); 
            //m_InputsIndex [i-1] contain tail
            //where you keep the next flit
    }
}

   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: stateChange()
{
   // The virtual method "controlAlgoritm ()" is overloaded in classes
   // daughters. thus, there is a behavior or CT according WH
   // class to which belongs the object we have instantiated.

   if( controlAlgoritm(false,m_inputs)==false ) m_SendStop=true;
   else m_SendStop=false;

   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: outputWriting()
{
   TPZMessage* flitnow;
if (m_flitsStored==0) return true;   
 for(int o=1;o<m_outputs+1;o++)
 {
      outputInterfaz(o)->clearData();
      
    if( ! outputInterfaz(o)->isStopActive() )
    {
        if(m_payload[o-1]==1)
         { //I keep getting message data
            if(m_memory[m_OutputIndex[o-1]].numberOfElements()!=0 )
            {
               m_memory[m_OutputIndex[o-1]].dequeue(flitnow);
                             #ifndef NO_TRAZA
               TPZWRITE2LOG(getComponent().asString() + " Dequeue:" + 
                       TPZString(m_OutputIndex[o-1])+ " -> " + 
                       TPZString(m_memory[m_OutputIndex[o-1]].numberOfElements()));
               #endif
                m_OutputIndex[o-1]++;
                if(flitnow->isTail())
                   {
                    //Last flit of the packet.
                    m_payload[o-1]=0;
                    m_OutputIndex[o-1]=0;
                   }
             sendFlit(flitnow,o);
             }
         }
        else
        { //Looking for a new package to exit.
        if(m_memory[0].numberOfElements()!=0)
            {
            m_payload[o-1]=1;
            m_memory[0].dequeue(flitnow);
            m_OutputIndex[o-1]=1; // Like + +

            #ifndef NO_TRAZA
              TPZWRITE2LOG(getComponent().asString() + " Dequeue:" + TPZString(0)+ 
                    " -> " + TPZString(m_memory[0].numberOfElements()));            
            #endif
            sendFlit(flitnow,o);
            } //No more packages to go
        }
    }//else (if! isStopactive)
 }// to the next exit


 if(!controlAlgoritm(false,m_inputs+1)) //the delta step the number of entries
 {   //controlalgoritm will be overloaded in child classes to indicate buffer full
     for(int index=1;index<m_inputs+1;index++)
     {
        inputInterfaz(index)->sendStop();   //here is a problem
                        //in cuthrought where to put full stops data
                        //the FIFO and is for the package that is entering
                        //we'll see how we solve
            //the easiest way to the previous component will keep on posting data
            //(Although this stop)
            //once you have entered your head
    }
    #ifndef NO_TRAZA
    uTIME time = getOwnerRouter().getCurrentTime();
    TPZString texto = getComponent().asString() + ": Buffer lleno->Stop ";
    texto += TPZString(time);
    TPZWRITE2LOG(texto);
   #endif
 }
 else //if (! outputInterfaz () -> isStopActive ())
 { 
    for(int index=1;index<m_inputs+1;index++)
    {
        inputInterfaz(index)->clearStop(); 
    }
    #ifndef NO_TRAZA
         uTIME time = getOwnerRouter().getCurrentTime();
         TPZString texto = getComponent().asString() + ": Buffer libre->StopDown ";
         texto += TPZString(time);
         TPZWRITE2LOG(texto);
   #endif
 }

 return true;   
}



//*************************************************************************
//:
//  f: virtual Boolean propagateStop ();
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: propagateStop()
{
   //if (controlAlgoritm ())
   //{
      // In this case, the buffer has space for more flits,
      // whatever it is the flow control policy, since it has been
      // overloaded method "controlAlgoritm ()" accordingly.
   //   setStopped (false);
   //   return false;
   //}
   //setStopped (true);
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
 return true;
//   Inhereited return: onStopUp (interface, cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZMultiportIOFifoFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
  // if (controlAlgoritm ())
  // {
  //    There are places in the buffer, so you can spread the signal
  //    stop without any danger of overflowing the buffer.
  //    Inhereited return: onStopDown (interface, cv);
  //}
   
   // In this case, there is no buffer space, so it is stopped.
   // When you can will store some flit, and engaged himself down
   // Stop signal.
   
   return true;
}


//*************************************************************************
//:
//  f: virtual void sendFlit (const TPZMessage & msg, unigned ointerfaz)
//
//  d:
//:
//*************************************************************************

void TPZMultiportIOFifoFlow :: sendFlit(TPZMessage* msg, unsigned ointerfaz)
{
    outputInterfaz(ointerfaz)->sendData(msg);
    m_flitsStored--;

    #ifndef NO_TRAZA
        uTIME delayTime = getOwnerRouter().getCurrentTime() ;
        TPZString texto = getComponent().asString() + " Flit Tx (" +
            TPZString(delayTime) + " # " + msg->asString() +
            ") holes= " + TPZString(bufferHoles());
        TPZWRITE2LOG( texto );
    #endif
}

//*************************************************************************
//:
//  f: virtual Boolean bubbleReady () const;
//  d: semifull control (used in bubble's fifo)
//:
//*************************************************************************

 Boolean TPZMultiportIOFifoFlow ::bubbleReady()const
 {     
    return true;
 }   
//*************************************************************************
unsigned TPZMultiportIOFifoFlow::bufferElements() const
{ 

       return m_flitsStored;
       /*
       unsigned kaka=0;
    for(int index=0; index<m_wideOfMemory;index++)
    {
        kaka=m_memory[index].numberOfElements()+kaka;
    }
    return kaka;/* */
}
// end of file
