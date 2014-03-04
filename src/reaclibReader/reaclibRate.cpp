/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibRate.cpp
////////////////////////////////////////////////////////////////////////////////
See reaclibRate.h for more information about this class.
//////////////////////////////////////////////////////////////////////////////*/

#include "reaclibRate.h"

ReaclibRate::ReaclibRate(){
	m_strEqn = NULL;
	m_pRate = new TiXmlElement("rate");
	m_nuclides[0] = NULL;
	m_nuclides[1] = NULL;
	m_nuclides[2] = NULL;
	m_nuclides[3] = NULL;
	m_nuclides[4] = NULL;
	m_nuclides[5] = NULL;
	m_chapter = 0;
}

ReaclibRate::ReaclibRate(TiXmlElement *pElement){
	m_strEqn = NULL;
	m_pRate = pElement;
	m_nuclides[0] = NULL;
	m_nuclides[1] = NULL;
	m_nuclides[2] = NULL;
	m_nuclides[3] = NULL;
	m_nuclides[4] = NULL;
	m_nuclides[5] = NULL;
	m_chapter = 0;
	
	// loop through all the elements children
	for(TiXmlNode *pNode = pElement->FirstChild(); pNode; pNode=pNode->NextSibling()){
		// check to see if this is another node
		if(pNode->Type() == TiXmlNode::ELEMENT){
			// convert this to a type
			TiXmlElement *pElement = pNode->ToElement();
			
			// check to see if this is a known type
			if(strcmp(pElement->Value(),"ver") ==0){
				// create the new version
				ReaclibVer* newVer = new ReaclibVer(pElement);
				
				// add the version to the listing
				addVersion(newVer, true);
			}
		}
	}
	
	// read in the attributes
	// get the equation
	const char *pstrValue;
	if((pstrValue = m_pRate->Attribute("eqn"))){
		setEqn(pstrValue);
	}
}

ReaclibRate::~ReaclibRate(){
	// delete elements as needed
	if(m_strEqn){
		delete[] m_strEqn;
		m_strEqn = NULL;
	}
}

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

void ReaclibRate::addVersion(ReaclibVer *newVer, bool bSkipLink){
	// check to see if we can link this
	if((bSkipLink == false) && (m_pRate!=NULL)){
		m_pRate->LinkEndChild(newVer->getXMLLink());
	}
	m_versions.push_back(newVer);
}

int ReaclibRate::getChapter(){
	return m_chapter;
}

ReaclibVer *ReaclibRate::getCurrentVersion(){
	ReaclibVer *version = NULL;
	
	// loop through all the versions
	for(unsigned int i = 0; i<m_versions.size(); i++){
		// get the version
		version = m_versions[i];
		
		// check to see if this is the current version
		if(version->isCurrent()){
			return version;
		}
	}
	
	return version;
}

float ReaclibRate::getDeltaA0(int version){
	return log(getReverseRatio(version));
}

// Equation
const char *ReaclibRate::getEqn(){
	return m_strEqn;
}

bool ReaclibRate::isReactant(int location){
	// check to make sure this is a valid location
	if((location < 0) || (location > m_nuclideCount)){
		return false;
	}
	
	return m_reactant[location];
}

void ReaclibRate::setEqn(const char *pstrValue){
	// check to see if the equation has information already
	if(m_strEqn){
		delete[] m_strEqn;
		m_strEqn = NULL;
	}

	// allocate space for the new information
	try {
		m_strEqn = new char[strlen(pstrValue)+1];
	} catch (bad_alloc &) {
		#ifdef DEBUG
		cerr << "Unable to allocate the space for the equation";
		#endif
		return;
	}
	
	// copy of the value
	strcpy(m_strEqn, pstrValue);
	
	// check to see if the link between the xml file exists
	if(m_pRate){
		m_pRate->SetAttribute("eqn", m_strEqn);
	}

	// save a copy of the equation
	string eqn = m_strEqn;
	
	// replace the -> with a plus
	// first find the ->
	string::size_type loc = eqn.find("->");
	
	// check to see if we were able to find this
	if(loc == string::npos){
		#ifdef DEBUG
		cerr << "Unable to find -> in " << eqn << endl;
		#endif
		return;
	}
	
	// break the reactant and products
	string reactants = eqn.substr(0, loc-1);
	string products = eqn.substr(loc+3);
	
	// break apart equation into it's parts
	vector<string> nuc;
	
	m_nuclideCount = 0;
	// loop through the reactants
	while((loc = reactants.find(" + ")) != string::npos){
		string strTemp = reactants.substr(0,loc);
		reactants = reactants.substr(loc+3);
		
		// add the nuclide to the vector
		nuc.push_back(strTemp);
		m_reactant[m_nuclideCount++] = true;
	}
	nuc.push_back(reactants);
	m_reactant[m_nuclideCount++] = true;
	
	int reactCount = m_nuclideCount;
	
	while((loc = products.find(" + ")) != string::npos){
		string strTemp = products.substr(0,loc);
		products = products.substr(loc+3);
		
		// add the nuclide to the vector
		nuc.push_back(strTemp);
		m_reactant[m_nuclideCount++] = false;
	}
	nuc.push_back(products);
	m_reactant[m_nuclideCount++] = false;
	
	// we need a copy of the nuclidesearch
	NuclideSearch *nucSearch = NuclideSearch::newInstance();
	
	m_nuclideCount = 0;
	// loop through the nuclides
	for(vector<string>::iterator iter = nuc.begin(); iter != nuc.end(); iter++){
		// find the nuclide
		const nuclide_t* nuc = nucSearch->getNuclide(((string)*iter).c_str());
		
		// check to see if there was an error
		if(nuc == NULL){
			cerr << "Unable to find " << *iter << " in " << eqn <<  endl;
		}
		
		// save the nuclide information
		m_nuclides[m_nuclideCount++] = nuc;
	}
	
	// set the chapter number
	if(m_nuclideCount == 2){
		m_chapter=1;
	}else if(m_nuclideCount == 3){
		if(reactCount == 1){
			m_chapter=2;
		} else if(reactCount ==2){
			m_chapter=4;
		} else {
			m_chapter=-1;
		}
	}else if(m_nuclideCount == 4){
		if(reactCount == 1){
			m_chapter=3;
		} else if(reactCount ==2){
			m_chapter=5;
		} else if(reactCount ==3){
			m_chapter=8;
		} else {
			m_chapter=-1;
		}
	}else if(m_nuclideCount == 5){
		if(reactCount ==2){
			m_chapter=6;
		} else if(reactCount ==3){
			m_chapter=9;
		} else if(reactCount ==1){
			m_chapter=11;
		} else {
			m_chapter=-1;
		}
	}else if(m_nuclideCount == 6){
		if(reactCount ==2){
			m_chapter=7;
		} else if(reactCount ==4){
			m_chapter=10;
		} else {
			m_chapter=-1;
		}
	} else {
		#ifdef DEBUG
		cerr << "Unhandled nuclide count: " << m_nuclideCount << endl;
		#endif
		m_chapter = -1;
	}
	
}

int ReaclibRate::getVersionSize(){
	return m_versions.size();
}

TiXmlElement *ReaclibRate::getXMLLink(){
	return m_pRate;
}

const nuclide_t *ReaclibRate::getNuclide(int location){
	// check to make sure this is a valid location
	if((location < 0) || (location >= m_nuclideCount)){
		return NULL;
	}
	
	return m_nuclides[location];
}

float ReaclibRate::getReverseRatio(int version){
	// get the version we need
	ReaclibVer *objVer = NULL;
	if(version == -1){
		objVer = getCurrentVersion();
	} else {
		objVer = getVer(version);
	}
	
	// check to see if we found a version
	if(objVer == NULL){
		return 0;
	}
	
	// calculate the power
	int intPow = 0;
	// loop through all the rates
	int i = 0;
	const nuclide_t *nuc = getNuclide(i);
	while(nuc != NULL){
		// check to see if this is a reactant
		if(isReactant(i)){
			intPow++;
		} else {
			intPow--;
		}
		nuc = getNuclide(++i);
	}
	
	// start the calculation
	double returnVal = pow(4600838,(1.5*intPow));
	
	string arrReac[6];
	int intRect = 0;
	string arrProd[6];
	int intProd = 0;
	
	// loop through the results
	i = 0;
	nuc = getNuclide(i);
	while(nuc != NULL){
		// check to see if the jvalue for this comp was sent
		if(nuc->jvalue == -1){
			cout << "missing jvalue: " << nuc->comp << "\n";
			return 0;
		}
	
		// check to see if the current comp is a reactant
		if(isReactant(i)){
			// multiply
			returnVal *= (2*nuc->jvalue+1);
			returnVal *= pow(nuc->mvalue,1.5);
			arrReac[intRect++] = string(nuc->comp);
		} else {
			// divide
			returnVal /= (2*nuc->jvalue+1);
			returnVal /= pow(nuc->mvalue,1.5);
			arrProd[intProd++] = string(nuc->comp);
		}
		nuc = getNuclide(++i);
	}
	
	// check to see if the number of reactants is greater than 1
	if(intRect > 1){
		// check to see if there are two reactants
		if(intRect == 2){
			if(arrReac[0] == arrReac[1]){
				returnVal /=2;
			}
		} else if(intRect == 3) {
			if ((arrReac[0] == arrReac[1]) && (arrReac[0] == arrReac[2])){
				returnVal /=6;
			} else if(arrReac[0] == arrReac[1]){
				returnVal /=2;
			} else if(arrReac[0] == arrReac[2]){
				returnVal /=2;
			} else if(arrReac[1] == arrReac[2]){
				returnVal /=2;
			}
		}
	}
	
	// check to see if the number of reactants is greater than 1
	if(intProd > 1){
		// check to see if there are two reactants
		if(intProd == 2){
			if(arrProd[0] == arrProd[1]){
				returnVal *=2;
			}
		} else if(intProd == 3) {
			if ((arrProd[0] == arrProd[1]) and (arrProd[0] == arrProd[2])){
				returnVal *=6;
			} else if(arrProd[0] == arrProd[1]){
				returnVal *=2;
			} else if(arrProd[0] == arrProd[2]){
				returnVal *=2;
			} else if(arrProd[1] == arrProd[2]){
				returnVal *=2;
			}
		}
	}
	
	// get the natural log of the number we have calculated so far
	return returnVal;
}

ReaclibVer *ReaclibRate::getVer(unsigned i){
	if((i>=0) && (i<m_versions.size())){
		return m_versions[i];
	}
	
	#ifdef DEBUG
	cerr << "Unable to find version: " << i << endl;
	#endif
	
	return NULL;
}
