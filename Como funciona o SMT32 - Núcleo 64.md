# ARM (SMT34 - NÚCLEO 64) | MICROPROCESSADOR
![ARM](https://i.imgur.com/IiONPyd.jpeg)

O núcleo-STM34 é um microprocessador, capaz de controlar periféricos através de registradores, que podem receber ou enviar sinais, fazendo comunicações entre software e hardware, ou seja, comunicação de códigos programados com componentes eletrônicos analógicos e ou digitais. Para isso, o núcleo deve estar conectado por um cabo USB mini-b e programado através da linguagem C e da biblioteca stmf4xx.h em um programa adequado, capaz de fazer a comunicação com o microcontrolador.

O microprocessador é o pequeno CI do meio, enquanto todos os outros componentes na placa são, ou periféricos, como os pinos de conexão, ou ligações. O chip sempre guarda o último código processado nele em sua memória, e é capaz de resproduzi-lo sem precisar estar com o programa aberto, apenas liga-lo na energia, através do cabo, por isso, nos códigos, é sempre recomendável zerar os pinos que serão usados em seu programa, para, caso haja uma memória guardada, não interferir no código anterior.

Para esses códigos aplicados aqui, usei o modelo STM32F446RE, mas esses também são aplicáveis ao STM32F411RE, ambos cedidos pela STMicroeletronics. Não garanto que a comunicação funcione com outros modelos, e, caso

[Datasheet Núcleo-STM32F446RE](https://www.st.com/resource/en/datasheet/stm32f446re.pdf)

[Datasheet Núcleo-STM32F411RE](https://www.st.com/resource/en/datasheet/stm32f411re.pdf)

# REGISTRADORES

MODER

ODR

IDR

PUPDR

TIM


# PINAGEM
![ARM](https://i.imgur.com/W6CxsfE.png)
