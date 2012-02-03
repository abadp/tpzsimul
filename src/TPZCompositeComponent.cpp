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
//    File: TPZCompositeComponent.cpp
//
//    Class:  TPZCompositeComponent
//
//    Inherited from:  TPZComponent
// :
//*************************************************************************
//end of header



#include <TPZCompositeComponent.hpp>

#ifndef __TPZInterfaz_HPP__
#include <TPZInterfaz.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZCompositeComponent,TPZComponent);

//*************************************************************************
//:
//  f: inline void setAutoDestroyChildren (Boolean destroy = true);
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZCompositeComponent (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZCompositeComponent :: TPZCompositeComponent(const TPZComponentId& id)
                       : TPZComponent(id),
                         m_DestroyChildren(true)
{

}


//*************************************************************************
//:
//  f: ~ TPZCompositeComponent ();
//
//  d:
//:
//*************************************************************************

TPZCompositeComponent :: ~TPZCompositeComponent()
{

}


//*************************************************************************
//:
//  f: virtual void terminate ();
//
//  d:
//:
//*************************************************************************

void TPZCompositeComponent :: terminate()
{
   if(m_DestroyChildren) removeAllComponents();
      
   Inhereited::terminate();
}


//*************************************************************************
//:
//  f: virtual TPZComponent * componentWithId (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZCompositeComponent :: componentWithId(const TPZComponentId& id)
{
   TPZComponent* comp = Inhereited::componentWithId(id);
   if(comp) return comp;

   TPZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      if( cursor.element()->componentId() == id )
         return cursor.element();
   }
   return 0;
}


//*************************************************************************
//:
//  f: virtual TPZComponent * componentWithIdLike (const TPZComponentId & id);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZCompositeComponent :: componentWithIdLike(const TPZComponentId& id)
{
   TPZComponent* comp = Inhereited::componentWithIdLike(id);
   if(comp) return comp;

   TPZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      if( cursor.element()->componentId().isLike(id) )
         return cursor.element();
   }
   return 0;
}


//*************************************************************************
//:
//  f: virtual TPZComponent * componentWithName (TPZString const & name);
//
//  d:
//:
//*************************************************************************

TPZComponent* TPZCompositeComponent :: componentWithName(const TPZString& name)
{
   TPZComponent* comp = Inhereited::componentWithName(name);
   if(comp) return comp;

   TPZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      if( cursor.element()->componentId().name() == name )
         return cursor.element();
   }
   
   return 0;
}


//*************************************************************************
//:
//  f: virtual void addComponent (TPZComponent * AComponent)
//
//  d:
//:
//*************************************************************************

void TPZCompositeComponent :: addComponent(TPZComponent* aComponent)
{
   // Only added if it is in the collection.
   if( aComponent && !m_ComponentSet.containsElement(aComponent) )
   {
      m_ComponentSet.add(aComponent);
      aComponent->setOwner(this);
   }
   
}


//*************************************************************************
//:
//  f: virtual void addPreRunComponent (TPZComponent * AComponent)
//
//  d:
//:
//*************************************************************************

void TPZCompositeComponent :: addPreRunComponent(TPZComponent* aComponent)
{
   // Only added if it is in the collection. This collection is not
   // owner of its elements, but only used to
   // refer to them, and therefore not its responsibility
   // the destruction of the components that comprise it.
   
   if( aComponent && !m_PreRunComponentSet.containsElement(aComponent) )
   {
      m_PreRunComponentSet.add(aComponent);
   }
}

//*************************************************************************
//:
//  f: virtual void addPostRunComponent (TPZComponent * AComponent)
//
//  d:
//:
//*************************************************************************

void TPZCompositeComponent :: addPostRunComponent(TPZComponent* aComponent)
{
   // Only added if it is in the collection. This collection is not
   // owner of its elements, but only used for
   // refer to them, and therefore not its responsibility
   // the destruction of the components that comprise it.
   
   if( aComponent && !m_PostRunComponentSet.containsElement(aComponent) )
   {
      m_PostRunComponentSet.add(aComponent);
   }
}


//*************************************************************************
//:
//  f: virtual void removeAllComponents ();
//
//  d:
//:
//*************************************************************************

void TPZCompositeComponent :: removeAllComponents()
{
   TPZComponentSet::Cursor cursor(m_ComponentSet);
   m_ComponentSet.removeAllElements();
   m_PreRunComponentSet.removeAllElements();
   m_PostRunComponentSet.removeAllElements();
   
}


//*************************************************************************
//:
//  f: virtual getInputInterfaz TPZInterfaz * (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZCompositeComponent :: getInputInterfaz(unsigned number)
{
   TPZComponentId idInterfaz( TPZComponentId::InputInterfaz, 
                              _Unknow_,
                              TPZString("I") + TPZString(number) );
                          
   TPZComponent* componentFound = componentWithIdLike(idInterfaz);                           
   TPZInterfaz* interfaz = POINTERCAST(TPZInterfaz,componentFound);
   return interfaz;
}


//*************************************************************************
//:
//  f: virtual getOutputInterfaz TPZInterfaz * (unsigned number = 1);
//
//  d:
//:
//*************************************************************************

TPZInterfaz* TPZCompositeComponent :: getOutputInterfaz(unsigned number)
{
   TPZComponentId idInterfaz( TPZComponentId::OutputInterfaz, 
                              _Unknow_,
                              TPZString("O") + TPZString(number) );
                              
   TPZComponent* componentFound = componentWithIdLike(idInterfaz);                           
   TPZInterfaz* interfaz = POINTERCAST(TPZInterfaz,componentFound);
   return interfaz;
}


//*************************************************************************
//:
//  f: numberOfComponents unsigned () const;
//
//  d:
//:
//*************************************************************************

unsigned TPZCompositeComponent :: numberOfComponents() const
{
   return m_ComponentSet.numberOfElements();
}


//*************************************************************************
//:
//  f: virtual void setSimulation (TPZComponent * simulation);
//
//  d:
//:
//*************************************************************************

void TPZCompositeComponent :: setSimulation(TPZComponent* simulation)
{
   TPZComponentSet::Cursor cursor(m_ComponentSet);
   forCursor(cursor)
   {
      cursor.element()->setSimulation(simulation);
   }
   
   Inhereited::setSimulation(simulation);
}
      
//*************************************************************************
//:
//  f: TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZCompositeComponent :: asString() const
{
   TPZString string = "TPZCompositeComponent: ";
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
