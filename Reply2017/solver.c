#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

int nt;

typedef struct point_t{
    int x;
    int y;
}point;

typedef struct triangle_t{
    point A;
    point B;
    point C;
}triangle;

typedef struct plug_t{
    triangle *Tr;
    point * oPath;
    point * shortpath;
    point goal;
    int size;
    int shortsize;
}plugin;

int Xor(int x,int y){
    return !x ^ !y;
}

int Area(point p1, point p2, point p3){
    return (p2.x-p1.x)*(p3.y-p1.y)-(p3.x-p1.x)*(p2.y-p1.y);
}

int Left(point p1,point p2,point p3){
    return Area(p1,p2,p3)>0;
}

int LeftOn(point p1,point p2,point p3){
    return Area(p1,p2,p3)>=0;
}

int Collinear(point p1,point p2,point p3){
    return Area(p1,p2,p3)==0;
}

int IntersectProp(point a,point b,point c,point d){
    //Eliminate improper case:
    if(Collinear(a,b,c) || Collinear(a,b,d) || Collinear(c,d,a) || Collinear(c,d,b)){
        return 0;
    }
    return Xor(Left(a,b,c),Left(a,b,d)) && Xor(Left(c,d,a),Left(c,d,b));
}

int Bewtween(point a,point b,point c){
    point ba,ca;
    if(!Collinear(a,b,c)){
        return 0;
    }
    if(a.x!=b.x){
        return ((a.x<=c.x)&&(c.x<=b.x)) || ((a.x>=c.x)&&(c.x>=b.x));
    }
    else{
        return ((a.y<=c.y)&&(c.y<=b.y)) || ((a.y>=c.y)&&(c.y>=b.y));
    }
}

bool Intersect(point a,point b,point c,point d){
    if(IntersectProp(a,b,c,d)){
        return true;
    }
    else if(Bewtween(a,b,c) || Bewtween(a,b,d) || Bewtween(c,d,a) || Bewtween(c,d,b)){
        return true;
    }
    else{
        return false;
    } 
}

int newINT(void){
    int x;
    scanf("%d",&x);
    return x;
}

long distance2(point a,point b){
    return (long)((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}

void generatePoints(point *W,point p){
    W[0].x=p.x-1;
    W[0].y=p.y-1;
    W[1].x=p.x-1;
    W[1].y=p.y;
    W[2].x=p.x-1;
    W[2].y=p.y+1;
    W[3].x=p.x;
    W[3].y=p.y-1;
    W[4].x=p.x;
    W[4].y=p.y+1;
    W[5].x=p.x+1;
    W[5].y=p.y-1;
    W[6].x=p.x+1;
    W[6].y=p.y;
    W[7].x=p.x+1;
    W[7].y=p.y+1;
}

bool samePoint(point p1,point p2){
    if(p1.x==p2.x && p1.y==p2.y){
        return true;
    }
    return false;
}

bool notIN(point a,point *W,int k){
    int i;
    for(i=0;i<k;i++){
        if(samePoint(a,W[i])){
            return false;
        }
    }
    return true;
}

bool lineofSight(point a, point b,triangle *Tr){
    int i;
    for(i=0;i<nt;i++){
        if(Intersect(a,b,Tr[i].A,Tr[i].B) || Intersect(a,b,Tr[i].B,Tr[i].C) || Intersect(a,b,Tr[i].C,Tr[i].A)){
            return false;
        }
    }
    return true;
}

int shortcut(point *Pts,int k,triangle *Tr){
    int i;

}

void newPoint(point *a,int x,int y){
    a->x=x;
    a->y=y;
}

void * FromStartToEnd(void *p){
    plugin *p1=p;
    int i,j,k,h;
    point * path=p1->oPath;
    point * shortcut=p1->shortpath;
    triangle *obs=p1->Tr;
    point end=p1->goal;
    k=p1->size;
    long d1,d2;
    point buff,ptr;
    point *W=malloc(8*sizeof(point));
    buff=path[0];
    ptr=buff;
    for(k=1;k<1000 && !samePoint(path[k],end);k++){
        d1=1000000000;
        printf("Thread 1,Iterazione::\t%d\tPunto:: %d,%d\n",k,path[k-1].x,path[k-1].y);
        if(lineofSight(ptr,end,obs)){
            path[k]=end;
            break;
        }
        for(i=0;i<nt;i++){
            generatePoints(W,obs[i].A);
            for(j=0;j<8;j++){
                d2=distance2(W[j],end);
                if(d2<d1 && notIN(W[j],path,k)){
                    if(lineofSight(ptr,W[j],obs)){
                        if(d2<d1){
                            buff=W[j];
                            d1=d2;
                        }
                    }
                }
            }
            generatePoints(W,obs[i].B);
            for(j=0;j<8;j++){
                d2=distance2(W[j],end);
                if(d2<d1 && notIN(W[j],path,k)){
                    if(lineofSight(ptr,W[j],obs)){
                        if(d2<d1){
                            buff=W[j];
                            d1=d2;
                        }
                    }
                }
            }
            generatePoints(W,obs[i].C);
            for(j=0;j<8;j++){
                d2=distance2(W[j],end);
                if(d2<d1 && notIN(W[j],path,k)){
                    if(lineofSight(ptr,W[j],obs)){
                        if(d2<d1){
                            buff=W[j];
                            d1=d2;
                        }
                    }
                }
            }
        }
        if(samePoint(buff,path[k-1])){
            k=1000;
            break;
        }
        path[k]=buff;
        ptr=buff;
    }
    p1->size=k;
    p1->shortsize=k;
    if(k<1000){
        printf("\nShortcutting route 1...\n");
        j=0;
        h=k-1;
        int cont=0;
        for(i=j;i<=k;i++){
            shortcut[cont]=path[i];
            cont++;
            for(h=k;h>i;h--){
                if(lineofSight(path[i],path[h],obs)){
                    i=h-1;
                    break;
                }
            }
        }
        p1->shortsize=cont;
    }
    return NULL;
}

void * FromEndToStart(void *p){
    plugin *p1=p;
    int i,j,k,h;
    point * path=p1->oPath;
    point * shortcut=p1->shortpath;
    triangle *obs=p1->Tr;
    point end=p1->goal;
    k=p1->size;
    point *W=malloc(8*sizeof(point));
    long d1,d2;
    point buff,ptr;
    buff=path[0];
    ptr=buff;
    for(k=1;k<1000 && !samePoint(path[k],end);k++){
        d1=1000000000;
        printf("Thread 2,Iterazione::\t%d\tPunto:: %d,%d\n",k,path[k-1].x,path[k-1].y);
        if(lineofSight(ptr,end,obs)){
            path[k]=end;
            break;
        }
        for(i=0;i<nt;i++){
            generatePoints(W,obs[i].A);
            for(j=0;j<8;j++){
                d2=distance2(W[j],end);
                if(d2<d1 && notIN(W[j],path,k)){
                    if(lineofSight(ptr,W[j],obs)){
                        if(d2<d1){
                            buff=W[j];
                            d1=d2;
                        }
                    }
                }
            }
            generatePoints(W,obs[i].B);
            for(j=0;j<8;j++){
                d2=distance2(W[j],end);
                if(d2<d1 && notIN(W[j],path,k)){
                    if(lineofSight(ptr,W[j],obs)){
                        if(d2<d1){
                            buff=W[j];
                            d1=d2;
                        }
                    }
                }
            }
            generatePoints(W,obs[i].C);
            for(j=0;j<8;j++){
                d2=distance2(W[j],end);
                if(d2<d1 && notIN(W[j],path,k)){
                    if(lineofSight(ptr,W[j],obs)){
                        if(d2<d1){
                            buff=W[j];
                            d1=d2;
                        }
                    }
                }
            }
        }
        if(samePoint(buff,path[k-1])){
            k=1000;
            break;
        }
        path[k]=buff;
        ptr=buff;
    }
    p1->size=k;
    p1->shortsize=k;
    if(k<1000){
        printf("\nShortcutting route 2...\n");
        j=0;
        h=k-1;
        int cont=0;
        for(i=j;i<=k;i++){
            shortcut[cont]=path[i];
            cont++;
            for(h=k;h>i;h--){
                if(lineofSight(path[i],path[h],obs)){
                    i=h-1;
                    break;
                }
            }
        }
        p1->shortsize=cont;
    }
    return NULL;
}

long TotalDistance(point * path,int size){
    int i;
    long d=0;
    for(i=0;i<size-1;i++){
        d=d+distance2(path[i],path[i+1]);
    }
    return d;
}

int main(){
    int i,j,k,k2,h;
    long d1,d2;
    /* clock_t ptime;
    ptime=clock(); */
    pthread_t pth[2];
    FILE *f1;
    f1=fopen("output_path.txt","w+");
    point start;
    point end;
    point buff;
    start.x=newINT();
    start.y=newINT();
    end.x=newINT();
    end.y=newINT();
    nt=newINT();
    triangle *obs=malloc(nt*sizeof(triangle));
    point *path=malloc(1000*sizeof(point));
    point *path2=malloc(1000*sizeof(point));
    point *shortcut1=malloc(1000*sizeof(point));
    point *shortcut2=malloc(1000*sizeof(point));
    for(i=0;i<nt;i++){
        obs[i].A.x=newINT();
        obs[i].A.y=newINT();
        obs[i].B.x=newINT();
        obs[i].B.y=newINT();
        obs[i].C.x=newINT();
        obs[i].C.y=newINT();
    }
    path[0]=start;
    path2[0]=end;
    plugin p1;
    plugin p2;
    p1.oPath=path;
    p1.Tr=obs;
    p1.size=0;
    p1.goal=end;
    p1.shortpath=shortcut1;
    p2.oPath=path2;
    p2.Tr=obs;
    p2.size=0;
    p2.goal=start;
    p2.shortpath=shortcut2;
    pthread_create(&pth[0],NULL,FromStartToEnd,&p1);
    pthread_create(&pth[1],NULL,FromEndToStart,&p2);
    pthread_join(pth[0],NULL);
    pthread_join(pth[1],NULL);
    k=p1.size;
    k2=p2.size;
    printf("\n%d %d\n",k,k2);
    if((k==1000 || !samePoint(path[k],end)) && (k2==1000 || !samePoint(path2[k2],start))){
        fprintf(f1,"IMPOSSIBLE\n");
    }
    else
    {   
        // SHORTCUT
        k=p1.shortsize;
        k2=p2.shortsize;
        if(k==1000){
            printf("Road from end to start\n");
            fprintf(f1,"%d\n",k2);
            for(i=k2-1;i>=0;i--){
                fprintf(f1,"%d %d\n",shortcut2[i].x,shortcut2[i].y);
            }
        } else if(k2==1000){
            printf("Road from Start to end\n");
            fprintf(f1,"%d\n",k);
            for(i=0;i<k;i++){
                fprintf(f1,"%d %d\n",shortcut1[i].x,shortcut1[i].y);
            }
        }
        else if(TotalDistance(shortcut1,k)<TotalDistance(shortcut2,k2)){
            printf("Road from Start to end: long: %ld  > %ld\n",TotalDistance(shortcut1,k),TotalDistance(shortcut2,k2));
            fprintf(f1,"%d\n",k);
            for(i=0;i<k;i++){
                fprintf(f1,"%d %d\n",shortcut1[i].x,shortcut1[i].y);
            }
        } else{
            printf("Road from end to start\n");
            fprintf(f1,"%d\n",k2);
            for(i=k2-1;i>=0;i--){
                fprintf(f1,"%d %d\n",shortcut2[i].x,shortcut2[i].y);
            }
        }
    }
    printf("DONE\n");
    fclose(f1);
    /* ptime=clock()-ptime;
    printf("%f",(float)ptime/CLOCKS_PER_SEC); */
    return 0;
}