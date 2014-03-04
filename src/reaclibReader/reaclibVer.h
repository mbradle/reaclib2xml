/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibVer.h
////////////////////////////////////////////////////////////////////////////////
Function: This file contains the implementation for the ReaclibReader version 
	class. The reaclibRate class provides the functionality for the rates.
	
Versions:
	0.0.0: 2008-03-24 Scott Warren (warren@nscl.msu.edu)
		Created file.
//////////////////////////////////////////////////////////////////////////////*/

#include <iostream>
#include "tinyxml.h"
#include "reaclibSet.h"
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

#ifndef __reaclibVer_h__
#define __reaclibVer_h__

class ReaclibVer {
public:
/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibVer

Purpose: This function is the default constructor for the ReaclibVer class.  
//////////////////////////////////////////////////////////////////////////////*/
ReaclibVer();

/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibVer

Purpose: This function is the XML constructor for the ReaclibVer class.  

Arguments: 
			pElement: a pointer to the XML element to load the information from.
//////////////////////////////////////////////////////////////////////////////*/
ReaclibVer(TiXmlElement *pElement);
	
/*//////////////////////////////////////////////////////////////////////////////
Function: ~ReaclibVer

Purpose: This function is the default destructor for the ReaclibVer class.  
//////////////////////////////////////////////////////////////////////////////*/
~ReaclibVer();

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

void addSet(ReaclibSet *newSet, bool bSkipLink = false);

// current
bool isCurrent();
void setCurrent(bool bCurrent);

// id
int getID();
void setID(int id);

// modified date
const char *getModified();
void setModified(const char *strModified);

// Q value
float getQ();
void setQ(float Q);

// the sets
ReaclibSet *getSet(unsigned i);
int getSetSize();

// version number
int getVersion();
void setVersion(int version);

TiXmlElement *getXMLLink();

private:
	// the element in the XML file
	TiXmlElement *m_pVer;

	// the Q value
	float m_Q;
	
	// the modified date 
	char *m_pstrModified;
	
	// if the version is current
	bool m_bCurrent;
	
	// the server side id of the version
	int m_id;
	
	// the version number
	int m_version;

	// the sets
	vector<ReaclibSet*> m_sets;
};

#endif
