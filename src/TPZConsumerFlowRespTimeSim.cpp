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
//  website (http://www.TPZ.unican.es/topaz/) and (2) e-mail a citation
//  for your published paper to topaz@TPZ.unican.es
//  
//  If you redistribute derivatives of this software, we request that
//  you notify us and either (1) ask people to register with us at our
//  website (http://www.TPZ.unican.es/topaz/) or (2) collect registration
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
//   along with the Multifacet GEMS; if not, write to the Free Software
//   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
//   02111-1307, USA
//
//   The GNU General Public License is contained in the file LICENSE.
//
// 	
//*************************************************************************
//:
//    File: TPZConsumerFlowRespTimeSim.cpp
//
//    Class: TPZConsumerFlowRespTimeSim
//
//    Inherited from: TPZConsumerFlow
// :
//*************************************************************************
//end of header

#include <TPZConsumerFlowRespTimeSim.hpp>

#ifndef __TPZGlobalData_HPP__
#include <TPZGlobalData.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZConsumer_HPP__
#include <TPZConsumer.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZRouter_HPP__
#include <TPZRouter.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
#endif
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZConsumerFlowRespTimeSim,TPZConsumerFlow);

//*************************************************************************
//:
//  f: TPZConsumerFlowRespTimeSim();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlowRespTimeSim::TPZConsumerFlowRespTimeSim(TPZComponent& component) :
		TPZConsumerFlow(component)
{
	//m_fifo = new TPZQueue();
	//m_stopCycles = 0;
	//si m_stopCycles comienza en 0 y la latencia de lectura/escritura es mayor, entonces
	//en el supuesto caso de que un paquete llegue en el instante t=0 deberia esperar ese numero de ciclos	
	m_wasReadNotWrite = true;//inicializo lectura a verdadero (deberia dar igual verdadero o falso)

	//ahora inicializo m_stopCycles con el valor de readLatency o writeLatency que hemos indicado en el Router3D.sgml
	//dependiendo de si m_wasReadNotWrite es inicializado a falso o verdadero. 
	// wasReadNotWrite(true)  => readLatency
	// wasReadNotWrite(false) => writeLatency
	m_stopCycles = ((TPZConsumer&)getComponent()).getReadLatency();
	
	m_readBuffer = true;
	
}

//*************************************************************************
//:
//  f: virtual ~TPZConsumerFlowRespTimeSim();
//
//  d:
//:
//*************************************************************************

TPZConsumerFlowRespTimeSim::~TPZConsumerFlowRespTimeSim() {
	delete &m_fifo;
	delete &m_stopCycles;
	delete &m_wasReadNotWrite;
	delete &m_readBuffer;
}

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************
Boolean TPZConsumerFlowRespTimeSim::inputReading() {
        
        //lectura del dato del 'head' del buffer de entrada
        if(m_readBuffer==true && m_fifo.numberOfElements()>0)
        {
           if (m_InputData) 
           {
              //cuando m_readBuffer = false (no se consume) m_inputData mantiene su valor
              //lo que puede producir dos releases consecutivos a la misma direccion => seg. fault
              m_InputData->setExternalInfo((void*) 0);
#ifdef PTOPAZ       
              unsigned n=m_InputData->getPoolIndex();
              getMessagePool(n).release(m_InputData);
#else
              getMessagePool().release(m_InputData);
#endif      
           }
           m_DataReceived = m_fifo.dequeue(m_InputData);
        } 
        else 
        {
           m_DataReceived = false;
        }
        
        //lectura del dato de la interfaz de entrada y escritura de dicho dato en el buffer
        if (inputInterfaz()->isReadyActive()) 
        {
           inputInterfaz()->getData(&m_inputPacket);
           m_fifo.enqueue(m_inputPacket);
#ifndef NO_TRAZA
           TPZString time = getOwnerRouter().getCurrentTime();
           TPZString texto = getComponent().asString() + " Flit Rx. TIME = " + time
                           + ".  " + m_inputPacket->asString();
           TPZWRITE2LOG(texto);
#endif

        }
        
        unsigned sizeForCT = ((TPZSimulation*)getComponent().getSimulation())->
                        getPacketLength((TPZNetwork*)getOwnerRouter().getOwner());

        //Control de flujo (estilo CT)
        if ((((TPZConsumer&) getComponent()).getBufferSize()
                                 - m_fifo.numberOfElements()) <= sizeForCT )
                inputInterfaz()->sendStopRightNow();
        else
                inputInterfaz()->clearStopRightNow();
        
        return true;
}


//*************************************************************************
//:
//  f: virtual Boolean stateChange ();
//
//  d:
//:
//*************************************************************************

Boolean TPZConsumerFlowRespTimeSim::stateChange() {
	
	if (m_DataReceived) {
#ifndef NO_TRAZA
		//Log: lectura del buffer: flit a procesar
		TPZString time = getOwnerRouter().getCurrentTime();
		TPZString texto = getComponent().asString() + " BUFFER-READ. TIME = " + time
				+ ".  " + m_InputData->asString();
#endif
		if (m_State == Information && m_InputData->isHeader()) {
			TPZString err;
			err.sprintf(
					"%s : I detect a Header flit before previous message Tail flit: %s",
					(char*) getComponent().asString(),
					(char*) m_InputData->asString());
			EXIT_PROGRAM(err);
		}

		if (m_State == Header) {
			if (m_InputData->isHeadTail())
				m_State = Header;
			else
				m_State = Information;

			if (m_InputData->destiny() != getOwnerRouter().getPosition()
					&& !m_InputData->isMulticast()) {
				TPZString err;
				err.sprintf(ERR_TPZCOFLO_001, (char*) getComponent().asString(),
						(char*) m_InputData->asString());
				EXIT_PROGRAM(err);
			}

			if (m_InputData->isHeadTail()) {
#ifndef NO_TRAZA
				texto += "  PACKET RX";
#endif

				m_InputData->setDestiny(getOwnerRouter().getPosition());
				getOwnerRouter().onPacketReceived(m_InputData);
				uTIME lastTime = getOwnerRouter().getCurrentTime();
				((TPZSimulation*) (getComponent().getSimulation()))->setLastMessage(
						lastTime);

				//Emulacion de contencion en el acceso a L2. Cuando se finaliza el envio del mensaje
                                //(lleva el ultimo flit) se completa la peticion de acceso a memoria, por lo que
                                //hay que esperar a que la memoria "devuelva el dato" (no se envia nada al consumidor hasta
                                //que transcurran t_acceso(L2) ns
				//Cuando el mensaje es 1 solo flit: read request (no lleva dato).
				if (getOwnerRouter().getPosition().valueForCoordinate(
						TPZPosition::Z) == 1){
					m_wasReadNotWrite = true;
					m_stopCycles = 0;
#ifndef NO_TRAZA
					texto += "  (Memory read request)";
#endif
				}

			}

		}

		if (m_InputData->isTail()) {
#ifndef NO_TRAZA
			texto += "  PACKET RX";
#endif
			getOwnerRouter().onPacketReceived(m_InputData);
			m_State = Header;
			uTIME lastTime = getOwnerRouter().getCurrentTime();
			((TPZSimulation*) (getComponent().getSimulation()))->setLastMessage(
					lastTime);

			//Emulacion de contencion en el acceso a L2.
			//Cuando el mensaje es de mas de un flit: write request (lleva dato).
			if (getOwnerRouter().getPosition().valueForCoordinate(
					TPZPosition::Z) == 1){
				m_wasReadNotWrite = false;
				m_stopCycles = 0;
#ifndef NO_TRAZA
				texto += "  (Memory write request)";
#endif								
			}

		}

#ifndef NO_TRAZA
		TPZWRITE2LOG(texto);
#endif
	}
        //emulacion de contencion en acceso a L2, control de flujo
        //se envia una senal de stop emulando el tiempo de acceso a memoria,
        //provocando que los demas mensajes esperen a la finalizacion de la peticion
	if(getOwnerRouter().getPosition().valueForCoordinate(TPZPosition::Z)==1)
        {
                m_stopCycles++; 
                
                if(m_wasReadNotWrite){          
                   if (m_stopCycles < ((TPZConsumer&) getComponent()).getReadLatency()) m_readBuffer = false;
                   else m_readBuffer = true;
                } 
                else {
                   if (m_stopCycles < ((TPZConsumer&) getComponent()).getWriteLatency())m_readBuffer = false;
                   else m_readBuffer = true;
                }
        }
        else
        {
           m_readBuffer = true;
        }
        return true;

}

// end of file
