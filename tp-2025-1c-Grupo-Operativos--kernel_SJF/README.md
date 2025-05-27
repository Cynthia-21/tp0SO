# 🧠 Trabajo Práctico SO - Grupo Operativos

## 🚀 Cómo levantar el proyecto

### 1. 🧱 Compilar los módulos

Desde el directorio raíz de cada módulo, ejecutar `make` para compilar:

```bash
utnso@utnso:~/Desktop/tp-2025-1c-Grupo-Operativos-/memoria$ make
utnso@utnso:~/Desktop/tp-2025-1c-Grupo-Operativos-/kernel$ make
utnso@utnso:~/Desktop/tp-2025-1c-Grupo-Operativos-/cpu$ make
utnso@utnso:~/Desktop/tp-2025-1c-Grupo-Operativos-/io$ make
```
### 2. ▶️ Orden de ejecución

Los módulos deben iniciarse en el siguiente orden:

1. `memoria`
2. `kernel instrucciones2.txt 128`
3. `cpu 0`
4. Presionar `ENTER` en la consola del kernel
5. `io`

> ⚠️ **Importante:** Cuando se ejecute el Kernel, este pedirá un `ENTER`. **No lo presiones aún** — primero asegurate de levantar la CPU.

---

### 3. 🎯 Argumentos de ejecución

| Módulo   | Argumentos                                |
|----------|-------------------------------------------|
| memoria  | *(sin argumentos)*                        |
| kernel   | `<archivo> <tamaño>` (por ejemplo: instrucciones2.txt 128) |
| cpu      | `<id_cpu>`   (por ejemplo: `0`)               |
| io       | `<dispositivo>` (por ejemplo: `DISCO`)|

---

### 4. 📦 Comandos para ejecutar los módulos

Desde la raíz de cada módulo, ejecutar:

#### 🧠 Memoria
```bash
./bin/memoria
```

#### 🧠 Kernel
```bash
./bin/kernel instrucciones2.txt 128
```

> 🔴 **No presionar ENTER todavía en la terminal del kernel.**

#### 🧮 CPU
```bash
./bin/cpu 0
```

#### ⌨️ Volver al kernel y presionar ENTER
Una vez levantada la CPU, ir a la terminal del kernel y presionar `ENTER` para comenzar la ejecución.

#### 💿 I/O
```bash
./bin/io DISCO
```

La consola de I/O deberia mostrar el siguiente mensaje:
```
[INFO] hh:mm:ss:ms LOGGER I/O/(PID): ## PID: 1 - Fin de IO
```

Y en la terminal de kernel:
```
[INFO] hh:mm:ss:ms LOGGER KERNEL/(PID): ## (1) finalizó IO y pasa a READY
```


# tp-scaffold

Esta es una plantilla de proyecto diseñada para generar un TP de Sistemas
Operativos de la UTN FRBA.

## Dependencias

Para poder compilar y ejecutar el proyecto, es necesario tener instalada la
biblioteca [so-commons-library] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-commons-library
cd so-commons-library
make debug
make install
```

## Compilación y ejecución

Cada módulo del proyecto se compila de forma independiente a través de un
archivo `makefile`. Para compilar un módulo, es necesario ejecutar el comando
`make` desde la carpeta correspondiente.

El ejecutable resultante de la compilación se guardará en la carpeta `bin` del
módulo. Ejemplo:

```sh
cd kernel
make
./bin/kernel
```

## Importar desde Visual Studio Code

Para importar el workspace, debemos abrir el archivo `tp.code-workspace` desde
la interfaz o ejecutando el siguiente comando desde la carpeta raíz del
repositorio:

```bash
code tp.code-workspace
```

## Checkpoint

Para cada checkpoint de control obligatorio, se debe crear un tag en el
repositorio con el siguiente formato:

```
checkpoint-{número}
```

Donde `{número}` es el número del checkpoint, ejemplo: `checkpoint-1`.

Para crear un tag y subirlo al repositorio, podemos utilizar los siguientes
comandos:

```bash
git tag -a checkpoint-{número} -m "Checkpoint {número}"
git push origin checkpoint-{número}
```

> [!WARNING]
> Asegúrense de que el código compila y cumple con los requisitos del checkpoint
> antes de subir el tag.

## Entrega

Para desplegar el proyecto en una máquina Ubuntu Server, podemos utilizar el
script [so-deploy] de la cátedra:

```bash
git clone https://github.com/sisoputnfrba/so-deploy.git
cd so-deploy
./deploy.sh -r=release -p=utils -p=kernel -p=cpu -p=memoria -p=io "tp-{año}-{cuatri}-{grupo}"
```

El mismo se encargará de instalar las Commons, clonar el repositorio del grupo
y compilar el proyecto en la máquina remota.

> [!NOTE]
> Ante cualquier duda, pueden consultar la documentación en el repositorio de
> [so-deploy], o utilizar el comando `./deploy.sh --help`.

## Guías útiles

- [Cómo interpretar errores de compilación](https://docs.utnso.com.ar/primeros-pasos/primer-proyecto-c#errores-de-compilacion)
- [Cómo utilizar el debugger](https://docs.utnso.com.ar/guias/herramientas/debugger)
- [Cómo configuramos Visual Studio Code](https://docs.utnso.com.ar/guias/herramientas/code)
- **[Guía de despliegue de TP](https://docs.utnso.com.ar/guías/herramientas/deploy)**

[so-commons-library]: https://github.com/sisoputnfrba/so-commons-library
[so-deploy]: https://github.com/sisoputnfrba/so-deploy
