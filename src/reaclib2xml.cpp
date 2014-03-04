#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>

#include <Libnucnet__Nuc.h>
#include <Libnucnet__Reac.h>
#include <Libnucnet.h>

#include "reaclibReader/reaclibReader.h"
#include "reaclibReader/NuclideSearch.h"

using namespace std;

typedef enum {
	WEAKRATE_BPLUS, // Beta+ decay
	WEAKRATE_BMINUS, // Beta- decay
	WEAKRATE_EC, // Electron capture
	WEAKRATE_BPLUS_OR_EC, // Either electron capture or beta+ decay (unable to determine)
	WEAKRATE_UNKNOWN, // Unable to determine reaction type
	WEAKRATE_ERROR // An error occurred during reaction type detection
} weakrate_type_t;

typedef struct {
	NuclideSearch *nuclides;
	int z;
	int has_error;
} total_charge_t;

int add_to_libnucnet(Libnucnet__Reac*, ReaclibReader*, NuclideSearch*);
int add_reaction_to_libnucnet(Libnucnet__Reac*, ReaclibRate*, ReaclibVer*, NuclideSearch*);
void add_fit_to_reaction(Libnucnet__Reaction*, ReaclibVer*);
int add_elements_to_reaction(Libnucnet__Reaction*, ReaclibRate*, ReaclibVer*, NuclideSearch*);
int update_weak_rate(Libnucnet__Reaction*, NuclideSearch*);
void update_non_weak_rate(Libnucnet__Reaction*, int, int);
weakrate_type_t get_weak_rate_type(Libnucnet__Reaction*, NuclideSearch*);
int total_charge(Libnucnet__Reaction__Element*, void*);

int main(int argc, char **argv) {
	char *nuclide_file, *reaclib_file, *xml_file;
	NuclideSearch *nuclides;
	ReaclibReader reaclib;
	Libnucnet__Reac *reac;
	
	// Get command line arguments
	if(argc != 4) {
		printf("Usage: %s nuclides.xml reaclibfile output.xml\n", argv[0]);
		return -1;
	}
	nuclide_file = argv[1];
	reaclib_file = argv[2];
	xml_file = argv[3];
	
	// Open nuclide list
	nuclides = NuclideSearch::newInstance();
	nuclides->loadNuclideFile(nuclide_file);
	
	// Open reaclib file
	reaclib.LoadREACLIB(reaclib_file);
	
	// Create libnucnet reaction list structure
	reac = Libnucnet__Reac__new();
	
	// Add reactions from Reaclib file to to libnucnet structure
	if(add_to_libnucnet(reac, &reaclib, nuclides) != 0) {
		fprintf(stderr, "Adding reactions to XML file failed. Abort.\n");
		return -1;
	}
	
	// Save the libnucnet XML file
	Libnucnet__Reac__writeToXmlFile(reac, xml_file);
	
	return 0;
}

// Add all reactions from a Reaclib file to a Libnucnet__Reac structure. Returns nonzero on failure.
int add_to_libnucnet(Libnucnet__Reac *reac, ReaclibReader *reaclib, NuclideSearch *nuclides) {
	int i, j;
	ReaclibRate *rate;
	ReaclibVer *ver;
	
	// Loop through all reactions in the Reaclib file
	for(i = 0; i < reaclib->getRateSize(); i += 1) {
		rate = reaclib->getRate(i);
		for(j = 0; j < rate->getVersionSize(); j += 1) {
			ver = rate->getVer(j);
			// Only add forward rates. Webnucleo takes care of reverse rates.
			if(ver->getSet(0)->getDir() != 'v') {
				if(add_reaction_to_libnucnet(reac, rate, ver, nuclides) != 0) {
					fprintf(stderr, "Failed to add reaction to XML file: %s.\n", rate->getEqn());
					return -1;
				}
			}
		}
	}
	
	return 0;
}

// Add a single Reaclib reaction to a Libnucnet__Reac structure. Returns nonzero on failure.
int add_reaction_to_libnucnet(Libnucnet__Reac *reac, ReaclibRate *rate, ReaclibVer *ver, NuclideSearch *nuclides) {
	Libnucnet__Reaction *reaction;
	
	// Set up the libnucnet reaction structure
	reaction = Libnucnet__Reaction__new();
	
	// Set the reaction source (i.e. Reaclib label)
	Libnucnet__Reaction__updateSource(reaction, ver->getSet(0)->getLabel());
	
	// Add products and reactants to the libnucnet structure
	if(add_elements_to_reaction(reaction, rate, ver, nuclides) != 0) {
		fprintf(stderr, "Unable to add elements to reaction %s\n", rate->getEqn());
		return -1;
	}
	
	// Add the fit
	add_fit_to_reaction(reaction, ver);
	
	// Add the reaction to the Libnucnet__Reac structure
	Libnucnet__Reac__addReaction(reac, reaction);
	
	return 0;
}

// Add the Reaclib fit to a libnucnet reaction
void add_fit_to_reaction(Libnucnet__Reaction *reaction, ReaclibVer *ver) {
	int i;
	ReaclibSet *set;
	double *a;
	char note[4];
	char res;
	
	// Check if this can be represented as a temperature-independent rate. If so, returns early.
	if(ver->getSetSize() == 1) {
		set = ver->getSet(0);
		if( ( set->getRes() == 'w' || set->getRes() == 's' )
			&& set->getA1() == 0
			&& set->getA2() == 0
			&& set->getA3() == 0
			&& set->getA4() == 0
			&& set->getA5() == 0
			&& set->getA6() == 0)
		{
			Libnucnet__Reaction__updateSingleRate(reaction, exp(set->getA0()));
			return;
		}
	}
	
	// Loop through the reaction's sets
	for(i = 0; i < ver->getSetSize(); i += 1) {
		set = ver->getSet(i);
		
		// Fit parameters
		a = (double*)calloc(8, sizeof(double));
		a[0] = set->getA0();
		a[1] = set->getA1();
		a[2] = set->getA2();
		a[3] = set->getA3();
		a[4] = set->getA4();
		a[5] = set->getA5();
		a[6] = set->getA6();
		a[7] = 0;
		
		// Note
		res = set->getRes();
		sprintf(note, "%c%d", res==' '?'n':res, i);
		
		Libnucnet__Reaction__addNonSmokerFit(
			reaction,
			note, // Note
			a, // Fit parameters
			0, // spint
			0, // spinf
			0.01, // tlowhf
			0.01, // tlowfit
			10, // thighfit
			0 // acc
		);
	}
}

// Add the products and reactants to a libnucnet reaction
int add_elements_to_reaction(Libnucnet__Reaction *reaction, ReaclibRate *rate, ReaclibVer *ver, NuclideSearch *nuclides) {
	int n_products = 0, n_reactants = 0, i;
	const nuclide_t *nuc;
	char comp[7];
	
	// Add normal products and reactants
	for(i = 0; i < 6; i += 1) {
		nuc = rate->getNuclide(i);
		if(nuc != NULL) {
			// Translate special names to generalized names
			if(strcmp(nuc->comp, "p") == 0) {
				strcpy(comp, "h1");
			} else if(strcmp(nuc->comp, "d") == 0) {
				strcpy(comp, "h2");
			} else if(strcmp(nuc->comp, "t") == 0) {
				strcpy(comp, "h3");
			} else {
				strncpy(comp, nuc->comp, 7);
			}
			
			// Add the reactant/product
			if(rate->isReactant(i)) {
				Libnucnet__Reaction__addReactant(reaction, comp);
				n_reactants += 1;
			} else {
				Libnucnet__Reaction__addProduct(reaction, comp);
				n_products += 1;
			}
		}
	}
	
	// Add additional products and reactants depending on if this is a weak or a non-weak rate
	if( ver->getSet(0)->getRes() == 'w' )
        {
		if(update_weak_rate(reaction, nuclides) != 0) {
			fprintf(stderr, "Unable to add additional elements to weak rate reaction\n");
			return -1;
		}
        }
	else if( ver->getSet(0)->getRes() == 's' )
        {
	} else {
		update_non_weak_rate(reaction, n_reactants, n_products);
	}
	
	return 0;
}

// Add (anti)electrons and (anti)neutrinos to weak rates
int update_weak_rate(Libnucnet__Reaction *reaction, NuclideSearch *nuclides) {
	weakrate_type_t type;
	
	type = get_weak_rate_type(reaction, nuclides);
	if(type == WEAKRATE_BPLUS || type == WEAKRATE_BPLUS_OR_EC) { // Assume B+
		// (Z,A) -> (Z-1,A) + positron + neutrino_e
		Libnucnet__Reaction__addProduct(reaction, POSITRON);
		Libnucnet__Reaction__addProduct(reaction, ELECTRON_NEUTRINO);
	} else if(type == WEAKRATE_BMINUS) {
		// (Z,A) -> (Z+1,A) + electron + anti-neutrino_e
		Libnucnet__Reaction__addProduct(reaction, ELECTRON);
		Libnucnet__Reaction__addProduct(reaction, ELECTRON_ANTINEUTRINO);
	} else if(type == WEAKRATE_EC) {
		// (Z,A) + electron -> (Z-1,A) + neutrino_e
		Libnucnet__Reaction__addReactant(reaction, ELECTRON);
		Libnucnet__Reaction__addProduct(reaction, ELECTRON_NEUTRINO);
	} else if(type == WEAKRATE_UNKNOWN) {
		// Leave the reaction alone
		fprintf(stderr,
			"Reaction %s is marked as a weak rate, but weak rate type detection failed.\n",
			Libnucnet__Reaction__getString(reaction));
	} else { // type == WEAKRATE_ERROR
		fprintf(stderr,
			"There was an error while determining the reaction type for %s.\n",
			Libnucnet__Reaction__getString(reaction));
		return -1;
	}
	
	return 0;
}

// Reaction product/reactant iterator function to get the total charge of products or reactants
int total_charge(Libnucnet__Reaction__Element *el, void *data) {
	total_charge_t *z_data = (total_charge_t*)data;
	const char *comp = Libnucnet__Reaction__Element__getName(el);
	const nuclide_t *nuc = z_data->nuclides->getNuclide(comp);
	if(nuc == NULL) {
		// Treat these 3 special because Reaclib stores them as p, d, and t instead of h1, h2, and h3
		if(strcmp(comp, "h1") == 0 || strcmp(comp, "h2") == 0 || strcmp(comp, "h3") == 0) {
			z_data->z += 1;
			return 1;
		}
		fprintf(stderr, "Unable to find nuclide %s in nuclides.xml file\n", comp);
		z_data->has_error = true;
		return 0;
	}
	z_data->z += nuc->zvalue;
	return 1;
}

// Determine what type of weak rate a reaction is, if possible
weakrate_type_t get_weak_rate_type(Libnucnet__Reaction *reaction, NuclideSearch *nuclides) {
	int reactant_z, product_z;
	total_charge_t charge_data;
	weakrate_type_t type;
	const char *label;
	
	// Get the total charge of the reactants and products
	charge_data.nuclides = nuclides;
	charge_data.z = 0;
	charge_data.has_error = 0;
	Libnucnet__Reaction__iterateReactants(reaction, total_charge, &charge_data);
	if(charge_data.has_error) {
		fprintf(stderr, "Problem computing charge of reactants in %s\n",
			Libnucnet__Reaction__getString(reaction));
		return WEAKRATE_ERROR;
	}
	reactant_z = charge_data.z;
	charge_data.z = 0;
	Libnucnet__Reaction__iterateProducts(reaction, total_charge, &charge_data);
	if(charge_data.has_error) {
		fprintf(stderr, "Problem computing charge of products in %s\n",
			Libnucnet__Reaction__getString(reaction));
		return WEAKRATE_ERROR;
	}
	product_z = charge_data.z;
	
	// Attempt to determine type
	if(product_z == reactant_z + 1) {
		// Beta- decay
		type = WEAKRATE_BMINUS;
	} else if(product_z == reactant_z - 1) {
		// Beta+ decay or electron capture
		label = Libnucnet__Reaction__getSource(reaction);
		if(strcmp(label, "bec") == 0 || strcmp(label, "ec") == 0) {
			type = WEAKRATE_EC;
		} else if(strcmp(label, "bet+") == 0
			|| strcmp(label, "bex+") == 0
			|| strcmp(label, "bhi+") == 0
			|| strcmp(label, "bqa+") == 0
			|| strcmp(label, "btyk") == 0) {
			type = WEAKRATE_BPLUS;
		} else {
			type = WEAKRATE_BPLUS_OR_EC;
		}
	} else {
		type = WEAKRATE_UNKNOWN;
	}
	
	return type;
}

// Add gamma to products and reactants for non-weak rates
void update_non_weak_rate(Libnucnet__Reaction *reaction, int n_reactants, int n_products) {
	if(n_reactants == 1) {
		Libnucnet__Reaction__addReactant(reaction, GAMMA);
	}
	if(n_products == 1) {
		Libnucnet__Reaction__addProduct(reaction, GAMMA);
	}
}
