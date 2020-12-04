#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <magic.h>
#include <fcntl.h>
#include <wait.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <dlfcn.h>

int stringFileSearch(char *name,char* look){
    FILE *fp;
    fp = fopen(name,"r");
    char c;
    if(fp==NULL){
        return 1;
    }
    int pos=0;      
    while(!feof(fp)){
        c=fgetc(fp);
        if(look[pos]=='\0'){
            return 1;
        }
        if(look[pos]==c){
            pos=pos+1;
        }
        else{
            pos=0;
        }
    }
    fclose(fp);
return 0;
}


char* part_string(const char* chaine,int l){
    int i=0;
    char *s=malloc(sizeof(char)*(l+1));
    while(i<=l){
        s[i]=chaine[i];
        i++;
    }
    s[l+1]='\0';
return s;

}




void droitFichier(char* name){
    const char *month[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    struct stat fileStat;
    struct passwd *pswd; //It provides information about a user account
    struct group *grp;// It provides information about a group
    struct tm *time;//Time struct
    stat(name,&fileStat);
    grp=getgrgid(fileStat.st_gid); 
    pswd=getpwuid(fileStat.st_uid);
    char stringTime[100]="";
        printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWUSR) ? "w" :"-");
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf(" ");
        printf("%ld ",(long)fileStat.st_nlink );
        printf("%s ",pswd->pw_name );//User's login name.
        printf("%s ",grp->gr_name);//Group name.
        printf("%lld ",(long long)fileStat.st_size );
        time=localtime(&fileStat.st_mtime);
        printf("%s ",month[time->tm_mon] ); //return the month
        strftime(stringTime,100,"%d %H:%M",localtime(&fileStat.st_mtime));//formats the time
        char  test='0';
        char test2=stringTime[0];
        if (test2==test){

            char stringTime2[100];
            int k ;
            for (int i=1; (unsigned)i < strlen(stringTime); i++){
                stringTime2[i]=stringTime[i];
                k=i;
            }

            stringTime2[k+1]='\0';
            printf(" %s",stringTime2 );
        }else{
            printf(" %s ",stringTime );    
        }
        
        printf("%s\n",name );
      

}

int fname(char *name, int level,char *look,char indice,char * texte){
    DIR *dir;
    struct dirent *input;
    if (!(dir = opendir(name))){

        return 1;
    }
    if (indice=='i'){
        
        while ((input = readdir(dir)) != NULL) {
            if (input->d_type == DT_DIR) {
                char path[1000];
                if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
                    sprintf(path, "%s/%s", name, input->d_name);   

                   fname(path,level+1,look,'i',texte);
                }else{
                    
                }
    
            } else {

                                        int isImage=0;
                                        magic_t (*magic_open)(int);
                                        int (*magic_load)(magic_t,const char*);
                                        const char* (*magic_file)(magic_t,const char*);
                                        void (*magic_close)(magic_t);
                                        void *libmagic = dlopen("libmagic.so",RTLD_LAZY);
                                        if (libmagic){
                                            magic_open = dlsym(libmagic,"magic_open");
                                            magic_load = dlsym(libmagic,"magic_load");
                                            magic_file = dlsym(libmagic,"magic_file");
                                            magic_close = dlsym(libmagic,"magic_close");
                                        }

                                                 char path[1000];
                                                sprintf(path, "%s/%s", name, input->d_name);
                                                const char *content;
                                                magic_t cookie;
                                                cookie = magic_open(MAGIC_MIME);
                                                magic_load(cookie, NULL);       
                                                content = magic_file(cookie, path);
                                               // printf("is image %s %s\n",content,path );
                                                        if(strcmp(part_string(content,4),"image")==0){                                            
                                                            magic_close(cookie);

                                                            isImage=1 ;
                                                        }else{
                                                            isImage=0;                    
                                                        }
                                                


                int equal=(strcmp(input->d_name,look)==0);
                if(strcmp(input->d_name,look)==0){
                  //  printf("%s %d\n",path,isImage );
                    if((level==0)&&(isImage)&&equal){
                        printf("%s\n", path);
                    }else if((level!=0)&&(isImage)&&equal){
                        printf("%s\n", path);
                    } else{
                    }
           
            }

        }
          
      }
      closedir(dir); 
    }
    else if(indice =='t'){

while ((input = readdir(dir)) != NULL) {
        if (input->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
               sprintf(path, "%s/%s", name, input->d_name);
                fname(path,level+1,look,'t',texte);
            }else{
                
            }
        } else {
            char path[1000];
            int res;
            int equal=(strcmp(input->d_name,look)==0);
            sprintf(path, "%s/%s", name, input->d_name);
            res=stringFileSearch(path,texte);
            
            if(res){
                if((level==0)&&equal){
                        printf("%s\n", path/*input->d_name*/);
                }else if((level==1)&&equal){
                        printf("%s\n", path/*input->d_name*/);
                }else{

                }   
            }
        }
    }





    }else{


            while ((input = readdir(dir)) != NULL) {
                if (input->d_type == DT_DIR) {
                    char path[1000];
                    if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
                       sprintf(path, "%s/%s", name, input->d_name);   

                        fname(path,level+1,look,'x',texte);
                    }else{
                        
                    }
            
                } else {
                    char path[1000];
                    sprintf(path, "%s/%s", name, input->d_name);
                    if(strcmp(input->d_name,look)==0){
                        if(level==0){
                            printf("%s\n", path);
                        }else{
                            printf("%s\n", path);
                        }  
                }
            }
           }
            closedir(dir);
}
return 0;

}

char *remplacerCrechets(char *cmd, char *path) { 
    int pathLen = strlen(path);
    int occ = 0,i=0; 
    char subCmd[2];

    for ( i = 0; cmd[i] != '\0'; i++) {
            strncpy(subCmd,cmd+i,2);
        if (strstr(subCmd, "{}") == subCmd) { 
            occ++;  
            i =i+ 1; 
        } 
    } 
    char *res; 

    res = (char *)malloc((int)strlen(cmd)+1 + occ * (pathLen - 2) ); 
    int j=0; 
    i=0;
    while (cmd[j]!='\0') { 
        strncpy(subCmd,cmd+j,2);
        if (strstr(subCmd, "{}") == subCmd) { 
            strcpy(&res[i], path); 
            i =i+ pathLen; 

            j=j+2;
        } 
        else{
            res[i] = cmd[j]; 
            i=i+1;
            j=j+1;
        }
        
    } 
    res[i] = '\0'; 
    return res; 
} 

void exec2(char *name,  char * cmd, char* d_name){
    pid_t d;
    int status;           
             if((d=fork())>0){
                waitpid(d,&status,0);
               
            }else{

                char path2[1000];

                sprintf(path2, "%s/%s", name, d_name); 
                char path3[1000];
                memset(path3, '\0', sizeof(path3));
                if(path2[strlen(path2)-1]=='/'){
                    strcpy(path3,path2); 
                    path3[strlen(path2)-1]='\0';
                }else{
                    strcpy(path3,path2);
                }
    
                char *cmdOps = NULL;
                cmdOps = remplacerCrechets(cmd,  path3); 
     
                execlp("sh","sh","-c",cmdOps,NULL);
               exit(0);
            }

}

int exec(char *name,int level,  char * cmd){
    DIR *dir;
    struct dirent *input;
    char cmdCopy[1000];  
    char *cmdSimple,*cmdSimple2 ;
    strcpy(cmdCopy, cmd);
    cmdSimple = strtok(cmdCopy,"\""); // cmd without the first double quote
    cmdSimple2=strtok(cmdSimple,"\"");   // cmd without both double quotes
    if (!(dir = opendir(name)))
        return 1;

    exec2(name,cmdSimple2,"");     
    while ((input = readdir(dir)) != NULL) {           
        if ( strcmp(input->d_name, "..")&& ( strcmp(input->d_name, ".") != 0))  { 
            if (input->d_type == DT_DIR ) {
            char path[1000];
               
               sprintf(path, "%s/%s", name, input->d_name); 
               
                exec(path,level+1,cmd);
            }else{
               exec2(name,cmdSimple2,input->d_name); 
            }
 
            
       

    
}
}
    closedir(dir);
return 0;
}



int text(char *name, int level,char *look)
{


    if (strcmp(name,"none")==0){
        DIR *dir;
    struct dirent *input;


    if (!(dir = opendir(".")))
        return 1;

    while ((input = readdir(dir)) != NULL) {
        if (input->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
               sprintf(path, "%s/%s", name,input->d_name);
                text(path,level+1,look);
            }else{
                
            }
        } else {
            char path[1000];
            int res;
            sprintf(path, "%s/%s",name, input->d_name);
            res=stringFileSearch(path,look);
            
            if(res){
                if(level==0){
                    printf("%s\n", path);
                }else{
                    printf("%s\n", path);
                }  
            }
        }
    }
    closedir(dir);
    }else{


    DIR *dir;
    struct dirent *input;

    if (!(dir = opendir(name)))
        return 1;

    while ((input = readdir(dir)) != NULL) {
        if (input->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
               sprintf(path, "%s/%s", name, input->d_name);
                text(path,level+1,look);
            }else{
                
            }
        } else {
            char path[1000];
            int res;
            sprintf(path, "%s/%s", name, input->d_name);
            res=stringFileSearch(path,look);
            
            if(res){
                if(level==0){
                    printf("%s\n", path);
                }else{
                    printf("%s\n", path);
                }  
            }
        }
    }
    closedir(dir);
    }
    
return 0;
}







int droit(char *name, int level)
{
    DIR *dir;
    struct dirent *input;
    if (!(dir = opendir(name)))
        return 1;
    if(level==0){
        droitFichier(name);
    }
    while ((input = readdir(dir)) != NULL) {
        if (input->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
               sprintf(path, "%s/%s", name, input->d_name);
                droitFichier(path) ;  
                droit(path,level+1);                                 
            }else{
                
            }
    
        } else {
             char path[1000];
            sprintf(path, "%s/%s", name, input->d_name);
            droitFichier(path) ;
    }
}
    closedir(dir);
return 0;
}



int image(char *name, int level)
{

magic_t (*magic_open)(int);

int (*magic_load)(magic_t,const char*);
const char* (*magic_file)(magic_t,const char*);
void (*magic_close)(magic_t);
void *libmagic = dlopen("libmagic.so",RTLD_LAZY);
if (libmagic){
   magic_open = dlsym(libmagic,"magic_open");
   
   magic_load = dlsym(libmagic,"magic_load");
   magic_file = dlsym(libmagic,"magic_file");
   magic_close = dlsym(libmagic,"magic_close");
}




    DIR *dir;
    struct dirent *input;

    if (!(dir = opendir(name)))
        return 1;

    while ((input = readdir(dir)) != NULL) {
        if (input->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
                sprintf(path, "%s/%s", name, input->d_name);
                const char *content;
                magic_t cookie;
                cookie = magic_open(MAGIC_MIME);
                magic_load(cookie, NULL);       
                content = magic_file(cookie, path);
                if(level==0){
                    if(strcmp(part_string(content,4),"image")==0){                                            
                        magic_close(cookie);
                        printf("%s\n", path);
                    }else{
                        //continue;                    
                    }
                }else{
                    if(strcmp(part_string(content,4),"image")==0){                                            
                        magic_close(cookie);
                        printf("%s\n", path);
                    }else{
                        //continue;                    
                    }
                }                                    
                image(path,level+1);
            }else{
                
            }
    
        } else {
            char path[1000];
            const char *content;
            magic_t cookie;
            cookie = magic_open(MAGIC_MIME);
            magic_load(cookie, NULL);
            sprintf(path, "%s/%s", name, input->d_name);       
            content = magic_file(cookie, path);
            if(level==0){
                    if(strcmp(part_string(content,4),"image")==0){                                            
                        magic_close(cookie);
                        printf("%s\n", path);
                    }else{
                        //continue;                    
                    }
            }else{
                    if(strcmp(part_string(content,4),"image")==0){                                            
                        magic_close(cookie);
                        printf("%s\n", path);
                    }else{
                        //continue;                    
                    }
            }
        }
    }
    closedir(dir);
    dlclose(libmagic);
return 0;
}




int sousrep(char *name, int level)
{
    DIR *dir;
    struct dirent *input;

    if (!(dir = opendir(name)))
        return 1;

    while ((input = readdir(dir)) != NULL) {
        if (input->d_type == DT_DIR) {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
               sprintf(path, "%s/%s", name, input->d_name);
                if(level==0){
                    printf("%s\n", path/*input->d_name*/);
                }else{
                    printf("%s\n", path/*input->d_name*/);
                }                                    
                sousrep(path,level+1);
            }else{
                
            }
    
        } else {
            char path[1000];
            if (strcmp(input->d_name, ".") != 0 && strcmp(input->d_name, "..") != 0){    
               sprintf(path, "%s/%s", name, input->d_name);
            }
            if(level==0){
                printf("%s\n", path/*input->d_name*/);
            }else{
                printf("%s\n", path/*input->d_name*/);
            }
        }
    }
    closedir(dir);
return 0;
}


static struct option longopts[] = {
   { "all",    no_argument,       NULL,    'a'   },
   { "help",    no_argument,       NULL,    'h'   },
   { "i", no_argument,       NULL, 'i'   },
   { "l"   , no_argument, NULL,         'l' },
   { "t"   , required_argument, NULL,         't' },
   { "exec"   , required_argument, NULL,         'e' },
   { "name"   , required_argument, NULL,         'n' },
   { "print"   , no_argument, NULL,         'p' },
   { 0, 0, 0, 0 }
};



int main(int argc, char  *argv[]){
	char c;

    extern char* optarg;
    extern int optind;
    char texte[100];


    while ( (c = getopt_long(argc, argv, " n:e:t:lihap",longopts,NULL)) != -1 ) {
        switch ( c ) {
            case 'a':
            	printf("les sous repertoires et fichiers à %s sont :\n",argv[2] );
                sousrep(argv[1],0);
                
                return(0);
                //break;
            case 'i': 
                if(strcmp(argv[1],"-i")==0){
                    image(".",0);
                }else{
                    image(argv[1],0);
                }
                
                return(0);
                
            case 'p':
                
                if(strcmp(argv[1],"--print")==0){
                	printf(".\n" );
                    sousrep(".",0);
                }else{
                	printf("%s\n",argv[1] );    
                    sousrep(argv[1],0);
                }
                return 0;
                
            case 'h':
                
                printf(" les options possibles : -n, -i,-h ,-l ,-t \n");
                printf(" L'éxecution de rsfind sans paramètres affichera par défaut les dossiers (et sous dossier) et fichiers (et sous fichiers)  \n");
                printf("-t : exemple : ./rsfind -t NOM_DOSSIER MOT_A_CHERHCER\n");
                printf("-e : exemple : ./rsfind -e NOM_DOSSIER COMMANDE\n");
                printf("-x : exemple : ./rsfind -x NOM_DOSSIER NOM_DE_FICHIER\n");
                printf("-i : l'option -i renvoie toutes les images Exemple : './rsfind -i' \n");
                printf("-n : l'option -n permet de lister les dossiers (et sous dossier) et fichiers (et sous fichiers)  dans le chemin donné Exemple: './rsfind -n ./toto' \n");
                printf("-l : l'option -l permet de lister les droits de l'utilisateur dans le dossier donné Exemple : './rsfind -l ./toto'\n");
                return 0;
            case 'l':
                
                if(strcmp(argv[1],"-l")==0){
                    droit(".",0);
                }else{
                    droit(argv[1],0);
                }
                
                
                return 0;
            case 't':

                    strcpy(texte,argv[optind-1]);
        			for( ;optind < argc /*&& *argv[optind] != '-'*/; optind++){
              			sprintf(texte,"%s %s",texte,argv[optind] );         
        			}
                    if(strcmp(argv[1],"-t")==0){
                        text(".",0,texte);
                    }else{
                        text(argv[1],0,texte);
                    }

                                
                return 0;
                //break;
            case 'e':

            	    strcpy(texte,argv[optind-1]);
        			for( ;optind < argc /*&& *argv[optind] != '-'*/; optind++){
              			sprintf(texte,"%s %s",texte,argv[optind] );         
        			}

                	if(strcmp(argv[1],"--exec")==0){
                    	exec(".",0,texte);
                	}else{
                    	exec(argv[1],0,texte);
                	}
                
            	return 0;
                //break;
            case 'n':
                    strcpy(texte,argv[optind-1]);
                    for( ;optind < argc && *argv[optind] != '-'; optind++){
                        sprintf(texte,"%s %s",texte,argv[optind] ); 
        
                    }
                    if (argc>4){
                            strcpy(texte,argv[argc-1]);
                            if(strcmp(argv[1],"--name")==0){
                                fname(".",0,optarg,argv[optind][1],texte);
                        	   
                            }else{
                            	fname(argv[1],0,optarg,argv[optind][1],texte);
                        	}

                    }else{
                        if (strcmp(argv[argc-1],"-i")==0){
                            if(strcmp(argv[1],"--name")==0){
                                fname(".",0,optarg,'i',texte);
                           
                            }else{
                                fname(argv[1],0,optarg,'x',texte);
                            }   
                        }else{
                            if(strcmp(argv[1],"--name")==0){
                                fname(".",0,optarg,'x',texte);
                           
                            }else{
                                fname(argv[1],0,optarg,'x',texte);
                            }
                            
                        }
                    }
                    
                    
                return 0;

            case '?':

                fprintf(stderr, "Ligne de commande incorrecte.\n");
                printf("Pour acceder au MAN de la commande RSFIND veuillez utiliser la commande -h 'exemple : ./rsfind -h' \n");
                return 1;
        }   
    }
if(argc>1){
	printf("%s\n",argv[1] );
    sousrep(argv[1],0);
}else{
	printf(".\n");
    sousrep(".",0);
}

	return 0;
}
