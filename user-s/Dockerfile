# Imagen base con herramientas de construcción
FROM debian:bullseye

# Actualizar e instalar herramientas esenciales
RUN apt-get update && apt-get install -y \\
    g++ \\
    cmake \\
    libboost-all-dev \\
    git \\
    wget && \\
    apt-get clean

# Crear directorio de trabajo
WORKDIR /app

# Copiar el código fuente al contenedor
COPY . /app

# Compilar el microservicio
RUN g++ -o user_service_app user_service_application.cpp -std=c++17 -lboost_system -lpthread

# Exponer el puerto del servicio
EXPOSE 18080

# Ejecutar el servicio
CMD [\"./user_service_app\"]
