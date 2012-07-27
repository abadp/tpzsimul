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
//    File: TPZRunnableComponent.cpp
//
//    Class:  TPZRunnableComponent
//
//    Inherited from:  TPZCompositeComponent
// :
//*************************************************************************
//end of header




#include <TPZRunnableComponent.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZFicheroLog_HPP__
#include <TPZFicheroLog.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZRunnableComponent,TPZCompositeComponent);


//*************************************************************************
//:
//  f: virtual void buildFlowControl () = 0;
//  d:
//
//  f: inline void setNumberOfInputs (unsigned inputs);
//  d:
//
//  f: inline void setNumberOfOutputs (unsigned output);
//  d:
//
//  f: inline unsigned numberOfInputs () const;
//  d:
//
//  f: inline unsigned numberOfOutputs () const;
//  d:
//
//  f: inline getHeaderDelay utime () const;
//  d:
//
//  f: inline getDataDelay utime () const;
//  d:
//
//  f: inline void setFlowControl (TPZFlow * flow);
//  d:
//
//  f: void setHeaderDelay (utime delay);
//  d:
//
//  f: void setDataDelay (utime delay);
//  d:
//:
//*************************************************************************

//*************************************************************************
//:
//  f: TPZRunnableComponent (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZRunnableComponent :: TPZRunnableComponent(const TPZComponentId& id)
                      : TPZCompositeComponent(id),
                        m_Inputs(1),
                        m_Outputs(1),
                        m_HeaderDelay(0),
                        m_DataDelay(0),
                        m_Flow(0)
{
   
}


//*************************************************************************
//:
//  f: virtual ~ TPZRunnableComponent ();
//
//  d:
//:
//*************************************************************************

TPZRunnableComponent :: ~TPZRunnableComponent()
{
   delete m_Flow;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRunnableComponent :: onReadyUp(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onReadyUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRunnableComponent :: onReadyDown(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onReadyDown(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRunnableComponent :: onStopUp(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onStopUp(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZRunnableComponent :: onStopDown(unsigned interfaz, unsigned cv)
{
   if( ! getFlowControl() )
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
   return getFlowControl() -> onStopDown(interfaz,cv);
}


//*************************************************************************
//:
//  f: virtual void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: initialize()
{
   if( ! isInitializated() )
   {
      buildInputInterfaz(m_Inputs);
      buildOutputInterfaz(m_Outputs);
      buildFlowControl();
      
   
      Inhereited::initialize();
   }
}


//*************************************************************************
//:
//  f: virtual void postInitialize ();
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: postInitialize()
{
   if( getFlowControl() )
   {
      getFlowControl()->postInitialize();
   }
}


//*************************************************************************
//:
//  f: virtual void run (utime runtime);
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: run(uTIME runTime)
{

   if( m_Flow )
   {
      m_Flow -> run(runTime);
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void preRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: preRun(uTIME time)
{
   if( m_Flow )
   {
      m_Flow -> preRun(time);
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
}

//*************************************************************************
//:
//  f: virtual void postRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: postRun(uTIME time)
{
   if( m_Flow )
   {
      m_Flow -> postRun(time);
   }
   else
   {
      TPZString err;
      err.sprintf(ERR_TPZRUNCO_001, (char*)asString() );
      EXIT_PROGRAM(err);
   }
}


//*************************************************************************
//:
//  f: virtual void buildInputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: buildInputInterfaz(unsigned number)
{
   for( int i=1; i<=number; i++ )
   {
      TPZInterfaz* interfaz = TPZInterfaz::createInputInterfaz(i);
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}


//*************************************************************************
//:
//  f: virtual void buildOutputInterfaz (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

void TPZRunnableComponent :: buildOutputInterfaz(unsigned number)
{
   for( int i=1; i<=number; i++ )   
   {
      TPZInterfaz* interfaz = TPZInterfaz::createOutputInterfaz(i);
      interfaz->setOwner(this);
      addComponent(interfaz);
   }
}

//*************************************************************************
//:
//  f: TPZString getStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRunnableComponent :: getStatus() const
{
   if( m_Flow )
   {
      return m_Flow -> getStatus();
   }
   return "";
}


//*************************************************************************
//:
//  f: virtual TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZRunnableComponent :: asString() const
{
   TPZString string = "TPZRunnableComponent: ";
   string += getName() + "\n ";
   TPZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      string += cursor.element()->asString() + "\n ";
   }
   return string;
}

//*************************************************************************



// end of file
