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
//    File: TPZCrossbarFlow.cpp
//
//    Class:  TPZCrossbarFlow
//
//    Inherited from:  TPZFlow
// :
//*************************************************************************
//end of header



#include <TPZCrossbarFlow.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
#endif

#ifndef __TPZCrossbar_HPP__
#include <TPZCrossbar.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZCrossbarFlow,TPZFlow);

//*************************************************************************
//:
//  f: TPZCrossbarFlow ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlow :: TPZCrossbarFlow( TPZComponent& component)
                 : TPZFlow(component),
                   m_PortAsignamentTable(0),
                   m_OutputStateTable(0),
                   m_MessageReceivedTable(0),
                   m_InputStateTable(0),
                   m_OutputLastUseTable(0),
                   m_CleanInterfaces(true)
{
   enableEvent(_CrossbarRequest_);
   enableEvent(_CrossbarPass_);
}


//*************************************************************************
//:
//  f: ~ TPZCrossbarFlow ();
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlow :: ~TPZCrossbarFlow()
{

   delete[] m_RequestQueue;   
   delete m_PortAsignamentTable;
   delete m_OutputStateTable;
   delete m_MessageReceivedTable;
   delete m_InputStateTable;
   delete m_OutputLastUseTable;

}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: initialize()
{
   Inhereited :: initialize();

   TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();
   m_PortAsignamentTable = new TPZCrossbarTable(crossbar.numberOfInputs());
   m_OutputStateTable    = new TPZArray<unsigned>(MAXIMO(1,crossbar.getOutputMux())*
                                                  crossbar.numberOfOutputs() );
   
   m_MessageReceivedTable= new TPZMessageTable(crossbar.numberOfInputs());
   m_InputStateTable     = new TPZStateTable(crossbar.numberOfInputs());
   m_OutputLastUseTable  = new TPZArray<unsigned>(crossbar.numberOfOutputs());
   m_RequestQueue        = new TPZRequestQueue[crossbar.numberOfOutputs()];

   // Tables are initialized to free values.
   
   m_PortAsignamentTable->initialize(uPAIR(0,0));
   m_OutputStateTable->initialize(0);
   m_MessageReceivedTable->initialize(NULL);
   m_OutputLastUseTable->initialize(0);
   m_InputStateTable->initialize(Free);   
}


//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: terminate()
{

}


//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: inputReading()
{
   return true;   
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: stateChange()
{
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean outputWriting ();
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: outputWriting()
{
   uTIME timeStamp;
   TPZSequence<TPZEvent> tempList;
   cleanOutputInterfaces();
   cleanInputInterfaces();
     
   if( getEventQueue().firstElementTimeStamp(timeStamp) )
   {
      while( timeStamp <= getOwnerRouter().getCurrentTime() )
      {
         TPZEvent event;
         getEventQueue().dequeue(event);
         if( ! dispatchEvent(event) )
         {
            tempList.addAsLast(event);
         }
         if( !getEventQueue().firstElementTimeStamp(timeStamp) ) break;
      }
   }
   
   TPZCursor<TPZEvent> cursor(tempList);
   forCursor(cursor)
      getEventQueue().enqueue(cursor.element());
   
   tempList.removeAllElements();
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean propagateStop ();
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: propagateStop()
{
    return true;
}


//*************************************************************************
//:
//  f: Boolean addConnectionRequest (unsigned src, dst uPAIR)
//
//  d: in the output corresponding to "dst.left ()" a pair of data
//     are enqueued, being "src" the requestor input and
//     "dst.right ()" the output channel requested;
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: addConnectionRequest(unsigned src, uPAIR dst, uTIME time)
{
   TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();

   if( (dst.left() > 0) && (dst.left() <= crossbar.numberOfOutputs()) )
   {
      // Ports from 1 to N.
      m_RequestQueue[dst.left()-1].enqueue(uPAIR(src,dst.right()));
      return true;
   #ifndef NO_TRAZA 
      TPZString texto = getComponent().asString() + ". Conexion Request ";
      texto += TPZString(dst.left())+ " => " + TPZString(dst.right());
      TPZWRITE2LOG(texto);
   #endif
   }
   #ifndef NO_TRAZA 
      TPZString texto2 = getComponent().asString() + ". Conexion Request Out of Bounds";
      texto2 += TPZString(dst.left())+ " => " + TPZString(dst.right());
      TPZWRITE2LOG(texto2);
   #endif
   return false;
}


//*************************************************************************
//:
//  f: getConnectionRequestFor unsigned (unsigned port);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlow :: getConnectionRequestFor(unsigned port)
{
   TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();

   if( (port > 0) && (port <= crossbar.numberOfOutputs()) )
   {
      uPAIR rc(0,0);
      if( m_RequestQueue[port-1].dequeue(rc) )
         return rc.left();
   }
   return 0;
}

//*************************************************************************
//:
//  f: getConnectionRequestFor unsigned (unsigned port);
//
//  d:
//:
//*************************************************************************

uPAIR TPZCrossbarFlow :: getConnectionRequestForWith(unsigned port)
{
   TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();

   if( (port > 0) && (port <= crossbar.numberOfOutputs()) )
   {
      uPAIR rc(0,0);
      if( m_RequestQueue[port-1].dequeue(rc) )
         return rc;
   }
   return uPAIR(0,0);
}

//*************************************************************************
//:
//  f: cleanConnectionRequestFor unsigned (unsigned port, const uPAIR & pair);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: cleanConnectionRequestFor( unsigned port,
                                                   const uPAIR& pair )
{
   TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();

   if( (port > 0) && (port <= crossbar.numberOfOutputs()) )
   {
      m_RequestQueue[port-1].removeAllElements();
   }
}


//*************************************************************************
//:
//  f: uPAIR getOutputForInput (unsigned input) const;
//
//  d:
//:
//*************************************************************************

uPAIR TPZCrossbarFlow :: getOutputForInput(unsigned input) const
{
   uPAIR result(0,0);
   m_PortAsignamentTable->valueAt(input,result);
      
   return result;
}

//*************************************************************************
//:
//  f: getInputForOutput unsigned (unsigned outputPort,
//                                             unsigned channel) const
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlow ::  getInputForOutput(unsigned outputPort, unsigned channel ) const
{
   unsigned state=0;
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   
   m_OutputStateTable->valueAt(MAXIMO(1,channel)+(outputPort-1)*MAXIMO(1,mux),state);
  
   return state;
}


//*************************************************************************
//:
//  f: virtual Boolean isOutputPortFree (unsigned outputPort,
//                                       unsigned channel = 0) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: isOutputPortFree( unsigned outputPort, 
                                             unsigned channel ) const
{
   unsigned state=0;
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   
   m_OutputStateTable->valueAt(MAXIMO(1,channel)+(outputPort-1)*MAXIMO(1,mux),state);
  
   return (state == 0) ? true : false;
}


//*************************************************************************
//:
//  f: void establishConnection (unsigned input,
//                               unsigned output,
//                               channnel unsigned = 0);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: establishConnection( unsigned input, 
                                             unsigned output,
                                             unsigned channel )
{
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   m_PortAsignamentTable->setValueAt(input,uPAIR(output,channel));
   m_OutputStateTable->setValueAt(MAXIMO(1,channel)+(output-1)*MAXIMO(1,mux),input);
   
   #ifndef NO_TRAZA 
      TPZString texto = getComponent().asString() + ". Connection ";
      texto += TPZString(input) + " => " + TPZString(output) + "," + 
               TPZString(channel);
      TPZWRITE2LOG(texto);
   #endif
}


//*************************************************************************
//:
//  f: void clearConnection (unsigned input);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: clearConnection(unsigned input)
{
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();
   uPAIR out(0,0);
   m_PortAsignamentTable->valueAt(input,out);
   m_PortAsignamentTable->setValueAt(input,uPAIR(0,0));
   m_OutputStateTable->setValueAt(MAXIMO(1,out.right())+(out.left()-1)*MAXIMO(1,mux),0);

   setStateForInput(input,Free);
   inputInterfaz(input)->clearStop();
   
   #ifndef NO_TRAZA
      TPZString text = getComponent().asString() + " CLEAR: " + 
                       TPZString(input) + " -> " + TPZString(out.left());
      TPZWRITE2LOG(text);
   #endif
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlow :: extractOutputPortNumber(TPZMessage* msg)
{
   return extractOutputPortNumber( msg->getRoutingPort(), 
                                   msg->getChannel() );
}


//*************************************************************************
//:
//  f: unsigned extractOutputPortNumber (TPZROUTINGTYPE dir, unsigned channel);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlow :: extractOutputPortNumber( TPZROUTINGTYPE dir,
                                                     unsigned channel )
{
   unsigned oPort = ((TPZCrossbar&)getComponent()).getOutputWith( dir, channel );
   
   if( !oPort )
   {
      TPZString err;
      err.sprintf( ERR_TPZCFLOW_001,
                   (char*)getComponent().asString(),
                   (char*)_routingType2string(dir),
                   channel );
      EXIT_PROGRAM(err);
   }
   
   return oPort;
}

//*************************************************************************
//:
//  f: unsigned extractOutputChannel (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlow :: extractOutputChannel(TPZMessage* msg) const
{
   return msg->getChannel();
}

//*************************************************************************
//:
//  f: void setStateForInput (unsigned i, TPZCrossbarState state);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: setStateForInput(unsigned i, TPZCrossbarState state)
{
   m_InputStateTable->setValueAt(i,state);
}


//*************************************************************************
//:
//  f: TPZCrossbarState getStateForInput (unsigned i);
//
//  d:
//:
//*************************************************************************

TPZCrossbarFlow::TPZCrossbarState TPZCrossbarFlow :: 
                                  getStateForInput(unsigned i) const
{
   TPZCrossbarState state = Free;
   m_InputStateTable->valueAt(i,state);
   return state;
}


//*************************************************************************
//:
//  f: virtual Boolean dispatchEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: dispatchEvent(const TPZEvent& event)
{

   if( ( event.type() == _CrossbarRequest_ ) ||
       ( event.type() == _CrossbarPass_ ) )
   {
      
      if( event.type() == _CrossbarRequest_)
      {
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWArbitration);
      }
      TPZMessage* msg;
      unsigned iPort = event.source();
      unsigned oPort = getOutputForInput(iPort).left();
      if( ! iPort )
      {
         TPZString err;
         err.sprintf(ERR_TPZCFLOW_002,(char*)getComponent().asString());
         EXIT_PROGRAM(err);
      }
      else if(!oPort)
      {
         TPZString err;
         m_MessageReceivedTable->valueAt(iPort,&msg);

         // If I've sent the message, step around
         if(!msg) return true;

         cerr << "Message=" << msg->asString() << " Input=" << iPort << endl;
         err.sprintf(ERR_TPZCFLOW_003,(char*)getComponent().asString());
         EXIT_PROGRAM(err);
      }
      
      if( outputInterfaz(oPort)->isStopActive() )
      {
         uTIME delayTime = getOwnerRouter().getCurrentTime() + 1;
         getEventQueue().enqueue(event,delayTime); 
         return true;
      }

      setStateForInput(iPort,Connect);      
      m_MessageReceivedTable->valueAt(iPort,&msg);

      // If I've sent the message, step around
      if(!msg) return true;

      inputInterfaz(iPort)->clearStopRightNow();
      updateMessageInfo(msg,oPort,1);
      outputInterfaz(oPort)->sendData(msg);
      ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::SWTraversal);
      if ( ((TPZCrossbar&)getComponent()).getTypeForOutput(oPort,1) != _LocalNode_ )
      {
         getOwnerRouter().incrLinkUtilization();
         ((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::LinkTraversal);
      }
            
      #ifndef NO_TRAZA
         TPZString time = getOwnerRouter().getCurrentTime();       
         TPZString texto = getComponent().asString() + " Flit Tx. TIME = ";
         texto += time + msg->asString() + " i=" + TPZString(iPort) +
                  ", o=" + TPZString(oPort);
         TPZWRITE2LOG( texto );
      #endif
      
      if( msg->isTail() || msg->isHeadTail() )
      {
       
         clearConnection(iPort);  
         unsigned iPortNew = getConnectionRequestFor(oPort);         
       #ifndef NO_TRAZA
         TPZString time = getOwnerRouter().getCurrentTime();       
         TPZString texto = getComponent().asString() + " Process pending request TIME = ";
         texto += time + " iPort=" + TPZString(iPortNew);
         TPZWRITE2LOG( texto );
      #endif
         if( iPortNew != 0 )
         {
            establishConnection(iPortNew,oPort);
            TPZEvent requestEvent(_CrossbarRequest_,iPortNew,oPort);
            uTIME delayTime = getOwnerRouter().getCurrentTime() +
                              MAXIMO(1,getHeaderDelay());
            
            if( ((TPZCrossbar&)getComponent()).isLocalNodeOutput(oPort) )
            {
               if( getHeaderDelay() > 1 )  // The delay must be at least 1 cycle
                  delayTime -= 1;
            }
            getEventQueue().enqueue(requestEvent,delayTime);
         
         }
      }
      // I have sent the message so I can remove it from the table of messages.
      m_MessageReceivedTable->setValueAt(iPort,(TPZMessage *)0);

   }
   else
   {
      Inhereited::dispatchEvent(event);
   }
   return true;
}


//*************************************************************************
//:
//  f: void cleanOutputInterfaces ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: cleanOutputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i,j;
      TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();
   
      forInterfaz(i,crossbar.numberOfOutputs())
      {
         forInterfaz(j,outputInterfaz(i)->numberOfCV() )
            outputInterfaz(i)->clearData(j);
      }
   }
}


//*************************************************************************
//:
//  f: void cleanInputInterfaces ();
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: cleanInputInterfaces()
{
   if( cleanInterfaces() )
   {
      unsigned i;
      TPZCrossbar& crossbar = (TPZCrossbar&)getComponent();
   
      forInterfaz(i,crossbar.numberOfInputs())
      {
         if( getStateForInput(i) == TPZCrossbarFlow::Free )
            inputInterfaz(i)->clearStop();
      }
   }
}


//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: controlAlgoritm(Boolean info, int delta)
{
   // To prevent it from being an abstract class.
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: onReadyUp(unsigned interfaz, unsigned cv)
{
   unsigned i = interfaz;
   TPZMessage* msg;
   TPZMessage* lastMessage;
   
   inputInterfaz(i)->getData(&msg);
   m_MessageReceivedTable->valueAt(i,&lastMessage);

   if(lastMessage)
      if( *msg == *lastMessage )
         return true;

   m_MessageReceivedTable->setValueAt(i,msg);
   
   uTIME delayTime = getOwnerRouter().getCurrentTime() ;

    #ifndef NO_TRAZA
            TPZString texto = getComponent().asString() + " Flit Rx. TIME = ";
            texto += TPZString(delayTime) + " # " + msg->asString();
            TPZWRITE2LOG( texto );
   #endif
   if( getOutputForInput(i).left() == 0 )
   {
      // This from the head.
      unsigned outPort = extractOutputPortNumber(msg);
      unsigned outChannel = extractOutputChannel(msg);
                 
      TPZEvent requestEvent(_CrossbarRequest_,i,outPort,outChannel);
            
      setStateForInput(i,Waiting);

      if( isOutputPortFree(outPort) )
      {
         // You must assign an output
         
         delayTime += getHeaderDelay();
         
         if( ((TPZCrossbar&)getComponent()).isLocalNodeOutput(outPort) )
         {
            if( getHeaderDelay() > 1 )
               delayTime -= 1;
         }
         
         establishConnection(i,outPort);
         getEventQueue().enqueue(requestEvent,delayTime);
      }
      else
      {
         // The output port is busy. A request is queued
         // waiting for the port to be free.
               
          #ifndef NO_TRAZA
               TPZString texto = getComponent().asString() + " Waiting for an OUTPUT !";
               texto += TPZString(delayTime) + " iport= " + TPZString(i);
               TPZWRITE2LOG( texto );
         #endif
                        
         addConnectionRequest(i,uPAIR(outPort,outChannel));
      }

       #ifndef NO_TRAZA
          TPZString time = getOwnerRouter().getCurrentTime();       
         TPZString texto = getComponent().getName() + " Stop Tx. TIME = ";
         texto += time + " iport= " + TPZString(i);
         TPZWRITE2LOG( texto );
      #endif
            
      inputInterfaz(i)->sendStopRightNow();
   }
   else
   {
      // Message body is passing. Therefore, there is a
      // connection already assigned.
      delayTime += getDataDelay();
      TPZEvent passEvent(_CrossbarPass_,i);
      getEventQueue().enqueue(passEvent,delayTime);
   }
      
   return true;      
  
}

      
//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: onStopUp(unsigned interfaz, unsigned cv)
{
   unsigned iPort=0;
   
   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();   
   m_OutputStateTable->valueAt( MAXIMO(1,cv)+(interfaz-1)*MAXIMO(1,mux), iPort );
   
   #ifndef NO_TRAZA
      TPZString texto = getComponent().asString() + " Stop Rx. iport=";
      texto += TPZString(iPort) + " oport=" + TPZString(interfaz);
      TPZWRITE2LOG( texto );
   #endif

   if( iPort && propagateStop() )
   {
      #ifndef NO_TRAZA
         TPZString texto = getComponent().asString() + " Stop Tx. oport=";
         texto += TPZString(interfaz) + " iport=" + TPZString(iPort);
         TPZWRITE2LOG( texto );
      #endif

      inputInterfaz(iPort)->sendStopRightNow();
   }
   setStopped(true);
   
   return true;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: onStopDown(unsigned interfaz, unsigned cv)
{
   unsigned iPort=0;

   unsigned mux = ((TPZCrossbar&)(getComponent())).getOutputMux();   
   m_OutputStateTable->valueAt( MAXIMO(1,cv)+(interfaz-1)*MAXIMO(1,mux), iPort );

   
   if( iPort && (getStateForInput(iPort) == Connect) )
   {
      #ifndef NO_TRAZA
         TPZCrossbar& crossbar = (TPZCrossbar&)(getComponent());
         TPZString texto = getComponent().asString() + " Stop Down. O=";
         texto += TPZString(interfaz) + "," + TPZString(cv) + 
                  "=>" + TPZString(iPort);
         for(int i=1; i<=crossbar.numberOfInputs(); i++)
         {
            uPAIR temp;
            m_PortAsignamentTable->valueAt(i,temp);
            if( temp.left() != 0 )
               texto += TPZString("; ") + TPZString(i) + "=>" + 
                        TPZString(temp.left()) + "," + TPZString(temp.right());
         }
         TPZWRITE2LOG(texto);
      #endif

      inputInterfaz(iPort)->clearStopRightNow();
      setStopped(false);
   }
   
   return true;
}


//*************************************************************************
//:
//  f: getLastInputForOutput unsigned (unsigned output) const;
//
//  d:
//:
//*************************************************************************

unsigned TPZCrossbarFlow :: getLastInputForOutput(unsigned output) const
{
   unsigned rc = 0;
   m_OutputLastUseTable->valueAt(output,rc);
   return rc;
}


//*************************************************************************
//:
//  f: void setLastInputForOutput (unsigned input, unsigned output);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: setLastInputForOutput(unsigned input,unsigned output)
{
   m_OutputLastUseTable->setValueAt(output,input);
}


//*************************************************************************
//:
//  f: Boolean isAnyRequestForOutput (unsigned output);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: isAnyRequestForOutput(unsigned output)
{
   // Ports from 1 to N.
   return m_RequestQueue[output-1].numberOfElements();

}


//*************************************************************************
//:
//  f: uPAIR checkMultipleRequestFor (unsigned output);
//
//  d: Returns a pair of numbers, representing the connection. "Left ()" gives
//     index of the input port, and "right ()" the associated channel;
//:
//*************************************************************************
 
uPAIR TPZCrossbarFlow :: checkMultipleRequestFor(unsigned output)
{ 
   #ifndef NO_TRAZA
      TPZString texto = getComponent().asString()  + 
                        " Request number (out=" +
                        TPZString(output) + ") : ";
      texto += TPZString(m_RequestQueue[output-1].numberOfElements());
      TPZWRITE2LOG(texto);
   #endif
   
   uPAIR rc(0,0);
      
   if( m_RequestQueue[output-1].numberOfElements() == 0 )
   {
      TPZString err;
      err.sprintf((char*)getComponent().asString());
      EXIT_PROGRAM(err);
   }
   
   unsigned lastInput = getLastInputForOutput(output);
   
   TPZRequestQueue::Cursor cursor(m_RequestQueue[output-1]);

   forCursor(cursor)
   {
      uPAIR temp = cursor.element();
      if( rc == uPAIR(0,0) ) rc = temp;
      if( (temp.left()!=lastInput) && (rc.left()==lastInput)) rc = temp; 
   }
   return rc;
}


//*************************************************************************
//:
//  f: void removeAllRequestEventsForInput (unsigned IPORT)
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: removeAllRequestEventsForInput(unsigned iPort)
{
   Boolean eventRemove;
   do
   {
      eventRemove = false;
      TEventQueue::Cursor cursor(getEventQueue());
      forCursor(cursor)
      {
         TPZEvent event(cursor.element().element());
         if( (event.type() == _CrossbarRequest_) &&
             (event.source() == iPort) )
         {
            #ifndef NO_TRAZA
               TPZString text = getComponent().asString() + " Removing ie: " +
                                TPZString(iPort) + "=>" + 
                                TPZString(event.destiny()) + "," +
                                TPZString(event.channel());
               TPZWRITE2LOG(text);
            #endif
            cursor.remove();
            eventRemove = true;
            break;
         }
      }
   }
   while(eventRemove);
}



//*************************************************************************
//:
//  f: void removeAllRequestEventsForOutput (unsigned port);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: removeAllRequestEventsForOutput(unsigned oPort)
{
   Boolean eventRemove;
   do
   {
      eventRemove = false;
      TEventQueue::Cursor cursor(getEventQueue());
      forCursor(cursor)
      {
         TPZEvent event(cursor.element().element());
         if( (event.type() == _CrossbarRequest_) &&
             (event.destiny() == oPort) )
         {
            #ifndef NO_TRAZA
               TPZString text = getComponent().asString() + " Removing oe: " +
                                TPZString(event.source()) + "=>" +
                                TPZString(oPort) + "," +
                                TPZString(event.channel());
               TPZWRITE2LOG(text);
            #endif
            cursor.remove();
            eventRemove = true;
            break;
         }
      }
   }
   while(eventRemove);
}

//*************************************************************************
//:
//  f: Boolean updateMessageInfo (TPZmessage & msg,
//                                unsigned port,
//                                unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: updateMessageInfo( TPZMessage* msg, 
                                              unsigned oPort,
                                              unsigned channel )
{
   // by default, does nothing on the header flit.
   return true;
}


//*************************************************************************
//:
//  f: void enableEvent (int event);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: enableEvent(int event)
{
   switch(event)
   {
      case _CrossbarRequest_: 
              m_EventsActive[0] = true;
              return;      

      case _CrossbarPass_:
              m_EventsActive[1] = true;
              return;      
   }

   TPZString err;
   err.sprintf( ERR_TPZCFLOW_005,
                (char*)getComponent().asString(),
                event );
   EXIT_PROGRAM(err);
}


//*************************************************************************
//:
//  f: void disableEvent (int event);
//
//  d:
//:
//*************************************************************************

void TPZCrossbarFlow :: disableEvent(int event)
{
   switch(event)
   {
      case _CrossbarRequest_: 
              m_EventsActive[0] = false;
              return;      

      case _CrossbarPass_:
              m_EventsActive[1] = false;
              return;      
   }

   TPZString err;
   err.sprintf( ERR_TPZCFLOW_005,
                (char*)getComponent().asString(),
                event );
   EXIT_PROGRAM(err);
}


//*************************************************************************
//:
//  f: Boolean isEventEnable (int event);
//
//  d:
//:
//*************************************************************************

Boolean TPZCrossbarFlow :: isEventEnable(int event)
{
   switch(event)
   {
      case _CrossbarRequest_: 
              return m_EventsActive[0];

      case _CrossbarPass_:
              return m_EventsActive[1];     
   }
   
   return false;
}


//*************************************************************************
//:
//  f: TPZString getStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZCrossbarFlow :: getStatus() const
{
   TPZCrossbar& crb = (TPZCrossbar&)getComponent();
   TPZString rs = crb.asString() + TPZString(":\tIn stop= ");

   int i, channel;
   for( i=1; i<=crb.numberOfInputs(); i++ )
   {
      if( inputInterfaz(i)->isStopActive() )
         rs+= TPZString("I") + TPZString(i) + " ";
   }
   rs += TPZString(".\tOut stop= ");
   for( i=1; i<=crb.numberOfOutputs(); i++ )
   {
      if( crb.isLocalNodeOutput(i) )
      {
         if( outputInterfaz(i)->isStopActive() )
            rs+= TPZString("O") + TPZString(i) + " ";
      }

      else for(channel=1; channel<=crb.getOutputMux(); channel++ )
      {
         if( outputInterfaz(i)->isStopActive(channel) )
            rs+= TPZString("O") + TPZString(i) + TPZString(channel) + " ";
      }
   }

   return rs;
}

//*************************************************************************


// end of file
