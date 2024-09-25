#include <iostream>
#include <stack>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <map>
#include <ctime>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif
#include <vector>

using namespace std;


/*
------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS
------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS
------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS------LISTAS
*/
struct TiposTarea{ // idTipoTarea / NombreTipoTarea / DescripcionTarea ---- Lista circular
    int idTipoTarea;
    string nombreTipoTarea;
    string descripcionTarea;
    TiposTarea*sig;
    TiposTarea(int id,string nom, string des){
        idTipoTarea=id;
        nombreTipoTarea=nom;
        descripcionTarea=des;
        sig=NULL;
    }
}*listaTiposTarea1;

struct Personas{ // nombre / apellido / cedula / edad ---- lista doble ---- enlace a listaPendientes (tareas)
    string nombre;
    string apellido;
    string cedula;
    int edad;
    Personas*sig;
    Personas*ant;
    struct listaPendientes*tareas;
    struct tareasCompletadas*tareasTerminadas;


    Personas(string nom,string ape, string ced, int ed){
            nombre=nom;
            apellido=ape;
            cedula=ced;
            edad=ed;
            sig=NULL;
            ant=NULL;
            tareas=NULL;
    }
}*listaPersonas1;

struct listaPendientes { // id / descripcion / nivelImportancia / dia / mes / year / hora ---- lista sencilla ---- enlace a TiposTarea (enlaceTipos) y avanceListas (avance)
    int id;
    string descripcion;
    string nivelImportancia;
    bool completado;
    int dia, mes, year; // Agrupamos la fecha en una estructura para mejor manejo
    string hora;
    listaPendientes* sig;
    TiposTarea* enlaceTipos;
    struct avanceListas* avance;

    listaPendientes(int _id, string desc, string nivel, int d, int m, int a, string ho, TiposTarea* tareas) {
        id = _id;
        descripcion=desc;
        nivelImportancia=nivel;
        dia=d;
        mes=m;
        year=a;
        hora=ho;
        sig=NULL;
        enlaceTipos=tareas;
    }
}*listaPendientes1;

struct avanceListas { // string nombreTarea / string comentario / int porcentaje / bool completado ---- lista simple
    string nombreTarea;
    string comentario;
    float porcentaje;
    bool completado;
    int idSubtarea;
    avanceListas*sig;

    avanceListas(string nom, string com, float por, bool comple){
        nombreTarea=nom;
        comentario=com;
        porcentaje=por;
        completado=comple;
        sig=NULL;
    }
};

struct tareasCompletadas{ // int id / string descripcion / int porcentajeCumplimiento / string comentario ---- lista simple ---- enlace a TiposTarea
    int id;
    string descripcion;
    string nivelImportancia;
    int porcentajeCumplimiento;
    string comentario;
    TiposTarea*tipo;
    tareasCompletadas*sig;
    tareasCompletadas(int i, string des, int por, string com,TiposTarea*type){
        id=i;
        descripcion=des;
        porcentajeCumplimiento=por;
        comentario=com;
        tipo=type;
    }
};


/*
------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones
------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones
------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones------Complementos a otras funciones
*/

//Prototipos
void imprimirTiposTarea(TiposTarea*lista,int,int);
void imprimirPersonas(Personas*lista,int);
/*
int contarTareasPorTipo(listaPendientes,int);
*/
//1
float calcularAvance(Personas*lista,string cedula,int idTarea) {
    Personas*tempPersona=lista;
    while(tempPersona!=NULL){
        if(tempPersona->cedula==cedula){
            listaPendientes*tempPendientes=tempPersona->tareas;
            while(tempPendientes!=NULL){
                if(tempPendientes->id==idTarea){
                    avanceListas*tempAvance=tempPendientes->avance;
                    int total = 0, completadas = 0;
                    while (tempAvance!=NULL) {
                        total++;
                        if (tempAvance->completado) {
                            completadas++;
                        }
                        tempAvance = tempAvance->sig;
                    }
                    if (total == 0) {
                        return 0;
                    } else {
                        return (float)completadas / total * 100;
                    }
                }
                else{
                    tempPendientes=tempPendientes->sig;
                }
            }
            cout<<"No se encuentra la tarea a la cual se le quiere sacar el calculo del avance."<<endl;
            sleep(2);
            return 0;
        }
        else{
            tempPersona=tempPersona->sig;
        }
    }
    cout<<"La persona no se encuentra, por lo que no se puede calcular el avance de sus tareas"<<endl;
    sleep(2);
    return 0;

}

//2
bool comprobarCedulasPersonas(Personas*lista,string cedula,bool soloFormato=false){ //Función encargada de buscar cédulas repetidas en la lista Personas, si encuentra impide la inserción de la persona, también se encarga de ver que el formato de la cédula sea correcta.
        if(cedula[3]=='-'){
            try{
                int primero=stoi(cedula.substr(0,3));
                int segundo=stoi(cedula.substr(4,6));
                if(cedula.size()==7){
                    if(soloFormato){
                        return true;
                    }
                    Personas*temp=lista;
                    while(temp!=NULL){
                        if(temp->cedula==cedula){
                            return false;
                        }
                        else{
                            temp=temp->sig;
                        }
                    }
                    return true;
                }
                else{
                    return false;
                }
            }
            catch(int myNum){
                return false;
            }
        }
        else{
            return false;
        }
}

//3
bool comprobarFormatoFecha(string rangoFecha){ //Función encargada de comprobar que el formato de la fecha sea correcta. Esta función se usa junto a "imprimirTareasActivasFecha"
    if(rangoFecha[8]=='-'){
        try{
            if(stoi(rangoFecha.substr(0,2))>31){
                return false;
            }
            if(stoi(rangoFecha.substr(3,2))>12){
                return false;
            }

            int primero=stoi(rangoFecha.substr(6,2));

            if(stoi(rangoFecha.substr(9,2))>31){
                return false;
            }
            if(stoi(rangoFecha.substr(12,2))>12){
                return false;
            }

            int segundo=stoi(rangoFecha.substr(15,2));

            if(rangoFecha.size()==17){
                return true;
            }
            else{
                return false;
            }
        }
        catch(int myNum){
            return false;
        }
    }
    else{
        return false;
    }
}

//4
Personas* buscarPersonas(Personas*lista, string cedula){
    Personas*temp=lista;
    while(temp!=NULL){
        if(temp->cedula==cedula){
            return temp;
        }
        temp=temp->sig;
    }
    return NULL;
}

//5
int calcularDiasRestantes(int dia1, int mes1, int year1, int dia2, int mes2, int year2){
    tm fechaActual = {0};
    fechaActual.tm_mday = dia1;
    fechaActual.tm_mon = mes1 - 1;
    fechaActual.tm_year = year1 - 1900;

    tm fechaTarea = {0};
    fechaTarea.tm_mday = dia2;
    fechaTarea.tm_mon = mes2 - 1;
    fechaTarea.tm_year = year2 - 1900;

    time_t tiempoActual = mktime(&fechaActual);
    time_t tiempoTarea = mktime(&fechaTarea);

    double diferencia = difftime(tiempoTarea, tiempoActual);
    int diasRestantes = diferencia / (60 * 60 * 24);

    return diasRestantes;
}

//6 Función sencilla para encontrar una tarea en una persona(No recorre la lista Personas, solo la listaPendientes)
listaPendientes*buscarTarea(Personas*lista,int idTarea){
    listaPendientes*temp=lista->tareas;
    while(temp!=NULL){
        if(temp->id==idTarea){
            return temp;
        }
        else{
            temp=temp->sig;
        }
    }
    return temp;
}

//8
TiposTarea *buscarTipoTarea(TiposTarea* lista, int idTipo){
    if (lista == NULL){
        return NULL;
    }
    TiposTarea* temp = lista;
    do{
        if (temp->idTipoTarea == idTipo){
            return temp;
        }
        temp = temp->sig;
    }
    while (temp != lista);
    return NULL;
}

//9
void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

//10
TiposTarea* buscarTipoTareaPorID(TiposTarea* lista, int id){
    if (lista == NULL){
        cout<<"Lista vacia" << endl;
        return NULL;
    }
    TiposTarea* temp = lista;
    do {
        if(temp->idTipoTarea == id){
            cout << "Nombre: " << temp->nombreTipoTarea << endl;
            return temp;
        }
        temp = temp->sig;
    }
    while (temp != lista);
    cout << "No se encontró la tarea" <<endl;
    return NULL;
}

//11
listaPendientes* insertarTareaPendiente(listaPendientes* lista, listaPendientes* nueva){
    if (lista == NULL || (nueva->year < lista->year) ||
       (nueva->year == lista->year && nueva->mes < lista->mes)||
       (nueva->year == lista->year && nueva->mes == lista->mes && nueva->dia < lista->dia)||
       (nueva->year == lista->year && nueva->mes == lista->mes && nueva->dia == lista->dia && nueva->hora < lista->hora)){
        nueva->sig = lista;
        return nueva;
    }
    listaPendientes* temp = lista;
    while(temp->sig != NULL &&
         (temp->sig->year < nueva->year ||
         (temp->sig->year == nueva->year && temp->sig->mes < nueva->mes)||
         (temp->sig->year == nueva->year && temp->sig->mes == nueva->mes && temp->sig->dia < nueva->dia) ||
         (temp->sig->year == nueva->year && temp->sig->mes == nueva->mes && temp->sig->dia == nueva->dia && temp->sig->hora < nueva->hora))){
            temp = temp->sig;
    }
    nueva->sig = temp->sig;
    temp->sig = nueva;
    return lista;
}

//12
listaPendientes* borrarTarea(listaPendientes* lista, int idTarea){
    if (lista == NULL){
        cout << "No hay tareas pendientes." << endl;
        return NULL;
    }
    listaPendientes* temp = lista;
    listaPendientes* prev = NULL;

    while (temp != NULL){
        if (temp->id == idTarea){
            if (prev == NULL){
                listaPendientes* toDelete = temp;
                lista = temp->sig;
                delete toDelete;
                cout << "Tarea borrada con éxito." << endl;
                return lista;
            }
            else {
                prev->sig = temp->sig;
                delete temp;
                cout << "Tarea borrada con éxito." << endl;
                return lista;
            }
        }
        prev = temp;
        temp = temp->sig;
    }
    cout << "Tarea no encontrada" << endl;
    return lista;
}

//13
void reprogramarTarea(listaPendientes* tarea){
    if (tarea == NULL){
        cout << "Tarea no encontrada" << endl;
        return;
    }

    int nuevoDia, nuevoMes, nuevoYear;
    string nuevaHora;
    cout << "Ingrese la nueva fecha (día mes año): ";
    cin >> nuevoDia >> nuevoMes >> nuevoYear;
    cout << "Ingrese la nueva hora [HH:MM]: ";
    cin >> nuevaHora;
    cout << endl;

    tarea->dia = nuevoDia;
    tarea->mes = nuevoMes;
    tarea->year = nuevoYear;
    tarea->hora = nuevaHora;
    clearScreen();
    cout << "Tarea reprogramada con éxito." << endl;
    sleep(2);
}

//14
void imprimirTareasDePersona(Personas* persona){
    if (persona->tareas == NULL){
        cout << "No tiene tareas pendientes" << endl;
    }
    listaPendientes* temp = persona->tareas;
    cout << "Tareas pendientes de " << persona->nombre <<" " <<persona->apellido <<endl,
    cout << "----------------------------------------" << endl;
    while (temp != NULL){
        cout << "ID: " << temp->id << endl;
        cout << "Descripción: " << temp->descripcion << endl;
        cout << "Nivel de importancia: " << temp->nivelImportancia << endl;
        cout << "Fecha: " << temp->dia << "/" << temp->mes << "/" <<temp->year;
        cout << " Hora: " << temp->hora << endl;
        if (temp->enlaceTipos != NULL){
            cout << "Tipo de tarea: " << temp->enlaceTipos->nombreTipoTarea << endl;
            cout << "----------------------------------------" << endl;
        }
        else{
            cout << "Tipo de tarea: No asignado" << endl;
        }
        temp = temp->sig;
    }
}

/*
------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN
------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN
------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN------ACTUALIZACIÓN DE INFORMACIÓN
*/

//1 Insertar tipo de tareas.
TiposTarea*insertarTiposTarea(TiposTarea*lista,int id, string nombre,string descripcion){ //Insertar en lista TiposTarea
      TiposTarea*nuevo=new TiposTarea(id,nombre,descripcion);
        if(lista==NULL){
            lista=nuevo;
            nuevo->sig=nuevo;
            return lista;
        }
        else;
            TiposTarea*temp=lista;
            while(temp->sig!=lista){
                temp=temp->sig;
            }
            temp->sig=nuevo;
            nuevo->sig=lista;
            //lista=nuevo;
            return lista;
}

//2 Insertar personas.
Personas*insertarPersonas(Personas*lista,string nombre,string apellido,string cedula,int edad){ //Insertar en lista Personas
    Personas*nueva=new Personas(nombre,apellido,cedula,edad);
    if(lista==NULL){
        lista=nueva;
        return lista;
        }
    if(lista->cedula.substr(0,2)>=cedula.substr(0,2)){
        nueva->sig=lista;
        lista->ant=nueva;
        lista=nueva;
        return lista;
    }
    else{
        Personas*temp=lista;
        while(true){
            if(temp->sig==NULL){
                temp->sig=nueva;
                nueva->ant=temp;
                return lista;
            }
            else if(temp->sig->cedula.substr(0,2)>=cedula.substr(0,2)){
                Personas*basura=temp->sig;
                temp->sig=nueva;
                nueva->ant=temp;
                nueva->sig=basura;
                basura->ant=nueva;
                return lista;
            }
            temp=temp->sig;
        }
    }

}

//3 Borrar personas.
Personas*borrarPersonas(Personas*lista,string cedula){ //Borrar personas de lista Personas
    Personas*temp=lista;
    if(lista->sig==NULL && lista->cedula==cedula){
            lista=NULL;
            cout<<"Persona eliminada con exito."<<endl;
            sleep(2);
            return(lista);
    }
    else{
        while(temp->sig!=NULL && temp->cedula!=cedula){
            temp=temp->sig;
        }
        if(temp==NULL){
                cout<<"La cedula que se digito no se encuentra en la lista."<<endl;
                sleep(2);
                return(lista);
        }
        else{
            if(temp==lista){
                lista=lista->sig;
                lista->ant=NULL;
                cout<<"Persona eliminada con exito."<<endl;
                sleep(2);
                return(lista);
            }
            else if(temp->sig==NULL){
                temp->ant->sig=NULL;
                cout<<"Persona eliminada con exito."<<endl;
                sleep(2);
                return(lista);
            }
            else{
                temp->ant->sig=temp->sig;
                temp->sig->ant=temp->ant;
                cout<<"Persona eliminada con exito."<<endl;
                sleep(2);
                return(lista);
            }
        }
    }
}

//4 Insertar tareas activas a una persona X.
void agregarTareaAPersona(Personas* listaPersonas, TiposTarea* listaTipos){
    string cedula;
    clearScreen();
    imprimirPersonas(listaPersonas1,0);
    cout <<endl<< "Ingrese la cédula de la persona: ";
    cin >> cedula;

    Personas* persona = buscarPersonas(listaPersonas, cedula);
    if (persona == NULL){
        clearScreen();
        cout << "Persona no encontrada" << endl;
        sleep(2);
        return;
    }

    string descripcion, nivelImportancia, hora;
    int dia, mes, year;

    cout << "Descripción de la tarea: ";
    cin.ignore();
    getline(cin, descripcion);
    cout << "Nivel de importancia de la tarea (Baja, Media, Alta): ";
    getline (cin, nivelImportancia);
    cout << "Fecha de la tarea (dia mes año): ";
    cin >> dia >> mes >> year;
    cout << "Hora de la tarea (HH:MM): ";
    cin >> hora;

    int idTipo;
    imprimirTiposTarea(listaTipos,0,-1);
    cout << "Ingrese el id del tipo de tarea de tu tarea: ";
    cin >> idTipo;
    TiposTarea* tipoTarea = buscarTipoTareaPorID(listaTipos, idTipo);
    if (tipoTarea == NULL){
        clearScreen();
        cout << "Tipo de tarea no existe" << endl;
        sleep(2);
        return;
    }
    listaPendientes*tempPendientes=persona->tareas;
    while(tempPendientes->sig!=NULL){
        tempPendientes=tempPendientes->sig;
    }
    listaPendientes* nuevaTarea = new listaPendientes(tempPendientes->id+1, descripcion, nivelImportancia, dia, mes, year, hora, tipoTarea);
    if (persona->tareas == NULL){
        persona->tareas = nuevaTarea;
    }
    else{
        persona->tareas = insertarTareaPendiente(persona->tareas, nuevaTarea);
    }
    clearScreen();
    cout << "Tarea agregada con éxito" << endl;
    sleep(3);
    clearScreen();
}

//5 Modificar tareas activas, para que se puede reprogramar en otra fecha y hora.
void reprogramarTareasDePersonas(Personas* listaPersonas){
    string cedula;
    cout << "Ingrese la cédula de la persona: ";
    cin >> cedula;
    cin.ignore(10000,'\n');

    Personas* persona = buscarPersonas(listaPersonas, cedula);
    if (persona == NULL){
        clearScreen();
        cout << "Persona no encontrada" << endl;
        sleep(2);
        return;
    }

    int idTarea;
    imprimirTareasDePersona(persona);
    cout << "Ingrese el ID de la tarea a reprogramar: ";
    cin >> idTarea;
    cin.ignore(10000,'\n');


    listaPendientes* tarea = persona->tareas;
    while (tarea != NULL){
        if (tarea->id == idTarea){
            reprogramarTarea(tarea);
            return;
        }
    }

    cout << "Tarea no encontrada" << endl;
}

//6 Borrar tareas activas.
void borrarTareaDePersona(Personas* listaPersonas){
    string cedula;
    cout << "Ingrese la cedula de la persona: ";
    cin >> cedula;

    Personas* persona = buscarPersonas(listaPersonas, cedula);
    if (persona == NULL){
        clearScreen();
        cout << "Persona no encontrada." << endl;
        return;
    }

    int idTarea;
    imprimirTareasDePersona(persona);
    cout << "Ingrese el id de la tarea a borrar: ";
    cin >> idTarea;
    persona->tareas = borrarTarea(persona->tareas, idTarea);
}

//7 Insertar subtareas.
Personas*insertarSubtarea(Personas*lista,string cedulaPersona,int idTarea,string nombreTarea,string comentario) {
        if(lista==NULL){
            cout<<"La lista personas esta vacia."<<endl;
            sleep(2);
            return lista;
        }
        else{
            Personas*tempPersonas=lista;
            while(tempPersonas!=NULL){
                if(tempPersonas->cedula==cedulaPersona){
                    listaPendientes*tempPendientes=tempPersonas->tareas;
                    while(tempPendientes!=NULL){
                        if(tempPendientes->id==idTarea){
                            if(tempPendientes->enlaceTipos->nombreTipoTarea=="Estudio"){
                                avanceListas*nuevoAvance= new avanceListas(nombreTarea,comentario,0,false);
                                if(tempPendientes->avance==NULL){
                                    tempPendientes->avance=nuevoAvance;
                                    cout<<"Completado con exito."<<endl;
                                    sleep(2);
                                    return lista;
                                }
                                else{
                                    avanceListas*tempAvance=tempPendientes->avance;
                                    while(tempAvance->sig!=NULL){
                                        tempAvance=tempAvance->sig;
                                    }
                                    tempAvance->sig=nuevoAvance;
                                    cout<<"Completado con exito."<<endl;
                                    sleep(2);
                                    return lista;
                                }
                            }
                            else{
                                cout<<"Lo sentimos, la tarea a la que se quiere subir un avance de subtareas no es de tipo Estudio"<<endl;
                                sleep(2);
                                return lista;
                            }
                        }
                        tempPendientes=tempPendientes->sig;
                    }
                    cout<<endl<<"Tarea no encontrada"<<endl;
                    sleep(2);
                    return lista;
                }
                tempPersonas=tempPersonas->sig;
            }
            cout<<"Persona no encontrada, asegurese que la cedula sea la correcta."<<endl;
            sleep(2);
            return lista;
        }
}

//8 Modificar el porcentaje de avance y estado de completado a una subtarea X.
Personas*modificarSubtarea(Personas*lista,string cedulaPersona,int idTarea, int idSubtarea, int nuevoPorcentaje) {
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==cedulaPersona){
            listaPendientes*tempPendiente=tempPersonas->tareas;
            while(tempPendiente!=NULL){
                if(tempPendiente->id==idTarea){
                    avanceListas* tempAvance = tempPendiente->avance;
                    while (tempAvance != NULL) {
                        if (tempAvance->idSubtarea == idSubtarea) {
                            // Actualizamos el porcentaje de avance
                            tempAvance->porcentaje = nuevoPorcentaje;
                            // Actualizamos el estado de completado si el porcentaje es 100
                            tempAvance->completado = (nuevoPorcentaje == 100);
                            return lista;
                        }
                        tempAvance = tempAvance->sig;
                    }
                    // Si no se encontró la subtarea, mostramos un mensaje de error (opcional)
                    cout << "Subtarea no encontrada" << endl;
                    return lista;
                }
                else{
                    tempPendiente=tempPendiente->sig;
                }
            }
            cout<<"La tarea a la cual se le quiere modificar su avance no se encuentra."<<endl;
            sleep(2);
            return lista;
        }
        else{
            tempPersonas=tempPersonas->sig;
        }
    }
    cout<<"La persona no se encuentra, por lo que no se puede modificar el avance de sus tareas."<<endl;
    sleep(2);
    return lista;
}

//9 Insertar que una tarea se completó (insertando en la sublista de tareas realizadas y borradas de la sublista de tareas activas).
Personas*marcarTareaComoCompletada(Personas* persona,string cedulaPersona, int idTarea) {
    Personas*tempPersonas=persona;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==cedulaPersona){
            listaPendientes* tarea = tempPersonas->tareas;
            while (tarea != NULL) {
                if (tarea->id == idTarea) {
                    string comentario;
                    cout<<"Escriba un comentario sobre la tarea: "<<endl;
                    getline(cin,comentario);
                    if(tarea->enlaceTipos->nombreTipoTarea=="Estudio"){
                        float porcentaje=calcularAvance(persona,cedulaPersona,idTarea);
                        tareasCompletadas* nuevaTareaCompletada = new tareasCompletadas(tarea->id,tarea->descripcion,porcentaje,comentario,tarea->enlaceTipos);
                        // Agregar la nueva tarea completada a la lista
                        nuevaTareaCompletada->sig = persona->tareasTerminadas;
                        persona->tareasTerminadas = nuevaTareaCompletada;

                        // Eliminar la tarea de la lista de tareas activas
                        listaPendientes*tempPendientes=tempPersonas->tareas;
                        if(tempPendientes->sig==NULL){
                            tempPendientes=NULL;
                            return persona;
                        }
                        while(tempPendientes->sig->id!=idTarea){
                            tempPendientes=tempPendientes->sig;
                        }
                        tempPendientes->sig=tempPendientes->sig->sig;

                        return persona;
                        }
                    else{
                        tareasCompletadas* nuevaTareaCompletada = new tareasCompletadas(tarea->id,tarea->descripcion,100,comentario,tarea->enlaceTipos);
                        // Agregar la nueva tarea completada a la lista
                        nuevaTareaCompletada->sig = persona->tareasTerminadas;
                        persona->tareasTerminadas = nuevaTareaCompletada;

                        // Eliminar la tarea de la lista de tareas activas
                        listaPendientes*tempPendientes=tempPersonas->tareas;
                        if(tempPendientes->sig==NULL){
                            tempPendientes=NULL;
                            return persona;
                        }
                        while(tempPendientes->sig->id!=idTarea){
                            tempPendientes=tempPendientes->sig;
                        }
                        tempPendientes->sig=tempPendientes->sig->sig;

                        return persona;
                    }
                }
                tarea = tarea->sig;
            }
            throw 505;
        }
        else{
            tempPersonas=tempPersonas->sig;
        }
    }
    // Si no se encontró la tarea, mostrar un mensaje de error
    cout << "Persona no fue encontrada" <<endl;
    return persona;
}

/*
-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS
-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS
-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS-------CONSULTAS
*/

//1 ¿Cuál es la persona que tiene más tareas activas?
void personaConMasTareasActivas(Personas* listaPersonas){
    if (listaPersonas == NULL){
        cout << "No hay personas en la lista." << endl;
        return;
    }

    int maxTareas = 0;
    vector<Personas*> empatados;

    Personas* temp = listaPersonas;
    while (temp != NULL){
        int contadorTareas = 0;
        listaPendientes* tareasTemp = temp->tareas;

        while (tareasTemp != NULL){
            contadorTareas++;
            tareasTemp = tareasTemp->sig;
        }
        if (contadorTareas > maxTareas){
            maxTareas = contadorTareas;
            empatados.clear();
            empatados.push_back(temp);
        } else if (contadorTareas == maxTareas){
            empatados.push_back(temp);
        }
        temp = temp->sig;
    }
    if (maxTareas > 0){
        cout << "Personas con mas tareas activas: " << endl;
        for (Personas* persona : empatados){
            cout << persona->nombre << " " << persona->apellido << endl;
        }
    }
    else{
        cout << "No hay tareas activas en la lista." << endl;
    }
}


//2 ¿Cuál es la persona que tiene más tareas activas de un tipo X?
void personaConMasTareasPorTipo(Personas* listaPersonas, TiposTarea* listaTipos) {
    if (listaPersonas == NULL) {
        cout << "No hay personas en la lista." << endl;
        return;
    }

    int idTipo;
    imprimirTiposTarea(listaTipos,0,-1);
    cout << "Ingrese el ID del tipo de tarea: ";
    cin >> idTipo;

    // Buscar el nombre del tipo de tarea
    TiposTarea* tipoBuscado = listaTipos;
    string nombreTipo;
    while (tipoBuscado != NULL) {
        if (tipoBuscado->idTipoTarea == idTipo) {
            nombreTipo = tipoBuscado->nombreTipoTarea;
            break;
        }
        tipoBuscado = tipoBuscado->sig;
    }

    if (nombreTipo.empty()) {
        cout << "No se encontró el tipo de tarea con ID " << idTipo << "." << endl;
        return;
    }

    // Contador para las tareas
    int maxTareas = 0;
    vector<Personas*> empatados;

    // Recorremos la lista de personas
    Personas* temp = listaPersonas;
    while (temp != NULL) {
        int contadorTareas = 0;
        listaPendientes* tareasTemp = temp->tareas;

        // Recorremos las tareas de cada persona
        while (tareasTemp != NULL) {
            if (tareasTemp->enlaceTipos != NULL && tareasTemp->enlaceTipos->idTipoTarea == idTipo) {
                contadorTareas++;
            }
            tareasTemp = tareasTemp->sig;
        }

        // Comparamos el contador con el máximo
        if (contadorTareas > maxTareas) {
            maxTareas = contadorTareas;
            empatados.clear();
            empatados.push_back(temp);
        } else if (contadorTareas == maxTareas) {
            empatados.push_back(temp);
        }

        temp = temp->sig;
    }

    // Mostramos el resultado
    if (maxTareas > 0) {
        cout << "Persona(s) con más tareas activas del tipo '" << nombreTipo << "': " << endl;
        for (Personas* persona : empatados) {
            cout << persona->nombre << " " << persona->apellido << "(" << maxTareas<<")" << endl;
        }
    } else {
        cout << "No hay tareas activas de este tipo en la lista." << endl;
    }
}

//3 ¿Qué tipo de tarea es el más común? En caso de empate indicarlo.
void tipoDeTareaMasComun(Personas* listaPersonas, TiposTarea* listaTipos) {
    if (listaPersonas == NULL) {
        cout << "No hay personas en la lista." << endl;
        return;
    }

    // Mapa para contar tareas por tipo
    map<int, int> contadorTipos;

    // Recorrer todas las personas y sus tareas
    Personas* temp = listaPersonas;
    while (temp != NULL) {
        listaPendientes* tareasTemp = temp->tareas;

        while (tareasTemp != NULL) {
            if (tareasTemp->enlaceTipos != NULL) {
                contadorTipos[tareasTemp->enlaceTipos->idTipoTarea]++;
            }
            tareasTemp = tareasTemp->sig;
        }
        temp = temp->sig;
    }

    // Determinar el tipo de tarea más común
    int maxTareas = 0;
    vector<string> tiposComunes;

    for (const auto& par : contadorTipos) {
        if (par.second > maxTareas) {
            maxTareas = par.second;
            tiposComunes.clear();
            // Obtener el nombre del tipo de tarea
            TiposTarea* tipoTemp = listaTipos;
            while (tipoTemp != NULL) {
                if (tipoTemp->idTipoTarea == par.first) {
                    tiposComunes.push_back(tipoTemp->nombreTipoTarea);
                    break;
                }
                tipoTemp = tipoTemp->sig;
            }
        } else if (par.second == maxTareas) {
            // Obtener el nombre del tipo de tarea
            TiposTarea* tipoTemp = listaTipos;
            while (tipoTemp != NULL) {
                if (tipoTemp->idTipoTarea == par.first) {
                    tiposComunes.push_back(tipoTemp->nombreTipoTarea);
                    break;
                }
                tipoTemp = tipoTemp->sig;
            }
        }
    }

    // Mostrar los resultados
    if (maxTareas > 0) {
        cout << "Tipo(s) de tarea más común(es) con " << maxTareas << "tareas asociadas:" << endl;
        for (const string& nombreTipo : tiposComunes) {
            cout << "Nombre de Tipo de Tarea: " << nombreTipo << endl;
        }
    } else {
        cout << "No hay tareas registradas." << endl;
    }
}

//4 ¿Cuál es la persona que tiene más tareas vencidas de un tipo X dado una fecha Y.
void contarTareasVencidasPorTipo(Personas* lista, int idTipoTarea, int dia, int mes, int year, string hora) {
    map<string,int> personasCantidadVencida;
    Personas* personaActual = lista;
    while (personaActual != NULL){
        listaPendientes* tarea = personaActual->tareas;
        while (tarea != NULL) {
            if ((tarea->enlaceTipos->idTipoTarea == idTipoTarea && tarea->year > year )
                ||(tarea->enlaceTipos->idTipoTarea == idTipoTarea && tarea->year == year && tarea->mes > mes)
                ||(tarea->enlaceTipos->idTipoTarea == idTipoTarea && tarea->year == year && tarea->mes == mes && tarea->dia > dia)
                ||(tarea->enlaceTipos->idTipoTarea == idTipoTarea && tarea->year == year && tarea->mes == mes && tarea->dia == dia && tarea->hora > hora)) {
                string idPersona=personaActual->cedula;
                personasCantidadVencida[idPersona]++;
            }
            tarea = tarea->sig;
        }
        personaActual = personaActual->sig;
    }
    if(personasCantidadVencida.empty()){
        clearScreen();
        cout<<"Lo sentimos, ninguna persona tiene una tarea vencida."<<endl;
        sleep(2);
        return;
    }
    string idPersonaOtro;
    int maxConteo = 0;
    for (auto par : personasCantidadVencida) {
        if (par.second > maxConteo) {
            maxConteo = par.second;
            idPersonaOtro = par.first;
        }
    }
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==idPersonaOtro){
            cout<<"La persona con la mayor cantidad de tareas vencidas es: "<<tempPersonas->nombre<<endl;
            sleep(2);
            return;
        }
        else{
            tempPersonas=tempPersonas->sig;
        }
    }
}

//5 ¿Cuál es el tipo de tareas más común que se vence sin completarse, dado una fecha Y.
void tipoTareaMasComunVencida(Personas* lista, int dia,int mes,int year,string hora) {
    map<int, int> conteoTipos; // Mapa para contar las ocurrencias de cada tipo de tarea

    Personas* personaActual = lista;
    while (personaActual != NULL) {
        listaPendientes* tarea = personaActual->tareas;
        while (tarea != NULL) {
            if ((tarea->year > year)
                ||(tarea->year == year && tarea->mes > mes)
                ||(tarea->year == year && tarea->mes == mes && tarea->dia>dia)
                ||(tarea->year == year && tarea->mes == mes && tarea->dia==dia && tarea->hora >hora)){
                int idTipo=tarea->enlaceTipos->idTipoTarea;
                conteoTipos[idTipo]++;
            }
            tarea = tarea->sig;
        }
        personaActual = personaActual->sig;
    }
    if(conteoTipos.empty()){
        clearScreen();
        cout<<"Lo sentimos, no hay ninguna tarea que vence después de la fecha que dio."<<endl;
        sleep(2);
        return;
    }
    // Encontrar el tipo de tarea con mayor conteo
    int tipoMasComun = -1;
    int maxConteo = 0;
    for (auto par : conteoTipos) {
        if (par.second > maxConteo) {
            maxConteo = par.second;
            tipoMasComun = par.first;
        }
    }
    TiposTarea*tempTipos=listaTiposTarea1;
    while(tempTipos!=NULL){
        if(tempTipos->idTipoTarea==tipoMasComun){
            cout<<"El tipo de tarea que mas suele vencer en la fecha dada es: "<<tempTipos->nombreTipoTarea<<endl;
            sleep(2);
            return;
        }
        tempTipos=tempTipos->sig;
    }
}

//6 ¿Cuál es el tipo de importancia más usado por las personas?
void importanciaMasComunListaPendientes(Personas*lista){ //Imprime en terminal la el tipo de importancia de las tareas mas comun entre las personas de la lista Personas
    int importanciaBaja=0;
    int importanciaMedia=0;
    int importanciaAlta=0;
    if(lista==NULL){
        cout<<endl<<"La lista personas esta vacia."<<endl;
        sleep(2);
        return;
    }
    else{
        Personas*tempPersonas=lista;
        while(tempPersonas!=NULL){
            listaPendientes*tempPendientes=tempPersonas->tareas;
            while(tempPendientes!=NULL){
                if(tempPendientes->nivelImportancia=="Baja"){
                    importanciaBaja+=1;
                }
                else if(tempPendientes->nivelImportancia=="Media"){
                    importanciaMedia+=1;
                }
                else if(tempPendientes->nivelImportancia=="Alta"){
                    importanciaAlta+=1;
                }
                tempPendientes=tempPendientes->sig;
            }
            tempPersonas=tempPersonas->sig;
        }
        if(importanciaBaja==importanciaMedia && importanciaBaja==importanciaAlta){
            cout<<endl<<"El nivel de importancia de tareas mas utilizada entre las personas son: Baja, media y alta"<<endl;
            return;
        }
        else if(importanciaBaja==importanciaMedia){
            cout<<endl<<"El nivel de importancia de tareas mas utilizada entre las personas son: baja y media"<<endl;
            return;
        }
        else if(importanciaBaja==importanciaAlta){
            cout<<endl<<"El nivel de importancia de tareas mas utilizada entre las personas son: baja y alta"<<endl;
            return;
        }
        else if(importanciaBaja>importanciaMedia && importanciaBaja>importanciaAlta){
            cout<<endl<<"El nivel de importancia de tareas mas utilizada entre las personas es: baja"<<endl;
            return;
        }
        else if(importanciaMedia>importanciaBaja && importanciaMedia>importanciaAlta){
            cout<<endl<<"El nivel de importancia de tareas mas utilizada entre las personas es: media"<<endl;
            return;
        }
        else{
            cout<<endl<<"El nivel de importancia de tareas mas utilizada entre las personas es: alta"<<endl;
            return;
        }
    }
}

//7 ¿Qué es el tipo de tarea más común en tareas activas de importancia media?
void tipoTareaActivasMasComunEnImportanciaMedia(Personas*lista){ //Imprime el tipoTarea más común entre la tareas activas de importnacia media de las personas de la lista Personas
    map<int,int> contadorTiposTarea;
    if(lista==NULL){
        cout<<endl<<"La lista personas esta vacia."<<endl;
        sleep(2);
        return;
    }
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        listaPendientes*tempPendientes=tempPersonas->tareas;
        while(tempPendientes!=NULL){
            if(tempPendientes->nivelImportancia=="Media"){
                int idTipo = tempPendientes->enlaceTipos->idTipoTarea;
                contadorTiposTarea[idTipo]++;
                tempPendientes = tempPendientes->sig;
            }
            else{
                tempPendientes = tempPendientes->sig;
            }
        }
        tempPersonas=tempPersonas->sig;
    }
    int idMasComun;
    int cantidadMasComun;
    bool primeraVez=true;
    for(auto pair:contadorTiposTarea){
        if(primeraVez){
            idMasComun=pair.first;
            cantidadMasComun=pair.second;
        }
        else if(pair.second>cantidadMasComun){
            idMasComun=pair.first;
            cantidadMasComun=pair.second;
        }
    }
    cout<<"El tipo de tarea mas comun con importancia media es: "<<endl<<buscarTipoTarea(listaTiposTarea1,idMasComun)->nombreTipoTarea<<" con la id: "<<buscarTipoTarea(listaTiposTarea1,idMasComun)->idTipoTarea<<endl;
    sleep(2);
    return;
}

//8 ¿Qué es el tipo de tarea más común en tareas realizadas de importancia Alta?
void tipoTareaRealizadasMasComunEnImportanciaAlta(Personas*lista){ //Imprime el tipoTarea más común entre la tareas completadas de importancia alta de las personas de la lista Personas
    map<int,int> contadorTiposTarea;
    if(lista==NULL){
        cout<<endl<<"La lista personas esta vacia."<<endl;
        sleep(2);
        return;
    }
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        tareasCompletadas*tempCompletadas=tempPersonas->tareasTerminadas;
        while(tempCompletadas!=NULL){
            if(tempCompletadas->nivelImportancia=="Alto"){
                int idTipo = tempCompletadas->tipo->idTipoTarea;
                contadorTiposTarea[idTipo]++;
                tempCompletadas = tempCompletadas->sig;
            }
            else{
                tempCompletadas = tempCompletadas->sig;
            }
        }
        tempPersonas=tempPersonas->sig;
    }
    if(contadorTiposTarea.empty()){
        clearScreen();
        cout<<"Nadie ha completado una tarea aún, volviendo al menú."<<endl;
        sleep(2);
        return;
    }
    int idMasComun;
    int cantidadMasComun;
    bool primeraVez=true;
    for(auto pair:contadorTiposTarea){
        if(primeraVez){
            idMasComun=pair.first;
            cantidadMasComun=pair.second;
        }
        else if(pair.second>cantidadMasComun){
            idMasComun=pair.first;
            cantidadMasComun=pair.second;
        }
    }
    cout<<"La tarea realizada mas comun con importancia alta es: "<<endl<<buscarTipoTarea(listaTiposTarea1,idMasComun)->nombreTipoTarea<<endl<<" con la id: "<<buscarTipoTarea(listaTiposTarea1,idMasComun)->idTipoTarea<<endl;
    sleep(2);
    return;

}

/*
-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES
-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES
-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES-----REPORTES
*/

//1 Imprimir la lista de tipos de tareas.
void imprimirTiposTarea(TiposTarea*lista,int numero=0,int id=-1){ //Imprimir en terminal la lista TiposTarea
    if(id==lista->idTipoTarea){
        return;
    }
    else{
        if(numero==0){
            cout<<"---------Tipos de tarea---------"<<endl<<endl;
            id=lista->idTipoTarea;
        }
        cout<<numero<<")    Tipo tarea: "<<lista->nombreTipoTarea<<endl;
        cout<<"      Id tarea: "<<lista->idTipoTarea<<endl;
        cout<<"      Descripcion: "<<lista->descripcionTarea<<endl<<endl;
        imprimirTiposTarea(lista->sig,numero+=1,id=id);
    }
}

//2 Imprimir las personas.
void imprimirPersonas(Personas*lista,int numero=0){ //Impimir en terminal la lista de Personas
    if(lista==NULL){
        return;
    }
    else{
        if(numero==0){
            cout<<"---------Personas lista---------"<<endl;
        }
        cout<<numero<<")     Nombre: "<<lista->nombre<<" "<<lista->apellido<<endl;
        cout<<"       Cedula: "<<lista->cedula<<endl;
        cout<<"       Edad: "<<lista->edad<<endl<<endl;
        imprimirPersonas(lista->sig,numero+=1);
    }
}

//3 Imprimir las personas sin tareas activas.
void imprimirPersonasSinTareas(Personas* listaPersonas){
    if (listaPersonas == NULL){
        cout << "No hay personas en la lista." << endl;
        return;
    }
    bool hayPersonasSinTareas = false;
    cout<< "Personas sin tareas:" << endl <<endl;
    Personas* temp = listaPersonas;
    while (temp != NULL){
        if (temp->tareas == NULL){
            cout << temp->nombre << " " << temp->apellido << endl;
            hayPersonasSinTareas = true;
        }
        temp = temp->sig;
    }
    if (!hayPersonasSinTareas){
    cout << "Todas las personas tienen tareas activas." << endl;
    }
}

//4 Imprimir las tareas activas de una persona X, por orden de fecha y hora. En un rango de fechas ingresado por el usuario (fecha inicio y fecha final).
void imprimirTareasActivasFecha(Personas*lista,string cedula,string rangoFecha){ //Imprime una serie de tareas activas de una persona X por fecha
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==cedula){
            listaPendientes*tempPendientes=tempPersonas->tareas;
            while(tempPendientes!=NULL){
                if((stoi(rangoFecha.substr(6,2))<tempPendientes->year && stoi(rangoFecha.substr(15,2))>tempPendientes->year)
                    ||(stoi(rangoFecha.substr(6,2))==tempPendientes->year && stoi(rangoFecha.substr(3,2))<tempPendientes->mes && stoi(rangoFecha.substr(15,2))>tempPendientes->year)
                    ||(stoi(rangoFecha.substr(6,2))==tempPendientes->year && stoi(rangoFecha.substr(3,2))==tempPendientes->mes && stoi(rangoFecha.substr(0,2))<=tempPendientes->dia && stoi(rangoFecha.substr(15,2))>tempPendientes->year)
                    ||(stoi(rangoFecha.substr(6,2))==tempPendientes->year && stoi(rangoFecha.substr(3,2))==tempPendientes->mes && stoi(rangoFecha.substr(0,2))<=tempPendientes->dia && stoi(rangoFecha.substr(15,2))==tempPendientes->year && stoi(rangoFecha.substr(12,2))>tempPendientes->mes)
                    ||(stoi(rangoFecha.substr(6,2))==tempPendientes->year && stoi(rangoFecha.substr(3,2))==tempPendientes->mes && stoi(rangoFecha.substr(0,2))<=tempPendientes->dia && stoi(rangoFecha.substr(15,2))==tempPendientes->year && stoi(rangoFecha.substr(12,2))==tempPendientes->mes && stoi(rangoFecha.substr(9,2))>=tempPendientes->dia)
                    ||(stoi(rangoFecha.substr(6,2))==tempPendientes->year && stoi(rangoFecha.substr(3,2))<tempPendientes->mes && stoi(rangoFecha.substr(15,2))==tempPendientes->year && stoi(rangoFecha.substr(12,2))==tempPendientes->mes && stoi(rangoFecha.substr(9,2))>=tempPendientes->dia)
                    ||(stoi(rangoFecha.substr(6,2))<tempPendientes->year && stoi(rangoFecha.substr(15,2))==tempPendientes->year && stoi(rangoFecha.substr(12,2))==tempPendientes->mes && stoi(rangoFecha.substr(9,2))>=tempPendientes->dia)
                   ){

                    cout<<endl<<"   ID: "<<tempPendientes->id;
                    cout<<endl<<"   Descripcion: "<<tempPendientes->descripcion;
                    cout<<endl<<"   Nivel Importancia: "<<tempPendientes->nivelImportancia;
                    cout<<endl<<"   Fecha limite: "<<tempPendientes->dia<<"/"<<tempPendientes->mes<<"/"<<tempPendientes->year<<" a las "<<tempPendientes->hora<<" horas"<<endl;
                }
                tempPendientes=tempPendientes->sig;
            }
            string tal;
            cout<<endl<<"Digite cualquier letra para salir: ";
            getline(cin,tal);
            return;
        }
        else{
            tempPersonas=tempPersonas->sig;
        }
    }
    cout<<"La persona que busco no existe."<<endl;
    sleep(2);
    return;
}

//5 Imprimir las tareas próximo a vencer (menos de una semana) de una fecha X.
void imprimirTareasProximasVencer(Personas* listaPersonas){
    int dia, mes, year;
    cout << "Ingrese la fecha (día mes año): ";
    cin >> dia >> mes >> year;
    if (listaPersonas == NULL){
        cout << "No hay personas en la lista." << endl;
        return;
    }
    Personas* tempPersona = listaPersonas;
    bool hayTareasProximas = false;

    while (tempPersona != NULL){
        listaPendientes* tempTarea = tempPersona->tareas;

        while (tempTarea != NULL){
            int diasRestantes = calcularDiasRestantes(dia, mes, year, tempTarea->dia, tempTarea->mes, tempTarea->year);

            if (diasRestantes >= 0 && diasRestantes <= 7){
                cout << "Tarea próxima a vencer de " << tempPersona->nombre << " " << tempPersona->apellido << ":" << endl;
                cout << "----------------------------------------------";
                cout << "   Descripción: " << tempTarea->descripcion << endl;
                cout << "   Fecha de vencimiento: " << tempTarea->dia << "/" << tempTarea->mes << "/" << tempTarea->year << endl;
                cout << "   Hora: " << tempTarea->hora << endl;
                cout << "----------------------------------------------";
                hayTareasProximas = true;
            }
            tempTarea = tempTarea->sig;
        }
        tempPersona = tempPersona->sig;
    }
    if (!hayTareasProximas){
        cout << "No hay tareas próximas a vencer." << endl;
    }
    string salir;
    cout<<endl<<"Digite cualquiera tecla para salir: "<<endl;
    getline(cin,salir);
    cin.ignore(10000,'\n');
}

//6 Imprimir todas las subtareas de una tarea X de una persona Y.
void imprimirSubtareas(Personas*lista,string cedulaPersona,int idTarea,bool salir=true) {
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==cedulaPersona){
           listaPendientes*tempPendientes=tempPersonas->tareas;
           while(tempPendientes!=NULL){
                if(tempPendientes->id==idTarea){
                    avanceListas*tempAvance = tempPendientes->avance;
                    cout << "Subtareas:" << endl;
                    while (tempAvance != NULL) {
                        cout << "  - ID: " << tempAvance->idSubtarea <<endl;
                        cout << "    Nombre: "<<tempAvance->nombreTarea<<endl;
                        cout << "    Descripcion: " << tempAvance->nombreTarea <<endl;
                        cout << "    Porcentaje de avance: " << tempAvance->porcentaje << "%" <<endl;
                        cout << "    Completada: " << (tempAvance->completado ? "Si" : "No")<<endl;
                        tempAvance = tempAvance->sig;
                    }
                    if(salir){
                        string salir;
                        cout<<endl<<"Desea salir?: "<<endl;
                        getline(cin,salir);
                        return;
                    }
                    else{
                        return;
                    }

                }
                else{
                    tempPendientes=tempPendientes->sig;
                }
           }
           cout<<"No se encontro la tarea a la cual imprimir las subtareas."<<endl;
           sleep(2);
           return;
        }
        else{
            tempPersonas=tempPersonas->sig;
        }
    }
    cout<<"No se encontro la persona a la cual imprimir las subtareas."<<endl;
    sleep(2);
    return;




}

//7 Imprimir las tareas realizadas por una persona X.
void imprimirTareasRealizadasPorPersona(Personas* lista, string cedula) {
   int contador=0;
   Personas*tempPersonas=lista;
   while(tempPersonas!=NULL){
        if(tempPersonas->cedula == cedula){
            cout<<"----------Imprimiendo tareas completadas----------"<<endl;
            tareasCompletadas*tempCompletadas=tempPersonas->tareasTerminadas;
            while(tempCompletadas!=NULL){
                cout<<endl<<contador<<")   ID: "<<tempCompletadas->id<<endl;
                cout<<"     Descripcion: "<<tempCompletadas->descripcion<<endl;
                cout<<"     Cumplimiento: "<<tempCompletadas->porcentajeCumplimiento<<endl;
                cout<<"     Comentario: "<<tempCompletadas->comentario<<endl<<endl;
                tempCompletadas=tempCompletadas->sig;
                contador+=1;
            }
            string salir;
            cout<<"Presione cualquier tecla para volver: "<<endl;
            getline(cin,salir);
            return;
        }
        tempPersonas=tempPersonas->sig;
    }
    cout<<"La persona que digito no existe"<<endl;
    sleep(2);
    return;
}

//8 Imprimir las tareas realizadas al 100%.
void imprimirTareasRealizadasAl100Porciento(Personas* lista) {
    int contador=0;
    cout << "----------Tareas realizadas al 100%----------" << endl;
    Personas* persona = lista;
    while (persona != NULL) {
        tareasCompletadas* tarea = persona->tareasTerminadas;
        while (tarea != NULL) {
            if (tarea->porcentajeCumplimiento == 100) {
                cout<<endl<<contador<< ")   ID: "<<tarea->id<<endl;
                cout<<"     Descripción: " << tarea->descripcion<<endl;
                cout<<"     Comentario: "<<tarea->comentario<<endl;
            }
            tarea = tarea->sig;
            contador+=1;
        }
        persona = persona->sig;
    }
    string salir;
    cout<<"Presione cualquier tecla para volver: "<<endl;
    getline(cin,salir);
    return;
}


/*
--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS
--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS
--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS--------CARGAR DATOS
*/


//Funciones auto completadas
Personas*agregarTareaAPersonaCarga(Personas* listaPersonas,string cedula,int idTarea,string descripcion,string nivelImportancia ,int dia,int mes,int year,string hora,int idTipo){
    Personas* persona = buscarPersonas(listaPersonas, cedula);
    if (persona == NULL){
        clearScreen();
        cout << "Persona no encontrada" << endl;
        sleep(2);
        return listaPersonas;
    }
    listaPendientes* nuevaTarea = new listaPendientes(idTarea,descripcion,nivelImportancia,dia,mes,year,hora,buscarTipoTarea(listaTiposTarea1,idTipo));
    if (persona->tareas == NULL){
        persona->tareas = nuevaTarea;
    }
    else{
        persona->tareas = insertarTareaPendiente(persona->tareas, nuevaTarea);
    }
    return listaPersonas;
}

Personas*insertarSubtareaAuto(Personas*lista,string cedulaPersona,int idTarea,string nombreTarea,string comentario) {
        if(lista==NULL){
            cout<<"La lista personas esta vacia."<<endl;
            sleep(2);
            return lista;
        }
        else{
            Personas*tempPersonas=lista;
            while(tempPersonas!=NULL){
                if(tempPersonas->cedula==cedulaPersona){
                    listaPendientes*tempPendientes=tempPersonas->tareas;
                    while(tempPendientes!=NULL){
                        if(tempPendientes->id==idTarea){
                            if(tempPendientes->enlaceTipos->nombreTipoTarea=="Estudio"){
                                avanceListas*nuevoAvance= new avanceListas(nombreTarea,comentario,0,false);
                                if(tempPendientes->avance==NULL){
                                    tempPendientes->avance=nuevoAvance;
                                    return lista;
                                }
                                else{
                                    avanceListas*tempAvance=tempPendientes->avance;
                                    while(tempAvance->sig!=NULL){
                                        tempAvance=tempAvance->sig;
                                    }
                                    tempAvance->sig=nuevoAvance;
                                    return lista;
                                }
                            }
                            else{
                                cout<<"Lo sentimos, la tarea a la que se quiere subir un avance de subtareas no es de tipo Estudio"<<endl;
                                sleep(2);
                                return lista;
                            }
                        }
                        tempPendientes=tempPendientes->sig;
                    }
                    cout<<endl<<"Tarea no encontrada"<<endl;
                    sleep(2);
                    return lista;
                }
                tempPersonas=tempPersonas->sig;
            }
            cout<<"Persona no encontrada, asegurese que la cedula sea la correcta."<<endl;
            sleep(2);
            return lista;
        }
}

Personas*marcarTareaComoCompletadaAuto(Personas* persona,string cedulaPersona, int idTarea,string comentario) {
    Personas*tempPersonas=persona;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==cedulaPersona){
            listaPendientes* tarea = tempPersonas->tareas;
            while (tarea != NULL) {
                if (tarea->id == idTarea) {
                    //string comentario;
                    //cout<<"Escriba un comentario sobre la tarea: "<<endl;
                    //getline(cin,comentario);
                    if(tarea->enlaceTipos->nombreTipoTarea=="Estudio"){
                        float porcentaje=calcularAvance(persona,cedulaPersona,idTarea);
                        tareasCompletadas* nuevaTareaCompletada = new tareasCompletadas(tarea->id,tarea->descripcion,porcentaje,comentario,tarea->enlaceTipos);
                        // Agregar la nueva tarea completada a la lista
                        nuevaTareaCompletada->sig = persona->tareasTerminadas;
                        persona->tareasTerminadas = nuevaTareaCompletada;

                        // Eliminar la tarea de la lista de tareas activas
                        listaPendientes*tempPendientes=tempPersonas->tareas;
                        if(tempPendientes->sig==NULL){
                            tempPendientes=NULL;
                            return persona;
                        }
                        while(tempPendientes->sig->id!=idTarea){
                            tempPendientes=tempPendientes->sig;
                        }
                        tempPendientes->sig=tempPendientes->sig->sig;

                        return persona;
                        }
                    else{
                        tareasCompletadas* nuevaTareaCompletada = new tareasCompletadas(tarea->id,tarea->descripcion,100,comentario,tarea->enlaceTipos);
                        // Agregar la nueva tarea completada a la lista
                        nuevaTareaCompletada->sig = persona->tareasTerminadas;
                        persona->tareasTerminadas = nuevaTareaCompletada;

                        // Eliminar la tarea de la lista de tareas activas
                        listaPendientes*tempPendientes=tempPersonas->tareas;
                        if(tempPendientes->sig==NULL){
                            tempPendientes=NULL;
                            return persona;
                        }
                        while(tempPendientes->sig->id!=idTarea){
                            tempPendientes=tempPendientes->sig;
                        }
                        tempPendientes->sig=tempPendientes->sig->sig;

                        return persona;
                    }
                }
                tarea = tarea->sig;
            }
            throw 505;
        }
        else{
            tempPersonas=tempPersonas->sig;
        }
    }
    // Si no se encontró la tarea, mostrar un mensaje de error
    cout << "Persona no fue encontrada" <<endl;
    return persona;
}

//Carga los datos
void cargarDatos(){
    //Insertando tipos tarea
    listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,0,"Estudio","Tareas, estudiar y hacer proyectos del tec.");
    listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,1,"quehaceres del hogar","Limpiar la casa, lavar los platos, lavar la ropa, etc.");
    listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,2,"Pago de impuestos","Pagar el agua, la luz, el internet, etc.");
    listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,3,"Deportes","Gym y entrenar con el equipo.");
    listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,4,"Vida y salud","Citas con la odontologa, medico y actividades recreativas.");
    //Insertando personas
    listaPersonas1=insertarPersonas(listaPersonas1,"Ryan","Vargas","345-214",18);
    listaPersonas1=insertarPersonas(listaPersonas1,"Aaron","Araya","123-456",2);
    listaPersonas1=insertarPersonas(listaPersonas1,"Daniel","Barrantes","672-192",100);
    listaPersonas1=insertarPersonas(listaPersonas1,"Vegeta","777","777-777",777);
    listaPersonas1=insertarPersonas(listaPersonas1,"Mario","Mario","644-633",64);
    //Insertando tareas
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"345-214",0,"Terminar el proyecto de datos (denos un 100 profe =) )","Alta",24,9,24,"23:45",0);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"345-214",1,"Terminar de limpiar el cuarto","Baja",25,10,24,"23:45",1);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"345-214",2,"Hacer ejercicio","Media",22,9,24,"23:45",3);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",0,"Pagar a los editores","Alta",30,9,24,"12:00",2);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",1,"Terminar la tarea","Alta",26,10,24,"13:00",0);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",2,"Pagar el internet","Media",15,3,26,"15:00",2);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",3,"Hacer ejercicio","Baja",5,1,23,"6:00",3);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",4,"Otra tarea","Media",31,5,20,"17:00",0);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",5,"Jugar minecraft","Alta",6,12,24,"18:00",4);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",6,"Descansar","Baja",1,1,19,"12:00",4);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"777-777",7,"Limpiar el carro","Media",12,12,26,"20:00",1);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"123-456",0,"Cita con el medico","Media",12,11,24,"13:00",4);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"123-456",1,"Jugar videojuegos","Media",25,1,25,"13:00",4);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"672-192",0,"Limpiar la cocina","Baja",30,12,30,"23:45",1);
    listaPersonas1=agregarTareaAPersonaCarga(listaPersonas1,"644-633",0,"Salvar a la princesa","Alta",12,12,24,"23:45",1);
    //Insertando subtareas
    /*
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Terminar las listas","Ya casi");
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Completar las funciones de Actualización de información","Ya casi");
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Completar las funciones de Consultas","Ya casi");
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Completar las funciones de Reportes","Ya casi");
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Terminar la documentacion","Ya casi");
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Arreglar bugs","Ya casi");
    listaPersonas1=insertarSubtareaAuto(listaPersonas1,"345-214",0,"Suplicarle a la profe por un 100%","Ya casi");
    */
    //Insertanto tareas completadas
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",1,"Facil");
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",2,"Que caro");
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",3,"Bien");
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",4,"Facil por 2");
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",5,"Bonito");
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",6,"Estoy cansado jefe");
    listaPersonas1=marcarTareaComoCompletadaAuto(listaPersonas1,"777-777",7,"Listo");







}




/*
--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN
--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN
--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN--------MAIN
*/

int main(){
    int opcion1=0;
    int opcion2=0;
    int idTipos=0;
    bool Carga=false;
    while(true){
        system("cls");
        opcion1=0;
        cout<<"-----Gestor de Tareas-----"<<endl;
        cout<<endl<<endl<<"Saludos Usuario. Que deseas hacer?" << endl
            <<endl<<"Actualizar Informacion (1)"
            <<endl<<"Consultas (2)"
            <<endl<<"Reportes (3)"
            <<endl<<"Cargar datos quemados(4)"
            <<endl<<endl<<"Digite su respuesta: ";
        cin>>opcion1;
        cin.ignore(10000,'\n');
        switch(opcion1){
            case 1:{ //listas
                bool salir=false;
                while(true){
                    if(salir==true){
                        break;
                    }
                    system("cls");
                    opcion2=0;
                    cout<<"----------Actualizar----------"<<endl;
                    cout<<"Bienvenido a Actualizar. Dale ponte al día."
                    <<endl<<endl<<"Insertar tipos de tareas (1)"
                    <<endl<<"Insertar personas a la lista (2)"
                    <<endl<<"Borrar personas de la lista (3)"
                    <<endl<<"Insertar tareas (4)"
                    <<endl<<"Modificar tareas activas (5)"
                    <<endl<<"Borrar tareas activa (6)"
                    <<endl<<"Insertar Subtareas (7)"
                    <<endl<<"Modificar el avance de una subtarea (8)"
                    <<endl<<"Completar una tarea (9)"
                    <<endl<<"Salir (10)"<<endl<<endl<<"Digite su respuesta: ";
                    cin>>opcion2;
                    cin.ignore(10000,'\n');
                    switch(opcion2){
                        case 1:{ //Insertar tipos de tareas (1)
                            clearScreen();
                            string nombre;
                            string descripcion;
                            TiposTarea * tipo;
                            cout<<"Digite el nombre de la tarea: ";
                            getline (cin,nombre);
                            cout<<endl<<"Escriba una descripcion de la tarea: ";
                            getline (cin,descripcion);
                            listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,idTipos,nombre,descripcion);
                            idTipos+=1;
                            system("cls");
                            cout<<endl<<"Nueva tarea insertada con exito."<<endl;
                            sleep(2);
                            continue;
                        }
                        case 2:{ //Insertar personas a la lista (2)
                            while(true){
                                clearScreen();
                                string nombre;
                                string apellido;
                                string cedula;
                                int edad;
                                cout<<"Digite el nombre de la persona a insertar: ";
                                getline(cin,nombre);
                                cout<<endl<<"Digite el apellido: ";
                                getline(cin,apellido);
                                cout<<endl<<"Digite la cedula (El formato de la cedula debe ser 123-456): ";
                                getline(cin,cedula);
                                if(comprobarCedulasPersonas(listaPersonas1,cedula)==true){
                                    cout<<endl<<"Digite la edad: ";
                                    cin>>(edad);
                                    cin.ignore(10000,'\n');
                                    listaPersonas1=insertarPersonas(listaPersonas1,nombre,apellido,cedula,edad);
                                    system("cls");
                                    cout<<"Nueva persona insertada con exito."<<endl;
                                    sleep(2);
                                    break;
                                }
                                else{
                                    cout<<"El formato de la cedula insertada es invalida, o por el contrario esta repetida en la lista."<<endl;
                                    sleep(2);
                                    cin.ignore(10000,'\n');
                                    continue;
                                }

                            }
                            continue;
                        }
                        case 3:{ //Borrar personas de la lista (3)
                            clearScreen();
                            if(listaPersonas1==NULL){
                                cout<<"La lista de personas esta vacia. Volviendo al menu"<<endl;
                                sleep(2);
                                continue;
                            }
                            else{
                                string personaBorrar;
                                imprimirPersonas(listaPersonas1);
                                cout<<endl<<"Digite la cedula de la persona a borrar: ";
                                getline(cin,personaBorrar);
                                listaPersonas1=borrarPersonas(listaPersonas1,personaBorrar);
                                continue;
                            }
                        }
                        case 4:{ //Insertar tareas (4)
                            clearScreen();
                            agregarTareaAPersona(listaPersonas1, listaTiposTarea1);
                            clearScreen();
                            sleep(2);
                            continue;
                        }
                        case 5:{ //Modificar tareas activas (5)
                            clearScreen();
                            imprimirPersonas(listaPersonas1);
                            reprogramarTareasDePersonas(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 6:{ //Borrar tareas activa (6)
                            clearScreen();
                            imprimirPersonas(listaPersonas1);
                            borrarTareaDePersona(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 7:{ //Insertar Subtareas (7)
                            while(true){
                                if(listaPersonas1==NULL){
                                    cout<<"Lo sentimos la lista esta vacia"<<endl;
                                    sleep(2);
                                    break;
                                }
                                clearScreen();
                                string cedula;
                                imprimirPersonas(listaPersonas1);
                                cout<<"Digite la cedula de la persona que quiera insertarle una subtarea (Formato de la cedula: 123-456): ";
                                getline(cin,cedula);
                                if(comprobarCedulasPersonas(listaPersonas1,cedula,true)){
                                    while(true){
                                        clearScreen();
                                        Personas*temp=buscarPersonas(listaPersonas1,cedula);
                                        imprimirTareasDePersona(temp);
                                        int idTarea;
                                        cout<<endl<<"Escriba la id de la tarea a la cual se le quiere insertar subtareas (Recuerde que solo es posible insertar subtareas a tareas de tipo Estudio): ";
                                        cin>>idTarea;
                                        cin.ignore(10000,'\n');
                                        listaPendientes*tempPendientes=buscarTarea(temp,idTarea);
                                        if(tempPendientes->enlaceTipos->nombreTipoTarea=="Estudio"){
                                            string nombreTarea;
                                            string comentario;
                                            cout<<endl<<"Escriba el nombre de la subtarea: ";
                                            getline(cin,nombreTarea);
                                            cout<<endl<<"Escriba el comentario de la subtarea: ";
                                            getline(cin,comentario);
                                            insertarSubtarea(listaPersonas1,cedula,idTarea,nombreTarea,comentario);
                                            cout<<endl<<"Subtarea insertada con exito"<<endl;
                                            sleep(2);
                                            break;
                                        }
                                        else{
                                            clearScreen();
                                            cout<<"Lo sentimos, la id digitada no es de tipo Estudio, vuelva a intentarlo.";
                                            sleep(2);
                                        }
                                    }
                                    break;
                                }
                                else{
                                    clearScreen();
                                    cout<<endl<<"El formato de la cedula dada es incorrecta"<<endl;
                                    sleep(2);
                                }
                            }
                            continue;
                        }
                        case 8:{ //Modificar el avance de una subtarea (8)
                            while(true){
                                if(listaPersonas1==NULL){
                                    cout<<"Lo sentimos, la lista esta vacia"<<endl;
                                    sleep(2);
                                    break;
                                }
                                clearScreen();
                                string cedula;
                                imprimirPersonas(listaPersonas1);
                                cout<<"Porfavor digite la cedula de la persona a modificar su subtarea (El formato de la cedula es 123-456): "<<endl;
                                getline(cin,cedula);
                                if(comprobarCedulasPersonas(listaPersonas1,cedula,true)){
                                    while(true){
                                        int idTarea;
                                        clearScreen();
                                        Personas*temp=buscarPersonas(listaPersonas1,cedula);
                                        imprimirTareasDePersona(temp);
                                        cout<<endl;
                                        cout<<"Porfavor, digite la id de la tarea a la cual se le quiere modificar sus subtareas (Recuerde que solo las tareas de tipo Estudio tienen subtareas):"<<endl;
                                        cin>>idTarea;
                                        cin.ignore(10000,'\n');
                                        listaPendientes*tempPendientes=buscarTarea(temp,idTarea);
                                        if(tempPendientes->enlaceTipos->nombreTipoTarea=="Estudio"){
                                            int idSubTarea;
                                            int nuevoAvance;
                                            clearScreen();
                                            imprimirSubtareas(listaPersonas1,cedula,idTarea,false);
                                            cout<<endl<<"Ahora digite la id de la subTarea a modificar: "<<endl;
                                            cin>>idSubTarea;
                                            cin.ignore(10000,'\n');
                                            cout<<endl<<"Por ultimo, digite el nuevo avance de la tarea: "<<endl;
                                            cin>>nuevoAvance;
                                            cin.ignore(10000,'\n');
                                            modificarSubtarea(listaPersonas1,cedula,idTarea,idSubTarea,nuevoAvance);
                                            string salir;
                                            clearScreen();
                                            imprimirSubtareas(listaPersonas1,cedula,idTarea,false);
                                            cout<<endl<<"La modificacion se realizo sin problemas. Desea salir?"<<endl;
                                            getline(cin,salir);
                                        }
                                        else{
                                            clearScreen();
                                            cout<<"Lo sentimos, la id digitada no corresponde al tipo Estudio, por favor vuelva a intentarlo"<<endl;
                                            sleep(2);
                                        }
                                    }
                                    break;
                                }
                                else{
                                    clearScreen();
                                    cout<<"Lo sentimos, el formato de la cedula es incorrecta, por favor intentelo de nuevo."<<endl;
                                    sleep(2);
                                }
                            }
                            continue;
                        }
                        case 9:{ //Completar una tarea (9)
                            while(true){
                                if(listaPersonas1==NULL){
                                    clearScreen();
                                    cout<<"Lista vacia, volviendo al menu."<<endl;
                                    sleep(2);
                                    break;
                                }
                                clearScreen();
                                string cedula;
                                imprimirPersonas(listaPersonas1);
                                cout<<endl<<"Digite la cedula de la persona a completar una de sus tareas: "<<endl;
                                getline(cin,cedula);
                                if(comprobarCedulasPersonas(listaPersonas1,cedula,true)){
                                    while(true){
                                        clearScreen();
                                        int idTarea;
                                        Personas*temp=buscarPersonas(listaPersonas1,cedula);
                                        imprimirTareasDePersona(temp);
                                        cout<<"Digite la id de la tarea que va a completar: "<<endl;
                                        cin>>idTarea;
                                        cin.ignore(10000,'\n');
                                        try{
                                            clearScreen();
                                            marcarTareaComoCompletada(listaPersonas1,cedula,idTarea);
                                            cout<<"La tarea fue completada con exito"<<endl;
                                            sleep(2);
                                            break;
                                        }
                                        catch (int myNum){
                                            clearScreen();
                                            cout<<"Error, la id fue mal dada"<<endl;
                                            sleep(2);
                                        }
                                    }
                                    break;
                                }
                                else{
                                    clearScreen();
                                    cout<<"La cedula digitada es incorrecta, por favor vuelva a intentarlo."<<endl;
                                    sleep(2);
                                }

                            }
                            continue;
                        }
                        case 10:{ //Salir (10)
                            salir=true;
                            continue;
                        }
                    }
                }
                continue;
            }
            case 2:{ //Consultas
                bool salir=false;
                while(true){
                    system("cls");
                    if(salir==true){
                        break;
                    }
                    cout<<"----------Consultas----------"<<endl<<endl;
                    int opcion4;
                    cout<<"Binvenidos a consultas. Elija una opcion: "<<endl;
                    cout<<"Imprimir la persona con mas tareas activas (1)"<<endl;
                    cout<<"Imprimir una persona con mas tareas activas de un determinado tipo (2)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun (3)"<<endl;
                    cout<<"Imprimir la persona con mas tareas vencidas de un determinado tipo (4)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun a vencer (5)"<<endl;
                    cout<<"Imprimir el tipo de importancia mas comun en la lista personas (6)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun con importancia media en la lista persona (7)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun realizadas con una importancia alta (8)"<<endl;
                    cout<<"Salir (9)"<<endl<<endl;
                    cout<<"Digite su respuesta: "<<endl;
                    cin>>opcion4;
                    cin.ignore(10000,'\n');
                    switch(opcion4){
                        case 1:{ //Imprimir la persona con mas tareas activas (1)
                            clearScreen();
                            personaConMasTareasActivas(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 2:{ //Imprimir una persona con mas tareas activas de un determinado tipo (2)
                            clearScreen();
                            personaConMasTareasPorTipo(listaPersonas1, listaTiposTarea1);
                            sleep(2);
                            continue;
                        }
                        case 3:{ //Imprimir el tipo de tarea mas comun (3)
                            clearScreen();
                            tipoDeTareaMasComun(listaPersonas1, listaTiposTarea1);
                            sleep(2);
                            continue;
                        }
                        case 4:{ //Imprimir la persona con mas tareas vencidas de un determinado tipo (4)
                            while(true){
                                clearScreen();
                                int tipoTarea;
                                imprimirTiposTarea(listaTiposTarea1);
                                cout<<endl<<"Digite la id del tipo tarea: "<<endl;
                                cin>>tipoTarea;
                                cin.ignore(10000,'\n');
                                if(buscarTipoTarea(listaTiposTarea1,tipoTarea)==NULL){
                                    clearScreen();
                                    cout<<"La id digitada no existe, por favor intente de nuevo."<<endl;
                                    sleep(2);
                                }
                                else{
                                    clearScreen();
                                    int dia,mes,year;
                                    string hora;
                                    cout<<"Cual es la fecha limite? (formato: dia mes año, con espacios y sin nada entre): "<<endl;
                                    cin>>dia>>mes>>year;
                                    cin.ignore(10000,'\n');
                                    cout<<endl<<"Y la hora? (Formato 00:00): ";
                                    getline(cin,hora);
                                    contarTareasVencidasPorTipo(listaPersonas1,tipoTarea,dia,mes,year,hora);
                                    break;
                                }
                            }
                            continue;
                        }
                        case 5:{ //Imprimir el tipo de tarea mas comun a vencer (5)
                            while(true){
                                clearScreen();
                                int dia,mes,year;
                                string hora;
                                cout<<"Cual es la fecha limite? (formato: dia mes año, con espacios y sin nada entre): "<<endl;
                                cin>>dia>>mes>>year;
                                cin.ignore(10000,'\n');
                                cout<<endl<<"Y la hora? (Formato 00:00): ";
                                getline(cin,hora);
                                tipoTareaMasComunVencida(listaPersonas1,dia,mes,year,hora);
                                break;
                            }
                            continue;
                        }
                        case 6:{ //Imprimir el tipo de importancia mas comun en la lista personas (6)
                            while(true){
                                string exit;
                                clearScreen();
                                importanciaMasComunListaPendientes(listaPersonas1);
                                cout<<endl<<"Presione cualquier tecla para salir: "<<endl;
                                getline(cin,exit);
                                break;
                            }
                            continue;
                        }
                        case 7:{ //Imprimir el tipo de tarea mas comun con importancia media en la lista persona (7)
                            while(true){
                                string exit;
                                clearScreen();
                                tipoTareaActivasMasComunEnImportanciaMedia(listaPersonas1);
                                cout<<endl<<"Presione cualquier tecla para salir: "<<endl;
                                getline(cin,exit);
                                break;
                            }
                            continue;
                        }
                        case 8:{ //Imprimir el tipo de tarea mas comun realizadas con una importancia alta (8)
                            while(true){
                                string exit;
                                clearScreen();
                                tipoTareaRealizadasMasComunEnImportanciaAlta(listaPersonas1);
                                cout<<endl<<"Presione cualquier tecla para salir: "<<endl;
                                getline(cin,exit);
                                break;
                            }
                            continue;
                        }
                        case 9:{ //Salir (9)
                            salir=true;
                            continue;
                        }
                    }

                }
                continue;
            }
            case 3:{ //Reportes
                bool salir=false;
                while(true){
                    system("cls");
                    if(salir==true){
                        break;
                    }
                    int opcion3;
                    cout<<"-----Reportes-----"<<endl<<endl<<"-Bienvenidos a Reportes. Elija una opcion: "
                    <<endl<<endl<<"Imprimir la lista de tipos de tareas(1)"
                    <<endl<<"Imprimir la lista de personas (2)"
                    <<endl<<"Imprimir las personas sin tarea (3)"
                    <<endl<<"Imprimir las tareas activas de una persona X (Por fecha) (4)"
                    <<endl<<"Imprimir las tareas a vencer (5)"
                    <<endl<<"Imprimir las subtareas de una tarea (6)"
                    <<endl<<"Imprimir las tareas realizadas por X persona (7)"
                    <<endl<<"Imprimir las tareas realizadas (todas) (8)"
                    <<endl<<"Salir (9)"
                    <<endl<<endl<<"Digite su respuesta: ";
                    cin>>opcion3;
                    cin.ignore(10000,'\n');
                    switch(opcion3){
                        case 1:{ //Imprimir la lista de tipos de tareas(1)
                            clearScreen();
                            char nada;
                            imprimirTiposTarea(listaTiposTarea1);
                            cout<<endl<<"Para volver digite cualquier tecla: ";
                            cin>>nada;
                            cin.ignore(10000,'\n');
                            continue;
                        }
                        case 2:{ //Imprimir la lista de personas (2
                            clearScreen();
                            char nada;
                            imprimirPersonas(listaPersonas1);
                            cout<<endl<<"Para volver digite cualquier tecla: ";
                            cin>>nada;
                            cin.ignore(10000,'\n');
                            continue;
                        }
                        case 3:{ //Imprimir las personas sin tarea (3)
                            clearScreen();
                            imprimirPersonasSinTareas(listaPersonas1);
                            sleep(2);
                            continue;
                            }
                        case 4:{ //Imprimir las tareas activas de una persona X (Por fecha) (4)
                            while(true){
                                clearScreen();
                                string cedula;
                                string rangoFecha;
                                imprimirPersonas(listaPersonas1);
                                cout<<"Introduce la cedula de la persona: "<<endl;
                                getline(cin,cedula);
                                cout<<"Introduce el rango de fecha a buscar(El formato es 01/01/01-02/02/02): "<<endl;
                                getline(cin,rangoFecha);
                                if(comprobarFormatoFecha(rangoFecha)==true){
                                    imprimirTareasActivasFecha(listaPersonas1,cedula,rangoFecha);
                                    break;
                                }
                                else{
                                    cout<<"Lo sentimos, el formato de la fecha es incorrecta."<<endl;
                                    sleep(2);
                                }

                            }
                            continue;
                        }
                        case 5:{ //Imprimir las tareas a vencer (5)
                            clearScreen();
                            imprimirTareasProximasVencer(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 6:{ //Imprimir las subtareas de una tarea (6)
                            while(true){
                                if(listaPersonas1==NULL){
                                    cout<<"La lista esta vacia, volviendo"<<endl;
                                    sleep(2);
                                    break;
                                }
                                clearScreen();
                                string cedula;
                                imprimirPersonas(listaPersonas1);
                                cout<<"Escriba la cedula de la persona a la cual quiere imprimir sus subtareas (El formato es 123-456): ";
                                getline(cin,cedula);
                                if(comprobarCedulasPersonas(listaPersonas1,cedula,true)){
                                    while(true){
                                        clearScreen();
                                        Personas*temp=buscarPersonas(listaPersonas1,cedula);
                                        imprimirTareasDePersona(temp);
                                        int idTarea;
                                        cout<<endl<<"Digite la id de la tarea a la cual se le quiere imprimir las subtareas (Recuerde que solo las tareas de tipo Estudios tiene subtareas): ";
                                        cin>>idTarea;
                                        cin.ignore(10000,'\n');
                                        listaPendientes*tempPendientes=buscarTarea(temp,idTarea);
                                        if(tempPendientes->enlaceTipos->nombreTipoTarea=="Estudio"){
                                            clearScreen();
                                            imprimirSubtareas(listaPersonas1,cedula,idTarea);
                                            break;
                                        }
                                        else{
                                            clearScreen();
                                            cout<<endl<<"Lo sentimos, la id que digito no corresponde a un tipo Estudio, por favor vuelva a intentarlo."<<endl;
                                            sleep(2);
                                        }
                                    }
                                    break;
                                }
                                else{
                                    clearScreen();
                                    cout<<endl<<"Lo sentimos, el formato de la cedula es incorrecta."<<endl;
                                    sleep(2);
                                }
                            }
                            continue;

                        }
                        case 7:{ //Imprimir las tareas realizadas por X persona (7)
                            while(true){
                                clearScreen();
                                string cedula;
                                imprimirPersonas(listaPersonas1);
                                cout<<"Digite la cedula de la persona a imprimir sus tareas completadas: "<<endl;
                                getline(cin,cedula);
                                if(comprobarCedulasPersonas(listaPersonas1,cedula,true)){
                                    imprimirTareasRealizadasPorPersona(listaPersonas1,cedula);
                                    break;

                                }
                                else{
                                    clearScreen();
                                    cout<<"Lo sentimos, la cedula insertada es incorrecta."<<endl;
                                    sleep(2);
                                }
                            }
                            continue;

                        }
                        case 8:{ //Imprimir las tareas realizadas (todas) (8)
                            while(true){
                                clearScreen();
                                imprimirTareasRealizadasAl100Porciento(listaPersonas1);
                                break;
                            }
                            continue;
                        }
                        case 9:{ //Salir (9)
                            salir=true;
                            continue;
                        }
                    }
                }
                continue;

            }
            case 4:{ //Cargar datos
                if(Carga==false){
                    clearScreen();
                    cargarDatos();
                    cout<<"Carga terminada, volviendo al menu."<<endl;
                    Carga=true;
                }
                else{
                    clearScreen();
                    cout<<"Ya se cargo los datos anteriormente, volviendo al menu."<<endl;
                }
                sleep(2);
                continue;
            }
        }
    }
}



