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
//    File: TPZConnection.cpp
//
//    Class:  TPZConnection
//
//    Inherited from:  TPZBase
// :
//*************************************************************************
//end of header



#include <TPZConnection.hpp>

#ifndef __TPZPortInput_HPP__
#include <TPZPortInput.hpp>
#endif

#ifndef __TPZPortOutput_HPP__
#include <TPZPortOutput.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZMessage_HPP__
#include <TPZMessage.hpp>
#endif

#ifndef __TPZConnectionDelay_HPP__
#include <TPZConnectionDelay.hpp>
#endif

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZConnection,TPZBase);

//*************************************************************************
//:
//  f: inline getInputPort TPZPortInput * () const;
//  d:
//
//  f: inline getOutputPort TPZPortOutput * () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZConnection (TPZPortOutput * org, TPZPortInput * dst);
//
//  d:
//:
//*************************************************************************

TPZConnection :: TPZConnection( TPZPortOutput* org,
                                TPZPortInput* dst)
               : m_OutputPort(org),
                 m_InputPort(dst)
{
   setSource(org);
   setDestiny(dst);
}


//*************************************************************************
//:
//  f: TPZConnection (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZConnection :: TPZConnection()
               : m_InputPort(0),
                 m_OutputPort(0)
{

}


//*************************************************************************
//:
//  f: ~ TPZConnection ();
//
//  d:
//:
//*************************************************************************

TPZConnection :: ~TPZConnection()
{

}


//*************************************************************************
//:
//  f: void SetSource (TPZPortOutput * org);
//
//  d:
//:
//*************************************************************************

void TPZConnection :: setSource(TPZPortOutput* org)
{
   if(!org) return;
   m_OutputPort = org;
   m_OutputPort->establishConnection(this);
}


//*************************************************************************
//:
//  f: void setDestiny (TPZPortInput * dst);
//
//  d:
//:
//*************************************************************************

void TPZConnection :: setDestiny(TPZPortInput* dst)
{
   if(!dst) return;
   m_InputPort = dst;
   m_InputPort->establishConnection(this);
}


//*************************************************************************
//:
//  f: Boolean sendData (const TPZMessage & data);
//
//  d:
//:
//*************************************************************************

Boolean TPZConnection :: sendData(TPZMessage* data)
{
   if( m_InputPort )
   {
      m_InputPort->sendData(data);
      return m_InputPort->onDataUp(0,0);
   }
   return false;
}


//*************************************************************************
//:
//  f: Boolean ClearData ();
//
//  d:
//:
//*************************************************************************

void TPZConnection :: clearData()
{
   if( m_InputPort )
   {
      m_InputPort->clearData();
      m_InputPort->onDataDown(0,0);
   }
}


//*************************************************************************
//:
//  f: Boolean sendStop ();
//
//  d:
//:
//*************************************************************************

Boolean TPZConnection :: sendStop()
{
   if( m_OutputPort )
   {
      m_OutputPort->sendStop();
      return m_OutputPort->onStopUp(0,0);
   }
   return false;
}


//*************************************************************************
//:
//  f: void clearStop ();
//
//  d:
//:
//*************************************************************************

void TPZConnection :: clearStop()
{
   if( m_OutputPort )
   {
      m_OutputPort -> clearStop();
      m_OutputPort -> onStopDown(0,0);
   }
}


//*************************************************************************
//:
//  f: virtual void run (utime time);
//
//  d:
//:
//*************************************************************************

void TPZConnection :: run(uTIME time)
{
   if( m_InputPort && m_OutputPort )
   {
      if( m_OutputPort->isDataChanged() )
      {
         if( m_OutputPort->isReadyActive() )
         {
            TPZMessage *msg=m_OutputPort->getData();
            m_InputPort->sendData(msg);
            m_InputPort->onDataUp(0,0);
         }
         else
         {
            m_InputPort->clearData();
            m_InputPort->onDataDown(0,0);
         }
         m_OutputPort->setDataChanged(false);
      }
      
      
      if( m_InputPort->isStopChanged() )
      {
         if( m_InputPort->isStopActive() )   // Stop Signal
         {
            m_OutputPort->sendStop();
            m_OutputPort->onStopUp(0,0);
         }
         else
         {
            m_OutputPort->clearStop();
            m_OutputPort->onStopDown(0,0);      
         }
         m_InputPort->setStopChanged(false);
      }
      

   }
}


//*************************************************************************
//:
//  f: static Boolean connectInterfaces (TPZComponent * owner,
//                                       TPZInterfaz * src,
//                                       TPZInterfaz * dst,
//                                       unsigned delay = 0);
//
//  d:
//:
//*************************************************************************

Boolean TPZConnection :: connectInterfaces( TPZComponent* owner,
                                            TPZInterfaz* src, 
                                            TPZInterfaz* dst,
                                            unsigned delay )
{
   if( delay != 0 )
     return TPZConnectionDelay::connectInterfaces(owner,src,dst,delay);
     
   if( ! (owner && src && dst) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCONEC_001,owner,src,dst);
      EXIT_PROGRAM(err);
   }
   if( src->numberOfCV() != dst->numberOfCV() )
   {
      TPZString err;
      err.sprintf( ERR_TPZCONEC_002, 
                   src->numberOfCV(), 
                   dst->numberOfCV() 
                   );
      EXIT_PROGRAM(err);
   }

   for( int i=1; i<=src->numberOfCV(); i++ )
   {
      TPZPortOutput* srcPort = POINTERCAST(TPZPortOutput,src->getPort(i));
      TPZPortInput*  dstPort = POINTERCAST(TPZPortInput, dst->getPort(i) );
      if( ! dstPort )
      {
         TPZString err;
         err.sprintf(ERR_TPZCONEC_003, (char*)(dst->getName()) );
         EXIT_PROGRAM(err);
      }
      if( ! srcPort )
      {
         TPZString err;
         err.sprintf(ERR_TPZCONEC_004, (char*)(src->getName()) );
         EXIT_PROGRAM(err);
      }
      
      TPZConnection* conn = new TPZConnection(srcPort,dstPort);

      // connections are sent to the owner in order to destroy them when appropriate.
   
      owner->addConnection(conn);
   }
   
   return true;
}

//*************************************************************************
//:
//  f: static TPZConnection * newFrom (TPZTag const * tag, TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

TPZConnection* TPZConnection :: newFrom(const TPZTag* tag, TPZComponent* owner)
{
   TPZString source, destiny;

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_SOURCE, source)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCONEC_005, (char*)(tag->tagId()) );
      EXIT_PROGRAM(err);
   }

   if( ! (tag->getAttributeValueWithName(TPZ_TAG_DESTINY, destiny)) )
   {
      TPZString err;
      err.sprintf(ERR_TPZCONEC_006, (char*)(tag->tagId()) );
      EXIT_PROGRAM(err);
   }
   
   // Identifiers can be indexed (with a dot)
   
   TPZString sourceName = TPZString::getStringLeftTo(source,'.');
   TPZString sourcePort = TPZString::getStringRightTo(source,'.');
   unsigned sPort = (sourcePort!=TPZString("")) ? sourcePort.asInteger() : 1; 

   TPZString destinyName = TPZString::getStringLeftTo(destiny,'.');
   TPZString destinyPort = TPZString::getStringRightTo(destiny,'.');
   unsigned dPort = (destinyPort!=TPZString("")) ? destinyPort.asInteger() : 1; 
   
   TPZComponent* sComponent = owner->componentWithName(sourceName);
   TPZComponent* dComponent = owner->componentWithName(destinyName);
   
   if( !sComponent )
   {
      TPZString err;
      err.sprintf(ERR_TPZCONEC_007, (char*)source, (char*)(tag->tagId()) ); 
      EXIT_PROGRAM(err);
   }
   
   if( !dComponent )
   {
      TPZString err;
      err.sprintf(ERR_TPZCONEC_008, (char*)destiny, (char*)(tag->tagId()) ); 
      EXIT_PROGRAM(err);
   }
   TPZConnection::connectInterfaces( owner,
                                     sComponent->getOutputInterfaz(sPort),
                                     dComponent->getInputInterfaz(dPort) );
                     
   
   return 0;
}

//*************************************************************************


// end of file
