/* module3.c: Based on Mazidi etal book. 
 * Initialize and display "EEE212" on the first row 
 * Display the number a on the second raw as decimal and hexadecimal 
 * using 8-bit data mode.
 * Data pins use Port D, control pins use Port A.
 * Then wait until a key pressed and display the typed keys on the f
 * first row after clearing screen
 * The LCD’s data pins are connected to PORTD of the microcontroller.
 * The LCD’s RS pin is connected to Pin 2 of PORTA of the microcontroller.
 * The LCD’s R/W pin is connected to Pin 4 of PORTA of the microcontroller.
 * The LCD’s E pin is connected to Pin 5 of PORTA of the microcontroller.
 * Polling of the busy bit of the LCD status bit is used for timing.
 * Raws of the keypad are connected to PORTC 0-3, Columns are connected to PORTC4-7.
  * pull-ups are enabled
 */

#include <MKL25Z4.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 5

#define RS 0x04     /* PTA2 mask */ 
#define RW 0x10     /* PTA4 mask */ 
#define EN 0x20     /* PTA5 mask */

void Delay(volatile unsigned int time_del);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void LCD_ready(void);
void keypad_init(void);
uint32_t keypad_getkey(void);
uint32_t enter_id(void);
void LCD_print(char input[], int lenght);
uint32_t enter_password(void);
void check_id_password(uint32_t new_id, uint32_t new_pass);
void delete_user(void);
void add_user(void);
char Get_Key(void);
int check_UID(uint32_t new_id);
	
void admin_mode(uint32_t new_id, uint32_t new_pass);

volatile typedef struct {
    uint32_t id;
    uint32_t password;
		uint32_t dolu;
} user_t;

volatile user_t users[MAX_USERS];
uint8_t num_users = 3;
//uint8_t admin_mode = 0;

user_t admin;
int lenght;
int lenght1,lenght2,lenght3,lenght4,lenght5,lenght6,lenght7,lenght8,lenght9,length10,length11,lenght12,lenght13,lenght14,lenght15;
uint32_t new_id,new_pass;
int check;

void LCD_print(char input[],int lenght){
	for (uint8_t i = 0; i < lenght-1; i++) 
	{
		LCD_data(input[i]);
  }
}

char Get_Key(void)
{
            char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
            uint32_t key;

            keypad_init();
            while(keypad_getkey() == 0)
                    {
                    }

                    while(1)
                    {
                    Delay(300000);
                    key=keypad_getkey();
                        if(key != 0)
                        {
                            char inputKey = lookup[key-1];
                            return inputKey;
                    }
}
}
uint32_t enter_id(void) {
		LCD_init();
	  char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	  uint32_t lookupNum[]={1,2,3,0,4,5,6,0,7,8,9,0,0,0,0,0};
    char id[10];
		uint32_t id_num=0;
    uint8_t uz = 0;
		LCD_command(0x80);
		lenght1 = sizeof("Enter id:");
		LCD_print("Enter id:",lenght1);
	  LCD_command(0xC0);
    while (uz < 10) {
        uint32_t index;
				index=keypad_getkey()-1;
        if (index == 14) {
            break;
        }
				else if (lookupNum[index]==0 && !(index==13)){
					continue;
				}
				else{
				id_num=id_num*10+lookupNum[index];
				LCD_data(lookup[index]);
				uz++;
				}
				eski: while(keypad_getkey() != 0){}
				Delay(300000);
				if (keypad_getkey() != 0){goto eski;}
    }
		return id_num;
		/*#uint32_t id_num;
    id_num = atoi(id);
    users[num_users].id = id_num;*/
}

uint32_t enter_password(void) {
    LCD_init();
		lenght2 = sizeof("Enter password:");
	  char lookup[]= {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
	  uint32_t lookupNum[]={1,2,3,0,4,5,6,0,7,8,9,0,0,0,0,0};
    char id[10];
		uint32_t pass_num=0;
    uint8_t uz = 0;
		LCD_command(0x80);
		LCD_print("Enter password:",lenght2);
	  LCD_command(0xC0);
    while (uz < 10) {
        uint32_t index;
				index=keypad_getkey()-1;
        if (index == 14) {
            break;
        }
				else if (lookupNum[index]==0 && !(index==13)){
					continue;
				}
				else{
				pass_num=pass_num*10+lookupNum[index];
				LCD_data(lookup[index]);
				uz++;
				}
				again: while(keypad_getkey() != 0){}
				Delay(300000);
				if (keypad_getkey() != 0){goto again;}
    }
		return pass_num;
}

void check_id_password(uint32_t new_id,uint32_t new_pass) {
		LCD_init();
		lenght3 = sizeof("Access Granted");
		lenght4 = sizeof("Access Denied");
    for (uint8_t i = 0; i < MAX_USERS; i++) {
        if (users[i].dolu == 1 && new_id == users[i].id && new_pass == users[i].password) {
						Delay(200000);
            LCD_print("Access Granted",lenght3);
            return;
        }
    }
		Delay(200000);
    LCD_print("Access Denied",lenght4);
}





int check_UID(uint32_t new_id){
	for(int i =0; i<MAX_USERS; i++){
		if(users[i].dolu == 1 && users[i].id == new_id){
			return 1;
		}
	}
	return 0;
}


int main(void) {
	LCD_init();
	keypad_init();
	LCD_command(0x80);
	users[1].id = 2200;
	users[1].password = 1234; users[1].dolu=1;
	users[2].id = 22002200;
	users[2].password = 3456; users[2].dolu=1;
	users[0].id = 12345; //administor
	users[0].password = 2605; users[0].dolu=1;
	users[3].id = 1007;
	users[3].password = 123;
	users[3].dolu=1;
	users[4].dolu=0;
	lenght14 = sizeof("User is Valid.");
	lenght15 =sizeof("User is not Valid.");
	
	
    while (1) {
			next2:	LCD_init();
		new_id = enter_id();
		Delay(3000000);
		for(int i =0; i<MAX_USERS; i++){
		if(users[i].dolu == 1 && users[i].id == new_id){
			check = 1;
			goto next;
		}
		}
		check = 0;
		next:	Delay(70000);
		if (check == 1){
			LCD_command(0x01);
			LCD_command(0x80);
			LCD_print("User isValid.",lenght14);
			Delay(9000000);
			new_pass = enter_password();
			Delay(3000000);
        if (new_id==12345 && new_pass==2605) {
							LCD_command(0x80);
							lenght5 = sizeof("Enter an Action:");
							lenght6 = sizeof("Pass, Del, Add");
							lenght7 = sizeof("Wrong key");
							LCD_print("Enter an Action:",lenght5);
							LCD_command(0xC0);
							LCD_print("Pass, Del, Add",lenght6);
							Delay(3000000);
							char c;
							c = Get_Key();
							if (c == 'A') {
									lenght3 = sizeof("Access Granted");
									lenght4 = sizeof("Access Denied");
									for (uint8_t i = 0; i < MAX_USERS; i++) {
											if (users[i].dolu == 1 && new_id == users[i].id && new_pass == users[i].password) {
													Delay(200000);
													LCD_print("Access Granted",lenght3);
													goto next2;
											}
									}
									Delay(200000);
									LCD_print("Access Denied",lenght4);
							} else if (c == 'B') {
											length10 = sizeof("Max Capacity Reached");
										length11 = sizeof("User Added");
										lenght13 = sizeof("UID 2 B Added");
										LCD_command(0x01);
										LCD_command(0x80);
										LCD_print("UID 2 B Added",lenght13);
										Delay(4000000);
											if (num_users == MAX_USERS) {
													LCD_command(0x01);
													LCD_command(0x80);
													LCD_print("Max Capacity Reached",length10);
													Delay(3000000);
													goto next2;
											}
										uint32_t added_id;
										uint32_t added_pass;
										LCD_command(0x01);
										LCD_command(0x80);
										added_id = enter_id();
										Delay(3000000);
										added_pass= enter_password();
										Delay(700000);
										int i;
										for(i=0 ; i<MAX_USERS;i++){
											if(users[i].dolu == 0){
												users[i].id = added_id;
												users[i].password = added_pass;
												users[i].dolu = 1;
												break;
											}
										} 
										num_users++;
										LCD_command(0x01);
										LCD_command(0x80);
										LCD_print("User Added",length11);
										Delay(3000000);
										goto next2;
							} else if (c == 'C') {
											LCD_command(0x01);
											LCD_command(0x80);
											uint32_t id_got;
											lenght12 = sizeof("UID 2 B Delete");
											LCD_print("UID 2 B Delete",lenght12);
											Delay(3000000);
											LCD_command(0x01);
											LCD_command(0x80);
											id_got=enter_id();
											LCD_command(0x01);
											LCD_command(0x80);
											lenght8 = sizeof("User Deleted");
											lenght9 = sizeof("User Not Found");
											for (uint8_t i = 0; i < MAX_USERS; i++) {
													if (users[i].dolu==1 && users[i].id == id_got) {
															users[i].dolu = 0;
															users[i].id = 0;
															users[i].password =0;
															num_users--;
															LCD_print("User Deleted",lenght8);
															Delay(3000000);
															goto next2;
													}
											}
											LCD_print("User Not Found",lenght9);
											Delay(3000000);
											goto next2;
							} else {
									LCD_command(0x01);
									LCD_command(0x80);
									LCD_print("Wrong key",lenght7);
									Delay(3000000);
							}
					}
					else{
						check_id_password(new_id,new_pass);
						Delay(3000000);
					}
						Delay(5000000);
				}
         
        
				 else{
			LCD_command(0x01);
			LCD_command(0x80);
			LCD_print("User isnotValid.",lenght15);
			Delay(9000000);
		}
		
     }
		
     return 0;
}








void LCD_init(void)
{
    SIM->SCGC5 |= 0x1000;       /* enable clock to Port D */ 
    PORTD->PCR[0] = 0x100;      /* make PTD0 pin as GPIO */
    PORTD->PCR[1] = 0x100;      /* make PTD1 pin as GPIO */
    PORTD->PCR[2] = 0x100;      /* make PTD2 pin as GPIO */
    PORTD->PCR[3] = 0x100;      /* make PTD3 pin as GPIO */
    PORTD->PCR[4] = 0x100;      /* make PTD4 pin as GPIO */
    PORTD->PCR[5] = 0x100;      /* make PTD5 pin as GPIO */
    PORTD->PCR[6] = 0x100;      /* make PTD6 pin as GPIO */
    PORTD->PCR[7] = 0x100;      /* make PTD7 pin as GPIO */
    PTD->PDDR = 0xFF;           /* make PTD7-0 as output pins */
    
    SIM->SCGC5 |= 0x0200;       /* enable clock to Port A */ 
    PORTA->PCR[2] = 0x100;      /* make PTA2 pin as GPIO */
    PORTA->PCR[4] = 0x100;      /* make PTA4 pin as GPIO */
    PORTA->PCR[5] = 0x100;      /* make PTA5 pin as GPIO */
    PTA->PDDR |= 0x34;          /* make PTA5, 4, 2 as output pins */
    
    LCD_command(0x38);      /* set 8-bit data, 2-line, 5x7 font */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* This function waits until LCD controller is ready to
 * accept a new command/data before returns.
 */
void LCD_ready(void)
{
    uint32_t status;
    
    PTD->PDDR = 0x00;          /* PortD input */
    PTA->PCOR = RS;         /* RS = 0 for status */
    PTA->PSOR = RW;         /* R/W = 1, LCD output */
    
    do {    /* stay in the loop until it is not busy */
			  PTA->PCOR = EN;
			  Delay(500);
        PTA->PSOR = EN;     /* raise E */
        Delay(500);
        status = PTD->PDIR; /* read status register */
        PTA->PCOR = EN;
        Delay(500);			/* clear E */
    } while (status & 0x80UL);    /* check busy bit */
    
    PTA->PCOR = RW;         /* R/W = 0, LCD input */
    PTD->PDDR = 0xFF;       /* PortD output */
}

void LCD_command(unsigned char command)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PCOR = RS | RW;    /* RS = 0, R/W = 0 */
    PTD->PDOR = command;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

void LCD_data(unsigned char data)
{
    LCD_ready();			/* wait until LCD is ready */
    PTA->PSOR = RS;         /* RS = 1, R/W = 0 */
    PTA->PCOR = RW;
    PTD->PDOR = data;
    PTA->PSOR = EN;         /* pulse E */
    Delay(500);
    PTA->PCOR = EN;
}

/* Delay n milliseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */

/* delay n microseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */


void Delay(volatile unsigned int time_del) {
  while (time_del--) 
		{
  }
}


void keypad_init(void)
{
    SIM->SCGC5 |= 0x0800;       /* enable clock to Port C */ 
    PORTC->PCR[0] = 0x103;      /* make PTC0 pin as GPIO and enable pullup*/
    PORTC->PCR[1] = 0x103;      /* make PTC1 pin as GPIO and enable pullup*/
    PORTC->PCR[2] = 0x103;      /* make PTC2 pin as GPIO and enable pullup*/
    PORTC->PCR[3] = 0x103;      /* make PTC3 pin as GPIO and enable pullup*/
    PORTC->PCR[4] = 0x103;      /* make PTC4 pin as GPIO and enable pullup*/
    PORTC->PCR[5] = 0x103;      /* make PTC5 pin as GPIO and enable pullup*/
    PORTC->PCR[6] = 0x103;      /* make PTC6 pin as GPIO and enable pullup*/
    PORTC->PCR[7] = 0x103;      /* make PTC7 pin as GPIO and enable pullup*/
    PTC->PDDR = 0x00;         /* make PTC7-0 as input pins */
}




uint32_t keypad_getkey(void)
{
    uint32_t row, col;
    const char row_select[] = {0x01, 0x02, 0x04, 0x08}; /* one row is active */

    /* check to see any key pressed */
    PTC->PDDR |= 0x0F;          /* rows output */
    PTC->PCOR = 0x0F;               /* ground rows */
    Delay(500);                 /* wait for signal return */
    col =  PTC->PDIR & 0xF0UL;     /* read all columns */
    PTC->PDDR = 0;              /*  rows input */
    if (col == 0xF0UL)
        return 0;               /* no key pressed */

    /* If a key is pressed, it gets here to find out which key.
     * It activates one row at a time and read the input to see
     * which column is active. */
    for (row = 0; row < 4; row++)
    {
        PTC->PDDR = 0;                  /* disable all rows */
        PTC->PDDR |= row_select[row];   /* enable one row */
        PTC->PCOR = row_select[row];    /* drive the active row low */
        Delay(500);                     /* wait for signal to settle */
        col = PTC->PDIR & 0xF0UL;         /* read all columns */
        if (col != 0xF0UL) break;         /* if one of the input is low, some key is pressed. */
    }
    PTC->PDDR = 0;                      /* disable all rows */
    if (row == 4) 
        return 0;                       /* if we get here, no key is pressed */
 
    /* gets here when one of the rows has key pressed, check which column it is */
    if (col == 0xE0UL) return row * 4 + 1;    /* key in column 0 */
    if (col == 0xD0UL) return row * 4 + 2;    /* key in column 1 */
    if (col == 0xB0UL) return row * 4 + 3;    /* key in column 2 */
    if (col == 0x70UL) return row * 4 + 4;    /* key in column 3 */

    return 0;   /* just to be safe */
}
