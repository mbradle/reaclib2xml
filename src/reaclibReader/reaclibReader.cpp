/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibReader.cpp
////////////////////////////////////////////////////////////////////////////////
See reaclibReader.h for more information about this class.
//////////////////////////////////////////////////////////////////////////////*/

#include "reaclibReader.h"

#include <iostream>
using namespace std;

ReaclibReader::ReaclibReader(){
	m_XML = new TiXmlDocument;
	TiXmlDeclaration *dec = new TiXmlDeclaration("1.0", "UTF-8","");
	m_XML->LinkEndChild(dec);
	m_pObjRates = new TiXmlElement("rates");
	m_XML->LinkEndChild(m_pObjRates);
}


ReaclibReader::~ReaclibReader(){
	// check to see if the XML file is valid
	if(m_XML){
		
	}
}

void ReaclibReader::addRate(ReaclibRate *newRate, bool bSkipLink){
	// check to see if the link is already defined
	if((bSkipLink == false) && (m_XML!=NULL)){
		m_pObjRates->LinkEndChild(newRate->getXMLLink());
	}
	
	m_rates.push_back(newRate);
}

bool ReaclibReader::LoadREACLIB(const char* strFilename){
	// open the file
	ifstream objFile;
	objFile.open(strFilename, ifstream::in);
	
	// check to make sure the file opened
	if(!objFile.is_open()){
		return false;
	}
	
	// the lines to hold the information in
	string line1;
	string line2;
	string line3;
	
	// the current chapter
	int chapter = 0;
	
	// the last equation and label
	string strLastEqn;
	string strLastLabel;
	
	// the last rate and version
	ReaclibRate *objRate = NULL;
	ReaclibVer *objVer = NULL;
	
	int intLine = 0;
	
	// loop through the results
	while(! objFile.eof()){
		getline(objFile,line1);
		getline(objFile,line2);
		getline(objFile,line3);
		
		intLine += 3;
		
		// check to see if this is a chapter line
		if(line1.substr(0,5) != string("     ")){
			// get the chapter
			chapter = atoi(line1.substr(0,5).c_str());
		} else {
			string nuc0;
			string nuc1;
			string nuc2;
			string nuc3;
			string nuc4;
			string nuc5;
			
			string label;
			char res;
			char dir;
			float Q;
			
			string eqn;
		
			// get the information out the first line
			// get the nucleuses
			nuc0 = trim(line1.substr(5,5));
			nuc1 = trim(line1.substr(10,5));
			nuc2 = trim(line1.substr(15,5));
			nuc3 = trim(line1.substr(20,5));
			nuc4 = trim(line1.substr(25,5));
			nuc5 = trim(line1.substr(30,5));
			
			// the label
			label = trim(line1.substr(43,4));
			
			// the res and dir 
			res = line1.substr(47,1).c_str()[0];
			dir = line1.substr(48,1).c_str()[0];
			
			// convert the Q value to a float
			if(EOF == sscanf(line1.substr(52,12).c_str(),"%f", &Q)){
				cerr << "Unable to convert Q value: " << line1.substr(52,12) 
					<< endl;
				return false;
			}
			
			// get out all the a0-a6 values
			float num0, num1, num2, num3, num4, num5, num6;
			if(EOF == sscanf(line2.substr(0,13).c_str(), "%f", &num0)){
				cerr << "Unable to convert num0 value: " << line2.substr(0,13) 
					<< endl;
				return false;
			}
			
			if(EOF == sscanf(line2.substr(13,13).c_str(), "%f", &num1)){
				cerr << "Unable to convert num1 value: " << line2.substr(13,13)
					<< endl;
				return false;
			}
			
			if(EOF == sscanf(line2.substr(26,13).c_str(), "%f", &num2)){
				cerr << "Unable to convert num2 value: " << line2.substr(26,13) 
					<< endl;
				return false;
			}
			
			if(EOF == sscanf(line2.substr(39,13).c_str(), "%f", &num3)){
				cerr << "Unable to convert num3 value: " << line2.substr(39,13) 
					<< endl;
				return false;
			}
			
			if(EOF == sscanf(line3.substr(0,13).c_str(), "%f", &num4)){
				cerr << "Unable to convert num4 value: " << line3.substr(0,13) 
					<< endl;
				return false;
			}
			
			if(EOF == sscanf(line3.substr(13,13).c_str(), "%f", &num5)){
				cerr << "Unable to convert num5 value: " << line3.substr(13,13)
					<< endl;
				return false;
			}
			
			if(EOF == sscanf(line3.substr(26,13).c_str(), "%f", &num6)){
				cerr << "Unable to convert num6 value: " << line3.substr(26,13) 
					<< endl;
				return false;
			}
			
			#ifdef DEBUG2
			cout << nuc0 << nuc1 << nuc2 << nuc3 << nuc4 << nuc5 << label 
				<< res << dir << Q << endl;
			cout << num0 << " " << num1 << " " << num2 << " " << num3 << " " 
				<< endl;
			cout << num4 << " " << num5 << " " << num6 << endl; 
			#endif
			
			// get the equation
			eqn = createEquationFromChapterNucs(chapter, nuc0, nuc1, nuc2, nuc3,
			nuc4, nuc5);
			
			// check to see if this is a new equation
			if(eqn != strLastEqn){
				// attempt to create the new rate
				try {
					objRate = new ReaclibRate();
				} catch (bad_alloc &){
					cerr << "Unable to allocate memory for new ReaclibRate in "
						<< __FILE__ << " on line: " << __LINE__ << endl;
					return false;
				}
				
				// add the rate
				addRate(objRate);
				
				// add the information
				objRate->setEqn(eqn.c_str());
			}
			
			// check to see if this is a new label
			if((label != strLastLabel) or (eqn!=strLastEqn)){
				// create the new version
				try{
					objVer = new ReaclibVer();
				} catch ( bad_alloc &) {
					cerr << "Unable to allocate memory for new ReaclibVer in " 
						<< __FILE__ << " on line: " << __LINE__ << endl;
					return false;
				}
				
				// add the version
				objRate->addVersion(objVer);
				
				// set the information
				objVer->setQ(Q);
			}
			
			ReaclibSet *objSet;
			
			// create the new set
			try{
				objSet = new ReaclibSet();
			} catch (bad_alloc&){
				cerr << "Unable to allocate memory for new ReaclibSet in " 
					<< __FILE__ << " on line: " << __LINE__ << endl;
				return false;
			}
			
			// add the set
			objVer->addSet(objSet);
			
			// set the sets information
			objSet->setA0(num0);
			objSet->setA1(num1);
			objSet->setA2(num2);
			objSet->setA3(num3);
			objSet->setA4(num4);
			objSet->setA5(num5);
			objSet->setA6(num6);
			
			objSet->setLabel(label.c_str());
			objSet->setRes(res);
			objSet->setDir(dir);
			
			// save the eqn
			strLastEqn = eqn;
			strLastLabel = label;
		} 
	}
	
	
	objFile.close();
	
	return true;
}

bool ReaclibReader::LoadXML(const char* strFilename){
	#ifdef DEBUG2
		cerr << "Loading XML file: " << strFilename << endl;
	#endif
	
	// set the correct filename
	m_XML = new TiXmlDocument(strFilename);
	
	// load the file and check for errors
	if(!m_XML->LoadFile()){
		#ifdef DEBUG
			cerr << reaclibReaderErrorsUnableToOpenFile << endl;
		#endif
		
		m_error = reaclibReaderErrorsUnableToOpenFile;
		return false;
	}
	
	// get the root element
	m_pObjRates = m_XML->FirstChildElement("rates");
	
	// check to make sure we got the element
	if(!m_pObjRates) {
		#ifdef DEBUG
		cerr << reaclibReaderErrorsUnableToFindRatesTag << endl;
		#endif
		
		m_error = reaclibReaderErrorsUnableToFindRatesTag;
		return false;
	}
	// loop through all the rates
	for(TiXmlNode *pChild = m_pObjRates->FirstChild(); pChild != NULL; 
		pChild = pChild->NextSibling()){
		// check to see if this a rate and an element
		if(pChild->Type() == TiXmlNode::ELEMENT){
			// convert this to an element
			TiXmlElement *pElement = pChild->ToElement();
			
			// check to see if this is a rate element
			if(strcmp(pElement->Value(),"rate") == 0){
				// create the new rate
				ReaclibRate *newRate = new ReaclibRate(pElement);
				
				// add the new rate
				addRate(newRate,true);
			}
		}
	}	
	
	#ifdef DEBUG2
		cerr << "Finished loading XML File. " << endl;
	#endif
	return true;
}

bool ReaclibReader::SaveXML(const char* strFilename){
	// make sure we have a valid XML document
	if(m_XML != NULL){
		return m_XML->SaveFile(strFilename);
	}
	
	return false;
}

bool ReaclibReader::SaveREACLIB(const char* strFilename){
	// open the file
	FILE *objFile = fopen(strFilename, "w");
	
	// check to make sure the file opened correctly
	if(objFile == NULL){
		#ifdef DEBUG
		cerr << "Unable to open file.\n";
		#endif
		return false;
	}
	
	// the old chapter
	int intOldChap = -1;
	
	// loop through all the rates
	vector<ReaclibRate*>::iterator iter;
	for(iter = m_rates.begin(); iter != m_rates.end(); iter++){
		ReaclibRate *objRate = *iter;
		
		// get the number of versions
		int intVerSize = objRate->getVersionSize();
		
		// loop through all the versions
		for(int intVer = 0; intVer < intVerSize; intVer++){
			ReaclibVer *objVer = objRate->getVer(intVer);
						
			// get the number sets
			int intSetSize = objVer->getSetSize();
			
			// loop through all the sets
			for(int intSet = 0; intSet < intSetSize; intSet++){
				// get the set
				ReaclibSet *objSet = objVer->getSet(intSet);
				
				// get the equation
				string strEqn = objRate->getEqn();
				
				int intReact = 0;
				int intProd = 0;
				
				// get the count of reactants and products
				unsigned intCount = 0;
				const nuclide_t *nuc = objRate->getNuclide(intCount); 
				
				// loop through all the nuclides
				while(nuc != NULL){
					// check to see if this is a reactant
					if(objRate->isReactant(intCount)){
						intReact++;
					} else {
						intProd++;
					}
					
					intCount++;
					nuc = objRate->getNuclide(intCount);
				}
				
				// calcuate the new chapter
				int intChap = findChapter(intReact, intProd);
				
				// check to see if this is a new chapter
				if(intChap != intOldChap){
					// output the chapter entry
					string temp = "";
					temp.append(73-(int)floor(log10(intChap)), ' ');
					fprintf(objFile, "%d%s\n", intChap, temp.c_str());
					fprintf(objFile, "%74s\n", " ");
					fprintf(objFile, "%74s\n", " ");
					intOldChap = intChap;
				}
				
				// start the first line
				fprintf(objFile,"%5s", " ");
				
				// loop through the elements
				intCount = 0;
				nuc = objRate->getNuclide(intCount); 
				while(nuc != NULL){
					
					fprintf(objFile,"%5s", nuc->comp);
					intCount++;
					nuc = objRate->getNuclide(intCount);
				}
				
				// add the extra spaces
				for(;intCount<6; intCount++){
					fprintf(objFile, "%5s", " ");
				}
				
				// output the label/res/dir/Q fields
				fprintf(objFile, "%8s%-4s%c%c   %c%-.5e", " ", 
					objSet->getLabel(), objSet->getRes(), objSet->getDir(), 
					(objVer->getQ()<0?'-':' '),
					sqrt(objVer->getQ()*objVer->getQ()));
				
				// end of first line
				fprintf(objFile, "%10s\n", " ");
				
				// print out the second line
				fprintf(objFile, "%c%-.6e%c%-.6e%c%-.6e%c%-.6e%22s\n", 
					(objSet->getA0()<0?'-':' '), 
					sqrt(objSet->getA0() * objSet->getA0()), 
					(objSet->getA1()<0?'-':' '), 
					sqrt(objSet->getA1() * objSet->getA1()), 
					(objSet->getA2()<0?'-':' '), 
					sqrt(objSet->getA2() * objSet->getA2()), 
					(objSet->getA3()<0?'-':' '), 
					sqrt(objSet->getA3() * objSet->getA3()), " ");
				fprintf(objFile, "%c%-.6e%c%-.6e%c%-.6e%35s\n", 
					(objSet->getA4()<0?'-':' '), 
					sqrt(objSet->getA4() * objSet->getA4()), 
					(objSet->getA5()<0?'-':' '), 
					sqrt(objSet->getA5() * objSet->getA5()), 
					(objSet->getA6()<0?'-':' '), 
					sqrt(objSet->getA6() * objSet->getA6()), " ");
			}
		}
	}
	
	// close the file
	fclose(objFile);
	
	return true;
}

ReaclibRate *ReaclibReader::getRate(unsigned i){
	// check to make sure this is a valid rate
	if((i >= 0) && (i < (unsigned)getRateSize())){
		return m_rates[i];
	}
	
	return NULL;
}

int ReaclibReader::getRateSize(){
	return m_rates.size();
}

bool ReaclibReader::search(ReaclibReader &reader, reaclibSearch &search, 
	bool linkReader){
	// check to make sure this search results haven't already been added
	if(reader.getRateSize() != 0){
		return false;
	}
	
	// loop through all the rates
	for(int intCurrentRate = 0; intCurrentRate < getRateSize(); intCurrentRate++){
		// get the rate
		ReaclibRate *objRate = getRate(intCurrentRate);
		
		// loop through all the versions
		for(int intCurrentVer = 0; intCurrentVer < objRate->getVersionSize(); intCurrentVer++){
			// if this is a match
			bool isMatch = false;
		
			// get the version
			ReaclibVer *objVer = objRate->getVer(intCurrentVer);
			
			// loop through all the sets
			for(int intCurrentSet = 0; intCurrentSet < objVer->getSetSize(); intCurrentSet++){
				// get the set
				ReaclibSet *objSet = objVer->getSet(intCurrentSet);
			}
			
			// check to see if this is a match
			if(isMatch){
				cout << "Match...";
				// todo: add code to add the match
			}
		}
	}
	
	return true;
	// we want all the default z and m values to be -1
	// search.intMax_z1 = search.intMin_z1 = -1;
	// search.intMax_m1 = search.intMin_m1 = -1;
	// search.intMax_z2 = search.intMin_z2 = -1;
	// search.intMax_m2 = search.intMin_m2 = -1;
	// search.intMax_z3 = search.intMin_z3 = -1;
	// search.intMax_m3 = search.intMin_m3 = -1;
	// search.intMax_z4 = search.intMin_z4 = -1;
	// search.intMax_m4 = search.intMin_m4 = -1;
	// search.intMax_z5 = search.intMin_z5 = -1;
	// search.intMax_m5 = search.intMin_m5 = -1;
	// search.intMax_z6 = search.intMin_z6 = -1;
	// search.intMax_m6 = search.intMin_m6 = -1;
	
	// no label
	//search.label = NULL;
	
	// any res or dir flag
	//search.res = search.dir = '*';
	
	// Q value of anything
	//search.min_Q = search.max_Q = -1;
	
	// only show the current or first rate
	//search.allRates = false;
}

string createEquationFromChapterNucs(int chapter, string &nuc0, string &nuc1, 
	string &nuc2, string &nuc3, string &nuc4, string &nuc5){
	
	// determine the format based on the chapter number
	switch(chapter){
		case(1):
			return string(nuc0 + " -> " + nuc1);
			break;
		case(2):
			return string(nuc0 + " -> " + nuc1 + " + " + nuc2);
			break;
		case(3):
			return string(nuc0 + " -> " + nuc1 + " + " + nuc2 + " + " + nuc3);
			break;
		case(4):
			return string(nuc0 + " + " + nuc1 + " -> " + nuc2);
			break;
		case(5):
			return string(nuc0 + " + " + nuc1 + " -> " + nuc2 + " + " + nuc3);
			break;
		case(6):
			return string(nuc0 + " + " + nuc1 + " -> " + nuc2 + " + " + nuc3 
				+ " + " + nuc4);
			break;
		case(7):
			return string(nuc0 + " + " + nuc1 + " -> " + nuc2 + " + " + nuc3 
				+ " + " + nuc4 + " + " + nuc5);
			break;
		case(8):
			// check to see if this is an old format chapter 8
			if(nuc4 == ""){
				return string(nuc0 + " + " + nuc1 + " + " + nuc2 + " -> " 
					+ nuc3);
			} else {
				return string(nuc0 + " + " + nuc1 + " + " + nuc2 + " -> "
					+ nuc3 + " + " + nuc4);
			}
			break;
		case(9):
			return string(nuc0 + " + " + nuc1 + " + " + nuc2 + " -> " + nuc3 
				+ " + " + nuc4);
			break;
		case(10):
			return string(nuc0 + " + " + nuc1 + " + " + nuc2 + " + " + nuc3 
				+ " -> " + nuc4 + " + " + nuc5);
			break;
		case(11):
			return string(nuc0 + " -> " + nuc1 + " + " + nuc2 + " + " + nuc3 
				+ " + " + nuc4);
			break;
		default:
			cerr << chapter << " unknown chapter in "
				<< "createEquationFromChapterNucs()\n";
			break;
	};
	
	return string("");
}

void initReaclibSearch(reaclibSearch &search){
	// we want all the default z and m values to be -1
	search.intMax_z1 = search.intMin_z1 = -1;
	search.intMax_m1 = search.intMin_m1 = -1;
	search.intMax_z2 = search.intMin_z2 = -1;
	search.intMax_m2 = search.intMin_m2 = -1;
	search.intMax_z3 = search.intMin_z3 = -1;
	search.intMax_m3 = search.intMin_m3 = -1;
	search.intMax_z4 = search.intMin_z4 = -1;
	search.intMax_m4 = search.intMin_m4 = -1;
	search.intMax_z5 = search.intMin_z5 = -1;
	search.intMax_m5 = search.intMin_m5 = -1;
	search.intMax_z6 = search.intMin_z6 = -1;
	search.intMax_m6 = search.intMin_m6 = -1;
	
	// no label
	search.label = NULL;
	
	// any res or dir flag
	search.res = search.dir = '*';
	
	// Q value of anything
	search.min_Q = search.max_Q = -1;
	
	// only show the current or first rate
	search.allRates = false;
	
	return;
}

int findChapter(int intReact, int intProd){
	// determine the chapter number
	if((intReact == 1) and (intProd == 1)){
		// chapter 1 (1 -> 1)
		return 1;
	} else if((intReact == 1) and (intProd == 2)){
		// chapter 2 (1 -> 2)
		return 2;
	} else if((intReact == 1) and (intProd == 3)){
		// chapter 3 (1 -> 3)
		return 3;
	} else if((intReact == 2) and (intProd == 1)){
		// chapter 4 (2 -> 1)
		return 4;
	} else if((intReact == 2) and (intProd == 2)){
		// chapter 5 (2 -> 2)
		return 5;
	} else if((intReact == 2) and (intProd == 3)){
		// chapter 6 (2 -> 3)
		return 6;
	} else if((intReact == 2) and (intProd == 4)){
		// chapter 7 (2 -> 4)
		return 7;
	} else if((intReact == 3) and (intProd == 1)){
		// chapter 8 (3 -> 1)
		return 8;
	} else if((intReact == 3) and (intProd == 2)){
		// chapter 9 (3 -> 2)
		return 9;
	} else if((intReact == 4) and (intProd == 2)){
		// chapter 10 (4 -> 2)
		return 10;
	} else if((intReact == 1) and (intProd == 4)){
		// chapter 11 (1 -> 4)
		return 11;
	} 
	return 1;
}

string trim(string oldstr){
	// remove the end set of blanks
	string returnVal = oldstr.erase(oldstr.find_last_not_of(" ")+1);
	
	// remove the first set of blanks
	return returnVal.erase(0, oldstr.find_first_not_of(" "));
}
