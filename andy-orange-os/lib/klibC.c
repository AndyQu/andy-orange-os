#include"type.h"
#include"const.h"

char* itoa(char *str,unsigned int num);
void disp_str(char*);
PUBLIC void disp_int(unsigned int input)
{
	char output[12];
	itoa(output,input);
	disp_str(output);
	disp_str(" ");
}
extern int disp_pos;
PUBLIC void disp_strPos(char*s, int pos)
{
	if(pos%2!=0||pos<0)return;
	int r=disp_pos;
	disp_pos=pos;
	disp_str(s);
	disp_pos=r;
}
/*
需要声明为unsigned int 类型，否则，如果num为负数，将不能显示其16进制表示。
*/
PUBLIC char* itoa(char *str,unsigned int num)
{
	int index=9;
	int remainder;
	int quotient;
	int i;
	for(i=0;i<11;i++){
		str[i]='0';
	}
	str[0]='0';
	str[1]='x';
	str[10]=0;
	while(num>0 && index>=2){
		quotient=num/16;
		remainder=num%16;
		if(remainder<10){
			str[index]=remainder+'0';
		}else{
			str[index]=remainder-10+'A';
		}
		index--;
		num=quotient;
	}
	return str;
}

PUBLIC	char*	strcpy(char*dest, char*src)
{
	int i=0;
	while(1){
		dest[i]=src[i];
		i++;
		if(src[i]==0)
			break;
	}
}

PUBLIC	int	strlen(char*s)
{
	int count=0;
	while(1){
		if(s[count]!=0){
			count++;
		}else{
			break;
		}
	}
	return	count;
}

