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
//    File: TPZInterfaz.cpp
//
//    Class:  TPZInterfaz
//
//    Inherited from:  TPZComponent
// :
//*************************************************************************
//end of header



#include <TPZInterfaz.hpp>

#ifndef __TPZMessage_HPP__
#include <TPZMessage.hpp>
#endif

#ifndef __TPZPortInput_HPP__
#include <TPZPortInput.hpp>
#endif

#ifndef __TPZPortOutput_HPP__
#include <TPZPortOutput.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

//*************************************************************************

   TPZString TPZInterfaz::sm_InputPrefix = "I";
   TPZString TPZInterfaz::sm_OutputPrefix = "O";
   
//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZInterfaz,TPZComponent);

//*************************************************************************
//:
//  f: inline unsigned numberOfCV () const;
//  d:
//
//  f: inline getReadyPort TPZPort * (unsigned channel = 1) const;
//  d:
//
//  f: inline getStopPort TPZPort * (unsigned channel = 1) const;
//  d:
//
//  f: inline getDataPort TPZPort * (unsigned channel = 1) const;
//  d:
//
//  f: inline unsigned getNumberId () const;
//  d:
//
//  f: inline void setNumberId (unsigned number);
//  d:
//
//  f: inline void setReadyPort (TPZPort * ready, unsigned channel = 1);
//  d:
//
//  f: inline void setStopPort (TPZPort * stop, unsigned channel = 1);
//  d:
//
//  f: inline void setDataPort (TPZPort * data, unsigned channel = 1);
//  d:
//
//  f: inline Boolean isChannelValid (unsigned ch) const;
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: TPZInterfaz (const TPZComponentId & id,
//                  numberCV unsigned,
//                  Boolean wrapper = false);
//
//  d:
//:
//*************************************************************************

TPZInterfaz :: TPZInterfaz( const TPZComponentId& id, 
                            unsigned numberCV,
                            Boolean wrapper )
             : TPZComponent(id),
               m_Ports(0),
               m_SizeCV(numberCV),
               m_Wrapper(wrapper)               
{
   m_Ports = new TPZPort*[m_SizeCV];
   for( int i=0; i<m_SizeCV; i++ )
   {
      *(m_Ports+i) = 0;
   }
}


//*************************************************************************
//:
//  f: TPZInterfaz (const TPZInterfaz & interface);
//
//  d:
//:
//*************************************************************************

TPZInterfaz :: TPZInterfaz(const TPZInterfaz& interfaz)
             : TPZComponent(interfaz),
               m_Ports(0),
               m_Number(interfaz.m_Number),
               m_SizeCV(interfaz.m_SizeCV),
               m_Wrapper(true)               
{
   m_Ports = new TPZPort*[m_SizeCV];
   for( int i=0; i<m_SizeCV; i++ )
   {
      *(m_Ports+i) = *(interfaz.m_Ports+i);
   }
}


//*************************************************************************
//:
//  f: virtual ~ TPZInterfaz ();
//
//  d:
//:
//*************************************************************************

TPZInterfaz :: ~TPZInterfaz()
{
   if( ! m_Wrapper )
   {
      for( int i=0; i<m_SizeCV; i++ )
      {
         delete *(m_Ports+i);
      }
   }

   delete[] m_Ports;
}


//*************************************************************************
//:
//  f: virtual void sendData (const TPZMessage & data, unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: sendData(TPZMessage* data, unsigned channel)                                    
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendData(data);      
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void sendDataRightNow (const TPZMessage & data, unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: sendDataRightNow(TPZMessage* data, unsigned channel)                                    
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendDataRightNow(data);      
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void ClearData (unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: clearData(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearData();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void clearDataRightNow (unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: clearDataRightNow(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearDataRightNow();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}

//*************************************************************************
//:
//  f: virtual Boolean isReadyActive (unsigned channel = 1) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: isReadyActive(unsigned channel) const
{
   if( m_Ports && isChannelValid(channel) )
   {
      return (*(m_Ports+channel-1))->isReadyActive();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }

   return false;
}


//*************************************************************************
//:
//  f: virtual void sendStop (unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: sendStop(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendStop();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void sendStopRightNow (unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: sendStopRightNow(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->sendStopRightNow();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
   
}

//*************************************************************************
//:
//  f: virtual void clearStop (unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: clearStop(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearStop();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void clearStopRightNow (unsigned channel = 1);
//
//  d:
//:
//*************************************************************************
      
void TPZInterfaz :: clearStopRightNow(unsigned channel)
{
   if( m_Ports && isChannelValid(channel) )
   {
      (*(m_Ports+channel-1))->clearStopRightNow();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual Boolean isStopActive (unsigned channel = 1) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: isStopActive(unsigned channel) const
{
   if( m_Ports && isChannelValid(channel) )
   {
      return (*(m_Ports+channel-1))->isStopActive();
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      err+=TPZString(m_SizeCV);
      err+=getOwner()->asString();
      EXIT_PROGRAM(err);
   }

   return false;
}




//*************************************************************************
//:
//  f: virtual Boolean getData (TPZMessage & msg, unsigned channel = 1) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: getData(TPZMessage ** msg, unsigned channel) const
{
   if( m_Ports && isChannelValid(channel) )
   {
      if( (*(m_Ports+channel-1))->isReadyActive() )
      {
          *msg = (*(m_Ports+channel-1))->getData();
          return true;
      }
      else
      {
         return false;
      }
   }
   else
   {
      TPZString err;
      err.sprintf( ERR_TPZINTER_001, channel );
      EXIT_PROGRAM(err);
   }

   return false;
}


//*************************************************************************
//:
//  f: static createInputInterfaz TPZInterfaz * (unsigned number = 1,
//                                              unsigned numberCV = 1);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZInterfaz :: createInputInterfaz( unsigned number,
                                                 unsigned numberCV)
{
   TPZString name = TPZInterfaz::sm_InputPrefix + TPZString(number);
   TPZComponentId idInterfaz( TPZComponentId::InputInterfaz,
                              _LocalNode_,
                              name );

   TPZInterfaz* interfaz = new TPZInterfaz(idInterfaz,numberCV);
   interfaz->setNumberId(number);
   for( int i=1; i<=numberCV; i++ )
   {
      
      TPZPort* port = new TPZPortInput(INPUT_PORT);
      port->setChannel(i);
      interfaz->setPort(port,i);
   }
      
   return interfaz;
}


//*************************************************************************
//:
//  f: static createOutputInterfaz TPZInterfaz * (unsigned number = 1,
//                                               unsigned numberCV = 1);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZInterfaz :: createOutputInterfaz(unsigned number,
                                                 unsigned numberCV )
{
   TPZString name = TPZInterfaz::sm_OutputPrefix + TPZString(number);
   TPZComponentId idInterfaz( TPZComponentId::OutputInterfaz,
                              _LocalNode_,
                              name );

   TPZInterfaz* interfaz = new TPZInterfaz(idInterfaz, numberCV);
   interfaz->setNumberId(number);
   for( int i=1; i<=numberCV; i++ )
   {
      TPZPort* port = new TPZPortOutput(OUTPUT_PORT);
      port->setChannel(i);
      interfaz->setPort(port,i);
   }
   
   return interfaz;
}

//*************************************************************************
//:
//  f: static TPZInterfaz * createWrapperFor (TPZInterfaz * interface []
//                                           unsigned number);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZInterfaz::createWrapperFor( TPZInterfaz* interfaz[],
                                            unsigned number )
{
   // A wrapper can be grouped to more than one interface, or one, the
   // which can have multiple virtual channels.
   
   for( int i=0; i<number; i++ )
   {
      if( ! interfaz[i] ) return 0;
   }
   
   unsigned numberCV = (number==1) ? interfaz[0]->numberOfCV() : number;
   
   TPZInterfaz* wrapper = new TPZInterfaz( interfaz[0]->componentId(),
                                           numberCV,
                                           true );
   
   if( number == 1 ) 
   {
      // In this case, it is a wrapper of an interface. Must be considered
      // which may have several virtual channels.
      
      for( int j=1; j<=numberCV; j++ )
         wrapper->setPort(interfaz[0]->getPort(j),j);
   }
   else
   {
      // Here there are several interfaces (with a CV, one assumes) to be
      // join in the wrapper.
      
      for( int j=0; j<number; j++ )
         wrapper->setPort(interfaz[j]->getPort(),j+1);   
   }

   wrapper->setNumberId(interfaz[0]->getNumberId());

   return wrapper;
}


//*************************************************************************
//:
//  f: virtual void SetOwner (TPZComponent * owner);
//
//  d:
//:
//*************************************************************************

void TPZInterfaz :: setOwner(TPZComponent* owner)
{
   if( ! m_Wrapper )
   {
      // Only when the interface is proprietary (not a wrapped) is
      // allowed the change of owner.
      
      Inhereited :: setOwner(owner);
      for( int i=0; i<m_SizeCV; i++ )
      {
         (*(m_Ports+i))->setOwner(this);
      }
   }
}


//*************************************************************************
//:
//  f: virtual Boolean onDataUp (unsigned interface, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: onDataUp(unsigned interfaz, unsigned canal)
{
   Boolean rc = getOwner()->onReadyUp( interfaz, canal );
   return rc;
}


//*************************************************************************
//:
//  f: virtual Boolean onDataDown (unsigned interface, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: onDataDown(unsigned interfaz, unsigned canal)
{
   return getOwner()->onReadyDown( interfaz, canal );
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: onStopUp(unsigned interfaz, unsigned canal)
{
   return getOwner()->onStopUp( interfaz, canal );
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned channel);
//
//  d:
//:
//*************************************************************************

Boolean TPZInterfaz :: onStopDown(unsigned interfaz, unsigned canal)
{
   return getOwner()->onStopDown(interfaz,canal);
}


//*************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//*************************************************************************

void TPZInterfaz :: run(uTIME runTime)
{
   // Does nothing. You need to not be an abstract class.
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZInterfaz :: asString() const
{
   TPZString string = "Interfaz:";
   string += getName();
   return string;
}

//*************************************************************************


// end of file
