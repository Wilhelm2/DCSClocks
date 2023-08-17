//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/AckComponent.msg.
//

#ifndef __ACKCOMPONENT_M_H
#define __ACKCOMPONENT_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
	#include <vector>
	#include <omnetpp.h>
	#include "../Clock/ProbabilisticClock.h"
	typedef ProbabilisticClock IntVecACK;
	
// }}

/**
 * Class generated from <tt>Messages/AckComponent.msg:28</tt> by nedtool.
 * <pre>
 * packet AckComponent
 * {
 *     IntVecACK component;
 *     unsigned int sourceId;
 *     unsigned int ackComponent;
 *     unsigned int delay;
 * }
 * </pre>
 */
class AckComponent : public ::omnetpp::cPacket
{
  protected:
    IntVecACK component;
    unsigned int sourceId;
    unsigned int ackComponent;
    unsigned int delay;

  private:
    void copy(const AckComponent& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AckComponent&);

  public:
    AckComponent(const char *name=nullptr, short kind=0);
    AckComponent(const AckComponent& other);
    virtual ~AckComponent();
    AckComponent& operator=(const AckComponent& other);
    virtual AckComponent *dup() const override {return new AckComponent(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual IntVecACK& getComponent();
    virtual const IntVecACK& getComponent() const {return const_cast<AckComponent*>(this)->getComponent();}
    virtual void setComponent(const IntVecACK& component);
    virtual unsigned int getSourceId() const;
    virtual void setSourceId(unsigned int sourceId);
    virtual unsigned int getAckComponent() const;
    virtual void setAckComponent(unsigned int ackComponent);
    virtual unsigned int getDelay() const;
    virtual void setDelay(unsigned int delay);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AckComponent& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AckComponent& obj) {obj.parsimUnpack(b);}


#endif // ifndef __ACKCOMPONENT_M_H

