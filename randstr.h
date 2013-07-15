char *rand_str(char *str,const int len)
{
	int i;
	for(i=0;i<len;++i)
		str[i]='A'+rand()%26;
	str[++i]='\0';
	return str;
}
