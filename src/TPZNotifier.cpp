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
//    File: TPZNotifier.cpp
//
//    Class:  TPZNotifier
//
//    Inherited from:  TPZBase
// :
//*************************************************************************
//end of header



#include <TPZNotifier.hpp>

#ifndef __TPZObserver_HPP__
#include <TPZObserver.hpp>
#endif

//************************************************************************

IMPLEMENT_RTTI_DERIVED(TPZNotifier,TPZBase);

//*************************************************************************
//:
//  f: inline isEnableNotification Boolean () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZNotifier ();
//
//  d:
//:
//*************************************************************************

TPZNotifier :: TPZNotifier()
             : m_EnableNotification(true)
{

}


//*************************************************************************
//:
//  f: ~ TPZNotifier ();
//
//  d:
//:
//*************************************************************************

TPZNotifier :: ~TPZNotifier()
{
//   removeAllObservers ();
}


//*************************************************************************
//:
//  f: void enableNotification ();
//
//  d:
//:
//*************************************************************************

void TPZNotifier :: enableNotification()
{
   m_EnableNotification = true;
}


//*************************************************************************
//:
//  f: void disableNotification ();
//
//  d:
//:
//*************************************************************************

void TPZNotifier :: disableNotification()
{
   m_EnableNotification = false;
}


//*************************************************************************
//:
//  f: void notifyObservers (const TPZNotificationEvent & event);
//
//  d:
//:
//*************************************************************************

void TPZNotifier :: notifyObservers(const TPZNotificationEvent& event)
{
   if( isEnableNotification() )
   {
      TPZObserversList::Cursor cursor(m_ObserverList);
      forCursor(cursor)
      {
         cursor.element()->dispTPZhNotificationEvent(event);
      }
   }
}


//*************************************************************************
//:
//  f: void addObserver (TPZObserver * pObserver)
//
//  d:
//:
//*************************************************************************

void TPZNotifier :: addObserver(TPZObserver* pObserver)
{
   if( ! m_ObserverList.containsElement(pObserver) )
      m_ObserverList.add(pObserver);
}


//*************************************************************************
//:
//  f: void removeObserver (const TPZObserver * pObserver)
//
//  d:
//:
//*************************************************************************

void TPZNotifier :: removeObserver(const TPZObserver* pObserver)
{
   TPZObserversList::Cursor cursor(m_ObserverList);
   forCursor(cursor)
      if( cursor.element() == pObserver )
      {
         // The observer can only be once in the list
         cursor.remove();
         break;
      }
}


//*************************************************************************
//:
//  f: void removeAllObserver ();
//
//  d:
//:
//*************************************************************************

void TPZNotifier :: removeAllObservers()
{
   TPZObserversList::Cursor cursor(m_ObserverList);
   forCursor(cursor)
   {
      cursor.element()->stopHandlingNotificationsFor(this);
      cout << "REMOVE" << endl;
   }
   m_ObserverList.removeAllElements();
}

//*************************************************************************


// end of file
