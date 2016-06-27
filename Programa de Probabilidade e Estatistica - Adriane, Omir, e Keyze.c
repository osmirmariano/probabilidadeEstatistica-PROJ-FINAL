#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>
int posiMaiorFi = 0, flag = 0;

/* Trabalho CALCULA VALORES - Probabilidade e Estatistica*/

/*--------------------------------INÍCIO DAS DECLARAÇÕES DAS ESTRUTURAS------------------------------------*/
typedef struct DadosTabRol{
    float XiRol;
    int FiRol;
}DadosTabRol;

typedef struct NoTabRol{
    DadosTabRol dadosRol;
    struct NoTabRol *prox;
}NoTabRol;

typedef struct TabRol{
    NoTabRol *Inicio;
    NoTabRol *Fim;
    int quant;
    int SomaFrequencia;
}TabRol;
//TabRol - Tabela primaria de elementos e frequencias

typedef struct dadosTab{
    int classe;
    float limInf;
    float limSup;
    float frequencia;
}dadosTab;

typedef struct NoTab {
    dadosTab DadosTab;
    struct NoTab *prox;
} NoTab;

typedef struct TAB {
    NoTab *inicio;
    NoTab *fim;
    int tam;
} TAB;
// TAB É A PRIMEIRA TABELA

typedef struct CONTEUDOGERAL {
    float media;
    float amplitude;
    float somaFi;
    float maiorFi;
    float somaXi;
    float somaXiFi;
    float somaXi2Fi;
    float somadiFi;
} CONTEUDOGERAL;

typedef struct NoDados {
    CONTEUDOGERAL dados;
    struct NoDados *seg;
} NoDados;

typedef struct DADOS{
    NoDados *comeco;
    NoDados *fim;
    int qtd;
} DADOS;
// TABELA DADOS GUARDA RESULTADOS DAS FÓRMULAS

typedef struct CONTEUDODADOSTABELA {
    float Xi;
    float XiFi;
    float Fac;
    float Xi2Fi;
    float diXiX;
    float diFi;
    float Fih;
    float AmplitudeClasse;
} CONTEUDODADOSTABELA;

typedef struct NoDadosTabela {
    CONTEUDODADOSTABELA conteudo;
    struct NoDadosTabela *seguinte;
} NoDadosTabela;

typedef struct DADOSTABELA {
    NoDadosTabela *inicia;
    NoDadosTabela *finaliza;
    int linha;
} DADOSTABELA;
// DADOSTABELA GUARDA VALORES PARA CÁLCULO DAS EQUAÇÕES
/*--------------------------------FINAL DAS DECLARAÇÕES DAS ESTRUTURAS--------------------------------*/


/*------------------------------------------INÍCIO DAS FUNÇÕES INICIA---------------------------------*/
void Inicia(TAB *t) {
    t->inicio = NULL;
    t->fim = NULL;
    t->tam = 0;
}

void IniciaDados(DADOS *d) {
    d->comeco = NULL;
    d->fim = NULL;
    d->qtd = 0;
}

void IniciaDadosTabela(DADOSTABELA *d) {
    d->inicia = NULL;
    d->finaliza = NULL;
    d->linha = 0;
}

void IniciaTabRol(TabRol *r){
    r->Inicio = NULL;
    r->Fim = NULL;
    r->quant = 0;
    r->SomaFrequencia = 0;
}
/*------------------------------------------FINAL DAS FUNÇÕES INICIA----------------------------------*/

/* ---------------------------- Destrutor ----------------------------*/
void TabelaDestrutor (TAB *t) {
	NoTab *temp, *tempValor;
	while(t->inicio != NULL) {
		temp = t->inicio;
		tempValor = temp;
		t->inicio = t->inicio->prox;
		t->tam--;
		free(tempValor);
	}
	t->fim = NULL;
	//printf("\n\n\n Tabela vazia!\n\n\n");
}

void ResultadosGeralDestrutor (DADOS *d) {
	NoDados *temp, *tempValor;
	while(d->comeco != NULL) {
		temp = d->comeco;
		tempValor = temp;
		d->comeco = d->comeco->seg;
		d->qtd--;
		free(tempValor);
	}
	d->fim = NULL;
	//printf("\n\n\n Dados gerais excluidos\n\n\n");
}

void TabelaResultadosDestrutor (DADOSTABELA *v) {
	NoDadosTabela *temp, *tempValor;
	while(v->inicia != NULL) {
		temp = v->inicia;
		tempValor = temp;
		v->inicia = v->inicia->seguinte;
		v->linha--;
		free(tempValor);
	}
	v->finaliza = NULL;
	//printf("\n\n\n Tabela de resultados vazia!\n\n\n");
}

void TabelaRolDestrutor (TabRol *r) {
	NoTabRol *temp, *tempValor;
	while(r->Inicio != NULL) {
		temp = r->Inicio;
		tempValor = temp;
		r->Inicio = r->Inicio->prox;
		r->quant--;
		free(tempValor);
	}
	r->Fim = NULL;
}
/* ---------------------------- Destrutor ----------------------------*/
/*---Funções Inserir-----*/
/*------------------------------------INICÍO DA FUNÇÃO INSERI DADOS TABELA----------------------------*/
void InseriDadosTabela (DADOSTABELA *d, CONTEUDODADOSTABELA  x) {
    NoDadosTabela *novo ;
    novo= (NoDadosTabela*)malloc(sizeof(NoDadosTabela));
    if(novo != NULL) {
        novo->conteudo=x;
        novo->seguinte = NULL;
        d->linha++;
    }
    else
        printf("\n\t Nao existe nenhum valor inserido!\n");
    if (d->inicia == NULL) {
        d->inicia = novo;
    }
    else {
        d->finaliza->seguinte = novo;
    }
    d->finaliza = novo;
}
/*------------------------------------FINAL DA FUNÇÃO INSERI DADOS TABELA-----------------------------*/


/*------------------------------------INICÍO DA FUNÇÃO INSERI DADOS GERAL-----------------------------*/
void InseriDadosGeral (DADOS *d, CONTEUDOGERAL x) {
    NoDados *novo ;
    novo= (NoDados*)malloc(sizeof(NoDados));
    if(novo != NULL) {
        novo->dados.media = x.media;
        novo->dados.amplitude = x.amplitude;
        novo->dados.somaFi = x.somaFi;
        novo->dados.maiorFi = x.maiorFi;
        novo->dados.somaXi = x.somaXi;
        novo->dados.somaXiFi = x.somaXiFi;
        novo->dados.somaXi2Fi = x.somaXi2Fi;
        novo->dados.somadiFi = x.somadiFi;
        novo->seg = NULL;
        d->qtd++;
    }
    else
        printf("\n\n WARNING ! Erro de memoria!\n");
    if (d->comeco == NULL) {
        d->comeco = novo;
    }
    else {
        d->fim->seg = novo;
    }
    d->fim = novo;
}
/*------------------------------------FINAL DA FUNÇÃO INSERI DADOS GERAL------------------------------*/


/*------------------------------------INÍCIO DA FUNÇÃO INSERE TAB-------------------------------------*/
void InsereTab (TAB *t, dadosTab x) {
    NoTab *novo = (NoTab*)malloc(sizeof(NoTab));
    if(novo != NULL) {
        novo->DadosTab.classe= t->tam;
        novo->DadosTab= x;
        novo->prox = NULL;
        t->tam++;
        if (t->inicio == NULL) {
            t->inicio = novo;
         }
        else {
            t->fim->prox = novo;
          }
        t->fim = novo;
    }
    else
        printf("\n --> ERRO DE MEMORIA!\n");
}
/*-------------------------------------FINAL DA FUNÇÃO INSERE TAB-------------------------------------*/


/*----------------------------------INÍCIO DA FUNÇÃO INSERE TAB ROL-----------------------------------*/
void InsereTabRol(TabRol *r,DadosTabRol w){
    NoTabRol *novo ;
    novo= (NoTabRol*)malloc(sizeof(NoTabRol));
    if(novo != NULL) {
        novo->dadosRol=w;
        novo->prox = NULL;
        r->quant++;
        if (r->Inicio == NULL) {
            r->Inicio = novo;
         }
        else {
            r->Fim->prox= novo;
          }
        r->Fim = novo;

    }
    else
        printf("\n --> ERRO DE MEMORIA!\n");
}
/*----------------------------------FINAL DA FUNÇÃO INSERE TAB ROL------------------------------------*/


/*------------------------------------INICÍO DA FUNÇÃO DEFINE DADOS-----------------------------------*/
void DefineDados (TAB *t, DADOS *d, DADOSTABELA *v) {
    NoTab *aux ;
    NoDadosTabela *aux2;
    aux = t->inicio;
    CONTEUDOGERAL n;
    CONTEUDODADOSTABELA q;

    float maiorFi=0, somaFi=0, somaXi=0, somaXiFi=0, somaXi2Fi=0, somadiFi=0,mediaClasse=0;
    float ampMenorFi=0, ampMaiorFi=0, ampTtl=0, Xi=0, XiFi=0, Fac=0, Xi2Fi=0, diXiX=0, Fih=0, h=0,diFi=0;

    if(t->tam>0){
        maiorFi = t->inicio->DadosTab.frequencia;
        ampMenorFi = t->inicio->DadosTab.limInf;
        ampMaiorFi = t->inicio->DadosTab.limSup;
        while(aux != NULL) {
            //USAR EM OUTROS CÁLCULOS
            if (aux->DadosTab.frequencia > maiorFi)
                maiorFi = aux->DadosTab.frequencia;
            if (aux->DadosTab.limInf < ampMenorFi)
                ampMenorFi = aux->DadosTab.limInf;
            if (aux->DadosTab.limSup > ampMaiorFi )
                ampMaiorFi = aux->DadosTab.limSup;
            aux = aux->prox;
        }
        ampTtl = ampMaiorFi-ampMenorFi;
        aux = t->inicio;
        while(aux != NULL){
            //TABELA DADOSTABELA
            h = aux->DadosTab.limSup - aux->DadosTab.limInf; //Amplitude da Classe
            Xi = (aux->DadosTab.limSup/2) + (aux->DadosTab.limInf/2); // Media da Classe
            XiFi = Xi* aux->DadosTab.frequencia;
            Fac = Fac + aux->DadosTab.frequencia; // Frequencia acumulada
            Xi2Fi = (Xi*Xi) * aux->DadosTab.frequencia;
            Fih = aux->DadosTab.frequencia / h;
            	if( Fih < 0)
               		Fih = Fih *(-1);
            //------------------------------------------
            somaFi = somaFi+aux->DadosTab.frequencia; // Soma das frequencias
            somaXi = somaXi + Xi; //Soma das médias das classes
            somaXiFi = somaXiFi + XiFi;
            somaXi2Fi = somaXi2Fi + Xi2Fi;
            //------------------------------------------
            q.Xi = Xi;
            q.XiFi = XiFi;
            q.Fac = Fac;
            q.Xi2Fi = Xi2Fi;
            q.diXiX = diXiX;
            q.Fih = Fih;
            q.AmplitudeClasse=h;
            InseriDadosTabela (v,q);
            aux = aux->prox;

        }
        mediaClasse = somaXiFi/somaFi;
        aux = t->inicio;
        aux2 = v->inicia;
        while(aux!=NULL){
            diXiX = (aux2->conteudo.Xi - (mediaClasse));
            if( diXiX < 0)
                diXiX = diXiX *(-1);
            diFi = diXiX * aux->DadosTab.frequencia;
            somadiFi = somadiFi + diFi;
            aux2->conteudo.diFi=diFi;
            aux2->conteudo.diXiX=diXiX;
            aux=aux->prox;
            aux2=aux2->seguinte;
        }
        //------------------------------------------
        n.media = mediaClasse;
        n.amplitude = ampTtl;
        n.somaFi = somaFi;
        n.maiorFi = maiorFi;
        n.somaXi = somaXi;
        n.somaXiFi = somaXiFi;
        n.somaXi2Fi = somaXi2Fi;
        n.somadiFi = somadiFi;
        /*------------------------------------------*/
        InseriDadosGeral (d,n);
        // amplitude e difi
    }
}
/*------------------------------------FINAL DA FUNÇÃO DEFINE DADOS------------------------------------*/


/*------Funções Imprimir------*/
/*-----------------------------INÍCIO DA FUNÇÃO IMPRIME DADOS INSERIDOS------------------------------*/
void ImprimeDadosInseridos (TAB *t) {
    NoTab *atual = t->inicio;  int cont = 1;
    if(atual == NULL) {
        printf("\n\n --------------------------------\n");
        printf("       TABELA DE FREQUENCIAS\n");
        printf(" --------------------------------");
        printf("\n 0 elementos na tabela\n");
        printf("\n --------------------------------\n\n\n");
    }
    else {
        printf("\n\n -------------------------------------\n");
        printf("          TABELA DE FREQUENCIAS\n");
        printf(" -------------------------------------");
        printf("\n     CLASSE      |    FREQUENCIA");
        printf("\n -------------------------------------");
        while (atual != NULL) {
            printf("\n%5.0f |--%5.0f   |    %5.0f\n", atual->DadosTab.limInf, atual->DadosTab.limSup, atual->DadosTab.frequencia);
            atual = atual->prox;
            cont ++;
        }
        printf("\n -------------------------------------\n\n\n");
    }
}
/*-----------------------------FINAL DA FUNÇÃO IMPRIME DADOS INSERIDOS-------------------------------*/


/*--------------------------INÍCIO DA FUNÇÃO IMPRIME TABELA DE RESULTADOS----------------------------*/
void ImprimeTabelaDeResultados (TAB *t, DADOS *d, DADOSTABELA *v) {

    NoTab *atual ;
    NoDados *usa;
    NoDadosTabela *aux ;

    atual= t->inicio;
    usa = d->comeco;
    aux = v->inicia;

    if(atual == NULL || aux == NULL) { //t->tam == d->qtd
        printf("\n\n --------------------------------\n");
        printf("         TABELA DE RESULTADOS\n");
        printf(" --------------------------------");
        printf("\n 0 resultados na tabela\n");
        printf("\n --------------------------------\n\n\n");
    }
    else {
        printf("\n\n -----------------------------------------------------------------------------------------------------------\n");
        printf("\t\t\t\t\t     TABELA DE RESULTADOS");
        printf("\n -----------------------------------------------------------------------------------------------------------\n");
        printf("\n      CLASSE    |    Fi   |     Xi    |    XiFi   |     Fac   |  di-(Xi-X) |    Xi2Fi   |    Fi/h   | |di|Fi");
        printf("\n -----------------------------------------------------------------------------------------------------------\n");
        while (atual != NULL && aux != NULL) {
            printf("\n%5.0f |--%5.0f  | %6.2f  | %7.2f   | %8.2f  | %7.2f   | %8.2f   | %9.2f  |  %6.2f   | %6.2f\n",
                atual->DadosTab.limInf, atual->DadosTab.limSup, atual->DadosTab.frequencia, aux->conteudo.Xi, aux->conteudo.XiFi,
                aux->conteudo.Fac, aux->conteudo.diXiX, aux->conteudo.Xi2Fi, aux->conteudo.Fih, aux->conteudo.diFi);
            atual = atual->prox;
            aux = aux->seguinte;
        }
        printf("\n -----------------------------------------------------------------------------------------------------------");
        while (usa!= NULL) {
            printf("\n  SOMA\t        | %6.2f  | %7.2f   | %8.2f  |      -    |      -     | %9.2f  |   %6.2f  |\n",
           usa->dados.somaFi, usa->dados.somaXi, usa->dados.somaXiFi, usa->dados.somaXi2Fi, usa->dados.somadiFi);
            usa = usa->seg;
        }
        printf(" -----------------------------------------------------------------------------------------------------------");
        printf ("\n  Amplitude das subclasses: %8.2f\n", v->inicia->conteudo.AmplitudeClasse);
        printf(" -----------------------------------------------------------------------------------------------------------\n\n\n");
    }
}
/*---------------------------FINAL DA FUNÇÃO IMPRIME TABELA DE RESULTADOS----------------------------*/


/*----------------------------INÍCIO DA FUNÇÃO IMPRIME RESULTADO GERAL-------------------------------*/
void ImprimeResultadoGeral (DADOS *d) {
    NoDados *aux ;
    aux = d->comeco;

    if(aux == NULL) {
        printf("\n\n ---------------------------\n");
        printf("       RESULTADO GERAL\n");
        printf(" ---------------------------");
        printf("\n 0 resultados\n");
        printf("\n ---------------------------\n\n\n");
    }
    else {
        printf("\n\n ----------------------------------\n");
        printf("          RESULTADO GERAL\n");
        printf(" ----------------------------------\n");
        printf ("   DESCRICAO        |  RESULTADO\n");
        printf(" ----------------------------------");
        while (aux!= NULL) {
            printf ("\n  Media             |   %3.0f\n", aux->dados.media);
            printf ("\n  Amplitude         |   %3.0f\n", aux->dados.amplitude);
            printf ("\n  Maior frequencia  |  %3.0f\n", aux->dados.maiorFi);
            printf ("\n  Soma Fi           |   %3.0f\n", aux->dados.somaFi);
            printf ("\n  Soma Xi           |    %3.0f\n", aux->dados.somaXi);
            printf ("\n  Soma XiFi         |    %3.0f\n", aux->dados.somaXiFi);
            printf ("\n  Soma diFi         |    %3.0f\n", aux->dados.somadiFi);
            printf ("\n  Soma Xi2Fi        |    %3.0f\n", aux->dados.somaXi2Fi);
            aux = aux->seg;
        }
    printf("\n ----------------------------------\n\n\n");
    }
}
/*-----------------------------FINAL DA FUNÇÃO IMPRIME RESULTADO GERAL-------------------------------*/


/*-----------------------------INÍCIO DA FUNÇÃO IMPRIME TABELA ROL-----------------------------------*/
void ImprimeTabelaRol(TabRol *r){
    NoTabRol *aux ;
    aux = r->Inicio;
    if(aux == NULL) {
        printf("\n\n --------------------------------\n");
        printf("      DISTRIBUICAO DE FREQUENCIAS\n");
        printf(" --------------------------------");
        printf("\n 0 resultados\n");
        printf("\n --------------------------------\n\n\n");
    }
    else {
        printf("\n\n -----------------------------\n");
        printf("  DISTRIBUICAO DE FREQUENCIAS\n");
        printf(" -----------------------------");
        printf("\n        Xi     |      Fi\n");
        printf(" -----------------------------\n\n");
        while (aux!= NULL) {
            printf("    %6.2f     |      %d  \n",aux->dadosRol.XiRol,aux->dadosRol.FiRol);
            aux=aux->prox;
        }
        printf("\n -----------------------------");
        printf("\n    Soma       |      %d\n",r->SomaFrequencia);
    }
    printf(" -----------------------------\n\n\n");
}
/*-----------------------------FINAL DA FUNÇÃO IMPRIME TABELA ROL-----------------------------------*/

void VerificaExcecao (int k) { // Trata exceção divisão pro zero
	if (flag == 1) {
		if (k == 1)
	    	printf ("\tImpossivel dividir por zero!");
		else
	    	printf ("\tRequer valor da moda nao possivel calcular!");
	}
	printf ("\n");
}
/*--------Operações Resultados-----*/
/*--------------------------------INÍCIO DA FUNÇÃO DESVIO MÉDIO--------------------------------------*/
float DesvioMedio (DADOS *d) {
    NoDados *aux ;
    aux = d->comeco;
    float DM = 0;
    DM = aux->dados.somadiFi / aux->dados.somaFi;
    return DM;
}
/*--------------------------------FINAL DA FUNÇÃO DESVIO MÉDIO--------------------------------------*/


/*--------------------------INÍCIO DA FUNÇÃO VARIÂNCIA POPULACIONAL---------------------------------*/
float VarianciaPopulacional (DADOS *d) {
    NoDados *aux ;
    aux = d->comeco;
    float R2 = 0;
    R2 = (1 / aux->dados.somaFi) * ((aux->dados.somaXi2Fi) - ((aux->dados.somaXiFi * aux->dados.somaXiFi) / aux->dados.somaFi));
    return R2;
}
/*--------------------------FINAL DA FUNÇÃO VARIÂNCIA POPULACIONAL----------------------------------*/


/*---------------------------INÍCIO DA FUNÇÃO DESVIO PADRÃO POPULACIONAL-----------------------------*/
float DesvioPadraoPopulacional (DADOS *d) {
    float R, R2 = 0;
    R2 =  VarianciaPopulacional (d);
    R = sqrt (R2);
    return R;
}
/*----------------------------FINAL DA FUNÇÃO DESVIO PADRÃO POPULACIONAL-----------------------------*/


/*----------------------INÍCIO DA FUNÇÃO COEFICIENTE DE VARIÂNCIA POPULACIONAL-----------------------*/
float CoefVarPopulacional (DADOS *d) {
    NoDados *aux ;
    aux = d->comeco;
    float CV = 0;
    CV = (DesvioPadraoPopulacional(d) / aux->dados.media) * 100;
    return CV;
}
/*----------------------FINAL DA FUNÇÃO COEFICIENTE DE VARIÂNCIA POPULACIONAL-----------------------*/


/*----Coeficiente de Assimetria (1º Coeficiente de Person)-----*/
/*----------------------INÍCIO DA FUNÇÃO COEFICIENTE DE ASSIMETRIA (MODA)----------------------------*/
float Moda (TAB *t, DADOSTABELA *d) {
    NoTab *atual;
    NoDadosTabela *aux;
    atual= t->inicio;
    aux = d->inicia;
    int cont = 0;
    float FiAnt = 0, FiAtual = 0, FiPost = 0, LiMd = 0, MD = 0, Delta1 = 0, Delta2 = 0;
    if(atual == NULL) {
        printf("\n\n Erro!\n\n\n");
    }
    else {
        while (atual != NULL ) {
            if(cont == posiMaiorFi-1) {
                FiAnt = atual->DadosTab.frequencia;
            }
            if(cont == posiMaiorFi) {
                FiAtual = atual->DadosTab.frequencia;
                LiMd = atual->DadosTab.limInf;
            }
            if(cont >= posiMaiorFi+1) {
                FiPost = atual->DadosTab.frequencia;
                break;
            }
            atual = atual->prox;
            cont++;
        }
        Delta1 = FiAtual - FiAnt;
        Delta2 = FiAtual - FiPost;
//        printf ("\n Delta1 moda %0.2f    Delta2 moda %0.2f\n ", Delta1, Delta2);
        if ( (Delta1 + Delta2) != 0) {
	    	MD = LiMd + ( (Delta1 / (Delta1 + Delta2)) * aux->conteudo.AmplitudeClasse );
	    	flag = 0;
	    } else {	
        	//printf ("\n ERRO: Impossivel dividir por zero\n ");
	    	flag = 1;
    	}
    }
    return MD;
}
/*----------------------FINAL DA FUNÇÃO COEFICIENTE DE ASSIMETRIA (MODA)----------------------------*/


/*------------------INÍCIO DA FUNÇÃO COEFICIENTE DE ASSIMETRIA POPULACIONAL-------------------------*/
float CoeficienteDeAssimetriaPopulacional (TAB *t, DADOS *d, DADOSTABELA *f) {
    NoDados *aux;
    aux = d->comeco;
    float AS = 0, MD = 0, DV = 0;
    MD = Moda (t, f);
    DV = DesvioPadraoPopulacional(d);
    AS = (aux->dados.media - MD) / DV;
    if (flag == 1)
    	AS = 0;
    return AS;
}
/*-------------------FINAL DA FUNÇÃO COEFICIENTE DE ASSIMETRIA POPULACIONAL-------------------------*/


/*--------Curtose------*/
/*--------------------------INÍCIO DA FUNÇÃO CURTOSE (QUARTIL 1)-----------------------------------*/
float Quartil1 (TAB *t, DADOS *d, DADOSTABELA *f) {

    NoTab *atual;
    NoDadosTabela *aux;
    NoDados *usa;
    atual= t->inicio;
    aux = f->inicia;
    usa = d->comeco;
    int cont = 0, PosElemQ1 = 0, PosClasseQ1 = 0;
    float Q1 = 0, LiQ1 = 0, FiAtual = 0, FacAnt = 0;
    PosElemQ1 = usa->dados.somaFi / 4;

    if(atual == NULL ) {
        printf("\n\n Erro!\n\n\n");
    }
    else {
        while (aux != NULL ) {
            if(aux->conteudo.Fac >= PosElemQ1) {
                PosClasseQ1 = cont;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        cont = 0;
        while (atual != NULL ) {
            if(cont == PosClasseQ1) {
                LiQ1 = atual->DadosTab.limInf;
                FiAtual = atual->DadosTab.frequencia;
                break;
            }
            atual = atual->prox;
            cont++;
        }
        cont = 0;
        aux = f->inicia;
        while (aux != NULL ) {
            if(cont == PosClasseQ1-1) {
                FacAnt = aux->conteudo.Fac;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        Q1 = LiQ1 + ( ( (PosElemQ1 - FacAnt) / FiAtual) * f->inicia->conteudo.AmplitudeClasse );
    }
    return Q1;
}
/*----------------------------FINAL DA FUNÇÃO CURTOSE (QUARTIL 1)-----------------------------------*/


/*--------------------------INÍCIO DA FUNÇÃO CURTOSE (QUARTIL 3)------------------------------------*/
float Quartil3 (TAB *t, DADOS *d, DADOSTABELA *f) {
    NoTab *atual;
    NoDadosTabela *aux;
    NoDados *usa;
    atual= t->inicio;
    aux = f->inicia;
    usa = d->comeco;
    int cont = 0, PosClasseQ3 = 0, PosElemQ3 = 0;
    float Q3 = 0, LiQ3 = 0, FiAtual = 0, FacAnt = 0;
    PosElemQ3 = (3 * usa->dados.somaFi) /4;
    //printf("\n somaFi: %0.2f\n", usa->dados.somaFi);
    //printf("\n Posicao elemento Q3: %d\n",PosElemQ3);

    if(atual == NULL) {
        printf("\n\n Erro!\n\n\n");
    }
    else {
        while (aux != NULL ) {
            if(aux->conteudo.Fac >= PosElemQ3) {
                PosClasseQ3 = cont;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        //printf("\n Posicao da Classe Q3: %d\n", PosClasseQ3);
        cont = 0;
        while (atual != NULL ) {
            if(cont == PosClasseQ3) {
                LiQ3 = atual->DadosTab.limInf;
                FiAtual = atual->DadosTab.frequencia;
                break;
            }
            atual = atual->prox;
            cont++;
        }
        //printf(" Limite Inferior: %0.2f.\n Frequencia atual: %0.2f\n", LiQ3, FiAtual);

        cont = 0;
        aux = f->inicia;
        while (aux != NULL ) {
            //printf("\n aux: %0.2f\ \n\n\n", aux->conteudo.Fac);
            if(cont == PosClasseQ3-1) {
                FacAnt = aux->conteudo.Fac;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        //printf ("\n Fac anterior da Q3:  %0.2f\n ", FacAnt);
        //printf ("\n Amplitude das subclasses %0.2f\n ", f->inicia->conteudo.AmplitudeClasse);
        Q3 = LiQ3 + ( ( (PosElemQ3 - FacAnt) / FiAtual) *  f->inicia->conteudo.AmplitudeClasse);
        //printf ("\n Quartil 3:  %0.2f\n ", Q3);
        //printf ("\n -----------------------------------------\n");
    }
    return Q3;
}
/*----------------------------FINAL DA FUNÇÃO CURTOSE (QUARTIL 3)-----------------------------------*/


/*----------------------------INÍCIO DA FUNÇÃO CURTOSE (PERCENTIL 10)-------------------------------*/
float Percentil10 (TAB *t, DADOS *d, DADOSTABELA *f) {
    NoTab *atual;
    NoDadosTabela *aux;
    NoDados *usa;
    atual= t->inicio;
    aux = f->inicia;
    usa = d->comeco;
    int cont = 0, PosClasseP10 = 0, PosElemP10 = 0;
    float P10 = 0, LiP10 = 0, FiAtual = 0, FacAnt = 0;
    PosElemP10 = (10 * usa->dados.somaFi) /100;
    //printf("\n somaFi: %0.2f\n", usa->dados.somaFi);
    //printf("\n Posicao elemento P10: %d\n", PosElemP10);

    if(atual == NULL) {
        printf("\n\n Erro!\n\n\n");
    }
    else {
        while (aux != NULL ) {
            if(aux->conteudo.Fac >= PosElemP10) {
                PosClasseP10 = cont;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        //printf("\n Posicao do Percentil 10: %d.\n", PosClasseP10);
        cont = 0;
        while (atual != NULL ) {
            if(cont == PosClasseP10) {
                LiP10 = atual->DadosTab.limInf;
                FiAtual = atual->DadosTab.frequencia;
                break;
            }
            atual = atual->prox;
            cont++;
        }
        //printf(" Limite Inferior: %0.2f.\n Frequencia atual: %0.2f\n", LiP10, FiAtual);
        cont = 0;
        aux = f->inicia;
        while (aux != NULL ) {
            if(cont == PosClasseP10-1) {
                FacAnt = aux->conteudo.Fac;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        //printf ("\n Fac anterior do P10:  %0.2f\n ", FacAnt);
        //printf ("\n Amplitude das subclasses %0.2f\n ", aux->conteudo.AmplitudeClasse);
        P10 = LiP10 + ( ( (PosElemP10 - FacAnt) / FiAtual) * f->inicia->conteudo.AmplitudeClasse);
        //printf ("\n Percentil 10:  %0.2f\n ", P10);
        //printf ("\n -----------------------------------------\n");
    }
    return P10;
}
/*-----------------------------FINAL DA FUNÇÃO CURTOSE (PERCENTIL 10)-------------------------------*/


/*----------------------------INÍCIO DA FUNÇÃO CURTOSE (PERCENTIL 90)-------------------------------*/
float Percentil90 (TAB *t, DADOS *d, DADOSTABELA *f) {
    NoTab *atual;
    NoDadosTabela *aux;
    NoDados *usa;
    atual= t->inicio;
    aux = f->inicia;
    usa = d->comeco;
    int cont = 0, PosClasseP90 = 0, PosElemP90 = 0;
    float P90 = 0, LiP90 = 0, FiAtual = 0, FacAnt = 0;
    PosElemP90 = (90 * usa->dados.somaFi) /100;

    if(atual == NULL) {
        printf("\n Erro!\n\n\n");
    }
    else {
        while (aux != NULL ) {
            if(aux->conteudo.Fac >= PosElemP90) {
                PosClasseP90 = cont;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        cont = 0;
        while (atual != NULL ) {
            if(cont == PosClasseP90) {
                LiP90 = atual->DadosTab.limInf;
                FiAtual = atual->DadosTab.frequencia;
                break;
            }
            atual = atual->prox;
            cont++;
        }
        cont = 0;
        aux = f->inicia;
        while (aux != NULL ) {
            if(cont == PosClasseP90-1) {
                FacAnt = aux->conteudo.Fac;
                break;
            }
            aux = aux->seguinte;
            cont++;
        }
        P90 = LiP90 + ( ( (PosElemP90 - FacAnt) / FiAtual) * f->inicia->conteudo.AmplitudeClasse);
    }
    return P90;
}
/*----------------------------FINAL DA FUNÇÃO CURTOSE (PERCENTIL 90)-------------------------------*/


/*-------------------------------------INÍCIO DA FUNÇÃO CURTOSE-------------------------------------*/
float Curtose (TAB *t, DADOS *d, DADOSTABELA *f) {
    float K = 0, Q1 = 0, Q3 = 0, P10 = 0, P90 = 0;
    Q1 = Quartil1 (t, d, f);
    Q3 = Quartil3 (t, d, f);
    P10 = Percentil10 (t, d, f);
    P90 = Percentil90 (t, d, f);

    K = (Q3 - Q1) / ( 2 * (P90 - P10) );
    return K;
}
/*-------------------------------------FINAL DA FUNÇÃO CURTOSE-------------------------------------*/


/*------------------------------INÍCIO DA FUNÇÃO VARIÂNCIA AMOSTRAL--------------------------------*/
float VarianciaAmostral (DADOS *d) {
    NoDados *aux ;
    aux = d->comeco;
    float S2 = 0;
    S2 = ((aux->dados.somaXi2Fi) - ((aux->dados.somaXiFi * aux->dados.somaXiFi) / aux->dados.somaFi)) / ( aux->dados.somaFi - 1);
    return S2;
}
/*------------------------------FINAL DA FUNÇÃO VARIÂNCIA AMOSTRAL--------------------------------*/


/*-----------------------------INÍCIO DA FUNÇÃO DESVIO PADRÃO AMOSTRAL----------------------------*/
float DesvioPadraoAmostral(DADOS *d) {
    float S = 0, S2 = 0;
    S2 = VarianciaAmostral(d);
    S = sqrt (S2);
    return S;
}
/*-----------------------------FINAL DA FUNÇÃO DESVIO PADRÃO AMOSTRAL----------------------------*/


/*-----------------------INÍCIO DA FUNÇÃO COEFICIENTE DE VARIAÇÃO AMOSTRAL-----------------------*/
float CoefVarAmostral (DADOS *d) {
    NoDados *aux ;
    aux = d->comeco;
    float CV = 0, S = 0;
    S = DesvioPadraoAmostral(d);
    CV = (S / aux->dados.media) * 100;
    return CV;
}
/*-----------------------FINAL DA FUNÇÃO COEFICIENTE DE VARIAÇÃO AMOSTRAL-----------------------*/


/*--------------------INÍCIO DA FUNÇÃO COEFICIENTE DE ASSIMETRIA AMOSTRAL-----------------------*/
float CoeficienteDeAssimetriaAmostral (TAB *t, DADOS *d, DADOSTABELA *f) {
    NoDados *aux ;
    aux = d->comeco;
    float AS = 0, MD = 0, S = 0;
    MD = Moda (t, f);
    S = DesvioPadraoAmostral(d);
    AS = (aux->dados.media - MD) / S;
    if (flag == 1)
    	AS = 0;
    return AS;
}
/*-----------------------FINAL DA FUNÇÃO COEFICIENTE DE ASSIMETRIA AMOSTRAL---------------------*/

/*-----Arquivo------*/
/*-----------------------------INÍCIO DA FUNÇÃO ABRE O ARQUIVO----------------------------------*/
void AbreArquivoRol(char nome[],TabRol *r,TAB *t){

 NoTabRol *aux1,*aux2 ;
 FILE *arquivo;
 float c,aux,amplitudeTotal=0,amplitudeClasse=0,LimInferior,LimSuperior;
 DadosTabRol o;
 dadosTab z;
 int x=0,y,tam=0,w=0,qtdDados=0,freq=0,SomaFreq=0,SomaFrequenciaClasse=0;

    strcat(nome,".txt");
    arquivo = fopen(nome,"r");
    if(arquivo != NULL){
        while( (fscanf(arquivo," %f \n",&c))!=EOF ){
            tam++;
        }
        fclose(arquivo);
        float vetor[tam];
        arquivo = fopen(nome,"r");
        if(arquivo != NULL) {
          printf("\n\n Arquivo %s foi aberto com sucesso!\n\n", nome);
          while((fscanf(arquivo," %f \n",&c))!=EOF ){
                vetor[w]=c;
                w++;
          }
          fclose(arquivo);
          printf("\n\n --> Dados Brutos: \n\n");
           w=0;
           while(w<tam){
                printf(" %0.0f  ",vetor[w]);
                w++;
            }
            printf("\n\n Tamanho: %d\n\n",tam);
            for(x=0;x<tam;x++){
                for(y=x+1;y<tam;y++){
                    if(vetor[x]>vetor[y]){
                        aux=vetor[x];
                        vetor[x]=vetor[y];
                        vetor[y]=aux;
                    }
                }
            }
            //Mostrar Rol
            printf("\n\n --> Rol: \n\n");
            for(x=0;x<tam;x++){
               printf(" %0.0f  ",vetor[x]);
            }
            printf("\n\n");
            //Contar frequencia da cada elemento
            freq = 1;
            for(x=0;x<tam;x++){
                if(vetor[x] == vetor[x+1])
                    freq++;
                else{
                    o.XiRol=vetor[x];
                    o.FiRol=freq;
                    SomaFreq=SomaFreq+freq;
                    InsereTabRol(r,o);
                    freq=1;
                }
            }

            r->SomaFrequencia=SomaFreq;
            ImprimeTabelaRol(r);
            int qtdClasses=0;
            if(r->SomaFrequencia<=25){
                qtdClasses=5;    
            }
            else{
                if(r->SomaFrequencia>25){
                    qtdClasses= sqrt(r->SomaFrequencia);
                }
            }
            printf("\n Quantidade de Classes: %d",qtdClasses);
            amplitudeTotal = r->Fim->dadosRol.XiRol - r->Inicio->dadosRol.XiRol;
            amplitudeClasse=amplitudeTotal/qtdClasses;
            amplitudeClasse=ceil(amplitudeClasse);
            aux1 = r->Inicio;
            aux2= r->Inicio;
            LimInferior=aux1->dadosRol.XiRol;
            while(aux1!=NULL && LimSuperior<=r->Fim->dadosRol.XiRol){
                LimSuperior=LimInferior+amplitudeClasse;
                z.limInf=LimInferior;
                z.limSup=LimSuperior;
                while(aux2!=NULL && aux2->dadosRol.XiRol>=LimInferior && aux2->dadosRol.XiRol<LimSuperior){
                        SomaFrequenciaClasse=SomaFrequenciaClasse+aux2->dadosRol.FiRol;
                    aux2=aux2->prox;
                }
                z.frequencia=SomaFrequenciaClasse;
                InsereTab (t,z);
                SomaFrequenciaClasse=0;
                LimInferior=LimSuperior;
                aux1=aux1->prox;
            }
        }
        else
           printf("\n\n Arquivo %s nao encontrado!", nome);
        printf("\n\n");
    }
     else
        printf("\n\n Arquivo %s nao encontrado!", nome);
      printf("\n\n");

}
/*-------------------------------FINAL DA FUNÇÃO ABRE O ARQUIVO----------------------------------*/

/*-----------------------------------INÍCIO DA FUNÇÃO MENU---------------------------------------*/
void MENU() {
    printf("\n\t -----------------------------\n");
    printf("\t         MENU PRINCIPAL");
    printf("\n\t -----------------------------\n");
    printf("\t < 1 > Informar Dados \n");
    printf("\t < 2 > Mostrar dados inseridos \n");
    printf("\t < 3 > Tabela de Resultados \n");
    printf("\t < 4 > Resultados Geral \n");
    printf("\t < 5 > Resultados das Formulas\n");
    printf("\t < 0 > Sair \n");
    printf("\t -----------------------------\n");
}
/*-----------------------------------FINAL DA FUNÇÃO MENU---------------------------------------*/


/*-----------------------------------INÍCIO DA FUNÇÃO FORMULAS ---------------------------------*/
void FORMULAS (TAB *t, DADOS *d, DADOSTABELA *v) {
    int z, w;
    printf("\n\n --------------------------------\n");
    printf("\t  MENU SECUNDARIO ");
    printf("\n --------------------------------\n");
    printf(" < 1 > Geral\n");
    printf(" < 2 > Individual\n");
    printf("\n --------------------------------\n");
    printf(" Mostrar: ");
    scanf(" %d",&z);
    if (z == 1) {
        printf("\n\n ----------------------------------------------------------\n");
        printf("\t           RESULTADO DAS FORMULAS\n");
        printf(" ----------------------------------------------------------\n");
        printf("            DESCRICAO                     |  RESULTADO\n");
        printf(" ----------------------------------------------------------");
        printf ("\n   Desvio Medio                           |   %0.2f\n", DesvioMedio (d));
        printf ("\n   Variancia Populacional                 |   %0.2f\n", VarianciaPopulacional (d));
        printf ("\n   Desvio Padrao Populacional             |   %0.2f\n", DesvioPadraoPopulacional (d));
        printf ("\n   Coeficiente de Variacao Populacional   |   %2.0f %%\n", CoefVarPopulacional (d));
        printf ("\n   Moda                                   |   %0.2f", Moda (t, v));		VerificaExcecao (1);
        printf ("\n   Coeficiente de Assimetria Populacional |   %0.2f", CoeficienteDeAssimetriaPopulacional (t, d, v));		VerificaExcecao (2);
        printf ("\n   Quartil 1                              |   %0.2f\n", Quartil1 (t, d, v));
        printf ("\n   Quartil 3                              |   %0.2f\n", Quartil3 (t, d, v));
        printf ("\n   Percentil 10                           |   %0.2f\n", Percentil10 (t, d, v));
        printf ("\n   Percentil 90                           |   %0.2f\n", Percentil90 (t, d, v));
        printf ("\n   Curtose                                |   %0.2f\n", Curtose (t,d,v));
        printf ("\n   Variancia Amostral                     |   %0.2f\n", VarianciaAmostral (d));
        printf ("\n   Desvio Padrao Amostral                 |   %0.2f\n", DesvioPadraoAmostral(d));
        printf ("\n   Coeficiente de Variacao Amostral       |   %0.2f %%\n", CoefVarAmostral (d));
        printf ("\n   Coeficiente de Assimetria Amostral     |   %0.2f", CoeficienteDeAssimetriaAmostral (t, d, v));		VerificaExcecao (2);
        printf (" ----------------------------------------------------------\n\n\n");
    }
    else {
        if (z == 2) {
        	printf("\n\n -------------------------------------------------\n");
	        printf("\t      RESULTADO DAS FORMULAS\n");
	        printf(" -------------------------------------------------\n");
	        printf(" < 1  > Desvio Medio\n");
	        printf(" < 2  > Variancia Populacional\n");
	        printf(" < 3  > Desvio Padrao Populacional \n");
	        printf(" < 4  > Coeficiente de Variacao Populacional \n");
	        printf(" < 5  > Moda \n");
	        printf(" < 6  > Coeficiente de Assimetria Populacional \n");
	        printf(" < 7  > Quartil 1 \n");
	        printf(" < 8  > Quartil 2 \n");
	        printf(" < 9  > Percentil 10 \n");
	        printf(" < 10 > Percentil 90 \n");
	        printf(" < 11 > Curtose \n");
	        printf(" < 12 > Variancia Amostral \n");
	        printf(" < 13 > Desvio Padrao Amostral \n");
	        printf(" < 14 > Coeficiente de Variacao Amostral \n");
	        printf(" < 15 > Coeficiente de Assimetria Amostral \n");
	        printf(" -------------------------------------------------\n");
	        printf(" Mostrar formula: ");
	        scanf(" %d",&w);
	        switch (w) {
	            case 1:
	                printf ("\n\n\tDesvio Medio ----------------------------------  | %0.2f\n\n\n", DesvioMedio (d));
	                break;
	            case 2:
	                printf ("\n\n\tVariancia Populacional ------------------------  | %0.2f\n\n\n", VarianciaPopulacional (d));
	                break;
	            case 3:
	                printf ("\n\n\tDesvio Padrao Populacional --------------------  | %0.2f\n\n\n", DesvioPadraoPopulacional (d));
	                break;
	            case 4:
	                printf ("\n\n\tCoeficiente de Variacao Populacional ----------  | %2.0f Porcento\n\n\n", CoefVarPopulacional (d));
	                break;
	            case 5:
	                printf ("\n\n\tModa ------------------------------------------  | %0.2f", Moda (t, v));
	                VerificaExcecao (1);  printf ("\n\n");
	                break;
	            case 6:
	                printf ("\n\n\tCoeficiente de Assimetria Populacional --------  | %0.2f", CoeficienteDeAssimetriaPopulacional (t, d, v));
	                VerificaExcecao (2);  printf ("\n\n");
	                break;
	            case 7:
	                printf ("\n\n\tQuartil 1 -------------------------------------  | %0.2f\n\n\n", Quartil1 (t, d, v));
	                break;
	            case 8:
	                printf ("\n\n\tQuartil 3 -------------------------------------  | %0.2f\n\n\n", Quartil3 (t, d, v));
	                break;
	            case 9:
	                printf ("\n\n\tPercentil 10 ----------------------------------  | %0.2f\n\n\n", Percentil10 (t, d, v));
	                break;
	            case 10:
	                printf ("\n\n\tPercentil 90 ----------------------------------  | %0.2f\n\n\n", Percentil90 (t, d, v));
	                break;
	            case 11:
	                printf ("\n\n\tCurtose ---------------------------------------  | %0.2f\n\n\n", Curtose (t,d,v));
	                break;
	            case 12:
	                printf ("\n\n\tVariancia Amostral ----------------------------  | %0.2f\n\n", VarianciaAmostral (d));
	                break;
	            case 13:
	                printf ("\n\n\tDesvio Padrao Amostral ------------------------  | %0.2f\n\n", DesvioPadraoAmostral(d));
	                break;
	            case 14:
	                printf ("\n\n\tCoeficiente de Variacao Amostral --------------  | %0.2f Porcento\n\n", CoefVarAmostral (d));
	                break;
	            case 15:
	                printf ("\n\n\tCoeficiente de Assimetria Amostral ------------  | %0.2f", CoeficienteDeAssimetriaAmostral (t, d, v));
	                VerificaExcecao (2);  printf ("\n\n");
	                break;
	            default:
	            	while(getchar()!='\n'){};
	                printf("\n Opcao Inexistente!\n\n");
	        }
	    } else {
	    	while(getchar()!='\n'){};
	    	printf("\n\n Opcao Inexistente!\n\n\n");
	    }
    }
}
/*------------------------------------FINAL DA FUNÇÃO FORMULAS ---------------------------------*/


/*-----------------------------INÍCIO DA FUNÇÃO PRINCIPAL MAIN----------------------------------*/
int main() {
    TabRol r;
    TAB t;
    DADOS d;
    DADOSTABELA v;
    dadosTab e;
    char nome[15], acUserName[100];

    FILE *arquivo;
    float c1,c2,c3;
    int op;
    int x;

    Inicia(&t);
    IniciaDados(&d);
    IniciaDadosTabela(&v);
    IniciaTabRol(&r);

    DWORD nUserName = sizeof(acUserName); //obtêm nome usuário
    if (GetUserName(acUserName, &nUserName)) {
        printf("\n\n\tOla %s, seja bem vindo(a)!\n\n",acUserName);
    }
    
    do {
        MENU();
        printf(" Opcao: ");
        setbuf(stdin, NULL);
        scanf(" %d", &op);

        switch(op) {
            case 1:
                printf("\n\n ---------------------------------------------\n");
                printf("\t\t   INSERIR");
                printf("\n ---------------------------------------------\n");
                printf(" < 1 > Limite das classes e frequencia\n");
                printf(" < 2 > Rol\n");
                printf("\n ---------------------------------------------\n");
                printf(" Inserir: ");
                scanf(" %d",&x);
                if (t.inicio!=NULL) {
                    TabelaDestrutor(&t);
                    ResultadosGeralDestrutor(&d);
                    TabelaResultadosDestrutor(&v);
                    TabelaRolDestrutor(&r);
                    posiMaiorFi = 0, flag = 0;
                }

                if(x==1){
                	printf("\n\n Nome do arquivo (formato txt): ");
                	scanf("%s",nome);
                    strcat(nome,".txt");
                    arquivo = fopen(nome,"r");
                    if(arquivo != NULL) {
                        printf("\n\n Arquivo %s foi aberto com sucesso!\n\n", nome);
                        while( (fscanf(arquivo,"%f %f %f\n",&c1,&c2,&c3))!=EOF ){
                            e.limInf=c1;
                            e.limSup=c2;
                            e.frequencia=c3;
                            InsereTab(&t,e);
                        }
                        DefineDados(&t,&d,&v);
                        fclose(arquivo);
                    }
                    else {
                       printf("\n\n Arquivo %s nao encontrado!", nome);
                   }
                       printf("\n\n\n");
                }
                else{
                    if(x==2){
                    	printf("\n\n Nome do arquivo (formato txt): ");
                		scanf("%s",nome);
                        AbreArquivoRol(nome,&r,&t);
                        DefineDados(&t, &d, &v);
                    } else {
                    	while(getchar()!='\n'){};
				    	printf("\n\n Opcao Inexistente!\n\n\n");
				    }
                }
                break;
            case 2:
                ImprimeDadosInseridos(&t);
                break;
            case 3:
                ImprimeTabelaDeResultados (&t, &d, &v);
                break;
            case 4:
                ImprimeResultadoGeral(&d);
                break;
            case 5:
                if(t.inicio!=NULL)
                    FORMULAS (&t, &d, &v);
                else
                    printf("\n\n\t Dados nao foram inseridos!\n\n\n");
                break;
            case 0:
                printf("");
                DWORD nUserName = sizeof(acUserName);
                if (GetUserName(acUserName, &nUserName)) {
                    printf("\n\n\t%s, obrigado por utilizar nosso programa!\n\n\n",acUserName);
                }
                exit(0);
                break;
            default:
                while(getchar()!='\n'){};
                printf("\n\n Opcao Inexistente. Digite uma valida!\n\n\n");
                break;
        }
        system("pause");
        system("cls");
    } while(op != 0);
    
    return 0;
}

/*-----------------------------FINAL DA FUNÇÃO PRINCIPAL MAIN----------------------------------*/
