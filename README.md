# MedidorSonoro
Medidor sonoro que integra pH-metro, termômetro e identificador de soluções eletrolíticas

<p align="center">
  <img src="https://raw.githubusercontent.com/VictorHerbert/MedidorSonoro/master/Images/sketch1.png" alt="Logo"/>
</p>


## DFPlayer Mini

### Prós
  * Nenhum
  
### Contras
  * Utiliza um metódo de indexação oculto, que pode depender de data de criação, tamanho ou nome, ninguém sabe
  * Não presta
  * Fala sério, é impossível usar essa coisa

## Medição de Temperatura

## Amostragem

Em razão da alta instabilidade nas medições, sobretudo nas medições de temperatura, foi implementado um filtro de Média Móvel. Tal filtro é um filtro digital Passa-baixa, i.e. retira altas frequências do sinal discreto amostrado, ou em palavras mais simples, atuena grandes variações na forma de onda amostrada.

<p align="center">
  <img src="https://www.forexmt4indicators.com/wp-content/uploads/2015/01/LaguerreFilter.png" alt="Logo"/>
</p>

Naturalmente, a medição de temperatura exigiu um buffer maior do que a medição de pH, sendo utilzados os valores de 250 e 50 como tamanhos dos vetores de armazenamento
