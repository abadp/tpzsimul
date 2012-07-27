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
//    File: TPZGlobalData.cpp
//
//    Class:  TPZGlobalData
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#include <TPZGlobalData.hpp>


//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZGlobalData);

//*************************************************************************
//:
//  f: TPZGlobalData ();
//
//  d:
//:
//*************************************************************************

TPZGlobalData :: TPZGlobalData()
   : m_cInitializated(false),
     m_cInputsType(0),
     m_cLocalPorts(0),
     m_cOutputMux(0),
     m_cOutputsType(0),
     m_rBufferControl(TPZString("")),
     m_rBufferSize(0),
     m_rInitializated(false),
     m_rInputsType(0),
     m_rOutputsType(0),
     m_rRoutingControl(TPZString("")),
     m_sInitializated(false),
     m_sInputsType(0),
     m_sHDInputsType(0),
     m_sLocalPorts(0),
     m_sOutputMux(0),
     m_sOutputsType(0),
     m_sHDOutputsType(0)     
{
}


//*************************************************************************
//:
//  f: ~ TPZGlobalData ();
//
//  d:
//:
//*************************************************************************

TPZGlobalData :: ~TPZGlobalData()
{
   delete m_rInputsType;
   delete m_rOutputsType;
   delete m_cInputsType;
   delete m_cOutputsType;
}

// end of file
