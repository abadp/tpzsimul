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
//    File: TPZEvent.cpp
//
//    Class:  TPZEvent
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#include <TPZEvent.hpp>

//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZEvent);

//*************************************************************************
//:
//  f: inline unsigned source () const;
//  d:
//
//  f: inline unsigned destiny () const;
//  d:
//
//  f: inline unsigned channel () const;
//  d:
//
//  f: inline TPZMessage message () const;
//  d:
//
//  f: inline TPZEVENTTYPE type () const;
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZEvent ();
//
//  d:
//:
//*************************************************************************

TPZEvent :: TPZEvent()
{
   #ifndef NO_TRAZA 
            m_Type=_NullEvent_;
            m_Source=0;
            m_Destiny=0;
            m_Channel=0;
            m_Message=0;
   #endif
}


//*************************************************************************
//:
//  f: TPZEvent (TPZEVENTTYPE type,
//               unsigned src = 0,
//               unsigned dst = 0,
//               unsigned channel = 0,
//               TPZMessage const & msg = TPZMessage ());
//
//  d:
//:
//*************************************************************************

TPZEvent :: TPZEvent( TPZEVENTTYPE type, 
                      unsigned src, 
                      unsigned dst,
                      unsigned channel,
                      TPZMessage* msg)
          : m_Type(type),
            m_Source(src),
            m_Destiny(dst),
            m_Channel(channel),
            m_Message(msg)
{
 
}


//*************************************************************************
//:
//  f: TPZEvent (TPZEVENTTYPE type,
//               TPZMessage const & msg);
//
//  d:
//:
//*************************************************************************

TPZEvent :: TPZEvent( TPZEVENTTYPE type,
                      TPZMessage* msg)
          : m_Type(type),
            m_Source(0),
            m_Destiny(0),
            m_Channel(0),
            m_Message(msg)
{
 
}

//*************************************************************************
//:
//  f: TPZEvent (TPZEVENTTYPE type,
//               TPZMessage const & msg);
//
//  d:
//:
//*************************************************************************

TPZEvent :: TPZEvent( TPZEVENTTYPE type,
                      unsigned src,
                      TPZMessage* msg)
          : m_Type(type),
            m_Source(src),
            m_Destiny(0),
            m_Channel(0),
            m_Message(msg)
{
 
}

//*************************************************************************
//:
//  f: TPZEvent (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

TPZEvent :: TPZEvent(const TPZEvent& event)
          : m_Type(event.m_Type),
            m_Source(event.m_Source),
            m_Destiny(event.m_Destiny),
            m_Channel(event.m_Channel),
            m_Message(event.m_Message)
{

}


//*************************************************************************
//:
//  f: TPZEvent & operator = (const TPZEvent & event);
//
//  d:
//:
//*************************************************************************

TPZEvent& TPZEvent :: operator=(const TPZEvent& event)
{
   m_Type    = event.m_Type;
   m_Source  = event.m_Source;
   m_Destiny = event.m_Destiny;
   m_Channel = event.m_Channel;
   m_Message = event.m_Message;
   
   return *this;
}


//*************************************************************************
//:
//  f: Boolean operator == (const TPZEvent & event) const;
//
//  d:
//:
//*************************************************************************

Boolean TPZEvent :: operator==(const TPZEvent& event) const
{
   if( (m_Type==event.m_Type) &&
       (m_Source == event.m_Source) &&
       (m_Destiny == event.m_Destiny) &&
       (m_Channel == event.m_Channel) && 
       (*m_Message == *event.m_Message) )
   {
      return true;
   }

   return false;
}


//*************************************************************************
//:
//  f: TPZString AsString () const;
//
//  d:
//:
//*************************************************************************

TPZString TPZEvent :: asString() const
{
   TPZString rc = TPZString(m_Type) + ",Src=" + TPZString(m_Source) +
                  ",Dst=" + TPZString(m_Destiny) + ",Chn=" +
                  TPZString(m_Channel);
   return rc;
}

//*************************************************************************

// end of file
