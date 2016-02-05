/****************************************************************/
/*  head file of sd_v100.lib                                    */
/*  Hardware supported:  uPAC-5001, iP-8441, ip-8841, G-4500    */
/*      and other controllers which supports microSD/SD.        */
/*                                                              */
/*  Note: head file version is different to                     */
/*        lib file version.                                     */
/*                                                              */
/* Last Update [2010,06,08] by Kevin                            */
/****************************************************************/

/*=============================================================================
======== Software Specification ===============================================
===============================================================================        
Storage Media: Supports 1GB and 2GB microSD card only.
               microSDHC is not supported.
               
File System: FAT16 format only.
             you can format the microSD card as FAT format on PC 
             or call pc_format to perform the formatting.               
        
Disk number: 1
Disk size  : 1GB or 2GB  
File number: 65,517 files max.
File size  : 2GB max. for each file
File name  : 12 bytes max. (8-3 format)
File operation modes:
            1. Read: 
            2. Write: Creates a new file to write data,
                      or overwrite to a file (if the file is already exit).
            3. Append: appends data to a file.

File handle: 6 max.
    The 6 file handles can all be used for read/write/append operations.
            
Read/Write performance:
    Condition 1: 32767 bytes x 10 loops
        pc_read  = 125.3 KB/Sec
        pc_write =  95.3 KB/Sec
    
    Condition 2: 500 bytes x 100 loops
        pc_read  = 47 KB/Sec
        pc_write = 12 KB/Sec   
        
    G-4500    
    Condition 1: 5000 bytes x 10 loops
        pc_read  = 100 KB/Sec
        pc_write =  37 KB/Sec
    
    Condition 2: 500 bytes x 100 loops
        pc_read  = 48 KB/Sec
        pc_write = 12 KB/Sec           

    
Max. length of writing data: 32767 bytes.
Max. length of reading data: 32767 bytes.

Hardware supported:
    iP-8444, iP-8841,
    iP-8411, iP-8811,
    G-4500 series,
    uPAC-5000 series
    
Hardware resource occupied:
    non.
*/

/*=============================================================================
======== microSD.h update history =============================================
===============================================================================

[2010,06,08]
    Add comments for APIs.

-------------------------------------------------------------------------------        
[2009,11,02] 
    rename 
        1. pc_lseek to pc_seek 
    add 
        2. pc_tell        
-------------------------------------------------------------------------------
[2009,09,21] first released.  

*/

#ifdef __cplusplus
extern "C" {
#endif

#define TRUE  1
#define FALSE 0

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;


#if (VFAT)
#define EMAXPATH        260  /* Maximum path length. Change if you like */
#else
#define EMAXPATH        148  /* Maximum path length. Change if you like */
#endif


/* Structure for use by pc_gfirst, pc_gnext */
typedef struct dstat 
{
    char    fname[9];           /* Null terminated file and extension */
	char    fext[4];
#if (VFAT)
    byte    lfname[FILENAMESIZE];   /* Long file name fro vfat. */
#endif
    char    filename[15];       /* Null terminated file.ext */
    byte    fattribute;         /* File attributes */
    word    ftime;              /* time & date lastmodified. See date */
    word    fdate;              /* and time handlers for getting info */
    dword   fsize;              /* File size */
    /* INTERNAL */
    int     driveno;
    byte    pname[9];           /* Pattern. */
    byte    pext[4];
    char    path[EMAXPATH];
    void   *pobj;               /* Info for getting at the inode */
    void   *pmom;               /* Info for getting at parent inode */
} DSTAT;


/* File access flags for flag of pc_open */
#define PO_RDONLY 0x0000    /* Open for read only*/
#define PO_WRONLY 0x0001    /* Open for write only*/
#define PO_RDWR   0x0002    /* Read/write access allowed.*/
#define PO_APPEND 0x0008    /* Seek to eof on each write*/
#define PO_CREATE 0x0100    /* Create the file if it does not exist.*/
#define PO_CREAT  0x0100    /* Create the file if it does not exist.*/
#define PO_TRUNC  0x0200    /* Truncate the file if it already exists*/
#define PO_EXCL   0x0400    /* Fail if creating and already exists*/
                                      
/* File creation permissions for mode of pc_open */
#define PS_IWRITE 0000400   /* Write permitted  */
#define PS_IREAD  0000200   /* Read permitted. (Always true anyway)*/                                      

/* Error codes for file system operations, like pc_open, pc_close, pc_read, pc_write, close, seek, tell */
/* When the APIs faile or return 'FALSE', call pc_get_errno to know the reasons. */
#define FSERR_BADFD     9   /* Invalid file descriptor*/
#define FSERR_NOENT     2   /* File not found or path to file not found*/
#define FSERR_NOFD     24   /* No file descriptors available (too many files open)*/
#define FSERR_EXIST    17   /* Exclusive access requested but file already exists.*/
#define FSERR_ACCESS   13   /* Attempt to open a read only file or a special (directory)*/
#define FSERR_INVAL    22   /* Seek to negative file pointer attempted.*/
#define FSERR_NOSPC    28   /* Write failed. Presumably because of no space*/
#define FSERR_SHARE    30   /* Open failed do to sharing */
#define FSERR_DEVICE   31   /* No Valid Disk Present */
#define FSERR_BADDIR   32   /* DELTREE -- Directory structure corrupt */

/* error code for pc_ertfs_init */
#define PCERR_BAD_FORMAT    1
#define PCERR_NO_CARD       2
#define PCERR_BAD_CARD      3
#define PCERR_CHANGED_CARD  4
#define PCERR_CARD_FAILURE  5

//=============================================================================
//============= API functions =================================================
//=============================================================================

int pc_getLibVer(void); //example: return 100 means ver 1.0.0
void pc_getLibDate(unsigned char *LibDate);

#define pc_ertfs_init pc_init
int pc_init(void);
    /*
    return: TRUE: ok
            FALSE: error
            
    pc_get_errno returns 
            1=PCERR_BAD_FORMAT (format should be FAT)
            2=PCERR_NO_CARD    (microSD is not inserted)
    */

int pc_open(char *file_name, unsigned int flag, unsigned int mode);
    /*
    file_name: file name in 8.3 fomat, 
        for example: "test.txt", "\dir1\test.txt"
    flag:
        PO_RDONLY: Read only
        PO_WRONLY: Write only
        PO_RDWR:   Read/write access allowed
        PO_APPEND: Append data to end of file.
        PO_CREAT:  Create the file if it does not exist.
        PO_TRUNC:  Truncate the file if it already exists.
    mode:
        PS_IWRITE   - Write permitted
        PS_IREAD    - Read permitted. (Always true anyway)
        
    return: 
        >=0: ok. the returned value is used as a file descriptor.
        -1: error.
    */
    
int pc_close(int fd);
    /*
    return:
        0: ok
        -1: error.
    */
    
int pc_read(int fd, unsigned char *buf, unsigned int count);
    /*
    return: 
        >=0: ok. the bytes of read.
        -1: error.
    */
    
int pc_write(int fd, unsigned char *buf, unsigned int count);
    /*
    return:
        >=0: ok. the bytes of written.
        -1: error.
    */
    
long pc_seek(int fd, long offset, int origin);
    /* Arguments to origin of pc_seek */
    #define PSEEK_SET   0   /* offset from begining of file*/
    #define PSEEK_CUR   1   /* offset from current file pointer*/
    #define PSEEK_END   2   /* offset from end of file*/
    /*
    Moves the file pointer to relative offset from the current offset.
    
    return:
        >=0: ok. the new offset.
        -1L: error.
    */
    
    
long pc_tell(int fd);
    /*
    Gets current offset of the file pointer.
    
    return:
        >=0: ok. the current offset.
        -1L: error
        
    */
int pc_eof(int fd);
    /*
    Checks for End-Of-File
    
    return: 
        1 (TRUE): ok
        0 (FALSE): error
    */

int pc_format(void);
    /*
    Formats the microSD to FAT.
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_mkdir(char *name);
    /*
    Creates a sudirectory in the path specified by name. Fails if a
    file or directory of the same name already exists or if the path
    is not found.
    
    *name: example: "subdir", "\dir1\subdir"
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_rmdir(char *name);
    /*
    Deletes the directory specified in name. Fail if name is not a directory,
    is read only or contains more than the entries . and ..
    
    *name: example: "subdir", "\dir1\subdir"
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_move(char *file_name, char *new_file_name);
    /*
    Renames the file in path (name) to newname. Fails if name is invalid,
    newname already exists or path not found.
    
    *name: example: "old.txt", "new.txt", "\dir1\old.txt", "dir1\new.txt"
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_del(char *file_name);
    /*
    Deletes the file in name. Fail if not a simple file,if it is open,
    does not exist or is read only.
    
    *name: example: "test.txt", "\dir\test.txt"
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_deltree(char *dir);
    /*
    Deletes the directory specified in name, all subdirectories of that
    directory, and all files contained therein.
    
    *dir: example: "subdir", "\dir1\subdir"
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_isdir(char *path_name);
    /*
    Checks if is a directory.
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */
    
int pc_isvol(char *name);
    /*
    Checks if is a volum.
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */

long pc_size(char *name);
    /* 
    Gets the size of the file.
    
    return:
        >=0: ok. bytes of the file.
        -1: error
    */
    
int pc_set_cwd(char *path);
int pc_get_cwd(char *path);
    /*
    Sets/Gets current working directory.
    
    *path: example: "subdir", "\dir1\subdir"
    
    return:
        1 (TRUE): ok
        0 (FLASE): erorr
    */    

long pc_get_freeSize_KB(void);
long pc_get_usedSize_KB(void);
long pc_get_totalSize_KB(void);
    /*
    return: 
        >=0: ok. the size in KB.
        
    */

//used to search multiple files. file_name can be *.exe, abc*.txt, *.*, etc.
int pc_gfirst(DSTAT *statobj, char *file_name);
int pc_gnext(DSTAT *statobj);
void pc_gdone(DSTAT *statobj);



int pc_get_attributes(char *file_name, unsigned char *p_return);
int pc_set_attributes(char *file_name, unsigned char attributes);
    /* File attributes */
    #define ARDONLY 0x1     //Read only
    #define AHIDDEN 0x2     //Hidden
    #define ASYSTEM 0x4     //System
    #define AVOLUME 0x8     //Volume
    #define ADIRENT 0x10    //Directory Entry
    #define ARCHIVE 0x20    //Rchive
    #define ANORMAL 0x00    //Normal
    #define CHICAGO_EXT 0x0f    //Chicago extended filename attribute

int pc_get_errno(void); //If other API function returns FALSE,
                        //call the function to get the error number.

#ifdef __cplusplus
}
#endif
