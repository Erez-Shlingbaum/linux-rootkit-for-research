#include "list.h"

struct file_list {
	char *file_name;
	struct file_list *next_file;
};

typedef struct file_list list;

list *hidden_files = NULL;

void addfile(const char *f)
{
	list *tmp;
	char *s;
	if (hidden_files == NULL) {
		tmp = (list*)vmalloc(sizeof(list));
		s = vmalloc(sizeof(*f));
		strcpy(s, f);
		tmp->file_name = s;
		tmp->next_file = hidden_files;
		hidden_files = tmp;
	} else {
		tmp = hidden_files;
		while (tmp != NULL && (strlen(tmp->file_name) != strlen(f) || strncmp(tmp->file_name, f, strlen(tmp->file_name)) != 0)) {
			tmp = tmp->next_file;
		}
		if (tmp == NULL) {
			list *tmp2;
			tmp2 = (list*)vmalloc(sizeof(list));
			s = vmalloc(sizeof(*f));
			strcpy(s, f);
			tmp2->file_name = s;
			tmp2->next_file = hidden_files;
			hidden_files = tmp2;
		}
	}
}

void remfile(const char *f)
{
	list *tmp, *tmp2;
	int c = 0;
	tmp = hidden_files;
	while (tmp != NULL) {
		if (strlen(tmp->file_name) == strlen(f)){
			if (strncmp(tmp->file_name, f, strlen(tmp->file_name)) == 0) {
				if (c == 0) {
					hidden_files = tmp->next_file;
					vfree(tmp->file_name);
					vfree(tmp);
					return;
				}
				tmp2->next_file = tmp->next_file;
				vfree(tmp->file_name);
				vfree(tmp);
			}
		}
		tmp2 = tmp;
		tmp = tmp->next_file;
		c += 1;
	}
}

void emptylist()
{
	list *tmp;
	tmp = hidden_files;
	while (tmp != NULL) {
		hidden_files = tmp->next_file;
		vfree(tmp->file_name);
		vfree(tmp);
		tmp = hidden_files;
	}
}

int lookupfilename(const char *f)
{
	list *tmp;
	tmp = hidden_files;
	while (tmp != NULL) {
		if (strlen(tmp->file_name) == strlen(f)){
			if (strncmp(f, tmp->file_name, strlen(tmp->file_name)) == 0){
				return 1;
			}
		}
		tmp = tmp->next_file;
	}
	return 0;
}

int extractfilename(const char *f)
{
	int i, n, c;
	size_t l;
	l = strlen(f);

	for(i = l-1, n = 0; i>=0; i--, n++){
		if(f[i] == '/'){
			i = -1;
			break;
		}
	}

	if(i == -1)
		c = n+1;
	else
		c = l;

	char s[c];
	memset(s, 0, c);

	for(i = 0; n>0; i++, n--)
		s[i] = f[l-n];

	return lookupfilename(s);
}
