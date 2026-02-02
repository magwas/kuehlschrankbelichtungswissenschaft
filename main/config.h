

#define COMMAND_TABLE \
    ENTRY(None,0,"","No message")\
    ENTRY(All,255,"","All messages")\
    ENTRY(Stdin,1,"c","Standard input")\
    ENTRY(Stdout,2,"a","Standard output")\
    ENTRY(Debug,3,"i i i","Debug")\
    ENTRY(Command,4,"a","Command")\
    ENTRY(OneHz,5,"","One Hz tick")\
    ENTRY(Square,6,"i","Square wawe on port")\
    ENTRY(AstableResult,7,"f i i i","astable result. R1|R2|C, channel, elapsed, interrupts")\
    ENTRY(astable,8,"irun=0,pin=1,r1=2,r2plus=3,r2=4,c=5,config=6 i i","config astable. cmd, ch (-1=en), val (in run 0=stop, 1=start)")\
    ENTRY(config,9,"istore=1,retrieve=2,format=3,list=4,apply=5","configuration command")\
    ENTRY(configadd,10,"a","add to configuration")\
    ENTRY(configdel,11,"i","remove nth configuration")\
    ENTRY(wifi,12,"iconnect=1,disconnect=2,report=3","wifi commands")\
    ENTRY(wifissid,13,"a","wifi SSID")\
    ENTRY(wifipasswd,14,"a","wifi password")\
    ENTRY(WifiStatus,15,"idisconnected=0,started=1,connected=2,ip=3,netmask=4,gateway=5 c c c c","with ip address if applicable")\
    ENTRY(thermometer,16,"imgmt=1,setpin=2,measure=3 i i", "mgmt: reset=1,search=2 measure: device id")\
    ENTRY(ThermometerFound,17,"i i", "address of the device")\
    ENTRY(Temperature,18,"f i i", "address and temperature")\
    ENTRY(mux,19,"ishow=0,a0=1,a1=2,a2=3,d=4,en0=10,en1=11,set0=30,clear0=50,set1=31,clear1=51 i", "demux device")\
    ENTRY(CO2Read,20,"i", "CO2 ppm value")\
    ENTRY(co2,21,"iconnect=0,disconnect=1,dump=2,start=3,stop=4,calibrate=5","CO2 sensor commands")\
    ENTRY(co2set,22,"i i","set register of CO2 sensor to value")

    enum class MessageType : uint8_t {
        #define ENTRY(type,value,args,help) type=value,
        COMMAND_TABLE
        #undef ENTRY
    };