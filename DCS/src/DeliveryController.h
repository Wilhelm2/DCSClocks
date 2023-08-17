//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __DYNAMICCLOCKSET_DELIVERYCONTROLLER_H_
#define __DYNAMICCLOCKSET_DELIVERYCONTROLLER_H_


#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <stdexcept>

#include "Utilitaries.h"

using namespace std;
using namespace omnetpp;

typedef struct s_msg
{
    // pas besoin de l'id car ce vecteur est le vecteur du mh en question
    int seq;
    vector<int> horloge; // horloge du msg
    int nbDeliv; // nombre de délivraisons déjà effectués
    vector<bool> booldeliver; // tableau pour contrôler qu'il n'y a pas de délivraison multiples
    simtime_t sendTime;
    simtime_t totalTimeDeliver;
}msg;

/**
 * TODO - Generated class
 */
class DeliveryController : public cSimpleModule
{
  public:
    virtual void initialize();
    void sendMessage(int idMH, int seq);
    msg *searchMessage(vector<msg*> vecMessages, int seq);
    bool recvMessage(int idSrc, int seq, int idDest, bool decision);
    bool canDeliver(int idSrc, int seq, int idDest);
    void deleteMessage(int idSrc, int seq);
    //void recvMessageMSS(int idSrc, int seq);

    void printHorloge (vector<int> v);
    void printHorlogeErr (vector<int> v);
    bool isDependency(int idMsg, int seqMsg, int idDep, int seqDep);
    void MissingDependencies(int idMsg, int seqMsg, int idDest);
    void compareHashVectors(vector<int>v1, int id, int seq);
    void compareMessagesDependencies(int idMsg1, int seqMsg1, int idMsg2, int seqMsg2);
    void testModule(void);
    vector<vector<msg *>> messages; // chaque MH (vector) a un vecteur (vecteur) de messages (<int, vector>)

    simtime_t totalTimeDelivered; // temps total de délivraison de tous les mobiles

    vector<vector<int>> Horloges;

//    int falseDeliv=0;
    int multipleDeliv=0;
    Utilitaries* ut;
};

#endif
