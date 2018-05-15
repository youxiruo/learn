#define <stdio.h>

typedef struct 
{
	int code;
	char *body;
	int len;
}WORD_t;

enum
{
	CONFIG_NULL,
	CONFIG_COMM,
	CONFIG_SYS,
	CONFIG_CPUBIND,
	CONFIG_ALARM,
	CONFIG_TRACE,
	CONFIG_BACKUP,
	CONFIG_MSIP,
	CONFIG_NETDEV,
};


#ifdef _SYS_MML
enum
{
	MMLKEY_SECTION=50,
	MMLKEY_BNUMBER,
    MMLKEY_ONUMBER,
	MMLKEY_XNUMBER,
	MMLKEY_HNUMBER,
	MMLKEY_EQUAL,
	MMLKEY_AND,
	MMLKEY_TO,
	MMLKEY_UNION,
	MMLKEY_CMD,
};
#else
enum
{
    MMLKEY_NULL,
	MMLKEY_SECTION,
    MMLKEY_ID,    
    MMLKEY_STRING,	
	MMLKEY_BNUMBER,
    MMLKEY_ONUMBER,
	MMLKEY_XNUMBER,
	MMLKEY_HNUMBER,
	MMLKEY_EQUAL,
	MMLKEY_AND,
	MMLKEY_TO,
	MMLKEY_CONNECTION,
	MMLKEY_UNION,
};
#endif


int  LowCase(char * c)
{
	if ( ( *c >= 'A' ) && ( *c <= 'Z') )
	{
	  *c = *c - 'A' +'a' ;
	 }
	return 0;
}

int DelSpaces(char *str)
{
	int i,j;
	int ret=0;
	for(i=0;str[i]!=0;i++)
	{
		if(str[i]==' '||str[i]=='\t'||str[i]=='\n')
		{
			ret++;
			for(j=i;str[j]!=0;j++)
				str[j]=str[j+1];
			i--;
		}
	}
	return ret;
}

int GetAWord(char *string,WORD_t *word)
{
	char c=0,d=0,*s;
	int  i=0,j=0,p=0,k;

	s=string;
	//c=*s;
	for(i=0;(c=s[i])!=0;i++)    
		if (IsSpace(c)==0) break;// ????
	if (c==0)
		return MMLKEY_NULL;
	p=i;
	s=&string[p];
	c=*s;

	if (c=='\'' || c=='\"')			// ????????????''""
    {
		for (j=0;(d=s[j+1])!=0;j++)
			if (d==c)		break;
		p++;	
		k=MMLKEY_STRING;			// k ???   j ????
    }
	else if(c=='[')
	{
		for (j=0;(d=s[j+1])!=0;j++)
			if (d==']')	break;
		p++;	
		k=MMLKEY_SECTION;			// k ???   j ????
	}
    else if (IsDecDigit(c))				// ??????
    {
		d=s[1];
		if (c=='0')
		{
			if(d=='x' || d=='X')
			{
				k=MMLKEY_XNUMBER;
				i+=2;
			}
			else if(d=='o' || d=='O')
			{
				k=MMLKEY_ONUMBER;
				i+=2;
			}
			else if(d=='b' || d=='B')
			{
				k=MMLKEY_BNUMBER;
				i+=2;
			}
			else k=MMLKEY_HNUMBER;
		}
		else k=MMLKEY_HNUMBER;		 
		for (j=i;(d=s[j])!=0;j++)
			if (IsHexDigit(d)==0) break;
					// k ???   j ????
    }
    else  if (IsAlpha(c))				// ??????
    {
		for (j=1;(d=s[j])!=0;j++)
		if (IsHexDigit(d)==0 && IsAlpha(d)==0) break;
		k=MMLKEY_ID;				// k ???   j ????
    }
    else  if (c=='=')
    {
		j=1;
		k=MMLKEY_EQUAL;
    }
    else if(c=='&')
	{
		d=s[1];
		if (d=='&')
		{
			j=2;
			k=MMLKEY_AND;
		}
		else
		{
			j=1;
			k=MMLKEY_TO;
		}
	}
	else if(c=='.')
	{
		j=1;
		k=MMLKEY_UNION;
	}
	else if(c==':')
	{
		for (j=1;(d=s[j])!=0;j++)
		{
			if (IsTermChar(d))
			{
				j++;
				break;
			}
		}
		k=MMLKEY_CMD;
	}
	else
    {
		//for (j=1;(d=s[j])!=0;j++)
		//if (IsHexDigit(d) || IsAlpha(d) || IsSpace(d) || d=='\'' || d=='\"' || d==';') break;
		//k=MMLKEY_CONNECTION;
		k=MMLKEY_NULL;
    }
	word->Code=k;				// ???
    word->Body=(unsigned char *)&string[p];				// ????
    word->Len=j;					// ????
	return k;
}


int Divsen(char *p,WORD_t *word)
{
int i,j=0;
	char c;
	char d;//add at 2005-06-29
	char *p,*q;
	

	DelSpaces(s);
	for(i=0;(c=s[i])!=0;i++)
		if (IsSpace(c)==0) break;// ???
	p=&s[i];
	if (( q=strchr(p , '/' )) != NULL )
	{
		if (q[1]=='/')  *q='\0';
	}
	c=*p;
	
	for(i=0;(c=p[i])!=0;i++)
	{
		if (c=='\'' || c=='\"')			// add at 2005-06-29
		{
			for (;(d=p[i+1])!=0;i++)
				if (d==c)		
				{
					i++;
					break;
				}
		}
		LowCase(&p[i]);
	}
	for(i=0;i<4096;i++)
	{
		if (GetAWord(p,&word[i])==MMLKEY_NULL) break;
		if((word[i].Code==MMLKEY_STRING)||(word[i].Code==MMLKEY_SECTION)) j=1;
		  else j=0;
		p=(char *)&word[i].Body[word[i].Len+j];
	}
	return i;	
}

int config_com(WORD_t *word,int wordcount,int section)
{

}

//输出文件中的值
void main()
{
	FILE *fp;
	char   sItem[8192];
	WORD_t word[4096];
	int    wordcount;
	int    section=CONFIG_NULL,count=0;
	
	if((fp=fopen('com_config.ini',"r"))==NULL)
	{
		printf("file open fail \r\n");
		return;
	}

	while((fgets(sItem,8192,fp) )!=NULL)
	{
		memset(word,0,10*sizeof(WORD_t));//为什么10个？
		wordcount=Divsen(sItem,word);
		if(wordcount<=0)continue;
		section=config_com(word,wordcount,section);
		memset(sItem,0,sizeof(sItem));
		printf("%s\r\n",word);
		count++;
	}

}
