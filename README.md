# CI5652_MDP
**Proyecto del curso de Diseño de Algoritmos II de la Universidad Simón Bolívar.**

Autores:
* Cristian Medina *10-10445*
* Manuel Pacheco *10-10524*

## Descripción
El proyecto es un estudio del resultado de la aplicación de metaheurísticas de
búsqueda local, trayectoria y poblacionales para la resolución del problema
MDP descrito en [opticom.es](http://www.optsicom.es/mdp/) a través de la
implentación de las metaheurísticas en el lenguaje C.

## Instancias del problema utilazados
Actualmente el repositorio contiene una serie de instancias contenidas en el
directorio `testcases` para las pruebas iniciales, mas queda pendiente
seleccionar un conjunto final.

## Como obtener y construir el proyecto
Para obtener el proyecto, puede utilizarse la interfaz de GitHub o bajo el
comando

`git clone https://github.com/ShadowManu/CI5652_MDP.git`

El proyecto utiliza el *buildsystem* cmake bajo un wrapper make. Una vez
obtenido, el proyecto puede construirse con `make build` o simplemente `make`

El ejecutable resultante estará ubicado en la misma carpeta con el nombre `mdp`.

## Como correr el proyecto (esta sección aun no es válida)
El programa espera como primer argumento, la ruta del archivo que
contiene la instancia del problema. Como segundo argumento, la ruta del archivo
de salida con resultados del algoritmo. De faltar alguno de los argumentos, se
utilizará la entrada y salida estándar en cada caso. Ej:

`./mdp MDG-x.txt resultados.txt`
