#define LN2 0.6931471805599453
struct MeasureResult {
    alignas(double) double value;
    uint8_t pin;
    alignas(int) int ticks;
    alignas(int) int elapsed;
};

#define NUM_CHANNELS 4
#define CHANNELS X(0) X(1) X(2) X(3)

class Measure {
    protected:
        static Measure channel[NUM_CHANNELS];
        #define X(N) static void ICACHE_RAM_ATTR handler##N()  {channel[N].numticks++;};
        CHANNELS
        #undef X
        #define X(N) static void ICACHE_RAM_ATTR listener##N(Message *msg)  {channel[N].measure();};
        CHANNELS
        #undef X
        volatile unsigned int numticks;
        int pin;
        int r1;
        int r2;
        int c;
        unsigned int lastMillis;
        void (*instanceHandler)();
        Listener listener;
        void measure() {
            unsigned int now=millis();
            unsigned int elapsed=now-lastMillis;
            unsigned int nt=numticks;
            MeasureResult result;
            if(r1==0) {
                result.value = (elapsed*1442695.04088896/(c*nt))-2.0*r2;
            } else if(r2==0) {
                result.value = (elapsed*721347.520444482)/(c*nt)-0.5*r1;
            } else {
                result.value = (elapsed*1442695.04088896)/(nt*(r1+2.0*r2));
            }
            result.pin=pin;
            result.ticks=nt;
            result.elapsed=elapsed;
            systemQueue.send(MessageType::Result, (char*)&result);
            numticks=0;
            lastMillis=millis();
        }
        static void command(Message * msg) {
            CommandPayload *payload = (CommandPayload *)msg->payload;
            int ch = payload->arg2;
            if(ch>NUM_CHANNELS)
                systemQueue.printf("have only %u channels",NUM_CHANNELS);
            switch(payload->arg1) {
                case 0:
                    channel[ch].pin = payload->arg3;
                    break;
                case 1:
                    channel[ch].start(ch);
                    break;
                case 2:
                    channel[ch].stop();
                    break;
                case 3:
                    channel[ch].r1 = payload->arg3;
                    break;
                case 4:
                    channel[ch].r2 = payload->arg3;
                    break;
                case 5:
                    channel[ch].c = payload->arg3;
                    break;
                case 6:
                    systemQueue.printf("pin: %u, r1=%u",channel[ch].pin,channel[ch].r1);
                    systemQueue.printf("r2: %u, c=%u",channel[ch].r2,channel[ch].c);
                    break;
                case 7:
                    if(ch==1)
                        Measure::enable();
                    else
                        Measure::disable();
                        break;
                default:
                    help();
            }
        }
        static void help() {
                    systemQueue.printf("0: set ch pin");
                    systemQueue.printf("1: start ch");
                    systemQueue.printf("2: stop ch");
                    systemQueue.printf("3: set ch r1 (Ohm)");
                    systemQueue.printf("4: set ch r2(Ohm)");
                    systemQueue.printf("5: set ch c (nF)");
                    systemQueue.printf("6: show ch");
                    systemQueue.printf("7: enable off(0)/on(1)");
        }
    public:
        Measure() {
            r1=0;
            r2=0;
            c=0;
        }
        static void begin() {
            #define X(N) channel[N].instanceHandler = handler##N;
            CHANNELS
            #undef X
            #define X(N) channel[N].listener= listener##N;
            CHANNELS
            #undef X
            systemQueue.registerListener(MessageType::Measure,command);
        }
        static void enable() {
            pinMode(MEASURE_EN,OUTPUT);
            digitalWrite(MEASURE_EN,HIGH);
        }
        static void disable() {
            digitalWrite(MEASURE_EN,LOW);
        }
        void start(int ch) {
            pinMode(pin, INPUT_PULLUP);
            numticks=0;
            lastMillis=millis();
            attachInterrupt(digitalPinToInterrupt(pin),  channel[ch].instanceHandler, FALLING);
            systemQueue.registerListener(MessageType::OneHz,channel[ch].listener);
        }
        void stop() {
            detachInterrupt(digitalPinToInterrupt(pin));
            systemQueue.unregisterListener(MessageType::OneHz,listener);
        }
};

Measure Measure::channel[NUM_CHANNELS];
