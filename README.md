# Estação IRIS
A Estação IRIS é a principal divisão do Projeto IRIS.
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
### GateIRIS -> [GateIRIS](https://github.com/SI-ART/GateIRIS).
### Desenvolvedor -> [Daniel Arndt](https://github.com/Chimeric-arch).
-------------------------------------------------------------------------------
### IRIS Station  -> [IRIS Station](https://github.com/SI-ART/Station-IRIS).
### Desenvolvedor -> [Daniel Arndt](https://github.com/Chimeric-arch).
-------------------------------------------------------------------------------
### IRIS App -> [IRIS App](https://github.com/SI-ART/IRIS_APP).
### Desenvolvedor -> [João Vitor](https://github.com/jajao1).
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
## BLUETOOTH LOW ENERGY
A configuração inicial da Estação *IRIS* é feita através do protocolo *Bluetooth Low Energy* (BLE), um protocolo de comunicação sem fio, caracterizado pelo seu baixo consumo de energia. Nesta etapa, a *IRIS* precisa receber e enviar as seguintes informações para ser conectada: 

- Receber o ChipID do GateIRIS a ser conectado;

- Enviar ChipID local;

As informações acima são compartilhadas a partir da seguinte conexão: A *IRIS* cria um servidor bluetooth para possibilitar a comunicação com o aplicativo *IRIS* (*clientAPP*).

-------------------------------------------------------------------------------
#### PROCEDIMENTOS DA COMUNICAÇÃO
Para uma comunicação estável, discreta e segura entre o *clientAPP* e a Estação *IRIS*, foram desenvolvidos procedimentos que facilitam a configuração da *IRIS* para o usuário:

1. O processo inicia pela conexão entre o *clientAPP* e o servidor *BLE* da *IRIS*. Não havendo conexão, ela permanece em espera.
- Função:
> waitingBT();            // Espera conexão com clientAPP

2. A *IRIS* recebe o *byte* de sincronização.
- Função:
> waitingSYNC();          // Espera sincronização do clientAPP

3. Uma requisição automática é enviada ao cliente para que este identifique se o dispositivo a ser conectado é ou não uma Estação *IRIS*. 
- Função:
> sendREQUEST();          // Envia requsição para o clientAPP

4. Quando o *clientAPP* receber a validação irá retornar uma requisição à *IRIS*, para validar o cliente autorizado. Caso a validação for inconsistente, o cliente será desconectado.
- Função:
> waitingREQUEST();       // Espera para receber o request do clientAPP

5. Com o sucesso das etapas, a *IRIS* irá esperar pelo envio e recebimento das crêdenciais necessárias.
- Função:
>  waitingSSID_WIFI();     // Espera clientAPP enviar SSID da rede
  
>  waitingPASSWORD_WIFI(); // Espera clientAPP enviar PASSWORD da rede


6. A Estação *IRIS* estará configurada para o funcionamento.

Segue um código de exemplo mostrando mais detalhes sobre os procedimentos de comunicação através do [Bluetooth Low Energy](https://github.com/SI-ART/GateIRIS/blob/main/BLE_GateIRIS/BLE_GateIRIS.ino).

-------------------------------------------------------------------------------
#### SENHAS
Para que o processo de requisição funcione corretamente, foram definidas as seguintes senhas:
|          SENHA            |       DESCRIÇÃO       |
|---------------------------|-----------------------|
|   hHo}8j<Sf(:4mTQW8W#l    |     Recebe do app     |
|   &i((3,iH0,+-z[i]PpAh    |    Envia para o app   |
> Tabela 1.
-------------------------------------------------------------------------------
#### FLAGS DE COMUNICAÇÃO
Para que as funções apresentadas funcionem corretamente, foram definidas as seguintes *flags*:
|  FLAG | DESCRIÇÃO                                |
|:-----:|------------------------------------------|
|  *@*  | *Início da comunicação*                  |
|  */*  | *APP pode enviar o ID de usuário*        |
|   *   | *APP pode enviar o ID da estação*        |
|  *~*  | *APP pode enviar o SSID da rede*         |
|  *^*  | *APP pode enviar a senha da rede*        |
|  *!*  | *Não foi possível conectar no WIFI*      |
|  *&*  | *IRIS foi conectada no WIFI com sucesso* |
> Tabela 2.
-------------------------------------------------------------------------------
-------------------------------------------------------------------------------
## Documentação sobre partições ESP32

[Espressif](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/partition-tables.html).

[PlatformIO](https://docs.platformio.org/en/latest/platforms/espressif32.html#partition-tables).

[Partições pré-definidas](https://github.com/espressif/arduino-esp32/tree/master/tools/partitions).
