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
//    File: TPZRouter.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZRouter_HPP__
#define __TPZRouter_HPP__

//************************************************************************

#include <TPZRunnableComponent.hpp>

#ifndef __TPZPosition_HPP__
#include <TPZPosition.hpp>
#endif

#ifndef __TPZArray_HPP__
#include <TPZArray.hpp>
#endif

#ifndef __TPZQueue_HPP__
#include <TPZQueue.hpp>
#endif

//************************************************************************

class TPZFifoMemory;
class TPZMultiportFifo;
class TPZMultiportIOFifo;
class TPZInjector;
class TPZInputStage;
class TPZOutputStage;

//************************************************************************

class TPZRouter : public TPZRunnableComponent {
	typedef TPZRunnableComponent Inhereited;
	typedef TPZSequence<TPZFifoMemory*> TPZBufferList;
	typedef TPZSequence<TPZMultiportIOFifo*> TPZMultiportIOBufferList;
	typedef TPZSequence<TPZMultiportFifo*> TPZMultiportBufferList;
	typedef TPZSequence<TPZInputStage*> TPZInputStageList;
	typedef TPZSequence<TPZOutputStage*> TPZOutputStageList;
	typedef TPZSequence<TPZComponent*> TPZInjectorList;
	typedef TPZSequence<TPZComponent*> TPZConsumerList;
	typedef TPZSequence<TPZMessage*> TPZMessageList;
	typedef TPZArray<unsigned> PAFProtocolMessages;
	typedef TPZQueue<void*> TPZExternalInfoQueue;

	friend class TPZRouterBuilder;	

public:

	enum {INJECT_RANDOM, INJECT_ROUND_ROBIN, INJECT_FREE,INJECT_VNET, INJECT_VC};

	TPZRouter(const TPZComponentId& id);
	virtual ~TPZRouter();

	virtual TPZString asString() const;
	virtual void run(uTIME runTime);
	//added temporally
	virtual void preRun(uTIME runTime);
	virtual void postRun(uTIME runTime);
	virtual void initialize();
	virtual void terminate();
	virtual void postInitialize();
	virtual void sendMessage(TPZMessage* msg);
	
	//*******************************************************************
	//ADDED FOR MULTIPLE TRAFFIC SOURCES SHARING THE ONLY VIRTUAL CHANNEL
	//*******************************************************************

	void incrProtocolNumber(unsigned index);

	void decrProtocolNumber(unsigned index);

	unsigned getProtocolNumber(unsigned index);

	void setNeighbourXplus(TPZRouter* router) {
		m_NeighbourXplus= router;
	}

	void setNeighbourXminus(TPZRouter* router) {
		m_NeighbourXminus= router;
	}

	void setNeighbourYplus(TPZRouter* router) {
		m_NeighbourYplus= router;
	}

	void setNeighbourYminus(TPZRouter* router) {
		m_NeighbourYminus= router;
	}
	
	void setNeighbourZplus(TPZRouter* router) {
		m_NeighbourZplus= router;
	}

	void setNeighbourZminus(TPZRouter* router) {
		m_NeighbourZminus= router;
	}

	//******************************************************************
	//END OF ADDITION
	//******************************************************************      

	virtual unsigned
			getBufferOccupation(TPZROUTINGTYPE type, unsigned channel =1);
	
	unsigned getMessagesToSend() const;
	unsigned getSentFlits() const;
	void clearSentFlits();
	void writeBufferInformation(ostream& os);
	void writeBufferStatus(ostream& os);
	TPZString writeBufferStatus() const;
	TPZString writeBuffersName() const;

	void addBuffer(TPZFifoMemory* fifo);
	void addMultiportIOBuffer(TPZMultiportIOFifo* mportio);
	void addMultiportBuffer(TPZMultiportFifo* mport);
	void addOutputStageBuffer (TPZOutputStage * ostage);
	void addInputStageBuffer (TPZInputStage * istage);
	void addInjector(TPZComponent* injector);
	void addConsumer(TPZComponent* consumer);

	void setBufferSize(unsigned size);

	void setNumberOfHosts(unsigned size) {
		m_numberOfHosts=size;
	}

	unsigned getNumberOfHosts() const {
		return m_numberOfHosts;
	}

	void onPacketReceived(TPZMessage* msg);
	void onHeaderPacketReceived(const TPZMessage* msg);
	void writeComponentStatus(ostream& os);

	unsigned getNumberOfBuffers() const {
		return m_BufferList.numberOfElements();
	}

	uTIME getCurrentTime() const {
		return m_CurrentTime;
	}

	TPZPosition& getPosition() const {
		return (TPZPosition&)m_Position;
	}

	int getFirstCoordinate() const {
		return m_Position.getFirstCoordinate();
	}

	void setPosition(const TPZPosition& position) {
		m_Position=position;
	}

	void setInjectorType(int type) {
		m_InjectionType = type;
	}

	void* getExternalInfo();

	//*********************************************************
	//Half Duplex Functions
	//*********************************************************
	void incrContadorLinkX()
	{ m_ContadorLinkX++; }
	
	void incrContadorLinkY()
	{ m_ContadorLinkY++; }
	
	void decrContadorLinkX()
	{ m_ContadorLinkX--; }
	
	void decrContadorLinkY()
	{ m_ContadorLinkY--; }
	
	unsigned getContadorLinkX()
	{ return m_ContadorLinkX; }
	
	unsigned getContadorLinkY()
	{ return m_ContadorLinkY; }
	
	void incrLinkUtilization()
	{ m_LinkUtilization++; }
	
	unsigned long getLinkUtilization()
	{ return m_LinkUtilization; }
	
	void incrLinkChangesDirection()
	{ m_LinkChangesDirection++;}
	
	unsigned long getLinkChangesDirection()
	{ return m_LinkChangesDirection; }
	
	void setStateLink(TPZROUTINGTYPE direction, Boolean state);
	Boolean getStateLink(TPZROUTINGTYPE direction);
        void setConsStateLink(Boolean reverse, Boolean state);
	Boolean getConsStateLink(Boolean reverse);
	//*********************************************************
	//Multicast traffic functions
	//*********************************************************

	unsigned long long getMask(TPZROUTINGTYPE direction);

	void setMask(TPZROUTINGTYPE direction, unsigned long long mask);

	//**********************************************************

	//Functions Rotary

        Boolean getFreeInOrder( TPZROUTINGTYPE direction );
        void setInOrderOccupied( TPZROUTINGTYPE direction );
        void setInOrderFree( TPZROUTINGTYPE direction );
	
	void incrContador(TPZROUTINGTYPE direction);

	void decrContador(TPZROUTINGTYPE direction);

	unsigned getContador(TPZROUTINGTYPE direction);

	TPZRouter* getNeighbour(TPZROUTINGTYPE direction);
	
	void setEscapePath( Boolean value)
	{ m_escapePathFree=value;}
	
	Boolean getEscapePath()
	{ return m_escapePathFree;}

	void incrContadorTotal() {
		m_ContadorTotal++;
	}

	void decrContadorTotal() {
		m_ContadorTotal--;
	}

	unsigned getContadorTotal() {
		return m_ContadorTotal;
	}

	void setExternalInfo(void* ptr);

	void setPoolSize(unsigned size);
	void addToPoolSize(unsigned size) {
		m_ExtraPoolSize += size;
	}
	
	// Run time information
	DEFINE_RTTI(TPZRouter);

	unsigned getInputWithType(TPZROUTINGTYPE type);
	unsigned getOutputWithType(TPZROUTINGTYPE type);

	unsigned valueForBufferSize();
	TPZString valueForBufferControl();
	TPZString valueForRoutingControl();

	//*********************************************************
	//Multithread functions
	//*********************************************************
#ifdef PTOPAZ	
        void setThreadNumber(unsigned n)
        { m_MyThread = n;}
    
        unsigned getThreadNumber() const
        {return m_MyThread;}
#endif
	//**********************************************************
	
protected:
	virtual void buildFlowControl();
	virtual void buildInputInterfaz(unsigned number=1);
	virtual void buildOutputInterfaz(unsigned number=1);

	TPZPosition positionOf(TPZROUTINGTYPE dir, const TPZPosition& pos,
			unsigned sizex, unsigned sizey);

	virtual TPZInjector* chooseInjector(unsigned vNetwork, TPZPosition& src,
			TPZPosition& dst);

private:
	unsigned getTotalBuffeSize() const;

	static TPZRouter* newFrom(const TPZTag* tag, TPZComponent* owner, INDEX& i);

	uTIME m_CurrentTime;
	TPZPosition m_Position;
	TPZBufferList m_BufferList;
	TPZMultiportIOBufferList m_MultiportIOBufferList;
	TPZMultiportBufferList m_MultiportBufferList;
	TPZInputStageList m_InputStageList;
	TPZOutputStageList m_OutputStageList;
	TPZInjectorList m_InjectorList;
	TPZConsumerList m_ConsumerList;
	unsigned m_InjectorRoundRobin;
	int m_InjectionType;
	TPZMessageList m_MessageList;
	TPZExternalInfoQueue m_ExternalInfo;
	unsigned m_ExtraPoolSize;
	unsigned m_numberOfHosts;
	
	//Mask for multicast routing

	unsigned long long m_maskXplus;
	unsigned long long m_maskXminus;
	unsigned long long m_maskYplus;
	unsigned long long m_maskYminus;
	unsigned long long m_maskZplus;
	unsigned long long m_maskZminus;
	unsigned long long m_maskLocalNode;
	
	Boolean m_escapePathFree;

        Boolean m_XplusFreeInOrder;
        Boolean m_XminusFreeInOrder;
        Boolean m_YplusFreeInOrder;
        Boolean m_YminusFreeInOrder;
        Boolean m_LocalNodeFreeInOrder;
	//roll in mixture of packages for the roundabout

	unsigned m_ContadorXplus;
	unsigned m_ContadorXminus;
	unsigned m_ContadorYplus;
	unsigned m_ContadorYminus;
	unsigned m_ContadorLocal;
	unsigned m_ContadorTotal;
	
	//Control occupancy of links to halfduplex
	Boolean m_StateLinkXplus;
	Boolean m_StateLinkXminus;
	Boolean m_StateLinkYplus;
	Boolean m_StateLinkYminus;
	Boolean m_StateLinkLocalNorm;
	Boolean m_StateLinkLocalRev;
	
	unsigned m_ContadorLinkX;
	unsigned m_ContadorLinkY;
	
	//Control of use of the links
	unsigned long m_LinkUtilization;
	unsigned long m_LinkChangesDirection;

	PAFProtocolMessages* m_ProtocolMessages;

	TPZRouter* m_NeighbourXplus;
	TPZRouter* m_NeighbourXminus;
	TPZRouter* m_NeighbourYplus;
	TPZRouter* m_NeighbourYminus;
	TPZRouter* m_NeighbourZplus;
	TPZRouter* m_NeighbourZminus;
	
#ifdef PTOPAZ	
        pthread_mutex_t  m_routerKey;
	unsigned        m_MyThread;
#endif

};

//*************************************************************************

#endif

// end of file
