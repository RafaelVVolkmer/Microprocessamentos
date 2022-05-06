# O QUE FOI PROPOSTO
- Conecte ao STM32F4xx quatro chaves (ou botões) e um display de 7 segmentos. Os 4 bits correspondentes às chaves formam um dígito hexadecimal;
- Identifique a posição (peso) de cada uma das chaves na formação deste número;
- Elabore um programa que exiba no display o valor numérico do dígito hexadecimal correspondente ao valor expresso pelas chaves;
- O valor deve ser expresso na notação hexadecimal.

# ESQUEMÁTICO DO CIRCUITO
![Esquemático](https://i.imgur.com/xaW56bj.png)
> Caso o Display for anodo, deve-se conectar o ultimo pino no gnd, como mostrado na imagem; caso for catado, ele irá no +3,3v, como as chaves.

> O código já mostra o que alterar para mudar a programação de catodo para anodo, então é bem intuitivo.

# ESQUEMÁTICO DO ARM (SMT34 - NÚCLEO 64)
![ARM](https://i.imgur.com/W6CxsfE.png)

# PROGRAMAÇÃO
> Toda a programação foi feita usando a biblioteca stm32f4xx.h
