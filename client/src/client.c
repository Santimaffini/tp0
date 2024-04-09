#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");

	
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	
	if(config == NULL){
			perror("Error: el config no se creo correctamente");
			exit(EXIT_FAILURE);
	}

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	
	ip = leer_variable_desde_config(config, "IP");
	puerto = leer_variable_desde_config(config, "PUERTO");
	valor = leer_variable_desde_config(config, "CLAVE");

	// Loggeamos el valor de config

	log_info(logger, "Valor de config = %s",valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create("tp0.log","logtp0",1,LOG_LEVEL_INFO);

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	
	leido = readline("> ");

	while(strcmp(leido,"")!=0)
	{
	
		add_history(leido);  //agrego al historial la linea

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	
		log_info(logger,leido);

		leido = readline("> ");

	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido = readline("> ");
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	
	while(strcmp(leido,"")!=0)
	{
		agregar_a_paquete(paquete, leido, strlen(leido));

		leido = readline("> ");

	}

	enviar_paquete(paquete, conexion);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
	eliminar_paquete(paquete);
	free(leido);

}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}

char* leer_variable_desde_config(t_config* config, char* key){
	if(config_has_property(config, key) == false){
		perror("Error: no se encontro la key en el config");
		exit(EXIT_FAILURE);
	}
	return config_get_string_value(config, key);
	
}

