#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Disciplina{
  char codigo[7];
  char nome[60];
  int cargahoraria;
  char prerequisitos[18];
  char docente[30];
  int turma;
  int qtd_alunos;
  int recursos[3];
  int nConexoes;
  int horario;
  struct Disciplina **conexoes;
} Disciplina;

typedef struct Disciplinas{
  Disciplina **listadisciplinas;
  int quantidade;
} Disciplinas;

typedef struct Sala{
  char codigo[5];
  int capacidade;
  int recursos[3];
} Sala;

typedef struct Salas{
  Sala **listasalas;
  int quantidade;
} Salas;

Disciplinas *inicializaDisciplinas();
Salas *inicializaSalas(char *urlsalas);
int lerDisciplinas(Disciplinas *disciplinas, char *url);
int contaArquivo(char *url);
Disciplina *alocaDisciplina();
Sala *alocaSala();
void criaConexao(Disciplinas *disciplinas, Disciplina *disciplina);
int verifConexao(Disciplina *disciplina1, Disciplina *disciplina2);
void coloreGrafo(Disciplinas *disciplinas);
int verifCoresAdj(Disciplina *disciplina, int *ncores);
void liberaEspaco(Disciplinas *disciplinas, Salas *salas);

// O(1)
Disciplinas *inicializaDisciplinas(){
  Disciplinas *disciplinas = (Disciplinas *) malloc(sizeof(Disciplinas));
  disciplinas->quantidade = 0;
  return disciplinas;
}
// O(2n) n=tamArquivo
Salas *inicializaSalas(char *urlsalas){
  FILE *arquivo;
  int quantidade;
  Salas *salas;
  Sala *sala;
  arquivo = fopen(urlsalas, "r");
  if(arquivo == NULL){
  	printf("Erro, nao foi possivel abrir o arquivo\n");
      return 0;
  }else{
    quantidade = contaArquivo(urlsalas);
    salas = (Salas *) malloc(sizeof(Salas));
    salas->quantidade = 0;
    salas->listasalas = (Sala **) malloc(quantidade*(sizeof(Sala)));

    while(!feof(arquivo)){
      sala = alocaSala();
      fscanf(arquivo,"%[^,]s", sala->codigo);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &sala->capacidade);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &sala->recursos[0]);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &sala->recursos[1]);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d\n", &sala->recursos[2]);

      salas->listasalas[salas->quantidade] = sala;
      salas->quantidade += 1;
    }
  }
  return salas;
}
// O(1)
Disciplina *alocaDisciplina(){
  Disciplina *disciplina = (Disciplina *) malloc(sizeof(Disciplina));
  return disciplina;
}
// O(1)
Sala *alocaSala(){
  Sala *sala = (Sala *) malloc(sizeof(Sala));
  return sala;
}
// O(n) n=tamArquivo
int contaArquivo(char *url){
  FILE *arquivo;
  arquivo = fopen(url, "r");
  char linha[500];
  int qtd = 0;
  if(arquivo == NULL){
    printf("Erro, nao foi possivel abrir o arquivo\n");
  }else{
    while( (fscanf(arquivo,"%[^\n]", linha))!=EOF ){
      fseek(arquivo, +2, SEEK_CUR);
      qtd++;
    }
  }
 fclose(arquivo);
 return qtd;
}
// O(2n+m) n=tamArquivo m=qtd_disciplinas
int lerDisciplinas(Disciplinas *disciplinas, char *url){
  int qtddisciplinas = contaArquivo(url);
  disciplinas->listadisciplinas = (Disciplina **) malloc(qtddisciplinas*(sizeof(Disciplina)));

  FILE *arquivo;
  Disciplina *disciplina;
  arquivo = fopen(url, "r");
  fflush(stdin);

  if(arquivo == NULL){
  	printf("Erro, nao foi possivel abrir o arquivo\n");
      return 0;
  }else{
  	while(!feof(arquivo)){
      disciplina = alocaDisciplina();
      fscanf(arquivo,"%[^,]s", disciplina->codigo);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%[^,]s", disciplina->nome);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &disciplina->cargahoraria);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%[^,]s", disciplina->prerequisitos);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%[^,]s", disciplina->docente);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &disciplina->turma);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &disciplina->qtd_alunos);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &disciplina->recursos[0]);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d", &disciplina->recursos[1]);
      fseek(arquivo, +1, SEEK_CUR);
      fscanf(arquivo,"%d\n", &disciplina->recursos[2]);

      disciplina->horario = 0;
      disciplina->nConexoes = 0;
      disciplina->conexoes = (Disciplina **) malloc(qtddisciplinas*(sizeof(Disciplina)));

      //AJUSTA ALUNOS
      if(disciplina->qtd_alunos > 30){
        disciplina->qtd_alunos = 60;
      }else if(disciplina->qtd_alunos > 20){
        disciplina->qtd_alunos = 30;
      }else{
        if(disciplina->recursos[1] == 1){
          disciplina->qtd_alunos = 30;
        }else{
          disciplina->qtd_alunos = 20;
        }
      }
      criaConexao(disciplinas, disciplina);

      disciplinas->listadisciplinas[disciplinas->quantidade] = disciplina;
      disciplinas->quantidade += 1;
    }
  }
  fclose(arquivo);
  return 1;
}
// O(n) n=qtd_disciplinas
void criaConexao(Disciplinas *disciplinas, Disciplina *disciplina){
  Disciplina *aux;
  int token;

  for(token = 0; token < disciplinas->quantidade; token++){
    aux = disciplinas->listadisciplinas[token];
    if(verifConexao(disciplina, aux)){
      disciplina->conexoes[disciplina->nConexoes] = aux;
      disciplina->nConexoes += 1;

      aux->conexoes[aux->nConexoes] = disciplina;
      aux->nConexoes += 1;
    }
  }
}
// O(1)
int verifConexao(Disciplina *disciplina1, Disciplina *disciplina2){
  if(!strcmp(disciplina1->docente, disciplina2->docente) || disciplina1->turma == disciplina2->turma){
    return 1;
  }else{
    if(disciplina1->qtd_alunos == disciplina2->qtd_alunos
       &&
       disciplina1->recursos[0] == disciplina2->recursos[0]
       &&
       disciplina1->recursos[1] == disciplina2->recursos[1]
       &&
       disciplina1->recursos[2] == disciplina2->recursos[2]){
      return 1;
    }
  }
  return 0;
}
// O(n^2+Gi+Gj) n=qtd_disciplinas Gi,Gj = Grau do nó i e j
void coloreGrafo(Disciplinas *disciplinas){
  Disciplina *disciplina, *aux;
  int token, i, ncoresatual, ncoresaux, cor;
  for(token = 0; token < disciplinas->quantidade; token++){
    disciplina = disciplinas->listadisciplinas[token];

    // ------ SAIO DESTE FOR COM O PRÓXIMO A SER COLORIDO
    for(i=0; i < disciplinas->quantidade; i++){
      aux = disciplinas->listadisciplinas[i];
      if(aux->horario != 0 || disciplina == aux){
        continue;
      }
      if(disciplina->horario != 0 && aux->horario == 0){
        disciplina = aux;
      }
      if(disciplina->nConexoes < aux->nConexoes){
        disciplina = aux;
      }else if(disciplina->nConexoes == aux->nConexoes){
        verifCoresAdj(disciplina, &ncoresatual);
        verifCoresAdj(aux, &ncoresaux);
        if(ncoresatual < ncoresaux){
          disciplina = aux;
        }
      }
    }
    // ------- FIM DA SELEÇÃO
    disciplina->horario = verifCoresAdj(disciplina, &ncoresatual);
  }
}
// O(Gn) Gn=grau do nó
int verifCoresAdj(Disciplina *disciplina, int *ncores){
  int token, cor = 9, cores[9] = {0,0,0,0,0,0,0,0,0};
  *ncores = 0;
  for(token = 0; token < disciplina->nConexoes; token++){
    if(disciplina->conexoes[token]->horario != 0){
      *ncores += 1;
      cores[disciplina->conexoes[token]->horario] = 1;
    }
  }
  for(token = 1; token < 9; token++){
    if(cores[token] == 0 && token < cor){
      cor = token;
    }
  }
  if(cor > 8){
    //PARA O PROGRAMA INTEIRO
  }
  return cor;
}
// O(n+m) n=qtd_disciplinas m=qtd_salas
void liberaEspaco(Disciplinas *disciplinas, Salas *salas){
  Disciplina *auxdisc;
  int token;

  for(token = 0; token < disciplinas->quantidade; token++){
    auxdisc = disciplinas->listadisciplinas[token];
    free(auxdisc->conexoes);
    free(auxdisc);
  }
  free(disciplinas->listadisciplinas);
  free(disciplinas);

  for(token = 0; token < salas->quantidade; token++){
    free(salas->listasalas[token]);
  }
  free(salas->listasalas);
  free(salas);
}
