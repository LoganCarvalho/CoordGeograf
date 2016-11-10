//Poligonal: Série de segmentos de retas conectados
//Perimetro é a soma de todos os lados da poligonal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef struct {
    double northing;
    double easting;
    int zona;
    char letra;
} UTM;  //pode ser entendido também como coordenadas

typedef struct {
    double latitude;
    double longitude;
} LatLon;

double degree2rad(double);
double rad2degree(double);

typedef struct {
    UTM v1,v2,v3;
} Triangulo;

UTM *coordenadas;
LatLon *coordForKml;
int gNumVertPolig;
char hemisferio;
int zona;

bool ler_arquivo (char nomeArquivo[]){

    FILE *ponteiro_arquivo;
    strcat(nomeArquivo, ".txt");
    int i;

    ponteiro_arquivo = fopen(nomeArquivo,"r");

    if (ponteiro_arquivo==NULL){
        printf("Erro ao abrir o arquivo!\n");
        return  false;
    }  else {

        fscanf(ponteiro_arquivo,"%i %c %i  ", &zona, &hemisferio, &gNumVertPolig);
        coordenadas = (UTM*) malloc(gNumVertPolig * sizeof(UTM));

        for (i=0; i < gNumVertPolig;i++){
            fscanf(ponteiro_arquivo,"%lf %lf",&coordenadas[i].easting,&coordenadas[i].northing);
        }

        fclose(ponteiro_arquivo);
        return true;
    }
}

double CalcularAreaPoligono (UTM vertices[], int gNumVertPolig){
    double area = 0;
    int i;

   	for (i=0;i<gNumVertPolig-1;i++) {
   	    area +=(vertices[i].easting*vertices[i+1].northing-vertices[i+1].easting*vertices[i].northing)/2;
   	}

    if (area < 0) {
        return (area * (-1));
    }
    return area;
}

//calculo é raiz quadrada da soma dos quadrados dos catetos
double CalcularPerimetroPoligono(UTM vertices[], int gNumVertPolig){
    double perimetro = 0;
    int i;

   	for (i=0;i<gNumVertPolig-1;i++) {
        perimetro += sqrt(pow(vertices[i+1].easting-vertices[i].easting,2) + pow(vertices[i+1].northing-vertices[i].northing,2));
   	}
    if (perimetro < 0) {
        return (perimetro * (-1));
    }
    return perimetro;
}

//retorna a area do triangulo calculado a partir dos 3 pares de coordenadas
double CalculaAreaTriangulo(Triangulo t) {
    double area = (t.v1.northing * t.v2.easting + t.v2.northing * t.v3.easting + t.v3.northing * t.v1.easting
                   -t.v1.easting * t.v2.northing - t.v2.easting * t.v3.northing - t.v3.easting * t.v1.northing)*0.5;

    if (area < 0) {
        return (area * (-1));
    }
    return area;
}

Triangulo CriaTriangulo (UTM p1, UTM p2, UTM pdado){
    Triangulo novo;

    novo.v1 = p1;
    novo.v2 = p2;
    novo.v3 = pdado;

    return novo;
}

//recebe uma coordenada "aleatoria" passada por quem deseja saber se esta inserida
bool isIn(UTM pdado){
    int i;
	int intersecoes=0;
    for(i=0; i<gNumVertPolig-1;i++ ){ 
    	UTM p1,p2,ptemp;
    	p1=coordenadas[i];
    	p2=coordenadas[i+1];
    	if (p1.easting>p2.easting) {
    		ptemp=p1;
    		p1=p2;
    		p2=ptemp;
    	}
    	double northingMin,northingMax;
    	if (p1.northing<p2.northing) {
    		northingMin=p1.northing;
    		northingMax=p2.northing;   		
    	}
    	else {
    		northingMin=p2.northing;
    		northingMax=p1.northing;     		
    	}
    	if (pdado.northing>northingMin && pdado.northing<northingMax) {
    		if (pdado.easting>p2.easting) {  			
    			intersecoes++;    			
    		}
    		else {
     			if (pdado.easting>p1.easting) {
     				double tanalfa1,tanalfa2,alfa1,alfa2;
     				tanalfa1=(p1.northing-pdado.northing)/(p1.easting-pdado.easting);
     				tanalfa2=(p1.northing-p2.northing)/(p1.easting-p2.easting);
     				alfa1=atan(tanalfa1);
     				alfa2=atan(tanalfa2);
     				printf("a1=%lf a2=%lf\n",alfa1,alfa2);
     				if (alfa1<alfa2)
     					intersecoes++;
     			}   			
    		}
    	}
    }
    printf("inter=%i",intersecoes);
    if (intersecoes%2!=0)
    	return true;
	return false;
}

//****************************************************************************************************
LatLon convertUTM2LatLon (UTM utm, int zona, char hemisferio) {
    LatLon coord;
    // parametros do elipsoide
    double a = 6378137;              // semi-eixo maior
    double f = 1 / 298.257222101;     // achatamento
    // dados auxiliares
    double eccSquared = 2 * f - f * f;   // excentricidade ao quadrado
    double k0 = 0.9996;           // fator de escala

    double e1 = (1 - sqrt(1 - eccSquared)) / (1 + sqrt(1 - eccSquared));

    double x = utm.easting - 500000.0; //remove offset de 500.000 metros para longitude
    double y = utm.northing;

    if (hemisferio == 'S' || hemisferio == 's') {
        y -= 10000000.0;   // remove offset de 10.000.000 metros para hemisferio sul
    }

    int longOrigin = (int)((zona - 1) * 6 - 180 + 3);  // +3 coloca no meio da zona

    double eccPrimeSquared = (eccSquared) / (1 - eccSquared);

    double M = y / k0;
    double mu = M / (a * (1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 - 5 * eccSquared * eccSquared * eccSquared / 256));

    double phi1Rad = (mu + (3 * e1 / 2 - 27 * e1 * e1 * e1 / 32) * sin(2 * mu)
                      + (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32) * sin(4 * mu)
                      + (151 * e1 * e1 * e1 / 96) * sin(6 * mu));

    double N1 = a / sqrt(1 - eccSquared * sin(phi1Rad) * sin(phi1Rad));
    double T1 = tan(phi1Rad) * tan(phi1Rad);
    double C1 = eccPrimeSquared * cos(phi1Rad) * cos(phi1Rad);
    double R1 = a * (1 - eccSquared) / pow(1 - eccSquared * sin(phi1Rad) * sin(phi1Rad), 1.5);
    double D = x / (N1 * k0);

    coord.latitude = phi1Rad - (N1 * tan(phi1Rad) / R1) * (D * D / 2 - (5 + 3 * T1 + 10 * C1 - 4 * C1 * C1 - 9 * eccPrimeSquared) * D * D * D * D / 24
                                                           + (61 + 90 * T1 + 298 * C1 + 45 * T1 * T1 - 252 * eccPrimeSquared - 3 * C1 * C1) * D * D * D * D * D * D / 720);
    coord.latitude = rad2degree(coord.latitude);

    coord.longitude = (D - (1 + 2 * T1 + C1) * D * D * D / 6 + (5 - 2 * C1 + 28 * T1 - 3 * C1 * C1 + 8 * eccPrimeSquared + 24 * T1 * T1)
                       * D * D * D * D * D / 120) / cos(phi1Rad);
    coord.longitude = longOrigin + rad2degree(coord.longitude);

    return coord;

}
double degree2rad(double deg){
    return deg*M_PI / 180;
}
double rad2degree(double rad) {

    return rad*180/M_PI ;
}
UTM converteLatLon2UTM(LatLon c) {
    // Converte latitude/longitude para coordenadas UTM (Equacoes USGS Bulletin 1532)
    // Longitudes (West-Negativas) (Leste-Positivas)
    // Latitudes (Norte-positivas) (Sul-Negativas)
    // Latitudes e Longitudes em graus decimais

    // coordenadas geograficas
    double latitude = c.latitude;
    double longitude = c.longitude;
    // coordenadas UTM
    double northing, easting;
    int ZoneNumber;

    // parametros do elipsoide
    double a = 6378137;              // semi-eixo maior
    double f = 1 / 298.257222101;     // achatamento

    // dados auxiliares
    double eccSquared = 2 * f - f * f;   // excentricidade ao quadrado
    double k0 = 0.9996;           // fator de escala
    double longitudeOrigin;
    double eccPrimeSquared;
    double N, T, C, A, M;

    //Certifica-se que a longitude esta no intervalo -180.00 .. 179.9
    double longitudeTemp = (longitude + 180) - (int)((longitude + 180) / 360) * 360 - 180; // -180.00 .. 179.9;
    double latitudeRad = degree2rad(latitude);
    double longitudeRad = degree2rad(longitudeTemp);
    double longitudeOriginRad;

    // calculo da zonas
    ZoneNumber = (int)((longitudeTemp + 180) / 6) + 1;

    // Zona Especial (32)
    if (latitude >= 56.0 && latitude < 64.0 && longitudeTemp >= 3.0 && longitudeTemp < 12.0)
        ZoneNumber = 32;

    // Zonas Especiais (31,33,35,37)
    if (latitude >= 72.0 && latitude < 84.0)
    {
        if (longitudeTemp >= 0.0 && longitudeTemp < 9.0) ZoneNumber = 31;
        else if (longitudeTemp >= 9.0 && longitudeTemp < 21.0) ZoneNumber = 33;
        else if (longitudeTemp >= 21.0 && longitudeTemp < 33.0) ZoneNumber = 35;
        else if (longitudeTemp >= 33.0 && longitudeTemp < 42.0) ZoneNumber = 37;
    }
    longitudeOrigin = (ZoneNumber - 1) * 6 - 180 + 3;  //+3 puts origin in middle of zone
    longitudeOriginRad = degree2rad(longitudeOrigin);

    eccPrimeSquared = (eccSquared) / (1 - eccSquared);

    N = a / sqrt(1 - eccSquared * sin(latitudeRad) * sin(latitudeRad));
    T = tan(latitudeRad) * tan(latitudeRad);
    C = eccPrimeSquared * cos(latitudeRad) * cos(latitudeRad);
    A = cos(latitudeRad) * (longitudeRad - longitudeOriginRad);

    M = a * ((1 - eccSquared / 4 - 3 * eccSquared * eccSquared / 64 - 5 * eccSquared * eccSquared * eccSquared / 256) * latitudeRad
             - (3 * eccSquared / 8 + 3 * eccSquared * eccSquared / 32 + 45 * eccSquared * eccSquared * eccSquared / 1024) * sin(2 * latitudeRad)
             + (15 * eccSquared * eccSquared / 256 + 45 * eccSquared * eccSquared * eccSquared / 1024) * sin(4 * latitudeRad)
             - (35 * eccSquared * eccSquared * eccSquared / 3072) * sin(6 * latitudeRad));

    easting = (double)(k0 * N * (A + (1 - T + C) * A * A * A / 6
                                 + (5 - 18 * T + T * T + 72 * C - 58 * eccPrimeSquared) * A * A * A * A * A / 120)
                       + 500000.0);

    northing = (double)(k0 * (M + N * tan(latitudeRad) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24
                                                          + (61 - 58 * T + T * T + 600 * C - 330 * eccPrimeSquared) * A * A * A * A * A * A / 720)));
    if (latitude < 0)
        northing += 10000000.0; //10000000 meter offset for southern hemisphere

    // calculo da letra da quadricula
    char letra;

    if ((84 >= latitude) && (latitude >= 72)) letra = 'X';
    else if ((72 > latitude) && (latitude >= 64)) letra = 'W';
    else if ((64 > latitude) && (latitude >= 56)) letra = 'V';
    else if ((56 > latitude) && (latitude >= 48)) letra = 'U';
    else if ((48 > latitude) && (latitude >= 40)) letra = 'T';
    else if ((40 > latitude) && (latitude >= 32)) letra = 'S';
    else if ((32 > latitude) && (latitude >= 24)) letra = 'R';
    else if ((24 > latitude) && (latitude >= 16)) letra = 'Q';
    else if ((16 > latitude) && (latitude >= 8)) letra = 'P';
    else if ((8 > latitude) && (latitude >= 0)) letra = 'N';
    else if ((0 > latitude) && (latitude >= -8)) letra = 'M';
    else if ((-8 > latitude) && (latitude >= -16)) letra = 'L';
    else if ((-16 > latitude) && (latitude >= -24)) letra = 'K';
    else if ((-24 > latitude) && (latitude >= -32)) letra = 'J';
    else if ((-32 > latitude) && (latitude >= -40)) letra = 'H';
    else if ((-40 > latitude) && (latitude >= -48)) letra = 'G';
    else if ((-48 > latitude) && (latitude >= -56)) letra = 'F';
    else if ((-56 > latitude) && (latitude >= -64)) letra = 'E';
    else if ((-64 > latitude) && (latitude >= -72)) letra = 'D';
    else if ((-72 > latitude) && (latitude >= -80)) letra = 'C';
    else letra = 'Z'; // flag indicando que a latitude esta fora dos limites UTM

    UTM utm;
    utm.easting=easting;
    utm.northing=northing;
    utm.zona=ZoneNumber;
    utm.letra=letra;
    return utm;
}
//********************************************************************************************************

LatLon ConverterLatLon(){
    int i;
    UTM coordutm;
    LatLon coordlatlon;

    coordForKml = (LatLon*) malloc(gNumVertPolig * sizeof(LatLon));

    for (i=0;i<gNumVertPolig;i++) {

        coordutm.easting= coordenadas[i].easting;
        coordutm.northing= coordenadas[i].northing;
        coordutm.zona = zona;
        coordutm.letra = hemisferio;
        coordlatlon=convertUTM2LatLon(coordutm, coordutm.zona, coordutm.letra);
        coordForKml[i].latitude = coordlatlon.latitude;
        coordForKml[i].longitude = coordlatlon.longitude;
        //printf("UTM (%lf,%lf)\n",coordutm.northing,coordutm.easting);
        //printf("LatLon (%lf,%lf)\n",coordlatlon.latitude,coordlatlon.longitude);
        //system("pause");
    }
}

//********************************************************************************************************

void createFileKml(){

     int i;
     FILE *fileKml;

     fileKml = fopen("fileKml.kml","w");
     fprintf(fileKml, "<?xml version=\"1.0\" encoding=\"UTF-8\"?> \n");
     fprintf(fileKml, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\"> \n");
     fprintf(fileKml, "  <Placemark> \n");
     fprintf(fileKml, "    <name>Areas de Desapropriação da Ilha de Itaparica até Vera Cruz - BA</name> \n");
     fprintf(fileKml, "    <description> \n");
     fprintf(fileKml, "\tO DECRETO Nº 13.389 DE 27 DE OUTUBRO DE 2011do Governo do Estado da Bahia declarou de utilidade pública,\n\t para fins de desapropriação, área de terra destinada à duplicação e implantação de obras de arte especiais na BA-001,\n\t nos Municípios de Itaparica e Vera Cruz - Bahia, conforme coordenadas da poligonal constantes do Anexo Único do Decreto.\n");
     fprintf(fileKml, "    </description> \n");
     fprintf(fileKml, "      <LinearRing> \n");
     fprintf(fileKml, "\t  <coordinates> \n");
     for(i=0;i<gNumVertPolig;i++){
        fprintf(fileKml,"\t\t %lf,%lf,0 \n",coordForKml[i].longitude,coordForKml[i].latitude);
     }
     fprintf(fileKml, "\t  </coordinates> \n");
     fprintf(fileKml, "\t</LinearRing> \n");
     fprintf(fileKml, "  </Placemark>\n");
     fprintf(fileKml, "</kml>");
     fclose(fileKml);
 }


int main() {
    int opcaoMenu;
    char opcaoSair;
    bool sair=false;
    char nomeArq[70];
    bool abriu = false;
    double area, perimetro;
    UTM coorde;
    LatLon coordseme;


    do {
        system("cls");
        printf ("\n\n======= Calculos com Poligonal ===============\n");
        printf("\n 1- Ler arquivo com Dados da Poligonal\n");
        printf("\n 6- Sair\n");
        printf("\n Selecione uma opcao do menu:");
        scanf("%i",&opcaoMenu);

        switch (opcaoMenu) {

            case 1:
                system("cls");

                printf("\n Digite o nome do arquivo para leitura: ");
                scanf("%s", nomeArq);
                abriu = ler_arquivo(nomeArq);

            case 2:
                system("cls");

                if (abriu)  {
                    do {

                        printf("\n 3- Calcular Area da Poligonal \n");
                        printf("\n 4- Calcular Perimetro da Poligonal\n");
                        printf("\n 5- Testar se um ponto (em coordenadas geograficas) pertence a poligonal\n");
                        printf("\n 6- Gerar Arquivo de Saida em KML \n");
                        printf("\n 7- Sair\n");
                        printf("\n Selecione uma opcao do menu:");
                        scanf("%i",&opcaoMenu);

                        system("cls");
                        switch (opcaoMenu) {
                                //system("cls");
                            case 3:
                                area = CalcularAreaPoligono(coordenadas,gNumVertPolig);
                                printf(" A area do poligono em metros quadrados e %lf : \n\n", area);
                                //  system("Pause");
                                break;
                            case 4:
                                perimetro = CalcularPerimetroPoligono(coordenadas, gNumVertPolig);
                                printf(" O perimetro do poligono em metros e %lf : \n\n", perimetro);
                                break;
                            case 5:
                                printf("\n Digite a latitude para verificacao: ");
                                scanf("%lf",&coordseme.latitude);
                                printf("\n Digite a longituder para verificacao: ");
                                scanf("%lf",&coordseme.longitude);
                                coorde=converteLatLon2UTM(coordseme);
                                if (isIn(coorde))
                                    printf("\n A coordenada fornecida está dentro da area de desapropriacao");
                                else
                                    printf("\n A coordenada fornecida nao esta dentro da area de desapropriacao");
                                break;

                            case 6:
                                ConverterLatLon();
                                createFileKml();
                                break;
                            case 7:
                                do {
                                    system("cls");
                                    printf("Digite S para sair e N para continuar:");
                                    scanf(" %c",&opcaoSair);
                                } while (opcaoSair!='S'&& opcaoSair!='s' && opcaoSair!='N' && opcaoSair!='n');
                                if (opcaoSair == 'S' || opcaoSair == 's') {
                                    sair=true;
                                }
                                break;

                            default:
                                printf("Opcao Invalida\n");
                                system("PAUSE");
                                break;

                        }
                    }while(!sair);
                }

                else{
                    printf("Arquivo nao foi lido corretamente.\n");
                    system("Pause");
                }

                break;
            case 8:
                do {
                    system("cls");
                    printf("Digite S para sair e N para continuar:");
                    scanf(" %c",&opcaoSair);
                } while (opcaoSair!='S'&& opcaoSair!='s' && opcaoSair!='N' && opcaoSair!='n');
                if (opcaoSair == 'S' || opcaoSair == 's') {
                    sair=true;
                }
                break;
            default:
                printf("Opcao Invalida\n");
                system("PAUSE");
                break;
        }
    } while(!sair);

}
