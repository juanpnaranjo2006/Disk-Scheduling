FROM japeto/so-tools:v64

# Directorio de trabajo dentro del contenedor
WORKDIR /app

# Copiar archivos uno por uno
COPY main.cpp .

# Compilar manualmente
# Compilar manualmente
RUN g++ -std=c++20 -O2 -static -static-libgcc -static-libstdc++ main.cpp -o disk