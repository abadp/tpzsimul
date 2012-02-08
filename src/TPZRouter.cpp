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
//    File: TPZRouter.cpp
//
//    Class:  TPZRouter
//
//    Inherited from:  TPZRunnableComponent
// :
//*************************************************************************
//end of header

#include <TPZRouter.hpp>

#ifndef __TPZConnection_HPP__
#include <TPZConnection.hpp>
#endif

#ifndef __TPZPortInput_HPP__
#include <TPZPortInput.hpp>
#endif

#ifndef __TPZPortOutput_HPP__
#include <TPZPortOutput.hpp>
#endif

#ifndef __TPZInjector_HPP__
#include <TPZInjector.hpp>
#endif

#ifndef __TPZConsumer_HPP__
#include <TPZConsumer.hpp>
#endif

#ifndef __TPZCrossbar_HPP__
#include <TPZCrossbar.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZRouterBuilder_HPP__
#include <TPZRouterBuilder.hpp>
#endif

#ifndef __TPZFifoMemory_HPP__
#include <TPZFifoMemory.hpp>
#endif

#ifndef __TPZMultiportFifo_HPP__
#include <TPZMultiportFifo.hpp>
#endif

#ifndef __TPZMultiportIOFifo_HPP__
#include <TPZMultiportIOFifo.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZConst_HPP__
#include <TPZConst.hpp>
#endif

#include <string.h>

//*************************************************************************

// Run time information
IMPLEMENT_RTTI_DERIVED(TPZRouter, TPZRunnableComponent);

//*************************************************************************
//:
//  f: inline unsigned getNumberOfBuffers () const;
//  d:
//
//  f: inline getCurrentTime utime () const;
//  d:
//
//  f: inline TPZPosition & getPosition () const;
//  d:
//
//  f: inline void setPosition (const TPZPosition & position);
//  d:
//
//  f: inline void setInjector (TPZComponent * component);
//  d:
//
//  f: inline void setConsumer (TPZComponent * component);
//  d:
//
//  f: inline getInjector TPZComponent * () const;
//  d:
//
//  f: inline getConsumer TPZComponent * () const;
//  d:
//
//  f: inline unsigned valueForBufferSize ();
//  d:
//
//  f: inline TPZString valueForBufferControl ();
//  d:
//
//  f: inline TPZString valueForRoutingControl ();
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZRouter (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZRouter::TPZRouter(const TPZComponentId& id) :
    TPZRunnableComponent(id), m_CurrentTime(0), m_Position(0),
            m_InjectorRoundRobin(0), m_InjectionType(INJECT_ROUND_ROBIN),
            m_ExtraPoolSize(0), m_numberOfHosts(1),
            m_ContadorXplus(0), m_ContadorXminus(0), m_ContadorYplus(0),
            m_ContadorYminus(0), m_ContadorLocal(0), m_ContadorTotal(0),
            m_ProtocolMessages(0), m_InOrderTableXplus(0), m_InOrderTableXminus(0),
            m_InOrderTableYplus(0), m_InOrderTableYminus(0),
            m_InOrderTableLocalNode(0), m_maskXplus(0), m_maskXminus(0),
            m_maskYplus(0), m_maskYminus(0), 
	    m_maskZplus(0), m_maskZminus(0), m_maskLocalNode(0),
            m_ContadorLinkX(0), m_ContadorLinkY(0),
            m_StateLinkXplus(false), m_StateLinkXminus(false), m_StateLinkYplus(false),
            m_StateLinkYminus(false), m_StateLinkLocalNorm(false), m_StateLinkLocalRev(false),
            m_LinkUtilization(0), m_LinkChangesDirection(0){	    
#ifdef PTOPAZ 	    
    pthread_mutex_init(&m_routerKey, NULL);
#endif
}

//*************************************************************************
//:
//  f: virtual ~ TPZRouter ();
//
//  d:
//:
//*************************************************************************

TPZRouter::~TPZRouter() {
    delete m_ProtocolMessages;
    delete m_InOrderTableXplus;
    delete m_InOrderTableXminus;
    delete m_InOrderTableYplus;
    delete m_InOrderTableYminus;
    delete m_InOrderTableLocalNode;
    TPZComponentSet::Cursor cursor(m_ComponentSet);
    forCursor(cursor) {
        cursor.element()->setRouter(0);
    }

    TPZMessageList::Cursor cursor2(m_MessageList);
    forCursor(cursor2) {
        delete cursor2.element();
    }

    m_BufferList.removeAllElements();
    m_InjectorList.removeAllElements();
    m_ConsumerList.removeAllElements();
    m_MessageList.removeAllElements();
}

void TPZRouter::terminate() {
    TPZComponentSet::Cursor cursor(m_ComponentSet);
    forCursor(cursor) {
        cursor.element()->terminate();
    }
#ifdef PTOPAZ 	    
   pthread_mutex_unlock(&m_routerKey);
   pthread_mutex_destroy(&m_routerKey);
#endif
}

//*************************************************************************
//:
//  f: unsigned valueForBufferSize ();
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::valueForBufferSize() {
    TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)getOwner());
    return gData.routerBufferSize();
}

//*************************************************************************
//:
//  f: TPZString valueForBufferControl ();
//
//  d:
//:
//*************************************************************************

TPZString TPZRouter::valueForBufferControl() {
    TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)getOwner());
    return gData.routerBufferControl();
}

//*************************************************************************
//:
//  f: TPZString valueForRoutingControl ();
//
//  d:
//:
//*************************************************************************

TPZString TPZRouter::valueForRoutingControl() {
    TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)getOwner());
    return gData.routerRoutingControl();
}

//*************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//*************************************************************************

void TPZRouter::run(uTIME runTime) {
    TPZComponentSet::Cursor componentCursor(m_ComponentSet);
    TPZConnectionSet::Cursor connectionCursor(m_ConnectionSet);
    m_CurrentTime = runTime;
    
    forCursor(componentCursor) {
        componentCursor.element()->run(runTime);
    }

    forCursor(connectionCursor) {
        connectionCursor.element()->run(runTime);
    }

}

void TPZRouter::preRun(uTIME runTime) {
    TPZComponentSet::Cursor preRunCursor(m_PreRunComponentSet);
    
    m_CurrentTime = runTime;
    
    forCursor(preRunCursor) {
        preRunCursor.element()->preRun(runTime);
    }

    
}

void TPZRouter::postRun(uTIME runTime) {
    TPZComponentSet::Cursor postRunCursor(m_PostRunComponentSet);
    
    m_CurrentTime = runTime;
    
    forCursor(postRunCursor) {
        postRunCursor.element()->postRun(runTime);
    }

    
}

//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZRouter::initialize() {
    if ( !isInitializated() ) {
        m_InOrderTableXplus = new PAFInOrderTable(5);
        m_InOrderTableXplus->initialize(0);
        m_InOrderTableXminus = new PAFInOrderTable(5);
        m_InOrderTableXminus->initialize(0);
        m_InOrderTableYplus = new PAFInOrderTable(5);
        m_InOrderTableYplus->initialize(0);
        m_InOrderTableYminus = new PAFInOrderTable(5);
        m_InOrderTableYminus->initialize(0);
        m_InOrderTableLocalNode = new PAFInOrderTable(5);
        m_InOrderTableLocalNode->initialize(0);
        TPZComponentSet::Cursor componentCursor(m_ComponentSet);
        forCursor(componentCursor) {
            TPZComponent* current = componentCursor.element();
            current->setRouter(this);
            current->setOwner(this);
            current->initialize();

        }
    }
    setInitializated(true);
    
}

//*************************************************************************
//:
//  f: virtual void postInitialize ();
//
//  d: This method is invoked after initializing the router, is
//     need any parts need some other
//     and the previously initialized;
//:
//*************************************************************************

void TPZRouter::postInitialize() {

    unsigned protocolNumber = ((TPZSimulation*)getSimulation())->getProtocolMessTypes();
    m_ProtocolMessages = new PAFProtocolMessages(protocolNumber);
    m_ProtocolMessages->initialize(0);
    
    TPZComponentSet::Cursor cursor(m_ComponentSet);
    forCursor(cursor) {
        cursor.element()->postInitialize();
    }
}

//*************************************************************************
//:
//  f: virtual void sendMessage (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZRouter::sendMessage(TPZMessage* msg) {
    TPZInjector* injector = chooseInjector(msg->getVnet(), msg->source(),
            msg->destiny());
    //If it returns 0 it is a router without injectors
    //Designed for indirect and irregular networks.
    //Obviously direct networks have at least one host
    //connected 

    if (injector) {
        injector->sendMessage(msg);
    }
}

//*************************************************************************
//:
//  f: void onHeaderPacketReceived (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZRouter::onHeaderPacketReceived(const TPZMessage* msg) {

    TPZNetwork* net = (TPZNetwork*)getOwner();
    
    net->onHeaderPacketReceived(msg);
    
}

//*************************************************************************
//:
//  f: void onPacketReceived (const TPZMessage & msg);
//
//  d:
//:
//*************************************************************************

void TPZRouter::onPacketReceived(TPZMessage* msg) {
    
    setExternalInfo(msg->getExternalInfo());
    TPZNetwork* net = (TPZNetwork*)getOwner();
    net->onPacketReceived(msg);

    if (msg->messageSize() == 1) {
        net->onMessageReceived(msg);
    } else {

        TPZMessageList::Cursor cursor(m_MessageList);
        TPZMessage* ptrMessage = 0;
        Boolean found = false;
        forCursor(cursor) {
            ptrMessage = cursor.element();
            if (ptrMessage->getIdentifier() == msg->getIdentifier() ) {
                found = true;
                break;
            }
        }

        if ( !found) {
            // It is the first message packet has been received.
            msg->setPacketNumber(1);
            m_MessageList.add(msg);
            return;
        } else {
            // 'PtrMessage' points to a message on the list received.
            if (msg->packetNumber() == 1) {
                ptrMessage->setPacketInjectionTime(msg->packetInjectionTime() );
            }
            unsigned newPacketNumber = ptrMessage->packetNumber() + 1;
            ptrMessage->setPacketNumber(newPacketNumber);
            if (newPacketNumber == ptrMessage->messageSize() ) {
                net->onMessageReceived(ptrMessage);
                delete ptrMessage;
                cursor.remove();
            }
        }
    }
}

//*************************************************************************
//:
//  f: virtual unsigned getBufferOccupation (TPZROUTINGTYPE type,
//                                           unsigned channel = 1);
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::getBufferOccupation(TPZROUTINGTYPE type, unsigned channel) {
    TPZComponentId id(TPZComponentId::FifoMemory, type, "*", channel);
    TPZComponent* component = componentWithIdLike(id);
    TPZFifoMemory* fifo = POINTERCAST(TPZFifoMemory, component);
    if (fifo) {
        return (fifo->getBufferSize() - fifo->bufferHoles()+1);
    }
    return 0;
}

//*************************************************************************
//:
//  f: virtual void buildFlowControl ();
//
//  d:
//:
//*************************************************************************

void TPZRouter::buildFlowControl() {
    // To prevent it from being an abstract class.
}

//*************************************************************************
//:
//  f: void* getExternalInfo ();
//
//  d:
//:
//*************************************************************************

void* TPZRouter::getExternalInfo() {
#ifdef PTOPAZ
   pthread_mutex_lock(&m_routerKey);
#endif
   void* rc;
   if(!m_ExternalInfo.firstElement(rc))
   {
      rc=0;
   }
   
   if(rc!=0)
   {
     m_ExternalInfo.dequeue(rc);
   }
#ifdef PTOPAZ
   pthread_mutex_unlock(&m_routerKey);
#endif
   return rc;
}

//*************************************************************************
//:
//  f: void setExternalInfo (void* ptr);
//
//  d:
//:
//*************************************************************************

void TPZRouter::setExternalInfo(void* ptr) {
#ifdef PTOPAZ
   pthread_mutex_lock(&m_routerKey);
#endif
   m_ExternalInfo.enqueue(ptr);
#ifdef PTOPAZ
   pthread_mutex_unlock(&m_routerKey);
#endif
}
	
//*************************************************************************
//:
//  f: virtual void buildInputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZRouter::buildInputInterfaz(unsigned number) {
    // The default action is not worth
}

//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZRouter::buildOutputInterfaz(unsigned number) {
    // The default action is worthless.
}

//*************************************************************************
//:
//  f: void writeComponentStatus (ostream & os);
//
//  d:
//:
//*************************************************************************

void TPZRouter::writeComponentStatus(ostream& os) {
    TPZComponent* component = componentWithName("CROSSBAR");
    if (component)
        os << component->getStatus() << endl;
    
}

//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRouter::asString() const {
    TPZSimulation* simul = (TPZSimulation*)getSimulation();
    TPZString texto = "Router: id=";
    texto += getName();
    texto += TPZString(" routing=") + simul->globalData((TPZNetwork*)getOwner()).routerRoutingControl();
    texto += " buffer=";
    texto += TPZString(simul->globalData((TPZNetwork*)getOwner()).routerBufferSize()) + " (";
    texto += simul->globalData((TPZNetwork*)getOwner()).routerBufferControl() + ")";
    return texto;
}

//*************************************************************************
//:
//  f: virtual TPZInjector * chooseInjector ();
//
//  d:
//:
//*************************************************************************

TPZInjector* TPZRouter::chooseInjector(unsigned vNetwork, TPZPosition& src,
        TPZPosition& dst) {
    TPZComponent* rc = 0;
    unsigned personalNumber=getNumberOfHosts();

    if (personalNumber==0)
        return 0;

    unsigned injectorNumber = m_InjectorList.numberOfElements();
    if (personalNumber>injectorNumber) {
        TPZString
                err="The number of hosts for this routers is greater than the number defined in the SGML";
        EXIT_PROGRAM(err);
    }

    //Only irregular networks can do this
    if (injectorNumber == 1)
        injectorNumber=personalNumber;

    if (injectorNumber == 1) {
        m_InjectorList.elementAt(0, rc);
    } else {
        if (m_InjectionType == INJECT_RANDOM) {
            unsigned index = rand() % injectorNumber;
            m_InjectorList.elementAt(index, rc);
        } else if (m_InjectionType == INJECT_ROUND_ROBIN) {
            m_InjectorList.elementAt(m_InjectorRoundRobin, rc);
            if ( (++m_InjectorRoundRobin) >= injectorNumber)
                m_InjectorRoundRobin = 0;
        } else if (m_InjectionType == INJECT_VNET) {
	    m_InjectorList.elementAt(m_InjectorList.numberOfElements()-vNetwork, rc);
        } else if (m_InjectionType == INJECT_VC) {
            unsigned index = rand() % injectorNumber;
            m_InjectorList.elementAt(index, rc);
        } 
    }

    return (TPZInjector*)rc;
}

//*************************************************************************
//:
//  f: void writeBufferInformation (ostream & os);
//
//  d:
//:
//*************************************************************************

void TPZRouter::writeBufferInformation(ostream& os) {
    TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)getOwner());
    unsigned short numeroBuffers = m_BufferList.numberOfElements();
    unsigned short sizeBuffers = gData.routerBufferSize();

    os.write((char*)&numeroBuffers, sizeof(unsigned short));
    os.write((char*)&sizeBuffers, sizeof(unsigned short));

    TPZBufferList::Cursor cursor(m_BufferList);
    forCursor(cursor) {
        char nameBuffer[32];
        unsigned short bufferSize = cursor.element()->getBufferSize();
        for (register int j=0; j<32; j++)
            nameBuffer[j]='\0';
        strcpy(nameBuffer, cursor.element()->asString());
        os.write(nameBuffer, 32);
        os.write((char*)&bufferSize, sizeof(unsigned short));
    }
}

//*************************************************************************
//:
//  f: void writeBufferStatus (ostream & os);
//
//  d:
//:
//*************************************************************************

void TPZRouter::writeBufferStatus(ostream& os) {
    unsigned short x = getPosition().valueForCoordinate(TPZPosition::X);
    unsigned short y = getPosition().valueForCoordinate(TPZPosition::Y);
    unsigned short z = getPosition().valueForCoordinate(TPZPosition::Z);

    os.write((char*)&x, sizeof(unsigned short));
    os.write((char*)&y, sizeof(unsigned short));
    os.write((char*)&z, sizeof(unsigned short));
    TPZBufferList::Cursor cursor(m_BufferList);
    forCursor(cursor) {
        unsigned short occupation = cursor.element()->bufferOccupation();
        os.write((char*)&occupation, sizeof(unsigned short));
    }
}

//*************************************************************************
//:
//  f: TPZString writeBufferStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRouter::writeBufferStatus() const {
    TPZString ocupa=TPZString("");

    TPZBufferList::Cursor cursor(m_BufferList);
    forCursor(cursor) {
        unsigned short occupation = cursor.element()->bufferOccupation();
        ocupa+=TPZString(" ")+TPZString(occupation);
    }
    return ocupa;
}
//*************************************************************************
//:
//  f: TPZString writeBufferStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRouter::writeBuffersName() const {
    TPZString idString=TPZString("");

    TPZBufferList::Cursor cursor(m_BufferList);
    forCursor(cursor) {
        idString+=cursor.element()->asString()+TPZString("\t");
    }
    return idString;
}

//*************************************************************************
//:
//  f: void addBuffer (TPZFifoMemory * fifo);
//
//  d:
//:
//*************************************************************************

void TPZRouter::addBuffer(TPZFifoMemory* fifo) {
    if (fifo)
        m_BufferList.add(fifo);
}

//*************************************************************************
//:
//  f: void addMultiportBuffer (TPZFifoMemory * fifo);
//
//  d:
//:
//*************************************************************************

void TPZRouter::addMultiportBuffer(TPZMultiportFifo* mport) {
    if (mport)
        m_MultiportBufferList.add(mport);
}

//*************************************************************************
//:
//  f: void addMultiportIOBuffer (TPZFifoMemory * fifo);
//
//  d:
//:
//*************************************************************************

void TPZRouter::addMultiportIOBuffer(TPZMultiportIOFifo* mportio) {
    if (mportio)
        m_MultiportIOBufferList.add(mportio);
}

//*************************************************************************
//:
//  f: void setBufferSize (unsigned size);
//
//  d:
//:
//*************************************************************************

void TPZRouter::setBufferSize(unsigned size) {
    TPZBufferList::Cursor cursor(m_BufferList);
    forCursor(cursor) {
        cursor.element()->setBufferSize(size);
    }
}
unsigned TPZRouter::getTotalBuffeSize() const {
    unsigned sizeTotal=0;
    TPZBufferList::Cursor cursor(m_BufferList);
    forCursor(cursor) {
        sizeTotal+=cursor.element()->getBufferSize();
    }
    TPZMultiportBufferList::Cursor cursor2(m_MultiportBufferList);
    forCursor(cursor2) {
        sizeTotal+=cursor2.element()->getBufferSize();
    }
    TPZMultiportIOBufferList::Cursor cursor3(m_MultiportIOBufferList);
    forCursor(cursor3) {
        sizeTotal+=cursor3.element()->getBufferSize();
    }
    return sizeTotal+m_ExtraPoolSize;
}
//*************************************************************************
//:
//  f: void setPoolSize (unsigned size);
//
//  d:
//:
//*************************************************************************

void TPZRouter::setPoolSize(unsigned size) {

    TPZInjectorList::Cursor cursor(m_InjectorList);
    forCursor(cursor) {
        TPZInjector* inj = (TPZInjector*)(cursor.element());
        inj->getFlowControl()->initializePool((unsigned int)(size*getTotalBuffeSize()));
        return;
    }
}

//*************************************************************************
//:
//  f: void addInjector (TPZComponent * injector);
//
//  d:
//:
//*************************************************************************

void TPZRouter::addInjector(TPZComponent* injector) {
    if (injector)
        m_InjectorList.add(injector);
}

//*************************************************************************
//:
//  f: void addConsumer (TPZComponent * consumer);
//
//  d:
//:
//*************************************************************************

void TPZRouter::addConsumer(TPZComponent* consumer) {
    if (consumer)
        m_ConsumerList.add(consumer);
}

//*************************************************************************
//:
//  f: getMessagesToSend unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::getMessagesToSend() const {
    unsigned messages = 0;
    TPZInjectorList::Cursor cursor(m_InjectorList);
    forCursor(cursor) {
        TPZInjector* inj = (TPZInjector*)(cursor.element());
        messages += inj->getMessagesToSend();
    }
    return messages;
}

//*************************************************************************
//:
//  f: getMessagesToSend unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::getSentFlits() const {
    unsigned messages = 0;
    TPZInjectorList::Cursor cursor(m_InjectorList);
    forCursor(cursor) {
        TPZInjector* inj = (TPZInjector*)(cursor.element());
        messages += inj->getSentFlits();
    }
    return messages;
}

//*************************************************************************
//:
//  f: getMessagesToSend unsigned () const;
//
//  d:
//:
//*************************************************************************

void TPZRouter::clearSentFlits() {
    unsigned messages = 0;
    TPZInjectorList::Cursor cursor(m_InjectorList);
    forCursor(cursor) {
        TPZInjector* inj = (TPZInjector*)(cursor.element());
        inj->clearSentFlits();
    }
}

//*************************************************************************
//:
//  f: unsigned getInputWithType (TPZROUTINGTYPE type);
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::getInputWithType(TPZROUTINGTYPE type) {
    TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)getOwner());

    for (int i=1; i<=gData.routerInputsType()->numberOfElements(); i++) {
        TPZROUTINGTYPE tempType;
        if (gData.routerInputsType()->valueAt(i, tempType) )
            if (tempType==type)
                return i;
    }
    return 0;
}

//*************************************************************************
//:
//  f: unsigned getOutputWithType (TPZROUTINGTYPE type);
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::getOutputWithType(TPZROUTINGTYPE type) {
    TPZGlobalData& gData = ((TPZSimulation*)getSimulation())->globalData((TPZNetwork*)getOwner());

    for (int i=1; i<=gData.routerOutputsType()->numberOfElements(); i++) {
        TPZROUTINGTYPE tempType;

        if (gData.routerOutputsType()->valueAt(i, tempType) )
            if (tempType==type)
                return i;
    }
    return 0;
}

//*************************************************************************
//:
//  f: static TPZRouter * newFrom (TPZTag const * tag,
//                                TPZComponent * owner,
//                                INDEX i)
//
//  d:
//:
//*************************************************************************

TPZRouter* TPZRouter::newFrom(const TPZTag* tag, TPZComponent* owner,
        INDEX& index) {
    TPZComponentId idRouter(TPZComponentId::Router, _LocalNode_, tag->tagId());
    TPZString bufferSize, bufferControl, injectorControl, routingControl, inputs, outputs, 
            numberOfVirtualNetworks;

    tag->getAttributeValueWithName(TPZ_TAG_BUFFER_SIZE, bufferSize);
    tag->getAttributeValueWithName(TPZ_TAG_BUFFER_CONTROL, bufferControl);
    tag->getAttributeValueWithName(TPZ_TAG_ROUTING_CONTROL, routingControl);
    tag->getAttributeValueWithName(TPZ_TAG_INPUTS, inputs);
    tag->getAttributeValueWithName(TPZ_TAG_OUTPUTS, outputs);
    tag->getAttributeValueWithName(TPZ_TAG_INJECT_CONTROL, injectorControl);
    tag->getAttributeValueWithName(TPZ_TAG_VIRTUALNETWORKS, numberOfVirtualNetworks);

    if ( (bufferSize == TPZString("")) || (bufferControl == TPZString(""))
            || (routingControl == TPZString("")) || (inputs == TPZString("") )
            || (outputs == TPZString("") )) {
        TPZString err;
        err.sprintf(ERR_TPZROUTE_001, (char*)(tag->tagId()));
        EXIT_PROGRAM(err);
    }

    TPZRouter* router = new TPZRouter(idRouter);
    router->setSimulation(owner->getSimulation());
    router->setOwner(owner);
    TPZGlobalData& gData = ((TPZSimulation*)(owner->getSimulation()))->globalData((TPZNetwork*)owner);

    if ( !gData.routerIsInitializated() ) {
            
        gData.routerSetBufferSize(bufferSize.asInteger());
        gData.routerSetBufferControl(bufferControl);
                
        gData.routerSetRoutingControl(routingControl);
        gData.routerSetInputsType(new TPZTypeArray(inputs.asInteger()));
        gData.routerSetOutputsType(new TPZTypeArray(outputs.asInteger()));
        
    }

    if (injectorControl == TPZ_TAG_INJ_RANDOM) {
        router->setInjectorType(TPZRouter::INJECT_RANDOM);
    } else if (injectorControl == TPZ_TAG_INJ_RROBIN) {
        router->setInjectorType(TPZRouter::INJECT_ROUND_ROBIN);
    } else if (injectorControl == TPZ_TAG_INJ_FREE) {
        router->setInjectorType(TPZRouter::INJECT_FREE);
    } else if (injectorControl == TPZ_TAG_INJ_VNETS) {
        router->setInjectorType(TPZRouter::INJECT_VNET);
    } else if (injectorControl == TPZ_TAG_INJ_VC) {
        router->setInjectorType(TPZRouter::INJECT_VC);
    } else {
        router->setInjectorType(TPZRouter::INJECT_ROUND_ROBIN);
    }

    TPZString endTag = TPZString("/") + tag->tagName();
    TPZTag* nextTag = TPZComponent::routerBuilder->getTagWithIndex(++index);

    TPZInterfaz** arrayInterfaz = new TPZInterfaz*[TPZ_MAX_NUMBER_OF_CV];
    
    while (nextTag && nextTag->tagName()!=endTag) {
        if (nextTag->tagName() == TPZ_TAG_INPUT) {
            // This is an entry to the router (a wrapper)
                        
            unsigned inputNumber = nextTag->tagId().asInteger();
            TPZString type, wrapper;
            nextTag->getAttributeValueWithName(TPZ_TAG_WRAPPER, wrapper);
            nextTag->getAttributeValueWithName(TPZ_TAG_TYPE, type);
            wrapper.replace(',', ' ');
            TPZString tempWrapper;
            int i=1;

            while ( (tempWrapper=wrapper.word(i)) != TPZString("") ) {
                TPZString wIzda = TPZString::getStringLeftTo(tempWrapper, '.');
                TPZString wDcha = TPZString::getStringRightTo(tempWrapper, '.');
                unsigned wNumb = (wDcha==TPZString("")) ? 1 : wDcha.asInteger();
                TPZComponent* comp = router->componentWithName(wIzda);
                if ( !comp) {
                    TPZString err;
                    err.sprintf(ERR_TPZROUTE_002, (char*)wrapper);
                    EXIT_PROGRAM(err);
                }
                if (i > TPZ_MAX_NUMBER_OF_CV) {
                    TPZString err;
                    err.sprintf(ERR_TPZROUTE_003, i);
                    EXIT_PROGRAM(err);
                }
                
                arrayInterfaz[i-1] = comp->getInputInterfaz(wNumb);
                i++;
            }

            TPZInterfaz* interfaz;
            interfaz = TPZInterfaz::createWrapperFor(arrayInterfaz, i-1);

            TPZString name = TPZString("I") + TPZString(inputNumber);
            interfaz->setName(name);
            router->addComponent(interfaz);

            if ( !gData.routerIsInitializated() )
                gData.routerInputsType()->setValueAt(inputNumber, _string2routingType(type));
        } else if (nextTag->tagName() == TPZ_TAG_OUTPUT) {
            // This is a solution to the router (a wrapper)
            

            unsigned outputNumber = nextTag->tagId().asInteger();
            TPZString type, wrapper;
            nextTag->getAttributeValueWithName(TPZ_TAG_WRAPPER, wrapper);
            nextTag->getAttributeValueWithName(TPZ_TAG_TYPE, type);
            wrapper.replace( ',', ' ');
            TPZString tempWrapper;
            int i=1;
            
            while ( (tempWrapper=wrapper.word(i)) != TPZString("") ) {
                TPZString wIzda = TPZString::getStringLeftTo(tempWrapper, '.');
                TPZString wDcha = TPZString::getStringRightTo(tempWrapper, '.');
                unsigned wNumb = (wDcha==TPZString("")) ? 1 : wDcha.asInteger();

                TPZComponent* comp = router->componentWithName(wIzda);
                if ( !comp) {
                    TPZString err;
                    err.sprintf(ERR_TPZROUTE_002, (char*)wrapper);
                    EXIT_PROGRAM(err);
                }
                if (i > TPZ_MAX_NUMBER_OF_CV) {
                    TPZString err;
                    err.sprintf(ERR_TPZROUTE_003, i);
                    EXIT_PROGRAM(err);
                }

                arrayInterfaz[i-1] = comp->getOutputInterfaz(wNumb);
                i++;
            }

            TPZInterfaz* interfaz;
            interfaz = TPZInterfaz::createWrapperFor(arrayInterfaz, i-1);

            TPZString name = TPZString("O") + TPZString(outputNumber);
            interfaz->setName(name);
            router->addComponent(interfaz);

            if ( !gData.routerIsInitializated() )
                gData.routerOutputsType()->setValueAt(outputNumber, _string2routingType(type));
        } else {
            // It is a component
            

            TPZComponent* newComponent = TPZComponent::routerBuilder->
            parseComponentDefinition(nextTag, router, index);
            if (newComponent) {
                newComponent->setOwner(router);
                newComponent->setRouter(router);
                newComponent->initialize();
                router->addComponent(newComponent);
            }
            
        }

        nextTag = TPZComponent::routerBuilder->getTagWithIndex(++index);

    } // while

    delete[] arrayInterfaz;

    gData.routerSetInitializated();

    router->initialize();
    router->postInitialize();

    return router;
}

//*************************************************************************
//:
//  f: void incrContaodr (TPZROUTINGTYPE direction);
//
//  d:
//:
//*************************************************************************

void TPZRouter::incrContador(TPZROUTINGTYPE direction) {
    switch (direction) {
    case _Xplus_:
        m_ContadorXplus++;
        break;
    case _Xminus_:
        m_ContadorXminus++;
        break;
    case _Yplus_:
        m_ContadorYplus++;
        break;
    case _Yminus_:
        m_ContadorYminus++;
        break;
    case _LocalNode_:
        m_ContadorLocal++;
        break;
    default:
        break;
    }
}

//*************************************************************************
//:
//  f: void decrContaodr (TPZROUTINGTYPE direction);
//
//  d:
//:
//*************************************************************************

void TPZRouter::decrContador(TPZROUTINGTYPE direction) {
    switch (direction) {
    case _Xplus_:
        m_ContadorXplus--;
        break;
    case _Xminus_:
        m_ContadorXminus--;
        break;
    case _Yplus_:
        m_ContadorYplus--;
        break;
    case _Yminus_:
        m_ContadorYminus--;
        break;
    case _LocalNode_:
        m_ContadorLocal--;
        break;
    default:
        break;
    }
}

//*************************************************************************
//:
//  f: unsigned getContaodr (TPZROUTINGTYPE direction);
//
//  d:
//:
//*************************************************************************

unsigned TPZRouter::getContador(TPZROUTINGTYPE direction) {
    switch (direction) {
    case _Xplus_:
        return m_ContadorXplus;
    case _Xminus_:
        return m_ContadorXminus;
    case _Yplus_:
        return m_ContadorYplus;
    case _Yminus_:
        return m_ContadorYminus;
    case _LocalNode_:
        return m_ContadorLocal;
    default:
        break;
    }
}

//*************************************************************************
//:
//  f: TPZRouter * getNeighbour (TPZROUTINGTYPE direction);
//
//  d:
//:
//*************************************************************************

TPZRouter* TPZRouter::getNeighbour(TPZROUTINGTYPE direction) {
    switch (direction) {
    case _Xplus_:
        return m_NeighbourXplus;
    case _Xminus_:
        return m_NeighbourXminus;
    case _Yplus_:
        return m_NeighbourYplus;
    case _Yminus_:
        return m_NeighbourYminus;
    case _Zplus_:
        return m_NeighbourZplus;
    case _Zminus_:
        return m_NeighbourZminus;
    case _LocalNode_:
        EXIT_PROGRAM("error");

    default:
        break;
    }
}
//*************************************************************************
//:
//  f: void incrProtocolNumber (unsigned index);
//
//  d:
//:
//*************************************************************************
void TPZRouter::incrProtocolNumber(unsigned index) {
    unsigned data;
    m_ProtocolMessages->valueAt(index, data);
    data++;
    m_ProtocolMessages->setValueAt(index, data);
}

//*************************************************************************
//:
//  f: void decrProtocolNumber (unsigned index);
//
//  d:
//:
//*************************************************************************
void TPZRouter::decrProtocolNumber(unsigned index) {
    unsigned data;
    m_ProtocolMessages->valueAt(index, data);
    data--;
    m_ProtocolMessages->setValueAt(index, data);
}

//*************************************************************************
//:
//  f: getProtocolNumber unsigned (unsigned index);
//
//  d:
//:
//*************************************************************************
unsigned TPZRouter::getProtocolNumber(unsigned index) {
    unsigned data;
    m_ProtocolMessages->valueAt(index, data);
    return data;
}

//*************************************************************************
//:
//  f: TPZPosition positionof (TPZROUTINGTYPE dir, const TPZPosition & pos)
//
//  d:
//:
//*************************************************************************
TPZPosition TPZRouter::positionOf(TPZROUTINGTYPE dir, const TPZPosition& pos,
        unsigned sizex, unsigned sizey) {

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

    }

    if (rp.valueForCoordinate(TPZPosition::X) < 0)
        rp.setValueAt(TPZPosition::X, sizex-1);

    if (rp.valueForCoordinate(TPZPosition::X) >= sizex)
        rp.setValueAt(TPZPosition::X, 0);

    if (rp.valueForCoordinate(TPZPosition::Y) < 0)
        rp.setValueAt(TPZPosition::Y, sizey-1);

    if (rp.valueForCoordinate(TPZPosition::Y) >= sizey)
        rp.setValueAt(TPZPosition::Y, 0);

    return rp;

}

//*************************************************************************
//:
//  f: void incrInOrderTable (TPZROUTINGTYPE type TPZROUTINGTYPE valor_table)
//
//  d:
//:
//*************************************************************************
void TPZRouter::incrInOrderTable(TPZROUTINGTYPE tipo, TPZROUTINGTYPE valor_table) {
    unsigned data;
    if (tipo == _Xplus_) {
        m_InOrderTableXplus->valueAt(valor_table, data);
        data++;
        m_InOrderTableXplus->setValueAt(valor_table, data);
    } else if (tipo == _Xminus_) {
        m_InOrderTableXminus->valueAt(valor_table, data);
        data++;
        m_InOrderTableXminus->setValueAt(valor_table, data);
    } else if (tipo == _Yplus_) {
        m_InOrderTableYplus->valueAt(valor_table, data);
        data++;
        m_InOrderTableYplus->setValueAt(valor_table, data);
    } else if (tipo == _Yminus_) {
        m_InOrderTableYminus->valueAt(valor_table, data);
        data++;
        m_InOrderTableYminus->setValueAt(valor_table, data);
    } else if (tipo == _LocalNode_) {
        m_InOrderTableLocalNode->valueAt(valor_table, data);
        data++;
        m_InOrderTableLocalNode->setValueAt(valor_table, data);
    }
}

//*************************************************************************
//:
//  f: unsigned getInOrderTable (TPZROUTINGTYPE type TPZROUTINGTYPE valor_table)
//
//  d:
//:
//*************************************************************************
unsigned TPZRouter::getInOrderTable(TPZROUTINGTYPE tipo,
        TPZROUTINGTYPE valor_table) {
    unsigned data;
    if (tipo == _Xplus_) {
        m_InOrderTableXplus->valueAt(valor_table, data);
    } else if (tipo == _Xminus_) {
        m_InOrderTableXminus->valueAt(valor_table, data);
    } else if (tipo == _Yplus_) {
        m_InOrderTableYplus->valueAt(valor_table, data);
    } else if (tipo == _Yminus_) {
        m_InOrderTableYminus->valueAt(valor_table, data);
    } else if (tipo == _LocalNode_) {
        m_InOrderTableLocalNode->valueAt(valor_table, data);
    }
    return data;
}
//*************************************************************************
//:
//  f: unsigned long long getMask (TPZROUTINGTYPE direction)
//
//  d:
//:
//*************************************************************************
unsigned long long TPZRouter::getMask(TPZROUTINGTYPE direction) {
    switch (direction) {
    case _Xplus_:
        return m_maskXplus;
    case _Xminus_:
        return m_maskXminus;
    case _Yplus_:
        return m_maskYplus;
    case _Yminus_:
        return m_maskYminus;
    case _Zplus_:
        return m_maskZplus;
    case _Zminus_:
        return m_maskZminus;
    case _LocalNode_:
        return m_maskLocalNode;

    default:
        break;
    }
}

//*************************************************************************
//:
//  f: void setMask (TPZROUTINGTYPE direction, unsigned long long mask)
//
//  d:
//:
//*************************************************************************
void TPZRouter::setMask(TPZROUTINGTYPE direction, unsigned long long mask) {
    switch (direction) {
    case _Xplus_:
        m_maskXplus= mask;
    case _Xminus_:
        m_maskXminus= mask;
    case _Yplus_:
        m_maskYplus= mask;
    case _Yminus_:
        m_maskYminus= mask;
    case _Zplus_:
        m_maskZplus= mask;
    case _Zminus_:
        m_maskZminus= mask;
    case _LocalNode_:
        m_maskLocalNode= mask;

    default:
        break;
    }
}

//*************************************************************************
//:
//  f: void setStateLink (TPZROUTINGTYPE type, Boolean state)
//
//  d:
//:
//*************************************************************************
void TPZRouter::setStateLink(TPZROUTINGTYPE direction, Boolean state)
{
   switch (direction) 
   {
      case _Xplus_:
      m_StateLinkXplus=state;
      break;
      case _Xminus_:
      m_StateLinkXminus=state;    
      break;
      case _Yplus_:
      m_StateLinkYplus=state;
      break;
      case _Yminus_:
      m_StateLinkYminus=state;
      break;
   }
}

void TPZRouter::setConsStateLink(Boolean reverse, Boolean state)
{
   if (reverse==false) m_StateLinkLocalNorm=state;
   else m_StateLinkLocalRev=state;
}
//*************************************************************************
//:
//  f: void getStateLink (TPZROUTINGTYPE type)
//
//  d:
//:
//*************************************************************************
Boolean TPZRouter::getStateLink(TPZROUTINGTYPE direction)
{
   switch (direction) 
   {
      case _Xplus_:
      return m_StateLinkXplus;
      break;
      case _Xminus_:
      return m_StateLinkXminus;    
      break;
      case _Yplus_:
      return m_StateLinkYplus;
      break;
      case _Yminus_:
      return m_StateLinkYminus;
      break;
      case _LocalNode_:
      return false;
      break;
   }
}

Boolean TPZRouter::getConsStateLink(Boolean reverse)
{
   if (reverse==false) return m_StateLinkLocalNorm;
   else return m_StateLinkLocalRev;
}
//*************************************************************************


// end of file
