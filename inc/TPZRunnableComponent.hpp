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
//    File: TPZRunnableComponent.hpp
//
//    Class: 
//
//    Inherited from: 
// :
//*************************************************************************
//end of header


#ifndef __TPZRunnableComponent_HPP__
#define __TPZRunnableComponent_HPP__

//************************************************************************

   #include <TPZCompositeComponent.hpp>

   #ifndef __TPZFlow_HPP__
   #include <TPZFlow.hpp>
   #endif
   
   
//************************************************************************

   class TPZRunnableComponent : public TPZCompositeComponent
   {
      typedef TPZCompositeComponent Inhereited;
      

   public:
      TPZRunnableComponent(const TPZComponentId& id);
      virtual ~TPZRunnableComponent();

      virtual void      run(uTIME time=0);
      virtual void      preRun(uTIME time);
      virtual void      postRun(uTIME time);
      virtual void      initialize();
      virtual void      postInitialize();
      
      virtual Boolean   onReadyUp(unsigned interfaz, unsigned cv);
      virtual Boolean   onReadyDown(unsigned interfaz, unsigned cv);
      virtual Boolean   onStopUp(unsigned interfaz, unsigned cv);
      virtual Boolean   onStopDown(unsigned interfaz, unsigned cv);

      virtual TPZString getStatus() const;
      virtual TPZString asString() const;

      void setNumberOfInputs(unsigned inputs)
      { m_Inputs = inputs; }

      void setNumberOfOutputs(unsigned outputs)
      { m_Outputs = outputs; }

      unsigned numberOfInputs()  const
      { return m_Inputs; }
      
      unsigned numberOfOutputs() const
      { return m_Outputs; }

      uTIME getHeaderDelay() const
      { return m_HeaderDelay; }

      uTIME getDataDelay() const
      { return m_DataDelay; }
      
      TPZFlow* getFlowControl() const
      { return m_Flow; }
      
      // Run time information
      DEFINE_RTTI(TPZRunnableComponent);
   
   protected:
      virtual void buildInputInterfaz(unsigned number=1);
      virtual void buildOutputInterfaz(unsigned number=1);
      virtual void buildFlowControl() = 0;
      
      void setFlowControl(TPZFlow* flow)
      { m_Flow = flow; }
      
      void setHeaderDelay(uTIME delay)
      { m_HeaderDelay = delay; }

      void setDataDelay(uTIME delay)
      { m_DataDelay = delay; }
               
   private:
      TPZFlow*      m_Flow;
      unsigned      m_Inputs;
      unsigned      m_Outputs;
      uTIME         m_HeaderDelay;
      uTIME         m_DataDelay;
            
   };

//*************************************************************************

#endif


// end of file
