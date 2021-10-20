#include <16f877a.h>
#use delay(clock=20000000)
#use rs232(baud = 9600, xmit = pin_c6, rcv = pin_c7) // padrão 8N1
#fuses XT,NOWDT,NOPROTECT,PUT,BROWNOUT,NOLVP,NOCPD,NOWRT
#define LCD_RS_PIN PIN_D1
#define LCD_RW_PIN PIN_D2
#define LCD_ENABLE_PIN PIN_D1
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
#include <lcd_2.c>
#byte PIR1 = 0x0c
#bit TMR1IF = 0x0c.0
#byte TMR1H = 0x0f
#byte TMR1L = 0x0e
#byte T1CON = 0x10
#bit TMR1IE = 0x8c.0
#bit GIE = 0x0b.7
#bit PEIE = 0x0b.6
#define COL1 PIN_B0 
#define COL2 PIN_B1 
#define COL3 PIN_B2 
#define LIN1 PIN_B3 
#define LIN2 PIN_B4 
#define LIN3 PIN_B5 
#define LIN4 PIN_B6 

       unsigned int carga,hora,min,day,aux,alg1,alg2,ind1,ind2;
	   int1 flag,flag2;
	   
	   //carga : armazena o valor da carga que o usuário configurou
	   //hora : armazena o valor da hora que o usuário configurou
	   //min : armazena o valor dos minutos que o usuário configurou
	   //aux, alg1, alg2 e flag: são variáveis auxiliares para o nosso programa
	   //ind1 e ind2 são variáveis que vão auxilar na leitura da memória (executar ou não a ação de ligar ou desligar)
		
	    


int le_teclado(){ 
//seleciona coluna1 
output_low(COL1); 
output_high(COL2); 
output_high(COL3); 

if (input(LIN1)==0){return 1;} 
if (input(LIN2)==0){return 4;} 
if (input(LIN3)==0){return 7;} 
if (input(LIN4)==0){return 10;} 

//seleciona coluna2 
output_high(COL1); 
output_low(COL2); 
output_high(COL3); 

if (input(LIN1)==0){return 2;} 
if (input(LIN2)==0){return 5;} 
if (input(LIN3)==0){return 8;} 
if (input(LIN4)==0){return 0;} 

//seleciona coluna3 
output_high(COL1); 
output_high(COL2); 
output_low(COL3); 

if (input(LIN1)==0){return 3;} 
if (input(LIN2)==0){return 6;} 
if (input(LIN3)==0){return 9;} 
if (input(LIN4)==0){return 11;} 

return 99; 
} 



typedef struct
{
  unsigned int8 sec;
  unsigned int8 min;
  unsigned int8 hrs;
  unsigned int8 dia;
  }RTC;
  
RTC time;

void RTC_Init();
void timer_config();
void acionar();
void desligar();
void read_memory_1();
void read_memory_2();
void write_memory_pro();
void write_memory_pro_2();
void print_dia();
void show_clock();
  
#INT_TIMER1
void clock()
  {
               bit_set(TMR1H,7); //Limpa a flag de interrupção
			   time.sec++;                        //******************MOSTRA RELÓGIO*************	  
			 
			if( time.sec == 60 )
			{
				time.sec = 0;
				time.min++;
				read_memory_1();
				read_memory_2();
				if(time.min == 60)
				{
				  time.sec = 0;
				  time.min = 0;
				  time.hrs++;	
				  if(time.hrs==24)
				  {
					 time.hrs=0;
					 if(time.dia<7)
						 {
							 
							 time.dia++;		 
							 
						 }
					 else if(time.dia==7)
						 {
							
							 time.dia=1;
												
						     
				     	 }
				  }	   
				}
		    }
}			


void print_dia(unsigned int a) // Essa função simplesmente imprime o dia selecionado na tela
  {
	 
	  switch(a)
	  {
	     case 1:     
			         printf(lcd_putc,"DOM");                    
			         break;
					 
		case 2:     
			         printf(lcd_putc,"SEG");                    
			         break;
					 
		case 3:     
			         printf(lcd_putc,"TER");                    
			         
		             break;
				 	 
		case 4:      
			         printf(lcd_putc,"QUA");                    
			         
		             break;
		
		case 5:      
			         printf(lcd_putc,"QUI");                    
			         
		             break;
					 
		case 6:      
			         printf(lcd_putc,"SEX");                    
			         
		             break;
					 
		case 7:      
			         printf(lcd_putc,"SAB");                    
			         
		             break;
	}	
}	  
  

void RTC_Init() // Configuração do relógio de tempo real (configura o TIMER1)
{
	setup_timer_1(t1_external);
	enable_interrupts(global | int_timer1);
     T1CON = 0b00001111; //Prescaler 1:1, Oscilador ligado, Clock Externo
     TMR1H = 0x80;  
     TMR1L = 0;     //Carrega valor de (TMR1H:TMR1L) com 32768
     TMR1IE = 1; //habilita interrupção do timer1
     PEIE = 1;  //habilita interrupção dos periféricos
     GIE = 1;  //Habilita interrupção global
}



void timer_config() // Configuração do dia, das horas e dos minutos de acordo com a data atual
{   
	
	
	
	alg1=0;
	alg2=0;
	flag=0;
	
	Printf(lcd_putc,"\fDigite o dia \natual(1 a 7):");
	
	while(flag==0)
	{
		alg1=le_teclado();
		 if(alg1>=1 && alg1<=7)
		 {
			 Printf(lcd_putc,"%d",alg1);
			 delay_ms(500);
			 flag++;
		 }	 
	}
	flag=0;
	time.dia=alg1; // dia
	
	reteste:
	printf(lcd_putc,"\fDigite a hora\natual:");
	
	do{
		
		alg1=le_teclado();
		
		if(alg1>=0 && alg1<=2)
		{
			printf(lcd_putc,"%d",alg1);
			delay_ms(300);
		    flag++;
		}	
	
     }while(flag==0);
	
	flag=0;
	 
	 do{
		
		alg2=le_teclado();
		
		if(alg2>=0 && alg2<=9)
		{
			printf(lcd_putc,"%d",alg2);
			delay_ms(800);
		    flag++;
		}	
	
     }while(flag==0);
	 
	 if(alg1 == 2 && alg2 > 4)
		 {
			goto reteste;
		 }
	 
	 time.hrs = alg1*10 + alg2;  //***********HORAS
			
		
	 
	 flag=0;
	 alg1=0;
     alg2=0;
	 
	 printf(lcd_putc,"\fDigite os\nminutos:");
	 do{
		
		alg1=le_teclado();
		if(alg1>=0 && alg1<=5)
		{
			printf(lcd_putc,"%d",alg1);
			delay_ms(300);
		    flag++;
		}	
	
     }while(flag==0);
	 
	 flag=0;
	 
	 do{
		
		alg2=le_teclado();
		
		if(alg2>=0 && alg2<=9)
		{
			printf(lcd_putc,"%d",alg2);
			delay_ms(800);
		    flag++;
		}	
	
     }while(flag==0);
		
	time.min= alg1*10 + alg2;     //*********minutos
	 
    flag=0;
	alg1=0;
	
	
		
 } 
      
void acionar()    //  configura a carga, o dia, as horas e os minutos que o usuário deseja acionar
{
	   
		
	   
	
		alg1=0;
	    alg2=0;	
		flag =0;
		
		printf(lcd_putc,"\fCarga que deseja\nacionar(1 a 16):");
		delay_ms(800);
		
		
		
		do{
		
			alg1=le_teclado();
			
			if(alg1>=0 && alg1<=1)
			{
			    printf(lcd_putc,"%d",alg1);
				delay_ms(300);
				flag++;
			}
						
		}while(flag==0);
		 
		flag=0;
		
		do{
		
			alg2=le_teclado();
			
			if(alg2>=0 && alg2<=6)
			{
			    printf(lcd_putc,"%d",alg2);
				delay_ms(800);
				flag++;
			}
			
		}while(flag==0);
		
		carga = alg1*10 + alg2; //**************VALOR DA CARGA
		flag=0;
		
		printf(lcd_putc,"\fDia de liga-la\n(1 A 7):");
		delay_ms(800);
		 		
		do{
		    
			aux=le_teclado();
			if( aux>=1 && aux<=7)
			{
				printf(lcd_putc," %d", aux);
				delay_ms(800);
				flag++;
		    }
		    	
			
		
		 }while(flag==0);
		 
		
					
		day=aux; 
	     printf(lcd_putc,"\fO dia seleciona\ndo foi:");
	     print_dia(day); 	  
		 delay_ms(800);
		 printf(lcd_putc,"\fHora que deseja\naciona-la:");
		 		
		 flag=0;
		
		do{
		
		alg1=le_teclado();
		
		if(alg1>=0 && alg1<=2)
		{
			printf(lcd_putc,"%d",alg1);
			delay_ms(300);
		    flag++;
		}	
	
     }while(flag==0);
	
	flag=0;
	 
	 do{
		
		alg2=le_teclado();
		
		if(alg2>=0 && alg2<=9)
		{
			printf(lcd_putc,"%d",alg2);
			delay_ms(800);
		    flag++;
		}	
	
     }while(flag==0);
	 
	 hora = alg1*10 + alg2;
	 
	
		 
	 flag=0;
	 alg1=0;
     alg2=0;
	 printf(lcd_putc,"\fDigite os\nminutos:");
	 
	 do{
		
		alg1=le_teclado();
		
		if(alg1>=0 && alg1<=5)
		{
			printf(lcd_putc,"%d",alg1);
			delay_ms(300);
		    flag++;
		}	
	
     }while(flag==0);
	 
	 flag=0;
	 
	 do{
		
		alg2=le_teclado();
		
		if(alg2>=0 && alg2<=9)
		{
			printf(lcd_putc,"%d",alg2);
			delay_ms(800);
		    flag++;
		}	
	
     }while(flag==0);
	
	    min = alg1*10 + alg2;
	   
		 
		
		printf(lcd_putc,"\fHora configurada\n:%dhrs e %dmin",hora,min);
		delay_ms(800);
	    printf(lcd_putc,"\fCarga configura-\nda:%d",carga);
		delay_ms(800);
	    flag2=0;    ////////************FLAG 2 AQUI!!!!!
        write_memory_pro();
		 
}		

		 

void desligar ()  //  configura a carga, o dia, as horas e os minutos que o usuário deseja desligar
{
	
	   
	
		alg1=0;
	    alg2=0;	
		flag =0;
		
	    printf(lcd_putc,"\fCarga que deseja\ndesligar(1 a 16):");
		delay_ms(800);
		
		
		
		do{
		
			alg1=le_teclado();
			
			if(alg1>=0 && alg1<=1)
			{
			    printf(lcd_putc,"%d",alg1);
				delay_ms(300);
				flag++;
			}
						
		}while(flag==0);
		 
		flag=0;
		
		do{
		
			alg2=le_teclado();
			
			if(alg2>=0 && alg2<=6)
			{
			    printf(lcd_putc,"%d",alg2);
				delay_ms(800);
				flag++;
			}
			
		}while(flag==0);
		
		carga = alg1*10 + alg2; //**************VALOR DA CARGA
		flag=0;
		
		printf(lcd_putc,"\fDia de desliga-\nla(1 a 7):");
		delay_ms(800);
		
		
		do{
		    
			aux=le_teclado();
			if( aux>=1 && aux<=7)
			{
				printf(lcd_putc," %d", aux);
				delay_ms(800);
				flag++;
		    }
		    	
			
		
		 }while(flag==0);
		 
	
					
	   day=aux;   // DIA DE DESLIGAR A CARGA
       printf(lcd_putc,"\fO dia seleciona\ndo foi:");
	   print_dia(day); 		 
		
		  
		 delay_ms(800);
		 printf(lcd_putc,"\fHora que deseja\ndesliga-la:");
		 		
		 flag=0;
		
		do{
		
		alg1=le_teclado();
		
		if(alg1>=0 && alg1<=2)
		{
			printf(lcd_putc,"%d",alg1);
			delay_ms(300);
		    flag++;
		}	
	
     }while(flag==0);
	
	flag=0;
	 
	 do{
		
		alg2=le_teclado();
		
		if(alg2>=0 && alg2<=9)
		{
			printf(lcd_putc,"%d",alg2);
			delay_ms(800);
		    flag++;
		}	
	
     }while(flag==0);
	 
	 hora = alg1*10 + alg2;
	 
	
		 
	 flag=0;
	 alg1=0;
     alg2=0;
	 printf(lcd_putc,"\fDigite os\nminutos:");
	 
	 do{
		
		alg1=le_teclado();
		
		if(alg1>=0 && alg1<=5)
		{
			printf(lcd_putc,"%d",alg1);
			delay_ms(300);
		    flag++;
		}	
	
     }while(flag==0);
	 
	 flag=0;
	 
	 do{
		
		alg2=le_teclado();
		
		if(alg2>=0 && alg2<=9)
		{
			printf(lcd_putc,"%d",alg2);
			delay_ms(800);
		    flag++;
		}	
	
     }while(flag==0);
	
	    min = alg1*10 + alg2;  //********minutos
	   
		 
		
		printf(lcd_putc,"\fHora configurada\n:%dhrs e %dmin",hora,min);
		delay_ms(800);
	    printf(lcd_putc,"\fCarga configura-\nda:%d",carga);
		delay_ms(800);	   
	    flag2=1;  ////////************FLAG 2 AQUI!!!!!
       write_memory_pro();
	
}




void write_memory_pro() // Salva na memória a carga que vai ser ligada ou desligada
{	
	
    if( flag2==0) // ACIONAR
        {
             aux=0;
    
        }
     if( flag2==1) // DESLIGAR
          {
                aux=1;
          }
	if( carga>8){ 	  write_memory_pro_2(); }

switch( carga )
		    {
			case 1:  				    
			      
			        write_eeprom(0, carga);
			        write_eeprom(aux+16,hora);
			        write_eeprom(aux+18,min);
			        write_eeprom(aux+20,day);
			        break;
			
			case 2:  				    
			     
			        write_eeprom(1, carga);
			        write_eeprom(aux+22,hora);
			        write_eeprom(aux+24,min);
			        write_eeprom(aux+26,day);
			        break;
			case 3: 				    
			      
			        write_eeprom(2, carga);
			        write_eeprom(aux+28,hora);
			        write_eeprom(aux+30,min);
			        write_eeprom(aux+32,day);
			        break;
			case 4: 			    
			      
			        write_eeprom(3, carga);
			        write_eeprom(aux+34,hora);
			        write_eeprom(aux+36,min);
			        write_eeprom(aux+38,day);
			        break;
			case 5: 			    
			      
			        write_eeprom(4, carga);
			        write_eeprom(aux+40,hora);
			        write_eeprom(aux+42,min);
			        write_eeprom(aux+44,day);
			        break;
			case 6: 	    
			        
			        write_eeprom(5, carga);
			        write_eeprom(aux+46,hora);
			        write_eeprom(aux+48,min);
			        write_eeprom(aux+50,day);
			        break;
			case 7: 			    
			       
			        write_eeprom(6, carga);
			        write_eeprom(aux+52,hora);
			        write_eeprom(aux+53,min);
			        write_eeprom(aux+54,day);
			        break;
			case 8: 				    
			        
			        write_eeprom(7, carga);
			        write_eeprom(aux+56,hora);
			        write_eeprom(aux+58,min);
			        write_eeprom(aux+60,day);
			        break;
			
          }
		  
}

void write_memory_pro_2() // continuação da função anterior
{
	
	switch(carga)
	{
        	case 9: 			    
			   
			        write_eeprom(8, carga);
			        write_eeprom(aux+62,hora);
			        write_eeprom(aux+64,min);
			        write_eeprom(aux+66,day);
			        break;
			
			case 10: 			    
			        
			        write_eeprom(9, carga);
			        write_eeprom(aux+68,hora);
			        write_eeprom(aux+70,min);
			        write_eeprom(aux+72,day);
			        break;
			
			case 11: 				    
			        
			        write_eeprom(10, carga);
			        write_eeprom(aux+74,hora);
			        write_eeprom(aux+76,min);
			        write_eeprom(aux+78,day);
			        break;
			
			case 12:  				    
			       
			        write_eeprom(11, carga);
			        write_eeprom(aux+80,hora);
			        write_eeprom(aux+82,min);
			        write_eeprom(aux+84,day);
			        break;
			
			case 13: 				    
			       
			        write_eeprom(12, carga);
			        write_eeprom(aux+86,hora);
			        write_eeprom(aux+88,min);
			        write_eeprom(aux+90,day);
			        break;
			
			case 14: 			    
			        
			        write_eeprom(13, carga);
			        write_eeprom(aux+92,hora);
			        write_eeprom(aux+94,min);
			        write_eeprom(aux+96,day);
			        break;
			
			case 15: 			    
			        
			        write_eeprom(14, carga);
			        write_eeprom(aux+98,hora);
			        write_eeprom(aux+100,min);
			        write_eeprom(aux+102,day);
			        break;
			
			case 16:       
			        write_eeprom(15, carga);
			        write_eeprom(aux+104,hora);
			        write_eeprom(aux+108,min);
			        write_eeprom(aux+110,day);
			        break;
	
		}
	
	
}
	

void read_memory_1() //essa função compara os dados do relógio com o do usuário. Caso os dados sejam iguais a carga será acionada
{
	
	    unsigned int a,b,c,d;
	    if(ind1>15){ind1=0;}
		a=read_eeprom(ind1);
	 	switch(a)
		 {
			 case 1: b=read_eeprom(16); //horas 
		             c=read_eeprom(18); // minutos
			         d=read_eeprom(20); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     output_high(pin_b7);
					 }	
				
					 break;
			 case 2: b=read_eeprom(22); //horas 
		             c=read_eeprom(24); // minutos
			         d=read_eeprom(26); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }
				 
					 break;
					 
			 case 3: b=read_eeprom(28); //horas 
		             c=read_eeprom(30); // minutos
			         d=read_eeprom(32); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
				
					 break;
					 
			 case 4: b=read_eeprom(34); //horas 
		             c=read_eeprom(36); // minutos
			         d=read_eeprom(38); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
					
					 break;
					 
			 case 5: b=read_eeprom(40); //horas 
		             c=read_eeprom(42); // minutos
			         d=read_eeprom(44); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
				 
					 break;
			
			 case 6: b=read_eeprom(46); //horas 
		             c=read_eeprom(48); // minutos
			         d=read_eeprom(50); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }
				
					 break;	
			 case 7: b=read_eeprom(52); //horas 
		             c=read_eeprom(54); // minutos
			         d=read_eeprom(56); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
					
					 break;
			 case 8: b=read_eeprom(58); //horas 
		             c=read_eeprom(60); // minutos
			         d=read_eeprom(62); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	 
					 break;
			 case 9: b=read_eeprom(64); //horas 
		             c=read_eeprom(66); // minutos
			         d=read_eeprom(68); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	 
				
					 break;
			 case 10: b=read_eeprom(70); //horas 
		             c=read_eeprom(72); // minutos
			         d=read_eeprom(74); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	 
				
					 break;
			 case 11: b=read_eeprom(76); //horas 
		             c=read_eeprom(78); // minutos
			         d=read_eeprom(80); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	 
					 break;
			 case 12: b=read_eeprom(82); //horas 
		             c=read_eeprom(84); // minutos
			         d=read_eeprom(86); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
					
					 break;
			 case 13: b=read_eeprom(88); //horas 
		             c=read_eeprom(90); // minutos
			         d=read_eeprom(92); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }
				
					 break;
			 case 14: b=read_eeprom(94); //horas 
		             c=read_eeprom(96); // minutos
			         d=read_eeprom(98); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
					
					 break;
			 case 15: b=read_eeprom(100); //horas 
		             c=read_eeprom(102); // minutos
			         d=read_eeprom(104); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }	
					
					 break;
			 case 16: b=read_eeprom(106); //horas 
		             c=read_eeprom(108); // minutos
			         d=read_eeprom(110); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_high();
					 }
			    	
					 break;		 
		 }
	 
	 ind1=ind1++;
}


void read_memory_2() //essa função compara os dados do relógio com o do usuário. Caso os dados sejam iguais a carga será desligada
{
	
	unsigned int a,b,c,d;
	
        if(ind2>15){ind2=0;}
	   
	     a=read_eeprom(ind2);
		
		switch(a)
		 {
			 case 1: b=read_eeprom(17); //horas 
		             c=read_eeprom(19); // minutos
			         d=read_eeprom(21); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     output_low(pin_b7);
					 }	 
					 
					 break;
			 case 2: b=read_eeprom(23); //horas 
		             c=read_eeprom(25); // minutos
			         d=read_eeprom(27); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	 
					 
					 break;
					 
			 case 3: b=read_eeprom(29); //horas 
		             c=read_eeprom(31); // minutos
			         d=read_eeprom(33); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_low();
					 }	 
					 read_memory_1();
					 break;
					 
			 case 4: b=read_eeprom(35); //horas 
		             c=read_eeprom(37); // minutos
			         d=read_eeprom(39); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	
					
					 break;
					 
			 case 5: b=read_eeprom(41); //horas 
		             c=read_eeprom(43); // minutos
			         d=read_eeprom(45); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_low();
					 }	 
					 
					 break;
			
			 case 6: b=read_eeprom(47); //horas 
		             c=read_eeprom(49); // minutos
			         d=read_eeprom(51); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	 
					
					 break;	
			 case 7: b=read_eeprom(53); //horas 
		             c=read_eeprom(55); // minutos
			         d=read_eeprom(57); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	 
					 break;
			 case 8: b=read_eeprom(59); //horas 
		             c=read_eeprom(61); // minutos
			         d=read_eeprom(63); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_low();
					 }	 
					
					 break;
			 case 9: b=read_eeprom(65); //horas 
		             c=read_eeprom(67); // minutos
			         d=read_eeprom(69); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	
					
					 break;
			 case 10: b=read_eeprom(71); //horas 
		             c=read_eeprom(73); // minutos
			         d=read_eeprom(75); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	 
					 
					 break;
			 case 11: b=read_eeprom(77); //horas 
		             c=read_eeprom(79); // minutos
			         d=read_eeprom(81); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_low();
					 }	 
					 
					 break;
			 case 12: b=read_eeprom(83); //horas 
		             c=read_eeprom(85); // minutos
			         d=read_eeprom(87); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }
				    
					 break;
			 case 13: b=read_eeprom(89); //horas 
		             c=read_eeprom(91); // minutos
			         d=read_eeprom(93); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_low();
					 }
				
					 break;
			 case 14: b=read_eeprom(95); //horas 
		             c=read_eeprom(97); // minutos
			         d=read_eeprom(99); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }	
					
					 break;
			 case 15: b=read_eeprom(101); //horas 
		             c=read_eeprom(103); // minutos
			         d=read_eeprom(105); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					    //output_low();
					 }
				     read_memory_1();	 
					 break;
			 case 16: b=read_eeprom(107); //horas 
		             c=read_eeprom(109); // minutos
			         d=read_eeprom(111); //dia
			         if(time.dia==d && time.hrs==b && time.min==c)
					 {
					     //output_low(pin_b7);
					 }
				    
					 break;		 
		 }        
	
	 ind2++;; //ind2 tem que ir até 121
}	

void menu() // interface de interação com o usuário
{
	   	    
	   unsigned int32 u;
	   u=0;
	   printf(lcd_putc"\f0-mostrar relogio\n");
	   printf(lcd_putc"1-acionar carga");
	   while(u!=130000)
	   {
           u++;
		   alg1=le_teclado();		   
		   if(alg1==0 || alg1==1 || alg1==2){u=130000;}
	    }   
	   printf(lcd_putc"\f1-acionar carga\n");
	   printf(lcd_putc"2-desligar carga");
       
	   while(u!=0)
	   {
		    u--;
		   alg1=le_teclado();		   
		   if(alg1==0 || alg1==1 || alg1==2){u=0;}
		  
	   }  
	 
	  
	   switch(alg1)
	   {
		   case 0: lcd_putc("\f");
			       show_clock();
		           break;
		   
		   case 1: lcd_putc("\f");
			       acionar();
		            break;
		   
		  case 2: lcd_putc("\f");
				  desligar();
		          break;
              
       }	

} 
 
 
void show_clock() // mostra relógio
 {         
	          lcd_putc("\f");
	          print_dia(time.dia);
	          while(alg1!=11)
			  {
	          printf(lcd_putc,"\n");
	          if(time.hrs>9){printf(lcd_putc, " %uh",  time.hrs);}
			  else{printf(lcd_putc, "\n  0%uh",  time.hrs);}
			  if(time.min>9){printf(lcd_putc, " %umin",  time.min);}
			  else{printf(lcd_putc, " 0%umin",  time.min);}
			  if(time.sec>9){printf(lcd_putc, " %us",  time.sec);}
			  else{printf(lcd_putc, " 0%us",  time.sec);}
			  alg1=le_teclado();
		      }
}  
 void main() // função principal
{
	ind1=0;
	ind2=0;
	port_b_pullups(true);
	lcd_init();	
	rtc_init();
	timer_config();
	
	

	lcd_putc("\f");
	print_dia(time.dia);	
	while(TRUE)
	{
		      
          menu();
     	
	}	  
	




}


 

