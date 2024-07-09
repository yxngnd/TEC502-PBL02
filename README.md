# TEC502-PBL02
Problema 2 da matéria TEC502 - Concorrência e Conectividade: Transações Bancárias Distribuídas.

### Sumário 
+ [Como Executar](#como-executar-a-solução)
+ [Introdução](#introdução)
+ [Produto Desenvolvido](#produto-desenvolvido)
+ [Gerenciamento de Contas](#gerenciamento-de-contas)
+ [Comunicação](#protocolo-de-comunicação)
+ [Algoritmo de Concorrência](#algoritmo-de-concorrencia)
+ [Tratamento da Confiabilidade](#tradatamento-da-confiabilidade)

## Como Executar:

A solução para o problema, como requerida, está no formato de um imagem Docker. Assim, para que seja feito o uso da solução é necessário possuir o Docker instalado na máquina e executar os seguintes comandos:

Execute o seguinte comando para obter a imagem do servidor:
```bash
docker pull yxngnd/bank_server:latest
```
Execute a imagem e substitua os campo *bankx_host* pelo ip do server de cada banco, o formato do ip deve sero seguinte: ip:8080, a porta 8080 deve ser sempre a utilizada devido a forma que o produto foi desenvolvido.
```bash
docker run -it --network=host -e BANK1={bank1_host} -e BANK2={bank2_host} -e BANK3={bank3_host} yxngnd/bank_server
```

## Introdução

É notável o crescimento do número de clientes de bancos que têm dado preferência pelo uso de serviços bancários em dispositivos móveis, tanto pela praticidade, quando pela facilidade. Com isso, em 2020 foi desenvolvido e lançado o Pix, pelo Banco Central do Brasil, uma plataforma de pagamento que permite uma rápida execução de pagamentos e transferências através de serviços de banco online. Tendo isso em mente, foi solicitado o desenvolvimento de uma plataforma que, de forma semelhante ao pix, permitisse que transferências fossem feitas de forma mais fácil em um consórcio de bancos descentralizados, ou seja, em uma situação onde não existe um banco central para fazer o gerenciamento dos mesmos.

A solução para o produto possui alguns requerimentos: a capacidade de criação de contas, onde poderia ser feito depósito e saque para a mesma, a visualização de todas as contas vinculadas ao cpf que está logado no momento, independente de qual dos bancos do consórcio ela está, e a realização de transferências para qualquer conta do consórcio, podendo até mesmo selecionar mais de uma conta do mesmo cliente para que seja dividido o valor total que seria transferido ou destinatário.    

O sistema em si foi desenvolvido por completo utilizando a linguagem de programação C++, fazendo uso da biblioteca **Crow** para desenvolver a API com auxílio da biblioteca **httplib** para algumas requisições *HTTP* internas. Por fim, a interface da aplicação foi desenvolvida com HTML, CSS e JavaScript.

## Produto Desenvolvido

O produto finalizado finalizado consiste em um servidor que faz todo gerenciamento das entidades *Account* e *Bank*, funcionando como uma API Restful, disponibilizando rotas e lidando com as requisições feitas pelo cliente através de uma interface. O consórcio, que é a comunicação feita entre três servidores, consite em três bancos que comunicam-se entre si através do protocolo HTTP utilizando endereços já conhecidos por cada um deles.


![ComunicacaoBancos](IMG/bancos.png)
- **Figura 1:** *Diagrama da comunicação entre os bancos.*

## Gerenciamento de Contas
Para fazer o gerenciamento de contas foram implementadas as funcionalidades de cadastro, login, depósito, saque e transferência, acessadas através de requisições na API, possibilitando que o cliente crie e acesse suas contas através da interface da aplicação, além de manipular o saldo das contas ao retirar, adicionar ou transferir dinheiro para outra conta.

O sistem faz uso de uma tela única para que o cliente faça o gerenciamento das suas contas, nessa tela é exibida cada conta que o cliente possui vinculada ao seu CPF, independente de qual banco esteja, as operações de saque e depósito são feitas apenas na conta do banco que o cliente está logado no momento, porém, na operação de transferência o cliente pode selecionar uma ou mais de suas contas, junto ao valor que vai ser retirado de cada uma delas para que seja enviado à conta destino.

## Comunicação

Para fazer a comunicação tanto da interface com o servidor do banco, quanto a comunicação entre banco foi utilizado o protocolo HTTP, um define os padrões para troca de dados entre programas de aplicação, essenciais para comunicação feita entre aplicaçãos em diferentes dispositivos, sendo desenvolvida uma API Restful, permitindo que sejam feitas requisições à API através da interface do cliente. Ao realizar alguma das operações na interface, uma solicitação é enviada a API e o servidor fica responsável por fazer o tratamento adequado e retornar uma mensagem de confirmação que pode ou não retornar dados, ou uma mensagem de erro em caso de falha de uma das operações.

As rotas desenvolvidas para cada uma das operações estão demonstradas na tabela a seguir:

| Rota          | Método        | Envio         | Resposta      |   
| ------------- | ------------- | ------------- | ------------- |
| "/login"      |      POST     |JSON: {"cpf": string, "password": string}|status: 200 - JSON: {"name": string, "cpf": string, "type": boolean, "balance": double}
status: 400
status: 401 - message: "Invalid credentials"|
| "/register"   |      POST     |JSON: {"name": string, "cpf": string, "type": boolean, "password": string}|status: 200 - message: "Account created successfully"
status: 400
status: 400 - message: "Error when creating account"|
| "/account/cpf"|      GET      |PARAMS: cpf=string|status: 200 - JSON: {[{"cpf": string, "bank": string, "balance": double}]}|
| "/accounts"   |      GET      |PARAMS: cpf=string|status: 500 - message: "Invalid JSON response from bank"
status: 200 - JSON: {["bank_name": string, "account": {"cpf": string, "bank": string, "balance": double}]}
status: 404 - message: "No accounts found in consortium for the given CPF"|
| "/deposit"    |      POST     |JSON: {"cpf": string, "value": double}|status: 400 - message: "Invalid JSON"
status: 200 - message: "Deposit make with success"
status: 404|
| "/withdraw"   |      POST     |JSON: {"cpf": string, "value": double}|status: 400 - message: "Invalid JSON"
status: 200 - message: "withdraw make with success"
status: 404|
| "/transfer"   |      POST     |JSON: {"senders": [{"cpf": string, "bank": string, "balance": double}], "receiver": {"cpf": string, "bank": string,"value": double}}|status: 200 - message: "Transfer completed successfully"
status: 400
status: 404 - message: "Transfer cannot be completed: deposit failed"|





















