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
//    File: TPZFifoMemoryFlowWH.cpp
//
//    Class:  TPZFifoMemoryFlowWH
//
//    Inherited from:  TPZFifoMemoryFlow
// :
//*************************************************************************
//end of header


#include <TPZFifoMemoryFlowWH.hpp>

#ifndef __TPZNetwork_HPP__
#include <TPZNetwork.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

#ifndef __TPZSimulation_HPP__
#include <TPZSimulation.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZFifoMemoryFlowWH, TPZFifoMemoryFlow);

//*************************************************************************
//:
//  f: TPZFifoMemoryFlowWH (TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZFifoMemoryFlowWH::TPZFifoMemoryFlowWH(TPZComponent& component) :
    TPZFifoMemoryFlow(component) {

}

//*************************************************************************
//:
//  f: virtual Boolean inputReading ();
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlowWH::inputReading() {
    TPZMessage* msg;
    inputInterfaz()->getData(&msg);

    if (!inputInterfaz()->isReadyActive() && m_tail==false) {
        uTIME delayTime = getOwnerRouter().getCurrentTime() ;
        TPZString err= "A tail flit was expected but transmission has been interrupted";
        err=err+msg->asString()+ "Time crash=" + TPZString(delayTime);
        EXIT_PROGRAM(err);
    }

    if (inputInterfaz()->isReadyActive() ) {
        msg->incDistance();
	((TPZNetwork*)(getOwnerRouter().getOwner()))->incrEventCount( TPZNetwork::BufferWrite);
        
	if (msg->isTail() || msg->isHeadTail()) {
            m_tail=true;
            setControlTail(true);
        }

        if (msg->isHeader() || msg->isHeadTail()) {
            getOwnerRouter().incrContadorTotal();
        }
        
        if (msg->isHeader() && m_tail==false) {
            uTIME delayTime = getOwnerRouter().getCurrentTime() ;
            TPZString err= "A head flit arrives before previous tail flit";
            err=err+msg->asString()+ "Time crash=" + TPZString(delayTime);
            EXIT_PROGRAM(err);
        }

        if ( !bufferHoles()) {
            uTIME delayTime = getOwnerRouter().getCurrentTime() ;
            TPZString err;
            err.sprintf(ERR_TPZFFLOW_001, (char*)getComponent().asString());
            err=err+msg->asString()+ "Time crash=" + TPZString(delayTime);
            EXIT_PROGRAM(err);
        }

        uTIME delay = getOwnerRouter().getCurrentTime() + getDataDelay();
        TPZEvent event(_BufferData_, msg);
        getEventQueue().enqueue(event, delay);

#ifndef NO_TRAZA
        uTIME delayTime = getOwnerRouter().getCurrentTime() ;
        TPZString texto = getComponent().asString() + " Flit Rx on Time" + TPZString(delayTime) + " # "
                + msg->asString() + ") holes= " + TPZString(bufferHoles());
        TPZWRITE2LOG(texto);
#endif

        stateChange();
        if (m_SendStop) {
            inputInterfaz()->sendStopRightNow();
#ifndef NO_TRAZA
            uTIME time = getOwnerRouter().getCurrentTime();
            TPZString texto = getComponent().asString() + ": Full Buffer->Stop ";
            texto += TPZString(time);
            TPZWRITE2LOG(texto);
#endif
        } else {
            inputInterfaz()->clearStopRightNow();
        }
        return true;
    }
    return false;
}

//*************************************************************************
//:
//  f: virtual Boolean controlAlgoritm (Boolean info = false, int delta = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZFifoMemoryFlowWH::controlAlgoritm(Boolean info, int delta) {
    unsigned delay = TPZNETWORK->getConnectionDelay() + 1;

    return (bufferHoles() > delay + 1 ) ? true : false;
}

//*************************************************************************


// end of file
