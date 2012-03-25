#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 *	move all the stars "*" in the string a to the head of a.
 *	input: a = string
 *	@return: number of star "*"
 * */
int preRemoveStar(char * a){
	int len = strlen(a);
	char * end = a + len - 1;	//points to current char
	char * star = end;	//points to '*'
	int cnt = 0;
	while(end>=a){
		while(end>=a && *end == '*')end--;
		if(end>=a && star >= a&&end!=star){
			*star = *end;	//each time replace star with current char
		}
		--star;	//star and end move backwards.
		--end;
	}

	cnt = star - a + 1;	//star always points to the first char that can be replacable.
	while(star>=a){
		*star='*';
		--star;
	}
	return cnt;
}


/*
 *	move all the stars "*" in the string a to the end of a
 * */
int backRemoveStar(char * a){
	int cnt  = 0;
	int len = strlen(a);
	char * end = a + len - 1;
	char * star = a;
	char * begin = a;
	while(begin <= end){
		while(begin<=end && *begin == '*')begin++;
		if(begin<=end && star <= end&&begin !=star){
			*star = *begin;
		}
		++begin;
		++star;
	}

	cnt = end-star + 1;
	while(star<=end){
		*star = '*';
		star++;
	}
	return cnt;
}

/*
 *	remove all the chars in b from a
 *	input:a,b
 */
void removeChars(char * a , char * b){
	int lena = strlen(a);
	int lenb = strlen(b);
	for(int i  = 0;i<lena;i++){
		for(int j = 0;j<lenb;j++){
			if(a[i] == b[j]){
				a[i] = '*';
				break;
			}
		}
	}

	int cnt = backRemoveStar(a);
	printf("%s\n",a);
	a[lena  - cnt] = 0;
}

int main()
{
	char a[]="they are students,we are good students";
	char b[]="are";
	printf("%s\n",a);
//	preRemoveStar(a);
//	backRemoveStar(a);
	removeChars(a,b);
	printf("%s\n",a);
}
