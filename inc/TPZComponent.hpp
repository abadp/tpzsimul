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
//    File: TPZComponent.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZComponent_HPP__
#define __TPZComponent_HPP__

//************************************************************************

   #ifndef __TPZNotifier_HPP__
   #include <TPZNotifier.hpp>
   #endif

   #ifndef __TPZComponentId_HPP__
   #include <TPZComponentId.hpp>
   #endif

   #ifndef __TPZBase_HPP__
   #include <TPZBase.hpp>
   #endif
      
//*************************************************************************

   class TPZMessage;
   class TPZConnection;
   class TPZInterfaz;
   class TPZBuilder;

//*************************************************************************

   class TPZComponent : public TPZNotifier
   {      
   public:   
      typedef TPZSequence<TPZConnection*> TPZConnectionSet;

      TPZComponent(const TPZComponentId& id);
      TPZComponent(const TPZComponent& component);
      virtual ~TPZComponent();

      virtual void          run(uTIME runTime=0) = 0;
      virtual void          preRun(uTIME runTime);
      virtual void          postRun(uTIME runTime);
      virtual void          initialize();
      virtual void          terminate();
      virtual void          postInitialize();
      
      virtual TPZComponent* componentWithId(const TPZComponentId& id);
      virtual TPZComponent* componentWithIdLike(const TPZComponentId& id);
      virtual TPZComponent* componentWithName(const TPZString& name);
      
      virtual void          addComponent(TPZComponent* aComponent);
      virtual void          addPreRunComponent(TPZComponent* aComponent);
      virtual void          addPostRunComponent(TPZComponent* aComponent);
      virtual void          removeAllComponents();
      virtual TPZInterfaz*  getInputInterfaz(unsigned number = 1);
      virtual TPZInterfaz*  getOutputInterfaz(unsigned number = 1);
      
      void    addConnection(TPZConnection* connection);
      
      virtual Boolean  onDataUp(unsigned interfaz, unsigned cv);
      virtual Boolean  onDataDown(unsigned interfaz, unsigned cv);
      virtual Boolean  onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean  onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean  onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean  onStopDown(unsigned interfaz, unsigned cv);
      virtual TPZString getStatus() const;      
      virtual TPZString asString() const;

      TPZComponentId& componentId() const
      { return (TPZComponentId&)m_Identifier; }

      virtual void setOwner(TPZComponent* owner)
      { m_Owner = owner; }

      TPZComponent* getOwner() const
      { return m_Owner; }

      virtual void setRouter(TPZComponent* router)
      { m_Router = router; }

      TPZComponent* getRouter() const
      { return m_Router; }

      TPZROUTINGTYPE getRoutingDirection() const
      { return m_Identifier.componentType(); }

      TPZROUTINGTYPE getDirection() const
      { return m_Identifier.componentType(); }
            
      TPZString getPortName() const
      { return  m_Identifier.getPortName(); }

      unsigned getChannel() const
      { return m_Identifier.channel(); }
      
      void setRoutingType(TPZROUTINGTYPE type)
      { m_Identifier.setComponentType(type); }

      TPZString getName() const
      { return m_Identifier.name(); }

      void setName(const TPZString& aName)
      { m_Identifier.setName(aName); }

      void setChannel(unsigned number)
      { m_Identifier.setChannel(number); }
      
      // Run time information
      DEFINE_RTTI(TPZComponent);

   protected:
      Boolean isInitializated() const
      { return m_Initializated; }

      Boolean isTerminated() const
      { return m_Terminated; }

      void setInitializated(Boolean val=true)
      { m_Initializated = val; }

      void setTerminated(Boolean val=true)
      { m_Terminated = val; }

      void setAutoDeleteConnectionSet(Boolean val)
      { m_DeleteConnectionSet = val; }

   public:
      TPZComponent* getSimulation() const
      { return m_Simulation; }

      virtual void setSimulation(TPZComponent* simulation)
      { m_Simulation = simulation; }
      
   protected:
      TPZConnectionSet   m_ConnectionSet;
      TPZComponentId     m_Identifier;
      TPZComponent*      m_Owner;
      TPZComponent*      m_Router;
      Boolean            m_Initializated;
      Boolean            m_Terminated;
      Boolean            m_DeleteConnectionSet;
         
   public:
      static TPZBuilder* routerBuilder;
      static TPZBuilder* networkBuilder;
      static TPZBuilder* simulationBuilder;
   
   private:
      TPZComponent* m_Simulation;

   };


//*************************************************************************

   #define TPZSIMULATION ((TPZSimulation*)(getComponent().getSimulation()))
   #define TPZNETWORK    (TPZSIMULATION->getNetwork())
   #define TPZROUTER     ((TPZRouter*)getOwner())
   
//*************************************************************************

#endif


// end of file
