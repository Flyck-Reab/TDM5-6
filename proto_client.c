/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "proto.h"


liste_bloc lecturefichier();

void
proto_gestion_fichier_1(char *host, char *commande, char *fichier1, bool_t ecraser, char *fichier2)
{
	CLIENT *clnt;
	ls_res  *result_1;
	type_nom  ls_1_arg=fichier1;
	read_res  *result_2;
	type_nom  read_1_arg=fichier1;
	int  *result_3;
	write_parm  write_1_arg;

	#ifndef	DEBUG
		clnt = clnt_create (host, PROTO_GESTION_FICHIER, PROTO_GESTION_FICHIER_V1, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}
	#endif	/* DEBUG */
	clnt->cl_auth = authunix_create_default();

	if(strlen(commande)==2 && strncmp(commande,"ls",2)==0)
	{
		result_1 = ls_1(&ls_1_arg, clnt);
		if (result_1 == (ls_res *) NULL) 
		{
			clnt_perror (clnt, "call failed");
		}
		int erreur = result_1->erreur;
		if (erreur == 0 )
		{
			cell_nom* cell1 = result_1->ls_res_u.liste;
			while(cell1!=NULL)
			{
				printf("%s\n", cell1->nom);
				cell1 = cell1->suivant;
			}
		}
		else
		{
			printf("Erreur\n");
		}
		xdr_free((xdrproc_t)xdr_ls_res,(char*)result_1);
	}
	else 
	{
		if(strlen(commande)==4 && strncmp(commande,"read",4)==0)
		{
			result_2 = read_1(&read_1_arg, clnt);
			if (result_2 == (read_res *) NULL) 
			{
				clnt_perror (clnt, "call failed");
			}
			int erreur = result_2->erreur;
			if (erreur == 0 )
			{
				cell_bloc* cell1 = result_2->read_res_u.fichier;
				while(cell1!=NULL)
				{
					printf("%s", cell1->bloc);
					cell1 = cell1->suivant;
				}
			}
			else
			{
				printf("Erreur\n");
			}
			xdr_free((xdrproc_t)xdr_read_res,(char*)result_2);
		}
		else
		{
			if(strlen(commande)==5 && strncmp(commande,"write",5)==0)
			{
				write_1_arg.nom=fichier1;
				write_1_arg.ecraser=ecraser;
				write_1_arg.donnees=lecturefichier(fichier2);

				result_3 = write_1(&write_1_arg, clnt);
				if (result_3 ==  NULL) 
				{
					clnt_perror (clnt, "call failed");
				}

				int erreur = *result_3;
				if(erreur==0)
				{
					printf("Pas d'erreur\n");
				}
				else
				{
					printf("Erreur\n");
				}
				xdr_free((xdrproc_t)xdr_liste_bloc,(char*)&write_1_arg.donnees);
			}
			else
			{
				printf("\nCommande inconnue.\n");
			}
		}
	}
	#ifndef	DEBUG
		clnt_destroy (clnt);
	#endif	 /* DEBUG */	
}


int
main (int argc, char *argv[])
{
	char *host;
	char *commande;
	char *fichier1;
	char *fichier2;
	bool_t ecraser;

		if(argc == 6)
		{
			host = argv[1];
			commande=argv[2];
			fichier2=argv[3];	
			ecraser= atoi(argv[4]);
			fichier1=argv[5];
			proto_gestion_fichier_1 (host, commande, fichier1, ecraser, fichier2);
		}	
		else
		{	
			printf (" Arguments  : IP commande fichier2 ecraser fichier1\necraser : \n0  pour NON \n1 pour OUI\n");
			exit(1);
		}
	exit (0);
}

liste_bloc lecturefichier(char *fichier)
{
	FILE *file;
	liste_bloc resultatTmp;	
	cell_bloc *cell1;
	cell_bloc *cell2;	
	char buffer[MAXBLOC];

	file = fopen(fichier,"r");
	if(file==NULL)
	{
		exit(1);
	}
	else
	{
		cell1=calloc(MAXBLOC,sizeof(char));
		cell1->bloc=calloc(MAXBLOC,sizeof(char));
		cell1->suivant=NULL;
		cell2=cell1;
		resultatTmp=cell1;
		while (fgets(buffer, MAXBLOC, file)!=NULL)
		{
				cell1->suivant=cell2;
				cell1=cell2;
				cell1->bloc= calloc(MAXBLOC,sizeof(char));
				cell1->suivant=NULL;
				strcpy(cell1->bloc, buffer);
				cell2=calloc(MAXBLOC,sizeof(char));
		}
		fclose(file);
	}

	return resultatTmp;
}