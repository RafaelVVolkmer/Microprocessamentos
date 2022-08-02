# ARM (SMT34 - NÚCLEO 64) | MICROPROCESSADOR
![ARM](https://i.imgur.com/IiONPyd.jpeg)

O núcleo-STM34 é um microprocessador, capaz de controlar periféricos através de registradores, que podem receber ou enviar sinais, fazendo comunicações entre software e hardware, ou seja, conectando códigos programados com componentes eletrônicos analógicos e ou digitais. Para isso, o núcleo deve estar conectado por um cabo USB mini-b e programado através da linguagem C e da biblioteca stmf4xx.h em um programa adequado, capaz de fazer a comunicação com o microcontrolador.

O microprocessador é o pequeno CI do meio, enquanto todos os outros componentes na placa são, ou periféricos, como os pinos de conexão, ou ligações seriais. O chip sempre guarda o último código processado nele em sua memória, e é capaz de resproduzi-lo sem precisar estar com o programa aberto, apenas liga-lo na energia, através do cabo, por isso, nos códigos, é sempre recomendável zerar os pinos que serão usados em seu programa, para, caso haja uma memória guardada, não interferir no novo código.

Para esses códigos aplicados aqui, usei o modelo STM32F446RE, também aplicáveis ao STM32F411RE, ambos cedidos pela STMicroeletronics. Não garanto que a comunicação funcione com outros modelos, e, caso funcione, também não garanto que seja o código com maior desempenho para o modelo em questão. As bibliotecas da STM32 e a programação para o ARM são compatíveis com outros microcontroladores, como o arduino, apenas com modelos da mesma distribuidora.

### Datasheets dos dois modelos usados em nossas aulas:
[Datasheet Núcleo-STM32F446xx](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)

[Datasheet Núcleo-STM32F411xx](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)

> Para mais informações e modelos, acesse: [STMicroeletronics](https://www.st.com/content/st_com/en.html)

> Esses produtos são importados e podem ser adquiridos com facilidade por sites de compra na internet, com o preço variando de 100 a 300 reais. Eu, partiularmente, recomendo a aquisição via Aliexpress, onde encontrará os preços mais em conta, sendo o núcleo-11 a opção mais barata, porém, com menos funcionalidade, o que me faz optar, também, pelo núcleo-46.
> 
# PINAGEM
![ARM](https://i.imgur.com/W6CxsfE.png)

# REGISTRADORES

MODER

ODR

IDR

PUPDR

TIM



