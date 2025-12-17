const MAXNAMELEN = 255; /* Longitud máxima del nombre de archivo */

typedef string nametype<MAXNAMELEN>; /* Tipo de dato para un nombre */

typedef struct namenode *namelist; /* Lista enlazada de nombres */

struct namenode {
 nametype name;
 namelist next;
};

program DIRECTORIOS {
 version VERSIONB {
 namelist get_directorios(string) = 1;
 } = 1;
} = 0x20000002;