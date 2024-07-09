# Usando uma imagem base do Ubuntu 20.04
FROM ubuntu:20.04

# Configurando a variável de ambiente para não precisar de interação durante a instalação
ENV DEBIAN_FRONTEND=noninteractive

# Instalando dependências do sistema
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    libssl-dev \
    libboost-all-dev \
    libasio-dev

# Clonando e instalando o Crow
RUN git clone --depth=1 https://github.com/CrowCpp/Crow /tmp/Crow && \
    cd /tmp/Crow && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# Baixando e instalando nlohmann-json
RUN wget https://github.com/nlohmann/json/releases/download/v3.10.5/json.hpp -O /usr/local/include/json.hpp

# Clonando e instalando a biblioteca httplib
RUN git clone --depth=1 https://github.com/yhirose/cpp-httplib /tmp/httplib && \
    cp -R /tmp/httplib /usr/local/include/httplib

# Criando o diretório de trabalho
WORKDIR /app

# Copiando o arquivo json.hpp para o diretório de trabalho
RUN cp /usr/local/include/json.hpp /app/json.hpp

RUN apt install nlohmann-json3-dev

# Copiando o código-fonte para o contêiner
COPY . .

# Compilando o código
RUN g++ -std=c++11 -o server server.cpp -I/app -I/usr/local/include/httplib -lboost_system -lpthread

# Expondo a porta que o servidor usará
EXPOSE 8080

# Comando para rodar o servidor
CMD ["./server"]
