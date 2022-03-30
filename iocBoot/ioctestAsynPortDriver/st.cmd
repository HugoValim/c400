#!/home/hugo/SOL/IOCs/portDriver/bin/linux-x86_64/testAsynPortDriver

#- You may have to change testAsynPortDriver to something else
#- everywhere it appears in this file

< envPaths
epicsEnvSet("ADDR", "1")
epicsEnvSet("TIMEOUT", "1")

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/testAsynPortDriver.dbd"
testAsynPortDriver_registerRecordDeviceDriver pdbbase

## Load record instances
#testAsynPortDriverConfigure("testAPD", 1000)
drvAsynIPPortConfigure("PS", "10.31.54.44:4003")
testAsynPortDriverConfigure("testAPD", "PS")

dbLoadRecords("db/testAsynPortDriver.db","P=testAPD:,R=scope1:,PORT=testAPD,ADDR=0,TIMEOUT=1")
#- Set this to see messages from mySub
#var mySubDebug 1

#- Run this to trace the stages of iocInit
traceIocInit

cd "${TOP}/iocBoot/${IOC}"
iocInit

## Start any sequence programs
#seq sncExample, "user=hugo"
