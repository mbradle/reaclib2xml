/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibRate.h
////////////////////////////////////////////////////////////////////////////////
Function: This file contains the implementation for the ReaclibReader rates 
	class. The reaclibRate class provides the functionality for the rates.
	
Versions:
	0.0.0: 2008-03-24 Scott Warren (warren@nscl.msu.edu)
		Created file.
//////////////////////////////////////////////////////////////////////////////*/

#ifndef __reaclibRate_h__
#define __reaclibRate_h__

#include "tinyxml.h"
#include "reaclibVer.h"
#include "reaclibReaderStructs.h"
#include "NuclideSearch.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

class ReaclibRate{
public:
/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibRate

Purpose: This function is the default constructor for the ReaclibRate class.  
//////////////////////////////////////////////////////////////////////////////*/
ReaclibRate();

/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibRate

Purpose: This function is a constructor for the ReaclibRate class to build the 
	rate with an XML element.  

Arguments: 
			pElement: a pointer to the XML element to load the information from.
//////////////////////////////////////////////////////////////////////////////*/
ReaclibRate(TiXmlElement *pElement);

/*//////////////////////////////////////////////////////////////////////////////
Function: ~ReaclibRate

Purpose: This function is the default destructor for the ReaclibRate class.  
//////////////////////////////////////////////////////////////////////////////*/
~ReaclibRate();

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

void addVersion(ReaclibVer *newVer, bool bSkipLink = false);

// Equation
const char *getEqn();
void setEqn(const char *pstrValue);

const nuclide_t *getNuclide(int location);

bool isReactant(int location);

int getVersionSize();

TiXmlElement *getXMLLink();

ReaclibVer *getVer(unsigned int);
ReaclibVer *getCurrentVersion();

float getReverseRatio(int version=-1);
float getDeltaA0(int version=-1);
int getChapter();

private:
	// the element in the XML file
	TiXmlElement *m_pRate;

	// the chapter number
	int m_chapter;
	
	// the equation
	char *m_strEqn;
	
	// the nuclides involved in this equation
	const nuclide_t *m_nuclides[6];
	bool m_reactant[6];
	
	// the count of nuclides
	int m_nuclideCount;
	
	// the list of different versions
	vector<ReaclibVer*> m_versions;
};
#endif
