#*****************************************************************************
#
#    TOPAZ Makefile
#
#*****************************************************************************

PTOPAZ =                     #Enables Parallel topaz (empty to disable)

POOL_OFF      = -DNO_POOL    #Disables static flit pool (empty to enable pool)

TRACE_OFF     = -DNO_TRAZA   #Disables trace debugging (empty to enable trace)
 
DEBUG =                    #Enables debugger ready exec (empty to disable)

#*****************************************************************************

HOST_TYPE := $(shell uname -m)

ifeq ($(shell uname),Darwin)
   CC =g++
   OPTS = -O3 -Wno-deprecated 
   OPTS_DEBUG = -Wno-deprecated  -ggdb -g 
else
ifeq ($(HOST_TYPE),i686)
   CC = g++
   OPTS = -O6 -Wno-deprecated -march=i686
   OPTS_DEBUG = -Wno-deprecated  -ggdb -g 
else
ifeq ($(HOST_TYPE),x86_64)
   CC=g++
   OPTS = -O6  -fPIC -Wno-deprecated -m64  -march=opteron -g3
   OPTS_DEBUG=  -fPIC -ggdb -Wno-deprecated  -g  -m64  -march=opteron
else
ifeq ($(HOST_TYPE),IP35))
   CC= CC
   OPTS = -n32 -LANG:std -r12000 -Ofast=ip27
   OPTS_DEBUG=  -ggdb -g3 
else
ifeq ($(HOST_TYPE),ia64))
    CC=icc
    OPTS = -O3  -IPA 
    OPTS_DEBUG = -c -g3 -wd1224 

else
        CC = /dev/null
endif
endif
endif
endif
endif

ifdef DEBUG
   DEBUG_DIX = _debug
endif



LIBS += -lm
PATH_INC  = ../inc  $(INC1) 
PATH_SRC  = ../src
PATH_LIB  = . -L./$(HOST_TYPE)$(DEBUG_DIX)
PATH_OBJ  = $(HOST_TYPE)$(DEBUG_DIX)
TARGET    = TPZSimul$(DEBUG_DIX) 
TPZ_DLL   = libtpz.so
TPZ_LIB   = libtpz.a
#TRAZA     =

ifdef PTOPAZ
   THREADS = -DPTOPAZ
endif


# *************************************************************************

VPATH = $(PATH_OBJ)
# *************************************************************************

CDEBUG_OPTS = -c $(OPTS_DEBUG) $(WARNING)  $(TRACE_OFF) $(THREADS) $(POOL_OFF) $(UNIROUTER) -I$(PATH_INC)
LDEBUG_OPTS = $(OPTS_DEBUG) -L$(PATH_LIB) -o $(TARGET) 

CFAST_OPTS = -c $(OPTS) $(WARNING)  $(TRACE_OFF) $(THREADS) $(POOL_OFF) $(UNIROUTER) -I$(PATH_INC)
LFAST_OPTS = $(OPTS) -L$(PATH_LIB) -o $(TARGET) 
DFAST_OPTS  = -v -shared -O3 all -o $(TPZ_DLL)
MAKE_DEPEND = $(CC) -P -M -I$(PATH_INC)


# *************************************************************************
ifndef DEBUG
   # Compilacion Optimizada
   COMPILER   = $(CC) $(CFAST_OPTS)
   LINKER     = $(CC) $(LFAST_OPTS)
   DLL_LINKER = $(CC) $(DFAST_OPTS)
else
   # Compilacion para debugger
   COMPILER   = $(CC) $(CDEBUG_OPTS)
   LINKER     = $(CC) $(LDEBUG_OPTS)
   DLL_BUILD  = $(CC) $(DDEBUG_OPTS)
endif

# *************************************************************************
MODULES = TPZAttribute  \
          TPZBase  \
          TPZBuilder  \
	  TPZComponent \
	  TPZComponentId \
	  TPZCompositeComponent \
	  TPZConnection \
	  TPZConnectionDelay \
	  TPZConst \
	  TPZConsumer \
	  TPZConsumerFlow \
	  TPZConsumerFlowBless \
	  TPZConsumerFlowReactive \
	  TPZConsumerFlowRespTimeSim \
	  TPZCrossbar \
	  TPZCrossbarFlow \
          TPZCrossbarFlowCT \
	  TPZCrossbarFlowCTMux \
          TPZCrossbarFlowMux \
	  TPZCrossbarFlowVC \
	  TPZCrossbarFlowVCMux \
	  TPZCrossbarFlowVCMuxOpt \
	  TPZCrossbarFlowVCOpt \
	  TPZCrossbarFlowVCTorus \
	  TPZCrossbarFlowVCTorusMux \
	  TPZCrossbarFlowWH \
	  TPZDemultiplexor \
          TPZDemultiplexorFlow \
          TPZError \
          TPZEvent \
	  TPZFicheroLog \
          TPZFifoMemory  \
          TPZFifoMemoryFlow \
	  TPZFifoMemoryFlowCT \
	  TPZFifoMemoryFlowCTMcast\
	  TPZFifoMemoryFlowDamq\
          TPZFifoMemoryFlowWH \
	  TPZFifoMemoryFlowWHMcast\
	  TPZFlow  \
          TPZGlobalData \
          TPZIniFile   \
          TPZInjector \
	  TPZInjectorFlow \
	  TPZInjectorFlowCT \
	  TPZInjectorFlowCTUC \
	  TPZInjectorFlowUC \
	  TPZInputStage \
	  TPZInputStageFlow \
          TPZInterfaz \
          TPZMessage  \
	  TPZMultiplexor \
          TPZMultiplexorFlow \
	  TPZMultiportFifo \
          TPZMultiportFifoFlow \
          TPZMultiportIOFifo \
          TPZMultiportIOFifoFlow \
          TPZNetwork   \
	  TPZNetworkBuilder \
	  TPZNetworkMesh \
          TPZNetworkMidimew \
	  TPZNetworkSquareMidimew \
	  TPZNetworkTorus \
          TPZNotifier \
          TPZObserver \
          TPZOptions   \
	  TPZOutputStage \
	  TPZOutputStageFlow \
          TPZPort  \
	  TPZPortInput \
	  TPZPortOutput \
          TPZPosition   \
	  TPZRouter \
	  TPZRouterBuilder \
          TPZRouting \
	  TPZRoutingFlow \
	  TPZRoutingFlowBubble \
	  TPZRoutingMuxed \
	  TPZRoutingMuxedFlow \
	  TPZRoutingMuxedFlowBubble \
	  TPZRoutingMuxedFlowBubbleAdap\
          TPZRunTimeInformation  \
          TPZRunnableComponent \
	  TPZSimpleRouter \
          TPZSimpleRouterFlow \
	  TPZSimpleRouterFlowBinoc \
	  TPZSimpleRouterFlowBless \
	  TPZSimpleRouterFlowDAMQ \
	  TPZSimpleRouterFlowFast \
	  TPZSimpleRouterFlowLigero \
	  TPZSimpleRouterFlowLigeroMcast \
	  TPZSimpleRouterFlowTorus \
	  TPZSimpleRouterFlowWH \
	  TPZSimulation \
	  TPZSimulationBuilder \
          TPZSimulator \
	  TPZString \
          TPZTag   \
	  TPZThread \
          TPZTrafficPattern 
            

       

OBJS = $(addsuffix .o, $(MODULES)) 

# *************************************************************************

$(TARGET): TPZMain.o $(TPZ_LIB)
	@echo
	@echo Building executable...
	$(LINKER) $(PATH_OBJ)/TPZMain.o -ltpz $(LIBS) -lpthread
	@echo
	@echo Good luck !
	@echo

$(TPZ_DLL): $(OBJS) TPZMain.o
	@echo 
	@echo Building shared library...
	$(DLL_BUILD) $(addprefix $(PATH_OBJ), $(OBJS))
	@echo Building executable...
	$(LINKER) $(PATH_OBJ)/TPZMain.o $(TPZ_DLL) $(LIBS) -lpthread
	@echo
	@echo Good luck !
	@echo

$(PATH_OBJ)/$(TPZ_LIB): $(OBJS)  
	@echo 
	@echo Building static library...
	rm -f $(PATH_OBJ)/$(TPZ_LIB)
	ar clq $(PATH_OBJ)/$(TPZ_LIB) $(addprefix $(PATH_OBJ)/, $(OBJS))
	ranlib $(PATH_OBJ)/$(TPZ_LIB)
dll: $(TPZ_DLL)

lib: $(TARGET)

$(PATH_OBJ)/Makedepend: $(PATH_OBJ)/created
	$(MAKE_DEPEND) $(addprefix $(PATH_SRC)/,$(addsuffix .cpp,$(MODULES))) > $(PATH_OBJ)/Makedepend
	
# *************************************************************************

include $(PATH_OBJ)/Makedepend

# *************************************************************************

%.o : $(PATH_SRC)/%.cpp $(PATH_OBJ)/created
	$(COMPILER) -o $(PATH_OBJ)/$@ $<

# *************************************************************************

%/created:
	mkdir -p $(dir $@)
	touch $@
	
# *************************************************************************
clean:
	rm -f -r $(PATH_OBJ)
	rm -f $(TARGET)                  
	rm -f $(TPZ_DLL)
	rm -f $(TPZ_LIB)
 

# *************************************************************************


# end of makefile
# DO NOT DELETE
