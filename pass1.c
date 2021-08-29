#include "header.h"
#include "global.h"
void readfile1 ( FILE * fd) {     /*reads through the file*/
	int read = 0;
	int line = 0 ; /*counts what line we are up to */
	char ch[SIZE_WORD];/*the word we want to check*/
        char lable[SIZE_WORD];
	g_memsymbol=0;
        g_IC = START_MEMORY;
        g_DC = 0;
        g_err=0;
        g_mem_DC=0;
        g_mem_IC =0;
	while(!feof(fd))/*while end of file not yet reached*/
	{
		
		line ++;/*updating line number*/
		g_label = 0;/*meenwhile the word isnt a label*/
		read = fscanf(fd,"%s",ch);/*reading first word checking if its a comment*/
		if (read != 1 || ch[0] == ';'){/*if this is a comment or scanf didnt reciev anything- an empty line*/
			fgets(g_arr,SIZE_LINE,fd);
			continue;
		}
		if(ch[(strlen(ch))-1] == ':' ) { /*checking if the workd is a label*/
			g_label = 1 ;  /*a label found*/               
			if((CheckLabel(ch)) == -1){
                            printf("The lable is not correct in line:%d\n",line);    /*function checks if its a correct label*/
				fgets(g_arr,SIZE_LINE,fd);/*reads till end of line and ready to read first word of new  line*/
				continue;
			}
	                strncpy((g_symbols[g_memsymbol].LabelName),ch, (strlen(ch))-1);/*adding label name to the label name table*/
		        g_symbols[g_memsymbol].LabelName[strlen(ch)-1] = '\0';
			g_memsymbol ++;
			fscanf(fd,"%s",ch);/*reading  the next word*/
		}
		if ((checkguide(ch) == -1)&&(checkcommand(ch)) == -1) { /*is the word a command  or a guidence*/
			g_err = 1;/*the word isnt a guidence or command-ERROR*/
			printf("An error was found -this word isn't a commands or guidence!!! the error was found in line: %d\n",line);
			fgets(g_arr,SIZE_LINE,fd);
			continue;
		}
		if(checkguide(ch) == 0) { /*the word is guidence*/
			int c, i=0;
			if (!(strcmp(ch , ".data"))){/*putting all the varibels into data array*/
				if ( g_label == 1){
					g_symbols[g_memsymbol-1].address = g_DC;
					g_symbols[g_memsymbol-1].isExternal = -1;
					g_symbols[g_memsymbol-1].isCode = -1 ;
				}
				if(((c = fgetc(fd) )== ' ' )||( c == '\t'))
					c = whitespace(fd );
			
				if (c == '\n'){
					printf("An error was found -missing parameters ur error was found in line%d\n",line);
					g_err =1 ;
				fgets(g_arr,SIZE_LINE,fd) ;
				continue ;
			}
			while(c != '\n'){/*while we are still in same line putting data to dc*/
				int negative = 0 ;
				int digit = 0;/*if the letter was a number - the data is correct*/
                                 i=0;
				if (c == '+' )
					c = fgetc(fd) ;
				else{
					if ( c == '-' ){	
						negative = 1 ;
                                               	c = fgetc(fd) ;
					}
                                }
				while(isdigit(c))
				 {	
					digit = 1;
					ch[i] = c;
					c=fgetc(fd);
					i++;
				}
	                        ch[i]='\0';
				if (digit == 0){
					printf("An error was found -the parameter isn't correct!! the error was found in line%d\n",line);
					g_err = 1;
					fgets(g_arr,SIZE_LINE,fd);
                                         c='\n';
					continue;
				}
				if (digit == 1){
					if ( negative == 1)
						g_data[g_DC].variable = (atoi(ch))*-1;
					else
						g_data[g_DC].variable = atoi(ch);
					g_DC ++;
				}
				if(c == ' '|| c =='\t')
				 	c = whitespace(fd);/*skip all whitespaces*/
				if (c == '\n')
					continue;
				if (c == ',' ){/*makes sure we dont have an extra comma*/
                                       c=fgetc(fd);
                                 if(c ==  ' ' || c == '\t')
					c = whitespace(fd);
                                       	if (c =='\n'){
						printf("An error was found - no comma should be put at end of line error was found in line%d\n",line);
						g_err =1 ;
						continue ;}
				}
			}
			continue;
		}
			
	if(!(strcmp(ch , ".string" ))){/*putting all the strings to the data*/
		int i = 1 ;
		if (g_label==1){
			g_symbols[g_memsymbol-1].address = g_DC;
			g_symbols[g_memsymbol-1].isExternal = -1;
			g_symbols[g_memsymbol-1].isCode = -1 ;
		}
		
		if ((c = fgetc(fd)) ==' '|| c == '\t' )
			
			c = whitespace(fd );
		if (c == '\n'){
			printf("An error was found -missing parameters your error was found in line%d\n",line);
			g_err =1 ;
			fgets(g_arr,SIZE_LINE,fd) ;
			continue ;
		}
                fseek(fd,-1,SEEK_CUR);
		fscanf(fd,"%s",ch);
		if ((ch[0] != '"') ||(ch[ (strlen(ch)-1)] != '"'))
		{ 
			printf("An error was found -The string  must begin  and finish with a Quotation mark! The error was found in line %d\n",line);
			fgets(g_arr,SIZE_LINE,fd);
			g_err = 1;/*a mistake was found*/
			continue;
		}
		while(i < (strlen(ch)-1))
		{
			g_data[g_DC].variable = ch[i];
			g_DC ++;
			i++;
		}
		g_data[g_DC].variable = 0;/*putting a zero at the end of each string*/
		g_DC++ ;
		if (((c = fgetc(fd) )== ' ') ||( c == '\t' ))
			c = fgetc(fd);
		if(c != '\n'){
			printf("An error was found -the first word must begin with a letter! The error was found in line %d\n",line);
			fgets(g_arr,SIZE_LINE,fd);
			g_err = 1;/*a mistake was found*/
			continue;
		}
		continue;
	}
	if(!(strcmp(ch, ".extern" ))){
		if (((c = fgetc(fd)) == ' ' )||(c == '\t'))
			c = whitespace(fd );
		if (c == '\n'){
			printf("An error was found -missing parameters ur error was found in line%d\n",line);
			g_err = 1 ;
			fgets(g_arr , SIZE_LINE , fd) ;
			continue ;
		}
                fseek(fd,-1,SEEK_CUR);
		fscanf(fd,"%s",ch);
		if (Checkextern(ch) == -1){
			printf("An error was found - the label isnt correct  your error was found in line%d\n",line);
			g_err =1 ;
			fgets(g_arr,SIZE_LINE,fd) ;
			continue ;
		}
                if ( g_label == 1)
                g_memsymbol--;
		strcpy(g_symbols[g_memsymbol].LabelName , ch) ;
		g_symbols[g_memsymbol].address = 0;
		g_symbols[g_memsymbol].isExternal = 1;
		g_symbols[g_memsymbol].isCode = -1;
                g_memsymbol++;
		continue;
	}
	if(!(strcmp(ch, ".entry" ))){
        	if ( g_label == 1)
                	g_memsymbol--;
		fgets(g_arr , SIZE_LINE , fd) ;
		continue;
		}
	}
	else{/*after all option for guidence were checked now we will check if its a command*/

		int i = 0 ;
		int c ; /*i stores  what index in command list,c stores the letters*/
		if ( g_label == 1){/*if there is a label*/
			g_symbols[g_memsymbol-1].address = g_IC ;
			g_symbols[g_memsymbol-1].isExternal = -1 ;
			g_symbols[g_memsymbol-1].isCode = 1 ;
		}
		while (strcmp(ch , commands[i].name))/*goes through the command list till finds the matching one*/	
			i++ ;   
		if((c = fgetc(fd) )==' ' ||c == '\t'){
			c = whitespace(fd);
		}
		if ( c == '\n' && commands[i].operands== 0){/*if  this commands doesnt need an iperand*/
			g_IC ++;/*another commands line added*/
			continue;
		}
		if ( c == '\n' && commands[i].operands != 0){
			printf("An error was found -U need to put an opperand!! the error was found in line%d\n",line);
			g_err = 1 ;
			continue;	
		}
		if (commands[i].operands == 0) { /*if no operands excpected but never reached enter */
			printf("An error was found unexcpected arguments!! the error was found in line%d\n",line);
			g_err =1;
			fgets(g_arr,SIZE_LINE,fd);
			continue;
		}
		else { /*if enter wasnt found and excpecting an operand and recieved one */
			int correct = 0 , j = 0 , oger = 0 , indx = 0;
			g_IC ++;/*another commands line added*/
			c = 0 ;
			fseek(fd,-1,SEEK_CUR); /*mooving a space back so scanf can accept the whole workd and function whitespace already accepted one letter*/
			while(c != '\n' ){/*goes throgh each command line till reaches enter*/
				indx=0;
				correct = 0 ; 
				if( (commands[i].code!= 9) && (commands[i].code!=10) && (commands[i].code!= 13)){/*not shitat miyun 2*/
					while((c = fgetc(fd)) != ','  &&  (c != ' ' ) && (c != '\t' )&& c != '\n'){
						ch[indx] = c ;
						indx ++;
					}
					ch[indx] = '\0';
				}
				indx=0;
				while(indx < 4 && correct == 0){/*goes through the command matrix in the array of commands and checks if the operand matches any of the options in the  matrix.*/
					if(commands[i].operands==1)
						j = 1 ;
					if ( commands[i].permit[j][indx] == 1){/*found an option for parameter*/	
						if (indx == 0){/*the type of operater is instant*/								
							if(instant(ch) == -1){/*doesnt match instant */
	 				 			indx++;
					 			continue;
							}
							else{	
								correct = 1;
								g_IC++;
								continue;
							}
						}
						if ( indx == 1 ){/*variable shitat miyun 1*/
							if(direct(ch) == -1){
								indx ++;				
								continue;
							}	
							else{
								g_IC++;
								correct = 1;
								continue;
							}
					   	}
						if(indx == 2 ){/*shitat miyun 2*/
							if((c = jump(fd)) == -1){
								indx = 4 ;
								continue;
							 }
							if(c == 1){/*when 2 ogrim 2 memory bites are needed*/
								g_IC += 2;
								correct =1;
								continue;
							}
							if ( c == 2 ){/*if not 2 ogrim 3 memory bites are needed*/
								g_IC += 3;
								correct = 1;
								continue;
							}
                                                         if ( c == 0 ){/*only one opernd from shita 0 or 1 or 3. needs only 1 memory bite*/
								g_IC ++;
								correct = 1;
								continue;
							}


						}
						if(indx == 3){/*shitat miyun 3 ogrim*/
							if (registerDirect(ch) == -1){
								indx++;
								continue;
							}
							else{
								if (oger == 0){
									g_IC++;/* 2 register operands only take one bite of memory*/
									oger++;
								}
								correct = 1;
								continue;
							}
						}
					}
					indx++;
				}/*end of while i<4 (goin through wich shitot miyun are permited for the operand )*/
				if( correct == 0){
					printf("An error was found in arguments!! the error was found in line%d\n",line);
					g_err =1;
                                         if(c!='\n'){
						while(c!='\n')
                                                     c=fgetc(fd);}
						continue;
					}
					 if(commands[i].permit[1][2]==1){/*shitat miyun 2*/
						c = '\n';
						continue;
					}
					if(j == 1) {/* if the line is correct all the way*/
						if((c== ' ') ||(c =='\t'))	
							c = whitespace(fd);					
						if (c == '\n')
							continue;
						printf("An error was found unexcpected arguments!! the error was found in line%d\n",line);
						g_err = 1;
						fgets(g_arr,SIZE_LINE,fd);
						c ='\n';
						continue;
					}
					if (j == 0 ){/*needs to recieve 2 opperands we need to check if there is a "," between both operands*/
						if( c == ' '|| c == '\t')
							c = whitespace(fd);
						if(c != ',') {
							printf("An error was found pls put a comma between the operends!! the error was found in line%d\n",line);
							g_err = 1;
                                                       if(c!='\n')
							fgets(g_arr,SIZE_LINE,fd);
							c = '\n' ;
							continue;
						}
						if (c == ',' ){
                                                   if((c=fgetc(fd))==' '||c=='\t')
                                                    c = whitespace(fd);
                                                    fseek(fd,-1,SEEK_CUR);
							j++;
							continue;
						}	
					}		
				}
			}
		}
	}
	if (g_err == 0){/*the link between both passes*/
		g_mem_IC = g_IC - START_MEMORY;
		g_mem_DC = g_DC;
		address(g_symbols);
		g_IC = 100;


	}
}


