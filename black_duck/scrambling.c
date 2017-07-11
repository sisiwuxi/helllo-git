/*
*20120223 for mnfs pass BD test and releasable
*/
//==================include==================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "scrambling.h"

//=================global define===============


FILE *fpNow = NULL;
char Cur_Parser_File = 0;
char Macro_Before = 0;
char Is_Include = 0;
char Is_Struct = 0;
char Is_Function = 0;
char Is_Typedef = 0;
char Is_Backlash = 0;
char Is_Well = 0;
char Save_Return = 0;
char temp[MAX_STR_LEN] = {0};
char scramble_temp[MAX_STR_LEN * 4] = {0};
struct NOPRO_ENTITY temp_record = {0};

#if 1
char Parser_File[][MAX_STR_LEN] = {
    "mstar_h1.h","mstar_h2.h","mstar_h3.h","mstar_h4.h","mstar_h5.h","mstar_h6.h","mstar_h7.h","mstar_h8.h","mstar_h9.h",
    "mstar_h10.h","mstar_h11.h","mstar_h12.h","mstar_h13.h","mstar_h14.h","mstar_h15.h","mstar_h16.h","mstar_h17.h",
    "mstar_h18.h","mstar_h19.h","mstar_h20.h","mstar_h21.h","mstar_h22.h","mstar_h23.h","mstar_h24.h","mstar_h25.h",
    "mstar_h26.h","mstar_h27.h",
    "mstar_h1h.h","mstar_h10h.h","mstar_h12h.h","mstar_h14h.h","mstar_h19h.h","mstar_h21h.h","mstar_h25h.h","mstar_h27h.h",
    "mstar_c1h.h","mstar_c2h.h","mstar_c3h.h","mstar_c4h.h","mstar_c5h.h","mstar_c6h.h","mstar_c7h.h","mstar_c8h.h",
    "mstar_c9h.h","mstar_c10h.h","mstar_c11h.h","mstar_c12h.h","mstar_c13h.h","mstar_c14h.h","mstar_c15h.h","mstar_c16h.h",
    "mstar_c17h.h","mstar_c18h.h","mstar_c19h.h","mstar_c20h.h","mstar_c21h.h","mstar_c22h.h","mstar_c23h.h",
    "mstar_c1.c","mstar_c2.c","mstar_c3.c","mstar_c4.c","mstar_c5.c","mstar_c6.c","mstar_c7.c","mstar_c8.c",
    "mstar_c9.c","mstar_c10.c","mstar_c11.c","mstar_c12.c","mstar_c13.c","mstar_c14.c","mstar_c15.c","mstar_c16.c",
    "mstar_c17.c","mstar_c18.c","mstar_c19.c","mstar_c20.c","mstar_c21.c","mstar_c22.c","mstar_c23.c",
};
#else
char Parser_File[][MAX_STR_LEN] = {
    "aops.h","attrib.h","attrlist.h","bitmap.h","bootsect.h","collate.h","compat.h","compress.h","config.h","debug.h",
    "dir.h","endians.h","index.h","inode.h","layout.h","lcnalloc.h","logfile.h","mft.h","misc.h","mst.h",
    "ntfstime.h","runlist.h","security.h","support.h","types.h","unistr.h","volume.h","aops.c","attrib.c","attrlist.c",
    "bitmap.c","bootsect.c","collate.c","compat.c","compress.c","debug.c","dir.c","file.c","index.c","inode.c",
    "lcnalloc.c","logfile.c","mft.c","mst.c","namei.c","runlist.c","security.c","super.c","unistr.c","volume.c",
};
#endif


char Symbol_Of_String_End[][1] = {
    0x5B,0x5D,0x28,0x29,0x2C,0x3B,0x3D,0x21,0x22,0x2B,0x2D,0x7E,0x26,0x27,0x7c,0x2A,0x3E,0x3C,0x3F,0x2F,0x0D,0x0A,0x20,0x2E,0x00,0x7B,0x7D,0x09,0x3A,0x25,
//      [       ]       (       )       ,      ;       =      !      ''       +      -       ~      &     '        |      *       >      <     ?       /   return return space   .              {       }              :      %
};

char Keywords_List[][MAX_KEYWORDS_LEN] = {
    "void", "char", "int", "float", "double",
    "short", "long", "signed", "unsigned",
    "struct", "union", "enum", "typedef", "sizeof",
    "auto", "static", "register", "extern", "const", "volatile",
    "return", "continue", "break", "goto",
    "if", "else", "switch", "case", "default",
    "for", "do", "while",
    "_cs", "_ds", "_es", "_ss", "cdecl", "far", "near", "huge", "interrupt", "pascal",
    "inline", "__inline__",
    "u8", "u16", "u32", "u64", "s8", "s16", "s32", "s64",
    "__force", "time_t", "mode_t", "loff_t", "sector_t", "gfp_t", "__init", "__exit",
    "va_list", "wait_queue_head_t", "bool", "pgoff_t", "dev_t",
    "defined", "endif", "ifndef", "define", "data1_len", "data2_len", "filldir_t", "kiocb", "iovec", "size_t",
};

char DWell_List[][MAX_KEYWORDS_LEN] = {
    "NA_##", "NAttr##", "NAttrSet##", "NAttrClear##", "NI_##", "AttrList##", "NI_EXT_##", "magic_##", "NV_##",
    "magic_FILE", "magic_INDX", "magic_HOLE", "magic_RSTR", "magic_RCRD", "magic_CHKD", "magic_BAAD", "magic_empty",
    "NA_Initialized", "NA_NonResident", "NI_Dirty", "NI_AttrList", "NI_AttrListDirty", "NI_FileNameDirty",
    "NV_ReadOnly", "NV_CaseSensitive", "NV_LogFileEmpty", "AttrList",
    "NI_EXT_NTFS_ATTR_NI", "NI_EXT_NTFS_MST_PROTECTED","Dirty", "FileNameDirty",
    "NTFS_ATTR_NI", "NTFS_MST_PROTECTED", "ENAMETOOLONG", "_IOR", "__u32", "DJGPP", "S_IFMT", "FILE", "INDX", "HOLE", "RSTR",
    "RCRD", "CHKD", "BAAD", "empty", "ptrdiff_t", "ReadOnly", "CaseSensitive", "LogFileEmpty",
    "GFP_NOFS", "__GFP_HIGHMEM", "__GFP_NOFAIL", "PAGE_SHIFT", "num_physpages", "PAGE_KERNEL",
    "PAGE_CACHE_SHIFT", "KM_BIO_SRC_IRQ", "BH_Uptodate_Lock", "PAGE_CACHE_SIZE", "KM_BIO_SRC_IRQ", "AS_EIO",
    "BIO_UPTODATE", "PF_MEMALLOC", "READ", "WRITE", "GFP_KERNEL", "MAX_BUF_PER_PAGE",
    "NULL", "__attribute__", "__packed__", "__transparent_union__", "Initialized", "NonResident",
    "KM_USER0", "KERN_WARNING", "BH_Eopnotsupp", "WRITE_BARRIER", "GFP_NOIO",
    "all_buffer_finished", "jiffies", "HZ", "GFP_ATOMIC","WB_SYNC_NONE", "b_this_page","BH_Uptodate", "BH_Dirty",
    "bh_end_io_t", "TASK_UNINTERRUPTIBLE", "PAGE_CACHE_MASK", "pgoff_t", "writeback_index", "LLONG_MAX", "PAGECACHE_TAG_DIRTY",
    "PAGEVEC_SIZE", "CURRENT_TIME_SEC", "__func__", "__FILE__", "__LINE__", "__FUNCTION__", "__u32", "__user", "EFAULT",
    "KERN_INFO", "NTFS3G_VERSION", "KERN_CRIT",
    "EINVAL", "EIO", "EPERM", "EOPNOTSUPP", "EBUSY", "ENXIO", "EACCES", "ENOENT", "ENOMEM",
    "LINUX_VERSION_CODE", "Compressed", "Encrypted", "Sparse", "__BIG_ENDIAN__", "__cplusplus", "DEBUG",
    "asm", "current", "flags", "bi_private", "bi_sector", "bi_end_io", "false", "true", "lru", "BIO_EOPNOTSUPP", "bv_page",
    "bv_len", "bv_offset", "b_page", "i_mode", "AOP_WRITEPAGE_ACTIVATE", "ENOSPC", "readpage", "readpages",
    "sync_page", "writepage", "writepages", "migratepage", "write_begin", "write_end", "set_page_dirty",
    "PAGE_SIZE", "EINTR", "ERANGE", "EEXIST", "unused", "EOVERFLOW", "end_buffer_read_sync", "KERN_ERR",
    "DT_REG", "DT_DIR", "ENOTDIR", "S_IFREG", "S_IFDIR", "S_IFIFO", "S_IFSOCK", "S_IFCHR", "S_IFBLK", "S_IFLNK",
    "ENOTEMPTY", "llseek", "read", "readdir", "fsync", "release", "ioctl", "S_IRWXUGO",
    "buffer_migrate_page", "generic_file_llseek", "generic_read_dir", "MAX_LFS_FILESIZE", "ATTR_UID", "ATTR_GID",
    "ATTR_MODE", "ATTR_SIZE", "ATTR_MTIME", "ATTR_CTIME", "ATTR_ATIME", "aio_read", "write", "aio_write",
    "mmap", "open", "splice_read", "do_sync_read", "generic_file_aio_read", "do_sync_write", "generic_file_mmap",
    "generic_file_splice_read", "truncate", "setattr", "getattr", "ssize_t", "I_NEW", "I_DIRTY_SYNC", "I_DIRTY_DATASYNC",
    "ESTALE", "get_parent", "fh_to_dentry", "fh_to_parent", "create", "lookup", "unlink", "mkdir", "rmdir", "rename",
    "EMSGSIZE", "ENODEV", "alloc_inode", "destroy_inode", "write_inode", "put_super", "statfs", "clear_inode",
    "show_options", "owner", "name", "get_sb", "kill_sb", "fs_flags", "SLAB_HWCACHE_ALIGN", "SLAB_RECLAIM_ACCOUNT",
    "SLAB_MEM_SPREAD", "end_buffer_write_sync", "THIS_MODULE", "kill_block_super", "FS_REQUIRES_DEV", "ntfs_pwrite",
    "PATH_MAX", "EILSEQ",
};

char Macro_Definition_List[][MAX_KEYWORDS_LEN] = {
    "#ifndef", "#define", "#include", "#endif", "#if", "#undef", "#ifdef", "#else",
};


SCRAMBLE_STR_TYPE Str_Type = SCRAMBLE_STR_TYPE_NULL;

//==================================================


//==================================================
char Is_Keywords(char *string)
{
    char i = 0;
    char b_Ret = FALSE;

    for(i = 0; i < (sizeof(Keywords_List) / MAX_KEYWORDS_LEN); i++)
    {
        if(!strcmp(string, Keywords_List[i]))
        {
            b_Ret = TRUE;
            break;
        }
    }
    return b_Ret;
}

//==================================================
char Is_DWwell(char *string)
{
    int i = 0;
    char b_Ret = FALSE;

    for(i = 0; i < (sizeof(DWell_List) / MAX_KEYWORDS_LEN); i++)
    {
        if(!strcmp(string, DWell_List[i]))
        {
            b_Ret = TRUE;
            break;
        }
    }
    return b_Ret;
}
//==================================================
char Is_MacroDefinition(char *string)
{
    char i = 0;
    char b_Ret = FALSE;

    for(i = 0; i < (sizeof(Macro_Definition_List) / MAX_KEYWORDS_LEN); i++)
    {
        if(!strcmp(string, Macro_Definition_List[i]))
        {
            b_Ret = TRUE;
            break;
        }
    }
    return b_Ret;
}


//take out the string one by one
void Scramble_Search_String(char *string, FILE *fp1)
{
    char i = 0;
    char str_len = 0;
    char FindOutSuspend = FALSE;

    while(!feof(fp1))
    {
        char one_c = fgetc(fp1);
        long offset = ftell(fp1);

        MAIN_DBG(("%x--", one_c));
        #if 1
        FindOutSuspend = FALSE;
        Str_Type = SCRAMBLE_STR_TYPE_NULL;
        for(i = 0; i < (sizeof(Symbol_Of_String_End) / 1); i++)
        {
            if(one_c == Symbol_Of_String_End[i][0])
            {
                FindOutSuspend = TRUE;
                Str_Type = SCRAMBLE_STR_TYPE_SUSPEND;
                break;
            }
            else
            {
                FindOutSuspend = FALSE;
            }
        }

        MAIN_DBG(("%d ", FindOutSuspend));
        if(FindOutSuspend)
        {
            MAIN_DBG(("%d ", str_len));
            if(0 == str_len)
            {
                if(0x2F == one_c)//"/"
                {
                    char next_c = fgetc(fp1);
                    if((0x2F == next_c) || (0x2A == next_c))//"/" or "*"
                    {
                        if(0x2F == next_c)
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_COMMENT_SINGLE;
                        }
                        else if(0x2A == next_c)
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_COMMENT_MULTI;
                        }
                        string[str_len] = one_c;
                        str_len++;
                        string[str_len] = next_c;
                        str_len++;
                        string[str_len] = 0;
                        return;
                    }
                    else
                    {
                        fseek(fp1, offset, SEEK_SET);
                        string[str_len] = one_c;
                        str_len++;
                        string[str_len] = 0;
                        MAIN_DBG(("[2]%s %d\n", string, Str_Type));
                        return;
                    }
                }
                else
                {
                    if(0x20 == one_c)
                    {
                        Str_Type = SCRAMBLE_STR_TYPE_SPACE;
                    }
                    else if(0x0A == one_c)
                    {
                        Str_Type = SCRAMBLE_STR_TYPE_RETURN;
                    }
                    else if(0x09 == one_c)
                    {
                        Str_Type = SCRAMBLE_STR_TYPE_TAB;
                    }
                    else if(0x22 == one_c)
                    {
                        if(!Is_Include)
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_STRING;
                        }
                    }
                    string[str_len] = one_c;
                    str_len++;
                    string[str_len] = 0;
                    MAIN_DBG(("[3]%s %d\n", string, Str_Type));
                    return;
                }
            }
            else
            {
                string[str_len] = 0;
                if((string[0] >= 0x30) && (string[0] <= 0x39))//0~9
                {
                    Str_Type = SCRAMBLE_STR_TYPE_DIGIT;
                }
                else
                {
                    if(0x28 == one_c)//"("
                    {
                        if(Is_Keywords(string))
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_KEYWORDS;
                        }
                        else if(Is_DWwell(string))
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_DWELL;
                        }
                        else if(Is_MacroDefinition(string))
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_MACRODEF;
                        }
                        else
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_FUNCTION;
                        }
                    }
                    else
                    {
                        if(Is_Keywords(string))
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_KEYWORDS;
                        }
                        else if(Is_DWwell(string))
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_DWELL;
                        }
                        else if(Is_MacroDefinition(string))
                        {
                            Str_Type = SCRAMBLE_STR_TYPE_MACRODEF;
                        }
                        else
                        {
                            if(0x20 == one_c)
                            {
                                long Cur_Offset = 0;
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x28)//"("
                                {
                                    Str_Type = SCRAMBLE_STR_TYPE_FUNCTION;
                                }
                                else
                                {
                                    Str_Type = SCRAMBLE_STR_TYPE_VARIABLE;
                                }
                                fseek(fp1, Cur_Offset, SEEK_SET);
                            }
                            else
                            {
                                Str_Type = SCRAMBLE_STR_TYPE_VARIABLE;
                            }
                        }
                    }
                }
                fseek(fp1, offset - 1, SEEK_SET);
                MAIN_DBG(("[4]%s %d\n", string, Str_Type));
                return;
            }
        }
        else
        {
            MAIN_DBG(("%d ", str_len));
            string[str_len] = one_c;
            str_len++;
            MAIN_DBG(("[5]%s \n", string));
            if(str_len >=2 && '#' == string[str_len-1] && '#' == string[str_len-2])
            {
                string[str_len] = 0;
                Str_Type = SCRAMBLE_STR_TYPE_DWELL;
                return;
            }
        }
        #endif
    }
    return;
}

//strcpy if string != NULL
void Scramble_Strcpy(char *dst, char *scr)
{
    if(scr != NULL)
    {
        strcpy(dst, scr);
    }
    return;
}

//Create Rand Character
char GetRandCharacter(char i)
{
    int temp = 0;
    char element = 0;

    temp = rand();
    element = temp % NUM_RANDOM_GROUP;
    if((element >= 0) && (element <= 9))
    {
        if(i != 0)
        {
            return (element + 0x30);//0--9
        }
        else
        {
            return (element + 0x41);//A-Z
        }
    }
    else if((element >= 10) && (element <= 35))
    {
        return ((element - 10) + 0x41);//A-Z
    }
    else if((element >= 37) && (element <= 62))
    {
        return ((element - 37) + 0x61);//a--z
    }
    return 0x41;
}

//Generate 10 bytes and copy to scramble_temp
void Generate_Random_String(char *string)
{
    char i = 0;
    memset(scramble_temp, 0x0, sizeof(char) * MAX_STR_LEN * 4);
    for(i = 0; i < RANDOM_STR_LEN; i++)
    {
        //using random digital or character
        scramble_temp[i] = GetRandCharacter(i);
    }
    scramble_temp[i] = 0;
    return;
}
//write the new replace relation to the end of replace.txt
void Write_Str_ToNewFile(char *string, FILE *fp)
{
    int i = 0;
    int len = strlen(string);

    if(string != NULL)
    {
        if(SCRAMBLE_STR_TYPE_MACRODEF == Str_Type && !Save_Return)
        {
            fputc(0x0A, fp);
        }
        if(Is_Typedef && !Save_Return)
        {
            Is_Typedef = 0;
            fputc(0x0A, fp);
        }
        for(i = 0; i < len; i++)
        {
            if(string[i] != -1)
            {
                fputc(string[i], fp);
            }
        }
    }
    return;
}


//Is in the struct replace record
char IsIn_ReplaceStruct_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

    if((fp1 = fopen("1replace_struct.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 1replace_struct.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);
            Scramble_Search_String(temp, fp1);
            if(temp[0] != ' ')
            {
                break;
            }

            fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
            for(i = 0; i < RANDOM_STR_LEN; i++)
            {
                scramble_temp[i] = fgetc(fp1);
            }
            scramble_temp[i] = 0x20;
            scramble_temp[i + 1] = 0;
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}

//Is in the enum replace record
char IsIn_ReplaceEnum_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

    if((fp1 = fopen("6replace_enum.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 6replace_enum.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);
            Scramble_Search_String(temp, fp1);
            if(temp[0] != ' ')
            {
                break;
            }

            fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
            for(i = 0; i < RANDOM_STR_LEN; i++)
            {
                scramble_temp[i] = fgetc(fp1);
            }
            scramble_temp[i] = 0x20;
            scramble_temp[i + 1] = 0;
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}


//Is in the union replace record
char IsIn_ReplaceUnion_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

    if((fp1 = fopen("7replace_union.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 7replace_union.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);
            Scramble_Search_String(temp, fp1);
            if(temp[0] != ' ')
            {
                break;
            }

            fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
            for(i = 0; i < RANDOM_STR_LEN; i++)
            {
                scramble_temp[i] = fgetc(fp1);
            }
            scramble_temp[i] = 0x20;
            scramble_temp[i + 1] = 0;
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}

//Is in the others replace record
char IsIn_ReplaceOthers_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

    if((fp1 = fopen("2replace_others.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 2replace_others.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);

            Scramble_Search_String(temp, fp1);

            if(0x20 == temp[0])
            {
                fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
                memset(scramble_temp, 0x0, sizeof(char) * MAX_STR_LEN * 4);
                for(i = 0; i < RANDOM_STR_LEN; i++)
                {
                    scramble_temp[i] = fgetc(fp1);
                }
                scramble_temp[i] = 0x20;
                scramble_temp[i + 1] = 0;

                b_Ret = TRUE;
                break;
            }
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}

//Is in the #define replace record
char IsIn_ReplaceDefine_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

    if((fp1 = fopen("8replace_define.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 8replace_define.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);

            Scramble_Search_String(temp, fp1);

            if(0x20 == temp[0])
            {
                fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
                memset(scramble_temp, 0x0, sizeof(char) * MAX_STR_LEN * 4);
                for(i = 0; i < RANDOM_STR_LEN; i++)
                {
                    scramble_temp[i] = fgetc(fp1);
                }
                scramble_temp[i] = 0;

                b_Ret = TRUE;
                break;
            }
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}

//record the symbol replace process
char IsIn_ReplaceFunc_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

	if((fp1 = fopen("4replace_func.txt","rb")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 4replace_func.txt", __LINE__));
		return b_Ret;
	}

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);
            Scramble_Search_String(temp, fp1);
            if(temp[0] != ' ')
            {
                break;
            }

            fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
            for(i = 0; i < RANDOM_STR_LEN; i++)
            {
                scramble_temp[i] = fgetc(fp1);
            }
            scramble_temp[i] = 0;
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}


//record the no prototype function name
char IsIn_NoPrototypeFunc_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char ReplaceName[MAX_STR_LEN] = {0};

    if((fp1 = fopen("5no_prototype_func.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 5no_prototype_func.txt", __LINE__));
        return b_Ret;
    }
    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}



//record the Digit replace process
char IsIn_ReplaceDigit_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

	if((fp1 = fopen("9replace_digit.txt","rb")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 9replace_digit.txt", __LINE__));
		return b_Ret;
	}

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);
            Scramble_Search_String(temp, fp1);
            if(temp[0] != ' ')
            {
                break;
            }
            fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
            while(!feof(fp1))
            {
                scramble_temp[i] = fgetc(fp1);
                if((0x0A == scramble_temp[i]) || (0x0D == scramble_temp[i]))
                {
                    scramble_temp[i] = 0;
                    break;
                }
                i++;
            }
            scramble_temp[i] = 0;
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}




//record the symbol replace process
char IsIn_NoP_Struct_InS_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char ReplaceName[MAX_STR_LEN] = {0};

    if((fp1 = fopen("nop_struct_in_struct.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open nop_struct_in_struct.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(0 == strcmp(string, temp))
        {
            strcpy(scramble_temp, string);
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}


//record the symbol replace process
char IsIn_ReplaceVar_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char temp[MAX_STR_LEN] = {0};

	if((fp1 = fopen("10replace_var.txt","rb")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 10replace_var.txt", __LINE__));
		return b_Ret;
	}

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(!strcmp(string, temp))
        {
            char i = 0;
            long offset = ftell(fp1);
            Scramble_Search_String(temp, fp1);
            if(temp[0] != ' ')
            {
                break;
            }

            fseek(fp1, offset + (sizeof(MID_REPLACE) - 1), SEEK_SET);
            for(i = 0; i < RANDOM_STR_LEN; i++)
            {
                scramble_temp[i] = fgetc(fp1);
            }
            scramble_temp[i] = 0;
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}

// replace function_string by random_string
char *Scramble_Function_String(char *string)
{
    if(string == NULL)
    {
        return NULL;
    }
    Is_Function = 1;

    //in 4replace_func.txt
    if(IsIn_ReplaceFunc_Record(string))
    {
        if(temp_record.Temp_Record_Status == 0)
        {
            temp_record.Temp_Record_Status = 1;
            temp_record.Left_Parenthesis = 0;
            temp_record.Right_Parenthesis = 0;
        }
        return scramble_temp;
    }
    if(IsIn_ReplaceDefine_Record(string))
    {
        return scramble_temp;
    }
    else
    {
        strcpy(scramble_temp, string);
        return scramble_temp;
    }
    return scramble_temp;
}

//record the temp no prototype parameters
void Clear_Temp_Record(void)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("11temp_noproto_entity.txt","wb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 11temp_noproto_entity.txt", __LINE__));
        return;
    }

    fclose(fp1);
    return;
}

//record the symbol replace process
char IsIn_Temp_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char ReplaceName[MAX_STR_LEN] = {0};

    if((fp1 = fopen("11temp_noproto_entity.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 11temp_noproto_entity.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(0 == strcmp(string, temp))
        {
            strcpy(scramble_temp, string);
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}


//record the temp no prototype parameters
void Set_Temp_Record(char *str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("11temp_noproto_entity.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 11temp_noproto_entity.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}

//record the no prototype struct name
char IsIn_NoPrototypeStruct_Record(char *string)
{
    FILE *fp1 = NULL;
    char b_Ret = FALSE;
    char ReplaceName[MAX_STR_LEN] = {0};

    if((fp1 = fopen("3no_prototype_struct.txt","rb")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 3no_prototype_struct.txt", __LINE__));
        return b_Ret;
    }

    while(!feof(fp1))
    {
        Scramble_Search_String(temp, fp1);
        if(0 == strcmp(string, temp))
        {
            b_Ret = TRUE;
            break;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type == SCRAMBLE_STR_TYPE_RETURN)
                {
                    break;
                }
            }
        }
    }
    fclose(fp1);
    return b_Ret;
}

//record the symbol replace process
void Set_ReplaceVar_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
	if((fp1 = fopen("10replace_var.txt","a+")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 10replace_var.txt", __LINE__));
		return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}

//record the no prototype struct name
void Set_NoPrototypeStruct_Record(char *string)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("3no_prototype_struct.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 3no_prototype_struct.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, string);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}


//record the struct replace process
void Set_ReplaceStruct_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("1replace_struct.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 1replace_struct.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}

//record the symbol replace process
void Set_ReplaceFunc_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
	if((fp1 = fopen("4replace_func.txt","a+")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 4replace_func.txt", __LINE__));
		return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}



//record the No Prototype function
void Set_NoPrototypeFunc_Record(char *string)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
	if((fp1 = fopen("5no_prototype_func.txt","a+")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 5no_prototype_func.txt", __LINE__));
		return;
    }
    strcpy(ReplaceName, string);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}

//record the enum replace process
void Set_ReplaceEnum_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("6replace_enum.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 6replace_enum.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}

//record the union replace process
void Set_ReplaceUnion_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("7replace_union.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 7replace_union.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}

//record the other typedef replace process
void Set_ReplaceOthers_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("2replace_others.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 2replace_others.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}



//record the #define replace process
void Set_ReplaceDefine_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
    if((fp1 = fopen("8replace_define.txt","a+")) == NULL)
    {
        MAIN_ERR(("\n[%d] error fp1 open 8replace_define.txt", __LINE__));
        return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}


//record the Digit replace process
void Set_ReplaceDigit_Record(char *old_str, char *new_str)
{
    FILE *fp1 = NULL;
    char ReplaceName[MAX_STR_LEN] = {0};
	if((fp1 = fopen("9replace_digit.txt","a+")) == NULL)
	{
		MAIN_ERR(("\n[%d] error fp1 open 9replace_digit.txt", __LINE__));
		return;
    }
    strcpy(ReplaceName, old_str);
    strcat(ReplaceName, MID_REPLACE);
    strcat(ReplaceName, new_str);
    ReplaceName[strlen(ReplaceName)] = 0x0A;
    Write_Str_ToNewFile(ReplaceName, fp1);
    fclose(fp1);
    return;
}




//===================================
void Collect_NoPrototype_Struct(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char StructName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    char left_parenthesis = 0;
    char right_parenthesis = 0;
    long Cur_Offset = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type != SCRAMBLE_STR_TYPE_KEYWORDS)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    if(Is_Typedef)
                    {
                        if(Str_Type != SCRAMBLE_STR_TYPE_SPACE && Str_Type != SCRAMBLE_STR_TYPE_TAB)
                        {
                            Is_Typedef = 0;
                        }
                    }
                    continue;
                }
                else
                {
                    Cur_Offset = ftell(fp1);
                }
                if(!strcmp("struct", temp))
                {
                    memset(StructName, 0x0, MAX_STR_LEN * sizeof(char));
                    while(!feof(fp1))
                    {
                        Scramble_Search_String(temp, fp1);
                        if(Str_Type == SCRAMBLE_STR_TYPE_VARIABLE)
                        {
                            strcpy(StructName, temp);
                            //printf("[%s]", StructName);
                            if(IsIn_ReplaceStruct_Record(StructName) || IsIn_ReplaceOthers_Record(StructName))
                            {
                                break;
                            }
                            else if(!IsIn_NoPrototypeStruct_Record(StructName))
                            {
                                b_Ret = TRUE;
                                Set_NoPrototypeStruct_Record(StructName);
                            }
                            break;
                        }
                        else if((SCRAMBLE_STR_TYPE_TAB == Str_Type) ||
                                (SCRAMBLE_STR_TYPE_SPACE == Str_Type))
                        {
                            continue;
                        }
                        else
                        {
                            b_Ret = FALSE;
                            break;
                        }
                    }
                }
                if(!b_Ret)
                {
                    //fseek(fp1, Cur_Offset, SEEK_SET);
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_Prototype_Struct(void)
{
	FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char StructName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    char left_parenthesis = 0;
    char right_parenthesis = 0;
    char struct_first = 0;
    long Cur_Offset = 0;
    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
    printf("[%d]",i);
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type != SCRAMBLE_STR_TYPE_KEYWORDS)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    if(Is_Typedef)
                    {
                        if(Str_Type != SCRAMBLE_STR_TYPE_SPACE && Str_Type != SCRAMBLE_STR_TYPE_TAB)
                        {
                            Is_Typedef = 0;
                        }
                    }
                    continue;
                }
                else
                {
                    Cur_Offset = ftell(fp1);
                }
                if(!strcmp("typedef", temp))
                {
                    Is_Typedef = 1;
                    struct_first = 0;
                    continue;
                }
                if(!strcmp("struct", temp))
                {
                    if(Is_Typedef)//typedef struct
                    {
                        Is_Typedef = 0;
                        if(struct_first)
                        {
                            continue;
                        }
                        while(!feof(fp1))
                        {
                            struct_char = fgetc(fp1);
                            if('{' == struct_char)
                            {
                                left_parenthesis++;
                                b_Ret = TRUE;
                                break;
                            }
                            else if((0x0D == struct_char) || (0x0A == struct_char) || (0x20 == struct_char) || (0x09 == struct_char))
                            {
                                continue;
                            }
                            else
                            {
                                b_Ret = FALSE;
                                break;
                            }
                        }
                        if(b_Ret)
                        {
                            while(!feof(fp1))
                            {
                                struct_char = fgetc(fp1);
                                if('{' == struct_char)
                                {
                                    left_parenthesis++;
                                }
                                if('}' == struct_char)
                                {
                                    right_parenthesis++;
                                }
                                if(left_parenthesis == right_parenthesis)
                                {
                                    break;
                                }
                            }
                            while(!feof(fp1))
                            {
                                Scramble_Search_String(temp, fp1);
                                if(SCRAMBLE_STR_TYPE_SUSPEND == Str_Type && ';' ==temp[0])
                                {
                                    b_Ret = FALSE;
                                    break;
                                }
                                if(SCRAMBLE_STR_TYPE_VARIABLE == Str_Type)
                                {
                                    b_Ret = TRUE;
                                    strcpy(StructName, temp);
                                    if(!IsIn_ReplaceStruct_Record(StructName))
                                    {
                                        Generate_Random_String(scramble_temp);
                                        Set_ReplaceStruct_Record(StructName, scramble_temp);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        memset(StructName, 0x0, MAX_STR_LEN * sizeof(char));
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if(Str_Type == SCRAMBLE_STR_TYPE_VARIABLE)
                            {
                                if(strlen(StructName))
                                {
                                    b_Ret = FALSE;
                                    memset(StructName, 0x0, MAX_STR_LEN * sizeof(char));
                                    break;
                                }
                                else
                                {
                                    strcpy(StructName, temp);
                                }
                            }
                            else if(Str_Type == SCRAMBLE_STR_TYPE_SUSPEND)
                            {
                                if(('{' ==temp[0]) && strlen(StructName))
                                {
                                    if(!IsIn_ReplaceStruct_Record(StructName))
                                    {
                                        Generate_Random_String(scramble_temp);
                                        Set_ReplaceStruct_Record(StructName, scramble_temp);
                                    }
                                    memset(StructName, 0x0, MAX_STR_LEN * sizeof(char));
                                    b_Ret = TRUE;
                                    break;
                                }
                            }
                            else if((SCRAMBLE_STR_TYPE_TAB == Str_Type) ||
                                    (SCRAMBLE_STR_TYPE_SPACE == Str_Type) ||
                                    (SCRAMBLE_STR_TYPE_RETURN == Str_Type))
                            {
                                continue;
                            }
                            else
                            {
                                b_Ret = FALSE;
                                memset(StructName, 0x0, MAX_STR_LEN * sizeof(char));
                                break;
                            }
                        }
                    }
                }
                else
                {
                    struct_first++;
                }
                if(!b_Ret)
                {
                    //fseek(fp1, Cur_Offset, SEEK_SET);
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_Prototype_Enum(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char EnumName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    char left_parenthesis = 0;
    char right_parenthesis = 0;
    long Cur_Offset = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                //printf("[%s]", temp);
                if(Str_Type != SCRAMBLE_STR_TYPE_KEYWORDS)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                if(!strcmp("typedef", temp))
                {
                    Is_Typedef = 1;
                    continue;
                }

                if(!strcmp("enum", temp))
                {
                    Cur_Offset = ftell(fp1);
                    if(Is_Typedef)//typedef struct
                    {
                        Is_Typedef = 0;
                        while(!feof(fp1))
                        {
                            struct_char = fgetc(fp1);
                            if('{' == struct_char)
                            {
                                left_parenthesis++;
                                b_Ret = TRUE;
                                break;
                            }
                            else if((0x0D == struct_char) || (0x0A == struct_char) || (0x20 == struct_char) || (0x09 == struct_char))
                            {
                                continue;
                            }
                            else
                            {
                                b_Ret = FALSE;
                                break;
                            }
                        }
                        if(b_Ret)
                        {
                            while(!feof(fp1))
                            {
                                struct_char = fgetc(fp1);
                                if('{' == struct_char)
                                {
                                    left_parenthesis++;
                                }
                                if('}' == struct_char)
                                {
                                    right_parenthesis++;
                                }
                                if(left_parenthesis == right_parenthesis)
                                {
                                    break;
                                }
                            }
                            while(!feof(fp1))
                            {
                                Scramble_Search_String(temp, fp1);
                                if(SCRAMBLE_STR_TYPE_SUSPEND == Str_Type && ';' ==temp[0])
                                {
                                    b_Ret = FALSE;
                                    break;
                                }
                                if(SCRAMBLE_STR_TYPE_VARIABLE == Str_Type)
                                {
                                    b_Ret = TRUE;
                                    strcpy(EnumName, temp);
                                    if(!IsIn_ReplaceEnum_Record(EnumName))
                                    {
                                        Generate_Random_String(scramble_temp);
                                        Set_ReplaceEnum_Record(EnumName, scramble_temp);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        b_Ret = FALSE;
                        break;
                    }
                }
                if(!b_Ret)
                {
                    //fseek(fp1, Cur_Offset, SEEK_SET);
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_Prototype_Union(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char UnionName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    char left_parenthesis = 0;
    char right_parenthesis = 0;
    char union_first = 0;
    long Cur_Offset = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                //printf("[%s]", temp);
                if(Str_Type != SCRAMBLE_STR_TYPE_KEYWORDS)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                if(!strcmp("typedef", temp))
                {
                    Is_Typedef = 1;
                    union_first = 0;
                    continue;
                }

                if(!strcmp("union", temp))
                {
                    Cur_Offset = ftell(fp1);
                    if(Is_Typedef)//typedef struct
                    {
                        Is_Typedef = 0;
                        if(union_first)
                        {
                            continue;
                        }
                        while(!feof(fp1))
                        {
                            struct_char = fgetc(fp1);
                            if('{' == struct_char)
                            {
                                left_parenthesis++;
                                b_Ret = TRUE;
                                break;
                            }
                            else if((0x0D == struct_char) || (0x0A == struct_char) || (0x20 == struct_char) || (0x09 == struct_char))
                            {
                                continue;
                            }
                            else
                            {
                                b_Ret = FALSE;
                                break;
                            }
                        }
                        if(b_Ret)
                        {
                            while(!feof(fp1))
                            {
                                struct_char = fgetc(fp1);
                                if('{' == struct_char)
                                {
                                    left_parenthesis++;
                                }
                                if('}' == struct_char)
                                {
                                    right_parenthesis++;
                                }
                                if(left_parenthesis == right_parenthesis)
                                {
                                    break;
                                }
                            }
                            while(!feof(fp1))
                            {
                                Scramble_Search_String(temp, fp1);
                                if(SCRAMBLE_STR_TYPE_SUSPEND == Str_Type && ';' ==temp[0])
                                {
                                    b_Ret = FALSE;
                                    break;
                                }
                                if(SCRAMBLE_STR_TYPE_VARIABLE == Str_Type)
                                {
                                    b_Ret = TRUE;
                                    strcpy(UnionName, temp);
                                    if(!IsIn_ReplaceUnion_Record(UnionName))
                                    {
                                        Generate_Random_String(scramble_temp);
                                        Set_ReplaceUnion_Record(UnionName, scramble_temp);
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        b_Ret = FALSE;
                        break;
                    }
                }
                else
                {
                    union_first++;
                }
                if(!b_Ret)
                {
                    //fseek(fp1, Cur_Offset, SEEK_SET);
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_Prototype_OtherTypedef(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char OtherName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char Is_struct = FALSE;
    char Is_left_parenthesis = FALSE;
    char struct_char = 0;
    long Cur_Offset = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                //printf("[%s]", temp);
                if(Str_Type != SCRAMBLE_STR_TYPE_KEYWORDS)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                if(!strcmp("typedef", temp))
                {
                    Is_struct = FALSE;
                    Is_left_parenthesis = FALSE;
                    memset(OtherName, 0x0, MAX_STR_LEN * sizeof(char));
                    while(!feof(fp1))
                    {
                        Scramble_Search_String(temp, fp1);
                        if((SCRAMBLE_STR_TYPE_SPACE == Str_Type) || (SCRAMBLE_STR_TYPE_TAB == Str_Type) ||
                            (SCRAMBLE_STR_TYPE_SUSPEND == Str_Type))
                        {
                            if(!strcmp("{", temp) || !strcmp(";", temp))
                            {
                                break;
                            }
                            else if(!strcmp("(", temp))
                            {
                                Is_left_parenthesis = TRUE;
                                struct_char = fgetc(fp1);
                                if('*' == struct_char)
                                {
                                    Scramble_Search_String(temp, fp1);
                                    if(SCRAMBLE_STR_TYPE_VARIABLE == Str_Type)
                                    {
                                        b_Ret = TRUE;
                                        strcpy(OtherName, temp);
                                        if(!IsIn_ReplaceOthers_Record(OtherName))
                                        {
                                            Generate_Random_String(scramble_temp);
                                            Set_ReplaceOthers_Record(OtherName, scramble_temp);
                                        }
                                        break;
                                    }
                                }
                            }
                            continue;
                        }
                        else if(SCRAMBLE_STR_TYPE_KEYWORDS == Str_Type)
                        {
                            if(!strcmp("struct", temp))
                            {
                                Is_struct = TRUE;
                            }
                            else
                            {
                                if(!strlen(OtherName))
                                {
                                    strcpy(OtherName, temp);
                                }
                            }
                            continue;
                        }
                        else if(SCRAMBLE_STR_TYPE_VARIABLE == Str_Type)
                        {
                            if(!strlen(OtherName))
                            {
                                strcpy(OtherName, temp);
                            }
                            else
                            {
                                b_Ret = TRUE;
                                strcpy(OtherName, temp);
                                if(!IsIn_ReplaceOthers_Record(OtherName))
                                {
                                    Generate_Random_String(scramble_temp);
                                    //printf("%s-%s ", temp, Parser_File[i]);
                                    Set_ReplaceOthers_Record(OtherName, scramble_temp);
                                }
                                break;
                            }
                        }
                        else
                        {
                            b_Ret = FALSE;
                            break;
                        }
                    }
                }
            }
            fclose(fp1);
        }
    }
    return;
}


//===================================
void Collect_Prototype_Define(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char DefineName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    long Cur_Offset = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type != SCRAMBLE_STR_TYPE_MACRODEF)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                if(!strcmp("#define", temp))
                {
                    while(!feof(fp1))
                    {
                        Scramble_Search_String(temp, fp1);
                        if(SCRAMBLE_STR_TYPE_VARIABLE == Str_Type ||
                            SCRAMBLE_STR_TYPE_FUNCTION == Str_Type)
                        {
                            b_Ret = TRUE;
                            strcpy(DefineName, temp);
                            if(!IsIn_ReplaceDefine_Record(DefineName))
                            {
                                Generate_Random_String(scramble_temp);
                                Set_ReplaceDefine_Record(temp, scramble_temp);
                            }
                            break;
                        }
                    }
                }
                else
                {
                    continue;
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_Prototype_Digit(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char i = 0;
    char j = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    long Cur_Offset = 0;
    int Num_System = 10;
    char len = 0;
    char start = 0;
    int temp0 = 0;
    int temp1 = 0;
    int temp2 = 0;
    int temp3 = 0;
    char string[MAX_STR_LEN] = {0};
    char string_ULL[MAX_STR_LEN] = {0};
    char temp_str[MAX_STR_LEN] = {0};
    char Have_ULL = 0;
    char last_ch = 0;

    for(j = 0; j < (sizeof(Parser_File) / MAX_STR_LEN); j++)
    {
        Cur_Parser_File = j;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[j]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type != SCRAMBLE_STR_TYPE_DIGIT)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                else
                {
                    Have_ULL = 0;
                    last_ch = 0;
                    memset(string, 0x0, MAX_STR_LEN * sizeof(char));
                    memset(string_ULL, 0x0, MAX_STR_LEN * sizeof(char));
                    //232 -->  (0x14b6+2119-0x1c15)
                    len = strlen(temp);
                    last_ch = temp[len-1];
                    if(last_ch)
                    {
                        if(last_ch >= '0' && last_ch <= '9')
                        {
                            last_ch = 1;
                        }
                        else if (last_ch >= 'a' && last_ch <= 'f')
                        {
                            last_ch = 1;
                        }
                        else if (last_ch >= 'A' && last_ch <= 'F')
                        {
                            last_ch = 1;
                        }
                        else if(('l' == last_ch) || ('L' == last_ch) || ('u' == last_ch) || ('U' == last_ch))
                        {
                            //last_ch = 1;
                            last_ch = 2;
                        }
                        else
                        {
                            last_ch = 0;
                        }
                    }
                    if(last_ch == 2)
                    {
                        //printf("%s ", temp);
                        strcpy(string, temp);
                        strcpy(scramble_temp, temp);
                        if(!IsIn_ReplaceDigit_Record(string))
                        {
                            Set_ReplaceDigit_Record(string, scramble_temp);
                        }
                    }
                    else if(last_ch == 1)
                    {
                        for(i = len; i > 0; i--)
                        {
                            if(('l' == temp[i]) || ('L' == temp[i]) || ('u' == temp[i]) || ('U' == temp[i]))
                            {
                                Have_ULL++;
                            }
                        }
                        strncpy(string, temp, (len - Have_ULL));
                        if(Have_ULL)
                        {
                            strncpy(string_ULL, temp + (len - Have_ULL), Have_ULL);
                        }
                        Num_System = 10;
                        start = 0;
                        //define the Number_System
                        if(len >= 2)
                        {
                            if(('0' == string[0]) && (string[1] != 'x'))
                            {
                                Num_System = 8;
                                start = 1;
                            }
                        }
                        if(len >= 3)
                        {
                            if(('0' == string[0]) && ('x' == string[1]))
                            {
                                Num_System = 16;
                                start = 2;
                            }
                        }
                        MAIN_DBG2(("(%d,%d)", Num_System, start));
                        if(len < 10)//0xffffffff=4294967295
                        {
                            if(8 == Num_System)
                            {
                                strcpy(scramble_temp, string);
                                if(Have_ULL)
                                {
                                    strcat(scramble_temp, string_ULL);
                                }
                            }
                            else
                            {
                                //change string to decimal
                                temp0 = 0;
                                for(i = start; i < len; i++)
                                {
                                    if((string[i] >= 0x30) && (string[i] <= 0x39))//0~9
                                    {
                                        temp0 += (string[i] - 0x30) * (int)pow(Num_System, (len - i - 1));
                                    }
                                    if((string[i] >= 0x41) && (string[i] <= 0x46))//A~F
                                    {
                                        temp0 += (string[i] - 0x41) * (int)pow(Num_System, (len - i - 1));
                                    }
                                    if((string[i] >= 0x61) && (string[i] <= 0x66))//a~f
                                    {
                                        temp0 += (string[i] - 0x61 + 10) * (int)pow(Num_System, (len - i - 1));
                                    }
                                }
                                MAIN_DBG2(("\n[%d %d %d]", start, len, temp0));
                                //printf("[%d %d %d]", start, len, temp0);
                                //Create 3 random digit for new expression
                                temp1 = rand() % 10000;
                                temp2 = rand() % 10000;
                                temp3 = temp0 + ((temp1 > temp2) ? (temp1 - temp2) : (temp2 - temp1));
                                MAIN_DBG2(("%d %d %d -- ", temp1, temp2, temp3));
                                //change express algorithm to string : temp1(hexadecimal) + temp3(decimal) - temp2(hexadecimal)
                                if(temp1 > temp2)
                                {
                                    sprintf(temp_str, "(0x%x", temp2);
                                    strcat(temp_str, " + ");
                                    sprintf(temp_str + strlen(temp_str), "%d", temp3);
                                    strcat(temp_str, " - ");
                                    sprintf(temp_str + strlen(temp_str), "0x%x)", temp1);
                                }
                                else
                                {
                                    sprintf(temp_str, "(0x%x", temp1);
                                    strcat(temp_str, " + ");
                                    sprintf(temp_str + strlen(temp_str), "%d", temp3);
                                    strcat(temp_str, " - ");
                                    sprintf(temp_str + strlen(temp_str), "0x%x)", temp2);
                                }
                                MAIN_DBG2(("%s ", temp_str));

                                strcpy(scramble_temp, temp_str);
                                if(Have_ULL)
                                {
                                    strcat(scramble_temp, string_ULL);
                                }
                            }
                        }
                        else
                        {
                            strcpy(scramble_temp, string);
                            if(Have_ULL)
                            {
                                strcat(scramble_temp, string_ULL);
                            }
                        }
                        //change express to character
                        if(Have_ULL)
                        {
                            strcat(string, string_ULL);
                        }
                        if(!IsIn_ReplaceDigit_Record(string))
                        {
                            //printf(" -- %d %d %d -- (%s %s) ]\n", temp1, temp2, temp3, string, scramble_temp);
                            Set_ReplaceDigit_Record(string, scramble_temp);
                        }
                    }
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_Prototype_Function(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char FunctionName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    long Cur_Offset = 0;
    char left_brace = 0;
    char right_brace = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                //printf("[%x %s-%d] ", ftell(fp1),temp, Str_Type);
                if(Str_Type != SCRAMBLE_STR_TYPE_FUNCTION)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                else
                {
                    Cur_Offset = ftell(fp1);
                    strcpy(FunctionName, temp);
                }
                //printf("[%x %s-%d] ", ftell(fp1),FunctionName, Str_Type);
                while(!feof(fp1))
                {
                    struct_char = fgetc(fp1);
                    if(struct_char != 0x20)
                    {
                        break;
                    }
                }
                left_brace = 0;
                right_brace = 0;
                if('(' == struct_char)
                {
                    left_brace++;
                    while(!feof(fp1))
                    {
                        struct_char = fgetc(fp1);
                        if('(' == struct_char)
                        {
                            left_brace++;
                        }
                        if(')' == struct_char)
                        {
                            right_brace++;
                            if(left_brace == right_brace)
                            {
                                b_Ret = TRUE;
                                break;
                            }
                        }
                        if('{' == struct_char)
                        {
                            if(left_brace != right_brace)
                            {
                                b_Ret = FALSE;
                                break;
                            }
                        }
                    }
                    while(b_Ret && !feof(fp1))
                    {
#if 1
                        Scramble_Search_String(temp, fp1);

                        if(Str_Type != SCRAMBLE_STR_TYPE_SUSPEND)
                        {
                            if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                            {
                                while(!feof(fp1))
                                {
                                    Scramble_Search_String(temp, fp1);
                                    if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                                    {
                                        Cur_Offset = ftell(fp1);
                                        if(fgetc(fp1) == 0x2F)// /
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            fseek(fp1, Cur_Offset, SEEK_SET);
                                        }
                                    }
                                }
                            }
                            else if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                            {
                                while(!feof(fp1))
                                {
                                    Scramble_Search_String(temp, fp1);
                                    if((strlen(temp) == 1) && (temp[0] == 0x0A))
                                    {
                                        break;
                                    }
                                }
                            }
                            else if(SCRAMBLE_STR_TYPE_SPACE == Str_Type ||
                                SCRAMBLE_STR_TYPE_RETURN == Str_Type ||
                                SCRAMBLE_STR_TYPE_TAB == Str_Type)
                            {
                            }
                            else
                            {
                                b_Ret = FALSE;
                                break;
                            }
                        }
                        else
                        {
                            if(!strcmp(temp, "{"))// {
                            {
                                b_Ret = TRUE;
                                break;
                            }
                            else
                            {
                                b_Ret = FALSE;
                                break;
                            }
                        }

#else
                        struct_char = fgetc(fp1);
                        if(0x7B == struct_char)// {
                        {
                            b_Ret = TRUE;
                            break;
                        }
                        if((0x20 == struct_char) || (0x0A == struct_char) || (0x0D == struct_char))
                        {
                        }
                        else
                        {
                            b_Ret = FALSE;
                            break;
                        }
#endif
                    }
                }

                if(b_Ret)
                {
                    if(!IsIn_ReplaceFunc_Record(FunctionName))
                    {
                        Generate_Random_String(scramble_temp);
                        Set_ReplaceFunc_Record(FunctionName, scramble_temp);
                    }
                }
                else
                {
                    fseek(fp1, Cur_Offset, SEEK_SET);
                }
            }
            fclose(fp1);
        }
    }
    return;
}

//===================================
void Collect_NoPrototype_Function(void)
{
    FILE *fp1 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char FunctionName[MAX_STR_LEN] = {0};
    char i = 0;
    char b_Ret = FALSE;
    char struct_char = 0;
    char left_parenthesis = 0;
    char right_parenthesis = 0;
    long Cur_Offset = 0;

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    {
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);

        if((fp1 = fopen(FileName,"rb")) == NULL)
        {
            MAIN_ERR(("\n[%d] error fp1 open %s", __LINE__, FileName));
            return;
        }
        else
        {
            while(!feof(fp1))
            {
                Scramble_Search_String(temp, fp1);
                if(Str_Type != SCRAMBLE_STR_TYPE_FUNCTION)
                {
                    if(SCRAMBLE_STR_TYPE_COMMENT_MULTI == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
                            {
                                Cur_Offset = ftell(fp1);
                                if(fgetc(fp1) == 0x2F)// /
                                {
                                    break;
                                }
                                else
                                {
                                    fseek(fp1, Cur_Offset, SEEK_SET);
                                }
                            }
                        }
                    }
                    if(SCRAMBLE_STR_TYPE_COMMENT_SINGLE == Str_Type)
                    {
                        while(!feof(fp1))
                        {
                            Scramble_Search_String(temp, fp1);
                            if((strlen(temp) == 1) && (temp[0] == 0x0A))
                            {
                                break;
                            }
                        }
                    }
                    continue;
                }
                else
                {
                    Cur_Offset = ftell(fp1);
                    strcpy(FunctionName, temp);
                    if(IsIn_ReplaceFunc_Record(FunctionName) || IsIn_ReplaceOthers_Record(FunctionName)
                        || IsIn_ReplaceDefine_Record(FunctionName))
                    {
                    }
                    else if(!IsIn_NoPrototypeFunc_Record(FunctionName))
                    {
                        b_Ret = TRUE;
                        Set_NoPrototypeFunc_Record(FunctionName);
                    }
                }
            }
            fclose(fp1);
        }
    }
    return;
}


// replace variable_string by expression
char *Scramble_Variable_String(char *str)
{
    char string[MAX_STR_LEN] = {0};
    if(str == NULL)
    {
        return NULL;
    }
    else
    {
        strcpy(string, str);
    }
    if(Is_Include)
    {
        strcpy(scramble_temp, string);
        return scramble_temp;
    }
    if((1 == temp_record.Temp_Record_Status) && (IsIn_Temp_Record(string) || IsIn_NoP_Struct_InS_Record(string)))
    {
        if(!IsIn_ReplaceVar_Record(string))
        {
            strcpy(scramble_temp, string);
        }
        //printf("[%s %s] ", string, scramble_temp);

        while(!feof(fpNow))
        {
            Scramble_Search_String(temp, fpNow);
            if(!strcmp(temp, "-") || !strcmp(temp, ">") ||
                !strcmp(temp, ".") ||
                (SCRAMBLE_STR_TYPE_VARIABLE ==Str_Type))
            {
                strcat(scramble_temp,temp);
            }
            else
            {
                strcat(scramble_temp,temp);
                return scramble_temp;
            }
        }
    }

    if(IsIn_NoPrototypeStruct_Record(string))
    {
        if(temp_record.Temp_Record_Status == 2)
        {
            temp_record.Temp_Record_Status = 1;

            //printf("4-%s ", string);
            if(!IsIn_Temp_Record(string))
            {
                Set_Temp_Record(string);
            }
        }
        else if(temp_record.Temp_Record_Status == 1)
        {
            long Cur_Offset = 0;
            Cur_Offset = ftell(fpNow);
            //if((temp_next != 0x2C) && (temp_next != 0x3B) && (temp_next != 0x29) && (temp_next != 0x2D))// , ; ) -
            while(!feof(fpNow))
            {
                Scramble_Search_String(temp, fpNow);
                if(Str_Type == SCRAMBLE_STR_TYPE_VARIABLE)
                {
                //printf("\n(%s) ", string);
                    temp_record.Temp_Record_Status = 2;
                    break;
                }
                else if(Str_Type == SCRAMBLE_STR_TYPE_TAB || Str_Type == SCRAMBLE_STR_TYPE_SPACE)
                {
                    continue;
                }
                else if(Str_Type == SCRAMBLE_STR_TYPE_SUSPEND)
                {
                    if(!strcmp("*", temp))
                    {
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            fseek(fpNow, Cur_Offset, SEEK_SET);
        }
        strcpy(scramble_temp, string);
        return scramble_temp;
    }
    else if(IsIn_ReplaceStruct_Record(string))
    {
        if(temp_record.Temp_Record_Status == 2)
        {
        //printf("1-%s ", string);
            temp_record.Temp_Record_Status = 1;
        }
        return scramble_temp;
    }
    else if(IsIn_ReplaceEnum_Record(string))
    {
        return scramble_temp;
    }
    else if(IsIn_ReplaceUnion_Record(string))
    {
        return scramble_temp;
    }
    else if(IsIn_ReplaceOthers_Record(string))
    {
        if(temp_record.Temp_Record_Status == 2)
        {
        //printf("2-%s ", string);
            temp_record.Temp_Record_Status = 1;
        }
        return scramble_temp;
    }
    else if(IsIn_ReplaceDefine_Record(string))
    {
        return scramble_temp;
    }
    else if(IsIn_ReplaceFunc_Record(string))
    {
        return scramble_temp;
    }
    else if(IsIn_NoPrototypeFunc_Record(string))
    {
        strcpy(scramble_temp, string);
        return scramble_temp;
    }
    else
    {
        if(strlen(string) == 1)
        {
            if(0x5C == string[0])
            {
                Is_Backlash = 1;
                strcpy(scramble_temp, string);
                return scramble_temp;
            }
            else if(0x23 == string[0])
            {
                Is_Well = 1;
                strcpy(scramble_temp, string);
                return scramble_temp;
            }
            else
            {
                long Cur_Offset = 0;
                Cur_Offset = ftell(fpNow);
                if(fgetc(fpNow) == 0x27)// '
                {
                    fseek(fpNow, Cur_Offset, SEEK_SET);
                    strcpy(scramble_temp, string);
                    return scramble_temp;
                }
                fseek(fpNow, Cur_Offset, SEEK_SET);
            }
        }
        if((strlen(string) == 2) && (0x5C == string[0]))
        {
            strcpy(scramble_temp, string);
            return scramble_temp;
        }
        if(temp_record.Temp_Record_Status == 2)
        {
            temp_record.Temp_Record_Status = 1;

            //printf("3-%s ", string);
            if(!IsIn_Temp_Record(string))
            {
                Set_Temp_Record(string);
            }
        }
        if((strlen(string) > 2) && (string[0] == 0x23) && (string[1] != 0x23))
        {
            char string_after_well[MAX_STR_LEN] = {0};
            strcpy(string_after_well, string + 1);
            if(IsIn_ReplaceVar_Record(string_after_well))
            {
                strcpy(string_after_well, scramble_temp);
                scramble_temp[0] = 0x23;
                scramble_temp[1] = 0;
                strcat(scramble_temp, string_after_well);
            }
            else
            {
                Generate_Random_String(scramble_temp);
                strcpy(string_after_well, scramble_temp);
                scramble_temp[0] = 0x23;
                scramble_temp[1] = 0;
                strcat(scramble_temp, string_after_well);
                Set_ReplaceVar_Record(string, scramble_temp);
            }
            //printf("%s %s ", string_after_well, scramble_temp);
            return scramble_temp;
        }
        //in 10replace_var.txt
        if(IsIn_ReplaceVar_Record(string))
        {
            return scramble_temp;
        }
        else
        {
            Generate_Random_String(scramble_temp);
            Set_ReplaceVar_Record(string, scramble_temp);
        }
    }

    return scramble_temp;
}


// replace variable_string by expression
char *Scramble_Digit_String(char *string)
{
    if(string == NULL)
    {
        return NULL;
    }
#if 0
    if(IsIn_ReplaceDigit_Record(string))
    {
        long Cur_Offset = 0;
        char temp_char = 0;
        Cur_Offset = ftell(fpNow);
        temp_char = fgetc(fpNow);
        fseek(fpNow, Cur_Offset, SEEK_SET);
        if(0x27 == temp_char && 1 == strlen(string))
        {
            strcpy(scramble_temp, string);
            return scramble_temp;
        }
        else
        {
            return scramble_temp;
        }
    }
    else
#endif
    {
        strcpy(scramble_temp, string);
    }
    return scramble_temp;
}

//replace string with its corresponding hexadecimal expression
char *Scramble_String_String(char *string)
{
    char i = 0;
    char temp[4] = {0};
    char temp_char = 0;
    char str_len = strlen(string);

    if(string == NULL)
    {
        return NULL;
    }
    else
    {
        strcpy(scramble_temp, string);// "
        //"cust"  --กต  "\x63\x75\x73\x74"
        while(!feof(fpNow))
        {
            temp_char = fgetc(fpNow);
            if(0x22 == temp_char)// "
            {
                temp[0] = temp_char;
                temp[1] = 0;
                strcat(scramble_temp, temp);
                break;
            }
            else if(0x5c == temp_char)
            {
                temp[0] = temp_char;
                temp[1] = 0;
                strcat(scramble_temp, temp);
                temp[0] = fgetc(fpNow);
                temp[1] = 0;
                strcat(scramble_temp, temp);
            }
            else
            {
#if 1
                temp[0] = temp_char;
                temp[1] = 0;
#else
                sprintf(temp, "0x%x", temp_char);
#endif
                strcat(scramble_temp, temp);
            }
        }
    }
    return scramble_temp;
}
//replace /**/with nothing
char *Scramble_CommentMulti_String(char *string)
{
    long Cur_Offset = 0;
    if(string == NULL)
    {
        return NULL;
    }
    else
    {
        while(!feof(fpNow))
        {
            Scramble_Search_String(temp, fpNow);
            if((strlen(temp) == 1) && (temp[0] == 0x2A))// *
            {
                Cur_Offset = ftell(fpNow);
                if(fgetc(fpNow) == 0x2F)// /
                {
                    break;
                }
                else
                {
                    fseek(fpNow, Cur_Offset, SEEK_SET);
                }
            }
        }
    }
    scramble_temp[0] = 0;
    return scramble_temp;
}

//replace // with nothing
char *Scramble_CommentSingle_String(char *string)
{
    if(string == NULL)
    {
        return NULL;
    }
    else
    {
        while(!feof(fpNow))
        {
            Scramble_Search_String(temp, fpNow);
            if((strlen(temp) == 1) && (temp[0] == 0x0A))
            {
                break;
            }
        }
    }
    scramble_temp[0] = 0;
    return scramble_temp;
}


//replace ' ' with nothing
char *Scramble_Space_String(char *string)
{
    long Cur_Offset = 0;
    if(string == NULL)
    {
        return NULL;
    }
    else
    {
        if(Macro_Before || Is_Struct || Is_Function || Is_Well)
        {
            Is_Well = 0;
            strcpy(scramble_temp, string);
            return scramble_temp;
        }
        scramble_temp[0] = 0;
        return scramble_temp;
    }
    return NULL;
}


//replace ' ' with nothing
char *Scramble_Tab_String(char *string)
{
    long Cur_Offset = 0;
    if(string == NULL)
    {
        return NULL;
    }
    else
    {
        if(Macro_Before || Is_Struct || Is_Function)
        {
            strcpy(scramble_temp, string);
            return scramble_temp;
        }
        scramble_temp[0] = 0;
        return scramble_temp;
    }
    return NULL;
}


//replace RETURN with nothing
char *Scramble_Return_String(char *string)
{
    long Cur_Offset = 0;
    if(string == NULL)
    {
        return NULL;
    }
    else
    {
        if(Is_Backlash)
        {
            Is_Backlash = 0;
            strcpy(scramble_temp, string);
            Save_Return = 1;
            return scramble_temp;
        }
        else if(Macro_Before)
        {
            Macro_Before = 0;
            Is_Include = 0;
            strcpy(scramble_temp, string);
            Save_Return = 1;
            return scramble_temp;
        }
        else
        {
            Is_Struct = 0;
        }
        Save_Return = 0;
        scramble_temp[0] = 0;
        return scramble_temp;
    }
    return NULL;
}


//replace MacroDef with nothing
char *Scramble_MacroDef_String(char *string)
{
    Macro_Before = 1;

    if(string == NULL)
    {
        return NULL;
    }
    if(!strcmp(string, "#if"))
    {
        long Cur_Offset = 0;
        Cur_Offset = ftell(fpNow);
        while(!feof(fpNow))
        {
            char next = fgetc(fpNow);
            if(next == '0')
            {
                while(!feof(fpNow))
                {
                    Scramble_Search_String(temp, fpNow);
                    if(!strcmp(string, "#endif"))
                    {
                        scramble_temp[0] = 0;
                        return scramble_temp;
                    }
                }
            }
            else if(next == 0x20 || next == 0x09)
            {
                continue;
            }
            else
            {
                fseek(fpNow, Cur_Offset, SEEK_SET);
                break;
            }
        }
    }
    if(!strcmp(string, "#include"))
    {
        Is_Include = 1;
    }
    memset(scramble_temp, 0x0, MAX_STR_LEN * 4 * sizeof(char));
    strcpy(scramble_temp, string);
    return scramble_temp;
}

//replace KeyWords with nothing
char *Scramble_KeyWords_String(char *string)
{
    if(string == NULL)
    {
        return NULL;
    }
    strcpy(scramble_temp, string);
    strcat(scramble_temp, " ");

    if(!strcmp(string,"struct"))
    {
        Is_Struct = 1;
    }
    if(!strcmp(string,"typedef"))
    {
        Is_Typedef = 1;
    }
    return scramble_temp;
}

//replace KeyWords before double well
char *Scramble_DWell_String(char *string)
{
    if(string == NULL)
    {
        return NULL;
    }

    strcpy(scramble_temp, string);

    return scramble_temp;
}

//replace End Symbol with itself
char *Scramble_EndSymbol_String(char *string)
{
    if(string == NULL)
    {
        return NULL;
    }
    strcpy(scramble_temp, string);
    if(Is_Function && !strcmp(")", string))
    {
        Is_Function = 0;
    }
    if(temp_record.Temp_Record_Status)
    {
        if(!strcmp(";", string))
        {
            if(temp_record.Left_Parenthesis == 0)
            {
                temp_record.Temp_Record_Status = 0;
                temp_record.Left_Parenthesis = 0;
                temp_record.Right_Parenthesis = 0;
                Clear_Temp_Record();
            }
        }
        if(!strcmp("{", string))
        {
            temp_record.Left_Parenthesis++;
        }
        if(!strcmp("}", string))
        {
            temp_record.Right_Parenthesis++;
            if(temp_record.Left_Parenthesis == temp_record.Right_Parenthesis)
            {
                temp_record.Temp_Record_Status = 0;
                temp_record.Left_Parenthesis = 0;
                temp_record.Right_Parenthesis = 0;
                Clear_Temp_Record();
            }
        }
    }
    return scramble_temp;
}

void Scramble_File(void)
{
	FILE *fp2 = NULL;
    char FileName[MAX_STR_LEN] = {0};
    char NewFileName[MAX_STR_LEN] = {0};
    char Scrambling[MAX_STR_LEN] = {0};
    int i = 0;
    int j = 0;
    printf("%d \n",(sizeof(Parser_File) / MAX_STR_LEN));

    for(i = 0; i < (sizeof(Parser_File) / MAX_STR_LEN); i++)
    //for(i = 0; i < 1; i++)
    {
        printf("[%d]",i);
        Cur_Parser_File = i;
        strcpy(FileName, DIR_PATH);
        strcat(FileName, Parser_File[i]);
        strcpy(NewFileName, DIR_PATH);
        strcat(NewFileName, "new/");
        strcat(NewFileName, Parser_File[i]);

        MAIN_DBG(("\nFileName = %s NewFileName = %s\n", FileName, NewFileName));

        if((fpNow = fopen(FileName,"rb")) == NULL)
            MAIN_ERR(("error fpNow open\n"));
        if((fp2 = fopen(NewFileName,"wb")) == NULL)
            MAIN_ERR(("error fp2 open\n"));
        while(!feof(fpNow))
        //for(j = 0; j < 1000; j++)
        {
            //temp evalued
            //printf("%x ",ftell(fpNow));
            Scramble_Search_String(temp, fpNow);
            MAIN_DBG2(("%s(%d)", temp, Str_Type));
            switch(Str_Type)
            {
                case SCRAMBLE_STR_TYPE_FUNCTION:
                    Scramble_Strcpy(Scrambling, Scramble_Function_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_VARIABLE:
                    Scramble_Strcpy(Scrambling, Scramble_Variable_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_DIGIT:
                    Scramble_Strcpy(Scrambling, Scramble_Digit_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_STRING:
                    Scramble_Strcpy(Scrambling, Scramble_String_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_COMMENT_MULTI:
                    Scramble_Strcpy(Scrambling, Scramble_CommentMulti_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_COMMENT_SINGLE:
                    Scramble_Strcpy(Scrambling, Scramble_CommentSingle_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_SPACE:
                    Scramble_Strcpy(Scrambling, Scramble_Space_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_TAB:
                    Scramble_Strcpy(Scrambling, Scramble_Tab_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_RETURN:
                    Scramble_Strcpy(Scrambling, Scramble_Return_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_MACRODEF:
                    Scramble_Strcpy(Scrambling, Scramble_MacroDef_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_KEYWORDS:
                    Scramble_Strcpy(Scrambling, Scramble_KeyWords_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_DWELL:
                    Scramble_Strcpy(Scrambling, Scramble_DWell_String(temp));
                    break;
                case SCRAMBLE_STR_TYPE_NULL:
                case SCRAMBLE_STR_TYPE_SUSPEND:
                case SCRAMBLE_STR_TYPE_MAX:
                default:
                    Scramble_Strcpy(Scrambling, Scramble_EndSymbol_String(temp));
                    break;
            }
            //Write new string to new file
            Write_Str_ToNewFile(Scrambling, fp2);
        }
        //Add junk code in struct, and record this operation in add.txt
        //Add_JunkCode_ToNewFile(fp2);
        fclose(fpNow);
        fclose(fp2);

    }
    return;
}

void main(void)
{
    srand((unsigned int)time(NULL));
#if 0
    Collect_Prototype_Struct();
    Collect_Prototype_OtherTypedef();
    Collect_NoPrototype_Struct();
    Collect_Prototype_Define();
    Collect_Prototype_Function();
    Collect_NoPrototype_Function();
    Collect_Prototype_Enum();
    Collect_Prototype_Union();
    Collect_Prototype_Digit();
#endif
    Scramble_File();
    return;
}


