#ifndef LD110_ANALYZER_SETTINGS
#define LD110_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>
#include <AnalyzerHelpers.h>

#include <vector>
#include <sstream>

class LD110AnalyzerSettings : public AnalyzerSettings
  {
  public:
    LD110AnalyzerSettings();
    virtual ~LD110AnalyzerSettings();

    virtual bool SetSettingsFromInterfaces();
    void UpdateInterfacesFromSettings();
    virtual void LoadSettings( const char* settings );
    virtual const char* SaveSettings();
    
    Channel* BCDChannelGet(int nIndex) { return &m_oBCDChannelVector[nIndex]; }
    Channel* DigitChannelGet(int nIndex) { return &m_oDigitChannelVector[nIndex]; }
    Channel* GlobalClockChannelGet() { return &m_oGlobalClockChannel; }
    
    short BCDAndChannelCountGet() { return m_nBCDAndDigitChannelCount; }
    
  protected:
  	SimpleArchive m_oArchive;
    short m_nBCDAndDigitChannelCount = 4;

    std::stringstream m_oTitle{std::ios_base::app | std::ios_base::out};
    std::stringstream m_oTooltip{std::ios_base::app | std::ios_base::out};

    std::vector<Channel> m_oBCDChannelVector{};
    std::vector<Channel> m_oDigitChannelVector{};

    Channel m_oGlobalClockChannel{UNDEFINED_CHANNEL};

    std::vector<std::unique_ptr<AnalyzerSettingInterfaceChannel>> m_oBCDChannelInterfaceVector{4};
    std::vector<std::unique_ptr<AnalyzerSettingInterfaceChannel>> m_oDigitChannelInterfaceVector{4};

    std::unique_ptr<AnalyzerSettingInterfaceChannel> m_oGlobalClockChannelInterface;
    
};

#endif //LD110_ANALYZER_SETTINGS
