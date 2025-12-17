/* authunion.x */

enum tipo_dato {
    ES_ENTERO = 0,
    ES_FLOTANTE = 1
};

union Dato switch (tipo_dato tipo) {
    case ES_ENTERO: int i;
    case ES_FLOTANTE: float f;
    default: string s<>;
};

program UNION_AUTH_PROG {
    version UNION_AUTH_VERS {
        int PROCESAR(Dato) = 1;
    } = 1;
} = 0x20000001;

