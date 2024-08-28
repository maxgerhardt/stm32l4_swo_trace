#include <Arduino.h>
// conflicts with COMP1/2 macros defined in headers below
#undef COMP2
#undef COMP1
#include <orbcode/trace/doc.h>
#include <orbcode/trace/dwt.h>
#include <orbcode/trace/itm.h>
#include <orbcode/trace/tpiu.h>

void vendor_specific_config() {
    __GPIOB_CLK_ENABLE(); //SWO is on PB3 
    DBGMCU->CR |= DBGMCU_CR_TRACE_IOEN; // trace on
    // trace mode set to 0 for asynch
    DBGMCU->CR &= ~DBGMCU_CR_TRACE_MODE;
}

void setup_trace() {
    vendor_specific_config();
    TpiuOptions tpiu;
    tpiu.FormattingEnabled = false;
    tpiu.Protocol = TpiuProtocolSwoUart; // 0x2
    tpiu.SwoPrescaler = 40; // 80/40 = 2.00 MHz // try to run at full 80MHz speed?
    tpiu.TracePortWidth = 1; // not used (parallel port)
    ITMOptions itm;
    itm.EnabledStimulusPorts = 0xffffffff;
    itm.EnableLocalTimestamp = false;
    itm.EnableSyncPacket = false; // needs or doesn't need "-n" switch for orbtools
    itm.ForwardDWT = true; // must be ON for PC sampling
    itm.GlobalTimestampFrequency = ITMGlobalTimestampFrequencyDisabled;
    itm.LocalTimestampPrescaler = ITMLocalTimestampPrescalerDivideBy64;
    itm.TraceBusID = 1; // irrelevant, only used when formatter is on
    DWTOptions dwt;
    dwt.CPICounterEvent = false;
    dwt.CycleTap = DWTCycleTap6; // every 2^6 = 64 cycles, measure PC
    //dwt.CycleTap = DWTCycleTap10; // 2^10 = 1024 cycles
    dwt.ExceptionOverheadCounterEvent = false;
    dwt.ExceptionTrace = true; // trace interrupts
    dwt.FoldedInstructionCounterEvent = false;
    dwt.LSUCounterEvent = false;
    dwt.PCSampling = true; // we want to enable PC sampling!!
    dwt.SamplingPrescaler = 1; // measure every dwt.CycleTap * 1 cycles
    dwt.SleepCounterEvent = false;
    dwt.SyncTap = DWTSyncTap26; // sync every 2^26 clocks

    // Apply configuration as defined in structs
    TpiuSetup(&tpiu);
    ITMSetup(&itm);
    DWTSetup(&dwt);
}