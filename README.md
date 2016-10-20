# CoordGeograf

Universidade do Estado da Bahia
DCET-I
Disciplina:
Linguagem de Programação I
Curso:
Sistemas de Informação
Professor (a):
Jorge Campos




O DECRETO Nº 13.389 DE 27 DE OUTUBRO DE 2011 do Governo do Estado da Bahia declarou de utilidade pública, para fins de desapropriação, área de terra destinada à duplicação e implantação de obras de arte especiais na BA-001, nos Municípios de Itaparica e Vera Cruz – Bahia, conforme coordenadas da poligonal constantes do Anexo Único do Decreto.
Você recebeu um arquivo texto com as coordenadas da poligonal publicada no diário oficial. O referido arquivo texto, com o nome “poligonal.txt”, possui a seguinte estrutura:

99 X 9999				Zona (int), Hemisfério (char) e Num. de vértices da poligonal (int)

9999999.999999  9999999.99999  
9999999.999999  9999999.99999 		Coordenadas UTM (easting,northing) (double)
...
9999999.999999  9999999.99999

Escreva um programa usando a linguagem C com as seguintes opções: 
Ler arquivo com dados da poligonal
Calcular Área da Poligonal
Calcular Perímetro da Poligonal
Testar se um ponto (em coordenadas geográficas) pertence a poligonal
Gerar Arquivo de Saída em KML
Sair


A opção 1 deverá solicitar o nome do arquivo com os dados da poligonal. A opção 6 sai do programa. 
As demais opções só devem ser habilitadas após a leitura bem-sucedida do arquivo (opção 1). A opção 2 calcula e imprime a área da poligonal. A opção 3 calcula e imprime o perímetro da poligonal. A opção 4 testa se um ponto dados pelas coordenadas geográficas (latidude, longitude) está contido na área de desapropriação. A opção 5 gera o arquivo no formato KML com as coordenadas da poligonal para visualização no Google Earth.

Informações Importantes:
DATA DA ENTREGA: 29/10/2016
O trabalho poderá ser desenvolvido em equipe com até 3 (três) participantes.
O programa deverá ser entregue em sala de aula por todos os membros da equipe.
Qualquer membro da equipe de desenvolvimento poderá ser questionado sobre o código fonte.
A ausência do aluno na entrega do trabalho implicará no seu desligamento da equipe. O aluno faltoso terá nota zero nesta avaliação 
Esta avaliação não contempla segunda chamada. 
