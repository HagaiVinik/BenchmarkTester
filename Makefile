include /home/hagai/Downloads/PcapPlusPlus-20.08/mk/platform.mk
include /home/hagai/Downloads/PcapPlusPlus-20.08/mk/PcapPlusPlus.mk


SOURCES := $(wildcard *.cpp)
OBJS_FILENAMES := $(patsubst %.cpp,Obj/%.o,$(SOURCES))

Obj/%.o: %.cpp
	@echo 'Building file: $<'
	@$(CXX) -c $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -fmessage-length=0 -MMD -MP -MF"$(@:Obj/%.o=Obj/%.d)" -MT"$(@:Obj/%.o=Obj/%.d)" -o "$@" "$<"


UNAME := $(shell uname)
CUR_TARGET := $(notdir $(shell pwd))

.SILENT:

all: dependents BenchmarkTester

start:
	@echo '==> Building target: $(CUR_TARGET)'

create-directories:
	@$(MKDIR) -p Obj
	@$(MKDIR) -p Bin

dependents:
# 	@cd $(PCAPPLUSPLUS_HOME) && $(MAKE) libs

BenchmarkTester: start create-directories $(OBJS_FILENAMES)
	@$(CXX) $(PCAPPP_BUILD_FLAGS) $(PCAPPP_LIBS_DIR) -o "./Bin/BenchmarkTester$(BIN_EXT)" $(OBJS_FILENAMES) $(PCAPPP_LIBS)
	#@$cm(PCAPPP_POST_BUILD)
	@echo 'Finished successfully building: $(CUR_TARGET)'
	@echo ' '

clean:
	@$(RM) -rf ./Obj/*
	@$(RM) -rf ./Bin/*F
	@echo 'Clean finished: $(CUR_TARGET)'
