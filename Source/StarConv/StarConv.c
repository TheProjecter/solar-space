#include <stdio.h>
#include <stdlib.h>
#include <string.h>




#define GET_VALUE(src,buff,hold,index,len)	\
	{										\
		strncpy(buff,src+index,len);		\
		buff[len]=0;						\
		hold=atof(buff);					\
	}

////////////////
#define VA_NUM 6
#define VA_FMT "%lf %lf | %f | %f %f %f"
#define VA_ARGS(var)	\
	var->Dec,			\
	var->RA,			\
	var->Mag,			\
	var->B_V,			\
	var->U_B,			\
	var->R_I
/////////////////////

typedef struct
	{
		double Dec, RA;
		float Mag;
		float U_B, B_V, R_I;
		struct stardata_s *prev;
		struct stardata_s *next;
	} stardata_s;

stardata_s starchain;








void writedesc(FILE *fout)
{
	fputs("//Stars file, generated by StarConv\n",fout);
	fputs("// using the Yale Bright Star Catalog\n",fout);
	fputs("//\n",fout);
	fputs("//NOTE: THE STARS MUST BE SORTED BY MAGNITUDE!!!\n",fout);
	fputs("//\n",fout);
	fputs("//Format:\n",fout);
	fputs("//number of stars\n",fout);
	fputs("//star line 1\n",fout);
	fputs("//star line 2\n",fout);
	fputs("//...........\n",fout);
	fputs("//\n",fout);
	fputs("//Star line format: (separator=='|')\n",fout);
	fputs("//Dec, degrees [0-180]\n",fout);
	fputs("//RA, degrees [0-360)\n",fout);
	fputs("//SEPARATOR\n",fout);
	fputs("//star magnitute, mag\n",fout);
	fputs("//SEPARATOR\n",fout);
	fputs("//B-V color index, relative units\n",fout);
	fputs("//U-B color index, relative units\n",fout);
	fputs("//R-I color index, relative units\n",fout);
	fputs("\n",fout);
}





void sortstar(stardata_s *star)
{
	stardata_s *cur=starchain.next;
	while (cur!=&starchain && star->Mag<cur->Mag)
	{
		cur=cur->next;
	}
	star->prev=cur->prev;
	star->next=cur;
	cur->prev=star;
	((stardata_s*)(star->prev))->next=star;
}






int lineconv(char *in_line)
{
	char buffer[32];
	double hold;
	stardata_s data;
	stardata_s *star;
	if (!in_line)
		return -1;
	{ //Mag
		GET_VALUE(in_line,buffer,hold,102,5);
		if (in_line[102+2]==' ') return 0;
		data.Mag=(float)hold;
	}
	{ //Dec
		GET_VALUE(in_line,buffer,hold,83,3);
		data.Dec=hold;
		GET_VALUE(in_line,buffer,hold,86,2);
		data.Dec+=hold/60.0;
		GET_VALUE(in_line,buffer,hold,88,2);
		data.Dec+=hold/(60.0*60.0);
		data.Dec=-data.Dec+90.0;
	}
	{ //RA
		GET_VALUE(in_line,buffer,hold,75,2);
		data.RA=hold;
		GET_VALUE(in_line,buffer,hold,77,2);
		data.RA+=hold/60.0;
		GET_VALUE(in_line,buffer,hold,79,4);
		data.RA+=hold/(60.0*60.0);
		data.RA=data.RA*(360.0/24.0);
	}
	{ //color
		GET_VALUE(in_line,buffer,hold,109,5);
		data.B_V=(float)hold;
		GET_VALUE(in_line,buffer,hold,115,5);
		data.U_B=(float)hold;
		GET_VALUE(in_line,buffer,hold,121,5);
		data.R_I=(float)hold;
	}
	star=(stardata_s*)malloc(sizeof(stardata_s));
	if (!star) return -1;
	memcpy(star,&data,sizeof(data));
	sortstar(star);
	return 1;
}









int fileconv(char *infile, char *outfile)
{
	FILE *fin;
	FILE *fout;
	char line_in[256];
	int numread=0;
	int i, res;
	stardata_s *star;
	fin=fopen(infile,"rt");
	if (!fin)
	{
		perror("unable to open file for reading\n");
		return -1;
	}
	fout=fopen(outfile,"w+t");
	if (!fout)
	{
		fclose(fin);
		perror("unable to open file for writing\n");
		return -2;
	}
	starchain.prev=starchain.next=&starchain;
	puts("Reading and sorting...");
	writedesc(fout);
	while (fgets(line_in,sizeof(line_in),fin))
	{
		res=lineconv(line_in);
		if (!res) continue;
		if (res<0) return -3;
		numread++;
		putchar('.');
	}
	fclose(fin);
	puts("done.");
	puts("Writing...");
	fprintf(fout,"%d\n",numread);
	for (i=0, star=starchain.next; i<numread; i++, star=star->next)
	{
		fprintf(fout,VA_FMT "\n",VA_ARGS(star));
		putchar('.');
	}
	star=starchain.next;
	while (star!=&starchain)
	{
		star=star->next;
		free(star->prev);
	}
	puts("done.");
	fclose(fout);
	return numread;
}







int main(int argc, char **argv)
{
	int n;
	if (argc!=3)
	{
		puts("Yale Bright Star Catalog Convertor.");
		puts("");
		puts("Usage: StarConv.exe <catalog_in> <catalog_out>");
		return 1;
	}
	n=fileconv(argv[1],argv[2]);
	if (n>=0)
	{
		printf("\n%d stars written and sorted.\n",n);
	}
	else
	{
		perror("error occured. terminating.\n");
		return (1-n);
	}
	return 0;
}
