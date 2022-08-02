# ARM (SMT34 - NÚCLEO 64) | MICROPROCESSADOR
![ARM](https://i.imgur.com/IiONPyd.jpeg)

O núcleo-STM34 é um microprocessador, capaz de controlar periféricos através de registradores, que podem receber ou enviar sinais, fazendo comunicações entre software e hardware, ou seja, comunicação de códigos programados com componentes eletrônicos analógicos e ou digitais. Para isso, o núcleo deve estar conectado por um cabo USB mini-b e programado através da linguagem C e da biblioteca stmf4xx.h em um programa adequado, capaz de fazer a comunicação com o microcontrolador.

O microprocessador é o pequeno CI do meio, enquanto todos os outros componentes na placa são, ou periféricos, como os pinos de conexão, ou ligações. O chip sempre guarda o último código processado nele em sua memória, e é capaz de resproduzi-lo sem precisar estar com o programa aberto, apenas liga-lo na energia, através do cabo, por isso, nos códigos, é sempre recomendável zerar os pinos que serão usados em seu programa, para, caso haja uma memória guardada, não interferir no código anterior.


  
# REGISTRADORES

MODER

ODR

IDR

PUPDR

TIM


# PINAGEM
![ARM](https://i.imgur.com/W6CxsfE.png)
