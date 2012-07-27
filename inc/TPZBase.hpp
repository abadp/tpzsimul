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
//    File: TPZBase.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZBase_HPP__
#define __TPZBase_HPP__

//************************************************************************

   #ifndef __TPZRunTimeInformation_HPP__
   #include <TPZRunTimeInformation.hpp>
   #endif

   #ifndef __TPZError_H__
   #include <TPZError.hpp>
   #endif

   #ifndef __TPZString_HPP__
   #include <TPZString.hpp>
   #endif
      
   #include <iostream>
   #include <fstream>
   
//************************************************************************

   typedef unsigned long uTIME;
   typedef unsigned Boolean;
   typedef unsigned int INDEX;
   typedef enum{ _LocalNodeGNP_ = -1,
                 _Unknow_,
                 _Xplus_,
                 _Xminus_,
                 _Yplus_,
                 _Yminus_,
                 _LocalNode_,
                 _Zplus_,
                 _Zminus_,
                 _ABPlus_,
                 _ABMinux_,
	         _ACPlus_,
	         _ACMinus_
               } TPZROUTINGTYPE;

   enum { CHANNEL_X=1, CHANNEL_Y, CHANNEL_Z, CHANNEL_N };
   
//************************************************************************

   #ifdef true
      #undef true
   #endif
   
   #ifdef false
      #undef false
   #endif

   #define true  1
   #define false 0
   
//************************************************************************

   TPZROUTINGTYPE _string2routingType(const TPZString& string);
   TPZString      _routingType2string(TPZROUTINGTYPE type);
   
//************************************************************************

   class TPZBase
   {
   public:
      TPZBase();
      TPZBase(const TPZBase& aBase);
      virtual ~TPZBase();

      static void printError(const TPZString& errorText);
      static void exitProgram(int exitCode = 0);

      static void setErrorLocation(const TPZString& loc)
      { sm_ErrorLocation = loc; }

      static void setErrorText(const TPZString& text)
      { sm_ErrorText = text; }

      static TPZString errorText()
      { return sm_ErrorText; }

      static TPZString errorLocation()
      { return sm_ErrorLocation; }


      // Run time information
      DEFINE_RTTI(TPZBase);

   private:
      static TPZString sm_ErrorLocation;
      static TPZString sm_ErrorText;
   };


//************************************************************************
//
//  Defining a set of macros to facilitate the tracing of errors:
//
//   THROW_ERROR (text, rc): Put "text" in the trace and returns "rc"
//   THROW_BERROR (text): Boolean Return false
//   THROW_PERROR (TEXT): Return NULL Pointer
//   EXIT_PROGRAM_ERROR (c): Exits the program with error code "c"
//
//************************************************************************

   #include <stdlib.h>   // exit ()

   #define THROW_ERROR(text,rc) { TPZBase::setErrorLocation(       \
                                      TPZString(" File=     ") +        \
                                      TPZString(__FILE__)+         \
                                      TPZString("\n Line=     ") +     \
                                      TPZString(__LINE__) );       \
                                  TPZBase::setErrorText(text);     \
                                  return rc; }

   #define THROW_BERROR(text)  THROW_ERROR(text,false)
   #define THROW_PERROR(text)  THROW_ERROR(text,0L)

   #define EXIT_PROGRAM_ERROR(code) { cerr << TPZBase::errorLocation() << endl;  \
                                      cerr << " Text=     " << TPZBase::errorText() << endl;\
                                      exit(code); }

   #define TPZTRAZA(text) cout << " File=     "    <<  \
                                    __FILE__        << \
                                    "\n Line=     " << \
                                    __LINE__        << \
                                    "\n Text=     " << \
                                    text << endl

   #define EXIT_PROGRAM(text) { cerr << " File=     "    << \
                                         __FILE__        << \
                                         "\n Line=     " << \
                                         __LINE__        << \
                                         "\n Text=     " << \
                                         text << endl;      \
                                exit(-1); }
   #define WARNING_MSG(text) { cerr << " File=     "    << \
                                         __FILE__        << \
                                         "\n Line=     " << \
                                         __LINE__        << \
                                         "\n Text=     " << \
                                         text << endl;      \
                                 }
   #define EXIT(text) { cerr << text << endl; \
                        exit(-1); }
#ifndef assert			
   #define assert(code) { if(!code) { \
                          cerr << TPZBase::errorLocation() << endl; \
                          cerr << " Text=     Assertion Failed" << endl; \
                          exit(-1);} }
#endif


   #define MINIMO(x,y) (((x)<(y)) ? (x) : (y) )
   #define MAXIMO(x,y) (((x)>(y)) ? (x) : (y) )


#endif


// end of file
