/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibReader.h
////////////////////////////////////////////////////////////////////////////////
Function: This file contains the implementation for the ReaclibReader class.
	The reaclibReader class provides a way to interface with XML files created 
	by the REACLIB database (http://www.nscl.msu.edu/~nero/db/) and in the 
	future interface with the reaclib database directly.
	
Versions:
	0.0.0: 2008-03-24 Scott Warren (warren@nscl.msu.edu)
		Created file.
	0.0.1: 2008-04-01 Scott Warren (warren@nscl.msu.edu)
		Reader successfully stores information internally from XML/REACLIB file.
		
Future Versions:
	0.1.0: 2008-??-?? Scott Warren (warren@nscl.msu.edu)
		Reader allows basic functionality including search.
	0.2.0: 2008-??-?? Scott Warren (warren@nscl.msu.edu)
		Reader allows for automatic update via website for rates currently in 
		XML/REACLIB file. 
	0.3.0: 2008-??-?? Scott Warren (warren@nscl.msu.edu)
		Reader allows for automatic updates of new rates that have been	entered 
		in database without redownloading.
	1.0.0: 2008-??-?? Scott Warren (warren@nscl.msu.edu)
		Reader allows for users to modify REACLIB database directly (with 
		authentication).
//////////////////////////////////////////////////////////////////////////////*/

// make sure this is included only once
#ifndef __reaclibReader_h__
#define __reaclibReader_h__

#include "tinyxml.h"
#include "reaclibRate.h"
#include "reaclibReaderStructs.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
using namespace std;

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////// Constants ////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/
const char reaclibReaderErrorsUnableToOpenFile[] = "Unable to open file";
const char reaclibReaderErrorsUnableToFindRatesTag[] = "Unable to find rates tag";

bool compareNuclide(nuclide_t const *x, nuclide_t const *y);

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////// ReaclibReader ////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/
class ReaclibReader{
public:
/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibReader

Purpose: This function is the default constructor for the ReaclibReader class.  
//////////////////////////////////////////////////////////////////////////////*/
ReaclibReader();
	
/*//////////////////////////////////////////////////////////////////////////////
Function: ~ReaclibReader

Purpose: This function is the default destructor for the ReaclibReader class.  
//////////////////////////////////////////////////////////////////////////////*/
~ReaclibReader();

void addRate(ReaclibRate *newRate, bool bSkipLink = false);

/*//////////////////////////////////////////////////////////////////////////////
Function: LoadREACIB

Purpose: This function loads data directly from a REACLIB file.

Arguments: 
			strFilename: The name of the file to load
			
Returns: Bool value indicating the success or failure of the load.
//////////////////////////////////////////////////////////////////////////////*/
bool LoadREACLIB(const char* strFilename);

/*//////////////////////////////////////////////////////////////////////////////
Function: LoadXML

Purpose: This function loads data directly from an XML file.

Arguments: 
			strFilename: The name of the file to load
			
Returns: Bool value indicating the success or failure of the load.
//////////////////////////////////////////////////////////////////////////////*/
bool LoadXML(const char* strFilename);

/*//////////////////////////////////////////////////////////////////////////////
Function: SaveREACLIB

Purpose: This function saves the data to a REACLIB file.

Arguments: 
			strFilename: The name of the file to save to
Returns: Bool value indicating the success or failure of the save.
//////////////////////////////////////////////////////////////////////////////*/
bool SaveREACLIB(const char* strFilename);

/*//////////////////////////////////////////////////////////////////////////////
Function: SaveXML

Purpose: This function saves the data to an XML file.

Arguments: 
			strFilename: The name of the file to save to
Returns: Bool value indicating the success or failure of the save.
//////////////////////////////////////////////////////////////////////////////*/
bool SaveXML(const char* strFilename);

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////////////
Function: getRate

Purpose: This function returns a pointer to a specific rate

Arguments: 
			i: the element to return
Returns: a pointer to rate
//////////////////////////////////////////////////////////////////////////////*/
ReaclibRate *getRate(unsigned int);

/*//////////////////////////////////////////////////////////////////////////////
Function: getRateSize

Purpose: This function returns the number of elements in the rates vector

Arguments: 
Returns: integer value of the count
//////////////////////////////////////////////////////////////////////////////*/
int getRateSize();

/*//////////////////////////////////////////////////////////////////////////////
Function: search

Purpose: This function performs the search on the rates

Arguments: 
			reader: the reader to save the information in
			reaclibSearch: the search terms
			linkReader: if the results reader should update the master
Returns: a boolean value that represents if there was a success
//////////////////////////////////////////////////////////////////////////////*/
bool search(ReaclibReader &reader, reaclibSearch &search, bool linkReader);

private:
	// the XML document
	TiXmlDocument *m_XML;
	
	// the error string
	const char *m_error;
	
	// the rates element
	TiXmlElement *m_pObjRates;
	
	// the rates
	vector<ReaclibRate*>  m_rates;
};

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Helper Functions /////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////////////
Function: createEquationFromChapterNucs

Purpose: This function converts a chapter number and a listing of nuclides to
		the equation they represent.

Arguments: 
			chapter: the chapter number
			nuc1-nuc6: the nuclides
Returns: a string that contains the equation
//////////////////////////////////////////////////////////////////////////////*/
string createEquationFromChapterNucs(int chapter, string &nuc1, string &nuc2, 
	string &nuc3, string &nuc4, string &nuc5, string &nuc6);

/*//////////////////////////////////////////////////////////////////////////////
Function: findChapter

Purpose: This function determines the chapter of an equation.

Arguments: 
			intReact: the count of nuclide in the products
			intProd: the count of nuclide in the products
Returns: an integer that represents the chapter number or 0 if we can't find
		 the chapter.
//////////////////////////////////////////////////////////////////////////////*/
int findChapter(int intReact, int intProd);

/*//////////////////////////////////////////////////////////////////////////////
Function: initReaclibSearch

Purpose: This function initializes the search structure

Arguments: 
			search: the structure to set up
Returns: nothing
//////////////////////////////////////////////////////////////////////////////*/
void initReaclibSearch(reaclibSearch &search);
	
/*//////////////////////////////////////////////////////////////////////////////
Function: trim

Purpose: This function removes the extra spaces before and after a string

Arguments: 
			oldstr: the string to trim
Returns: a string that oldstr minus the spaces on the left and right
//////////////////////////////////////////////////////////////////////////////*/
string trim(string oldstr);

#endif
