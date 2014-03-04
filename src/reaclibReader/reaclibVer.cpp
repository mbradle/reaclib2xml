/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibVer.cpp
////////////////////////////////////////////////////////////////////////////////
See reaclibVer.h for more information about this class.
//////////////////////////////////////////////////////////////////////////////*/

#include "reaclibVer.h"

ReaclibVer::ReaclibVer(){
	m_Q = 0;
	m_pstrModified = NULL;
	m_bCurrent = false;
	m_version = 1;
	m_id = 0;
	
	// allocate the space for this elements information
	try {
		m_pVer = new TiXmlElement("ver");
	} catch (bad_alloc&){
		cerr << "Unable to allocate memory for new version element in: " << __FILE__ << " on line: " << __LINE__ << endl;
		return;
	}
	
	
}

ReaclibVer::ReaclibVer(TiXmlElement *pElement){
	// set the default values
	m_Q = 0;
	m_pstrModified = NULL;
	m_bCurrent = false;
	m_version = 1;
	m_id = 0;
	
	// keep track of this
	m_pVer = pElement;
	// loop through all the elements children
	for(TiXmlNode *pNode = m_pVer->FirstChild(); pNode; pNode=pNode->NextSibling()){
		// check to see if this is another node
		if(pNode->Type() == TiXmlNode::ELEMENT){
			// convert this to a type
			TiXmlElement *pElement = pNode->ToElement();
			
			// check to see if this is a known type
			if(strcmp(pElement->Value(),"set") ==0){
				// create the new version
				ReaclibSet* newSet = new ReaclibSet(pElement);
				
				// add the version to the listing
				addSet(newSet, true);
			}
		}
	}
	
	// load the attributes
	// the modified date
	const char *pstrValue;
	if((pstrValue = m_pVer->Attribute("modified"))){
		setModified(pstrValue);
	} 
	
	// the id value
	if(m_pVer->Attribute("id")){
		// try to convert the id to the new value
		if(EOF == sscanf(m_pVer->Attribute("id"), "%d", &m_id)){
			cerr << "Unable to convert id " << m_pVer->Attribute("id") << " to an integer." << endl;
			return;
		}
	}
	
	// the version number
	if(m_pVer->Attribute("num")){
		if(EOF == sscanf(m_pVer->Attribute("num"), "%d", &m_version)){
			cerr << "Unable to convert version number " << m_pVer->Attribute("num") << " to an integer." << endl;
		}
	}
	
	// the Q value
	if(m_pVer->Attribute("q")){
		if(EOF == sscanf(m_pVer->Attribute("q"), "%e", &m_Q)){
			cerr << "Unable to convert version Q " << m_pVer->Attribute("q") << " to a float." << endl;
		}
	}
	
	// the current value
	if(m_pVer->Attribute("current")){
		// check to see if this is the current value
		if(strcmp(m_pVer->Attribute("current"), "1") == 0){
			m_bCurrent = true;
		}
	}
		
	return;
}

ReaclibVer::~ReaclibVer(){
	
}

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

void ReaclibVer::addSet(ReaclibSet *newSet, bool bSkipLink){
	if((bSkipLink == false) && (m_pVer !=NULL)){
		m_pVer->LinkEndChild(newSet->getXMLLink());
	}
	m_sets.push_back(newSet);
}	

// current
bool ReaclibVer::isCurrent(){
	return m_bCurrent;
}

void ReaclibVer::setCurrent(bool bCurrent){
	// update the value
	m_bCurrent = bCurrent;
	
	// check to see if this is linked to an xml element
	if(m_pVer){
		// update the value
		m_pVer->SetAttribute("current", (m_bCurrent?"1":"0"));
	}
}

// id
int ReaclibVer::getID(){
	return m_id;
}

void ReaclibVer::setID(int id){
	// set the value
	m_id = id;
	
	// check to see if this is linked to an xml element
	if(m_pVer){
		// update the value
		m_pVer->SetAttribute("id", m_id);
	}
}

// modified date
const char *ReaclibVer::getModified(){
	return m_pstrModified;
}

void ReaclibVer::setModified(const char *strModified){
	// allocate space for the information
	try{
		m_pstrModified = new char[strlen(strModified)+1];
	} catch (bad_alloc&) {
		#ifdef DEBUG
		cerr << "Unable to allocate memory for the modified date file: " << __FILE__ << " Line: " << __LINE__ << endl;
		#endif
		return;
	}
	strcpy(m_pstrModified,strModified);
	
	// check to see if this is linked to an xml element
	if(m_pVer){
		// update the value
		m_pVer->SetAttribute("modified", m_pstrModified);
	}
}

// Q value
float ReaclibVer::getQ(){
	return m_Q;
}

void ReaclibVer::setQ(float Q){
	m_Q = Q;
	
	// check to see if this is linked to an xml element
	if(m_pVer){
		// check to see if this is the default value
		if(m_Q != 0){
			// update the value
			m_pVer->SetDoubleAttribute("q", m_Q);
		} else {
			m_pVer->RemoveAttribute("q");
		}
	}
}

ReaclibSet *ReaclibVer::getSet(unsigned i){
	// check to make sure this is a valid set
	if((i >= 0) && (i<m_sets.size())){
		return m_sets[i];
	}
	
	return NULL;
}

int ReaclibVer::getSetSize(){
	return m_sets.size();
}

// version number
int ReaclibVer::getVersion(){
	return m_version;
}

void ReaclibVer::setVersion(int version){
	m_version = version;
	
	// check to see if this is linked to an xml element
	if(m_pVer){
		// update the value
		m_pVer->SetAttribute("version", m_version);
	}
}

TiXmlElement *ReaclibVer::getXMLLink(){
	return m_pVer;
}
