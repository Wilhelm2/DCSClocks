//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/AckComponent.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "AckComponent_m.h"

namespace omnetpp
{

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T, A>& v)
{
	int n = v.size();
	doParsimPacking(buffer, n);
	for (int i = 0; i < n; i++)
		doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T, A>& v)
{
	int n;
	doParsimUnpacking(buffer, n);
	v.resize(n);
	for (int i = 0; i < n; i++)
		doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T, A>& l)
{
	doParsimPacking(buffer, (int) l.size());
	for (typename std::list<T, A>::const_iterator it = l.begin(); it != l.end(); ++it)
		doParsimPacking(buffer, (T&) *it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T, A>& l)
{
	int n;
	doParsimUnpacking(buffer, n);
	for (int i = 0; i < n; i++)
	{
		l.push_back(T());
		doParsimUnpacking(buffer, l.back());
	}
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T, Tr, A>& s)
{
	doParsimPacking(buffer, (int) s.size());
	for (typename std::set<T, Tr, A>::const_iterator it = s.begin(); it != s.end(); ++it)
		doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T, Tr, A>& s)
{
	int n;
	doParsimUnpacking(buffer, n);
	for (int i = 0; i < n; i++)
	{
		T x;
		doParsimUnpacking(buffer, x);
		s.insert(x);
	}
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K, V, Tr, A>& m)
{
	doParsimPacking(buffer, (int) m.size());
	for (typename std::map<K, V, Tr, A>::const_iterator it = m.begin(); it != m.end(); ++it)
	{
		doParsimPacking(buffer, it->first);
		doParsimPacking(buffer, it->second);
	}
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K, V, Tr, A>& m)
{
	int n;
	doParsimUnpacking(buffer, n);
	for (int i = 0; i < n; i++)
	{
		K k;
		V v;
		doParsimUnpacking(buffer, k);
		doParsimUnpacking(buffer, v);
		m[k] = v;
	}
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
	for (int i = 0; i < n; i++)
		doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
	for (int i = 0; i < n; i++)
		doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
	throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s",
			omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
	throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s",
			omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T, A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out, const T&)
{
	return out;
}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T, A>& vec)
{
	out.put('{');
	for (typename std::vector<T, A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
	{
		if (it != vec.begin())
		{
			out.put(',');
			out.put(' ');
		}
		out << *it;
	}
	out.put('}');

	char buf[32];
	sprintf(buf, " (size=%u)", (unsigned int) vec.size());
	out.write(buf, strlen(buf));
	return out;
}

class IntVecACKDescriptor: public omnetpp::cClassDescriptor
{
private:
	mutable const char **propertynames;
public:
	IntVecACKDescriptor();
	virtual ~IntVecACKDescriptor();

	virtual bool doesSupport(omnetpp::cObject *obj) const override;
	virtual const char **getPropertyNames() const override;
	virtual const char *getProperty(const char *propertyname) const override;
	virtual int getFieldCount() const override;
	virtual const char *getFieldName(int field) const override;
	virtual int findField(const char *fieldName) const override;
	virtual unsigned int getFieldTypeFlags(int field) const override;
	virtual const char *getFieldTypeString(int field) const override;
	virtual const char **getFieldPropertyNames(int field) const override;
	virtual const char *getFieldProperty(int field, const char *propertyname) const override;
	virtual int getFieldArraySize(void *object, int field) const override;

	virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
	virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
	virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

	virtual const char *getFieldStructName(int field) const override;
	virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(IntVecACKDescriptor)

IntVecACKDescriptor::IntVecACKDescriptor() :
		omnetpp::cClassDescriptor("IntVecACK", "omnetpp::cObject")
{
	propertynames = nullptr;
}

IntVecACKDescriptor::~IntVecACKDescriptor()
{
	delete[] propertynames;
}

bool IntVecACKDescriptor::doesSupport(omnetpp::cObject *obj) const
{
	return dynamic_cast<IntVecACK *>(obj) != nullptr;
}

const char **IntVecACKDescriptor::getPropertyNames() const
{
	if (!propertynames)
	{
		static const char *names[] = { "existingClass", nullptr };
		omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
		const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
		propertynames = mergeLists(basenames, names);
	}
	return propertynames;
}

const char *IntVecACKDescriptor::getProperty(const char *propertyname) const
{
	if (!strcmp(propertyname, "existingClass"))
		return "";
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int IntVecACKDescriptor::getFieldCount() const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	return basedesc ? 0 + basedesc->getFieldCount() : 0;
}

unsigned int IntVecACKDescriptor::getFieldTypeFlags(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldTypeFlags(field);
		field -= basedesc->getFieldCount();
	}
	return 0;
}

const char *IntVecACKDescriptor::getFieldName(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldName(field);
		field -= basedesc->getFieldCount();
	}
	return nullptr;
}

int IntVecACKDescriptor::findField(const char *fieldName) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *IntVecACKDescriptor::getFieldTypeString(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldTypeString(field);
		field -= basedesc->getFieldCount();
	}
	return nullptr;
}

const char **IntVecACKDescriptor::getFieldPropertyNames(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldPropertyNames(field);
		field -= basedesc->getFieldCount();
	}
	switch (field)
	{
		default:
			return nullptr;
	}
}

const char *IntVecACKDescriptor::getFieldProperty(int field, const char *propertyname) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldProperty(field, propertyname);
		field -= basedesc->getFieldCount();
	}
	switch (field)
	{
		default:
			return nullptr;
	}
}

int IntVecACKDescriptor::getFieldArraySize(void *object, int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldArraySize(object, field);
		field -= basedesc->getFieldCount();
	}
	IntVecACK *pp = (IntVecACK *) object;
	(void) pp;
	switch (field)
	{
		default:
			return 0;
	}
}

const char *IntVecACKDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldDynamicTypeString(object, field, i);
		field -= basedesc->getFieldCount();
	}
	IntVecACK *pp = (IntVecACK *) object;
	(void) pp;
	switch (field)
	{
		default:
			return nullptr;
	}
}

std::string IntVecACKDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldValueAsString(object, field, i);
		field -= basedesc->getFieldCount();
	}
	IntVecACK *pp = (IntVecACK *) object;
	(void) pp;
	switch (field)
	{
		default:
			return "";
	}
}

bool IntVecACKDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->setFieldValueAsString(object, field, i, value);
		field -= basedesc->getFieldCount();
	}
	IntVecACK *pp = (IntVecACK *) object;
	(void) pp;
	switch (field)
	{
		default:
			return false;
	}
}

const char *IntVecACKDescriptor::getFieldStructName(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldStructName(field);
		field -= basedesc->getFieldCount();
	}
	return nullptr;
}

void *IntVecACKDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldStructValuePointer(object, field, i);
		field -= basedesc->getFieldCount();
	}
	IntVecACK *pp = (IntVecACK *) object;
	(void) pp;
	switch (field)
	{
		default:
			return nullptr;
	}
}

Register_Class(AckComponent)

AckComponent::AckComponent(const char *name, short kind) :
		::omnetpp::cPacket(name, kind)
{
	this->sourceId = 0;
	this->targetId = 0;
	this->componentIndex = 0;
}

AckComponent::AckComponent(const AckComponent& other) :
		::omnetpp::cPacket(other)
{
	copy(other);
}

AckComponent::~AckComponent()
{
}

AckComponent& AckComponent::operator=(const AckComponent& other)
{
	if (this == &other)
		return *this;
	::omnetpp::cPacket::operator=(other);
	copy(other);
	return *this;
}

void AckComponent::copy(const AckComponent& other)
{
	this->component = other.component;
	this->sourceId = other.sourceId;
	this->targetId = other.targetId;
	this->componentIndex = other.componentIndex;
}

void AckComponent::parsimPack(omnetpp::cCommBuffer *b) const
{
	::omnetpp::cPacket::parsimPack(b);
	doParsimPacking(b, this->component);
	doParsimPacking(b, this->sourceId);
	doParsimPacking(b, this->targetId);
	doParsimPacking(b, this->componentIndex);
}

void AckComponent::parsimUnpack(omnetpp::cCommBuffer *b)
{
	::omnetpp::cPacket::parsimUnpack(b);
	doParsimUnpacking(b, this->component);
	doParsimUnpacking(b, this->sourceId);
	doParsimUnpacking(b, this->targetId);
	doParsimUnpacking(b, this->componentIndex);
}

IntVecACK& AckComponent::getComponent()
{
	return this->component;
}

void AckComponent::setComponent(const IntVecACK& component)
{
	this->component = component;
}

unsigned int AckComponent::getSourceId() const
{
	return this->sourceId;
}

void AckComponent::setSourceId(unsigned int sourceId)
{
	this->sourceId = sourceId;
}

unsigned int AckComponent::getTargetId() const
{
	return this->targetId;
}

void AckComponent::setTargetId(unsigned int targetId)
{
	this->targetId = targetId;
}

unsigned int AckComponent::getComponentIndex() const
{
	return this->componentIndex;
}

void AckComponent::setComponentIndex(unsigned int componentIndex)
{
	this->componentIndex = componentIndex;
}

class AckComponentDescriptor: public omnetpp::cClassDescriptor
{
private:
	mutable const char **propertynames;
public:
	AckComponentDescriptor();
	virtual ~AckComponentDescriptor();

	virtual bool doesSupport(omnetpp::cObject *obj) const override;
	virtual const char **getPropertyNames() const override;
	virtual const char *getProperty(const char *propertyname) const override;
	virtual int getFieldCount() const override;
	virtual const char *getFieldName(int field) const override;
	virtual int findField(const char *fieldName) const override;
	virtual unsigned int getFieldTypeFlags(int field) const override;
	virtual const char *getFieldTypeString(int field) const override;
	virtual const char **getFieldPropertyNames(int field) const override;
	virtual const char *getFieldProperty(int field, const char *propertyname) const override;
	virtual int getFieldArraySize(void *object, int field) const override;

	virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
	virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
	virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

	virtual const char *getFieldStructName(int field) const override;
	virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(AckComponentDescriptor)

AckComponentDescriptor::AckComponentDescriptor() :
		omnetpp::cClassDescriptor("AckComponent", "omnetpp::cPacket")
{
	propertynames = nullptr;
}

AckComponentDescriptor::~AckComponentDescriptor()
{
	delete[] propertynames;
}

bool AckComponentDescriptor::doesSupport(omnetpp::cObject *obj) const
{
	return dynamic_cast<AckComponent *>(obj) != nullptr;
}

const char **AckComponentDescriptor::getPropertyNames() const
{
	if (!propertynames)
	{
		static const char *names[] = { nullptr };
		omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
		const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
		propertynames = mergeLists(basenames, names);
	}
	return propertynames;
}

const char *AckComponentDescriptor::getProperty(const char *propertyname) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int AckComponentDescriptor::getFieldCount() const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	return basedesc ? 4 + basedesc->getFieldCount() : 4;
}

unsigned int AckComponentDescriptor::getFieldTypeFlags(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldTypeFlags(field);
		field -= basedesc->getFieldCount();
	}
	static unsigned int fieldTypeFlags[] =
			{ FD_ISCOMPOUND | FD_ISCOBJECT, FD_ISEDITABLE, FD_ISEDITABLE, FD_ISEDITABLE, };
	return (field >= 0 && field < 4) ? fieldTypeFlags[field] : 0;
}

const char *AckComponentDescriptor::getFieldName(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldName(field);
		field -= basedesc->getFieldCount();
	}
	static const char *fieldNames[] = { "component", "sourceId", "targetId", "componentIndex", };
	return (field >= 0 && field < 4) ? fieldNames[field] : nullptr;
}

int AckComponentDescriptor::findField(const char *fieldName) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	int base = basedesc ? basedesc->getFieldCount() : 0;
	if (fieldName[0] == 'c' && strcmp(fieldName, "component") == 0)
		return base + 0;
	if (fieldName[0] == 's' && strcmp(fieldName, "sourceId") == 0)
		return base + 1;
	if (fieldName[0] == 't' && strcmp(fieldName, "targetId") == 0)
		return base + 2;
	if (fieldName[0] == 'c' && strcmp(fieldName, "componentIndex") == 0)
		return base + 3;
	return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *AckComponentDescriptor::getFieldTypeString(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldTypeString(field);
		field -= basedesc->getFieldCount();
	}
	static const char *fieldTypeStrings[] = { "IntVecACK", "unsigned int", "unsigned int", "unsigned int", };
	return (field >= 0 && field < 4) ? fieldTypeStrings[field] : nullptr;
}

const char **AckComponentDescriptor::getFieldPropertyNames(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldPropertyNames(field);
		field -= basedesc->getFieldCount();
	}
	switch (field)
	{
		default:
			return nullptr;
	}
}

const char *AckComponentDescriptor::getFieldProperty(int field, const char *propertyname) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldProperty(field, propertyname);
		field -= basedesc->getFieldCount();
	}
	switch (field)
	{
		default:
			return nullptr;
	}
}

int AckComponentDescriptor::getFieldArraySize(void *object, int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldArraySize(object, field);
		field -= basedesc->getFieldCount();
	}
	AckComponent *pp = (AckComponent *) object;
	(void) pp;
	switch (field)
	{
		default:
			return 0;
	}
}

const char *AckComponentDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldDynamicTypeString(object, field, i);
		field -= basedesc->getFieldCount();
	}
	AckComponent *pp = (AckComponent *) object;
	(void) pp;
	switch (field)
	{
		default:
			return nullptr;
	}
}

std::string AckComponentDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldValueAsString(object, field, i);
		field -= basedesc->getFieldCount();
	}
	AckComponent *pp = (AckComponent *) object;
	(void) pp;
	switch (field)
	{
		case 0:
		{
			std::stringstream out;
			out << pp->getComponent();
			return out.str();
		}
		case 1:
			return ulong2string(pp->getSourceId());
		case 2:
			return ulong2string(pp->getTargetId());
		case 3:
			return ulong2string(pp->getComponentIndex());
		default:
			return "";
	}
}

bool AckComponentDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->setFieldValueAsString(object, field, i, value);
		field -= basedesc->getFieldCount();
	}
	AckComponent *pp = (AckComponent *) object;
	(void) pp;
	switch (field)
	{
		case 1:
			pp->setSourceId(string2ulong(value));
			return true;
		case 2:
			pp->setTargetId(string2ulong(value));
			return true;
		case 3:
			pp->setComponentIndex(string2ulong(value));
			return true;
		default:
			return false;
	}
}

const char *AckComponentDescriptor::getFieldStructName(int field) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldStructName(field);
		field -= basedesc->getFieldCount();
	}
	switch (field)
	{
		case 0:
			return omnetpp::opp_typename(typeid(IntVecACK));
		default:
			return nullptr;
	};
}

void *AckComponentDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
	omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
	if (basedesc)
	{
		if (field < basedesc->getFieldCount())
			return basedesc->getFieldStructValuePointer(object, field, i);
		field -= basedesc->getFieldCount();
	}
	AckComponent *pp = (AckComponent *) object;
	(void) pp;
	switch (field)
	{
		case 0:
			return (void *) reinterpret_cast<omnetpp::cObject *>(&pp->getComponent());
			break;
		default:
			return nullptr;
	}
}

