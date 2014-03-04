/*//////////////////////////////////////////////////////////////////////////////
Filename: NuclideSearch.cpp
////////////////////////////////////////////////////////////////////////////////
See NuclideSearch.h for more information about this class.
//////////////////////////////////////////////////////////////////////////////*/


#include "NuclideSearch.h"

NuclideSearch* NuclideSearch::m_instance = 0;

NuclideSearch* NuclideSearch::newInstance(){
	// check to see if this is the first call
	if(m_instance == 0){
		// create the new instance
		m_instance = new NuclideSearch;
	}
	
	return m_instance;
}

NuclideSearch::NuclideSearch(){
	
}

const nuclide_t *NuclideSearch::getNuclide(const char *comp){
	// check to see if we have a nuclides loaded
	if(m_nuclides.size() == 0){
		return NULL;
	}
	
	// look for the nuclides
	unsigned int high = m_nuclides.size();
	unsigned int low = 0;
	unsigned int mid = -1;
	unsigned int old_mid = 0;
	
	// loop until the two have met
	while((high != low) && (old_mid != mid)){
		// save the old midpoint
		old_mid = mid;
		
		// calculate the midpoint
		mid = (high-low)/2+low;
		nuclide_t *nuc_mid = m_nuclides[mid];
		
		// check to see if there is a match
		if(strcmp(comp,nuc_mid->comp) == 0){
			return nuc_mid;
		// check to see if this is higher
		} else if(strcmp(comp, nuc_mid->comp) > 0) {	
			low = mid;
		} else {
			high = mid;
		}
	}
	
	
	// we didn't find anything so return null
	return NULL;
}

bool NuclideSearch::loadNuclideFile(char *strFilename){
	// attempt to load the file
	TiXmlDocument doc;
	if(!doc.LoadFile(strFilename)){
		#ifdef DEBUG
		cerr << "Unable to open nuclide file.\n";
		#endif
		return false;
	}
	
	// get the root element 
	TiXmlElement *root = doc.FirstChildElement("nuclides");
	
	// loop through the elements
	for(TiXmlNode *node =root->FirstChild(); node != NULL; node=node->NextSibling()){
		// check to see if this is an element
		if(node->Type() == TiXmlNode::ELEMENT){
			// convert this to an element
			TiXmlElement *pElement = node->ToElement();
			
			// check to see if this is a rate element
			if(strcmp(pElement->Value(),"nuclide") == 0){
				// create a new nuclide
				nuclide_t *nuclide;
				try{
					nuclide = new nuclide_t;
				} catch (bad_alloc &){
					cerr << "Unable to allocate memory for new nuclide in file: "
						<< __FILE__ << " on line: " << __LINE__ << endl;
					return false;
				}
				
				// load up the information
				// zvalue
				if(pElement->Attribute("zvalue")){
					nuclide->zvalue = atoi(pElement->Attribute("zvalue"));
				} else {
					nuclide->zvalue = 0;
				}
				
				// mvalue
				if(pElement->Attribute("mvalue")){
					nuclide->mvalue = atoi(pElement->Attribute("mvalue"));
				} else {
					nuclide->mvalue = 0;
				}
				
				const char *temp;
				if((temp = pElement->Attribute("nuc"))){
					// allocate space for the nuclide name
					try{
						nuclide->nuc = new char[strlen(temp)+1];
					} catch (bad_alloc &) {
						cerr << "Unable to allocate memory for new nuc in file:"
							<< __FILE__ << " on line: " << __LINE__ << endl;
						return false;
					}
					
					// copy over the data
					strcpy(nuclide->nuc, temp);
				}
				
				if((temp = pElement->Attribute("comp"))){
					// allocate space
					try {
						nuclide->comp = new char[strlen(temp)+1];
					} catch (bad_alloc &) {
						cerr << "Unable to allocate memory for new comp in file:"
							<< __FILE__ << " on line: " << __LINE__ << endl;
						return false;
					}
					
					// copy over the data
					strcpy(nuclide->comp, temp);
				}
				
				if((temp = pElement->Attribute("jvalue"))){
					nuclide->jvalue = atof(temp);
				} else {
					nuclide->jvalue = -1;
				}
				
				// save the information
				m_nuclides.push_back(nuclide);
			}
		}
	}
	
	#ifdef DEBUG
	cerr << m_nuclides.size() << " nuclides loaded...\n";
	#endif
	
	// sort the nuclides
	std::sort(m_nuclides.begin(), m_nuclides.end(), compareNuclide);
	
	return true;
}

bool compareNuclide(nuclide_t const *x, nuclide_t const *y){
	return (strcmp(x->comp, y->comp) < 0);
}

