#ifndef MEMCHECK_H
#define MEMCHECK_H
static int totalsize=0;
/*#define malloc(size)\
({\
	void *result = malloc(size);\
	//printf("malloc(%u),returns %p[%s:%d,%s]\n", (unsigned int) size,result,__FILE__,__LINE__,__FUNCTION__);\
        printf ("malloc %p from %s:%d,%s size %u  \n", result,__FILE__,__LINE__,__FUNCTION__, (unsigned int) size);\
	totalsize+=size;\
	result;\
})*/

/*#define calloc(nmemb,size)\
({\
	void *result = calloc(nmemb,size);\
	//printf("calloc(%u*%u),returns %p[%s:%d,%s]\n", (unsigned int) nmemb, (unsigned int) size,result,__FILE__,__LINE__,__FUNCTION__);\
        printf ("realloc %p from %s:%d,%s size %u  \n", result,__FILE__,__LINE__,__FUNCTION__, (unsigned int) size * nmemb);\
	totalsize+=nmemb*size;\
	result;\
})
*/
/*#define free(ptr)\
({\
	free(ptr);\
	printf("free %p[%s:%d,%s]\n",ptr,__FILE__,__LINE__,__FUNCTION__);\
})
*/



/*************************S2.wrap**************************/
/*void *__real_calloc(size_t num, size_t size);
   
  
void *__wrap_calloc(size_t num, size_t size)  
{  
	void *result = __real_calloc(num,size);  
        printf("calloc(%u*%u) called from X returns %p\n", (unsigned int) num, (unsigned int) size, result);  
        return result;  
  
}  */


/*void* __real_malloc(size_t size); // 只声明不定义__real_malloc
void* __wrap_malloc(size_t size) // 定义__wrap_malloc
{
    printf("__wrap_malloc called, size:%zd\n", size); // log输出
    return __real_malloc(size); // 通过__real_malloc调用真正的malloc
}*/





/*****************************S1.HOOK*************************/
/*https://linux.die.net/man/3/__malloc_hook*/
/*https://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html*/
/*http://blog.csdn.net/lbo4031/article/details/51534547
/*ONLY FOR malloc,realloc,memalign,free,malloc_initialize,after_morecore*/
/*IT's unsafe in mutithreads, when it's in mutithread should consider using way 2.wrap*/
#include <malloc.h>
/* Prototypes for our hooks.  */
static void my_init_hook (void);
static void my_free_hook (void*, const void *);
static void *my_malloc_hook(size_t, const void *);
static void *my_realloc_hook(void *, size_t, const void *);
/* Variables to save original hooks. */
static void *(*old_malloc_hook)(size_t, const void *);
static void (*old_free_hook)(void*, const void *);
static void *(*old_realloc_hook)(void *, size_t, const void *);

#define PRINTLOG(type,...) { printf(#type " %p from %p size %u  \n",##__VA_ARGS__); }
static void hook_save_old(void)
{
  old_malloc_hook = __malloc_hook;
  old_realloc_hook = __realloc_hook;
  old_free_hook = __free_hook;

}
static void hook_restore_old(void)
{
  __malloc_hook = old_malloc_hook;
  __realloc_hook = old_realloc_hook;
  __free_hook = old_free_hook;
}

static void hook_ini(void)
{
  __malloc_hook = my_malloc_hook;
  __realloc_hook = my_realloc_hook;
  __free_hook = my_free_hook;
}

static void my_init(void)
{
  hook_save_old();
  hook_ini();
}


/*
about the print
1.format,should like this:
malloc/realloc/free %addr% from %caller% size %size%
becase the script for pasing check.awk will analyze the logs
2.if log start with *, then using check.awk will print the whole line,not parsing it,it's a comment line
*/
static void * my_malloc_hook (size_t size, const void *caller)
{
  void *result;
  /* Restore all old hooks */
  hook_restore_old();
  /* Call recursively */
  result = malloc (size);
  /* Save underlying hooks */
  hook_save_old();
  /* printf might call malloc, so protect it too. */
  printf ("malloc  %12p from %12p size %10u  \n", result,caller, (unsigned int) size);
  //PRINTLOG(malloc, result,caller, (unsigned int) size);
  /* Restore our own hooks */
  hook_ini();
  return result;
}

static void *my_realloc_hook(void *ptr, size_t size, const void * caller )
{
  void *result;
  /* Restore all old hooks */
  hook_restore_old();
  /* Call recursively */
  result = realloc (ptr,size);
  /* Save underlying hooks */
  hook_save_old();
  /* printf might call malloc, so protect it too. */
  printf ("realloc %12p from %12p size %10u oldptr %12p  \n", result,caller, (unsigned int) size, ptr);
  //PRINTLOG(reaclloc, result,caller, (unsigned int) size);
  /* Restore our own hooks */
  hook_ini();
  return result;
}


static void my_free_hook(void *ptr, const void *caller)
{
  /* Restore all old hooks */
  hook_restore_old();
  /* Call recursively */
  free (ptr);
  /* Save underlying hooks */
  hook_save_old();
  /* printf might call free, so protect it too. */
  printf ("free    %12p from %12p\n", ptr,caller);
  /* Restore our own hooks */
  hook_ini();
}

#endif


