CFLAGS    :=`root-config --cflags --libs`
SYSLIB    := -lz -l TreePlayer -lMathMore
ROOTCLINGORCINT   := rootcling

SrcSuf    := cpp
ObjSuf    := o
DepSuf    := h
PcmSuf    := _rdict.pcm

OBJS := MBReader.$(ObjSuf) MBReaderData.$(ObjSuf) MBReaderCustomized.$(ObjSuf)
OBJS += HTMicroballRootEvent.$(ObjSuf) MBCalibratedRootEvent.$(ObjSuf)
OBJS += shared.$(ObjSuf)
DEPS := $(_OBJS:.$(ObjSuf)=.$(DepSuf))

ROOTHTMB_HDRS  := HTMicroballRootEvent.h HTMicroballLinkDef.h
ROOTHTMB_DICT  := HTMicroballDict.$(SrcSuf)
ROOTHTMB_DICTH := $(ROOTHTMB_DICT:.$(SrcSuf)=.h)
ROOTHTMB_DICTO := $(ROOTHTMB_DICT:.$(SrcSuf)=.$(ObjSuf))
ROOTHTMB_PCM   := HTMicroballDict$(PcmSuf)

ROOTMBCALIBRATED_HDRS  := MBCalibratedRootEvent.h MBCalibratedLinkDef.h
ROOTMBCALIBRATED_DICT  := MBCalibratedDict.$(SrcSuf)
ROOTMBCALIBRATED_DICTH := $(ROOTMBCALIBRATED_DICT:.$(SrcSuf)=.h)
ROOTMBCALIBRATED_DICTO := $(ROOTMBCALIBRATED_DICT:.$(SrcSuf)=.$(ObjSuf))
ROOTMBCALIBRATED_PCM   := MBCalibratedDict$(PcmSuf)

INCLUDES  := -I./include

PROG      := $(wildcard exec_*.$(SrcSuf))
PROG      := $(patsubst %.$(SrcSuf), %, $(PROG))

CXXFLAGS  += $(INCLUDES) -std=c++11 -fPIC -O3

all: $(PROG)

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(PcmSuf)

$(PROG): $(OBJS) $(ROOTHTMB_DICTO) $(ROOTMBCALIBRATED_DICTO)
	$(CXX) $(CXXFLAGS) -o ${@} ${@}.cpp $^ $(SYSLIB) $(CFLAGS) $(RLIBS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CFLAGS)

$(ROOTHTMB_DICT):
	@echo "Generating dictionary $@..."
	$(ROOTCLINGORCINT) -f $@ -p $(INCLUDES) $(ROOTHTMB_HDRS)

$(ROOTMBCALIBRATED_DICT):
	@echo "Generating dictionary $@..."
	$(ROOTCLINGORCINT) -f $@ -p $(INCLUDES) $(ROOTMBCALIBRATED_HDRS)

.PHONY: clean
clean:
	@$(RM) -f $(OBJS) $(ROOTHTMB_DICT) $(ROOTHTMB_PCM) $(ROOTHTMB_DICTO) $(ROOTHTMB_DICTH) $(ROOTHTMB_DICTH) $(ROOTMBCALIBRATED_DICT) $(ROOTMBCALIBRATED_DICTO) $(ROOTMBCALIBRATED_PCM)
	@echo "$(RM) -f $(OBJS) $(ROOTHTMB_DICT) $(ROOTHTMB_PCM) $(ROOTHTMB_DICTO) $(ROOTHTMB_DICTH) $(ROOTMBCALIBRATED_DICT) $(ROOTMBCALIBRATED_DICTO) $(ROOTMBCALIBRATED_PCM)"
