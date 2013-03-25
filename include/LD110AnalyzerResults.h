#ifndef LD110_ANALYZER_RESULTS
#define LD110_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class LD110Analyzer;
class LD110AnalyzerSettings;

class LD110AnalyzerResults : public AnalyzerResults
  {
  public:
    LD110AnalyzerResults( LD110Analyzer* analyzer, LD110AnalyzerSettings* settings );
    virtual ~LD110AnalyzerResults();

    virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
    virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

    virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
    virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
    virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

  protected:
    LD110AnalyzerSettings* m_poSettings;
    LD110Analyzer* m_poAnalyzer;
  };

#endif //LD110_ANALYZER_RESULTS
