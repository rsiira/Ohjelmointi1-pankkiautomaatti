#include <stdio.h>

typedef struct asiakas {
int tunnusluku;
double saldo;
} AsiakasTietue;

int main (void) {

	int tilinro = 6789;
	int tunnusluku = 9876;
	double saldo = 10000000.0;
	
	AsiakasTietue asiakas = {tunnusluku, saldo};

	printf("Tunnusluku: %d \nSaldo: %.2lf \n", asiakas.tunnusluku, asiakas.saldo);
	luoUusiTili (asiakas, tilinro);

	return 0;
}

void luoUusiTili ( AsiakasTietue as, int tilinro) {
	
FILE *tiedosto;

	char tiedosto_nimi[10];
	sprintf(tiedosto_nimi, "%d.tili", tilinro);

if( (tiedosto = fopen (tiedosto_nimi, "w")) == NULL){
	printf("\nTiedostoa ei voitu avata\n");
	}
	
else{
	rewind( tiedosto ); 
 	
	fprintf( tiedosto, "%-4d\n%-9.2lf\n", as.tunnusluku, as.saldo);
	fclose( tiedosto );
	} 
}


