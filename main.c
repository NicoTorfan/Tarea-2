#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>
#include "map.h"
#include "list.h"
#define KEY_UP 72
#define KEY_DOWN 80

typedef struct{
    char *id;
    char *ciudad;
    char *direccion;
    char *tipo;
    size_t capacidad;
    size_t precio;
}Propiedad;

typedef struct{
    Propiedad *propiedad;
    char * nombreLista;
    int cantidad;
}PropiedadesFav;

void gotoxy(int x,int y){
      HANDLE hcon;
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);
      COORD dwPos;
      dwPos.X = x;
      dwPos.Y= y;
      SetConsoleCursorPosition(hcon,dwPos);
 }

const char *get_csv_field(char *tmp, int k){
    int open_mark = 0;
    char *ret= (char *)malloc(100 * sizeof(char));
    int ini_i= 0, i= 0;
    int j= 0;
    while (tmp[i + 1] != '\0'){
        if (tmp[i] == '\"'){
            open_mark= 1-open_mark;
            if (open_mark)
                ini_i= i+1;
            i++;
            continue;
        }
        if (open_mark || tmp[i] != ','){
            if (k == j)
                ret[i-ini_i]= tmp[i];
            i++;
            continue;
        }
        if (tmp[i] == ','){
            if (k == j){
                ret[i-ini_i]= 0;
                return ret;
            }
            j++;
            ini_i= i+1;
        }
        i++;
    }
    if (k == j){
        ret[i-ini_i]=0;
        return ret;
    }
    return NULL;
}

int is_equal_str(void *key1, void *key2){
    if (strcmp((char*)key1, (char*)key2) == 0) return 1;
    return 0;
}

int lower_than_str(void *key1, void *key2){
    if (strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

void siuu(char *s){
    while (*s && *s != '\n' && *s != '\r') s++;
    *s= 0;
}

Propiedad * crearPropiedad(){
    Propiedad *new= (Propiedad *)malloc(sizeof(Propiedad));
    new->id= (char *)calloc(100, sizeof(char));
    new->ciudad= (char*)calloc(100,sizeof(char));
    new->direccion= (char*)calloc(100,sizeof(char));
    new->tipo= (char*)calloc(30,sizeof(char));
    new->capacidad= 0;
    new->precio= 0;
    return new;
}

void agregarPropiedad(Map* mapa){
    Propiedad* new= crearPropiedad();

    fflush(stdin);
    printf("Ingrese id de la propiedad: \n");
    fgets(new->id, 100, stdin);
    siuu(new->id);
    printf("\n");
    if (searchMap(mapa, new->id) != NULL){
        Propiedad * copiaprop= searchMap(mapa, new->id);
        PlaySound("fart-meme-sound.wav",NULL, SND_FILENAME|SND_ASYNC);
        printf("Propiedad Existente\n\n");
        free(new);
        return;
    }
    fflush(stdin);
    printf("Ingrese ciudad de la propiedad: \n");
    fgets(new->ciudad,100,stdin);
    siuu(new->ciudad);
    printf("\n");

    printf("Ingrese direccion de la propiedad: \n");
    fgets(new->direccion,100,stdin);
    siuu(new->direccion);
    printf("\n");

    printf("Ingrese tipo de la propiedad: \n");
    fgets(new->tipo,30,stdin);
    siuu(new->tipo);
    printf("\n");

    printf("Ingrese capacidad de la propiedad: \n");
    scanf("%ld",&new->capacidad);
    printf("\n");

    printf("Ingrese precio de la propiedad: \n");
    scanf("%ld",&new->precio);
    printf("\n\n");

    insertMap(mapa, new->id, new);
    PlaySound("siuu.wav",NULL, SND_FILENAME|SND_ASYNC);
}

void importarCSV(Map* mapaNombre){
    char nombreArchivo [100];
    printf("Ingrese Nombre del Archivo: \n");
    fflush(stdin);
    fgets(nombreArchivo,100,stdin);
    siuu(nombreArchivo);
    PlaySound("xs.wav",NULL, SND_FILENAME|SND_ASYNC);
    printf("\n");
    printf("Importando...\n");
    for (int i=0;i<=40;i++){
        gotoxy(i,18);printf("%c",177);
    }
    for (int i=0;i<=40;i++){
        gotoxy(i,18);printf("%c",219);
        Sleep(180);
    }
    printf("\n");
    FILE *fp = fopen(nombreArchivo,"r");
    if (fp == NULL){
        PlaySound("fart-meme-sound.wav",NULL, SND_FILENAME|SND_ASYNC);
        printf("Archivo no encontrado\n\n");
        return;
    }
    char linea[1024];
    fgets(linea,1024,fp);

    while (fgets(linea, 1023, fp) != NULL){
        Propiedad * new= crearPropiedad();

        strcpy(new->id, get_csv_field(linea, 0));
        siuu(new->id);
        strcpy(new->ciudad, get_csv_field(linea, 1));
        siuu(new->ciudad);
        strcpy(new->direccion, get_csv_field(linea, 2));
        siuu(new->direccion);
        strcpy(new->tipo, get_csv_field(linea, 3));
        siuu(new->tipo);
        new->capacidad= atoi(get_csv_field(linea, 4));
        new->precio= atoi(get_csv_field(linea,5));

        insertMap(mapaNombre, new->id, new);
    }
    PlaySound("siuu.wav",NULL, SND_FILENAME|SND_ASYNC);
    printf("Archivo importado Correctamente \n\n");
    fclose(fp);
}

void mostrarPropiedad(Propiedad * prop){
    printf("Id: %s\n",prop->id);
    printf("Ciudad: %s\nDireccion: %s\nTipo: %s\n",prop->ciudad,prop->direccion,prop->tipo);
    printf("Capacidad: %ld\nPrecio: %ld\n\n",prop->capacidad,prop->precio);
}

void mostrarMapa(Map * mapa){
    Propiedad * aux= firstMap(mapa);
    int x=0;
    while (aux){
        mostrarPropiedad(aux);
        aux= nextMap(mapa);
        x++;
    }
    if (!x) {
        printf("No hay Propiedades\n\n");
        PlaySound("Bruh-Sound-Effect-_HD_.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
    else {
    printf("\n\n");
    PlaySound("siuu.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
}

void mostrarporCiudad(Map * mapa){
    char key [100];
    printf("Ingrese Ciudad de propiedad a buscar: \n");
    fflush(stdin);
    fgets(key,100,stdin);
    siuu(key);
    printf("\n\n");

    Propiedad * aux= firstMap(mapa);
    int x=0;
    printf("Imprimiendo Por Ciudad...\n");
    PlaySound("Keyboard_typing_sound_effect.wav",NULL, SND_FILENAME|SND_ASYNC);
    for (int i=0;i<=40;i++){
        gotoxy(i,19);printf("%c",177);
    }
    for (int i=0;i<=40;i++){
        gotoxy(i,19);printf("%c",219);
        Sleep(50);
    }
    printf("\n");
    while (aux != NULL){
        if (strcmp(aux->ciudad,key) == 0){
            mostrarPropiedad(aux);
            x++;
        }
        aux= nextMap(mapa);
    }
    if (!x){
        printf("No se han encontrado propiedades en esta Ciudad\n\n");
        PlaySound("Bruh-Sound-Effect-_HD_.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
    else {
        printf("\n\n");
        PlaySound("siuu.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
}

void mostrarporTipo(Map * mapa){
    char key [30];
    printf("Ingrese Tipo de propiedad a buscar: \n");
    fflush(stdin);
    fgets(key,30,stdin);
    siuu(key);
    printf("\n\n");
    printf("Imprimiendo Por Tipo...\n");
    PlaySound("Keyboard_typing_sound_effect.wav",NULL, SND_FILENAME|SND_ASYNC);
    for (int i=0;i<=40;i++){
        gotoxy(i,19);printf("%c",177);
    }
    for (int i=0;i<=40;i++){
        gotoxy(i,19);printf("%c",219);
        Sleep(50);
    }
    printf("\n");
    Propiedad * aux= firstMap(mapa);
    int x=0;
    while (aux != NULL){
        if (strcmp(aux->tipo,key) == 0){
            mostrarPropiedad(aux);
            x++;
        }
        aux= nextMap(mapa);
    }
    if (!x) {
        printf("No se han encontrado propiedades de este Tipo\n\n");
        PlaySound("Bruh-Sound-Effect-_HD_.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
    else {
        printf("\n\n");
        PlaySound("siuu.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
}

void mostrarporCapacidad(Map * mapa){
    int cap;
    printf("Ingrese Capacidad de propiedad a buscar: \n");
    scanf("%ld",&cap);
    printf("\n\n");
    printf("Imprimiendo Por Tipo...\n");
    PlaySound("Keyboard_typing_sound_effect.wav",NULL, SND_FILENAME|SND_ASYNC);
    for (int i=0;i<=40;i++){
        gotoxy(i,19);printf("%c",177);
    }
    for (int i=0;i<=40;i++){
        gotoxy(i,19);printf("%c",219);
        Sleep(50);
    }
    printf("\n");
    Propiedad * aux= firstMap(mapa);
    int x=0;
    while (aux != NULL){
        if (aux->capacidad >= cap){
            mostrarPropiedad(aux);
            x++;
        }
        aux= nextMap(mapa);
    }
    if (!x) {
        printf("No se han encontrado propiedades con esta capacidad o mas\n\n");
        PlaySound("Bruh-Sound-Effect-_HD_.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
    else {
        printf("\n\n");
        PlaySound("siuu.wav",NULL, SND_FILENAME|SND_ASYNC);
    }
}

PropiedadesFav * searchList(List * lista, Propiedad * buscado){
    PropiedadesFav * aux= firstList(lista);

    while (aux != NULL){
        if (aux->propiedad->id == buscado->id) return aux;
        aux= nextList(lista);
    }
    return NULL;
}

void agregaraFav(Map * mapa, Map * favorito){
    char favs[9]="Favoritos";
    char * numeroID= (char *)malloc(100*sizeof(char));
    fflush(stdin);
    printf("Ingrese id de la propiedad: \n");
    fgets(numeroID,100, stdin);
    siuu(numeroID);
    printf("\n");

    Propiedad * prop= searchMap(mapa, numeroID);
    if (prop == NULL){
        PlaySound("videoplayback-_2_.wav",NULL, SND_FILENAME|SND_ASYNC);
        printf("Id no existente\n\n");
        return;
    }

    PropiedadesFav *new = (PropiedadesFav *)malloc(100 * sizeof(PropiedadesFav));
    new->propiedad = prop;

    printf("\n");
    new->nombreLista = (char *)malloc(100 * sizeof(char));
    strcpy(new->nombreLista, favs);

    List *auxF = searchMap(favorito, favs);

    if (auxF == NULL){
        List* listaF= createList();
        insertMap(favorito, favs, listaF);
        auxF= searchMap(favorito, favs);
    }
    else{
        PropiedadesFav * auxT= searchList(auxF, new->propiedad);
        if (auxT != NULL){
            PlaySound("videoplayback-_2_.wav",NULL, SND_FILENAME|SND_ASYNC);
            printf("Este Id ya se encuentra en favoritos\n");
            return;
        }
    }
    pushBack(auxF, new);
    PlaySound("videoplayback-_3_.wav",NULL, SND_FILENAME|SND_ASYNC);
    printf("Se ha agregado correctamente a Favoritos la propiedad\n");
}

void mostrarListaFav(List * lista){
    int x=1;
    PropiedadesFav* aux= firstList(lista);
    printf("Lista de Favoritos: \n\n");
    while (aux != NULL){
        printf("Id: %s\n",aux->propiedad->id);
        printf("Ciudad: %s\n", aux->propiedad->ciudad);
        printf("Direccion: %s\n", aux->propiedad->direccion);
        printf("Tipo: %s\n", aux->propiedad->tipo);
        printf("Capacidad: %ld\n", aux->propiedad->capacidad);
        printf("Precio: %ld\n", aux->propiedad->precio);
        printf("\n");
        aux= nextList(lista);
        x++;
    }
    printf("\n");
}

void mostrarPropiedadesFav(Map * favorito){
    List* aux= firstMap(favorito);
    if (aux == NULL){
        PlaySound("Bruh-Sound-Effect-_HD_.wav",NULL, SND_FILENAME|SND_ASYNC);
        printf("No hay propiedades en Favoritos\n\n");
        return;
    }
    printf("Imprimiendo Favoritos...\n");
    PlaySound("Keyboard_typing_sound_effect.wav",NULL, SND_FILENAME|SND_ASYNC);
    for (int i=0;i<=40;i++){
        gotoxy(i,16);printf("%c",177);
    }
    for (int i=0;i<=40;i++){
        gotoxy(i,16);printf("%c",219);
        Sleep(50);
    }
    printf("\n");
    while (aux != NULL){
        mostrarListaFav(aux);
        aux= nextMap(favorito);
    }
    printf("\n\n");
}

void exportarPropiedades(Map * mapa){
    char nombreArchivo[100];
    printf("Ingrese Nombre del Archivo: \n");
    fflush(stdin);
    fgets(nombreArchivo, 100, stdin);
    siuu(nombreArchivo);
    PlaySound("WAIT-WHAT-SOUND-EFFECT.wav",NULL, SND_FILENAME|SND_ASYNC);
    printf("\n");
    printf("Exportando...\n");
    for (int i=0;i<=40;i++){
        gotoxy(i,18);printf("%c",177);
    }
    for (int i=0;i<=40;i++){
        gotoxy(i,18);printf("%c",219);
        Sleep(200);
    }
    printf("\n");
    FILE *fp = fopen(nombreArchivo, "w");
    Propiedad * propiedadexp;
    propiedadexp= firstMap(mapa);
    fputs("Id,Ciudad,Direccion,Tipo,Capacidad,Valor",fp);
    fputc('\n', fp);
    while (propiedadexp != NULL){
        fputs(propiedadexp->id, fp);
        fputc(',', fp);
        fputs(propiedadexp->ciudad, fp);
        fputc(',', fp);
        fputs(propiedadexp->direccion, fp);
        fputc(',', fp);
        fputs(propiedadexp->tipo, fp);
        fputc(',', fp);
        fprintf(fp, "%d", propiedadexp->capacidad);
        fputc(',', fp);
        fprintf(fp, "%d", propiedadexp->precio);
        fputc('\n', fp);
        propiedadexp= nextMap(mapa);
    }
    fclose(fp);
}
void hallowen(){
	system("COLOR 6");
	gotoxy(55,1);
	printf("                  *\n");
	gotoxy(55,2);
	printf("                 **\n");
	gotoxy(55,3);
	printf("                ***\n");
	gotoxy(55,4);
	printf("         ******************\n");
	gotoxy(55,5);
	printf("      ************************\n");
	gotoxy(55,6);
	printf("   ******* ************ ********\n");
	gotoxy(55,7);
	printf("  *******  ************  ********\n");
	gotoxy(55,8);
	printf("  *******                ********\n");
	gotoxy(55,9);
	printf("  *******************************\n");
	gotoxy(55,10);
	printf("  *******   **           ********\n");
	gotoxy(55,11);
	printf("   *********            ********\n");
	gotoxy(55,12);
	printf("     *************************\n");
	gotoxy(55,13);
	printf("           *************\n");
}

int main(){
    Map * mapa= createMap(is_equal_str);
    Map * favorito= createMap(is_equal_str);

    setSortFunction(favorito, lower_than_str);
    setSortFunction(mapa, lower_than_str);

    //while(menu(mapa, favorito));
    int y2=3;
    char tecla=0;
    while (1)
    {
        PlaySound("Thriller.wav",NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
        system("cls");
        hallowen();
        gotoxy(0,0);
        printf("\nSeleccione una opci%cn\n",162);
        printf("\n[ ] Importar propiedades desde un archivo\n");
        printf("[ ] Agregar propiedad\n");
        printf("[ ] Mostrar todas las propiedades\n");
        printf("[ ] Mostrar propiedades de cierta ciudad\n");
        printf("[ ] Mostrar propiedades seg%cn su tipo\n",163);
        printf("[ ] Mostrar propiedades con capacidad m%cnima\n",161);
        printf("[ ] Agregar a favoritos\n");
        printf("[ ] Mostrar favoritos\n");
        printf("[ ] Exportar base de datos actualizada\n");
        printf("[ ] Salir\n");
        gotoxy(64,29);printf("%c Nintendo | %c Michael Jackson | %c Rodney L. Temperton | ",184, 184 ,184 ,184);
        gotoxy(90,0);printf("%c We Don't Talk About Bruno",184);
        while (tecla!=13){
            gotoxy(1,y2);tecla=getch();
            if (tecla==KEY_DOWN){
                y2=y2+1;
                if(y2>12){
                    y2=12;
                }
                gotoxy(1,y2);
            }
            else if(tecla==KEY_UP){
                y2=y2-1;
                if(y2<3){
                    y2=3;
                }
                gotoxy(1,y2);
            }
        }
        tecla=0;
        y2=y2-2;
        gotoxy(1,14);
        switch (y2)
        {
        case 1:
            importarCSV(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 2:
            agregarPropiedad(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 3:
            printf("\n");
            mostrarMapa(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 4:
            mostrarporCiudad(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 5:
            mostrarporTipo(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 6:
            mostrarporCapacidad(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 7:
            agregaraFav(mapa, favorito);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 8:
            mostrarPropiedadesFav(favorito);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 9:
            exportarPropiedades(mapa);printf("Presione una tecla para volver al men%c",163);getch();
            break;
        case 10:
            PlaySound("Ardilla.wav",NULL, SND_FILENAME);
            exit(-1);
        }
    y2=3;
    }
    fflush(stdin);
    getchar();
    return 0;
}
