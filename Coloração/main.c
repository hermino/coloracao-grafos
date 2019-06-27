#include"disciplinas.h"

int main(int argc, char const *argv[]) {
  char urldisc[20] = "disciplinas.csv";
  char urlsalas[20] = "salas.csv";
  char cores[9][3] = {"z","r","g","b","y","R","G","B","Y"};
  Disciplinas *disciplinas = inicializaDisciplinas();
  Salas *salas = inicializaSalas(urlsalas);

  lerDisciplinas(disciplinas, urldisc);
  coloreGrafo(disciplinas);
  int i,j,k;
  // O(9*nDisciplinas*nSalas)
  for(i=1 ; i<9 ; i++){
    for(j=0; j<disciplinas->quantidade;j++){
      if(disciplinas->listadisciplinas[j]->horario == i){
        printf("HORARIO: %s | DISCIPLINA: %s | ", cores[i], disciplinas->listadisciplinas[j]->codigo);
        for(k=0; k<salas->quantidade; k++){
          if(disciplinas->listadisciplinas[j]->qtd_alunos == salas->listasalas[k]->capacidade){
            if(disciplinas->listadisciplinas[j]->recursos[0] == salas->listasalas[k]->recursos[0]
               &&
               disciplinas->listadisciplinas[j]->recursos[1] == salas->listasalas[k]->recursos[1]
               &&
               disciplinas->listadisciplinas[j]->recursos[2] == salas->listasalas[k]->recursos[2]){
                printf("SALA: %s\n", salas->listasalas[k]->codigo);
              }
          }
        }
      }
    }
  }
  printf("oi\n");
  liberaEspaco(disciplinas, salas);
  return 0;
}
