//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/broadcastNotify.msg.
//

#ifndef __BROADCASTNOTIFY_M_H
#define __BROADCASTNOTIFY_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>Messages/broadcastNotify.msg:16</tt> by nedtool.
 * <pre>
 * packet BroadcastNotify
 * {
 * }
 * </pre>
 */
class BroadcastNotify : public ::omnetpp::cPacket
{
  protected:

  private:
    void copy(const BroadcastNotify& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const BroadcastNotify&);

  public:
    BroadcastNotify(const char *name=nullptr, short kind=0);
    BroadcastNotify(const BroadcastNotify& other);
    virtual ~BroadcastNotify();
    BroadcastNotify& operator=(const BroadcastNotify& other);
    virtual BroadcastNotify *dup() const override {return new BroadcastNotify(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const BroadcastNotify& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, BroadcastNotify& obj) {obj.parsimUnpack(b);}


#endif // ifndef __BROADCASTNOTIFY_M_H
