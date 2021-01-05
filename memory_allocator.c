

#include <stdio.h>

typedef enum {bestFit, worstFit, firstFit} Strategy;

Strategy strategy = bestFit;


typedef struct{
    unsigned int size;
    unsigned int isfree;
}Info;

typedef struct{
    Info info;           /*number of  16 byte blocks*/
    struct block *next;  /*next free*/
    char data[0];        /*start of the allocated memory*/
}Block;
/*Block: |4byte(size)|4byte(isfree)|8-byte next|0-byte(data)|*/

static Block *free_list = NULL;     /*start of the free list*/
static Block *heap_start = NULL;    /*head of allocated memory from sbrk */


void *mymalloc(size_t size);
void *myfree(void *p);
Block *split(Block *b, size_t size);
int test(int fitStrategy);
#include <stdio.h>
#include <zconf.h>
#include "mymalloc.h"

void *mymalloc(size_t size){
    int first=1;
    while(first){
        heap_start=sbrk(sizeof(Block)*2*1024);
        if (heap_start == (void*) -1)
            return NULL;
        heap_start->info.isfree=1;
        heap_start->info.size=1024;
        heap_start->next=NULL;
        heap_start->data[0]=" ";
        free_list=(void*)heap_start+ sizeof(Block)+heap_start->info.size;
        free_list->info.isfree=0;
        free_list->info.size=1024;
        free_list->next=NULL;
        first--;
    }
    if(strategy==firstFit){
        Block *p = heap_start;
        while( p != free_list&&(p->info.isfree|| p->info.size )){
            p = (void *)p + sizeof(Block) + p->info.size;
        }
        if (p == NULL){
            fprintf(stderr,"no available space");
            return NULL;
        }else if(size < p->info.size){
            split(p, size);
        }
        return (void*)p+sizeof(Block); /*start of the block address*/
    }
    else if(strategy==worstFit){
        Block *p = heap_start;
        while(p!=free_list&&(p->info.size)){
            p = (void *)p + sizeof(Block) + p->info.size;
            if(p->info.isfree &&(free_list->info.size<p->info.size)){
                free_list->info.size<=p->info.size;
            }
        }
        if(size < p->info.size){
            split(p, size);
        } else if (p==NULL) {
            fprintf(stderr,"no available space");
            return NULL;
        }
        return (void*)p+sizeof(Block);
    }
    else if (strategy==bestFit){
        Block *p = heap_start;
        while(p!=free_list&&(p->info.size)){
            p = (void *)p + sizeof(Block) + p->info.size;
            if(p->info.isfree &&((free_list->info.size)>(p->info.size))){
                ////////////////
                p->info.size=free_list->info.size;
            }
        }
        if(size > free_list->info.size){
            split(p, free_list->info.size);
        } else if (p==NULL) {
            printf(stderr,"no available space");
            return NULL;
        }
        return (void*)p+sizeof(Block);
    }
}

Block *split(Block *b, size_t size){
   ////freeliste free yeri eklenecek

    int size1=b->info.size;
    Block *k=sizeof(Block)+size+sizeof(b)+b;
    k->info.size=size1-size- sizeof(Block);
    return k;
}

int test(int fitStrategy){
    if (fitStrategy==1){
        strategy=worstFit;
    } else if (fitStrategy==2){
        strategy=bestFit;
    } else if (fitStrategy==3){
        strategy=firstFit;
    }
    mymalloc(10);
    myfree(mymalloc(10));
}

void *myfree(void *p){
    Block *w= p- sizeof(Block);
    w->info.isfree=0;
    free_list->next=(Block*)w+sizeof(w->data)+ sizeof(w->info)+ sizeof(w->next)+w->info.size;
    free_list->info.isfree=1;
    Block *next=w+w->info.size;
    if(next->info.isfree==0){
        w->info.size+=next->info.size+ sizeof(Block);
        free_list->next=(Block*)w+sizeof(w->data)+ sizeof(w->info)+ sizeof(w->next)+w->info.size;
        free_list->info.size+=w->info.size;
    }
    Block *prev=w-sizeof(Block);
    if(prev->info.isfree==0){
        w->info.size+=prev->info.size+ sizeof(Block);
        free_list->next=(Block*)w+sizeof(w->data)+ sizeof(w->info)+ sizeof(w->next)+w->info.size;
        free_list->info.size+=w->info.size;
    }
    return (void *)w;
}

int main() {
    int i=1;
    test(i);
    return 0;
}
