#include <iostream>
#include <stack>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <map>
#include <ctime>

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
        enlaceTipos=NULL;
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

void agregarTareaPersona(Personas* lista, string cedula, int id,
                         string descripcion, string nivelImportancia,
                         int dia, int mes, int año, string hora, int idTipoTarea){
    //buscar persona
    Personas* persona=buscarPersonas(lista,cedula);
    if(persona!=NULL){
        //buscar tipo de tarea
        TiposTarea* tipoTarea=buscarTipoTarea(listaTiposTarea1,idTipoTarea);
        if(tipoTarea==NULL){
            cout << "Tipo de tarea inexistente" << endl;
            return;
        }
        //agregar tarea
        listaPendientes* nuevaTarea = new listaPendientes(id, descripcion, nivelImportancia, dia, mes, año, hora, tipoTarea);

        // si la lista de tareas está vacía, agregar tarea
        if (persona->tareas == NULL){
            persona->tareas = nuevaTarea;
        }
        else{
            listaPendientes* temp = persona->tareas;
            listaPendientes* ant = NULL;
            // encontrar posicion correcta
            while (temp != NULL){
                if ((temp->year > año) ||
                   (temp->year == año && temp->mes > mes) ||
                   (temp->year == año && temp->mes == mes && temp->dia > dia) ||
                   (temp->year == año && temp->mes == mes && temp->dia == dia && temp->hora > hora)) {
                    break; // Encontró la posición para insertar
                }
                ant = temp;
                temp = temp->sig;
            }
            // insertar tarea
            if (ant == NULL){
                nuevaTarea->sig = persona->tareas;
                persona->tareas = nuevaTarea;
            }
            else{
                nuevaTarea->sig = temp;
                ant->sig = nuevaTarea;
            }
        }
    }
    else{
        cout<<"Persona no encontrada"<<endl;
    }
}

void modificarTareaPersona(Personas* lista, string cedula, int id, int nuevoDia, int nuevoMes, int nuevoAño, string nuevaHora){
    //buscar persona
    Personas* persona=buscarPersonas(lista,cedula);
    if(persona!=NULL){
        //buscar tarea
        listaPendientes* tarea = persona->tareas;
        while (tarea != NULL){
            if (tarea->id == id){
                //Modificar fecha y hora
                tarea->dia = nuevoDia;
                tarea->mes = nuevoMes;
                tarea->year = nuevoAño;
                tarea->hora = nuevaHora;
                cout << "Tarea reprogramada con éxito"<< endl;
                return;
            }
            tarea = tarea->sig;
        }
        cout << "Tarea no encontrada"<< endl;
    }
    else{
        cout<<"Persona no encontrada"<<endl;
    }
}

void borrarTareaPersona(Personas* lista, string cedula, int id){
    //buscar persona
    Personas* persona=buscarPersonas(lista,cedula);
    if(persona!=NULL){
        listaPendientes* tarea = persona->tareas;
        listaPendientes* ant = NULL;
        //buscar tarea con id
        while (tarea != NULL){
            if (tarea->id == id){
                //borrar tarea
                if (ant == NULL){
                    persona->tareas = tarea->sig;
                }
                else{
                    ant->sig = tarea->sig;
                }
                cout << "Tarea eliminada con éxito"<< endl;
                return;
            }
            ant = tarea;
            tarea = tarea->sig;
        }
        cout << "Tarea no encontrada"<< endl;
    }
    else{
        cout<<"Persona no encontrada"<<endl;
    }
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

int contarTareasActivas(listaPendientes* tareas){
    int contador = 0;
    listaPendientes* temp = tareas;
    while (temp != NULL){
        contador ++;
        temp = temp->sig;
    }
    return contador;
}

int contarTareasPorTipo(listaPendientes* tareas, int idTipoTareas){
    int contador = 0;
    listaPendientes* temp = tareas;
    while (temp != NULL){
        if (temp->enlaceTipos->idTipoTarea == idTipoTareas){
            contador++;
        }
    temp = temp->sig;
    }
    return contador;
}

Personas* cantTareasPersona(Personas* lista){
    Personas * personaMasTareas = NULL;
    int maxTareas = 0;
    Personas* temp = lista;
    while (temp != NULL){
        int tareasActivas = contarTareasActivas(temp->tareas);

        if (tareasActivas > maxTareas){
            maxTareas = tareasActivas;
            personaMasTareas = temp;
        }
        temp = temp->sig;
    }
    return personaMasTareas;
}

void contarTareasDeCadaTipo(Personas *lista, string cedula, int idTipotarea){
    Personas* persona = buscarPersonas(lista, cedula);
    if (persona != NULL){
        int totalTareas = contarTareasPorTipo(persona->tareas, idTipotarea);
        cout << "La persona tiene " << totalTareas << " tareas del tipo " << idTipotarea << endl;
    }
    else{
        cout << "Persona no encontrada" << endl;
    }
}

void tipoTareaMasComun(Personas* lista){
    map<int, int> conteoTiposTarea;
    Personas* temp = lista;
    while (temp != NULL){
        listaPendientes* tarea = temp->tareas;

        while (tarea != NULL){
            int idTipo = tarea->enlaceTipos->idTipoTarea;
            conteoTiposTarea[idTipo]++;
            tarea = tarea->sig;
        }
        temp = temp->sig;
    }
    int max = 0;
    vector<int> tiposMasComunes;
    for (const auto& par : conteoTiposTarea){
        if (par.second > max){
            tiposMasComunes.clear();
            tiposMasComunes.push_back(par.first);
            max = par.second;
        }
        else if (par.second == max){
            tiposMasComunes.push_back(par.first);
        }
    }
    if (tiposMasComunes.size() == 1){
        cout << "El tipo de tarea mas comun es el " << tiposMasComunes[0] << " con" << max << " tareas." << endl;
    }
    else{
        cout << "Hay un emopate entre los tipos de taras ";
        for (int tipo : tiposMasComunes){
            cout << tipo << " ";
        }
        cout << "con " << max << " tareas." << endl;
    }
}

void imprimirPersonasSinTareas(Personas* lista){
    Personas* temp = lista;
    bool PersonasSinTareas = false;
    while (temp != NULL){
        if(temp->tareas == NULL){
            cout << "Persona sin tareas: " << temp->nombre << " "
                 << temp->apellido<< endl;
            PersonasSinTareas = true;
        }
        temp = temp->sig;
    }
    if (!PersonasSinTareas){
        cout << "No hay personas sin tareas." << endl;
    }
}

void obtenerFechaActual(int& dia, int& mes, int& año){
    time_t fechaActual = time(0);
    tm* tiempoActual = localtime(&fechaActual);
    dia = tiempoActual->tm_mday;
    mes = tiempoActual->tm_mon + 1;
    año = tiempoActual->tm_year + 1900;
}

int convertirADias(int dia, int mes, int año){
    tm fecha = {0};
    fecha.tm_mday = dia;
    fecha.tm_mon = mes - 1;
    fecha.tm_year = año - 1900;
    time_t tiempo = mktime(&fecha);
    return tiempo / (60 * 60 * 24);
}

int diferenciaEnDias(int dia1, int mes1, int año1, int dia2, int mes2, int año2){
    int dias1 = convertirADias(dia1, mes1, año1);
    int dias2 = convertirADias(dia2, mes2, año2);
    return dias2 - dias1;
}

void imprimirTareasProximas(Personas * listaPersonas, int diaActual, int mesActual, int añoActual){
    int diaActual, mesActual, añoActual;
    obtenerFechaActual(diaActual, mesActual, añoActual);

    Personas* temp = listaPersonas;
    bool tareasProximas = false;

    while (temp != NULL){
        listaPendientes * tarea = temp->tareas;

        while (tarea != NULL){
            int diferenciaDias = diferenciaEnDias(diaActual, mesActual, añoActual, tarea->dia, tarea->mes, tarea->year);

            if (diferenciaDias > 0 && diferenciaDias <= 7){
                cout << "Tarea próxima a vencer de " << temp->nombre << " " << temp->apellido
                     << ": " << tarea->descripcion << " - Fecha de vencimiento: " << tarea->dia << "/"
                     << tarea->mes << "/" << tarea->year << " - Hora: " << tarea->hora << endl;
                tareasProximas = true;
            }
            tarea = tarea->sig;
        }
        temp = temp->sig;
    }
    if (!tareasProximas){
        cout << "No hay tareas próximas a vencer." << endl;
    }
}

void importanciaMasComunListaPendientes(Personas*lista){
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

void tipoTareaActivasMasComunEnImportanciaMedia(Personas*lista){
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

void tipoTareaRealizadasMasComunEnImportanciaAlta(Personas*lista){
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

void imprimirTareasActivasFecha(Personas*lista,string cedula,string rangoFecha){
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

bool comprobarFormatoFecha(string rangoFecha){
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
    while(true){
        system("cls");
        opcion1=0;
        cout<<"-----Gestor de Tareas-----"<<endl;
        cout<<endl<<endl<<"Que quieres hacer papu?"
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
                    cout<<"----------Listas----------"<<endl;
                    cout<<"Que quieres hacer papu otra vez xddd: "
                    <<endl<<endl<<"Insertar tipos de tareas (1)"
                    <<endl<<"Insertar personas a la lista (2)"
                    <<endl<<"Borrar personas de la lista (3)"
                    <<endl<<"Insertar tareas (X persona)(4)"
                    <<endl<<"Modificar tareas activas (X persona)(5)"
                    <<endl<<"Borrar tareas activas (X persona)(6)"
                    <<endl<<"Insertar Subtareas (A una tarea activa)(7)"
                    <<endl<<"Modificar el avance de una subtarea (8)"
                    <<endl<<"Completar una tarea (X persona)(9)"
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
                    cout<<"Binvenidos a consultas. Introduzca la opcion a elegir: "<<endl;
                    cout<<"Imprimir la persona con mas tareas activas (1)"<<endl;
                    cout<<"Imprimir una persona con mas tareas activas de un determinado tipo (2)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun (3)"<<endl;
                    cout<<"Imprimir la persona con mas tareas vencidas de un determinado tipo (fecha requerida) (4)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun a vencer (fecha requerida) (5)"<<endl;
                    cout<<"Imprimir el tipo de importancia mas comun en la lista personas (6)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun con importancia media en la lista persona (7)"<<endl;
                    cout<<"Imprimir el tipo de tarea mas comun realizadas con una importancia alta (8)"<<endl;
                    cout<<"Salir (9)"<<endl<<endl;
                    cout<<"Digite su respuesta: "<<endl;
                    cin>>opcion4;
                    cin.ignore(10000,'\n');
                    switch(opcion4){
                        case 1:{
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
                    cout<<"-----Reportes-----"<<endl<<endl<<"-Bienvenidos a Reportes. Introduzca la opcion a realizar: "<<endl<<endl<<"Imprimir la lista de tipos de tareas(1)"<<endl<<"Imprimir la lista de personas(2)"<<endl<<"Imprimir las personas sin tarea (3)"<<endl<<"Imprimir las tareas activas(X persona)(4)"<<endl<<"Imprimir las tareas a vencer (5)"<<endl<<"Imprimir las subtareas de una tarea (6)"<<endl<<"Imprimir las tareas realizadas (X persona)(7)"<<endl<<"Imprimir las tareas realizadas (Cualquiera)(8)"<<endl<<"Salir (9)"<<endl<<endl<<"Digite su respuesta: ";
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


