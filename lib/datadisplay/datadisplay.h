#ifndef DATADISPLAY_H
#define DATADISPLAY_H

#include <NMEA2000.h>
#include <N2kMessages.h>



class DataDisplay {
    public:
        DataDisplay(Stream *outputStream);
        void HandleMsg(const tN2kMsg &N2kMsg);

        bool showData= false;
    private:
        void SystemTime(const tN2kMsg &N2kMsg);
        void Rudder(const tN2kMsg &N2kMsg);
        void EngineRapid(const tN2kMsg &N2kMsg);
        void EngineDynamicParameters(const tN2kMsg &N2kMsg);
        void TransmissionParameters(const tN2kMsg &N2kMsg);
        void TripFuelConsumption(const tN2kMsg &N2kMsg);
        void Speed(const tN2kMsg &N2kMsg);
        void WaterDepth(const tN2kMsg &N2kMsg);
        void BinaryStatus(const tN2kMsg &N2kMsg);
        void DCBatteryStatus(const tN2kMsg &N2kMsg);
        void FluidLevel(const tN2kMsg &N2kMsg);
        void OutsideEnvironmental(const tN2kMsg &N2kMsg);
        void EnvironmentalParams(const tN2kMsg &N2kMsg);
        void Temperature(const tN2kMsg &N2kMsg);
        void TemperatureExt(const tN2kMsg &N2kMsg);
        void DCStatus(const tN2kMsg &N2kMsg);
        void BatteryConfigurationStatus(const tN2kMsg &N2kMsg);
        void COGSOG(const tN2kMsg &N2kMsg);
        void GNSS(const tN2kMsg &N2kMsg);
        void LocalOffset(const tN2kMsg &N2kMsg);
        void Attitude(const tN2kMsg &N2kMsg);
        void Heading(const tN2kMsg &N2kMsg);
        void Humidity(const tN2kMsg &N2kMsg);
        void Pressure(const tN2kMsg &N2kMsg);
        void UserDatumSettings(const tN2kMsg &N2kMsg);
        void GNSSSatsInView(const tN2kMsg &N2kMsg);
        void AddressClaim(const tN2kMsg &N2kMsg);
        void BinaryStatusFull(const tN2kMsg &N2kMsg);


        void printLLNumber(Stream *OutputStream, unsigned long long n, uint8_t base=10);


        Stream *OutputStream;
};

template<typename T> void PrintLabelValWithConversionCheckUnDef(Stream *OutputStream, const char* label, T val, double (*ConvFunc)(double val)=0, bool AddLf=false, int8_t Desim=-1 ) {
  OutputStream->print(label);
  if (!N2kIsNA(val)) {
    if ( Desim<0 ) {
      if (ConvFunc) { OutputStream->print(ConvFunc(val)); } else { OutputStream->print(val); }
    } else {
      if (ConvFunc) { OutputStream->print(ConvFunc(val),Desim); } else { OutputStream->print(val,Desim); }
    }
  } else OutputStream->print("not available");
  if (AddLf) OutputStream->println();
}




#endif