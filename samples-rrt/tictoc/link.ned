simple nodo
{
    parameters:
        @display("i=block/routing");
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
    @display("bgb=468.71997,287.37");
    types:
        channel Channel extends ned.DatarateChannel
        {
            datarate = 3000bps;
            delay = 5ms;
            per = 0.25;
        }
    submodules:
        nodo1: nodo {
            @display("p=138.56999,137.64");
            probabilidad = 1/4;
            final = false;
            gates:
                inPort[3];
                outPort[2];
        }
        nodo2: nodo {
            @display("p=228.77998,112.53");
            probabilidad = 1/3;
            final = false;
            gates:
                inPort[5];
                outPort[4];
        }
        nodo5: nodo {
            @display("p=228.77998,191.57999");
            probabilidad = 1/2;
            final = false;
            gates:
                inPort[4];
                outPort[3];
        }
        nodo3: nodo {
            @display("p=341.31,112.53");
            probabilidad = 1;
            final = true;
            gates:
                inPort[1];
                outPort[1];
        }
        nodo4: nodo {
            @display("p=341.31,191.57999");
            probabilidad = 1;
            final = true;
            gates:
                inPort[2];
                outPort[2];
        }
        fuente1: fuente {
            @display("p=54.87,137.64");
            meanPacketLength = 9600/3;
        }
        fuente2: fuente {
            @display("p=206.45999,46.499996");
            meanPacketLength = 9600/3;
        }
        fuente5: fuente {
            @display("p=144.15,249.23999");
            meanPacketLength = 9600/3;
        }
    connections:
        nodo1.outPort[0] --> Channel --> nodo2.inPort[2];
        nodo1.inPort[0] <-- Channel <-- nodo2.outPort[2];

        nodo1.outPort[1] --> Channel --> nodo5.inPort[2];
        nodo1.inPort[1] <-- Channel <-- nodo5.outPort[2];

        nodo2.outPort[0] --> Channel --> nodo3.inPort[0];
        nodo2.inPort[0] <-- Channel <-- nodo3.outPort[0];

        nodo2.outPort[1] --> Channel --> nodo4.inPort[0];
        nodo2.inPort[1] <-- Channel <-- nodo4.outPort[0];

        nodo5.outPort[0] --> Channel --> nodo2.inPort[3];
        nodo5.inPort[0] <-- Channel <-- nodo2.outPort[3];

        nodo5.outPort[1] --> Channel --> nodo4.inPort[1];
        nodo5.inPort[1] <-- Channel <-- nodo4.outPort[1];

        fuente1.outPort --> nodo1.inPort[2];
        fuente2.outPort --> nodo2.inPort[4];
        fuente5.outPort --> nodo5.inPort[3];
}
