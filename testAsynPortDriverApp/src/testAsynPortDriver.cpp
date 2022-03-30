/*
 * testAsynPortDriver.cpp
 *
 * Asyn driver that inherits from the asynPortDriver class to demonstrate its use.
 * It simulates a digital scope looking at a 1kHz 1000-point noisy sine wave.  Controls are
 * provided for time/division, volts/division, volt offset, trigger delay, noise amplitude, update time,
 * and run/stop.
 * Readbacks are provides for the waveform data, min, max and mean values.
 *
 * Author: Mark Rivers
 *
 * Created Feb. 5, 2009
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <iostream>

#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <iocsh.h>

#include "asynPortDriver.h"
#include "asynOctetSyncIO.h"
#include "testAsynPortDriver.h"
#include <epicsExport.h>


#define TIMEOUT 1.0
// static int allVoltsPerDivSelections[NUM_VERT_SELECTIONS]={1,2,5,10};

static const char *driverName="testAsynPortDriver";
/** Constructor for the testAsynPortDriver class.
  * Calls constructor for the asynPortDriver base class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
testAsynPortDriver::testAsynPortDriver(const char *portName, char *ip)
   : asynPortDriver(portName,
                    1, /* maxAddr */
                    asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
                    0, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0) /* Default stack size*/
{
    asynStatus status;
    int i;
    const char *functionName = "testAsynPortDriver";

    eventId_ = epicsEventCreate(epicsEventEmpty);
    createParam(P_RunString,                asynParamInt32,         &P_Run);
    createParam(P_MaxPointsString,          asynParamInt32,         &P_MaxPoints);
    createParam(P_TimePerDivString,         asynParamFloat64,       &P_TimePerDiv);
    createParam(P_TimePerDivSelectString,   asynParamInt32,         &P_TimePerDivSelect);
    createParam(P_VertGainString,           asynParamFloat64,       &P_VertGain);
    createParam(P_VertGainSelectString,     asynParamInt32,         &P_VertGainSelect);
    createParam(P_VoltsPerDivString,        asynParamFloat64,       &P_VoltsPerDiv);
    createParam(P_VoltsPerDivSelectString,  asynParamInt32,         &P_VoltsPerDivSelect);
    createParam(P_VoltOffsetString,         asynParamFloat64,       &P_VoltOffset);
    createParam(P_TriggerDelayString,       asynParamFloat64,       &P_TriggerDelay);
    createParam(P_NoiseAmplitudeString,     asynParamFloat64,       &P_NoiseAmplitude);
    createParam(P_UpdateTimeString,         asynParamFloat64,       &P_UpdateTime);
    createParam(P_WaveformString,           asynParamFloat64Array,  &P_Waveform);
    createParam(P_TimeBaseString,           asynParamFloat64Array,  &P_TimeBase);
    createParam(P_MinValueString,           asynParamFloat64,       &P_MinValue);
    createParam(P_MaxValueString,           asynParamFloat64,       &P_MaxValue);
    createParam(P_MeanValueString,          asynParamFloat64,       &P_MeanValue);

    setIntegerParam(P_MaxPoints,         100);
    setIntegerParam(P_Run,               0);
    setIntegerParam(P_VertGainSelect,    10);
    setDoubleParam(P_VertGain, 10);
    setDoubleParam (P_VoltsPerDiv,       1.0);
    setDoubleParam (P_VoltOffset,        0.0);
    setDoubleParam (P_TriggerDelay,      0.0);
    setDoubleParam (P_TimePerDiv,        0.001);
    setDoubleParam (P_UpdateTime,        0.5);
    setDoubleParam (P_NoiseAmplitude,    0.1);
    setDoubleParam (P_MinValue,          0.0);
    setDoubleParam (P_MaxValue,          0.0);
    setDoubleParam (P_MeanValue,         0.0);

    // pasynOctetSyncIO->connect(ip, 0, &pasynUserEcho, NULL);
}


/** Called when asyn clients call pasynInt32->write().
  * This function sends a signal to the simTask thread if the value of P_Run has changed.
  * For all parameters it sets the value in the parameter library and calls any registered callbacks..
  * \param[in] pasynUser pasynUser structure that encodes the reason and address.
  * \param[in] value Value to write. */
asynStatus testAsynPortDriver::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
    size_t nRead, nActual;
    int eomReason;
    double readValue;
    asynStatus status = asynSuccess;
    const char *paramName;
    const char* functionName = "writeInt32";
    // /* Set the parameter in the parameter library. */
    // // status = (asynStatus) setIntegerParam(function, value);

    // /* Fetch the parameter string name for possible use in debugging */
    // getParamName(function, &paramName);
    // // sprintf(writeBuffer, "%c", "CONFigure:DHI?\n");
    // char writeBuffer[40] = "CONFigure:DHI?\n";
    // char readBuffer[100];
    // pasynOctetSyncIO->writeRead(pasynUserEcho, writeBuffer, strlen(writeBuffer), readBuffer,
    //                                      sizeof(readBuffer), TIMEOUT, &nActual, &nRead, &eomReason);
    // // std::string buff = readBuffer;
    // float res;
    // res = get_channel_val(std::string (readBuffer), 1);
    // std::cout << res << std::endl;
    return status;
}

asynStatus testAsynPortDriver::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    size_t nRead, nActual;
    int eomReason;
    double readValue;
    // char writeBuffer[40] = "CONFigure:DHI?\n";
    // char readBuffer[100];
    // float res;
    // const char *functionName = "readFloat64";
    // std::cout << "Function write "<< std::endl;

    // pasynOctetSyncIO->writeRead(pasynUserEcho, writeBuffer, strlen(writeBuffer), readBuffer,
    //                                      sizeof(readBuffer), TIMEOUT, &nActual, &nRead, &eomReason);
    // res = get_channel_val(std::string (readBuffer), 1);
    // std::cout << res << std::endl;
    // setDoubleParam(P_TimePerDiv, res);
    // status = (asynStatus) callParamCallbacks();
    // std::cout << "Status: "<< status << std::endl;
    // return status;
}

asynStatus testAsynPortDriver::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    size_t nRead, nActual;
    int eomReason;
    double readValue;
    char writeBuffer[40] = "CONFigure:DHI?\n";
    char readBuffer[100];
    float res;
    const char *functionName = "readFloat64";
    std::cout << "Function read "<< std::endl;


    status = (asynStatus) setDoubleParam(function, *value);

    // pasynOctetSyncIO->writeRead(pasynUserEcho, writeBuffer, strlen(writeBuffer), readBuffer,
    //                                      sizeof(readBuffer), TIMEOUT, &nActual, &nRead, &eomReason);
    // res = get_channel_val(std::string (readBuffer), 1);
    // std::cout << res << std::endl;
    // status = (asynStatus) callParamCallbacks();
    // std::cout << "Status: "<< status << std::endl;
    return status;
}

float testAsynPortDriver::get_channel_val(std::string val, int channel)
{
    std::string parse = val;
    int end_line = parse.find("\n", 0) + 1;
    printf("end_line %d \n", end_line);

    int first_val = parse.substr(end_line).find(" ", 0);

    printf("end_line %d \n", first_val);
    std::string final = parse.substr(end_line, first_val);
    std::cout << final << std::endl;
    return std::stof(final);
}

asynStatus testAsynPortDriver::readEnum(asynUser *pasynUser, char *strings[], int values[], int severities[], size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    size_t i;
    // if (function == P_VoltsPerDivSelect) {
    //     for (i=0; ((i<NUM_VERT_SELECTIONS) && (i<nElements)); i++) {
    //         if (strings[i]) free(strings[i]);
    //         strings[i] = epicsStrDup(voltsPerDivStrings_[i]);
    //         values[i] = voltsPerDivValues_[i];
    //         severities[i] = 0;
    //     }
    // }
    // else {
    //     *nIn = 0;
    //     return asynError;
    // }
    // *nIn = i;
    return asynSuccess;
}

asynStatus testAsynPortDriver::readFloat64Array(asynUser *pasynUser, epicsFloat64 *value,
                                         size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    size_t ncopy;
    epicsInt32 itemp;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "readFloat64Array";

    return status;
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxPoints The maximum  number of points in the volt and time arrays */
int testAsynPortDriverConfigure(const char *portName, char *ip)
{
    new testAsynPortDriver(portName, ip);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */

static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg initArg1 = { "max points",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1};
static const iocshFuncDef initFuncDef = {"testAsynPortDriverConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    testAsynPortDriverConfigure(args[0].sval, args[1].sval);
}

void testAsynPortDriverRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(testAsynPortDriverRegister);

}

