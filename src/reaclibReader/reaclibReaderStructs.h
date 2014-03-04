/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibReaderStructs.h.h
////////////////////////////////////////////////////////////////////////////////
Function: This file contains the structures for the ReaclibReader classes. 
	
Versions:
	0.0.0: 2008-03-27 Scott Warren (warren@nscl.msu.edu)
		Created file.
//////////////////////////////////////////////////////////////////////////////*/

#ifndef __reaclibReaderStructs_h__
#define __reaclibReaderStructs_h__

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Structures ///////////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

// this is the structure with the various search terms that can be searched
struct reaclibSearch {
	int intMax_z1, intMin_z1, intMax_m1, intMin_m1;
	int intMax_z2, intMin_z2, intMax_m2, intMin_m2;
	int intMax_z3, intMin_z3, intMax_m3, intMin_m3;
	int intMax_z4, intMin_z4, intMax_m4, intMin_m4;
	int intMax_z5, intMin_z5, intMax_m5, intMin_m5;
	int intMax_z6, intMin_z6, intMax_m6, intMin_m6;
	
	char *label;
	char res,dir;
	
	float min_Q, max_Q;
	
	bool allRates;
};



#endif
