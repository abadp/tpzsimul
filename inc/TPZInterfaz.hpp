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
//    File: TPZInterfaz.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZInterfaz_HPP__
#define __TPZInterfaz_HPP__

//*************************************************************************

   #include <TPZComponent.hpp>

   #ifndef __TPZPort_HPP__
   #include <TPZPort.hpp>
   #endif
   
//*************************************************************************

   class TPZMessage;
      
//*************************************************************************

   class TPZInterfaz : public TPZComponent
   {
   
      typedef TPZComponent Inhereited;
      
   public:
      TPZInterfaz( const TPZComponentId& id,
                   unsigned numberCV=1,
                   Boolean wrapper=false );
      TPZInterfaz(const TPZInterfaz& interfaz);
      ~TPZInterfaz();
      
      virtual TPZString   asString() const;
            
      virtual Boolean     isReadyActive(unsigned channel=1) const;      
      virtual void        sendStop(unsigned channel=1);
      virtual void        sendStopRightNow(unsigned channel=1);
      virtual void        clearStop(unsigned channel=1);
      virtual void        clearStopRightNow(unsigned channel=1);
      virtual Boolean     isStopActive(unsigned channel=1) const;
      virtual void        sendData(TPZMessage* data, unsigned channel=1);
      virtual void        sendDataRightNow(TPZMessage* data, unsigned channel=1);
      virtual void        clearData(unsigned channel=1);
      virtual void        clearDataRightNow(unsigned channel=1);
      virtual Boolean     getData(TPZMessage** msg, unsigned channel=1) const;      
      
      static TPZInterfaz* createInputInterfaz( unsigned id=1, 
                                               unsigned numberCV=1 );
      static TPZInterfaz* createOutputInterfaz( unsigned id=1,
                                                unsigned numberCV=1 );

      static TPZInterfaz* createWrapperFor( TPZInterfaz* interfaz[], 
                                            unsigned number );
      
      virtual Boolean     onDataUp(unsigned i, unsigned c);
      virtual Boolean     onDataDown(unsigned i, unsigned c);
      virtual Boolean     onStopUp(unsigned i, unsigned c);
      virtual Boolean     onStopDown(unsigned i, unsigned c);
      
      virtual void        setOwner(TPZComponent* owner);
      virtual void        run(uTIME runTime);
      
      unsigned numberOfCV() const
      { return m_SizeCV; }

      TPZPort* getPort(unsigned channel=1) const
      { return ( (m_Ports) ? *(m_Ports+channel-1) : 0 ); }
      
      unsigned getNumberId() const
      { return m_Number; }
      
      void setNumberId(unsigned number)
      { m_Number = number; }

      // Run time information
      DEFINE_RTTI(TPZInterfaz);

      static TPZString sm_InputPrefix;
      static TPZString sm_OutputPrefix;

   protected:

      void setPort(TPZPort* port, unsigned channel=1) {
         if( m_Ports && isChannelValid(channel) )
            *(m_Ports+channel-1) = port;
      }

      Boolean isChannelValid(unsigned ch) const
      //{If (ch> = 1 & & ch <= m_SizeCV) return true; else return false;}
      {return ( ((ch>=1)&&(ch<=m_SizeCV)) ? true : false ); }
      
   private:
      unsigned  m_Number;
      TPZPort** m_Ports;
      unsigned  m_SizeCV;
      Boolean   m_Wrapper;      
   };

//*************************************************************************

   #define forInterfaz(i,size)  for(i=1;i<=size;i++)

//*************************************************************************

#endif


// end of file
