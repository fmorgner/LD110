#include "LD110Analyzer.h"
#include "LD110AnalyzerSettings.h"
#include <AnalyzerChannelData.h>

LD110Analyzer::LD110Analyzer() :	Analyzer(), m_oSettings(new LD110AnalyzerSettings()), m_bSimulationIsInitialized(false)
  {
  SetAnalyzerSettings( m_oSettings.get() );
  }

LD110Analyzer::~LD110Analyzer()
  {
	KillThread();
  }

void LD110Analyzer::WorkerThread()
  {
	m_oResults.reset(new LD110AnalyzerResults(this, m_oSettings.get()));
  m_oResults->AddChannelBubblesWillAppearOn(*m_oSettings->GlobalClockChannelGet());
	SetAnalyzerResults(m_oResults.get());

  m_oBCDChannelDataVector.clear();
  m_oDigitChannelDataVector.clear();

  m_poGlobalClockChannelData = GetAnalyzerChannelData(*m_oSettings->GlobalClockChannelGet());

  for(int nIndex = 0; nIndex < 4; nIndex++)
    {
    m_oBCDChannelDataVector.push_back(GetAnalyzerChannelData(*m_oSettings->BCDChannelGet(nIndex)));
    m_oDigitChannelDataVector.push_back(GetAnalyzerChannelData(*m_oSettings->DigitChannelGet(nIndex)));
    }
  
  AnalyzerChannelData debugData = *m_oDigitChannelDataVector[0];
  
  if(m_oDigitChannelDataVector[0]->GetBitState() == BIT_LOW)
    {
    AdvanceAllChannelsToSample(m_oDigitChannelDataVector[0]->GetSampleOfNextEdge());
    AdvanceAllChannelsEightClockCycles();
    }
   else
    {
    AdvanceAllChannelsToSample(m_oDigitChannelDataVector[0]->GetSampleNumber());
    }

	while(1)
		{
    U8 anDigits[4] = {0,0,0,0};

		Frame oFrame;
		oFrame.mStartingSampleInclusive = m_poGlobalClockChannelData->GetSampleNumber();

    for(int nBCDChannel = 0; nBCDChannel < 4; nBCDChannel++)
      {
      anDigits[0] |= m_oBCDChannelDataVector[nBCDChannel]->GetBitState() << nBCDChannel;
      }

		AdvanceAllChannelsEightClockCycles();
		
    for(int nBCDChannel = 0; nBCDChannel < 4; nBCDChannel++)
      {
      anDigits[2] |= m_oBCDChannelDataVector[nBCDChannel]->GetBitState() << nBCDChannel;
      }
		
		AdvanceAllChannelsEightClockCycles();
		
    for(int nBCDChannel = 0; nBCDChannel < 4; nBCDChannel++)
      {
      anDigits[1] |= m_oBCDChannelDataVector[nBCDChannel]->GetBitState() << nBCDChannel;
      }
		
		AdvanceAllChannelsEightClockCycles();
		
    for(int nBCDChannel = 0; nBCDChannel < 4; nBCDChannel++)
      {
      anDigits[3] |= m_oBCDChannelDataVector[nBCDChannel]->GetBitState() << nBCDChannel;
      }
		
		oFrame.mData1 = 0 + 1000 * anDigits[0] + 100 * anDigits[1] + 10 * anDigits[2] + anDigits[3];
		oFrame.mFlags = 0;
		oFrame.mEndingSampleInclusive   = m_poGlobalClockChannelData->GetSampleNumber();
		
		m_oResults->AddFrame(oFrame);
		m_oResults->CommitResults();

		ReportProgress(oFrame.mEndingSampleInclusive);

		AdvanceAllChannelsEightClockCycles();
		}
  }

void LD110Analyzer::AdvanceAllChannelsToSample(U64 nSampleIndex)
  {
  for(int nIndex = 0; nIndex < 3; nIndex++)
    {
    m_oDigitChannelDataVector[nIndex]->AdvanceToAbsPosition(nSampleIndex);
    m_oBCDChannelDataVector[nIndex]->AdvanceToAbsPosition(nSampleIndex);
    }

  m_poGlobalClockChannelData->AdvanceToAbsPosition(nSampleIndex);
  }

void LD110Analyzer::AdvanceAllChannelsEightClockCycles()
	{
  for(int i = 0; i < 16; i++)
    {
    m_oResults->AddMarker((m_poGlobalClockChannelData->GetSampleNumber() + m_poGlobalClockChannelData->GetSampleOfNextEdge()) / 2, AnalyzerResults::MarkerType::Dot, *m_oSettings->GlobalClockChannelGet());
    m_poGlobalClockChannelData->AdvanceToNextEdge();
    }
	
  AdvanceAllChannelsToSample(m_poGlobalClockChannelData->GetSampleNumber());
	}

#pragma mark - Housekeeping

bool LD110Analyzer::NeedsRerun()
  {
	return false;
  }

U32 LD110Analyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
  {
  if( m_bSimulationIsInitialized == false )
    {
		m_oSimulationDataGenerator.Initialize( GetSimulationSampleRate(), m_oSettings.get() );
		m_bSimulationIsInitialized = true;
    }

	return m_oSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
  }

U32 LD110Analyzer::GetMinimumSampleRateHz()
  {
	return 500000;
  }

const char* LD110Analyzer::GetAnalyzerName() const
  {
	return "LD110 BCD Analyzer";
  }

const char* GetAnalyzerName()
  {
	return "LD110 BCD Analyzer";
  }

Analyzer* CreateAnalyzer()
  {
	return new LD110Analyzer();
  }

void DestroyAnalyzer( Analyzer* analyzer )
  {
	delete analyzer;
  }
