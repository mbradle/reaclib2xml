#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <boost/algorithm/string.hpp>

#include <Libnucnet__Nuc.h>
#include <Libnucnet__Reac.h>
#include <Libnucnet.h>

#define I_FIELD  9  // Use 9 for winvn and 12 for winvne

using namespace std;

int main(int argc, char **argv) {
	// Read in command line arguments
	if(argc != 3)
        {
          cerr << "Usage: " << argv[0] << " partitionfile outputfile" << endl
	       << "(partitionfile is a winvn file without the header and nuclide list)" << endl;
		return -1;
	}
	char *partitionFile = argv[1];
	char *outputFile = argv[2];
	
	// Build the T9 vector
	double t9[24] = {
		0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0,
		1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0,
		6.0, 7.0, 8.0, 9.0, 10.0
	};
	gsl_vector *t9Vec = gsl_vector_alloc(24);
	for(int i = 0; i < 24; i += 1) {
		gsl_vector_set(t9Vec, i, t9[i]);
	}
	
	// Set up Libnucnet data structures
	Libnucnet__Nuc *lnnNuc = Libnucnet__Nuc__new();
	
	// Read in partition file
	fstream partition;
	partition.open(partitionFile, fstream::in);
	string line1, line234[3];
        std::string nuc, source;
	double spin, massExcess;
	unsigned int z, a, n;
	gsl_vector *pfVec = gsl_vector_alloc(24);
	while(!partition.eof()) {
		// Read in the 4 lines for an entry
		getline(partition, line1);
		getline(partition, line234[0]);
		getline(partition, line234[1]);
		if(partition.eof()) break;
		getline(partition, line234[2]);
		
		// Parse data from line 1
                nuc = line1.substr(0, 5);
                boost::algorithm::trim( nuc );
		a = atoi(line1.substr(5, 12).c_str());
		z = atoi(line1.substr(17, 4).c_str());
		n = atoi(line1.substr(21, 4).c_str());
		spin = atof(line1.substr(25, 6).c_str());
		massExcess = atof(line1.substr(31, 10).c_str());
                boost::algorithm::trim( source );
		
		// Read partition function data from lines 2, 3, and 4
		for(int i = 0; i < 3; i += 1) {
	          for(int j = 0; j < 8; j += 1) {
	            double pf_value =
                      atof(line234[i].substr(I_FIELD*j, I_FIELD).c_str());
		    gsl_vector_set(pfVec, i*8+j, log10(pf_value));
                  }
		}
		
		// Create the Libnucnet nuclide entry
		Libnucnet__Species *species =
                  Libnucnet__Species__new(
                    z,
                    a,
                    source.c_str(),
                    0,
                    NULL,
                    massExcess,
                    spin,
                    t9Vec,
                    pfVec
                  );
		if(!Libnucnet__Nuc__addSpecies(lnnNuc, species)) {
			cerr << "Unable to add nuclide: " << nuc << endl;
			continue;
		}
	}
	partition.close();
	
	// Save Libnucnet XML file
	Libnucnet__Nuc__writeToXmlFile(lnnNuc, outputFile);
	
	// Free pointers
	Libnucnet__Nuc__free(lnnNuc);
	
	return 0;
}
