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
} coord;

typedef struct {
    coord v1,v2,v3;
} Triangulo;

coord *coordenadas;
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
        coordenadas = (coord*) malloc(gNumVertPolig * sizeof(coord));
        
        for (i=0; i < gNumVertPolig;i++){
            fscanf(ponteiro_arquivo,"%lf %lf",&coordenadas[i].easting,&coordenadas[i].northing);
        }
        
        fclose(ponteiro_arquivo);
        return true;
    }
}

double CalcularAreaPoligono (coord vertices[], int gNumVertPolig){
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
double CalcularPerimetroPoligono(coord vertices[], int gNumVertPolig){
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

Triangulo CriaTriangulo (coord p1, coord p2, coord pdado){
    Triangulo novo;
    
    novo.v1 = p1;
    novo.v2 = p2;
    novo.v3 = pdado;
    
    return novo;
}

//recebe uma coordenada "aleatoria" passada por quem deseja saber se esta inserida
bool isIn(coord pdado){
    int i;
    Triangulo n;
    double areaTotal = 0;
    
    for(i=0; i<gNumVertPolig-1;i++ ){
        n = CriaTriangulo(coordenadas[i], coordenadas[i+1], pdado);  //retorna os 3 pontos no formato (X,Y) in a struct
        areaTotal = CalculaAreaTriangulo(n) + areaTotal;
    }
    if (areaTotal == CalcularAreaPoligono(coordenadas,gNumVertPolig))  //poderia ser usado uma variavel global
        return true;
    return false;
}

int main() {
    int opcaoMenu;
    char opcaoSair;
    bool sair=false;
    char nomeArq[70];
    bool abriu = false;
    double area, perimetro;
    coord coorde;
    
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
                        
                        switch (opcaoMenu) {
                                system("cls");
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
                                printf("\n Digite a primeira coordenada para verificacao: ");
                                scanf("%lf",&coorde.easting);
                                printf("\n Digite a segunda coordenada para verificacao: ");
                                scanf("%lf",&coorde.northing);
                                if (isIn(coorde))
                                    printf("\n A coordenada fornecida está dentro da area de desapropriacao");
                                else
                                    printf("\n A coordenada fornecida nao esta dentro da area de desapropriacao");
                                break;
                                
                            case 6:
                              //  convertUTM2LatLon(coordenadas,zona,hemisferio);
                               // printf(" Arquivo utm e %lf : \n\n", UTM2LatLon);
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

