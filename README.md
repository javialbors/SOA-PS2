# SOA-PS2

_Por: Javier Albors Sánchez_

## ¿Cómo compilar la práctica?

Para compilar la práctica, se puede ejecutar el comando ``make`` para que se ejecute el código de compilación presente en el fichero ``makefile``.

Se compilarán todos los archivos necesarios y se creará un ejecutable.

```
shooter <command> [arguments]
```

## Explicación de los sistemas de ficheros

### FAT16

El sistema de ficheros FAT16, del inglés _File Allocation Table_, consiste en un archivo binario que contiene información sobre cómo se distribuyen una serie de ficheros y subdirectorios partiendo de un directorio principal o _root_.

El sistema de ficheros FAT16 se distribuye de la siguiente manera:

Los primeros 90 bytes de información que nos encontramos en esta arquitectura contienen metadatos que ayudan a identificar diversos aspectos de este sistema de ficheros, sobre todo para poder proceder a su lectura posteriormente. En este bloque podemos encontrar, por ejemplo, el nombre del sistema, el numero de FATs (tablas), los sectores por FAT, el tamaño en bytes del sector, etc. Este bloque de información se llama _Boot sector_.

Lo siguiente que podemos encontrar en este archivo serán las FATs o _File Allocation Tables_, habrá tantas como se indique en el _Boot sector_.

Después de las FATs, encontraremos la información del directorio principal o _root_, secciones de 32 bytes que identificarán todo el contenido de este directorio, como posibles ficheros o subdirectorios, así como información sobre éstos como el nombre, la extensión o el _cluster_ donde se encuentra su información.

Un _cluster_ será la sección dónde podremos encontrar información sobre un directorio o fichero.

Posteriormente al sector del directorio _root_ encontramos el área de información, que contendrá la información de ficheros, directorios y subdirectorios, identificados y localizables a través de su número de _cluster_.

### EXT2

El sistema de ficheros EXT2 (segundo sistema de ficheros extendido) consiste en un archivo binario que contiene información sobre cómo se distribuyen una serie de ficheros y subdirectorios siguiendo una estructura de bloques e inodos.

Si empezamos a leer la arquitectura del sistema EXT2 desde el primer byte, encontraremos 1024 bytes de información correspondientes a un sector llamado _Boot block_.

Posteriormente, a este _Boot block_ encontraremos la estructura principal que sigue la arquitectura del sistema de ficheros EXT2, los grupo de bloques. Cada
grupo de bloques está formado por un _Super bloque_, N bloques de _Group descriptors_, un  _bitmap_ de bloques, un _bitmap_ de inodos, N bloques de tablas de inodos y N bloques de información.

El _super bloque_ es un bloque de información de 1024 bytes de longitud, presente en cada grupo de bloques, que contiene información general sobre todo el sistema de ficheros como, por ejemplo, la longitud en bytes de un inodo, el número de inodos, el tamaño en bytes de un bloque, el número de inodos por grupo, etc.

Un inodo es un elemento en nuestro sistema de ficheros, ya sea un archivo, directorio, subdirectorio, etc. El inodo número 2, corresponde al inodo principal o _root_, a partir del cual se enlazan los demás inodos.

Para acceder a un determinado inodo, sabiendo su índice (por ejemplo, el inodo 2 corresponde al inodo _root_), mediante un cálculo que depende de información que puede ser obtenida en el _super bloque_, podemos obtener en qué grupo de bloques se encuentra, y cuál es su _offset_ dentro de este grupo.
Este inodo se encontrará en la tabla de inodos de su respectivo grupo donde se puede obtener información interesante como el tamaño de éste inodo/fichero/directorio o su posición en la sección de información del grupo de bloques donde se encuentra el inodo.

Al buscar un inodo en la sección de información de su grupo de bloques, se sigue la estructura definida como _Directory Entry Structure_, donde podemos parsear la información de este inodo para obtener su nombre, tipo de fichero, o la siguiente posición de la siguiente entrada del directorio.

## Explicación de la práctica

Esta práctica ha consistido en la navegación e interpretación de dos sistemas de ficheros diferentes, FAT16 y EXT2, para posteriormente realizar tres acciones diferentes sobre éstos sistemas de ficheros utilizando los siguientes tres comandos:

* **/info**: Este comando debe mostrar la información de éstos sistemas de ficheros o un error en el caso de que no se trate de un archivo válido.

* **/find**: Este comando busca por todo el sistema de ficheros un archivo que el usuario debe indicar. Se mostrará si se ha encontrado o no, y en caso afirmativo, se mostrará el tamaño que ocupa este fichero en el sistema de ficheros.

* **/delete**: Este comando eliminará o dejará de tener en cuenta el fichero indicado por el usuario siempre y cuando éste se encuentre en el sistema de ficheros.

### Requisitos

Para poder trabajar sobre esta práctica, el único requisito es disponer de un entorno de compilación en C y de tener Git instalado. En mi caso, el entorno que he utilizado ha sido el servidor de la universidad _@montserrat.salle.url.edu_ mediante al cual he accedido de manera remota utilizando un _VPN_ y utilizando el protocolo _SSH_ desde _terminal_ en mi ordenador local.

### Diseño

El diseño de esta práctica consta principalmente de dos módulos, ``ext2.h`` y ``fat16.h`` así como sus archivos ``.c`` que contienen las diferentes funciones y estructuras de datos que se centran en los dos sistemas de ficheros respectivamente, funciones que dan respuesta a los comandos ``/info``, ``/find``, ``/delete`` y otras funciones auxiliares.

Además, en el proyecto también podemos encontrar un fichero llamado ``shooter.c`` que contiene la función main, y estructura el código. Detecta, valida y da respuesta a los comandos introducidos en caso de error, y llama a las funciones de dichos módulos para obtener una respuesta en caso de que no haya ningún error.

### Estructuras de datos utilizadas

Las principales estructuras de datos utilizadas durante la práctica han sido diferentes _``structs``_, estructuras en C que han permitido estructurar de una manera más clara el código para guardar datos necesarios durante el transcurso de la práctica como la información general de los sistemas de ficheros FAT16 y/o EXT2 o la respuesta de algunas funciones para obtener más de una variable como respuesta.

### Método de pruebas utilizado

El método de pruebas que he utilizado ha sido, sobre todo, ir printando por pantalla cada paso de la navegación de estos sistemas de fichero para tener un control sobre dónde estoy accediendo con el programa.

Además, también he utilizado un editor de texto en hexadecimal (https://hexed.it/) para poder tener también un control visual de en qué punto estaba en el programa y en qué punto debería estar.

### Problemas observados

Los problemas observados durante el transcurso de la práctica han sido los siguientes:

* El principal problema y el más genérico ha sido trabajar sobre ficheros binarios extensos, cosa que ha causado algún problema al no poder interpretar esta información a simple vista, por lo tanto ha sido un poco abstracto.

* Entender el funcionamiento de estos dos sistemas de fichero ha resultado algo confuso al inicio de la práctica.

* Entender la estructura de grupos de bloques del EXT2 no fue cosa fácil en el inicio.

* No hay demasiada información por internet sobre estos sistemas de ficheros, sobre todo en cosas específicas como por ejemplo el proceso a seguir para eliminar un fichero.

* Un problema menor ha sido el cambio de paradigma al implementar funciones de ambos sistemas de ficheros a la vez, ya que sus estructuras son diferentes.

* Durante el transcurso de la práctica me he visto en situaciones de bloqueo a causa de confusiones al entender el funcionamiento o localización de diferentes datos de interés o contradicciones al buscar información por internet.

### Estimación temporal

Tras la realización de la práctica, pienso que, como estimación, contando el tiempo utilizado tanto como para programar como para la búsqueda de información para llevar a cabo esta práctica, la duración estimada ha sido de unas 30 horas aproximadamente.

## Explicación y valoración de Git

### Explicación

Git es una herramienta de gestión y control de versiones para proyectos de programación como éste mismo, que permite estructurar el código por secciones y tener un control más amplio de éste así como de sus versiones y su cronología.

Sus principales características son la habilidad de clonar un repositorio remoto a tu ordenador local de manera que se puede editar o añadir código al proyecto para posteriormente actualizarlo nuevamente en el repositorio local.

Los comandos más utilizados, sobre todo durante el transcurso de esta práctica han sido los siguientes:

``git clone <URL-repository>`` ➡️ Para clonar el proyecto del repositorio indicado

``git branch <branch-name>`` ➡️ Para crear una rama en el repositorio desde la rama actual

``git checkout <branch-name>`` ➡️ Para navegar hacia una rama

``git add <file>`` ➡️ Añade un fichero al seguimiento de git sobre los ficheros a actualizar en el proyecto

``git commit -m "<message>"`` ➡️ Para añadir los ficheros modificados o creados como una nueva actualización de la rama, para posteriormente actualizarla en el repositorio en remoto.

``git push`` ➡️ Actualiza la rama en local en la misma rama del repositorio en remoto

``git status`` ➡️ Para obtener información del estado de la rama en la que se está, así como los ficheros añadidos, modificados o creados en esta rama.

### Valoración

Bajo mi opinión personal, pienso que Git es una herramienta muy potente para llevar un seguimiento sobre un proyecto, de manera cronológica y sobre todo para no sobreescribir el proyecto continuamente ya que esto puede conllevar riesgos si el proyecto tiene que cambiar posteriormente, ya que creando diferentes ramas podemos crear diferentes _checkpoints_ del proyecto.

Además, pienso que es ideal para los proyectos grupales o para aplicaciones o proyectos _open source_, para que pueda haber un seguimiento de posibles actualizaciones, además de que la comunidad pueda valorar de manera positiva o negativa estas actualizaciones para ser añadidas o no al proyecto.

## Conclusiones

Las conclusiones que he podido extraer durante el transcurso de esta práctica han sido las siguientes:

* He aprendido el funcionamiento de dos sistemas de ficheros, FAT16 y EXT2, además de como están estructurados y cómo navegar sobre ellos para obtener datos de ficheros, directorios, subdirectorios, etc.

* He podido comprender diferentes maneras de como los sistemas operativos actuales gestionan los ficheros de manera interna.

* Me he hecho una idea de posibles implementaciones internas a comandos de uso habitual en nuestro gremio como el comando ``ls`` y sus derivados, por ejemplo.

* He aprendido a encontrar un fichero en un sistema de ficheros FAT16 y EXT2 y la información que éste contiene.

* He aprendido a navegar sobre ficheros binarios gracias al uso de diversas funciones como ``lseek()``, así como a leer y a escribir sobre estos utilizando las funciones ``read()`` ``write()``.

* He practicado, aplicado y ampliado mi visión sobre las técnicas de programación recursiva.

* He aprendido a utilizar un editor hexadecimal y a navegar por éste para encontrar y interpretar diferentes datos de interés.

* He ampliado y agilizado mis habilidades sobre la herramienta Git.

* He aprendido a crear un archivo README siguiendo el lenguaje Markdown.

* He refrescado y mejorado mis habilidades de programación en lenguaje C.

## Webgrafía

* https://hexed.it/
* http://www.science.unitn.it/~fiorella/guidelinux/tlk/node100.html
* https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system#Directory_entry
* https://www.ntfs.com/fat-root.htm
* https://www.ntfs.com/fat-folder-structure.htm
* https://www.youtube.com/watch?v=q_e0f5H_UPY&ab_channel=Adonbilivit213Adonbilivit213
* https://www.datarecoverytools.co.uk/2009/12/07/file-deletion-in-fat16/
* https://www.slashroot.in/how-does-file-deletion-work-linux
