

/*
	Name: mission4_equipe_2.c
	Copyright: 
	Author: BOUHADOUF Nassim
	Date: 24/01/19 17:01
	Description: 
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>




// l'arbre
typedef struct node
{
	char c ; 
	int occ ; 
	struct node* left ; 
	struct node* right ; 
	
} node ; 

// le tas contenant des noeuds de l'arbre
typedef struct tas
{
	struct node* n ; 
    struct tas* next ;
    
} tas ; 

struct tas* getNewTas(struct node* r,struct tas* next) ;   // créer nouvau tas
struct node* getNewNode(char c, int occ, struct node* left, struct node* right) ; // créer arbre binaire 
struct tas* insertTas(struct tas* t, struct node* r) ;  // inserer un noeud dans le tas
void deleteTas(struct tas** t,struct node* r) ; // supprimer un noeud du tas
struct node* insert(struct node* r, char c, int occ) ; // inserer un noeaud dans un arbre
struct tas* getMin(struct tas* t) ; // avoir les deux min d'un tas
struct node* insertNodeInNode(struct node* r, struct node* s) ; // inserer un abre dans un arbre
struct tas* tri(struct tas* t) ; // trier la liste 
struct node* huff_c(struct tas* t) ; 

void write_bin(struct node* r,int b[],int n,FILE* f) ; // ecrire les codes 


int charocc(char c,const char* s) ; 
struct tas* remplir(struct tas* t, const char* s) ; // remplir un tas
int exist(char s, const char* c) ; 

int length(struct tas* t) ; // longueur d'un tas

const char* read_file(const char* file) ; 


void print(struct node* s) ;  // afficher un arbre
void printTas(struct tas* t) ;  // afficher un tas


// la chaine en global 
const char* global_s ; 
// lmoy
float moyenne = 0 ;

int main()
{

	//***** lecture du fichier 
	global_s = read_file("Equipe_2_Mission4_in.txt ") ;
	//  la liste d'arbres vide
	struct tas* t = NULL ; 
	// remplir la liste par des noeuds d'arbres contenant les caractères et leurs occurences
	t = remplir(t,global_s) ; 
	
	//****** arbre résultat
	struct node* r = NULL ; 
	
	r = huff_c(t) ; 
	//*************************
	
	int b[100] ; 
	int n = 0 ; 
	
	//*** ouverture du fichier de sortie
	FILE* f = fopen("Equipe_2_Mission4_out.txt","w") ; 
	
	if(f!=NULL)
	{
		
		//***** parcourir l'abre binaire et ecrire dans le fichier les codes
		write_bin(r,b,n,f) ;
		//****  lmoy 
		fprintf(f,"\n\n %f ",moyenne) ; 
	}
	else
	{
		printf("fichier introuvable") ; 
	}
	
	fclose(f) ; 


}

//************* lecture d'un fichier

 const char* read_file(const char* file)
{
	FILE* f1 = fopen(file,"r+") ; 
	
	char* c = (char*)malloc(200*sizeof(char)) ; 
	char* c2 = (char*)malloc(200*sizeof(char)) ; 

		
	if(f1!=NULL)
	{
		fgets(c,200,f1) ; 
		
		while(fgets(c2,200,f1)) 
		{
			c = strcat(c,c2) ;
			c = strcat(c,"\n") ;
		}
	}
	else 
	{
		printf(" fichier introuvable ") ; 
	}
	
	
	fclose(f1) ;
	
	
	
	return c ; 
}


//************ créer nouvelle liste
struct tas* getNewTas(struct node* r,struct tas* next)
{
	
	struct tas* p = (struct tas*) malloc(sizeof(tas)) ;
	
	p->n = r ; 
	p->next = next ; 
	
	return p ; 
	 
}

//************** inserer dans la liste
struct tas* insertTas(struct tas* t, struct node* r) 
{
	
	if(t==NULL)
	{
		t = getNewTas(r,NULL) ; 
	}
	else
	{
		t->next = insertTas(t->next,r) ; 
	}
	
	return t ; 
}

//************* créer arbre binaire 
struct node* getNewNode(char c, int occ, struct node* left, struct node* right)
{
	struct node* p = (struct node*) malloc(sizeof(node)) ; 
	
	p->c = c ;  // caractère
	p->occ = occ ;  // nombre d'occurences
	p->left = left ;  // fils gauche
	p->right = right ;  // fils droit
	
	
	return p ; 
	
}

//********** inserer dans l'arbre 
struct node* insert(struct node* r, char c, int occ)
{
	// l'ordre de l'insertion n'a pas d'importance
	
	if(r==NULL)
	{
		r = getNewNode(c,occ,NULL,NULL) ; 	
	}
	else if(r->occ>occ) // si l'occurence est plus petite que celle du noeaud parent (verification optionel) 
	{
		r->left = insert(r->left,c,occ) ; 
	}
	else  // si l'occurence est plus grande que celle du noeaud parent (verification optionel)
	{
		r->right = insert(r->right,c,occ) ; 
	}
	
	return r ; 
}



//********** longueure d'une liste
int length(struct tas* t)
{
	if(t!=NULL)
	{
		return length(t->next) + 1 ; 
	}
	else
	{
		return 0 ; 
	}
	
}

//************ supprimer un noeud d'une liste
void deleteTas(struct tas **t, struct node* r) 
{ 
   struct tas *cur, *prev;

  prev = NULL;

  
  for (cur = *t;
	cur != NULL;
	prev = cur, cur = cur->next) {

    if (cur->n->occ == r->occ && cur->n->c == r->c) { 
      if (prev == NULL) {
        *t = cur->next;
      } else {
        prev->next = cur->next;
      }


      free(cur);


      return;
    }
  }
	
} 

//************* avoir les deux min d'une liste
struct tas* getMin(struct tas* t)
{
	
 	struct tas* s = t ; 
	
	// une liste qui contient les deux min
	struct tas* min = NULL ; 
	
	// avoir le premier min 
	
	int min1 = 100 ; 
	char min1_c ; 
	
	// créer un noeud d'arbre qui contient les valeur du min trouvé
	struct node* premier_min = NULL ;
	
	// parcourir la liste et trouver le premier min 	
	while(s!=NULL)
	{
		if(s->n->occ<=min1)
		{
			min1 = s->n->occ ; 
			min1_c = s->n->c ; 
			premier_min = s->n ; 
			
		}
		
		s = s->next ; 
	}
	
	
	// inserer le noeud dans le tas qui contient les deux min
	min = insertTas(min,premier_min) ; 
	
	s = t ; 
	
	if(length(s)>1) // ( optionnel ) 
	{
	
		// deuxième min 
		
		int min2 = 100 ; 
	
		
		// créer un noeud d'un arbre qui contient le min trouvé
		struct node* deuxiem_min = NULL ;
		
		char min2_c ; 
	
		while(s!=NULL)
		{
			// si le caractère qui possède deuxième min n'est pas le caractère qui possède premier min
			if((s->n->occ<=min2)&&(s->n->c!=min1_c))
			{
				min2 = s->n->occ ; 
				min2_c = s->n->c ; 
				deuxiem_min = s->n ; 
			}
			
			s = s->next ; 
		}
		
		// inserer le noeud dans la liste qui contient les deux min
		min = insertTas(min,deuxiem_min) ; 
		
		
	
	}

	return min ; 
	

}

//***********  l'occurence d'un caractère 
int charocc(char c,const char* s)
{
	
	int i = 0 ; 
	
	int occ = 0 ; 
	
	while(i<strlen(s))
	{
		if(s[i]==c)
		{
			occ++ ; 	
		}	
		
		i++ ; 
	}
	
	return occ ; 
}

//************ remplir une liste
//** la liste contiendra des noeuds d'arbre , ou chacun 
//** contient un carcatère unique de la chaine et son occurence 
struct tas* remplir(struct tas* t, const char* s)
{
	int i = 0 ; 
	int j = 0 ; 
	
	char c[200] ; 
	
	struct node* r  ; 

	while(i<strlen(s))
	{
		if(!exist(s[i],c))
		{
			// créeation d'un noeud d'arbre qui contient le caractère unique et son occurence
			r = NULL ; 
			r = insert(r,s[i],charocc(s[i],s)) ; 
			// inserer le noeud dans le tas
			t = insertTas(t,r) ; 
			c[j] = s[i] ; 
			j++ ; 
		}
		
		i++ ; 
	}
	
	return t ; 
	

}
//**** verifier l'existance d'un caractère dans une chaine 
int exist(char s, const char* c)
{
	int i = 0 ; 

	while(i<strlen(c))
	{
		if(c[i]==s)
		{
			return 1 ; 
		}

	   i++ ; 
	   
	}
	
	return 0 ; 
	
}

//********* trier une liste 
struct tas* tri(struct tas* t)
{
	
 	int i ;
	int j ;
	struct node* a ;

    struct tas *s1 , *s2;

    for(s1=t;s1!=NULL;s1=s1->next)
      {
        for(s2=s1->next ; s2!=NULL ; s2=s2->next)
          { 
            if(s2->n->occ > s1->n->occ)
              {
                a = s1->n ;
                s1->n = s2->n ;
                s2->n = a ;
              }
           }
       }
       
    s1 = t ; 
    
	return t ;  
}
//***** affichage 
void print(struct node* s)
{
	if(s!=NULL)
	{
		printf(" %c : %d - ",s->c,s->occ) ; 
		print(s->left) ; 
		print(s->right) ;  
	}
	else
	{
		printf(" * ") ; 
	}
}

void printTas(struct tas* t)
{
	if(t!=NULL)
	{
		printf(" %c : %d - ",t->n->c,t->n->occ) ; 
		printTas(t->next) ; 
	}
	else
	{
		printf("\n") ; 
	}
}

//****** copier un eliste dans une autre
struct tas* copy(struct tas* r,struct tas* t)
{
	struct tas* s = r ; 
	
	while(s!=NULL)
	{
		t = insertTas(t,s->n) ; 
		
		s = s->next ; 
	 } 
	 
	 return t ;  
}

//******** copier arbre
struct node* copyNode(struct node* r,struct node* t)
{
	if(r==NULL)
	{
		return r ; 
	}
	
	// créer directement une racine qui contient le caractère de la racine
	// de l'arbre qu'on veut copier ainsi que son occurence et ses fils	
	t = getNewNode(r->c,r->occ,copyNode(r->left,t),copyNode(r->right,t)) ; 
	
	return t ; 
}

//*************** remplire le fichier en parcourant l'arbre
void write_bin(struct node* r,int b[],int n,FILE* f)
{			
    
	if(r!=NULL)
	{
		//**** si le fils gauche n'est pas null	
		if(r->left!=NULL)
		{
			// si l'occurence dans le fils droit est plus grande que celle dans le fils gauche
			if((r->right!=NULL)&&(r->right->occ>r->left->occ))
			b[n] = 1 ;  // assigner 1 dans le tableau temporaire
			else
			b[n] = 0 ; // assigner 0 dans le tableau temporaire
			
			// appel reccursif à la fonction avec en paramètre le fils gauche, le tableau temporaire et l'indice + 1 
			write_bin(r->left,b,n+1,f) ; 
	
		}
		
		//***** si le fils droit n'est pas null
		if(r->right!=NULL)
		{	
			// la même chose que précedement
			if((r->left!=NULL)&&(r->right->occ>r->left->occ))
			b[n] = 0 ; 
			else
			b[n] = 1 ; 
			
			write_bin(r->right,b,n+1,f) ; 
	
		}
		
		//**** dans le cas d'une feuille 
		// si c'est une feuille alors c'est le caractère de départ
		if(r->left==NULL&&r->right==NULL)
		{
			//*** inserer le caractère dans le fichier	
			fprintf(f,"\n %c : ",r->c) ; 
			printf("\n %c : \n",r->c) ; 
			
			//**** remplir le fichier par le tableau temporaire 
			// en le parcourant jusqu'a l'indice
			int i ; 
			for(i=0 ; i < n ; ++i )
			{
				printf("%d",b[i]) ; 
				fprintf(f,"%d",b[i]) ; 
			}
			
			// calcule lmoy
			moyenne += ((float)charocc(r->c,global_s)/(float)strlen(global_s))*n ; 
			

		}
		
	
		
	}
}

//******** algorithme de Huffman construction de l'arbre
struct node* huff_c(struct tas* t)
{
	
	struct tas* s = NULL ; 
	s = copy(t,s) ; 
	
	// arbre resultat
	struct node* a = NULL ; 
	
	// trier la liste de départ ( optionnel ) 	
	s = tri(s) ;

	// si la taille du tas = 1 
	// alors on sait qu'il ne rest que la racine de l'arbre 
	while(length(s)>1) 
	{
		
	
		// un tas qui contiendra les min 
		struct tas* min = getMin(s) ; 
		
		// calculer la somme des deux min
		int new_occ = min->n->occ + min->next->n->occ ; 
		// créer une nouveau noeud d'arbre avec cette nouvelle somme 
		// et qui a pour fils gauche le plus petit des deux min et pour le fils droit le plus grand des deux min
		// mettre l'un des deux caractère dans le nouveau noeud
		// un noeud d'arbre 
		struct node* r = NULL ; 
		r = insert(r,min->n->c,new_occ) ; 
		
		struct node* min1 = NULL ; 
		struct node* min2 = NULL ; 
		min1 = copyNode(min->n,min1) ; 
		min2 = copyNode(min->next->n,min2) ; 
	
		//******* si l'occurence dans le premier noeud dans la liste du min > celle du deuxième 		
	    if(min1->occ<min2->occ)
		{
			r->left = min1 ;  // fils gauche reçoit le plus petit
			r->right = min2 ; // fils droit reçoit le plus grand
		}
		else
		{
			r->left = min2 ;
			r->right = min1 ; 	
		}


		// inserer le noveau noeud dans le tas
		//printTas(s) ; 
		s = insertTas(s,r) ; 
	
		
		// supprimer les deux min du tas
		deleteTas(&s,min->n) ; 
		deleteTas(&s,min->next->n) ; 
		
		

	}
	
	return s->n ; 
	
}


