//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/AckRep.msg.
//

#ifndef __ACKREP_M_H
#define __ACKREP_M_H

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
 * Class generated from <tt>Messages/AckRep.msg:17</tt> by nedtool.
 * <pre>
 * packet AckRep
 * {
 *     unsigned int idDest;
 *     unsigned int sourceId;
 *     unsigned int targetId;
 *     unsigned int ackComponent;
 *     bool ack;
 * }
 * </pre>
 */
class AckRep : public ::omnetpp::cPacket
{
  protected:
    unsigned int idDest;
    unsigned int sourceId;
    unsigned int targetId;
    unsigned int ackComponent;
    bool ack;

  private:
    void copy(const AckRep& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AckRep&);

  public:
    AckRep(const char *name=nullptr, short kind=0);
    AckRep(const AckRep& other);
    virtual ~AckRep();
    AckRep& operator=(const AckRep& other);
    virtual AckRep *dup() const override {return new AckRep(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getIdDest() const;
    virtual void setIdDest(unsigned int idDest);
    virtual unsigned int getSourceId() const;
    virtual void setSourceId(unsigned int sourceId);
    virtual unsigned int getTargetId() const;
    virtual void setTargetId(unsigned int targetId);
    virtual unsigned int getAckComponent() const;
    virtual void setAckComponent(unsigned int ackComponent);
    virtual bool getAck() const;
    virtual void setAck(bool ack);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AckRep& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AckRep& obj) {obj.parsimUnpack(b);}


#endif // ifndef __ACKREP_M_H

