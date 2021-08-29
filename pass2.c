#include "header.h"
#include "global.h"
/*pass2 goes through the memory collects the addresses needed for the program */
void readfile2( FILE * fd) {/*reads through the file for second time*/
	regist R ; /*the unien for oger*/
	addressing A ;/*the union for numbers and variables shitat miyun 0 ,1*/
	cpuOpcode C ;/*the uninon for the command names*/
	int Indx_IC = 0 ;/*what index in ic are we up to */
	int numParam = 0 ;
	int oger = 0 ;
        int memoger=0;
	int line = 0 ;
	int i = 0;
	char param1[SIZE_WORD];
	char param2[SIZE_WORD];
	char ch[SIZE_WORD];
	int c ,j =0 ;
        g_ext_labelNum=0;
        g_ent_labelNum=0;
	while(!feof(fd))/*while end of file not yet reached*/
	{
		line++;/*updating line number*/	
		fscanf(fd,"%s",ch);/*reading first word*/
		if (ch[0] == ';'){/*checking if its a comment*/
			fgets(g_arr,SIZE_LINE,fd);
			continue;
		}
		if(ch[(strlen(ch))-1] == ':')/*checking if the word is a label*/
			fscanf(fd,"%s",ch);/*the label was already read in the first parsing*/
		if (checkguide(ch)==0 && strcmp(ch,".entry")){/*is ch a guidence?*/
			fgets(g_arr,SIZE_LINE,fd);	
	  		continue;
                 }
		if (!(strcmp(ch,".entry"))){/*if ch is .entry*/
			fscanf(fd,"%s",ch);
			if (LabelInList( ch)){
				printf("An error was found -This lable wanst found! The error was found in line %d",line);
				fgets(g_arr,SIZE_LINE,fd);
				g_err = 1;/*a mistake was found*/
				continue;
			}
			strcpy(g_ent_label[g_ent_labelNum].label , ch);
			g_ent_label[g_ent_labelNum].address = saveNum(ch);
			g_ent_labelNum ++ ;
			fgets(g_arr,SIZE_LINE,fd);
			continue;
		}
                  j=0;
		while(strcmp(ch,commands[j].name))
			j++;
		if((commands[j].operands )== 0){/*if the commands doesnt recieve an argument*/
			C.u.era = 0;
			C.u.opDest = 0;
			C.u.opSource = 0;
			C.u.opcode=commands[j].code;/*command number*/
			C.u.param1=0;
			C.u.param2=0;
			g_code[Indx_IC]= C.fullReg;
			Indx_IC++;
			g_IC++;
			fgets(g_arr,SIZE_LINE,fd);
			continue;
		}
		if(commands[j].operands==1){/*if the command receives one opperand*/
			if ((commands[j].code != 9 )&& (commands[j].code!=10 ) && (commands[j].code != 13)){/*if we are not in shitat 
miyun 2(not jsr or jmp or bne)*/ 
				fscanf(fd,"%s",ch);
				if(!(direct(ch))&&(LabelInList(ch))){
					printf("An error was found -This lable wanst found! The error was found in line %d",line);
					fgets(g_arr,SIZE_LINE,fd);
					g_err = 1;/*a mistake was found*/
					continue;
			}
				C.u.era = 0 ;
				C.u.opDest = typeParam(ch);
				C.u.opSource = 0;
				C.u.opcode = commands[j].code;
				C.u.param1 = 0;
				C.u.param2 = 0;	
				g_code[Indx_IC] = C.fullReg;
				Indx_IC++;
				g_IC++;
				if(!direct(ch))	{/*if shitat miyun 1*/
					if ( saveNum(ch)==0){/*if an extern variable */
						strcpy(g_ext_label[g_ext_labelNum].label,ch);
						g_ext_label[g_ext_labelNum].address=g_IC;
						g_ext_labelNum++;
						A.a.kind = 1 ;		
						A.a.num = 0 ;
						g_code[Indx_IC] = A.fullReg;
						Indx_IC++;
						g_IC++;
						fgets(g_arr,SIZE_LINE,fd);
						continue;
					}
					A.a.kind = 2;
					A.a.num = saveNum(ch);
					g_code[Indx_IC] = A.fullReg;
					Indx_IC++;
					g_IC++;
					fgets(g_arr,SIZE_LINE,fd);					
					continue;
				}			
				if (!instant(ch)){/*if shitat miyun 0*/
					A.a.kind = 0;	
					A.a.num = saveNum(ch);
					g_code[Indx_IC] = A.fullReg;
					Indx_IC++;
					g_IC++;
                                	fgets(g_arr,SIZE_LINE,fd);
					continue;
				}
				if ( ! registerDirect(ch)){/*if shitat miyun 3*/
					R.reg.kind = 0;	
					R.reg.destination=saveNum(ch);
					R.reg.source = 0 ;
					g_code[Indx_IC] = R.fullReg;
					Indx_IC++;
					g_IC++;
					fgets(g_arr,SIZE_LINE,fd);
					continue;
				}
			}		
			else{/*checks the commands "jump", "jsr" ,"bne"*/
			whitespace(fd);
			fseek(fd,-1,SEEK_CUR); 
			i = 0 ;
			while((c=fgetc(fd))!='(' && c!=' ' && c != '\t'&&c!='\n'){
				ch[i]=c;
				i++;
			}
                        ch[i]='\0';
			if (c == ' ' || c == '\t')
				c = whitespace(fd);
                                if(c=='\n'){/*if the command is jump "bne" or "jsr"or "jump" and not shitat miyun2*/
                                	C.u.era = 0 ;
					C.u.opDest = typeParam(ch);
					C.u.opSource = 0;
					C.u.opcode = commands[j].code;
					C.u.param1 = 0;
					C.u.param2 = 0;	
					g_code[Indx_IC] = C.fullReg;
					Indx_IC++;
					g_IC++;
                	                if(!(direct(ch))&&( LabelInList(ch)))	{/*checks if operand is in the list already*/
                	                       printf("An error was found -This lable wanst found! The error was found in line %d",line);
						g_err = 1;/*a mistake was found*/
						continue;
                                        }
                                        if(!direct(ch)){/*if shitat miyn1*/
					if ( saveNum(ch)==0){/*if an extern variable */
						strcpy(g_ext_label[g_ext_labelNum].label,ch);  
						g_ext_label[g_ext_labelNum].address=g_IC;
						g_ext_labelNum++;
						A.a.kind = 1 ;		
						A.a.num = 0 ;
						g_code[Indx_IC] = A.fullReg;
					Indx_IC++;
					g_IC++;
					
				continue;
					}
					A.a.kind = 2;/*if shitat miyun 1 and not external */
					A.a.num = saveNum(ch);
					g_code[Indx_IC] = A.fullReg;
					Indx_IC++;
					g_IC++;
					
			
				continue;
				}

                         if ( ! registerDirect(ch)){/*if shitat miyun 3*/
					R.reg.kind = 0;	
					R.reg.destination=saveNum(ch);
					R.reg.source = 0 ;
					g_code[Indx_IC] = R.fullReg;
					Indx_IC++;
					g_IC++;
					
					continue;
					}
				}/*shitat miyun 2*/
                                if ((c = fgetc(fd)) == ' ' || c == '\t') 
                                       c = whitespace(fd);
                                 i=0; 
				while(c != ',' && c != ' ' &&  c != '\t'){
					param1 [i] = c;		/*done collecting first param*/	
					c=fgetc(fd);
					i++;
				}
                                param1[i]='\0';
			 	if (c == ' ' || c == '\t')/*collect all whitespace*/
					c = whitespace(fd);	
				c = fgetc(fd);/*the space before comma*/
				if (c == ' ' || c == '\t')
                                    c = whitespace(fd);/*the spaces after the comma*/
				i = 0 ;
				while((c != ')' ) && (c  != ' ' ) && (c != '\t' )){/*collecting second param*/	
					param2[i] = c;			
					c = fgetc(fd);
					i++;
				}
                                param2[i]='\0';
				if (((typeParam(param1) == 1 ) && (LabelInList(param1)))||((typeParam(param2) == 1 ) && (LabelInList(param2)))){
					printf("An error was found -This lable wanst found! The error was found in line %d",line);
					fgets(g_arr,SIZE_LINE,fd);
					g_err = 1;/*a mistake was found*/
					continue;
				}
				C.u.era = 0;
				C.u.opDest = 2;
				C.u.opSource = 0;
				C.u.opcode = commands[j].code;			
				C.u.param1 = typeParam(param2);
				C.u.param2 = typeParam(param1);
				g_code[Indx_IC]=C.fullReg;
				Indx_IC++;
				g_IC++;	
				if(( LabelInList(ch)))	{
                                       printf("An error was found -This lable wanst found! The error was found in line %d",line);
					g_err = 1;/*a mistake was found*/
					continue;
                                }
				if ( saveNum(ch)==0){
					strcpy(g_ext_label[g_ext_labelNum].label,ch);
					g_ext_label[g_ext_labelNum].address=g_IC;
					g_ext_labelNum++;
					A.a.kind = 1;
				}
				else
					A.a.kind =2;/*stores in the memory the address of the label*/
				A.a.num = saveNum(ch);
				g_code[Indx_IC] = A.fullReg;		
				Indx_IC++;
				g_IC++;
                                numParam = 0 ;
                                oger = 0 ;
                                memoger=0;
				while(numParam<2){/*checks of both parameters are in shitat miuyn 2*/
					if (numParam== 0)/*saving first parameter to memory*/
						strcpy(ch,param1);
					if (numParam==1)/*saving first parameter to memory*/
						strcpy(ch,param2);
					if (typeParam(ch)==1 && saveNum(ch)==0){/*extern shitat miyun 1*/
						strcpy(g_ext_label[g_ext_labelNum].label,ch);
						g_ext_label[g_ext_labelNum].address = g_IC;
						g_ext_labelNum++ ;
						A.a.kind = 1;
						A.a.num = 0;
						g_code[Indx_IC] =A.fullReg;
						Indx_IC++;
						g_IC++;
                                                numParam++; 
                                                continue; 
					}
					if (typeParam(ch)==1 ){/*not extern shitat miyun 1*/				         	A.a.kind = 2;
						A.a.num = saveNum(ch);
						g_code[Indx_IC] =A.fullReg;
						Indx_IC++;
						g_IC++;
                                                numParam++; 
                                                continue; 
					}
					if (typeParam(ch)==0){/*shitat miyun 0*/
						A.a.kind = 0;
						A.a.num = saveNum(ch);
						g_code[Indx_IC] = A.fullReg;
						Indx_IC++;
						g_IC++;
                                                 numParam++; 
                                                 continue;
					}
					if (typeParam(ch)==3){/*ogrim -shitat miyun 3*/
						if(oger==0){
                                                        oger++;/*how many ogrim in the command*/
							R.reg.kind = 0 ;
                                                    	if ( numParam==0){	/*storing source operand*/
								R.reg.destination =0;
								R.reg.source = saveNum(ch);
                                                	       g_code[Indx_IC] = R.fullReg;
								Indx_IC++;
								g_IC++;
                        	                                memoger= saveNum(ch);/*saving the memory of the source oger it will be needed if destination is also an oger*/
								numParam++;
								continue;
							}
      							if (numParam==1){	/*storing destination operand and its the first oger in the command*/			
							R.reg.destination = saveNum(ch);
							R.reg.source = 0;
							g_code[Indx_IC] = R.fullReg;
							Indx_IC++;
							g_IC++;
                                     
                                                        numParam++; 
		
                                                        continue; 
                                                 }	
					}				
						if (oger == 1){	/*if this is the second oger */						
							R.reg.kind = 0 ;
							R.reg.destination =  saveNum(ch);
							R.reg.source =memoger;
							g_code[Indx_IC-1] = R.fullReg;
						}
					}
					numParam++;
				}
				fgets(g_arr,SIZE_LINE,fd);
				continue;
			}
		}
		if(commands[j].operands == 2){
			whitespace(fd);
			fseek(fd,-1,SEEK_CUR); 
			i=0;
			while(((c=fgetc(fd)) != ',')&& (c!= ' ') && c!='\t'){
				param1[i]=c;
				i++;
			}
           		param1[i]='\0';
			if (c ==' '||c=='\t')
		        	whitespace(fd);
			whitespace(fd);
			fseek(fd,-1,SEEK_CUR); 
			i=0;
			while(((c=fgetc(fd)) != ' ' ) && (c!='\t')&&c != '\n'){
				param2[i]=c;
				i++;
			}
			param2[i]='\0';
			if (((typeParam(param1) == 1 ) && (LabelInList(param1)))||((typeParam(param2) == 1 ) && (LabelInList(param2)))){
			printf("An error was found -This lable wanst found! The error was found in line %d",line);
			fgets(g_arr,SIZE_LINE,fd);
			g_err = 1;/*a mistake was found*/
			continue;
			}
	C.u.era = 0;
	C.u.opDest =typeParam(param2);
	C.u.opSource = typeParam(param1);
	C.u.opcode = commands[j].code;				
	C.u.param1 = 0;
	C.u.param2 = 0;
	g_code[Indx_IC]=C.fullReg;
        g_IC++;
	Indx_IC++;
        memoger=0;
	oger = 0 ;
	numParam = 0 ;
	while(numParam<2){
		if (numParam== 0)/*saving first parameter to memory*/
			strcpy(ch,param1);
			if (numParam==1)/*saving first parameter to memory*/
				strcpy(ch,param2);
			if (((typeParam(ch))==1) && ((saveNum(ch))==0)){/*extern shitat miyun 1*/
					strcpy(g_ext_label[g_ext_labelNum].label,ch);
					g_ext_label[g_ext_labelNum].address = g_IC;
					g_ext_labelNum++;
                                        A.a.kind = 1; 
					A.a.num = 0;
					g_code[Indx_IC] =A.fullReg;
					Indx_IC++;
					g_IC++;
                                        numParam++; 
                                        continue; 
			}
			if (typeParam(ch)==1){/*not extern shitat miyun 1*/
                                        A.a.kind = 2;
					A.a.num = saveNum(ch);
					g_code[Indx_IC] =A.fullReg;
					Indx_IC++;
					g_IC++;
                                        numParam++; 
                                        continue;   
				}
				if (typeParam(ch)==0){/*shitat miyun 0*/
					A.a.kind = 0;
					A.a.num = saveNum(ch);
					g_code[Indx_IC] = A.fullReg;
					Indx_IC++;
					g_IC++;
                                        numParam++; 
                                        continue; 
				}
		if (typeParam(ch)==3){/*ogrim*/
						if(oger==0){
                                                        oger++;
							R.reg.kind = 0 ;
                                                    	if ( numParam==0){	
							R.reg.destination =0;
							R.reg.source = saveNum(ch);
                                                        g_code[Indx_IC] = R.fullReg;
							Indx_IC++;
							g_IC++;
                                                        memoger= saveNum(ch);
							numParam++;
							continue;
							}
      							if (numParam==1){
							R.reg.destination = saveNum(ch);
							R.reg.source = 0;
							g_code[Indx_IC] = R.fullReg;
							Indx_IC++;
							g_IC++;
                                                        numParam++; 
                                                        continue; 
                                                 }	
					}				
						if (oger == 1){							
							R.reg.kind = 0 ;
							R.reg.destination =  saveNum(ch);
							R.reg.source =memoger;
							g_code[Indx_IC-1] = R.fullReg;
						}			
					}
				
				numParam++;
				}
			}
			if (c==!'\n')	
	                  fgets(g_arr,SIZE_LINE,fd);
			
		}
	}
 


