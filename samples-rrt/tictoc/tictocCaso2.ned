//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


//
// Here we make the model look a bit prettier in the GUI. We assign the
// "block/routing" icon to the simple module. All submodules of type
// Txc2 will use this icon by default
//
simple TxcCaso2
{
    parameters:
        @display("i=block/routing"); // add a default icon
    gates:
        input in[];
        output out[];
}

//
// Make the two module look a bit different with colorization effect.
// Use cyan for `tic', and yellow for `toc'.
//
network tictocCaso2
{
    submodules:
        nodo1: TxcCaso2 {
            @display("p=79,159");
        }
        nodo2: TxcCaso2 {
            @display("p=274,55");
        }
        nodo3: TxcCaso2 {
            @display("p=502,55");
        }
        nodo4: TxcCaso2 {
            @display("p=394,244");
        }
        nodo5: TxcCaso2 {
            @display("p=187,244");
        }
    connections:
//      Enlaces entre el nodo1 y el nodo2
        nodo1.out[0] --> {  delay = 100ms; } --> nodo2.in[0];
        nodo1.in[0] <-- {  delay = 100ms; } <-- nodo2.out[0];

//      Enlaces entre el nodo1 y el nodo5
        nodo1.out[1] --> {  delay = 100ms; } --> nodo5.in[0];
        nodo1.in[1] <-- {  delay = 100ms; } <-- nodo5.out[0];

//      Enlaces entre el nodo2 y el nodo3
        nodo2.out[1] --> {  delay = 100ms; } --> nodo3.in[0];
        nodo2.in[1] <-- {  delay = 100ms; } <-- nodo3.out[0];
        
//      Enlaces entre el nodo2 y el nodo4
        nodo2.out[3] --> {  delay = 100ms; } --> nodo4.in[1];
        nodo2.in[3] <-- {  delay = 100ms; } <-- nodo4.out[1];
        
//      Enlaces entre el nodo5 y el nodo2
        nodo5.out[2] --> {  delay = 100ms; } --> nodo2.in[2];
        nodo5.in[2] <-- {  delay = 100ms; } <-- nodo2.out[2];
        
//      Enlaces entre el nodo5 y el nodo4
        nodo5.out[1] --> {  delay = 100ms; } --> nodo4.in[0];
        nodo5.in[1] <-- {  delay = 100ms; } <-- nodo4.out[0];
}

