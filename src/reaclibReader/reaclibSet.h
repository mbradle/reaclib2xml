/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibSet.h
////////////////////////////////////////////////////////////////////////////////
Function: This file contains the implementation for the ReaclibReader sets 
	class. The reaclibRate class provides the functionality for the rates.
	
Versions:
	0.0.0: 2008-03-24 Scott Warren (warren@nscl.msu.edu)
		Created file.
//////////////////////////////////////////////////////////////////////////////*/

#ifndef __reaclibSet_h__
#define __reaclibSet_h__

#include "tinyxml.h"
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class ReaclibSet {
public:
/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibSet

Purpose: This function is the default constructor for the ReaclibSet class.
//////////////////////////////////////////////////////////////////////////////*/
ReaclibSet();

/*//////////////////////////////////////////////////////////////////////////////
Function: ReaclibSet

Purpose: This function is the XML constructor for the ReaclibSet class.  

Arguments: 
			pElement: a pointer to the XML element to load the information from.
//////////////////////////////////////////////////////////////////////////////*/
ReaclibSet(TiXmlElement *pElement);
	
/*//////////////////////////////////////////////////////////////////////////////
Function: ~ReaclibSet

Purpose: This function is the default destructor for the ReaclibSet class.  
//////////////////////////////////////////////////////////////////////////////*/
~ReaclibSet();

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

// a0
float getA0();
void setA0(float a0);

// a1
float getA1();
void setA1(float a1);

// a2
float getA2();
void setA2(float a2);

// a3
float getA3();
void setA3(float a3);

// a4
float getA4();
void setA4(float a4);

// a5
float getA5();
void setA5(float a5);

// a6
float getA6();
void setA6(float a6);

// dir
char getDir();
void setDir(char dir);

// id
int getID();
void setID(int id);

// label
const char *getLabel();
void setLabel(const char *label);

// res
char getRes();
void setRes(char res);

TiXmlElement *getXMLLink();

private:
	// the element in the XML file
	TiXmlElement *m_pSet;

	// the id of the set
	int m_id;
	
	// the a values
	float m_a0;
	float m_a1;
	float m_a2;
	float m_a3;
	float m_a4;
	float m_a5;
	float m_a6;
	
	// flag fields
	char m_res;
	char m_dir;
	
	// the label
	char *m_pLabel;
};

#endif
