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

Os termistores são resistores não lineares que alteram suas características de resistência com a temperatura. Simplificando, à medida que a temperatura aumenta, a resistência do termistor diminui.
Existem de dois tipo, NTC (Negative Temperature Coefficient) cuja de variação de resistência com a temperatura é negativo, portanto a resistência diminui com o aumento da temperatura, e o PTC (Positive Temperature Coefficient) cujo funcionamento é analogamente inverso. O valor de resistência do termistor pode variar em uma determinada faixa de temperatura dependendo da sua curva característica. Entretanto eles podem servir de proteção contra o sobreaquecimento, limitando a corrente elétrica quando uma determinada temperatura é ultrapassada.

### Metodo do fator beta
A maneira pela qual a resistência de um termistor diminui está relacionada a uma constante conhecida na indústria de termistor como beta (β). O valor beta de um termistor NTC é medido em graus Kelvin (K) e é calculado usando apenas duas temperaturas em um determinado intervalo.

1T=1T0+1Bln RR0 
 
Onde:
 
T= Temperatura medida
T0= Temperatura nominal do termistor
β= Coeficiente Beta
R= Resistência medida
R0= Resistência nominal do termistor


Na prática a equação acima só funciona para faixas pequenas de variação de temperatura, como a obtida no projeto. Porém, para faixas maiores de variação de temperatura a equação de Steinhart-Hart apresenta uma precisão maior, que usa três temperaturas em um determinado intervalo.

1T=a+b lnR+c ln3(R)


Onde:
 
T= Temperatura medida
a,b,c= Coeficientes específicos de cada termistor
R= Resistência medida

Para este protótipo, tendo em vista a baixo nível de complexidade exigido, o valor de temperatura foi tido através da equação do fator beta, com o coeficiente discriminado no datasheet do fabricante.


## Amostragem

Em razão da alta instabilidade nas medições, sobretudo nas medições de temperatura, foi implementado um filtro de Média Móvel. Tal filtro é um filtro digital Passa-baixa, i.e. retira altas frequências do sinal discreto amostrado, ou em palavras mais simples, atuena grandes variações na forma de onda amostrada.

<p align="center">
  <img src="https://www.forexmt4indicators.com/wp-content/uploads/2015/01/LaguerreFilter.png" alt="Logo"/>
</p>

Naturalmente, a medição de temperatura exigiu um buffer maior do que a medição de pH, sendo utilzados os valores de 250 e 50 como tamanhos dos vetores de armazenamento
