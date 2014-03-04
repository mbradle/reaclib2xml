/*//////////////////////////////////////////////////////////////////////////////
Filename: reaclibSet.cpp
////////////////////////////////////////////////////////////////////////////////
See reaclibSet.h for more information about this class.
//////////////////////////////////////////////////////////////////////////////*/


#include "reaclibSet.h"
using namespace std;

ReaclibSet::ReaclibSet(){
	
	// allocate a new element
	try{
		m_pSet = new TiXmlElement("set");
	} catch (bad_alloc&){
		cerr << "Unable to allocate space for new set element in file: " << __FILE__ << " on line: " << __LINE__ << endl;
		return;
	}
	
	
	// set the default values
	m_id = 0;
	m_a0 = 0;
	m_a1 = 0;
	m_a2 = 0;
	m_a3 = 0;
	m_a4 = 0;
	m_a5 = 0;
	m_a6 = 0;
	m_res = ' ';
	m_dir = ' ';
	m_pLabel = NULL;
}


ReaclibSet::ReaclibSet(TiXmlElement *pElement){
	// keep track of this
	m_pSet = pElement;
	
	// set the default values
	m_id = 0;
	m_a0 = 0;
	m_a1 = 0;
	m_a2 = 0;
	m_a3 = 0;
	m_a4 = 0;
	m_a5 = 0;
	m_a6 = 0;
	m_res = ' ';
	m_dir = ' ';
	m_pLabel = NULL;
	
	// load the attributes
	// label
	if(m_pSet->Attribute("label")){
		setLabel(m_pSet->Attribute("label"));
	}

	// the id
	if(m_pSet->Attribute("id")){
		if(EOF == sscanf(m_pSet->Attribute("id"), "%d", &m_id)){
			cerr << "Unable to convert id " << m_pSet->Attribute("id") << " to an integer." << endl;
		}
	}
	
	// the res field
	if(m_pSet->Attribute("res")){
		m_res = m_pSet->Attribute("res")[0];
	}
	
	// the dir field
	if(m_pSet->Attribute("dir")){
		m_dir = m_pSet->Attribute("dir")[0];
	}
	
	// the num fields
	if(m_pSet->Attribute("num0")){
		if(EOF == sscanf(m_pSet->Attribute("num0"), "%f", (float*)&m_a0)){
			cerr << "Unable to convert num0 " << m_pSet->Attribute("num0") << " to a float." << endl;
		}
	}
	if(m_pSet->Attribute("num1")){
		if(EOF == sscanf(m_pSet->Attribute("num1"), "%f", &m_a1)){
			cerr << "Unable to convert num1 " << m_pSet->Attribute("num1") << " to a float." << endl;
		}
	}
	if(m_pSet->Attribute("num2")){
		if(EOF == sscanf(m_pSet->Attribute("num2"), "%f", &m_a2)){
			cerr << "Unable to convert num2 " << m_pSet->Attribute("num2") << " to a float." << endl;
		}
	}
	if(m_pSet->Attribute("num3")){
		if(EOF == sscanf(m_pSet->Attribute("num3"), "%f", &m_a3)){
			cerr << "Unable to convert num3 " << m_pSet->Attribute("num3") << " to a float." << endl;
		}
	}
	if(m_pSet->Attribute("num4")){
		if(EOF == sscanf(m_pSet->Attribute("num4"), "%f", &m_a4)){
			cerr << "Unable to convert num4 " << m_pSet->Attribute("num4") << " to a float." << endl;
		}
	}
	if(m_pSet->Attribute("num5")){
		if(EOF == sscanf(m_pSet->Attribute("num5"), "%f", &m_a5)){
			cerr << "Unable to convert num5 " << m_pSet->Attribute("num5") << " to a float." << endl;
		}
	}
	if(m_pSet->Attribute("num6")){
		if(EOF == sscanf(m_pSet->Attribute("num6"), "%f", &m_a6)){
			cerr << "Unable to convert num6 " << m_pSet->Attribute("num6") << " to a float." << endl;
		}
	}
	
	return;
}

ReaclibSet::~ReaclibSet(){
	
}

/*//////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Get/Set Functions ////////////////////////////
//////////////////////////////////////////////////////////////////////////////*/

// a0
float ReaclibSet::getA0(){
	return m_a0;
}

void ReaclibSet::setA0(float a0){
	// set the a0 value
	m_a0 = a0;
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a0!=0){
			m_pSet->SetDoubleAttribute("num0", m_a0);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num0");
		}
	}
}

// a1
float ReaclibSet::getA1(){
	return m_a1;
}

void ReaclibSet::setA1(float a1){
	// set the a1 value
	m_a1 = a1;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a1!=0){
			m_pSet->SetDoubleAttribute("num1", m_a1);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num1");
		}
		
	}
}

// a2
float ReaclibSet::getA2(){
	return m_a2;
}

void ReaclibSet::setA2(float a2){
	// set the a2 value
	m_a2 = a2;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a2!=0){
			m_pSet->SetDoubleAttribute("num2", m_a2);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num2");
		}
	}
}

// a3
float ReaclibSet::getA3(){
	return m_a3;
}

void ReaclibSet::setA3(float a3){
	// set the a3 value
	m_a3 = a3;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a3!=0){
			m_pSet->SetDoubleAttribute("num3", m_a3);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num3");
		}
	}
}

// a4
float ReaclibSet::getA4(){
	return m_a4;
}

void ReaclibSet::setA4(float a4){
	// set the a4 value
	m_a4 = a4;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a4!=0){
			m_pSet->SetDoubleAttribute("num4", m_a4);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num4");
		}
	}
}

// a5
float ReaclibSet::getA5(){
	return m_a5;
}

void ReaclibSet::setA5(float a5){
	// set the a5 value
	m_a5 = a5;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a5!=0){
			m_pSet->SetDoubleAttribute("num5", m_a5);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num5");
		}
	}
}

// a6
float ReaclibSet::getA6(){
	return m_a6;
}

void ReaclibSet::setA6(float a6){
	// set the a0 value
	m_a6 = a6;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_a6!=0){
			m_pSet->SetDoubleAttribute("num6", m_a6);
		} else {
			// delete this if we are switching to the default value
			m_pSet->RemoveAttribute("num6");
		}
	}
}

// dir
char ReaclibSet::getDir(){
	return m_dir;
}

void ReaclibSet::setDir(char dir){
	m_dir = dir;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_dir != ' '){
			char temp[2];
			temp[0] = m_dir;
			temp[1] = '\0';
			m_pSet->SetAttribute("dir", temp);
		} else {
			m_pSet->RemoveAttribute("dir");
		}
	}
}

// id
int ReaclibSet::getID(){
	return m_id;
}

void ReaclibSet::setID(int id){
	m_id = id;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		m_pSet->SetAttribute("id", m_id);
	}
}

// label
const char *ReaclibSet::getLabel(){
	return m_pLabel;
}

void ReaclibSet::setLabel(const char *label){
	try {
		m_pLabel = new char[strlen(label)+1];
	} catch (bad_alloc &) {
		cerr << "Unable to allocate space for the label field in " << __FILE__ << " on line: " << __LINE__ << endl;
		return;
	}
	
	// copy over the value
	strcpy(m_pLabel, label);
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		m_pSet->SetAttribute("label", m_pLabel);
	}
}

// res
char ReaclibSet::getRes(){
	return m_res;
}

void ReaclibSet::setRes(char res){
	m_res = res;
	
	// check to see if this is linked to an xml element
	if(m_pSet){
		// check to see if this is the default value
		if(m_res!=' '){
			char temp[2];
			temp[0] = m_res;
			temp[1] = '\0';
			m_pSet->SetAttribute("res", temp);
		} else {
			m_pSet->RemoveAttribute("res");
		}
	}
}

TiXmlElement *ReaclibSet::getXMLLink(){
	return m_pSet;
}
