
/* Définition de l'architecture */
OUTPUT_ARCH ( arm )

/* Définition du point d'entrée */
ENTRY ( mk_shellApplication_main )

/* Définition de l'organisation mémoire */
MEMORY
{
  	K_MEMORY_RAM ( rwx ) : ORIGIN = 0x0000, LENGTH = 0x10000
}

/**
 *
 */

/* Définition des sections */
SECTIONS
{
    /* ********************************************************************** */ 
    
    /* Mapping de la section .descriptor */
    .descriptor : ALIGN ( 4 )
    {
    	KEEP ( *( .descriptor ) ) *( .descriptor.* );
    
    } > K_MEMORY_RAM
    
    /* ********************************************************************** */ 
    
    /* Mapping de la section .data */    
    .data : ALIGN ( 4 ) 
    { 
       	*( .data.* )
        
    } > K_MEMORY_RAM 
   
   	/* ********************************************************************** */ 
    
    /* Mapping de la section .bss */
    .bss ( NOLOAD ) : ALIGN ( 4 )
    {
       	*( .bss.* )  

    } > K_MEMORY_RAM 
    
    /* ********************************************************************** */ 
    
    /* Mapping de la section .stack */
    .stack ( NOLOAD ) : ALIGN ( 4 )
    {
    	KEEP ( *( .stack ) ) *( .stack );

    } > K_MEMORY_RAM 

    /* ********************************************************************** */ 
    
    /* Mapping de la section .got */
    .got : ALIGN ( 4 )
    {
       	*( .got.* )  
      
    } > K_MEMORY_RAM 
        
    /* ********************************************************************** */ 
    
    /* Mapping de la section .text */  
    .text : ALIGN ( 4 ) 
    { 
       *( .text.* ) 
       
    } > K_MEMORY_RAM 
 
    /* ********************************************************************** */ 
 
    /* Mapping de la section .rodata */  
    .rodata : ALIGN ( 4 ) 
    { 
       *( .rodata.* ) 
       
    } > K_MEMORY_RAM 

}







