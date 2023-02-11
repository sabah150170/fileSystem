
/*
BUSE NUR SABAH, 150170002
28/12/2021

Compile: gcc hello.c check_with_magic.c  -o o -ansi -W -std=c99 -g -ggdb -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -lfuse -lmagic
Mount: ./o project3_root -f -o nonempty
Open file: xdg-open <filename> 
file needs to have read permission

commands:
	getattr: call in the beginning each time

	rm: 	access-unlink
	move: 	rename
	touch: 	utime
	echo: 	open-truncate-release
	cp: 	open-truncate-release
	chmod: 	chmod
	chown: 	chown
	cd: 	access
	find: 	access

	xdg-open: access-open-read-release-access-open
	cat: 	open-read-release
	grep:   open-read-release
*/

#define FUSE_USE_VERSION 26

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <dirent.h>
#include <unistd.h>
#include <fuse.h>
#include <stdbool.h>
#include <sys/xattr.h>

void check_magic(char* filename, char *mpath);


/******************************************************************************************************/
static int my_getattr(const char *path, struct stat *stbuf){
	printf("GETATTR...%s \n", path);

    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) {
    	printf("ROOT\n");
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else{
		for(uint i=1; i<strlen(path); i++){
			if(!strncmp(&path[i], "/", 1)){ // it is a file
				printf("FILE\n");

				char new_path[300];
				strcpy(new_path, "/home/");
				strcat(new_path, &(path[i+1]));

			    if(lstat(new_path, stbuf) == -1){
			    	printf("Error in lstat!..\n");
			        return -errno;
			    }
			    return 0;						
			}
		}	
		
		printf("DIRECTORY\n");
        stbuf->st_mode = S_IFDIR | 0555;
        stbuf->st_nlink = 2;
    }
    return 0;
}

static int my_open(const char *path, struct fuse_file_info *fi){ //check for permissions and presence
	printf("OPEN...\n");

	printf("writepage:%d, direct_io:%d, keep_cache:%d, flush:%d, nonseekable:%d, flag:%d, padding:%d, fh:%lld, lock_owner:%lld\n",fi->writepage, fi->direct_io, fi->keep_cache,
		fi->flush, fi->nonseekable, fi->flags, fi->padding, fi->fh, fi->lock_owner);

    int flags = fi->flags; //what we want to do with this file
    if ((flags & O_WRONLY) || (flags & O_RDWR) || (flags & O_CREAT) || (flags & O_EXCL) || (flags & O_TRUNC) || (flags & O_APPEND)){ //allowed only for read
        return -EROFS;
    }

	for(uint i=1; i<strlen(path); i++){
		if(!strncmp(++path, "/", 1)){ //it is a file
			char new_path[300];
			strcpy(new_path, "/home/");
			strcpy(&new_path[26], path);

			int fd = open(new_path, O_RDONLY); //check for presence 
		    if(fd == -1) {
		        return -errno;
		    }
		    close(fd);		
		}
	}	
    return 0;
}

static int my_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){ 
	printf("READ...\n");

	int res = 0;
	for(uint i=1; i<strlen(path); i++){
		if(!strncmp(++path, "/", 1)){ // it is a file
			char new_path[300];
			strcpy(new_path, "/home/");
			strcpy(&new_path[26], path);

			int fd = open(new_path, O_RDONLY);
		    if(fd == -1) {
		        return -errno;
		    }
		   
		    res = pread(fd, buf, size, offset); 
		    if(res == -1) {
		        res = -errno;
		    }
		    close(fd);		
		}
	}	
	
	buf[res] = '\0'; // prevent to print unwanted area
	printf("output: %s\n", buf);
    return res; //how many bytes are read
}

static int my_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) { //return one or more directory entries 
	printf("READDIR... %s\n", path);
	
	DIR *dp;
    struct dirent *de;

    dp = opendir("/home/");
    
    if(dp == NULL) {
        return -errno;
    }
	
	filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    
    char arr[256][20]; //keeps which extention is already listed
    int size=0;
    bool flag = 1;
    bool flag2 = 1;
    char mpath[300];
    
    if(!strcmp("/", path)){ //list extensions
		 while((de = readdir(dp)) != NULL) { //traverse root directory
			  
			if(strncmp(de->d_name, "..", 2) && strncmp(de->d_name, ".", 1)){
				check_magic(de->d_name, mpath); // get file type
				printf("--> %s\n", mpath);

				for(int i=0; i<size; i++){ //check whether this extension is already listed
					if(strcmp(arr[i], mpath) == 0){
						flag = 0;
						break; 
					}
				}
			
				if(flag){ //this extention has not been listed yet
					strcpy(arr[size++], mpath); //add this extention into the array
					if (filler(buf, mpath, NULL, 0)){
						printf("Error in filler!..\n");
						break;
					}
				}
				flag = 1;
			}
		}
	}
	
	else{ //list a subdirectory or files inside subdirectories, ls my_file/pdf | ls ./pdf | ls pdf | ls pdf/ --> /pdf
		bool flag2 = 1;
		path++; //get subdirectory by eliminating "/"
		while((de = readdir(dp)) != NULL)  { //traverse root directory
			if(strncmp(de->d_name, "..", 2) && strncmp(de->d_name, ".", 1)){
				check_magic(de->d_name, mpath); //get file type

				if(!strcmp(mpath, path)){ //check whether file's extension is same with demanded one
					flag2 = 0;

			        if (filler(buf, de->d_name, NULL, 0)){
			        	printf("Error in filler!..\n");
			            break;
			        }
				}
			}
		}
		if(flag2){
			return -ENOENT;
		}
	}

    closedir(dp);
    return 0;
}

static int my_unlink(const char *path){ //remove the given file, symbolic-link, hard-link, or node
    printf("UNLINK...\n");

    for(uint i=1; i<strlen(path); i++){
		if(!strncmp(++path, "/", 1)){ // it is a file
			char new_path[300];

			strcpy(new_path, "/home/");
			strcpy(&new_path[26], path);

			if(remove(new_path)){ //remove actual file
				printf("Error in removing the file!..\n");
			}
			return 0;			
		}
	}	
    return -EROFS; 
}

static int my_access(const char *path, int mode){ //check read permission for xdg-open
    printf("ACCESS...\n");

    if( strcmp("/", path) == 0){ //root directory
    	return 0;
    }

    //if it is a file
    char mpath[300];
    strcpy(mpath, path);
    for(uint i=1; i<strlen(path); i++){
		if(!strncmp(++path, "/", 1)){ 
			char new_path[300];

			strcpy(new_path, "/home/");
			strcpy(&new_path[26], path);

			int res = access(new_path, mode);
			printf("access code: %d\n", res);
		    if (res == -1) {
		        return -EACCES;
		    }	
		    return res;
		}
	}

	//if it is a directory, cd directory
	DIR *dp;
    struct dirent *de;
    dp = opendir("/home/");
	char mpath2[300];
	while((de = readdir(dp)) != NULL)  { //traverse root directory
		if(strncmp(de->d_name, "..", 2) && strncmp(de->d_name, ".", 1)){
			check_magic(de->d_name, mpath2); //get file type

			if(!strcmp(mpath2, &(mpath[1]))){ //check whether file's extension is same with demanded one
				return 0;
			}
		}
	}
	return -ENOENT;
}

/**************************************** RETURN -EROFS ***********************************************/

static int my_mknod(const char *path, mode_t mode, dev_t rdev){
    printf("ROFS_MKNOD\n");
    return -EROFS;
}
static int my_mkdir(const char *path, mode_t mode){
    printf("ROFS_MKDIR\n");
    return -EROFS;
}



static int my_statfs(const char *path, struct statvfs *st_buf){
    printf("ROFS_STATFS\n");
    return -EROFS;
}

static int my_fsync(const char *path, int crap, struct fuse_file_info *finfo){
    printf("ROFS_FSYNC\n");
    return -EROFS;
}

static int my_release(const char *path, struct fuse_file_info *finfo){ //free up any temporarily allocated data structures, IBM: one release per open
    printf("RELEASE...\n");
    return 0;;
}

static int my_chmod(const char *path, mode_t mode){
    printf("CHMOD...\n");
    return -EROFS;;
}

static int my_chown(const char *path, uid_t uid, gid_t gid){
    printf("CHOWN...\n");
    return -EROFS;
}

static int my_truncate(const char *path, off_t size){ //never gonna be used because "open" handles
    printf("TRUNCATE...\n");
    return -EROFS;;
}

static int my_utime(const char *path, struct utimbuf *buf){ 
    printf("UTIME...\n");
    return -EROFS;;
}

static int my_rename(const char *from, const char *to){ 
    printf("RENAME...\n");	
    return -EROFS; 
}

static int my_rmdir(const char *path){ //remove directory
    printf("RMDIR...\n");
    return -EROFS;
}

static int my_symlink(const char *from, const char *to){ //create a symbolic link
    printf("SYMLINK\n");
    return -EROFS;
}

static int my_link(const char *from, const char *to){ //create a hard link
    printf("LINK\n");
    return -EROFS;
}

static int my_readlink(const char *path, char *buf, size_t size){ //read symbolic link, never gonne be used because there is no symbolic link
    printf("ROFS_READLINK\n");
    return -EROFS;
}

static int my_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *finfo){
    printf("ROFS_WRITE\n");
    return -EROFS;
}

static int my_setxattr(const char *path, const char *name, const char *value, size_t size, int flags){ //set the value of an extended attribute
    printf("setxattr...\n");
    return -EROFS;
}

static int my_getxattr(const char *path, const char *name, char *value, size_t size){ //get the value of an extended attribute
    printf("getxattr...\n");

    char new_path[300];
    strcpy(new_path, "/home/");
	strcpy(&new_path[26], path);

    int res = lgetxattr(new_path, name, value, size);

    printf("name:%s, value:%s, size:%d\n", name, value, size);

    if(res == -1) {
        return -errno;
    }
    return res;
}

static int my_listxattr(const char *path, char *list, size_t size){ //list the supported extended attributes
    printf("listxattr...\n");

    char new_path[300];
    strcpy(new_path, "/home/");
	strcpy(&new_path[26], path);

    int res = llistxattr(new_path, list, size);

    if(res == -1) {
        return -errno;
    }
    return res;
}

static int my_removexattr(const char *path, const char *name){ //remove an extended attribute
    printf("removexattr...\n");	
    return -EROFS;
}
/*****************************************************************************************************/

static struct fuse_operations my_oper = {
    .getattr	 = my_getattr,
    .readdir	 = my_readdir, 
    .open		 = my_open,
    .read		 = my_read,
    .readlink    = my_readlink,
    .mknod       = my_mknod,
    .mkdir       = my_mkdir,
    .symlink     = my_symlink,
    .unlink      = my_unlink,
    .rmdir       = my_rmdir,
    .rename      = my_rename,
    .link        = my_link,
    .chmod       = my_chmod,
    .chown       = my_chown,
    .truncate    = my_truncate,
    .utime       = my_utime,
    .write       = my_write,
    .statfs      = my_statfs,
    .release     = my_release,
    .fsync       = my_fsync,
    .access      = my_access, 

    //Extended attributes support for userland interaction 
    .setxattr    = my_setxattr,
    .getxattr    = my_getxattr,
    .listxattr   = my_listxattr,
    .removexattr = my_removexattr
};



int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &my_oper, NULL);
}
