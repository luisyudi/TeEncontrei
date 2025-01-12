# Projeto - Te Encontrei

## Descrição

O projeto busca oferecer uma maneira de obter a localização de uma criança a partir de dois dispositivos, uma bolsa para o responsável, e um tênis para a criança. O projeto utiliza dois ESP32 para os vestíveis. Desenvolvido na disciplina ACH2157 - Computação Física e Aplicações com o professores Nakano, Isabel, Marcicano e Dib.

Integrantes do Grupo: Andre Morales, Luis Yudi, Rafaela Cossielo e Rebeca Oliveira.

### Lista de Materiais

| Quantidade | Componente |
| :---: | :---: |
| 2 | Placa de desenvolvimento ESP32 |
| 2 | Cabo USB |
| 2 | Módulo LoRa SX1276 |
| 22 | Jumpers |
| 8 | LEDs |

## Ferramentas de Desenvolvimento

Foram utilizadas as ferramentas Arduino IDE para programação dos ESP's, e o MIT App Inventor para a elaboração do aplicativo mobile

## Comunicação

![modelo](/img/modelo.png)

A comunicação entre os ESP ocorrem via pacotes do módulo LoRa, enviados constantemente pelo Tênis. A bolsa recebe os pacotes, e baseado no RSSI, calcula uma distância em metros e envia via Bluetooth para o aplicativo. O aplicativo também podem enviar mensagens à bolsa. 
Quando necessário, a bolsa envia mensagens de volta para o tênis, indicando o estado em que ela deve ficar.