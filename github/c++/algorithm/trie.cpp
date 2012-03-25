#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_COUNT 256
#define MAX_WORD_LENGTH 128

struct tree_node
{
	int count;
	tree_node * next[MAX_COUNT];	
};

tree_node root = {0,{NULL}};

void insert(const char * word)
{
	tree_node * newnode;
	tree_node * current = &root;
	if(word[0] == 0)
	{
		return;
	}
	for(int i = 0;;i++)
	{
		if(word[i] == 0)break;
		if(current->next[word[i]] == NULL)
		{
			newnode = (tree_node *)malloc(sizeof(tree_node));	
			memset(newnode,0,sizeof(tree_node));
			current->next[word[i]] = newnode;
		}
		current = current->next[word[i]];
	}
	++current->count;
}

void do_traverse(tree_node * rootp)
{
	static char word[MAX_WORD_LENGTH+1];
	static int pos = 0;
	tree_node * current = rootp;

	if(current->count)
	{
		word[pos]=0;
		printf("%s\t%d\n",word,current->count);
	}

	for(int i = 0; i< MAX_COUNT;++i)
	{
		if(current->next[i] == NULL)continue;
		word[pos++] = i;
		do_traverse(current->next[i]);
		--pos;
	}
}

int main()
{
	const char * delimeter = "\t\n\0,./() ";
	char * linebuf = NULL,*line,*word;
	linebuf = (char*)malloc(MAX_WORD_LENGTH+1);
	memset(linebuf,0,sizeof(char)*MAX_WORD_LENGTH+1);
	size_t bufsize = MAX_WORD_LENGTH;
	int ret = 0;
	while(1)
	{
		ret = getline(&linebuf,&bufsize,stdin);
		if(ret == -1)break;
		if(linebuf[0]=='\n')break;
		line = linebuf;
		while(1)
		{		
			word = strsep(&line,delimeter);
			if(NULL == word)
			{
				break;
			}
			if(word[0] == 0)
			{
				continue;    	
			}
			insert(word);
			
		}
	}
	do_traverse(&root);
	return 0;
}
