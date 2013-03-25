#ifndef LD110_ANALYZER_H
#define LD110_ANALYZER_H

#include <Analyzer.h>
#include "LD110AnalyzerResults.h"
#include "LD110SimulationDataGenerator.h"

#include <vector>
#include <memory>

class LD110AnalyzerSettings;

class ANALYZER_EXPORT LD110Analyzer : public Analyzer
  {
  public:
    LD110Analyzer();
    virtual ~LD110Analyzer();
    virtual void WorkerThread();

    virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
    virtual U32 GetMinimumSampleRateHz();

    virtual const char* GetAnalyzerName() const;
    virtual bool NeedsRerun();

  protected:
    std::unique_ptr<LD110AnalyzerSettings> m_oSettings;
    std::unique_ptr<LD110AnalyzerResults>  m_oResults;
    
    std::vector<AnalyzerChannelData*> m_oBCDChannelDataVector;
    std::vector<AnalyzerChannelData*> m_oDigitChannelDataVector;
    
    AnalyzerChannelData* m_poGlobalClockChannelData;
    LD110SimulationDataGenerator m_oSimulationDataGenerator;
    
    bool m_bSimulationIsInitialized;
    
  protected:
    void AdvanceAllChannelsToSample(U64 nSampleIndex);
		void AdvanceAllChannelsEightClockCycles();

};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer*   __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void        __cdecl DestroyAnalyzer(Analyzer* analyzer);

#endif //LD110_ANALYZER_H
