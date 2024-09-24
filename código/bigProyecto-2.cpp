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

using namespace std;

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

struct listaPendientes{ // id / descripcion / nivelImportancia / dia / mes / year / hora ---- lista sencilla ---- enlace a TiposTarea (enlaceTipos) y avanceListas (avance)
    int id;
    string descripcion;
    string nivelImportancia;
    //fecha
    int dia;
    int mes;
    int year;
    string hora;
    listaPendientes*sig;
    TiposTarea* enlaceTipos;
    struct avanceListas*avance; // solo para los tipos estudio

    listaPendientes(int _id,string desc, string nivel,int d,int m, int a, string ho, TiposTarea* tareas){
        id=_id;
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

struct avanceListas{ // string nombreTarea / string comentario / int porcentaje / bool completado ---- lista simple
    string nombreTarea;
    string comentario;
    int porcentaje;
    bool completado;
    avanceListas*sig;

    avanceListas(string nom, string com, int por, bool comple){
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

    tareasCompletadas(int i, string des, int por, string com){
        id=i;
        descripcion=des;
        porcentajeCumplimiento=por;
        comentario=com;
    }
};

void clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

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
                lista->sig=nueva;
                nueva->ant=lista;
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

bool comprobarCedulasPersonas(Personas*lista,string cedula){ //Función encargada de buscar cédulas repetidas en la lista Personas, si encuentra impide la inserción de la persona, también se encarga de ver que el formato de la cédula sea correcta.
        if(cedula[3]=='-'){
            try{
                int primero=stoi(cedula.substr(0,3));
                int segundo=stoi(cedula.substr(4,6));
                if(cedula.size()==7){
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

void agregarTareaAPersona(Personas* listaPersonas, TiposTarea* listaTipos, int &id){
    string cedula;
    clearScreen();
    cout << "Ingrese la cédula de la persona: ";
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
    imprimirTiposTarea(listaTipos);
    cout << "Ingrese el id del tipo de tarea de tu tarea: ";
    cin >> idTipo;
    TiposTarea* tipoTarea = buscarTipoTareaPorID(listaTipos, idTipo);
    if (tipoTarea == NULL){
        clearScreen();
        cout << "Tipo de tarea no existe" << endl;
        sleep(2);
        return;
    }

    listaPendientes* nuevaTarea = new listaPendientes(id, descripcion, nivelImportancia, dia, mes, year, hora, tipoTarea);
    id ++;
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

void reprogramarTareasDePersonas(Personas* listaPersonas){
    string cedula;
    cout << "Ingrese la cédula de la persona: ";
    cin >> cedula;

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
    
    listaPendientes* tarea = persona->tareas;
    while (tarea != NULL){
        if (tarea->id == idTarea){
            reprogramarTarea(tarea);
            return;
        }
    }

    cout << "Tarea no encontrada" << endl;
}

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

void personaConMasTareasPorTipo(Personas* listaPersonas, TiposTarea* listaTipos) {
    if (listaPersonas == NULL) {
        cout << "No hay personas en la lista." << endl;
        return;
    }

    int idTipo;
    imprimirTiposTarea(listaTipos);
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
}

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
    cout<<"La tarea mas comun con importancia media es: "<<endl<<buscarTipoTarea(listaTiposTarea1,idMasComun)->nombreTipoTarea<<endl<<" con la id: "<<buscarTipoTarea(listaTiposTarea1,idMasComun)->idTipoTarea<<endl;
    sleep(2);
    return;
}

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

void imprimirTareasActivasFecha(Personas*lista,string cedula,string rangoFecha){ //Imprime una serie de tareas activas de una persona X por fecha
    Personas*tempPersonas=lista;
    while(tempPersonas!=NULL){
        if(tempPersonas->cedula==cedula){
            listaPendientes*tempPendientes=tempPersonas->tareas;
            while(tempPendientes!=NULL){
                if(stoi(rangoFecha.substr(0,1))<=tempPendientes->dia
                        && stoi(rangoFecha.substr(3,4))<=tempPendientes->mes
                        && stoi(rangoFecha.substr(6,7))<=tempPendientes->year
                        && stoi(rangoFecha.substr(9,10))>=tempPendientes->dia
                        && stoi(rangoFecha.substr(12,13))>=tempPendientes->mes
                        && stoi(rangoFecha.substr(15,16))>=tempPendientes->year){

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

bool comprobarFormatoFecha(string rangoFecha){ //Función encargada de comprobar que el formato de la fecha sea correcta. Esta función se usa junto a "imprimirTareasActivasFecha"
    if(rangoFecha[8]=='-'){
        try{
            if(stoi(rangoFecha.substr(0,1))>32){
                return false;
            }
            if(stoi(rangoFecha.substr(3,4))>12){
                return false;
            }

            int primero=stoi(rangoFecha.substr(6,7));

            if(stoi(rangoFecha.substr(9,10))>32){
                return false;
            }
            if(stoi(rangoFecha.substr(12,13))>12){
                return false;
            }

            int segundo=stoi(rangoFecha.substr(15,16));

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

int main(){
    int opcion1=0;
    int opcion2=0;
    int idTipos=0;
    int idD = 1;
    while(true){
        system("cls");
        opcion1=0;
        cout<<"-----Gestor de Tareas-----"<<endl;
        cout<<endl<<endl<<"Saludos Metódico. ¿Qué deseas hacer?" << endl
            <<endl<<"Actualizar Información (1)"
            <<endl<<"Consultas (2)"
            <<endl<<"Reportes (3)"
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
                        case 1:{
                            system("cls");
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
                        case 2:{
                            while(true){
                                system("cls");
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
                        case 3:{
                            system("cls");
                            if(listaPersonas1==NULL){
                                cout<<"La lista de personas esta vacia. Volviendo al menu"<<endl;
                                sleep(2);
                                continue;
                            }
                            else{
                                string personaBorrar;
                                cout<<"Digite la cedula de la persona a borrar: ";
                                getline(cin,personaBorrar);
                                listaPersonas1=borrarPersonas(listaPersonas1,personaBorrar);
                                continue;
                            }
                        }
                        case 4:{
                            clearScreen();
                            agregarTareaAPersona(listaPersonas1, listaTiposTarea1, idD);
                            clearScreen();
                            sleep(2);
                            continue;
                        }
                        case 5:{
                            clearScreen();
                            reprogramarTareasDePersonas(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 6:{
                            clearScreen();
                            borrarTareaDePersona(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 10:{
                            salir=true;
                            continue;
                        }
                    }
                }
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
                        case 1:{
                            clearScreen();
                            personaConMasTareasActivas(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 2:{
                            clearScreen();
                            personaConMasTareasPorTipo(listaPersonas1, listaTiposTarea1);
                            sleep(2);
                            continue;
                        }
                        case 3:{
                            clearScreen();
                            tipoDeTareaMasComun(listaPersonas1, listaTiposTarea1);
                            sleep(2);
                            continue;
                        }
                        case 6:{
                            while(true){
                                string exit;
                                system("cls");
                                importanciaMasComunListaPendientes(listaPersonas1);
                                cout<<endl<<"Presione cualquier tecla para salir: "<<endl;
                                getline(cin,exit);
                                break;
                            }
                            continue;
                        }
                        case 7:{
                            while(true){
                                string exit;
                                system("cls");
                                tipoTareaActivasMasComunEnImportanciaMedia(listaPersonas1);
                                cout<<endl<<"Presione cualquier tecla para salir: "<<endl;
                                getline(cin,exit);
                                break;
                            }
                            continue;
                        }
                        case 8:{
                            while(true){
                                string exit;
                                system("cls");
                                tipoTareaRealizadasMasComunEnImportanciaAlta(listaPersonas1);
                                cout<<endl<<"Presione cualquier tecla para salir: "<<endl;
                                getline(cin,exit);
                                break;
                            }
                            continue;
                        }

                        case 9:{
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
                    <<endl<<"Imprimir la lista de personas(2)"
                    <<endl<<"Imprimir las personas sin tarea (3)"
                    <<endl<<"Imprimir las tareas activas(4)"
                    <<endl<<"Imprimir las tareas a vencer (5)"
                    <<endl<<"Imprimir las subtareas de una tarea (6)"
                    <<endl<<"Imprimir las tareas realizadas (7)"
                    <<endl<<"Imprimir las tareas realizadas (8)"
                    <<endl<<"Salir (9)"
                    <<endl<<endl<<"Digite su respuesta: ";
                    cin>>opcion3;
                    cin.ignore(10000,'\n');
                    switch(opcion3){
                        case 1:{
                            system("cls");
                            char nada;
                            imprimirTiposTarea(listaTiposTarea1);
                            cout<<endl<<"Para volver digite cualquier tecla: ";
                            cin>>nada;
                            cin.ignore(10000,'\n');
                            continue;
                        }
                        case 2:{
                            system("cls");
                            char nada;
                            imprimirPersonas(listaPersonas1);
                            cout<<endl<<"Para volver digite cualquier tecla: ";
                            cin>>nada;
                            cin.ignore(10000,'\n');
                            continue;
                        }
                        case 3:{
                            clearScreen();
                            imprimirPersonasSinTareas(listaPersonas1);
                            sleep(2);
                            continue;
                            }
                        case 4:{
                            while(true){
                                system("cls");
                                string cedula;
                                string rangoFecha;
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
                        case 5:{
                            clearScreen();
                            imprimirTareasProximasVencer(listaPersonas1);
                            sleep(2);
                            continue;
                        }
                        case 9:{
                            salir=true;
                            continue;
                        }
                    }
                }
                continue;

            }

        }
    }




}


