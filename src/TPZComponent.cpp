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
//    File: TPZComponent.cpp
//
//    Class:  TPZComponent
//
//    Inherited from:  TPZNotifier
// :
//*************************************************************************
//end of header



#include <TPZComponent.hpp>

#ifndef __TPZConnection_HPP__
#include <TPZConnection.hpp>
#endif

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

#ifndef __TPZBuilder_HPP__
#include <TPZBuilder.hpp>
#endif

//*************************************************************************

TPZBuilder*   TPZComponent::routerBuilder     = 0;
TPZBuilder*   TPZComponent::networkBuilder    = 0;
TPZBuilder*   TPZComponent::simulationBuilder = 0;

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZComponent,TPZNotifier);

//*************************************************************************
//:
//  f: virtual void run (utime runtime = 0) = 0;
//  d:
//
//  f: inline TPZComponentId ComponentId () const;
//  d:
//
//  f: inline virtual void SetOwner (TPZComponent * owner);
//  d:
//
//  f: inline getOwner TPZComponent * () const;
//  d:
//
//  f: inline virtual void setRouter (TPZComponent * router);
//  d:
//
//  f: inline getRoute TPZComponent * () const;
//  d:
//
//  f: inline TPZROUTINGTYPE getRoutingDirection () const;
//  d:
//
//  f: inline void setRoutingType (TPZROUTINGTYPE type);
//  d:
//
//  f: inline TPZString getName () const;
//  d:
//
//  f: inline void setName (const TPZString & aName);
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZComponent (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponent :: TPZComponent(const TPZComponentId& id)
              : m_Identifier(id),
                m_Owner(0),
                m_Router(0),
                m_Initializated(false),
                m_Terminated(false),
                m_DeleteConnectionSet(true),
                m_Simulation(0)
{

}


//*************************************************************************
//:
//  f: TPZComponent (const TPZComponent & component);
//
//  d:
//:
//*************************************************************************

TPZComponent :: TPZComponent(const TPZComponent& component)
              : m_Identifier(component.m_Identifier),
                m_Owner(component.m_Owner),
                m_Router(component.m_Router),
                m_DeleteConnectionSet(component.m_DeleteConnectionSet),
                m_Simulation(component.m_Simulation)
{

}

//*************************************************************************
//:
//  f: virtual ~ TPZComponent ();
//
//  d:
//:
//*************************************************************************

TPZComponent :: ~TPZComponent()
{
   if( m_DeleteConnectionSet )
   {
      TPZConnectionSet::Cursor cursor(m_ConnectionSet);
      forCursor(cursor)
      { 
         delete cursor.element();
      }
   }
}


//*************************************************************************
//:
//  f: void initialize ();
//
//  d:
//:
//*************************************************************************

void TPZComponent :: initialize()
{
   // Initialization is not required at this point.
   setInitializated(true);
}


//*************************************************************************
//:
//  f: void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZComponent :: terminate()
{
   // Initialization is not required at this point.
   setTerminated(true);
}

//*************************************************************************
//:
//  f: void postInitialize ();
//
//  d:
//:
//*************************************************************************

void TPZComponent :: postInitialize()
{
   // No need for post-initialization at this point.
}


//*************************************************************************
//:
//  f: void postRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZComponent :: preRun(uTIME time)
{
   // There are certain components that require actions in
   // instant before making the run. These components must
   // pre-register through "addPreRunComponent ()"
}

//*************************************************************************
//:
//  f: void postRun (utime time);
//
//  d:
//:
//*************************************************************************

void TPZComponent :: postRun(uTIME time)
{
   // There are certain components that require actions in
   // second half period of clock. These components must be registered
   // advance through "addPostRunComponent ()"
}


//*************************************************************************
//:
//  f: TPZComponent * componentWithId (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZComponent :: componentWithId(const TPZComponentId& id)
{
   if( componentId() == id )
      return this;
   return 0;
}


//*************************************************************************
//:
//  f: TPZComponent * componentWithIdLike (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZComponent :: componentWithIdLike(const TPZComponentId& id)
{
   if( componentId().isLike(id) )
      return this;
   return 0;
}


//*************************************************************************
//:
//  f: TPZComponent * componentWithName (TPZString const & name);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZComponent :: componentWithName(const TPZString& name)
{
   if( componentId().name() == name )
      return this;
   return 0;
}

//*************************************************************************
//:
//  f: void addComponent (TPZComponent * AComponent)
//
//  d:
//:
//*************************************************************************

void TPZComponent :: addComponent(TPZComponent* aComponent)
{
   // By default, does nothing
}


//*************************************************************************
//:
//  f: void addPreRunComponent (TPZComponent * AComponent)
//
//  d:
//:
//*************************************************************************

void TPZComponent :: addPreRunComponent(TPZComponent* aComponent)
{
   // By default, does nothing
}


//*************************************************************************
//:
//  f: void addPostRunComponent (TPZComponent * AComponent)
//
//  d:
//:
//*************************************************************************

void TPZComponent :: addPostRunComponent(TPZComponent* aComponent)
{
   // By default, does nothing
}


//*************************************************************************
//:
//  f: void removeAllComponents ();
//
//  d:
//:
//*************************************************************************

void TPZComponent :: removeAllComponents()
{
   // By default, does nothing
}

//*************************************************************************
//:
//  f: virtual getInputInterfaz TPZInterfaz * (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZComponent :: getInputInterfaz(unsigned number)
{
   return 0;
}


//*************************************************************************
//:
//  f: virtual getOutputInterfaz TPZInterfaz * (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZComponent :: getOutputInterfaz(unsigned number)
{
   return 0;
}


//*************************************************************************
//:
//  f: void AddConnection (TPZConnection * connection);
//
//  d:
//:
//*************************************************************************

void TPZComponent :: addConnection(TPZConnection* connection)
{
   //if( ! m_ConnectionSet.containsElement(connection) ) //This was here for no reason
   // and delays network build enormously in big systems. 
   m_ConnectionSet.add(connection);
}
      
      
//*************************************************************************
//:
//  f: virtual Boolean onDataUp (unsigned interface, cv unsigned);
//
//  d:
//:
//*************************************************************************

Boolean TPZComponent :: onDataUp(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onDataDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZComponent :: onDataDown(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZComponent :: onReadyUp(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onReadyDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZComponent :: onReadyDown(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopUp (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZComponent :: onStopUp(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: virtual Boolean onStopDown (unsigned interface, unsigned cv)
//
//  d:
//:
//*************************************************************************

Boolean TPZComponent :: onStopDown(unsigned interfaz, unsigned cv)
{
   return false;
}


//*************************************************************************
//:
//  f: TPZString getStatus () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZComponent :: getStatus() const
{
   return "";
}


//*************************************************************************
//:
//  f: TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZComponent :: asString() const
{
   return "TPZComponent";
}


//*************************************************************************


// end of file
