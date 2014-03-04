/*//////////////////////////////////////////////////////////////////////////////
Filename: NuclideSearch.h
////////////////////////////////////////////////////////////////////////////////
Function: This file contains the implementation for the NuclideSearch class.
	The NuclideSearch class allows a way to find information about a nuclide
	when it's information is not known.  The information is retrived from an XML
	file at http://www.nscl.msu.edu/~nero/db/.
	
Versions:
	0.0.0: 2008-04-11 Scott Warren (warren@nscl.msu.edu)
		Created file.
//////////////////////////////////////////////////////////////////////////////*/

// make sure this is included only once
#ifndef __NuclideSearch_h__
#define __NuclideSearch_h__

#include <iostream>
#include <vector>
#include <algorithm>
#include "tinyxml.h"
using namespace std;

// this structure holds the information about a nuclide
typedef struct{
	char *comp;
	char *nuc;
	int zvalue;
	int mvalue;
	float jvalue;
} nuclide_t;

bool compareNuclide(nuclide_t const *x, nuclide_t const *y);

class NuclideSearch{
public:
static NuclideSearch *newInstance();

/*//////////////////////////////////////////////////////////////////////////////
Function: loadNuclideFile

Purpose: This function loads a nuclide file.

Arguments: 
			strFilename: the filename to load
Returns: the boolean value that indicates if the data was loaded successfully.
//////////////////////////////////////////////////////////////////////////////*/
bool loadNuclideFile(char *strFilename);

/*//////////////////////////////////////////////////////////////////////////////
Function: getNuclide

Purpose: This function returns a pointer to the information about a nuclide

Arguments: 
			nuc: the nuclide
Returns: a pointer to nuclide or NULL is nothing was found
//////////////////////////////////////////////////////////////////////////////*/
const nuclide_t *getNuclide(const char *);
	
protected:
NuclideSearch();
private:
// the instance of the single instance of this class
static NuclideSearch* m_instance;

// the nuclide listing
vector<nuclide_t*> m_nuclides;
};

#endif
