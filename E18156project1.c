/*
* CO222: Programming Methodology
* Project 1: Digital Clock - Specification
*
* E/18/156
* JAYATHILAKE W.A.T.N.
*/

#include <stdio.h>
#include <time.h>         // for time
#include <unistd.h>	  // for sleep()
#include <stdlib.h>
#include <ctype.h>        // tolower()
#include <string.h>       // strcpy()
#include <signal.h>       // for signal handling

void sighandler(int);
int checkColorIn(char *s);
int setColor( char * arg);
void restoreConsole(void);
void printTime(int Ele, int position, int clrCode);
char* toLower(char *ch);


int clr = 47 ;
int STOP, found = 0;
char clockArray[10];
char check[100];
char dateArray[10];
char sub[10];


char * colors[] = {"black", "red", "green","yellow","blue","magenta","cyan","white"};
int len = sizeof(colors)/sizeof(colors[0]),i;


int main(int argc, char ** argv){
	
// ******************  Error handling part

	if ( argc == 1 || strcmp(argv[1],"--") == 0);	// No argument and when 1st is  --  default color

	for (int i = 1; i < argc ; i++ ){                // Go through argumant list
		
		if ( argv[i][0] == '-' ){  // Identify a flag

			if ( (argv[i][1] == '-') && strlen(argv[i]) > 2 ) {                   // when first flag is --xx type
				printf("Invalid use of arguments.\n");
				printf("usage : clock -h                quick help on cmd\n");
				printf("usage : clock -c <color>        print clock with a color\n");
				return 0;
			}

			if ( (argv[i][1] == '-') || strcmp(argv[i],"-")==0 )                    // when first falg is -- or - print in default
				break;

			if (argv[i][1] == 'h')  {                                             // when first flag is -h
				printf("usage : clock -h                quick help on cmd\n");
				printf("usage : clock -c <color>        print clock with a color\n");
				printf("<color-black|red|green|yellow|blue|magenta|cyan|white>  supported colors\n");
				return 0;
			}

			if ( (argv[i][1] != 'c') &&  (argv[i][1] != '-') ){                  // when first flag is like -k type
				printf("Invalid use of arguments.\n");
				printf("usage : clock -h                quick help on cmd\n");
				printf("usage : clock -c <color>        print clock with a color\n");
				return 0;
			}
			
			if  (argv[i][1] == 'c'){                                             // when first flag is -c or -cbla type
									
				if ( (strcmp(argv[i],"-c") == 0) ){                          //  when 1st is : -c type
					if ( i+1 == argc ){                                               // if last is only -c give invalid
						printf("Invalid use of arguments.\n");
						printf("usage : clock -h                quick help on cmd\n");
						printf("usage : clock -c <color>        print clock with a color\n");
						return 0;							
					}

					else {
					
						strcpy(check,toLower(argv[i+1]));            // Make the word to lowercase

						found = checkColorIn(check);                    // check if it is a color & set code if it is a color

						if ( found != 1){                             //  if not a color with last -c 
							printf("%s :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",check);
							return 0;
						}
					}
				}
			   	
				else if ( (argv[i][1] == 'c') && strlen(argv[i]) > 2 ){      // when 1st is  : -cbla type

					int position =2 , lenOfCom, j = 0;           //  get color part from -cbla
					lenOfCom = strlen(argv[i]) ;
					while (j < lenOfCom) {
						sub[j] = argv[i][position+j];
						 j++;
					}
					sub[j] = '\0';

					strcpy(sub,toLower(sub));                     // Make the word to lowercase

					found = checkColorIn(sub);                    // check if it is a color & set code if it is a color

					if ( found != 1){                           //  if not a color with last -c
						 printf("%s :This is not a valid color, Please enter one of these colours: black, red, green, yellow, blue, magenta, cyan, white\n",sub);
						return 0;
					}
				}	
			}
		}
	}
// ............................................... Argument chacking error handling over



//  print the current time, and date

	
	// Make cursor invisible
	printf("\x1b[?25l");  

        //Go to 0,0 position in the screen
	printf("\x1b[H");

	// Terminal clear
	printf("\x1b[2J");

	signal(SIGINT, sighandler); // calling function for handle ctrl + c for terminating prog

	char ele;

	
	while(!STOP){                       // **************  print current time & date untill terminate prog
		
		printf("\x1b[?1049h");       // open the alternative screen
		printf("\x1b[2J");           // Terminal clear

	 // Get the sysytem's time data
		time_t currentTime;              // time_t is the type returned by time() function 
		time(&currentTime);                     // pass currentTime variable to time() function
		struct tm *myTime = localtime(&currentTime);    // pointer to tm structure and get the time
		
		// take the current time to a string 
		strftime(clockArray,10,"%H:%M:%S", myTime); 

		// Position where should 1st pixel start in first row
		int position = 2;  

		// Go through current time string
                for (int i=0;  clockArray[i] != '\0'; i++){
				
			ele = clockArray[i];     // print one by one element of the clock at given scale in project
			printTime(ele, position, clr);

                	if ( ele == ':')         //  change position of column after one element printed
				position += 5;
			else
				position += 7;

		}


		printf("\x1b[%dm",clr-10);       // print current date in relevant color in relevant place     clr - 10 is for get relevant foreground color from background one
		printf("\x1b[8;23H%04d-%02d-%02d\n", myTime->tm_year + 1900,myTime->tm_mon + 1, myTime->tm_mday);
		
		printf ("\x1b[0m");    // Reset color	
		printf("\x1b[H");      // Go to 0,0 position in the screen

		sleep(1);                        // Delay clock by one second

	}
	printf("\x1b[?25h");  // cursor visible

	printf("\x1b[?1049l");         // close the alternative screen

	return 0;
}




char* toLower(char *ch){                                   // ********* Function to convert lower case
	for (int i = 0; ch[i] != '\0'; i++){
		ch[i] = tolower(ch[i]);
	}
	return ch;
}

int checkColorIn(char *s){                                 // *********  Function for check whether that if s is a valid color & call setcolor to get color code
	int founded = 0;
	for(i = 0; i < len; ++i){
		if( strcmp( colors[i], s) == 0 ){
			founded  = 1;
			clr = setColor(s);
		}
	}
	return founded;
}


int setColor( char * arg){                                 // *********** Function for set relevant  color code 
	if ( strcmp(arg, "black") == 0 )
		return 40;
    else if (  strcmp(arg, "red") == 0 )
		return 41;
	else if ( strcmp(arg, "green") == 0 )
		return 42;
	else if ( strcmp(arg, "yellow") == 0 )
		return 43;
	else if ( strcmp(arg, "blue") == 0  )
		return 44;
	else if ( strcmp(arg, "magenta") == 0 )
		return 45;
	else if ( strcmp(arg, "cyan") == 0 )
		return 46;
	 else if ( strcmp(arg, "white") == 0 )
		 return 47;
		   
	return 0;
}


void sighandler(int signalNum){                         // ************ Function for handle ctrl + c command
	printf ("\x1b[0m");    // Reset color
	STOP = 1;              // To stop the clock printing
}


// *********** Function for print one by one number of the clock in relevant position of the screen in color

void printTime(int Ele, int position, int clrCode){               

	if( Ele == '0'){
	
	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
    printf("\x1b[3;%dH\x1b[%dm  \x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);	
	printf("\x1b[4;%dH\x1b[%dm  \x1b[4;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[5;%dH\x1b[%dm  \x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}

	if( Ele == '1'){

		for (int i=1; i<6; i++){
			printf("\x1b[%d;%dH\x1b[%d;%dH\x1b[%dm  \x1b[0m\n",i+1,position,i+1,position+4,clrCode);
		}
	}

	if( Ele == '2'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}


	if( Ele == '3'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}	

	if( Ele == '4'){

	printf("\x1b[2;%dH\x1b[%dm  \x1b[2;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[3;%dH\x1b[%dm  \x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[6;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	}

	if( Ele == '5'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}

	if( Ele == '6'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[%dm  \x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}

	if( Ele == '7'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[4;%dH\x1b[4;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[5;%dH\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[6;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	}

	if( Ele == '8'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[%dm  \x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[%dm  \x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}

	if( Ele == '9'){

	printf("\x1b[2;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[3;%dH\x1b[%dm  \x1b[3;%dH\x1b[%dm  \x1b[0m\n",position,clrCode,position+4,clrCode);
	printf("\x1b[4;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	printf("\x1b[5;%dH\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position,position+4,clrCode);
	printf("\x1b[6;%dH\x1b[%dm      \x1b[0m\n",position,clrCode);
	}

	if( Ele == ':'){

        printf("\x1b[3;%dH\x1b[%dm  \x1b[0m\n",position+1,clrCode);
        printf("\x1b[5;%dH\x1b[%dm  \x1b[0m\n",position+1,clrCode);
	}

	return;

}


// End of my project ... Thank you !!! ;)






