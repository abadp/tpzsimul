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
//    File: TPZNotifier.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZNotifier_HPP__
#define __TPZNotifier_HPP__

//************************************************************************

   #ifndef __TPZBase_HPP__
   #include <TPZBase.hpp>
   #endif

   #ifndef __TPZSequence_HPP__
   #include <TPZSequence.hpp>
   #endif

//************************************************************************

   class TPZObserver;
  
//************************************************************************

   class TPZNotificationEvent
   {
   public:
      TPZNotificationEvent( int id, 
                            double v1=0.0, 
                            double v2=0.0, 
                            double v3=0.0, 
                            double v4=0.0 )
                   : m_Id(id)
      {
         m_Value[0] = v1;
         m_Value[1] = v2;
         m_Value[2] = v3;
         m_Value[3] = v4;
      }
      
      TPZNotificationEvent(int id, TPZString text)
                   : m_Id(id),
                     m_Text(text)
      {
         m_Value[0] = m_Value[1] = m_Value[2] = m_Value[3] = 0.0;
      }
      
      int id() const
      { return m_Id; }      

      TPZString text() const
      { return m_Text; }
      
      double valueAt(int i) const
      { return ((i>=0) && (i<4)) ? m_Value[i] : 0.0; }      

   private:
      int       m_Id;
      double    m_Value[4];
      TPZString m_Text;
   };

//************************************************************************

   class TPZNotifier : public TPZBase
   {
   public:

      typedef TPZSequence<TPZObserver*> TPZObserversList;

      TPZNotifier();
      ~TPZNotifier();

      virtual void notifyObservers(const TPZNotificationEvent& event);
      virtual void addObserver(TPZObserver* pObserver);
      virtual void removeObserver(const TPZObserver* pObserver);
      virtual void removeAllObservers();

      void enableNotification();
      void disableNotification();

      Boolean isEnableNotification() const
      { return m_EnableNotification; }

      // Run time information
      DEFINE_RTTI(TPZNotifier);

   private:

      TPZObserversList m_ObserverList;
      Boolean m_EnableNotification;
   };

//************************************************************************

   #define EV_LATENCY  1
   #define EV_RESULT   2

//************************************************************************

#endif


// end of file
