#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "paquete_m.h"
#include <iostream>

using namespace omnetpp;
using namespace std;

class nodo : public cSimpleModule
{
    private:
        cChannel *channel[2];
        cQueue *queue[2];
        double prob;
        bool finalNode;
        long numSent;
        long numReceived;
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        virtual void sendNew(paquete_struct *pkt);
        virtual void sendNext(int index);
        virtual void sendPacket(paquete_struct *pkt, int index);
        virtual int checkWhichOutputIndex();
        virtual void refreshDisplay() const override;
};

Define_Module(nodo);

void nodo::initialize() {

    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);
    prob = (double) par("probabilidad");
    finalNode = (bool) par("final");
    // Get cChannel pointers from gates

    if(finalNode == false){
        channel[0] = gate("outPort",0) -> getTransmissionChannel();
        queue[0] = new cQueue("queueZero");

        queue[1] = new cQueue("queueOne");
        channel[1] = gate("outPort",1) -> getTransmissionChannel();
    }

    // Create one queue for each channel
    // Initialize random number generator
    srand(time(NULL));
}

//Gestiona la entrada de paquetes en el nodo
//Es decir trata el paquete en funcion del tipo de paquete: fuente, otro nodo, ACK o NAK.
void nodo::handleMessage(cMessage *msg)
{
    paquete_struct *pkt = check_and_cast<paquete_struct *> (msg);
    EV << "Tipo de paquete: "+to_string(pkt->getKind())+"\n";

    int queueIndex = pkt -> getArrivalGate()->getIndex();
    EV << "Paquete recibido\n";
    numReceived++;

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
            send(nak, "outPort",queueIndex);
        }
        else {
            EV << "Paquete recibido sin error. Se va a enviar un ACK\n";
            paquete_struct *ack = new paquete_struct("ACK");
            ack -> setKind(2);
            send(ack, "outPort",queueIndex);
            if(finalNode == false) {
               sendNew(pkt);
            }
        }
    }
    else if (pkt -> getKind() == 2) { // 2: ACK
        EV << "ACK recibido\n";
        if (queue[queueIndex] -> isEmpty())
            EV << "La cola esta vacia, no hay paquetes para transmitir. Esperando nuevos paquetes...";
        else {
            queue[queueIndex] -> pop();
            sendNext(queueIndex);
        }
    }
    else { // 3: NAK
        EV << "NAK recibido\n";
        sendNext(queueIndex);
    }
}

//
void nodo::sendNew(paquete_struct *pkt) {
    int index = checkWhichOutputIndex();
    if (queue[index] -> isEmpty()) {
        EV << "La cola esta vacia, envia el paquete directamente\n";
        // Insert in queue (it may have to be sent again)
        queue[index]-> insert(pkt);
        sendPacket(pkt,index);
    } else {
        EV << "La cola no esta vacia, se añade al final y se espera a que se envie cuando toque\n";
        queue[index] -> insert(pkt);
    }
}

//Envia el paquete que esta primero en la cola
void nodo::sendNext(int index) {
    if (queue[index] -> isEmpty())
        EV << "La cola esta vacia, no se pueden enviar mas paquetes\n";
    else {
        paquete_struct *pkt = check_and_cast<paquete_struct *> (queue[index] -> front());
        sendPacket(pkt, index);
    }
}

//Si el canal esta vacio, envia el paquete (una copia) por la salida
void nodo::sendPacket(paquete_struct *pkt, int index) {
    double time = channel[index]-> getTransmissionFinishTime().dbl();
    bool busy = channel[index] -> isBusy();
    EV << "timpo restante: "+to_string(time)+"\n";
    EV << "canal ocupado: "+to_string(busy)+"\n";
    if (channel[index] -> isBusy()) {
        EV << "El canal esta ocupado en estos momentos";
    } else {
        // OMNeT++ can't send a packet while it is queued, must send a copy
        EV << "No esta ocupado\n";
        paquete_struct *newPkt = check_and_cast<paquete_struct *> (pkt -> dup());
        send(newPkt, "outPort",index);
        numSent++;
    }
}

int nodo::checkWhichOutputIndex(){
    int index;
    double rnd = ((float) rand() / (RAND_MAX));

    if (rnd < prob){
        index = 0;
    }
    else {
        index = 1;
    }
    return index;
}

void nodo::refreshDisplay() const{
    char buf[40];
    sprintf(buf, "rcvd: %ld sent: %ld",numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}
