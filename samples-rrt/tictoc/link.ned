simple nodo
{
    parameters:
        @display("i=block/departure");
    gates:
        input inPort[];
        output outPort;
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
            gates:
                inPort[2];
        }
        nodo2: nodo {
            @display("p=233.43,71.61");
            gates:
                inPort[1];
        }
        fuente1: fuente {
            @display("p=60.449997,71.61");
            meanPacketLength = 1000/3;
        }
    connections:
        nodo1.outPort --> Channel --> nodo2.inPort[0];
        nodo1.inPort[1] <-- Channel <-- nodo2.outPort;

        fuente1.outPort --> nodo1.inPort[0];
}
