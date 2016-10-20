#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef struct {
	double northing;
	double easting;
	//int zona;
	//char letra;
} UTM;
typedef struct {
	double latitude;
	double longitude;
} LatLon;
double degree2rad(double);
double rad2degree(double);
LatLon convertUTM2LatLon (UTM, int, char);
UTM converteLatLon2UTM(LatLon);
/*main() {
	
		UTM coordutm;
		LatLon coordlatlon;
		coordutm.easting=536927.070;		
		coordutm.northing=8572722.548;

 		coordlatlon=convertUTM2LatLon(coordutm,24,'S');
 		printf("UTM (%lf,%lf)\n",coordutm.northing,coordutm.easting);
 		printf("LatLon (%lf,%lf)\n",coordlatlon.latitude,coordlatlon.longitude);
        system("pause");       
}*/
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
		//	utm.zona=ZoneNumber;
			//utm.letra=letra;
            return utm;
        }

