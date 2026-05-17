# Disk Scheduler Visualizer

## Cómo usar el proyecto

### 1. Construir la imagen de Docker

Abrir una terminal en la carpeta del proyecto y ejecutar:

```bash
docker build -t disk-scheduler .
```

---

### 2. Crear una carpeta vacía para los resultados

Crear o escoger una carpeta vacía donde se deseen guardar las gráficas generadas por el programa.

Luego, abrir una terminal dentro de dicha carpeta.

---

### 3. Ejecutar el contenedor

Dentro de la carpeta escogida, ejecutar:

```bash
docker run -it --rm -v "$(pwd):/app" disk-scheduler bash
```

Esto montará la carpeta actual como el directorio `/app` dentro del contenedor, permitiendo que todos los archivos generados aparezcan directamente en el sistema anfitrión.

---

### 4. Ejecutar el programa

Una vez abierta la terminal bash dentro del contenedor, ejecutar:

```bash
./disk [pos_inicial_cabeza]
```

Por ejemplo:

```bash
./disk 7
```

---

### 5. Visualizar las gráficas

Al finalizar la ejecución, en la carpeta vacía seleccionada originalmente aparecerán varios archivos generados, incluyendo:

- `fcfs.svg`
- `scan_up.svg`
- `scan_down.svg`
- `cscan_up.svg`
- `cscan_down.svg`
- `index.html`

Abrir `index.html` en cualquier navegador para visualizar todas las gráficas generadas por los algoritmos de planificación de disco.
