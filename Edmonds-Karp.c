
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct {

	int id;
	int menorCapacidade;

}ListaVertices;


ListaVertices preencherListaVertices(int id, int menorCapacidade){
	
	ListaVertices listaVertices;
	
	listaVertices.id = id;
	listaVertices.menorCapacidade = menorCapacidade;
	
	return listaVertices;
	
}


typedef struct noListaVertices {

	ListaVertices listaVertices;
	struct noListaVertices* proximo;
	struct noListaVertices* anterior;

}NoListaVertices;

NoListaVertices* criaListaVertices(NoListaVertices* topoListaVertices, int id, int menorCapacidade){
	
	NoListaVertices* novo = malloc(sizeof(NoListaVertices));
	
	if(novo){
		
		novo->listaVertices = preencherListaVertices(id, menorCapacidade);
		
		novo->proximo = topoListaVertices;
		novo->anterior = NULL;
		if(topoListaVertices) topoListaVertices->anterior = novo;	
		
	}else{
		
		printf("\nNao foi possivel alocar memoria para criar ListasVertices\n");
		system("pause");
		
	}
	
	return novo;
	
}


void empilhaListaVertices(NoListaVertices** topoListaVertices, NoListaVertices* novo){
		
		novo->proximo = *topoListaVertices;
		*topoListaVertices = novo;	
}

NoListaVertices* excluiListaVertices(NoListaVertices* ultimo, NoListaVertices** topoListaVertice){
	
	NoListaVertices *anterior = ultimo->anterior;
	
	if(anterior) anterior->proximo = NULL;
	else *topoListaVertice = NULL;
	
	return anterior;
		
}


typedef struct {

	int capacidade;
	int capacidadeOriginal;
	int origem;
	int destino;
	int qtdCritico;

}Aresta;

typedef struct noAresta {

	Aresta aresta;
	struct noAresta* proximo;
	struct noAresta* par;

}NoAresta;

Aresta preencherAresta(int capacidade, int origem, int destino){
	
	Aresta aresta;
	
	aresta.capacidade = capacidade;
	aresta.capacidadeOriginal = capacidade;
	aresta.origem = origem;
	aresta.destino = destino;
	aresta.qtdCritico = 0;
	
	return aresta;
	
}

NoAresta* encontraArestaPar(NoAresta* topoAresta, int destino){
	
	NoAresta *auxAresta = topoAresta;
	
	while(auxAresta && auxAresta->aresta.destino < destino){
		
		auxAresta = auxAresta->proximo;
		
	}
	
	if(auxAresta && auxAresta->aresta.destino != destino){
		
		auxAresta = NULL;
	}
	
	return auxAresta;
}

NoAresta *insereArestaNaLista(NoAresta *arestas[], NoAresta *novaAresta){
	
	NoAresta *anterior, *listaAresta = arestas[novaAresta->aresta.origem];
	
	if(listaAresta == NULL){
		
		novaAresta->proximo = NULL;
		
		arestas[novaAresta->aresta.origem] = novaAresta;
		
		return novaAresta;
		
	}else if(listaAresta->aresta.destino < novaAresta->aresta.destino){
	
			novaAresta->proximo = listaAresta;
			
			arestas[novaAresta->aresta.origem] = novaAresta;
			
			return novaAresta;
		
	}else{
		
		
		while(listaAresta && listaAresta->aresta.destino > novaAresta->aresta.destino){
			
			anterior = listaAresta;
			
			listaAresta = listaAresta->proximo;
		}
		
		if(listaAresta && listaAresta->aresta.destino == novaAresta->aresta.destino){
		
			listaAresta->aresta.capacidade = listaAresta->aresta.capacidade + novaAresta->aresta.capacidade;
			listaAresta->aresta.capacidadeOriginal = listaAresta->aresta.capacidadeOriginal + novaAresta->aresta.capacidadeOriginal;
	
			free(novaAresta);
			
			return listaAresta;
		
		}else{
			
			anterior->proximo = novaAresta;
			novaAresta->proximo = listaAresta;
		
			return novaAresta;
			
		}
		
		
	}
	
}

void criaAresta(NoAresta *arestas[], int capacidade, int origem, int destino){
	
	NoAresta* arestaPar = (NoAresta*)malloc(sizeof(NoAresta));
	
	NoAresta* novo = (NoAresta*) malloc(sizeof(NoAresta));
	
	if(novo && arestaPar){
		
		novo->aresta = preencherAresta(capacidade, origem, destino);
		
		arestaPar->aresta = preencherAresta(0, destino, origem);
		
		novo = insereArestaNaLista(arestas, novo);
		arestaPar = insereArestaNaLista(arestas, arestaPar);
		
		novo->par = arestaPar;
		arestaPar->par = novo;
		
	}else{
		
		printf("\nNao foi possivel alocar memoria para criar Aresta e Aresta Par\n");
		system("pause");
		
	}
	
	
}


int calculaFluxoMaximo(NoAresta *saidas){
	
	int fluxoMaximo = 0;
	
	while(saidas){
		
		fluxoMaximo = fluxoMaximo + saidas->aresta.capacidadeOriginal - saidas->aresta.capacidade;
		
		saidas = saidas->proximo;
		
	}
	
	return fluxoMaximo;
	
}

void excluiListaVerticesUsados(NoListaVertices* topoListaVertices, NoAresta* rotulo[]){
		
		NoListaVertices *anterior;
		
		while(topoListaVertices){
			
			rotulo[topoListaVertices->listaVertices.id] = NULL;
			
			anterior = topoListaVertices;
			
			topoListaVertices = topoListaVertices->proximo;
			
			free(anterior);
			
		}
		
			
}

void compilaCriticidadeArcos(NoAresta* arestas[], int quantidadeVertices, int *quantidadeArcosCriticos, int *totalVezesCriticos){
	
	int i;
	NoAresta* arestaAtual;
	
	
	for(i=0; i<quantidadeVertices; i++){
		
		arestaAtual = arestas[i];
		
		while(arestaAtual){
			
			if(arestaAtual->aresta.qtdCritico>0){
				
				*quantidadeArcosCriticos =  *quantidadeArcosCriticos + 1;	
				*totalVezesCriticos = *totalVezesCriticos + arestaAtual->aresta.qtdCritico;	
				
			}
			
			arestaAtual = arestaAtual->proximo;
			
		}
		
			
	}
	
	
}

void saveFile(int nr, int quantidadeAresta, int quantidadeVertice, int verticeInicio, int verticeFim, int fluxo, int tempo, int quantidadeOperacoes, int quantidadeBuscaLargura, int profundidadeBuscaLargura, int qtdeVerticeCritico, int criticidadeTotal) {

	FILE* pont_arqu;

	pont_arqu = fopen("resultado.csv", "a");

	fprintf(pont_arqu, "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;\n", nr, quantidadeVertice, quantidadeAresta, verticeInicio+1, verticeFim+1, fluxo, tempo, quantidadeOperacoes, quantidadeBuscaLargura, profundidadeBuscaLargura, qtdeVerticeCritico, criticidadeTotal);

	fclose(pont_arqu);

}

int main(){

	int i, instancia, nr=1, tempoInicial, tempoTotal, fluxoMaximo, verticeAtual, caminhoPossivel, menorCapacidade, maiorCapacidade = 20, indiceS=0, indiceT=3, quantidadeVertices = 6, quantidadeArestas;
	
	int quantidadeIteracoes=0, quantidadeBuscaEmLargura = 0, profundidadeBuscaEmLargura=0, quantidadeArcosCriticos = 0, totalVezesCriticos = 0;
	
	int mediaProfundidadeBuscaEmLargura = 0, buscaEmLargura = 0;
	
	NoListaVertices* topoListaVertices, *ultimoListaVertices, *verticesUsados;

	char *valor;
	
	char *valor1;
	
	char *valor2;	
	
	char espaco[100];

	fgets(espaco, 100, stdin);
	
	while (espaco[0] != 'p') {
		
		fgets(espaco, 100, stdin);
	
	}

	valor = strtok(espaco, " ");
	valor = strtok(NULL, " ");
	valor = strtok(NULL, " ");
	
	quantidadeVertices = atoi(valor);
	
	NoAresta* rotulo[quantidadeVertices];
	NoAresta* arestas[quantidadeVertices];
	
	for(i=0; i<quantidadeVertices; i++){
		
		rotulo[i] = NULL;
		arestas[i] = NULL;
		
	}

	valor = strtok(NULL, " ");
	
	quantidadeArestas = atoi(valor);

	while (espaco[0] != 'n') {
		
		fgets(espaco, 100, stdin);
	
	}
			
	valor = strtok(espaco, " ");
	valor = strtok(NULL, " ");
			
	indiceS = atoi(valor) - 1;			
				
	fgets(espaco, 100, stdin);
			
	valor = strtok(espaco, " ");
	valor = strtok(NULL, " ");
			
	indiceT = atoi(valor) - 1;	
	
	while (espaco[0] != 'a') {
		
		fgets(espaco, 100, stdin);
	
	}
	
	while (!feof(stdin) && espaco[0] == 'a') {
		
		valor = strtok(espaco, " ");
		valor = strtok(NULL, " ");
		valor1 = strtok(NULL, " ");
		valor2 = strtok(NULL, " ");
	
		criaAresta(arestas, atoi(valor2), atoi(valor)-1, atoi(valor1)-1);
		
		if(maiorCapacidade < atoi(valor2)) maiorCapacidade = atoi(valor2); 
	
		fgets(espaco, 100, stdin);
	
	}
						
	NoAresta *arestaAtual;
	NoAresta *par;
	
	caminhoPossivel = 1;
	
	tempoInicial = time(NULL);
	
	while(caminhoPossivel==1){
		
		quantidadeIteracoes++;
		
		rotulo[indiceT] = NULL;
		
		caminhoPossivel = 0;
		
		topoListaVertices = NULL;
		
		topoListaVertices = criaListaVertices(topoListaVertices, indiceS, maiorCapacidade);
		
		verticesUsados = NULL;
		
		ultimoListaVertices = topoListaVertices;
		
		mediaProfundidadeBuscaEmLargura = 0;
		
		buscaEmLargura = 0;
		
		while(topoListaVertices && rotulo[indiceT] == NULL){
			
			buscaEmLargura++;
			
			verticeAtual = ultimoListaVertices->listaVertices.id;
			
			arestaAtual = arestas[verticeAtual];
			
			while(arestaAtual && rotulo[indiceT] == NULL){
				
				mediaProfundidadeBuscaEmLargura++;
				
				if(arestaAtual->aresta.capacidade>0 && rotulo[arestaAtual->aresta.destino] == NULL){
				
					if(arestaAtual->aresta.capacidade < ultimoListaVertices->listaVertices.menorCapacidade){
						
						menorCapacidade = arestaAtual->aresta.capacidade;
						
					}else{
						
						menorCapacidade = ultimoListaVertices->listaVertices.menorCapacidade;
						
					}
						
					rotulo[arestaAtual->aresta.destino] = arestaAtual;
						
					topoListaVertices = criaListaVertices(topoListaVertices, arestaAtual->aresta.destino, menorCapacidade);	
				
				}
				
				arestaAtual = arestaAtual->proximo;
				
			}
			
			
			empilhaListaVertices(&verticesUsados, ultimoListaVertices);
			
			ultimoListaVertices = excluiListaVertices(ultimoListaVertices, &topoListaVertices);
			
		}
		
		profundidadeBuscaEmLargura = profundidadeBuscaEmLargura + (mediaProfundidadeBuscaEmLargura/buscaEmLargura);
		
		quantidadeBuscaEmLargura = quantidadeBuscaEmLargura + buscaEmLargura;
		
		//se saiu aqui ou não tem mais caminhou ou chegou-se ao T
		if(rotulo[indiceT] != NULL){
			
			caminhoPossivel=1;
			
			i = indiceT;
			
			while(i!=indiceS){
			
				rotulo[i]->aresta.capacidade = 	rotulo[i]->aresta.capacidade - menorCapacidade;
				
				if(rotulo[i]->aresta.capacidade == 0) rotulo[i]->aresta.qtdCritico = rotulo[i]->aresta.qtdCritico + 1;
				
				par = rotulo[i]->par;
				
				par->aresta.capacidade = par->aresta.capacidade + menorCapacidade;
				
				i = rotulo[i]->aresta.origem;
				
			}
			
			
			excluiListaVerticesUsados(topoListaVertices, rotulo);
			excluiListaVerticesUsados(verticesUsados, rotulo);
			
		}
		
		
	}

	tempoTotal = time(NULL) - tempoInicial;

	fluxoMaximo = calculaFluxoMaximo(arestas[indiceS]);
	
	fprintf(stdout, "%d", fluxoMaximo);
	
	//compilaCriticidadeArcos(arestas, quantidadeVertices, &quantidadeArcosCriticos, &totalVezesCriticos);
	
	//saveFile(nr, quantidadeArestas, quantidadeVertices, indiceS, indiceT, fluxoMaximo, tempoTotal, quantidadeIteracoes, quantidadeBuscaEmLargura, profundidadeBuscaEmLargura, quantidadeArcosCriticos, totalVezesCriticos);
		
    return 0;
}
