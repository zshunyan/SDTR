const MAX_NAMELEN = 255;
const MAX_MUESTRAS = 100;
const NUM_SENSORES = 3;

typedef string NombreEstacion<MAX_NAMELEN>;

enum TipoSensor {
    TEMPERATURA = 0,
    HUMEDAD = 1,
    PRESION = 2
};

struct Sensor {
    TipoSensor tipo;
    float muestras<MAX_MUESTRAS>;
};

struct Estacion {
    NombreEstacion nombre;
    Sensor sensores[NUM_SENSORES];
};

struct ConsultaSensor {
    NombreEstacion nombre;
    TipoSensor tipo;
};

struct Resultado1 {
    float valores<MAX_MUESTRAS>;
};

struct Resultado2 {
    float mediaTemperatura;
    float mediaHumedad;
    float mediaPresion;
};

program METEO_PROG {
    version VERS_GRUPO {
        int         setEstacion(NombreEstacion)          = 1;
        float       getMediaSensor_Estacion(ConsultaSensor) = 2;
        Resultado1  getSensor_Estacion(ConsultaSensor)   = 3;
        Resultado2  getMediasAllEstaciones()            = 4;
        int         darBaja()                           = 5;
    } = 1;
} = 0x20000001;
