# -*- mode:python -*-

# Copyright (C) 1998-2011 by Galerna Project, the University of
# Cantabria, Spain.
# 
# This file is part of the TOPAZ network simulator, originallty developed
# at the Unviersity of Cantabria
# 
# TOPAZ shares a large proportion of code with SICOSYS which was 
# developed by V.Puente and J.M.Prellezo
# 
# TOPAZ has been developed by P.Abad, L.G.Menezo, P.Prieto and
# V.Puente
#  
# --------------------------------------------------------------------
# 
# If your use of this software contributes to a published paper, we
# request that you (1) cite our summary paper that appears on our
# website (http://www.atc.unican.es/topaz/) and (2) e-mail a citation
# for your published paper to topaz@atc.unican.es
#  
# If you redistribute derivatives of this software, we request that
# you notify us and either (1) ask people to register with us at our
# website (http://www.atc.unican.es/topaz/) or (2) collect registration
# information and periodically send it to us.
#  
# --------------------------------------------------------------------
# 
# TOPAZ is free software; you can redistribute it and/or
# modify it under the terms of version 2 of the GNU General Public
# License as published by the Free Software Foundation.
# 
# TOPAZ is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with the TOPAZ simulator; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
# 02111-1307, USA
# 
# The GNU General Public License is contained in the file LICENSE.
# 
#  	
# *************************************************************************
# :
# This python script is employed for Topaz and Gem5 integration.
# It is called from Gem5 SConstruct to build Topaz as an external
# library that can be employed by Ruby network components.    
# :
# *************************************************************************
# end of header

import os, subprocess

Import('main')

topaz_files = []
def TopazFile(filename):
    topaz_files.append(File(filename))

TopazFile('src/TPZBase.cpp')
TopazFile('src/TPZAttribute.cpp')
TopazFile('src/TPZBuilder.cpp')
TopazFile('src/TPZComponent.cpp')
TopazFile('src/TPZComponentId.cpp')
TopazFile('src/TPZCompositeComponent.cpp')
TopazFile('src/TPZConnection.cpp')
TopazFile('src/TPZConnectionDelay.cpp')
TopazFile('src/TPZConst.cpp')
TopazFile('src/TPZConsumer.cpp')
TopazFile('src/TPZConsumerFlow.cpp')
TopazFile('src/TPZConsumerFlowBless.cpp')
TopazFile('src/TPZConsumerFlowReactive.cpp')
TopazFile('src/TPZConsumerFlowRespTimeSim.cpp')
TopazFile('src/TPZCrossbar.cpp')
TopazFile('src/TPZCrossbarFlow.cpp')
TopazFile('src/TPZCrossbarFlowCT.cpp')
TopazFile('src/TPZCrossbarFlowCTMux.cpp')
TopazFile('src/TPZCrossbarFlowMux.cpp')
TopazFile('src/TPZCrossbarFlowVC.cpp')
TopazFile('src/TPZCrossbarFlowVCMux.cpp')
TopazFile('src/TPZCrossbarFlowVCMuxOpt.cpp')
TopazFile('src/TPZCrossbarFlowVCOpt.cpp')
TopazFile('src/TPZCrossbarFlowVCTorus.cpp')
TopazFile('src/TPZCrossbarFlowVCTorusMux.cpp')
TopazFile('src/TPZCrossbarFlowWH.cpp')
TopazFile('src/TPZDemultiplexor.cpp')
TopazFile('src/TPZDemultiplexorFlow.cpp')
TopazFile('src/TPZError.cpp')
TopazFile('src/TPZEvent.cpp')
TopazFile('src/TPZFicheroLog.cpp')
TopazFile('src/TPZFifoMemory.cpp')
TopazFile('src/TPZFifoMemoryFlow.cpp')
TopazFile('src/TPZFifoMemoryFlowCT.cpp')
TopazFile('src/TPZFifoMemoryFlowCTMcast.cpp')
TopazFile('src/TPZFifoMemoryFlowDamq.cpp')
TopazFile('src/TPZFifoMemoryFlowWH.cpp')
TopazFile('src/TPZFifoMemoryFlowWHMcast.cpp')
TopazFile('src/TPZFlow.cpp')
TopazFile('src/TPZGlobalData.cpp')
TopazFile('src/TPZIniFile.cpp')
TopazFile('src/TPZInjector.cpp')
TopazFile('src/TPZInjectorFlowCT.cpp')
TopazFile('src/TPZInjectorFlowCTUC.cpp')
TopazFile('src/TPZInjectorFlow.cpp')
TopazFile('src/TPZInjectorFlowUC.cpp')
TopazFile('src/TPZInputStage.cpp')
TopazFile('src/TPZInputStageFlow.cpp')
TopazFile('src/TPZInterfaz.cpp')
TopazFile('src/TPZMain.cpp')
TopazFile('src/TPZMessage.cpp')
TopazFile('src/TPZMultiplexor.cpp')
TopazFile('src/TPZMultiplexorFlow.cpp')
TopazFile('src/TPZMultiportFifo.cpp')
TopazFile('src/TPZMultiportFifoFlow.cpp')
TopazFile('src/TPZMultiportIOFifo.cpp')
TopazFile('src/TPZMultiportIOFifoFlow.cpp')
TopazFile('src/TPZNetwork.cpp')
TopazFile('src/TPZNetworkBuilder.cpp')
TopazFile('src/TPZNetworkMesh.cpp')
TopazFile('src/TPZNetworkMidimew.cpp')
TopazFile('src/TPZNetworkSquareMidimew.cpp')
TopazFile('src/TPZNetworkTorus.cpp')
TopazFile('src/TPZNotifier.cpp')
TopazFile('src/TPZObserver.cpp')
TopazFile('src/TPZOptions.cpp')
TopazFile('src/TPZOutputStage.cpp')
TopazFile('src/TPZOutputStageFlow.cpp')
TopazFile('src/TPZPort.cpp')
TopazFile('src/TPZPortInput.cpp')
TopazFile('src/TPZPortOutput.cpp')
TopazFile('src/TPZPosition.cpp')
TopazFile('src/TPZRouter.cpp')
TopazFile('src/TPZRouterBuilder.cpp')
TopazFile('src/TPZRouting.cpp')
TopazFile('src/TPZRoutingFlow.cpp')
TopazFile('src/TPZRoutingFlowBubble.cpp')
TopazFile('src/TPZRoutingMuxed.cpp')
TopazFile('src/TPZRoutingMuxedFlow.cpp')
TopazFile('src/TPZRoutingMuxedFlowBubble.cpp')
TopazFile('src/TPZRoutingMuxedFlowBubbleAdap.cpp')
TopazFile('src/TPZRunTimeInformation.cpp')
TopazFile('src/TPZRunnableComponent.cpp')
TopazFile('src/TPZSimpleRouter.cpp')
TopazFile('src/TPZSimpleRouterFlow.cpp')
TopazFile('src/TPZSimpleRouterFlowBinoc.cpp')
TopazFile('src/TPZSimpleRouterFlowBless.cpp')
TopazFile('src/TPZSimpleRouterFlowDAMQ.cpp')
TopazFile('src/TPZSimpleRouterFlowFast.cpp')
TopazFile('src/TPZSimpleRouterFlowLigero.cpp')
TopazFile('src/TPZSimpleRouterFlowLigeroMcast.cpp')
TopazFile('src/TPZSimpleRouterFlowTorus.cpp')
TopazFile('src/TPZSimpleRouterFlowWH.cpp')
TopazFile('src/TPZSimulation.cpp')
TopazFile('src/TPZSimulationBuilder.cpp')
TopazFile('src/TPZSimulator.cpp')
TopazFile('src/TPZString.cpp')
TopazFile('src/TPZTag.cpp')
TopazFile('src/TPZThread.cpp')
TopazFile('src/TPZTrafficPattern.cpp')

TPZenv = main.Clone()
TPZenv.Prepend(CPPPATH=Dir('../../ext/TOPAZ/inc'))
TPZenv.Replace(CCFLAGS=['-Wno-deprecated','-O3'])
TPZenv.Append(CPPDEFINES = ['NO_TRAZA'])

# Build topaz as a static library
TPZenv.Library('tpz', [TPZenv.SharedObject(f) for f in topaz_files])

main.Prepend(CPPPATH=Dir('../../ext/TOPAZ/inc'))
main.Append(LIBS=['tpz'])
main.Prepend(LIBPATH=[Dir('.')])

