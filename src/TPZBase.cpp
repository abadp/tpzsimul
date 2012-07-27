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
//    File: TPZBase.cpp
//
//    Class:  TPZBase
//
//    Inherited from: 
// :
//*************************************************************************
//end of header



#include <TPZBase.hpp>
#include <stdlib.h>

#ifndef __TPZConst_H__
#include <TPZConst.hpp>
#endif

//*************************************************************************

IMPLEMENT_RTTI_BASE(TPZBase);

//*************************************************************************

TPZString TPZBase :: sm_ErrorLocation = "";
TPZString TPZBase :: sm_ErrorText = "" ;

//*************************************************************************
//:
//  f: static inline void setErrorLocation (const TPZString & loc);
//  d:
//
//  f: static inline void setErrorText (const TPZString & text);
//  d:
//
//  f: static inline TPZString ErrorText ();
//  d:
//
//  f: static inline TPZString errorLocation ();
//  d:
//:
//*************************************************************************


//*************************************************************************
//:
//  f: TPZBase ();
//
//  d:
//:
//*************************************************************************

TPZBase :: TPZBase()
{

}


//*************************************************************************
//:
//  f: TPZBase (const TPZBase & Abbas);
//
//  d:
//:
//*************************************************************************

TPZBase :: TPZBase(const TPZBase& aBase)
{

}


//*************************************************************************
//:
//  f: ~ TPZBase ();
//
//  d:
//:
//*************************************************************************

TPZBase :: ~TPZBase()
{

}


//*************************************************************************
//:
//  f: void printError (const TPZString & ErrorText)
//
//  d:
//:
//*************************************************************************

void TPZBase :: printError(const TPZString& errorText)
{
   cout << errorText;
}


//*************************************************************************
//:
//  f: void exitProgram (int exitCode)
//
//  d:
//:
//*************************************************************************

void TPZBase :: exitProgram(int exitCode)
{
   exit(exitCode);
}

//*************************************************************************
//*************************************************************************

TPZROUTINGTYPE _string2routingType(const TPZString& string)
{
   if( string == TPZ_TAG_XPLUS ) return _Xplus_;
   else if( string == TPZ_TAG_XMINUS ) return _Xminus_;
   else if( string == TPZ_TAG_YPLUS )  return _Yplus_;
   else if( string == TPZ_TAG_YMINUS ) return _Yminus_;
   else if( string == TPZ_TAG_ZPLUS )  return _Zplus_;
   else if( string == TPZ_TAG_ZMINUS ) return _Zminus_;
   else if( string == TPZ_TAG_LOCAL )  return _LocalNode_;
   
   return _Unknow_;
}

//*************************************************************************

TPZString _routingType2string(TPZROUTINGTYPE type)
{
   switch(type)
   {
      case _Xplus_ :    return TPZ_TAG_XPLUS;
      case _Xminus_:    return TPZ_TAG_XMINUS;
      case _Yplus_ :    return TPZ_TAG_YPLUS;
      case _Yminus_:    return TPZ_TAG_YMINUS;
      case _Zplus_ :    return TPZ_TAG_ZPLUS;
      case _Zminus_:    return TPZ_TAG_ZMINUS;
      case _LocalNode_: return TPZ_TAG_LOCAL;
   }
   
   return "";
}
  
//*************************************************************************

   
// end of file
