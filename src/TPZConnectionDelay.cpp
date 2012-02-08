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
//    File: TPZConnectionDelay.cpp
//
//    Class:  TPZConnectionDelay
//
//    Inherited from:  TPZConnection
// :
//*************************************************************************
//end of header



#include <TPZConnectionDelay.hpp>

#ifndef __TPZPortInput_HPP__
#include <TPZPortInput.hpp>
#endif

#ifndef __TPZPortOutput_HPP__
#include <TPZPortOutput.hpp>
#endif

#ifndef __TPZMessage_HPP__
#include <TPZMessage.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif


//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZConnectionDelay,TPZConnection);


//*************************************************************************
//:
//  f: TPZConnectionDelay (TPZPortOutput * org,
//                         TPZPortInput * dst,
//                         unsigned cycle);
//
//  d:
//:
//*************************************************************************

TPZConnectionDelay :: TPZConnectionDelay( TPZPortOutput* org,
                                          TPZPortInput* dst,
                                          unsigned ciclo )
                    : TPZConnection(org,dst),
                      m_Delay(ciclo),
                      m_MessageDelay(0)
{
   m_MessageDelay = new TPZMessage*[m_Delay];
   for( int i=0; i<m_Delay; i++ )
      *(m_MessageDelay+i) = 0;
}


//*************************************************************************
//:
//  f: ~ TPZConnectionDelay ();
//
//  d:
//:
//*************************************************************************

TPZConnectionDelay :: ~TPZConnectionDelay()
{
   if( m_MessageDelay !=0) delete[] m_MessageDelay;
}


//*************************************************************************
//:
//  f: virtual void run (TIME Runtime);
//
//  d:
//:
//*************************************************************************

void TPZConnectionDelay :: run(uTIME runTime)
{
   if( getInputPort() && getOutputPort() )
   {
      TPZMessage* newMsg = (getOutputPort()->isReadyActive()) ? 
                           getOutputPort()->getData() :
                           0 ;
      if(newMsg) TPZMSGTRACE(newMsg,"TPZConnectionDelay :: run(uTIME time)    in");
      addMessage(newMsg);
      
      newMsg = getNextMessage();
      if(newMsg) TPZMSGTRACE(newMsg,"TPZConnectionDelay :: run(uTIME time)     out");
      
      if( newMsg )
      {
         getInputPort()->sendData(newMsg);
         getInputPort()->onDataUp(0,0);
      }
      else
      {
         getInputPort()->clearData();
         getInputPort()->onDataDown(0,0);
      }
      
      if(  getInputPort()->isStopChanged() )
      {
         if(  getInputPort()->isStopActive()   )   // Stop Signal
         {
            getOutputPort()->sendStop();
            getOutputPort()->onStopUp(0,0);
         }
         else
         {
            getOutputPort()->clearStop();
            getOutputPort()->onStopDown(0,0);      
         }
         getInputPort()->setStopChanged(false);
      }
   }
}


//*************************************************************************
//:
//  f: Boolean sendData (const TPZMessage & data);
//
//  d:
//:
//*************************************************************************

Boolean TPZConnectionDelay :: sendData(TPZMessage* data)
{
   if( getInputPort() )
   {
      addMessage(data);
      TPZMessage* newMsg = getNextMessage();
      if( newMsg )
      {
         getInputPort()->sendData(newMsg);
         getInputPort()->onDataUp(0,0);
         delete newMsg;
      }
      else
      {
         getInputPort()->clearData();
         getInputPort()->onDataDown(0,0);
      }
      
      return true;
   }

   return false;
}


//*************************************************************************
//:
//  f: void ClearData ();
//
//  d:
//:
//*************************************************************************

void TPZConnectionDelay :: clearData()
{
   if( getInputPort() )
   {
      TPZMessage* newMsg = 0;
      addMessage(newMsg);
      newMsg = getNextMessage();
      if( newMsg )
      {
         getInputPort()->sendData(newMsg);
         getInputPort()->onDataUp(0,0);
      }
      else
      {
         getInputPort()->clearData();
         getInputPort()->onDataDown(0,0);
      }
   }
}

//*************************************************************************
//:
//  f: void addMessage (TPZMessage * msg);
//
//  d:
//:
//*************************************************************************

void TPZConnectionDelay :: addMessage(TPZMessage* msg)
{
   // The messages go to 0 and exit through m_delay-1;
   *m_MessageDelay = msg;
}


//*************************************************************************
//:
//  f: TPZMessage * getNextMessage ();
//
//  d:
//:
//*************************************************************************

TPZMessage* TPZConnectionDelay :: getNextMessage()
{
   // Messages get in through 0 and exit through m_delay-1;
   
   TPZMessage* rm = *(m_MessageDelay+m_Delay-1);
   for(int i=m_Delay-1; i>0; i-- )
      *(m_MessageDelay+i) = *(m_MessageDelay+i-1);
      
   *m_MessageDelay = 0;   
   
   return rm;
}


//*************************************************************************
//:
//  f: static Boolean connectInterfaces (TPZComponent * owner,
//                                       TPZInterfaz * src,
//                                       TPZInterfaz * dst,
//                                       unsigned cycles);
//
//  d:
//:
//*************************************************************************

Boolean TPZConnectionDelay :: connectInterfaces( TPZComponent* owner,
                                                 TPZInterfaz* src, 
                                                 TPZInterfaz* dst,
                                                 unsigned ciclos )
{
   if( ! (owner && src && dst) )
   {
      TPZString err;
      err.sprintf(ERR_TPZDECON_001,owner,src,dst);
      EXIT_PROGRAM(err);
   }
   if( src->numberOfCV() != dst->numberOfCV() )
   {
      TPZString err;
      err.sprintf( ERR_TPZDECON_002, 
                   src->numberOfCV(), 
                   dst->numberOfCV() );
      EXIT_PROGRAM(err);
   }

   for( int i=1; i<=src->numberOfCV(); i++ )
   {
      TPZPortOutput* srcPort = POINTERCAST(TPZPortOutput,src->getPort(i));
      TPZPortInput*  dstPort = POINTERCAST(TPZPortInput, dst->getPort(i) );
   
      if( ! dstPort )
      {
         TPZString err;
         err.sprintf(ERR_TPZDECON_003, (char*)(dst->getName()) );
         EXIT_PROGRAM(err);
      }
      if( ! srcPort )
      {
         TPZString err;
         err.sprintf(ERR_TPZDECON_004, (char*)(src->getName()) );
         EXIT_PROGRAM(err);
      }
   
      TPZConnectionDelay* conn = 
                          new TPZConnectionDelay(srcPort,dstPort, ciclos);
   
      owner->addConnection(conn);

   }
   
   return true;
}

//*************************************************************************


// end of file
