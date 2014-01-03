#ifndef LD110_SIMULATION_DATA_GENERATOR
#define LD110_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class LD110AnalyzerSettings;

class LD110SimulationDataGenerator
{
public:
	LD110SimulationDataGenerator();
	~LD110SimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, LD110AnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	LD110AnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateBCD();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //LD110_SIMULATION_DATA_GENERATOR
