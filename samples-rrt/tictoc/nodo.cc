#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"

using namespace omnetpp;

class nodo : public cSimpleModule
{
    private:
        cChannel *channel;
        cQueue *queue;
        double probability;  //Como es solo un enlace de salida siempre sera 1
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendNew(paquete_struct *pkt);
        virtual void sendNext();
        virtual void sendPacket(paquete_struct *pkt);
};

Define_Module(nodo);

void nodo::initialize() {
    // Get cChannel pointers from gates
    channel = gate("outPort") -> getTransmissionChannel();

    // Create one queue for each channel
    queue = new cQueue("queueZero");

    // Initialize random number generator
    srand(time(NULL));

    // Get probability parameter
    probability = 1;
}

//Gestiona la entrada de paquetes en el nodo
//Es decir trata el paquete en funcion del tipo de paquete: fuente, otro nodo, ACK o NAK.
void nodo::handleMessage(cMessage *msg)
{
    paquete_struct *pkt = check_and_cast<paquete_struct *> (msg);
    cGate *arrivalGate = pkt -> getArrivalGate();
    EV << "Paquete recibido\n";

    if (pkt -> getFromSource()) { //Paquete recibido de la fuente
        EV << "Se ha recibido un paquete de la fuente\n";
        pkt -> setFromSource(false);
        sendNew(pkt);
        return;
    }
    else if (pkt -> getKind() == 1) { // 1: Paquete recibido de otro nodo
        if (pkt -> hasBitError()) {
            EV << "Paquete recibido con error. Se va a enviar un NAK\n";
            paquete_struct *nak = new paquete_struct("NAK");
            nak -> setKind(3);
            send(nak, "outPort");
        }
        else {
            EV << "Packet recibido sin error. Se va a enviar un ACK\n";
            paquete_struct *ack = new paquete_struct("ACK");
            ack -> setKind(2);
            send(ack, "outPort");
        }
    }
    else if (pkt -> getKind() == 2) { // 2: ACK
        EV << "ACK recibido\n";
        if (queue -> isEmpty())
            EV << "La cola esta vacia, no hay paquetes para transmitir. Esperando nuevos paquetes...";
        else {
            queue -> pop();
            sendNext();
        }
    }
    else { // 3: NAK
        EV << "NAK recibido\n";
        sendNext();
    }
}

//
void nodo::sendNew(paquete_struct *pkt) {
    if (queue -> isEmpty()) {
        EV << "La cola esta vacia, envia el paquete directamente\n";
        // Insert in queue (it may have to be sent again)
        queue-> insert(pkt);
        sendPacket(pkt);
    } else {
        EV << "La cola no esta vacia, se añade al final y se espera a que se envie cuando toque\n";
        queue -> insert(pkt);
    }
}

//Envia el paquete que esta primero en la cola
void nodo::sendNext() {
    if (queue -> isEmpty())
        EV << "La cola esta vacia, no se pueden enviar mas paquetes\n";
    else {
        paquete_struct *pkt = check_and_cast<paquete_struct *> (queue -> front());
        sendPacket(pkt);
    }
}

//Si el canal esta vacio, envia el paquete (una copia) por la salida
void nodo::sendPacket(paquete_struct *pkt) {
    if (channel -> isBusy()) {
        EV << "WARNING: channel is busy, check that everything is working fine\n";
    } else {
        // OMNeT++ can't send a packet while it is queued, must send a copy
        paquete_struct *newPkt = check_and_cast<paquete_struct *> (pkt -> dup());
        send(newPkt, "outPort");
    }
}
