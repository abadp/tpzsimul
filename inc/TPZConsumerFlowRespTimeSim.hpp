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
//    File: TPZConsumerFlowRespTimeSim.hpp
//
//    Class: TPZConsumerFlowRespTimeSim
//
//    Inherited from: TPZConsumerFlow
// :
//*************************************************************************
//end of header


#ifndef __TPZConsumerFlowRespTimeSim_HPP__
#define __TPZConsumerFlowRespTimeSim_HPP__

//*************************************************************************

#include <TPZConsumerFlow.hpp>

 
//*************************************************************************

   
//*************************************************************************   
   class TPZConsumerFlowRespTimeSim : public TPZConsumerFlow
   {
      typedef TPZConsumerFlow Inhereited;
      typedef TPZQueue<TPZMessage*> TPZMessageQueue;
   
   public:
      TPZConsumerFlowRespTimeSim( TPZComponent& component);
      virtual ~TPZConsumerFlowRespTimeSim();

      virtual Boolean inputReading();
      virtual Boolean stateChange();
      
      // Run time information
      DEFINE_RTTI(TPZConsumerFlowRespTimeSim);
   
   private:
      //Contencion de acceso a L2: 
      //  -Buffer de entrada necesario para consumidores Cut-through. 
      TPZMessageQueue m_fifo;
      // -Tamano de dicho buffer se coge de TPZConsumer

      
      //  -Flag que permite el consumo de un flit del buffer de entrada.
      Boolean m_readBuffer;
      
      //  -Paquete que entra al consumidor y se inserta al final de la cola.
      // Sirve para diferenciar m_inputData y se necesita para enlazar 
      // inputInterfaz()->getData(datoDeEntrada) y m_fifo.enqueue(datoDeEntrada)
      TPZMessage* m_inputPacket;
      
      //  -Contador de ciclos de espera.
      int m_stopCycles;
      
      //  -Flag que indica si el ultimo mensaje fue una peticion 
      //de lectura (true) o de escritura (false).
      Boolean m_wasReadNotWrite;
};

//*************************************************************************


#endif


// end of file
