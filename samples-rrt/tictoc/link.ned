simple nodo
{
    parameters:
        @display("i=block/departure");
        double probabilidad;
        bool final;
    gates:
        input inPort[];
        output outPort[];
}

simple fuente
{
    parameters:
        @display("i=block/source");
        double meanPacketLength;
    gates:
        output outPort;
}

network linkSW
{
    @display("bgb=285.6,182.28");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 1000bps;
            delay = 100us;
            per = 0.25;
        }
    submodules:
        nodo1: nodo {
            @display("p=144.15,71.61");
            probabilidad = 0.5;
            final = false;
            gates:
                inPort[3];
				outPort[2];
        }
        nodo2: nodo {
            @display("p=233.43,125.549995");
            probabilidad = 1;
            final = true;
            gates:
                inPort[1];
                outPort[1];
        }
        nodo3: nodo {
            @display("p=233.43,35.34");
            probabilidad = 1;
            final = true;
            gates:
                inPort[1];
                outPort[1];
        }
        fuente1: fuente {
            @display("p=60.449997,71.61");
            meanPacketLength = 1000/3;
        }
    connections:
        nodo1.outPort[0] --> Channel --> nodo2.inPort[0];
        nodo1.inPort[0] <-- Channel <-- nodo2.outPort[0];
        
        nodo1.outPort[1] --> Channel --> nodo3.inPort[0];
        nodo1.inPort[1] <-- Channel <-- nodo3.outPort[0];

        fuente1.outPort --> nodo1.inPort[2];
}
