/*
Pankkiautomaatti - Ohjelmointi 1 - kotiteht‰v‰ 6

@author: Reeta Siira

Ohjelmassa oletetaan ett‰ jokaista asiakasta kohden on oma
tekstimuotoinen tilitiedosto, joka on muotoa tilinumero.tili,
esimerkiksi 1234.tili.

Tiedosto sis‰lt‰‰ kaksi rivinvaihdolla erotettua merkkijonoa:
Tunnusluku
saldo

Pankkiautomaattia k‰ytt‰m‰ll‰ tilinomistaja voi:
1. nostaa tililt‰ rahaa
2. tallettaa tilille rahaa
3. kysy‰ tilin saldoa
*/

#include <stdio.h>

#define STOP 0

void valikko(char tiedosto_nimi[10]);
int otto(char tiedosto_nimi[10]);
int talletus(char tiedosto_nimi[10]);
void saldo(char tiedosto_nimi[10]);
void jaaRahat(int summa);

int lueKokonaisluku(void);
void lueRoskat(void);

typedef struct asiakas{
int tunnusluku;
double saldo;
} AsiakasTietue;

int main(){
	
	FILE * tiedosto;
	AsiakasTietue as;
	char tiedosto_nimi[10];
	int tilinro = 0;
	int tunnusluku = 0;
	int count = 1; //laskuri, joka tarkistaa montako kertaa tunnusluku on kirjoitettu v‰‰rin 
	
	//Kysyt‰‰n tilinumero
	printf("Tervetuloa! Anna tilinumero, ole hyva.\n");
	tilinro = lueKokonaisluku();
	
	//Tarkistetaan tilinumeron pituus, jonka oltava v‰lill‰ 1000-9999.
	while(tilinro<1000 || tilinro > 9999){
		printf("Tilinumeron pituus on oltava 4 merkkia eika tilinumero voi alkaa nollalla.\n");
		printf("Anna tilinumero, ole hyva.\n");
		tilinro = lueKokonaisluku();
	}

	//Tallennetaan tiedoston nimeksi tilinro.tili esim 1234.tili
	sprintf(tiedosto_nimi, "%d.tili", tilinro);
	
	//Tarkistetaan onko tili‰ olemassa
 	while(( tiedosto = fopen (tiedosto_nimi, "r" )) == NULL ){ //tiedosto avataan
 		printf("Tilia ei ole olemassa.\n");	
 		printf("Anna tilinumero, ole hyva.\n");
		tilinro = lueKokonaisluku();
		sprintf(tiedosto_nimi, "%d.tili", tilinro);
	}
	
	//Kysyt‰‰n tunnuluku
	printf("Nappaile tunnusluku. Suojaa tunnuslukusi. \n"); 
	tunnusluku = lueKokonaisluku();	
	
	//Tallennetaan tiedoston tiedot tunnusluku ja saldo AsiakasTietueeseen as
	fscanf(tiedosto, "%d\n%lf", &as.tunnusluku, &as.saldo); //Tilitiedostossa tunnusluku ja saldo on erotettuna rivinvaihdolla
	fclose( tiedosto ); //suljetaan tiedosto
	
	//Tarkistetaan onko tunnusluku oikea
	do{
		if(tunnusluku == as.tunnusluku){ 	//Jos tunnusluku on oikea niin avataan tili ja valikko
			
			valikko(tiedosto_nimi);
			break;
	
		}
		
		else if(tunnusluku != as.tunnusluku && count < 3){ 		//Jos tunnusluku ei ole oikea ja v‰‰ri‰ yrityksi‰ on v‰hemm‰n kuin kolme,
			printf("Vaara tunnusluku. Yrita uudelleen.\n");		//kysyt‰‰n tunnuslukua uudestaan.
			count++;
			tunnusluku = lueKokonaisluku();
			fscanf(tiedosto, "%d\n%lf", &as.tunnusluku, &as.saldo);
		}
		
		else if(tunnusluku != as.tunnusluku && count == 3){		//Jos tunnusluku ei ole oikea ja v‰‰ri‰ yrityksi‰ on kolme,
			printf("Kirjoitit vaaran tunnusluvun 3 kertaa.\n");	//lopetetaan ohjelma.
			printf("Kirjaudutaan ulos.\n");
			count++;
		}
		
	
	}while(count < 4);
	
	return 0;
}

//Valikko tilitoimintoihin
//Mahdolliset valinnat ovat 1) Otto, 2) Talletus tai 3) Saldo.
//Ohjelman voi lopettaa syˆtt‰m‰ll‰ 0.
void valikko(char tiedosto_nimi[10]){
	
	int valinta = 0;
	int lopetus = 1; //arvo 0 lopettaa ohjelman

	do{
		printf("Valitse 1) Otto, 2) Talletus tai 3) Saldo. Lopeta syottamalla 0.\n");
		
		valinta = lueKokonaisluku();
		
			if (valinta == 1){				// 1) Otto
			
				printf("Otto valittu.\n");
				lopetus = otto(tiedosto_nimi); //arvo 0 (STOP) lopettaa ohjelman
				
			}
			
			else if (valinta == 2){			// 2) Talletus
			
				printf("Talletus valittu.\n");
				lopetus = talletus(tiedosto_nimi); //arvo 0 (STOP) lopettaa ohjelman
		
			}
		
			else if (valinta == 3){ 		// 3) Saldo
				
				printf("Saldo valittu.\n");
				saldo(tiedosto_nimi);
				
			}
			
			else if (valinta == STOP){
			
				printf("Kiitos kaynnista. Tervetuloa uudelleen.\n");
			}
			
			
			else{
				printf("Virheellinen valinta.\n");	
			}
			
		}while(valinta != STOP && lopetus != STOP);
}

//Kysyy k‰ytt‰j‰lt‰ nostettavan summan ja v‰hent‰‰ sen tilin saldosta,
//jos tilin saldo riitt‰‰.
//Palauttaa arvon 0, jos tililta kirjaudutaan ulos ja ohjelma lopetetaan. 
//Palauttaa arvon 1, jos palataan aloitusvalikkoon.
//Parametrit: tilitiedoston nimi
int otto(char tiedosto_nimi[10]){
	
	FILE * tiedosto;
	AsiakasTietue as;
	int summa = 0;
	printf("Valitse 20 euroa, 40 euroa, 60 euroa, 90 euroa, 140 euroa, 240 euroa tai muu summa. Palaa alkuun syottamalla 0.\n");	 
	summa = lueKokonaisluku();
	
	if (summa == 0){
		
		return 1; //palaa alkuun
		
	}
	
	//Tarkistetaan sallitut summat
	else if ((summa%10 == 0 && summa >= 40 && summa <= 1000) || summa == 20){	
		 
		if( (tiedosto = fopen (tiedosto_nimi, "r+" )) == NULL){
			printf("\nTiedostoa ei voitu avata\n");
		}
		
		else{
			fscanf(tiedosto, "%d\n%lf", &as.tunnusluku, &as.saldo);
	
			//Tarkistetaan riitt‰‰kˆ saldo
			if((as.saldo-summa)<0){
			
			printf("Tilin saldo ei riit‰. \n Kirjaudutaan ulos.");
			return STOP; //kirjaudutaan ulos
			}
			
			else if((as.saldo-summa)>=0){
			
				//	V‰hennet‰‰n valittu summa tilin saldosta.
				as.saldo = as.saldo - summa;
				
				//Tallennetaan uusi saldo tiedostoon.
				rewind( tiedosto ); 
				fprintf( tiedosto, "%-4d\n%-9.2lf\n", as.tunnusluku, as.saldo);
				fclose( tiedosto ); //suljetaan tiedosto
		
				//Tulostetaan rahat esim. Saat 1 kpl 20 euron seteleit‰
				jaaRahat(summa);
				
				printf("Tilin uusi saldo on %.2lf.\n", as.saldo);
				printf("Kiitos kaynnista. Tervetuloa uudelleen.");
				return STOP; //kirjaudutaan ulos
			
			}	
			
		}
		
	}
	
	else if(summa > 1000){
		
		printf("Suurin sallitu nosto on 1000 euroa.\n");
		printf("Palataan alkuun.\n");
		return 1; //palataan alkuun
	}

	else{
		printf("Syotettya summaa ei voi tulostaa.\n");
		printf("Palataan alkuun.\n");
		return 1; //palataan alkuun
	}
	
}


//Jakaa summan 20 ja 50 euron setelihin.
//Tulostaa 20 ja 50 euron seteleiden kappalem‰‰r‰n.
void jaaRahat(int summa){
	
	int kpl_50 = 0;	 //tulostettavien 50e seteleiden kpl m‰‰r‰
	int kpl_20 = 0;  //tulostettavien 20e seteleiden kpl m‰‰r‰
	
	int jakojaannos1 = 0;
	int jakojaannos2 = 0;
	
	jakojaannos1 = summa % 50;
	
	if (jakojaannos1 == 0){		//jos jaollinen 50:lla
		kpl_50 = summa / 50;
		kpl_20 = 0;
	}
	
	else if(jakojaannos1 !=0 ){	//jos ei ole jaollinen 50:lla
		
		jakojaannos2 = jakojaannos1 % 20;
		
		if (jakojaannos2 == 0){	//jos jaollinen 20:lla
		kpl_50 = summa / 50;
		kpl_20 = (summa - kpl_50 * 50) / 20;
		}
		
		else if(jakojaannos2 != 0){ //jos ei ole jaollinen 20:lla
			kpl_50 = (summa -50) / 50;
			kpl_20 = (summa - kpl_50 * 50) / 20;
		}
	}

	printf("Saat: \n");
	if(kpl_50 != 0){
		printf("%d kpl 50 euron seteleita.\n", kpl_50);
	}

	if(kpl_20 != 0){
	printf("%d kpl 20 euron seteleita.\n", kpl_20);
	}
	
}

//Kysyy k‰ytt‰j‰lt‰ talletettavan summan ja lis‰‰ sen tilin saldoon.
//Palauttaa arvon 0, jos tililta kirjaudutaan ulos ja ohjelma lopetetaan. 
//Palauttaa arvon 1, jos palataan aloitusvalikkoon.
//Parametrit: tilitiedoston nimi
int talletus(char tiedosto_nimi[10]){
	
	FILE * tiedosto;
	AsiakasTietue as;
	int summa = 0;
	//Talleta summa
	
	printf("Valitse 20 euroa, 40 euroa, 60 euroa, 90 euroa, 140 euroa, 240 euroa tai muu summa. Palaa alkuun syottamalla 0.\n");	 
	summa = lueKokonaisluku();
	
	
	if (summa == 0){
		
		return 1; //palataan alkuun
		
	}
	
	//Tarkistetaan sallitut summat
	else if ((summa%10 == 0 && summa >= 40 && summa <= 1000) || summa == 20){
		
		if( (tiedosto = fopen (tiedosto_nimi, "r+" )) == NULL){
			printf("\nTiedostoa ei voitu avata\n");
		}
		
		else{
			fscanf(tiedosto, "%d\n%lf", &as.tunnusluku, &as.saldo);
				
				//Tallennetaan uusi saldo tiedostoon
				as.saldo = as.saldo + summa;
				rewind( tiedosto); 
				fprintf( tiedosto, "%-4d\n%-9.2lf\n", as.tunnusluku, as.saldo);
				fclose( tiedosto ); //suljetaan tiedosto
				
				printf("%d euroa talletettu tilille.\n", summa);
				printf("Tilin uusi saldo on %.2lf.\n", as.saldo);
				printf("Kiitos kaynnista. Tervetuloa uudelleen.");
				return STOP; //kirjaudutaan ulos
				
		}
	}
	
	else{
		printf("Syotettya summaa ei voi tallettaa.\n");
		printf("Palataan alkuun.\n");
		return 1; //palataan alkuun
	}
	
}

//Tulostaa tilin saldon.
//Parametrit: tilitiedoston nimi.
void saldo (char tiedosto_nimi[10]){
	
	FILE * tiedosto;
	AsiakasTietue as;
	int alkuun = 1;  //arvo 0 palautaa aloitusvalikon

	if( (tiedosto = fopen (tiedosto_nimi, "r" )) == NULL){
		printf("\nTiedostoa ei voitu avata\n");
	}
		
	else{
		fscanf(tiedosto, "%d\n%lf", &as.tunnusluku, &as.saldo); 
		printf("Tilin saldo: %.2lf\n", as.saldo);	
		fclose( tiedosto ); //suljetaan tiedosto
	}
	
	printf("Palaa alkuun syottamalla 0.\n");
	while(alkuun != 0){
		alkuun = lueKokonaisluku();
	}
		
}

//Apufunktiot
int lueKokonaisluku(void){
   
   int luku;
   char mki;
   int status;
   
   while((status = scanf("%d%c", &luku, &mki))==0  || (2 == status && mki != '\n') ){
      lueRoskat();
      printf("Et syottanyt kokonaislukua. Yrita uudelleen. \n");
   }
   
   return luku;
}

void lueRoskat(void){
   
   while( fgetc(stdin) != '\n');
   
}
