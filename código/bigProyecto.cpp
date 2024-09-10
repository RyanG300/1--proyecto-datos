#include <iostream>
#include <stack>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string>

using namespace std;

struct TiposTarea{ // idTipoTarea / NombreTipoTarea / DescripcionTarea ---- Lista sencilla
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
    TiposTarea*enlaceTipos;
    struct avanceListas*avance; // solo para los tipos estudio

    listaPendientes(int i,string des, string niv,int d,int m, int a,string ho){
        id=i;
        descripcion=des;
        nivelImportancia=niv;
        dia=d;
        mes=m;
        year=a;
        hora=ho;
        sig=NULL;
        enlaceTipos=NULL;
    }
};

struct avanceListas{
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

struct tareasCompletadas{
    int id;
    string descripcion;
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

TiposTarea*insertarTiposTarea(TiposTarea*lista,int id, string nombre,string descripcion){
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
            lista=nuevo;
            return lista;
}

Personas*insertarPersonas(Personas*lista,string nombre,string apellido,string cedula,int edad){
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

bool comprobarCedulasPersonas(Personas*lista,string cedula){
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

Personas*borrarPersonas(Personas*lista,string cedula){
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
                cout<<"La cedula que se digitó no se encuentra en la lista."<<endl;
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

void imprimirTiposTarea(TiposTarea*lista,int numero=0){
    if(lista==NULL){
        return;
    }
    else{
        if(numero==0){
            cout<<"---------Tipos de tarea---------"<<endl<<endl;
        }
        cout<<numero<<")    Tipo tarea: "<<lista->nombreTipoTarea<<endl;
        cout<<"       Id tarea: "<<lista->idTipoTarea<<endl;
        cout<<"       Descripcion: "<<lista->descripcionTarea<<endl;
        imprimirTiposTarea(lista->sig,numero+=1);
    }
}



int main(){
    int opcion1=0;
    int opcion2=0;
    while(true){
        system("cls");
        opcion1=0;
        cout<<"-----Listas locas (titulo provisional)-----"<<endl;
        cout<<endl<<endl<<"Que quieres hacer papu?"<<endl<<"Editar listas (1)"<<endl<<"Otro (no hay xddd)(2)"<<endl<<"Reportes (3)"<<endl<<endl<<"Digite su respuesta: ";
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
                    cout<<"Que quieres hacer papu otra vez xddd: "<<endl<<endl<<"Insertar tipos de tareas (1)"<<endl<<"Insertar personas a la lista (2)"<<endl<<"Borrar personas de la lista (3)"<<endl<<"Insertar tareas (X persona)(4)"<<endl<<"Modificar tareas activas (X persona)(5)"<<endl<<"Borrar tareas activas (X persona)(6)"<<endl<<"Insertar Subtareas (A una tarea activa)(7)"<<endl<<"Modificar el avance de una subtarea (8)"<<endl<<"Completar una tarea (X persona)(9)"<<endl<<"Salir (10)"<<endl<<endl<<"Digite su respuesta: ";
                    cin>>opcion2;
                    cin.ignore(10000,'\n');
                    switch(opcion2){
                        case 1:{
                            system("cls");
                            string nombre;
                            string descripcion;
                            cout<<"Digite el nombre de la tarea: ";
                            getline (cin,nombre);
                            cout<<endl<<"Escriba una descripcion de la tarea: ";
                            getline (cin,descripcion);
                            listaTiposTarea1=insertarTiposTarea(listaTiposTarea1,0,nombre,descripcion);
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
            case 2:{ //Consultes
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
                        case 9:{
                            salir=true;
                            continue;
                        }
                    }
                }

            }

        }
    }




}
