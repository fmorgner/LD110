#include "LD110AnalyzerSettings.h"

LD110AnalyzerSettings::LD110AnalyzerSettings() : m_oGlobalClockChannel(UNDEFINED_CHANNEL)
  {
  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oBCDChannelInterfaceVector[nIndex].reset(new AnalyzerSettingInterfaceChannel());
    m_oBCDChannelVector.push_back(Channel(UNDEFINED_CHANNEL));
    
    m_oDigitChannelInterfaceVector[nIndex].reset(new AnalyzerSettingInterfaceChannel());
    m_oDigitChannelVector.push_back(Channel(UNDEFINED_CHANNEL));
    }

  m_oGlobalClockChannelInterface.reset(new AnalyzerSettingInterfaceChannel());

  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oTitle.str("BCD Bit ");
    m_oTitle << (nIndex + 1);
    
    m_oTooltip.str("BCD Bit ");
    m_oTooltip << nIndex + 1 << " (Pin " << 16 - nIndex << ")";
    
    m_oBCDChannelInterfaceVector[nIndex]->SetTitleAndTooltip(m_oTitle.str().c_str(), m_oTooltip.str().c_str());
    m_oBCDChannelInterfaceVector[nIndex]->SetChannel(m_oBCDChannelVector[nIndex]);

    AddInterface(m_oBCDChannelInterfaceVector[nIndex].get());

    m_oTitle.str("Digit ");
    m_oTitle << nIndex + 1 << " clock";
    
    m_oTooltip.str("The clock for digit ");
    m_oTooltip << nIndex + 1 << " (Pin " << 1 + nIndex << ")";
    
    m_oDigitChannelInterfaceVector[nIndex]->SetTitleAndTooltip(m_oTitle.str().c_str(), m_oTooltip.str().c_str());
    m_oDigitChannelInterfaceVector[nIndex]->SetChannel(m_oDigitChannelVector[nIndex]);

    AddInterface(m_oDigitChannelInterfaceVector[nIndex].get());
    }
  
  m_oGlobalClockChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
  m_oGlobalClockChannelInterface->SetTitleAndTooltip("Global clock", "The clock for LD110 IC");
  m_oGlobalClockChannelInterface->SetChannel(m_oGlobalClockChannel);

  AddInterface(m_oGlobalClockChannelInterface.get());

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();

  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oTitle.str("BCD bit ");
    m_oTitle << nIndex + 1;
    AddChannel(m_oBCDChannelVector[nIndex], m_oTitle.str().c_str(), false);

    m_oTitle.str("Digit ");
    m_oTitle << nIndex + 1 << " clock";
    AddChannel(m_oDigitChannelVector[nIndex], m_oTitle.str().c_str(), false);
    }

	AddChannel(m_oGlobalClockChannel, "Global IC clock", false);
  }

LD110AnalyzerSettings::~LD110AnalyzerSettings()
  {
  }

bool LD110AnalyzerSettings::SetSettingsFromInterfaces()
  {
  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oBCDChannelVector[nIndex]   = m_oBCDChannelInterfaceVector[nIndex]->GetChannel();
    m_oDigitChannelVector[nIndex] = m_oDigitChannelInterfaceVector[nIndex]->GetChannel();
    }

  m_oGlobalClockChannel = m_oGlobalClockChannelInterface->GetChannel();
  
	ClearChannels();

  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oTitle.str("BCD bit ");
    m_oTitle << nIndex + 1;
    AddChannel(m_oBCDChannelVector[nIndex], m_oTitle.str().c_str(), true);

    m_oTitle.str("Digit ");
    m_oTitle << nIndex + 1 << " clock";
    AddChannel(m_oDigitChannelVector[nIndex], m_oTitle.str().c_str(), true);
    }

	AddChannel(m_oGlobalClockChannel, "Global IC clock", true);

	return true;
  }

void LD110AnalyzerSettings::UpdateInterfacesFromSettings()
  {
  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oBCDChannelInterfaceVector[nIndex]->SetChannel(m_oBCDChannelVector[nIndex]);
    m_oDigitChannelInterfaceVector[nIndex]->SetChannel(m_oDigitChannelVector[nIndex]);
    }

  m_oGlobalClockChannelInterface->SetChannel(m_oGlobalClockChannel);
  }

void LD110AnalyzerSettings::LoadSettings( const char* settings )
  {
	m_oArchive.SetString( settings );

  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oArchive >> m_oBCDChannelVector[nIndex];
    m_oArchive >> m_oDigitChannelVector[nIndex];
    }

  m_oArchive >> m_oGlobalClockChannel;

	ClearChannels();

  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oTitle.str("BCD bit ");
    m_oTitle << nIndex + 1;
    AddChannel(m_oBCDChannelVector[nIndex], m_oTitle.str().c_str(), true);

    m_oTitle.str("Digit ");
    m_oTitle << nIndex + 1 << " clock";
    AddChannel(m_oDigitChannelVector[nIndex], m_oTitle.str().c_str(), true);
    }

	AddChannel(m_oGlobalClockChannel, "Global IC clock", true);

	UpdateInterfacesFromSettings();
  }

const char* LD110AnalyzerSettings::SaveSettings()
  {
  for(int nIndex = 0; nIndex < m_nBCDAndDigitChannelCount; nIndex++)
    {
    m_oArchive << m_oBCDChannelVector[nIndex];
    m_oArchive << m_oDigitChannelVector[nIndex];
    }

  m_oArchive << m_oGlobalClockChannel;

	return SetReturnString( m_oArchive.GetString() );
  }
