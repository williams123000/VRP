#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include<stdbool.h>// Booleanos

typedef struct {
    int Ciudad_Inicial;
    int Ciudad_Final;
    int Capacidad;
    float Capacidad_Restante;
    int *Ruta;
    int Num_Ciudades;
} Vehiculo;




float Calcular_Distancia(float EjeX_1, float EjeY_1, float EjeX_2, float EjeY_2){
    float Distancia = pow((EjeX_2-EjeX_1),2) + pow((EjeY_2-EjeY_1),2);
    Distancia = sqrt(Distancia);
    printf("La distancia entre el punto ( %.2f , %.2f ) y ( %.2f , %.2f ) es: %.2f\n", EjeX_1, EjeY_1, EjeX_2, EjeY_2, Distancia);
    return Distancia;
}

float Calcular_Longitud_Total(float **MatrizDistancias, Vehiculo *Vehiculos, int numero_vehiculos){
    printf("\n\nLongitudes entre los puntos de cada vehiculo: \n\n");
    //variable para definir la long total de la distancia de todos los vehiculos
    float longitudTotal = 0.0;
    for (int v = 0; v < numero_vehiculos; ++v) {
        Vehiculo Vehiculo = Vehiculos[v];
        printf("Vehiculo %d: \n", v);
        
        //Variable para la ditancia de cada uno de los vehiculos
        float longitudRuta = 0.0;
        printf("Numero de ciudades del vehiculo: %d \n", Vehiculo.Num_Ciudades);
        for (int i = 0; i < Vehiculo.Num_Ciudades - 1 ; i++) {         
            int ciudad_actual = Vehiculo.Ruta[i];
            int ciudad_siguiente = Vehiculo.Ruta[i + 1];
            //Guardar sus cordenadas en variables dependiendo de su ciudad acual y siguente
            float EjeX1 = MatrizDistancias[ciudad_actual-1][1];
            float EjeY1 = MatrizDistancias[ciudad_actual-1][2];
            float EjeX2 = MatrizDistancias[ciudad_siguiente-1][1];
            float EjeY2 = MatrizDistancias[ciudad_siguiente-1][2];
            //Sacar su disctancia entre las coordenada dadas e ir sumando esa cantidad
            longitudRuta += Calcular_Distancia(EjeX1,EjeY1, EjeX2, EjeY2);
            printf("Longitud: %f \n\n", longitudRuta);
        }

        //Guardar coordenadas de la ultima ciudad con el deposito
        float EjeX1 = MatrizDistancias[Vehiculo.Ruta[Vehiculo.Num_Ciudades-1]-1][1];
        float EjeY1 = MatrizDistancias[Vehiculo.Ruta[Vehiculo.Num_Ciudades-1]-1][2];
        float EjeX2 = MatrizDistancias[Vehiculo.Ruta[0]-1][1];
        float EjeY2 = MatrizDistancias[Vehiculo.Ruta[0]-1][2];

        longitudRuta += Calcular_Distancia(EjeX1,EjeY1, EjeX2, EjeY2);
        printf("Longitud: %f \n\n", longitudRuta);
        //suma todas las distancias de los vehiculos que se asignaron
        longitudTotal += longitudRuta;
    }
    return longitudTotal;
}

int Generar_Numero(int Number_City){
    int Number = 0;
    //Excluir numero aleatorio == 0
    while(Number==0){
        Number = rand() % Number_City;
    } 
    return Number;
}

bool Generar_Solucion_Inicial(Vehiculo *Vehiculos, int numero_vehiculos, int numero_ciudades, float city_inicial, float city_final, float capacity, int Number_City_Archive, float **matriz_Demandas, int Numero_Archivo_Demandas) {
    //Sacar el modulo de numero de ciudades entre el numero de vehiculos
    int modulo = numero_ciudades % numero_vehiculos; 
    //Se asigna un auxiliar para almacenar el numero de ciudades menos el modulo, esto para tener una division entera
    int aux_numero_ciudades = numero_ciudades - modulo; 
    //Se obtiene la division entera sin afectar o tener decimales
    int num_ciudades_x_Vehiculo = aux_numero_ciudades / numero_vehiculos;
    //Se crea el array de las cantidades de ciudades para cada vehiculo
    int Array_Num_Ciudades[numero_vehiculos];

    //Se asigna el valor entero a cada una de la ciudades
    for (int v = 0; v < numero_vehiculos; v++){
        Array_Num_Ciudades[v] = num_ciudades_x_Vehiculo;        
    }

    //Se agrega en caso de exista el valor del restante de la division entera al ultimo vehiculo, NOTA: En caso de querer tener el vehiculo 0 o el primero cambiar la posicion del Array
    Array_Num_Ciudades[numero_vehiculos-1] = Array_Num_Ciudades[numero_vehiculos-1] + modulo;
    //Array para acumular las ciudades que ya han sido visitadas
    int Array_Ciudades_Visitadas[numero_ciudades];

    //Contador para saber en que ciudad vamos del Array anterior
    int contador_Array_Ciudades_Visitadas = 0;
    
    for (int v = 0; v < numero_vehiculos; ++v) {
        //Creamos el struct Vehiculo
        Vehiculo *Vehiculo = &Vehiculos[v];

        //Asignacion de Atributos
        Vehiculo->Num_Ciudades = Array_Num_Ciudades[v] + 1;
        printf("Numero de ciudades: *** %d",  Vehiculo->Num_Ciudades);
        Vehiculo->Ciudad_Inicial = (int) city_inicial; 
        Vehiculo->Ciudad_Final = (int) city_final;
        Vehiculo->Capacidad = (int) capacity;
        Vehiculo->Capacidad_Restante = 0.00;

        //Reservar memoria para las rutas de cada vehiculo
        //Se suma 1 ya que ese seria el deposito
        Vehiculo->Ruta = (int *)malloc( (Vehiculo->Num_Ciudades) * sizeof(int));
        int Limitador_Iteracion = 0;
        for (int i = 0; i < Vehiculo->Num_Ciudades-1; i++) {
            //Se genera numero aleatorio para la posicion de la ciudad del vehiculo (esto dependeria de las ciudades que se den o de todas las cuidades que existan)
            int ciudad_generada = Generar_Numero(Number_City_Archive);
            int x = 0;

            //Encontrar la Demanda de la ciudad dependiendo del numero aleatorio dado
            float Capacidad_Ciudad = matriz_Demandas[ciudad_generada-1][2];
            printf("Capacidad Ciudad ***** %f  }*********** %d \n" , Capacidad_Ciudad, ciudad_generada);
            //Sumar esa demanda a la capacidad del vehiculo donde fue su ruta
            if (i < Vehiculo->Num_Ciudades - 1){
                Vehiculo->Capacidad_Restante = Vehiculo->Capacidad_Restante + Capacidad_Ciudad;
            }
            
            printf("%f *********** \n", Vehiculo->Capacidad_Restante);
            //Condicion para saber si ciudad ya fue visitada
            while ( x < contador_Array_Ciudades_Visitadas){
                if(Limitador_Iteracion == numero_ciudades*2){
                    printf("Generacion de solucion fallida\n");
                    return false;
                }
                //Condicion para saber si el num aleatorio ya esta entre las ciudades visitadas O exede la capacidad del vehiculo
                if(ciudad_generada == Array_Ciudades_Visitadas[x] || Vehiculo->Capacidad_Restante>capacity){
                    printf("Excede!!");
                    //Restarle esa demanda de la ciudad que se visito
                    Vehiculo->Capacidad_Restante = Vehiculo->Capacidad_Restante - Capacidad_Ciudad;

                    //Generar un nuevo numero o ciudad
                    ciudad_generada = Generar_Numero(Number_City_Archive);
                    
                    //Encontrar la Demanda de la ciudad dependiendo del numero aleatorio nuevo dado 
                    Capacidad_Ciudad = matriz_Demandas[ciudad_generada-1][2];

                    //Sumar esa demanda nueva que se genero
                    Vehiculo->Capacidad_Restante = Vehiculo->Capacidad_Restante + Capacidad_Ciudad;
                    
                    //Reiniciar el verificado de las ciudades visitadas
                    x = 0;
                    //Aumentador l limitador de iteracion
                    Limitador_Iteracion++;
                }else{
                    //Si no continua hacia la siguiente ciudad
                    x++;
                    
                }
            }

            //Si el numero aleatorio no esta dentro de las ciudades visitas se agrega a las ciudades visitads
            Array_Ciudades_Visitadas[contador_Array_Ciudades_Visitadas] = ciudad_generada;

            //asigna la ruta del vehiculo con esa ciudad
            Vehiculo->Ruta[i] = ciudad_generada;

            //Incrementa el numero de ciudad visitadas
            contador_Array_Ciudades_Visitadas++;
        }
        
        //Barajea el arreglo de rutas (excepto el depósito)
        for (int i = Vehiculo->Num_Ciudades - 2; i > 0; i--) {
            int j = rand() % i;
            int temporal = Vehiculo->Ruta[i];
            Vehiculo->Ruta[i] = Vehiculo->Ruta[j];
            Vehiculo->Ruta[j] = temporal;
        }

        //Incluir el deposito en la ruta de cada uno de los vehiculos
        Vehiculo->Ruta[Array_Num_Ciudades[v]] = Number_City_Archive ;
    }   
    return true;
}


void generarVecinos(Vehiculo *Vehiculos, int numero_vehiculos,float **MatrizDistancias){
    int numvehialea = rand() % numero_vehiculos;
    int i = 0, j = 0;

    while ( i == j){
        while (i == 0 || i == j) {
            i = rand() % (Vehiculos[numvehialea].Num_Ciudades-1);
        }
        while (j == 0 || i == j) {
            j = rand() % (Vehiculos[numvehialea].Num_Ciudades-1);
        }
 
    }
    
    

    
    int ciudad =  Vehiculos[numvehialea].Ruta[i];
    Vehiculos[numvehialea].Ruta[i] = Vehiculos[numvehialea].Ruta[j];
    Vehiculos[numvehialea].Ruta[j] = ciudad;
   

    for (int v = 0; v < numero_vehiculos; ++v) {
        printf("Ruta del vehiculo %d (Capacidad: %d) (Capacidad_Acumulada): %.2f       " , v , Vehiculos[v].Capacidad , Vehiculos[v].Capacidad_Restante );
        for ( int i = 0 ; i < Vehiculos[v].Num_Ciudades ; ++i ) {
            printf( "%d" , Vehiculos[v].Ruta[i] );
            if( i < Vehiculos[v].Num_Ciudades - 1 ) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    
    float longitudTotal = Calcular_Longitud_Total(MatrizDistancias, Vehiculos, numero_vehiculos);
    printf("Longitud total de las rutas: %lf\n", longitudTotal);

}

float Calcular_Capacidad (Vehiculo Vehiculo, float **Matriz_Demandas){
    int Numero_Ciudades = Vehiculo.Num_Ciudades;
    float Capacidad = 0;
    for (int i = 0; i < Numero_Ciudades-1; i++){
        float Capacidad_Ciudad = Matriz_Demandas[Vehiculo.Ruta[i]-1][2];
        //printf("Capac Ciudad: %f\n", Capacidad_Ciudad);
        Capacidad = Capacidad + Capacidad_Ciudad;
    }

    return Capacidad;
}

void generarVecinos2(Vehiculo *Vehiculos, int numero_vehiculos,float **MatrizDistancias, float **Matriz_Demandas){
    bool Check = false;
    
    while (Check == false){
        int ruta_fuente = rand () % numero_vehiculos;
    int ruta_destino;
    do{
        ruta_destino = rand () % numero_vehiculos;
    }while(ruta_destino==ruta_fuente);

    int i = 0, j = 0;
    while ( i == j){
        while (i == 0 || i == j) {
            i = rand() % (Vehiculos[ruta_destino].Num_Ciudades-1);
        }
        while (j == 0 || i == j) {
            j = rand() % (Vehiculos[ruta_fuente].Num_Ciudades-1);
        }
 
    }
    
    int ciudad = Vehiculos[ruta_fuente].Ruta[j];
    Vehiculos[ruta_fuente].Ruta[j] = Vehiculos[ruta_destino].Ruta[i];
    Vehiculos[ruta_destino].Ruta[i] = ciudad;
    
    float Capacidad_Destino = Calcular_Capacidad(Vehiculos[ruta_destino], Matriz_Demandas);
    float Capacidad_Fuente = Calcular_Capacidad(Vehiculos[ruta_fuente], Matriz_Demandas);
    
    if (Capacidad_Destino <= Vehiculos[ruta_destino].Capacidad && Capacidad_Fuente <= Vehiculos[ruta_fuente].Capacidad ){
        Check = true;
        Vehiculos[ruta_destino].Capacidad_Restante = Capacidad_Destino;
        Vehiculos[ruta_fuente].Capacidad_Restante = Capacidad_Fuente;
    }else{
        int ciudad = Vehiculos[ruta_destino].Ruta[i] ;
        Vehiculos[ruta_destino].Ruta[i] = Vehiculos[ruta_fuente].Ruta[j];
        Vehiculos[ruta_fuente].Ruta[j] = ciudad;
    }
    
    for (int v = 0; v < numero_vehiculos; ++v) {
        printf("Ruta del vehiculo %d (Capacidad: %d) (Capacidad_Acumulada): %.2f       " , v , Vehiculos[v].Capacidad , Vehiculos[v].Capacidad_Restante );
        for ( int i = 0 ; i < Vehiculos[v].Num_Ciudades ; ++i ) {
            printf( "%d" , Vehiculos[v].Ruta[i] );
            if( i < Vehiculos[v].Num_Ciudades - 1 ) {
                printf(" -> ");
            }
        }
        printf("\n");
    }
    }
    
    
    

    
    float longitudTotal = Calcular_Longitud_Total(MatrizDistancias, Vehiculos, numero_vehiculos);
    printf("Longitud total de las rutas: %lf\n", longitudTotal);

}


void Imprimir_Matriz_Puntos(float **Matriz_Coordenadas, int Numero_Ciudades){
    printf("Matriz de Coordenadas: \n\n");
    printf("ID,   CX,    CY,    Type\n");
    for (int i = 0; i < Numero_Ciudades; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%.2f  ", Matriz_Coordenadas[i][j]);
        }
        printf("\n");
    }
}

void Imprimir_Matriz_Demandas(float **Matriz_Demandas, int Numero_Demandas){
    printf("Matriz de demandas: \n\n");
    printf("ID,   ID_Ciudad,    Demanda\n");
    for (int i = 0; i < Numero_Demandas; ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%.2f       ", Matriz_Demandas[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Uso: %s <número de ciudades> <número de vehículos> <archivo de distancias.txt> <archivo profilevehiculo.txt> <archivo de denamanda.txt>\n", argv[0]);
        return 1;
    }

    int numero_ciudades = atoi(argv[1]); // Número de ciudades que visitara cada Vehiculo
    int numero_vehiculos = atoi(argv[2]); // Número de vehículos totales

    srand(time(NULL)); 
    
    printf("\nNumero de Ciudades: %d", numero_ciudades);
    printf("\nNumero de Vehiculos: %d", numero_vehiculos);

    FILE *archivo_distancias = fopen(argv[3], "r");
    if (archivo_distancias == NULL) {
        perror("Error al abrir el archivo de distancias");
        return 1;
    }
    
    // Validar cuantas ciudades hay en el archivo de Texto
    int c = 0;
    int Numero_Ciudades_Archivo = 0;
    while((c = fgetc(archivo_distancias)) != EOF){
        if( c == '\n')
            Numero_Ciudades_Archivo++;
    }
    fclose(archivo_distancias);

    // Generar la matriz con los puntos X y Y
    // ID, CX, CY, Type, 
    FILE *archivo_distancias_ = fopen(argv[3], "r");
    if (archivo_distancias_ == NULL) {
        perror("Error al abrir el archivo de distancias");
        return 1;
    }
    
    printf ("\nNumero de ciudades del archivo: %d\n\n",Numero_Ciudades_Archivo);

    // Reservar memoria para la matriz de puntos
    float **matriz_distancias = (float **)malloc(Numero_Ciudades_Archivo * sizeof(float *));
    for (int i = 0; i < Numero_Ciudades_Archivo; ++i) {
        matriz_distancias[i] = (float *)malloc(4 * sizeof(float));
    }

    //Extraer datos del archivo
    for (int i = 0; i < Numero_Ciudades_Archivo; ++i) {
        for (int j = 0; j < 4; ++j) {
            fscanf(archivo_distancias_, "%f, ", &matriz_distancias[i][j]);
        }
    }
    //Impresion para la matriz de puntos
    //Imprimir_Matriz_Puntos(matriz_distancias, Numero_Ciudades_Archivo);
    fclose(archivo_distancias_);

    //Abrir el archivo del profile vehiculo
    FILE *Vehicle_Profile = fopen(argv[4], "r");
    if (Vehicle_Profile == NULL) {
        perror("Error al abrir el archivo de distancias");
        return 1;
    }

    //Datos del Vehiculo
    int Ciudad_Inicial = 0;
    int Ciudad_Final = 0;
    float Capacidad = 0.00;

    //Leer la ciudad inicial, final y la capacidad de los vehiculos contenidos en el archivo
    while( fscanf(Vehicle_Profile, "%i, %i, %f", &Ciudad_Inicial, &Ciudad_Final, &Capacidad) == 3){
        printf("Datos del Vehiculo extraidos: \nCiudad_Inicial: %i \nCiudad_Final: %i \nCapacidad: %f\n\n", Ciudad_Inicial, Ciudad_Final, Capacidad);
    }
    fclose(Vehicle_Profile);

    //Abrir el archivo de Demanda para las ciudades
    FILE *Demanda = fopen(argv[5], "r");
    if (Demanda == NULL) {
        perror("Error al abrir el archivo de distancias");
        return 1;
    }
    // Generar el numero de demandas dependiendo de las ciudades
    c = 0;
    int Numero_Demandas_Archivo = 0;
    while((c = fgetc(Demanda)) != EOF){
        if( c == '\n')
            Numero_Demandas_Archivo++;
    }
    fclose(Demanda);
   
    //Abrir el archivo de Demanda para las ciudades
    FILE *Demanda_ = fopen(argv[5], "r");
    if (Demanda_ == NULL) {
        perror("Error al abrir el archivo de distancias");
        return 1;
    }
    // Reservar memoria para la matriz de demandas
    float **matriz_demandas = (float **)malloc(Numero_Demandas_Archivo * sizeof(float *));
    for (int i = 0; i < Numero_Demandas_Archivo; ++i) {
        matriz_demandas[i] = (float *)malloc(3 * sizeof(float));
    }
    //Extraer datos para generar la matriz de demandas
    for (int i = 0; i < Numero_Demandas_Archivo; ++i) {
        for (int j = 0; j < 3; ++j) {
            fscanf(Demanda_, "%f, ", &matriz_demandas[i][j]);
        }
    }

    //Imprimir matriz de demandas
    //Imprimir_Matriz_Demandas(matriz_demandas, Numero_Demandas_Archivo);

    fclose(Demanda_);

    //Generacion de nuestros vehiculos
    bool Generar_Solucion = false;
    Vehiculo *Vehiculos = (Vehiculo *)malloc(numero_vehiculos * sizeof(Vehiculo));
    while (Generar_Solucion == false)
    {
        
    Generar_Solucion = Generar_Solucion_Inicial(Vehiculos, numero_vehiculos, numero_ciudades, Ciudad_Inicial, Ciudad_Final, Capacidad, Numero_Ciudades_Archivo, matriz_demandas, Numero_Demandas_Archivo);

    printf("Response: %d \n \n \n", Generar_Solucion );
    }
    
    

    for (int v = 0; v < numero_vehiculos; ++v) {
        printf("Ruta del vehiculo %d (Capacidad: %d) (Capacidad_Acumulada): %.2f       " , v , Vehiculos[v].Capacidad , Vehiculos[v].Capacidad_Restante );
        for ( int i = 0 ; i < Vehiculos[v].Num_Ciudades ; ++i ) {
            printf( "%d" , Vehiculos[v].Ruta[i] );
            if( i < Vehiculos[v].Num_Ciudades - 1 ) {
                printf(" -> ");
            }
        }
        printf("\n");
    }

    float longitudTotal = Calcular_Longitud_Total(matriz_distancias, Vehiculos, numero_vehiculos);
    printf("Longitud total de las rutas: %lf\n", longitudTotal);
    //printf ("****************************************************GENERAR SOLUCIONES*******************************************************\n");
    //generarVecinos(Vehiculos,numero_vehiculos,matriz_distancias);
    printf ("****************************************************GENERAR SOLUCIONES2*******************************************************\n");
    generarVecinos2(Vehiculos,numero_vehiculos,matriz_distancias, matriz_demandas);



    return 0;
}
