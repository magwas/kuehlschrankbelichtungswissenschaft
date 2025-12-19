#define MINTEMP -40
#define NTC_PARTNO NDBG504F3950B1F
#define TOOHIGHTEMP 32000

class NtcThermistor {
    protected:
        static float calibrationdata[];

        int resistance2temperature(float resistance);

        float voltage2resistance(float v, float r1) ;

    public:
        int voltage2temperature(float v, float r1);
};
