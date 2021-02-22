#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define C 6
#define N 100

//albero per entità
struct nodo {
    char *nome;
    struct nodo *sinistra;
    struct nodo *destra;
};
struct nodo *first = NULL;

//liste per relazioni
struct nodorel {
    char *rel;
    struct nodocoppia *lista;
    struct nodorel *nextrel;
};
struct nodorel *firstrel = NULL;
struct nodocoppia {
    char *orig;
    char *dest;
    struct nodocoppia *nextcoppia;
};

//lista per report
struct nodorep {
    char *ent;
    int conto;
    struct nodorep *nextrep;
};

void report();
void addent(char *ent);
void delent(char *ent);
void addrel(char *ent1, char *ent2, char *relazione);
void delrel(char *ent1, char *ent2, char *relazione);
//funzione chiamata da delent per cancellare tutte le relazioni che contengono l'entità eliminata
void trova(char *ent); 

int main () {
    char comando[C];
    char nome1[N];
    char nome2[N];
    char relazione[N];
    while (1) {
        scanf("%s", comando);
        if(strcmp(comando, "end") == 0) {
            return 0;
        }
        if(strcmp(comando, "report") == 0) {
            report();
        }
        if(strcmp(comando, "addent") == 0) {
            scanf("%s", nome1);
            addent(nome1);
        }
        if(strcmp(comando, "delent") == 0) {
            scanf("%s", nome1);
            delent(nome1);
        }
        if(strcmp(comando, "addrel") == 0) {
            scanf("%s", nome1);
            scanf("%s", nome2);
            scanf("%s", relazione);
            addrel(nome1, nome2, relazione);
        }
        if(strcmp(comando, "delrel") == 0) {
            scanf("%s", nome1);
            scanf("%s", nome2);
            scanf("%s", relazione);
            delrel(nome1, nome2, relazione);
        }
    }
    return 0;
}

void report() {
    struct nodorel *sondarel;
    sondarel = firstrel;
    //se la lista è vuota
    if(sondarel == NULL) {
        fputs("none\n", stdout);
        return;
    }
    while(sondarel != NULL) {
        fputs(sondarel->rel, stdout);
        fputs(" ", stdout);
        struct nodocoppia *sondacoppia;
        sondacoppia = sondarel->lista;
        //comincio a scrivere nella lista della report
        struct nodorep *firstrep;
        struct nodorep *newrep;
        newrep = (struct nodorep*)malloc(sizeof(struct nodorep));
        newrep->ent = (char*)malloc(sizeof(char)*strlen(sondacoppia->dest) + 1);
        strcpy(newrep->ent, sondacoppia->dest);
        newrep->conto = 1;
        newrep->nextrep = NULL;
        firstrep = newrep;
        struct nodorep *prec;
        prec = newrep;
        sondacoppia = sondacoppia->nextcoppia;
        int x = 1;
        while(sondacoppia != NULL) {
            if(strcmp(prec->ent, sondacoppia->dest) == 0) {
                int a = prec->conto;
                prec->conto = a + 1;
                if(prec->conto > x) {
                    x = prec->conto;
                }
                sondacoppia = sondacoppia->nextcoppia;
            } else {
                struct nodorep* newrep;
                newrep = (struct nodorep*)malloc(sizeof(struct nodorep));
                newrep->ent = (char*)malloc(sizeof(char)*strlen(sondacoppia->dest) + 1);
                strcpy(newrep->ent, sondacoppia->dest);
                newrep->conto = 1;
                prec->nextrep = newrep;
                newrep->nextrep = NULL;
                prec = newrep;
                sondacoppia = sondacoppia->nextcoppia;
            }
        }
        //tutte le entità che hanno come conto il numero pari a x vengono stampate
        struct nodorep *sonda;
        sonda = firstrep;
        while(firstrep != NULL) {
            if(firstrep->conto == x) {
                fputs(firstrep->ent, stdout);
                fputs(" ", stdout);
            }
            sonda->nextrep = firstrep->nextrep;
            free(firstrep->ent);
            free(firstrep);
            firstrep = sonda->nextrep;
        }
        printf("%d", x);
        fputs("; ", stdout);
        sondarel = sondarel->nextrel;
    }
    fputs("\n", stdout);
    return;
}

void addent(char *ent) {
    struct nodo *sonda1;
    sonda1 = first;
    //se l'albero è vuoto inserisco in cima
    if(first == NULL) {
        struct nodo *new;
        new = (struct nodo*)malloc(sizeof(struct nodo));
        new->nome = (char*)malloc(sizeof(char)*strlen(ent) + 1);
        strcpy(new->nome, ent);
        new->sinistra = NULL;
        new->destra = NULL;
        first = new;
        return;
    }
    //se l'entità da inserire è minore di quella del nodo, vado a sinistra; se è maggiore, a destra
    if(strcmp(ent, sonda1->nome) < 0) {
        sonda1 = sonda1->sinistra;
    } else if(strcmp(ent, sonda1->nome) > 0) {
        sonda1 = sonda1->destra;
    }
    struct nodo *sonda2;
    sonda2 = first;
    while(sonda1 != NULL) {
        //se l'entità è già monitorata, concludo
        if(strcmp(ent, sonda1->nome) == 0) {
            return;
        }
        //se l'entità da inserire è minore di quella del nodo, vado a sinistra; se è maggiore, a destra
        if(strcmp(ent, sonda1->nome) < 0) {
            sonda1 = sonda1->sinistra;
        } else if(strcmp(ent, sonda1->nome) > 0) {
            sonda1 = sonda1->destra;
        }
        if(strcmp(ent, sonda2->nome)<0) {
            sonda2 = sonda2->sinistra;
        } else if(strcmp(ent, sonda2->nome) > 0) {
            sonda2 = sonda2->destra;
        }
    }
    //se l'entità non è presente, va inserita nella posizione in cui si trova sonda
    struct nodo *new;
    new = (struct nodo*)malloc(sizeof(struct nodo));
    new->nome = (char*)malloc(sizeof(char)*strlen(ent) + 1);
    strcpy(new->nome, ent);
    new->sinistra = NULL;
    new->destra = NULL;
    //sonda2 è l'ultimo nodo non NULL; quindi in base al valore di ent, la nuova entità è a sinistra o a destra
    if(strcmp(ent, sonda2->nome) < 0) {
        sonda2->sinistra = new;
    } else if(strcmp(ent, sonda2->nome) > 0) {
        sonda2->destra = new;
    }
    return;
}

void delent(char *ent) {
    //cerco l'entità da cancellare
    struct nodo *sonda1;
    sonda1 = first;
    //se l'albero delle entità è vuoto, termino
    if(sonda1 == NULL) {
        return;
    }
    //se l'entità da cancellare è in testa
    if(strcmp(ent, sonda1->nome) == 0) {
        //se il nodo non ha nodi successori nè a destra nè a sinistra
        if(sonda1->sinistra == NULL && sonda1->destra == NULL) {
            first = NULL;
            free(sonda1->nome);
            free(sonda1);
            trova(ent);
            return;
        }
        //se il nodo ha un solo succesore (destro o sinistro)
        if(sonda1->sinistra == NULL) {
            first = sonda1->destra;
            free(sonda1->nome);
            free(sonda1);
            trova(ent);
            return;
        }
        if(sonda1->destra == NULL) {
            first = sonda1->sinistra;
            free(sonda1->nome);
            free(sonda1);
            trova(ent);
            return;
        }
        //se il nodo ha due sottoalberi, cerco il nodo minore del sottoalbero destro
        struct nodo *sonda2;
        sonda2 = sonda1->destra;
        //se sonda2 è una foglia o non ha figli sinistri, inserisco sonda2 al posto di sonda1
        if(sonda2->sinistra == NULL) {
            first = sonda2;
            sonda2->sinistra = sonda1->sinistra;
            free(sonda1->nome);
            free(sonda1);
            trova(ent);
            return;
        }
        //scorro a sinistra
        struct nodo *sonda3;
        sonda3 = sonda2;
        sonda2 = sonda2->sinistra;
        while(sonda2->sinistra != NULL) {
            sonda2 = sonda2->sinistra;
            sonda3 = sonda3->sinistra;
        }
        //se sonda2 è una foglia, inserisco sonda2 al posto di sonda1
        if(sonda2->sinistra == NULL && sonda2->destra == NULL) {
            first = sonda2;
            sonda2->sinistra = sonda1->sinistra;
            sonda2->destra = sonda1->destra;
            sonda3->sinistra = NULL;
            free(sonda1->nome);
            free(sonda1);
            trova(ent);
            return;
        }
        //se sonda2 non ha figli sinistri ma figli destri, sostituisco il figlio sinistro di sonda3 con il destro di sonda2
        if(sonda2->sinistra == NULL) {
            sonda3->sinistra = sonda2->destra;
            first = sonda2;
            sonda2->sinistra = sonda1->sinistra;
            sonda2->destra = sonda1->destra;
            free(sonda1->nome);
            free(sonda1);
            trova(ent);
            return;
        }
    }
    //scorro di un nodo
    if(strcmp(ent, sonda1->nome) < 0) {
        sonda1 = sonda1->sinistra;
    } else if(strcmp(ent, sonda1->nome) > 0) {
        sonda1 = sonda1->destra;
    }
    //se non ci sono altre entità, termino
    if(sonda1 == NULL) {
        return;
    }
    struct nodo *sonda2;
    sonda2 = first;
    while(sonda1 != NULL){
        //se trovo il nodo con l'entità corrispondente
        if(strcmp(ent, sonda1->nome) == 0) {
            //se il nodo non ha nodi successori nè a destra nè a sinistra
            if(sonda1->sinistra == NULL && sonda1->destra == NULL) {
                //se si è andati a sinistra o a destra, il figlio sinistro o destro di sonda2 sarà NULL
                if(strcmp(ent, sonda2->nome) < 0) {
                    sonda2->sinistra = NULL;
                } else if(strcmp(ent, sonda2->nome) > 0) {
                    sonda2->destra = NULL;
                }
                free(sonda1->nome);
                free(sonda1);
                trova(ent);
                return;
            }
            //se il nodo ha un solo succesore (destro o sinistro)
            if(sonda1->sinistra == NULL) {
               //se si è andati a sinistra o a destra, il figlio sinistro o destro di sonda2 sarà l'unico figlio di sonda1
                if(strcmp(ent, sonda2->nome) < 0) {
                    sonda2->sinistra = sonda1->destra;
                } else if(strcmp(ent, sonda2->nome) > 0) {
                    sonda2->destra = sonda1->destra;
                }
                free(sonda1->nome);
                free(sonda1);
                trova(ent);
                return;
            }
            if(sonda1->destra == NULL) {
                if(strcmp(ent, sonda2->nome) < 0) {
                    sonda2->sinistra = sonda1->sinistra;
                } else if(strcmp(ent, sonda2->nome) > 0) {
                    sonda2->destra = sonda1->sinistra;
                }
                free(sonda1->nome);
                free(sonda1);
                trova(ent);
                return;
            }
            //se il nodo ha due sottoalberi, cerco il nodo minore del sottoalbero destro
            struct nodo *sonda3;
            sonda3 = sonda1->destra;
            //se sonda3 è una foglia o non ha figli sinistri, inserisco sonda3 al posto di sonda1
            if(sonda3->sinistra == NULL) {
                if(strcmp(ent, sonda2->nome) < 0) {
                    sonda2->sinistra = sonda3;
                    sonda3->sinistra = sonda1->sinistra;
                } else if(strcmp(ent, sonda2->nome) > 0) {
                    sonda2->destra = sonda3;
                    sonda3->sinistra = sonda1->sinistra;
                }
                free(sonda1->nome);
                free(sonda1);
                trova(ent);
                return;
            }
            //scorro a sinistra
            struct nodo *sonda4;
            sonda4 = sonda3;
            sonda3 = sonda3->sinistra;
            while(sonda3->sinistra != NULL) {
                sonda3 = sonda3->sinistra;
                sonda4 = sonda4->sinistra;
            }
            //se sonda3 è una foglia, inserisco sonda3 al posto di sonda1
            if(sonda3->sinistra == NULL && sonda3->destra == NULL) {
                if(strcmp(ent, sonda2->nome) < 0) {
                    sonda2->sinistra = sonda3;
                } else if(strcmp(ent, sonda2->nome) > 0) {
                    sonda2->destra = sonda3;
                }
                sonda3->sinistra = sonda1->sinistra;
                sonda3->destra = sonda1->destra;
                sonda4->sinistra = NULL;
                free(sonda1->nome);
                free(sonda1);
                trova(ent);
                return;
            }
            //se sonda3 non ha figli sinistri ma figli destri, sostituisco il figlio sinistro di sonda4 con il destro di sonda3
            if(sonda3->sinistra == NULL) {
                sonda4->sinistra = sonda3->destra;
                if(strcmp(ent, sonda2->nome) < 0) {
                    sonda2->sinistra = sonda3;
                } else if(strcmp(ent, sonda2->nome) > 0) {
                    sonda2->destra = sonda3;
                }
                sonda3->sinistra = sonda1->sinistra;
                sonda3->destra = sonda1->destra;
                free(sonda1->nome);
                free(sonda1);
                trova(ent);
                return;
            }
        }
        //se non trovo l'entità scorro nell'albero: sia per sonda1 che per sonda2
        if(strcmp(ent, sonda1->nome) < 0) {
            sonda1 = sonda1->sinistra;
        } else if(strcmp(ent, sonda1->nome) > 0) {
            sonda1 = sonda1->destra;
        }
        if(strcmp(ent, sonda2->nome) < 0) {
            sonda2 = sonda2->sinistra;
        } else if(strcmp(ent, sonda2->nome) > 0) {
            sonda2 = sonda2->destra;
        }
    }
    return;
}


void addrel(char *ent1, char *ent2, char *relazione) {
    //verifico se le entità in questione sono monitorate
    struct nodo *sondav1;
    sondav1 = first;
    while(sondav1 != NULL) {
        if(strcmp(sondav1->nome, ent1) == 0) {//se trovo la prima entità, cerco la seconda
            struct nodo *sondav2;
            sondav2 = first;
            while(sondav2 != NULL) {
                if(strcmp(sondav2->nome, ent2) == 0) {//se anche la seconda entità è monitorata
                    //se la lista delle relazioni è vuota, inserisco la nuova relazione in testa
                    if(firstrel == NULL) {
                        struct nodorel *newrel;
                        struct nodocoppia *newcoppia;
                        newrel = (struct nodorel*)malloc(sizeof(struct nodorel));
                        newrel->rel = (char*)malloc(sizeof(char)*strlen(relazione) + 1);
                        strcpy(newrel->rel, relazione);
                        newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                        newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                        strcpy(newcoppia->orig, ent1);
                        newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                        strcpy(newcoppia->dest, ent2);
                        newrel->lista = newcoppia;
                        newrel->nextrel = firstrel;
                        firstrel = newrel;
                        newcoppia->nextcoppia = NULL;
                        return;
                    }
                    //se il tipo di relazione da inserire è minore di quello di testa, inserisco in testa
                    if(strcmp(relazione, firstrel->rel) < 0) {
                        struct nodorel *newrel;
                        struct nodocoppia *newcoppia;
                        newrel = (struct nodorel*)malloc(sizeof(struct nodorel));
                        newrel->rel = (char*)malloc(sizeof(char)*strlen(relazione) + 1);
                        strcpy(newrel->rel, relazione);
                        newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                        newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                        strcpy(newcoppia->orig, ent1);
                        newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                        strcpy(newcoppia->dest, ent2);
                        newrel->lista = newcoppia;
                        newrel->nextrel = firstrel;
                        firstrel = newrel;
                        newcoppia->nextcoppia = NULL;
                        return;
                    }
                    //cerco se il tipo di relazione è già presente
                    struct nodorel *sondarel1;
                    sondarel1 = firstrel;
                    //se il tipo di relazione da inserire è uguale a quella di testa, verifico se esiste già con stesse etità
                    if(strcmp(sondarel1->rel, relazione) == 0) {
                        struct nodocoppia *sondacoppia1;
                        sondacoppia1 = sondarel1->lista;
                        //se le entità corrispondono con la prima coppia, concludo
                        if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
                            return;
                        }
                        //se non corrisponde con la prima coppia e l'entità dest è minore di quella presente, inserisco in testa
                        if(strcmp(ent2, sondacoppia1->dest) < 0) {
                            struct nodocoppia *newcoppia;
                            newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                            newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                            strcpy(newcoppia->orig, ent1);
                            newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                            strcpy(newcoppia->dest, ent2);
                            sondarel1->lista = newcoppia;
                            newcoppia->nextcoppia = sondacoppia1;
                            return;
                        }
                        //se è maggiore o uguale di dest della prima coppia, scorro
                        sondacoppia1 = sondacoppia1->nextcoppia;
                        struct nodocoppia *sondacoppia2;
                        sondacoppia2 = sondarel1->lista;
                        //se sondacoppia1 è NULL, inserisco in coda
                        if(sondacoppia1 == NULL) {
                            struct nodocoppia *newcoppia;
                            newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                            newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                            strcpy(newcoppia->orig, ent1);
                            newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                            strcpy(newcoppia->dest, ent2);
                            sondacoppia2->nextcoppia = newcoppia;
                            newcoppia->nextcoppia = sondacoppia1;
                            return;
                        }
                        //finchè l'entità di destinazione è maggiore o uguale alla destinazione già presente in lista
                        while(strcmp(ent2, sondacoppia1->dest) >= 0) {
                            //se le entità corrispondono, concludo
                            if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0 ) {
                                return;
                            }
                            sondacoppia1 = sondacoppia1->nextcoppia;
                            sondacoppia2 = sondacoppia2->nextcoppia;
                            //se sondacoppia1 diventa NULL, la coppia da inserire è l'ultima in ordine alfabetico
                            if(sondacoppia1 == NULL) {
                                struct nodocoppia *newcoppia;
                                newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                                newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                                strcpy(newcoppia->orig, ent1);
                                newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                                strcpy(newcoppia->dest, ent2);
                                sondacoppia2->nextcoppia = newcoppia;
                                newcoppia->nextcoppia = NULL;
                                return;
                            }
                        }
                        //se la coppia non è presente, va inserita nella posizione tra sonda2 e sonda1
                        struct nodocoppia *newcoppia;
                        newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                        newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                        strcpy(newcoppia->orig, ent1);
                        newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                        strcpy(newcoppia->dest, ent2);
                        sondacoppia2->nextcoppia = newcoppia;
                        newcoppia->nextcoppia = sondacoppia1;
                        return;
                    }
                    //se il tipo di relazione non è uguale a quello di testa, scorro
                    sondarel1 = sondarel1->nextrel;
                    //se sondarel1 è NULL, la relazione va inserita in coda
                    if(sondarel1 == NULL) {
                        struct nodorel *newrel;
                        struct nodocoppia *newcoppia;
                        newrel = (struct nodorel*)malloc(sizeof(struct nodorel));
                        newrel->rel = (char*)malloc(sizeof(char)*strlen(relazione) + 1);
                        strcpy(newrel->rel, relazione);
                        newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                        newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                        strcpy(newcoppia->orig, ent1);
                        newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                        strcpy(newcoppia->dest, ent2);
                        newrel->lista = newcoppia;
                        firstrel->nextrel = newrel;
                        newrel->nextrel = NULL;
                        newcoppia->nextcoppia = NULL;
                        return;
                    }
                    struct nodorel *sondarel2;
                    sondarel2 = firstrel;
                    //finchè il tipo di relazione è maggiore o uguale alla relazione della lista
                    while(strcmp(relazione, sondarel1->rel) >= 0) {
                        //se le relazioni corrispondono
                        if(strcmp(relazione, sondarel1->rel) == 0) {
                            struct nodocoppia *sondacoppia1;
                            sondacoppia1 = sondarel1->lista;
                            //se le entità corrispondono con la prima coppia, concludo
                            if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
                                return;
                            }
                            //se non corrisponde con la prima coppia e l'entità dest è minore di quella presente, inserisco in testa
                            if(strcmp(ent2, sondacoppia1->dest) < 0) {
                                struct nodocoppia *newcoppia;
                                newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                                newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                                strcpy(newcoppia->orig, ent1);
                                newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                                strcpy(newcoppia->dest, ent2);
                                sondarel1->lista = newcoppia;
                                newcoppia->nextcoppia = sondacoppia1;
                                return;
                            }
                            //se è maggiore o uguale di dest della prima coppia, scorro
                            sondacoppia1 = sondacoppia1->nextcoppia;
                            struct nodocoppia *sondacoppia2;
                            sondacoppia2 = sondarel1->lista;
                            //se sondacoppia1 è NULL, inserisco in coda
                            if(sondacoppia1 == NULL) {
                                struct nodocoppia *newcoppia;
                                newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                                newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                                strcpy(newcoppia->orig, ent1);
                                newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                                strcpy(newcoppia->dest, ent2);
                                sondacoppia2->nextcoppia = newcoppia;
                                newcoppia->nextcoppia = sondacoppia1;
                                return;
                            }
                            //finchè l'entità di destinazione è maggiore o uguale alla destinazione già presente in lista
                            while(strcmp(ent2, sondacoppia1->dest) >= 0) {
                                //se le entità corrispondono, concludo
                                if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
                                    return;
                                }
                                sondacoppia1 = sondacoppia1->nextcoppia;
                                sondacoppia2 = sondacoppia2->nextcoppia;
                                //se sondacoppia1 diventa NULL, la coppia da inserire è l'ultima in ordine alfabetico
                                if(sondacoppia1 == NULL) {
                                    struct nodocoppia *newcoppia;
                                    newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                                    newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                                    strcpy(newcoppia->orig, ent1);
                                    newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                                    strcpy(newcoppia->dest, ent2);
                                    sondacoppia2->nextcoppia = newcoppia;
                                    newcoppia->nextcoppia = NULL;
                                    return;
                                }
                            }
                            //se la coppia non è presente, va inserita nella posizione tra sonda2 e sonda1
                            struct nodocoppia *newcoppia;
                            newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                            newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                            strcpy(newcoppia->orig, ent1);
                            newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                            strcpy(newcoppia->dest, ent2);
                            sondacoppia2->nextcoppia = newcoppia;
                            newcoppia->nextcoppia = sondacoppia1;
                            return;
                        }
                        sondarel1 = sondarel1->nextrel;
                        sondarel2 = sondarel2->nextrel;
                        //se sondarel1 diventa NULL, la relazione da inserire è l'ultima in ordine alfabetico
                        if(sondarel1 == NULL) {
                            struct nodorel *newrel;
                            struct nodocoppia *newcoppia;
                            newrel = (struct nodorel*)malloc(sizeof(struct nodorel));
                            newrel->rel = (char*)malloc(sizeof(char)*strlen(relazione) + 1);
                            strcpy(newrel->rel, relazione);
                            newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                            newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                            strcpy(newcoppia->orig, ent1);
                            newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                            strcpy(newcoppia->dest, ent2);
                            newrel->lista = newcoppia;
                            newrel->nextrel = sondarel1;
                            newcoppia->nextcoppia = NULL;
                            sondarel2->nextrel = newrel;
                            return;
                        }
                    }
                    //se la relazione non è presente, va inserita tra sondarel2 e sondarel1
                    struct nodorel *newrel;
                    struct nodocoppia *newcoppia;
                    newrel = (struct nodorel*)malloc(sizeof(struct nodorel));
                    newrel->rel = (char*)malloc(sizeof(char)*strlen(relazione) + 1);
                    strcpy(newrel->rel, relazione);
                    newcoppia = (struct nodocoppia*)malloc(sizeof(struct nodocoppia));
                    newcoppia->orig = (char*)malloc(sizeof(char)*strlen(ent1) + 1);
                    strcpy(newcoppia->orig, ent1);
                    newcoppia->dest = (char*)malloc(sizeof(char)*strlen(ent2) + 1);
                    strcpy(newcoppia->dest, ent2);
                    newrel->lista = newcoppia;
                    newrel->nextrel = sondarel1;
                    newcoppia->nextcoppia = NULL;
                    sondarel2->nextrel = newrel;
                    return;
                }
                if(strcmp(ent2, sondav2->nome) < 0) {
                    sondav2 = sondav2->sinistra;
                } else if(strcmp(ent2, sondav2->nome) > 0) {
                    sondav2 = sondav2->destra;
                }
            }
            //se non è stata trovata la seconda entità, termino
            break;
        }
        if(strcmp(ent1, sondav1->nome) < 0) {
            sondav1 = sondav1->sinistra;
        } else if(strcmp(ent1, sondav1->nome) > 0) {
            sondav1 = sondav1->destra;
        }
    }
    return;
}

void delrel(char *ent1, char *ent2, char *relazione) {
    //cerco la relazione da cancellare
    struct nodorel *sondarel1;
    sondarel1 = firstrel;
    //se la lista delle relazioni è vuota, termino
    if(sondarel1 == NULL) {
        return;
    }
    //se la relazione da eliminare è in testa
    if(strcmp(sondarel1->rel, relazione) == 0) {
        struct nodocoppia *sondacoppia1;
        sondacoppia1 = sondarel1->lista;
        //se la coppia da cancellare è in testa ed è l'unica
        if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0 && sondacoppia1->nextcoppia == NULL) {
            free(sondacoppia1->orig);
            free(sondacoppia1->dest);
            free(sondacoppia1);
            firstrel = sondarel1->nextrel;
            free(sondarel1->rel);
            free(sondarel1);
            return;
        }
        //se la coppia da cancellare è in testa ma non è l'unica
        if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
            sondarel1->lista = sondacoppia1->nextcoppia;
            free(sondacoppia1->orig);
            free(sondacoppia1->dest);
            free(sondacoppia1);
            return;
        }
        //se la coppia da cancellare non è in testa
        sondacoppia1 = sondacoppia1->nextcoppia;
        //se la coppia successiva non esiste, la coppia non esiste e quindi concludo
        if(sondacoppia1 == NULL) {
            return;
        }
        struct nodocoppia *sondacoppia2;
        sondacoppia2 = sondarel1->lista;
        //finchè la coppia da cancellare è maggiore o uguale di quella corrente
        while(strcmp(ent2, sondacoppia1->dest) >= 0) {
            //se la coppia da cancellare corrisponde
            if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
                sondacoppia2->nextcoppia = sondacoppia1->nextcoppia;
                free(sondacoppia1->orig);
                free(sondacoppia1->dest);
                free(sondacoppia1);
                return;
            }
            sondacoppia1 = sondacoppia1->nextcoppia;
            //se si arriva a NULL, la coppia non esiste e quindi concludo
            if(sondacoppia1 == NULL) {
                return;
            }
            sondacoppia2 = sondacoppia2->nextcoppia;   
        }
        //se ent2 diventa minore di dest, la coppia non è presente e quindi concludo
        return;
    }
    //se non è in testa, scorro alla seconda relazione
    sondarel1 = sondarel1->nextrel;
    //se si arriva a NULL, la relazione non esiste e quindi concludo
    if(sondarel1 == NULL) {
        return;
    }
    //se la relazione cercata è minore di quella corrente, concludo
    if(strcmp(relazione, sondarel1->rel) < 0) {
        return;
    }
    struct nodorel *sondarel2;
    sondarel2 = firstrel;
    //finchè la relazione da cancellare è maggiore o corrisponde con quella corrente
    while(strcmp(relazione, sondarel1->rel) >= 0) {
        if(strcmp(sondarel1->rel, relazione) == 0) {
            struct nodocoppia *sondacoppia1;
            sondacoppia1 = sondarel1->lista;
            //se la coppia da cancellare è in testa ed è l'unica
            if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0 && sondacoppia1->nextcoppia == NULL) {
                free(sondacoppia1->orig);
                free(sondacoppia1->dest);
                free(sondacoppia1);
                sondarel2->nextrel = sondarel1->nextrel;
                free(sondarel1->rel);
                free(sondarel1);
                return;
            }
            //se la coppia da cancellare è in testa ma non è l'unica
            if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
                sondarel1->lista = sondacoppia1->nextcoppia;
                free(sondacoppia1->orig);
                free(sondacoppia1->dest);
                free(sondacoppia1);
                return;
            }
            //se la coppia da cancellare non è in testa
            sondacoppia1 = sondacoppia1->nextcoppia;
            //se la coppia successiva non esiste, la coppia non esiste e quindi concludo
            if(sondacoppia1 == NULL) {
                return;
            }
            struct nodocoppia *sondacoppia2;
            sondacoppia2 = sondarel1->lista;
            //finchè la coppia da cancellare è maggiore o uguale di quella corrente
            while(strcmp(ent2, sondacoppia1->dest) >= 0) {
                //se la coppia da cancellare corrisponde
                if(strcmp(ent1, sondacoppia1->orig) == 0 && strcmp(ent2, sondacoppia1->dest) == 0) {
                    sondacoppia2->nextcoppia = sondacoppia1->nextcoppia;
                    free(sondacoppia1->orig);
                    free(sondacoppia1->dest);
                    free(sondacoppia1);
                    return;
                }
                sondacoppia1 = sondacoppia1->nextcoppia;
                //se si arriva a NULL, la coppia non esiste e quindi concludo
                if(sondacoppia1 == NULL) {
                    return;
                }
                sondacoppia2 = sondacoppia2->nextcoppia;
            }
            //se ent2 diventa minore di dest, la coppia non è presente e quindi concludo
            return;
        }
        sondarel1 = sondarel1->nextrel;
        sondarel2 = sondarel2->nextrel;
        if(sondarel1 == NULL) {
            return;
        }
    }
    return;
}

void trova(char *ent) {
    //cerco nella prima relazione
    struct nodorel *sondarel1;
    sondarel1 = firstrel;
    //se la lista delle relazioni è vuota oppure è terminata, termino
    if(sondarel1 == NULL) {
        return;
    }
    //controllo tutte le entità per la relazione
    struct nodocoppia *sondacoppia1;
    sondacoppia1 = sondarel1->lista;
    while(1){
        //se l'entità è presente nella prima coppia ed è unica
        if((strcmp(ent, sondacoppia1->orig) == 0 || strcmp(ent, sondacoppia1->dest) == 0) && sondacoppia1->nextcoppia == NULL) {
            free(sondacoppia1->orig);
            free(sondacoppia1->dest);
            free(sondacoppia1);
            firstrel = sondarel1->nextrel;
            free(sondarel1->rel);
            free(sondarel1);
            sondarel1 = firstrel;
            //se sondarel1 è NULL, concludo
            if(sondarel1 == NULL) {
                return;
            }
            sondacoppia1 = sondarel1->lista;
        }
        //se è nella prima coppia ma non è l'unica
        if((strcmp(ent, sondacoppia1->orig) == 0 || strcmp(ent, sondacoppia1->dest) == 0) && sondacoppia1->nextcoppia != NULL) {
            sondarel1->lista = sondacoppia1->nextcoppia;
            free(sondacoppia1->orig);
            free(sondacoppia1->dest);
            free(sondacoppia1);
            sondacoppia1 = sondarel1->lista;
        }
        //se non è nella prima coppia, termino il ciclo
        if(strcmp(ent, sondacoppia1->orig) != 0 && strcmp(ent, sondacoppia1->dest) != 0) {
            break;
        }
    }
    //scorro alla seconda coppia
    sondacoppia1 = sondacoppia1->nextcoppia;
    //se sondacoppia1 è NULL, passo alla prossima relazione
    if(sondacoppia1 == NULL) {
        sondarel1 = sondarel1->nextrel;
    } else{
        struct nodocoppia *sondacoppia2;
        sondacoppia2 = sondarel1->lista;
        while(sondacoppia1 != NULL) {
            if(strcmp(ent, sondacoppia1->orig) == 0 || strcmp(ent, sondacoppia1->dest) == 0) {
                sondacoppia2->nextcoppia = sondacoppia1->nextcoppia;
                free(sondacoppia1->orig);
                free(sondacoppia1->dest);
                free(sondacoppia1);
                sondacoppia1 = sondacoppia2->nextcoppia;
            } else {
                sondacoppia1 = sondacoppia1->nextcoppia;
                sondacoppia2 = sondacoppia2->nextcoppia;
            }
        }
        //arrivato a NULL, procedo con la prossima relazione
        sondarel1 = sondarel1->nextrel;
    }
    //se la seconda relazione è NULL, concludo
    if(sondarel1 == NULL) {
        return;
    }
    struct nodorel *sondarel2;
    sondarel2 = firstrel;
    //fino a quando non arrivo all'ultima relazione
    while(sondarel1 != NULL) {
        sondacoppia1 = sondarel1->lista;
        while(1){
            //se l'entità è presente nella prima coppia ed è unica
            if((strcmp(ent, sondacoppia1->orig) == 0 || strcmp(ent, sondacoppia1->dest) == 0) && sondacoppia1->nextcoppia == NULL) {
                free(sondacoppia1->orig);
                free(sondacoppia1->dest);
                free(sondacoppia1);
                sondarel2->nextrel = sondarel1->nextrel;
                free(sondarel1->rel);
                free(sondarel1);
                sondarel1 = sondarel2->nextrel;
                //se la relazione successiva è NULL, concludo
                if(sondarel1 == NULL) {
                    return;
                }
                sondacoppia1 = sondarel1->lista;
            }
            //se è nella prima coppia ma non è l'unica
            if((strcmp(ent, sondacoppia1->orig) == 0 || strcmp(ent, sondacoppia1->dest) == 0) && sondacoppia1->nextcoppia != NULL) {
                sondarel1->lista = sondacoppia1->nextcoppia;
                free(sondacoppia1->orig);
                free(sondacoppia1->dest);
                free(sondacoppia1);
                sondacoppia1 = sondarel1->lista;
            }
            //se non è nella prima coppia, termino il ciclo
            if(strcmp(ent, sondacoppia1->orig) != 0 && strcmp(ent, sondacoppia1->dest) !=0) {
                break;
            }
        }
        //scorro alla seconda coppia
        sondacoppia1 = sondacoppia1->nextcoppia;
        //se sondacoppia1 è NULL, passo alla prossima relazione
        if(sondacoppia1 == NULL) {
            sondarel1 = sondarel1->nextrel;
            sondarel2 = sondarel2->nextrel;
        } else {
            struct nodocoppia *sondacoppia2;
            sondacoppia2 = sondarel1->lista;
            while(sondacoppia1 != NULL) {
                if(strcmp(ent, sondacoppia1->orig) == 0 || strcmp(ent, sondacoppia1->dest) == 0) {
                    sondacoppia2->nextcoppia = sondacoppia1->nextcoppia;
                    free(sondacoppia1->orig);
                    free(sondacoppia1->dest);
                    free(sondacoppia1);
                    sondacoppia1 = sondacoppia2->nextcoppia;
                } else {
                    sondacoppia1 = sondacoppia1->nextcoppia;
                    sondacoppia2 = sondacoppia2->nextcoppia;
                }
            }
            //arrivato a NULL, procedo con la prossima relazione
            sondarel1 = sondarel1->nextrel;
            sondarel2 = sondarel2->nextrel;
        }
    }
    return;
}