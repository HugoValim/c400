TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
PROD_IOC += testAsynPortDriver

DBD += testAsynPortDriver.dbd

LIBRARY_IOC += testAsynPortDriverSupport
testAsynPortDriverSupport_SRCS += testAsynPortDriver.cpp
testAsynPortDriverSupport_LIBS += asyn
testAsynPortDriverSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
testAsynPortDriver_SRCS_DEFAULT += testAsynPortDriver_registerRecordDeviceDriver.cpp testAsynPortDriverMain.cpp
testAsynPortDriverVx_SRCS_vxWorks  += -nil-
testAsynPortDriver_LIBS += testAsynPortDriverSupport asyn
testAsynPortDriver_LIBS += $(EPICS_BASE_IOC_LIBS)


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE