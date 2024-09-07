#include <iostream>
#include <stack>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

struct TiposTarea{
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

struct listaPersonas{ //Recuerda hacer una funcion para que no hayan cedulas repetidas
    string nombre;
    string apellido;
    string cedula;
    int edad;
    listaPersonas*sig;
    listaPersonas*ant;
    struct listaPendientes*tareas;

    listaPersonas(string nom,string ape, string ced, int ed){
            nombre=nom;
            apellido=ape;
            cedula=ced;
            edad=ed;
            sig=NULL;
            ant=NULL;
            tareas=NULL;
    }
};

struct listaPendientes{
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

    listaPendiente(int i,string des, string niv,int d,int m, int a,string ho){
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



int main(){
    int opcion1=0;
    int opcion2=0;
    while(true){
        system("cls");
        opcion1=0;
        cout<<"-----Listas locas (titulo provisional)-----"<<endl;
        cout<<endl<<endl<<"Que quieres hacer papu?"<<endl<<"Editar listas (1)"<<endl<<"Otro (no hay xddd)(2)"<<endl<<endl<<"Digite su respuesta: ";
        cin>>opcion1;
        cin.ignore(10000,'\n');
        switch(opcion1){
            case 1:{
                system("cls");
                opcion2=0;
                cout<<"----------Listas----------"<<endl;
                cout<<"Que quieres hacer papu otra vez xddd: "<<endl<<endl<<"Insertar tipos de tareas (1)"<<endl<<"nada xdd(2)"<<endl<<endl<<"Digite su respuesta: ";
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
                        insertarTiposTarea(listaTiposTarea1,0,nombre,descripcion);
                        cout<<endl<<"Nueva tarea insertada con exito."<<endl;
                        sleep(2);
                    }
                }
            }
        }
    }




}
