#include <string.h>
#include <magic.h>
#include <stdbool.h> //true
#include <stdio.h> //printf
#include <stdlib.h> //exit




void extract_extension(char* type, char* filename){
	for(int i=strlen(filename)-1; i>0; i--){ //extract extention from file's name
		if(filename[i] == '.'){
			strcpy(type, &(filename[i+1]));  
			//printf("2 %s\n", type);
			return;
		}				 
	}
	strcpy(type, "none");  
}

void get_type(char* mime, char* type, char* filename){
		if 		( !strcmp(mime,  "audio/aac")) strcpy(type, "aac");
		else if ( !strcmp(mime,  "application/x-abiword" )) strcpy(type, "abw"); 
		else if ( !strcmp(mime,  "application/x-freearc" )) strcpy(type, "arc"); 
		else if ( !strcmp(mime,  "video/x-msvideo" )) strcpy(type, "avi"); 
		else if ( !strcmp(mime,  "application/vnd.amazon.ebook" )) strcpy(type, "azv"); 
		else if ( !strcmp(mime,  "application/octet-stream" )) strcpy(type, "bin"); 
		else if ( !strcmp(mime,  "image/bmp" )) strcpy(type, "bmp"); 
		else if ( !strcmp(mime,  "application/x-bzip" )) strcpy(type, "bz"); 
		else if ( !strcmp(mime,  "application/x-bzip2" )) strcpy(type, "bz2"); 
		else if ( !strcmp(mime,  "application/x-cdf" )) strcpy(type, "cda"); 
		else if ( !strcmp(mime,  "application/x-csh" )) strcpy(type, "csh"); 
		else if ( !strcmp(mime,  "text/css" )) strcpy(type, "css"); 
		else if ( !strcmp(mime,  "text/csv" )) strcpy(type, "csv"); 
		else if ( !strcmp(mime,  "application/msword" )) strcpy(type, "doc"); 
		else if ( !strcmp(mime,  "application/vnd.openxmlformats-officedocument.wordprocessingml.document" )) strcpy(type, "docx"); 
		else if ( !strcmp(mime,  "application/vnd.ms-fontobject" )) strcpy(type, "eot"); 
		else if ( !strcmp(mime,  "application/epub+zip" )) strcpy(type, "ebup"); 
		else if ( !strcmp(mime,  "application/gzip" )) strcpy(type, "gz"); 
		else if ( !strcmp(mime,  "image/gif" )) strcpy(type, "gif"); 
		else if ( !strcmp(mime,  "text/html" )) strcpy(type, "html"); 
		else if ( !strcmp(mime,  "image/vnd.microsoft.icon" )) strcpy(type, "ico"); 
		else if ( !strcmp(mime,  "text/calendar" )) strcpy(type, "ics"); 
		else if ( !strcmp(mime,  "application/java-archive" )) strcpy(type, "jar"); 
		else if ( !strcmp(mime,  "image/jpeg" )) extract_extension(type, filename); 
		else if ( !strcmp(mime,  "text/javascript" )) extract_extension(type, filename); 
		else if ( !strcmp(mime,  "application/json" )) strcpy(type, "json"); 
		else if ( !strcmp(mime,  "application/ld+json" )) strcpy(type, "jsonld"); 
		else if ( !strcmp(mime,  "audio/midi" )) strcpy(type, "midi"); 
		else if ( !strcmp(mime,  "audio/x-midi" )) strcpy(type, "midi"); 
		else if ( !strcmp(mime,  "audio/mpeg" )) extract_extension(type, filename); 
		else if ( !strcmp(mime,  "video/mp4" )) strcpy(type, "mp4"); 
		else if ( !strcmp(mime,  "application/vnd.apple.installer+xml" )) strcpy(type, "mpkg"); 
		else if ( !strcmp(mime,  "application/vnd.oasis.opendocument.presentation" )) strcpy(type, "odp"); 
		else if ( !strcmp(mime,  "application/vnd.oasis.opendocument.spreadsheet" )) strcpy(type, "ods"); 
		else if ( !strcmp(mime,  "application/vnd.oasis.opendocument.text" )) strcpy(type, "odt"); 
		else if ( !strcmp(mime,  "audio/ogg" )) strcpy(type, "oga"); 
		else if ( !strcmp(mime,  "video/ogg" )) strcpy(type, "ogv"); 
		else if ( !strcmp(mime,  "application/ogg" )) strcpy(type, "ogx"); 
		else if ( !strcmp(mime,  "audio/opus" )) strcpy(type, "opus"); 
		else if ( !strcmp(mime,  "font/otf" )) strcpy(type, "otf"); 
		else if ( !strcmp(mime,  "image/png" )) strcpy(type, "png");
		else if ( !strcmp(mime,  "application/pdf" )) strcpy(type, "pdf"); 
		else if ( !strcmp(mime,  "application/x-httpd-php" )) strcpy(type, "php"); 
		else if ( !strcmp(mime,  "application/vnd.ms-powerpoint" )) strcpy(type, "ppt"); 
		else if ( !strcmp(mime,  "application/vnd.openxmlformats-officedocument.presentationml.presentation" )) strcpy(type, "pptx"); 
		else if ( !strcmp(mime,  "application/vnd.rar" )) strcpy(type, "rar"); 
		else if ( !strcmp(mime,  "application/rtf" )) strcpy(type, "rtf"); 
		else if ( !strcmp(mime,  "application/x-sh" )) strcpy(type, "sh"); 
		else if ( !strcmp(mime,  "image/svg+xml" )) strcpy(type, "svg"); 
		else if ( !strcmp(mime,  "application/x-shockwave-flash" )) strcpy(type, "swf"); 
		else if ( !strcmp(mime,  "application/x-tar" )) strcpy(type, "tar"); 
		else if ( !strcmp(mime,  "image/tiff" )) strcpy(type, "tiff"); 
		else if ( !strcmp(mime,  "video/mp2t" )) strcpy(type, "ts"); 
		else if ( !strcmp(mime,  "font/ttf" )) strcpy(type, "ttf"); 
		else if ( !strcmp(mime,  "text/plain" )) extract_extension(type, filename); 
		else if ( !strcmp(mime,  "application/vnd.visio" )) strcpy(type, "vsd"); 
		else if ( !strcmp(mime,  "audio/wav" )) strcpy(type, "wav"); 
		else if ( !strcmp(mime,  "audio/webm" )) strcpy(type, "weba"); 
		else if ( !strcmp(mime,  "video/webm" )) strcpy(type, "webm"); 
		else if ( !strcmp(mime,  "image/webp" )) strcpy(type, "webp"); 
		else if ( !strcmp(mime,  "font/woff" )) strcpy(type, "woff"); 
		else if ( !strcmp(mime,  "font/woff2" )) strcpy(type, "woff2"); 
		else if ( !strcmp(mime,  "application/xhtml+xml" )) strcpy(type, "xhtml"); 
		else if ( !strcmp(mime,  "application/vnd.ms-excel" )) strcpy(type, "xls"); 
		else if ( !strcmp(mime,  "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet" )) strcpy(type, "xlsx"); 
		else if ( !strcmp(mime,  "application/xml" )) strcpy(type, "xml"); 
		else if ( !strcmp(mime,  "application/vnd.mozilla.xul+xml" )) strcpy(type, "xul"); 
		else if ( !strcmp(mime,  "application/zip" )) strcpy(type, "zip"); 
		else if ( !strcmp(mime,  "video/3gpp" )) strcpy(type, "3gp"); 
		else if ( !strcmp(mime,  "audio/3gpp" )) strcpy(type, "3gp"); 
		else if ( !strcmp(mime,  "video/3gpp2"  )) strcpy(type, "3g2"); 
		else if ( !strcmp(mime,  "audio/3gpp2" )) strcpy(type, "3g2"); 
		else if ( !strcmp(mime,  "application/x-7z-compressed"  )) strcpy(type, "7z"); 
		else if ( !strcmp(mime,  "application/tar+gzip" )) strcpy(type,"tar+gzip"); 
		else extract_extension(type, filename); 
}


void check_magic(char* filename, char *mpath){	
	strcpy(mpath, "/home/buse/Desktop/deneme/");
	strcpy(&(mpath[26]), filename);
    if (access(mpath, 4) == -1) {
        extract_extension(mpath, filename); //magic_file does not work
        return;
    }	

    struct magic_set *magic = magic_open(MAGIC_MIME);
	if (magic == NULL) {
        printf("Magic library initialization is fail!..\n");
        return;
    }

    if(magic_load(magic, NULL)){ //load default magic db
    	magic_close(magic); 
    	exit(1);
    }

	strcpy(mpath, magic_file(magic, mpath));
	magic_close(magic);
	
	for(int i=0; true; i++){ //extract extention --> "sth./extension; sth."  
		for(i=0; true; i++){ 
			if(mpath[i] == ';'){ 
				mpath[i] = '\0'; //only mime type 
				get_type(mpath, mpath, filename);
				return;
			}				 
		}			 
	}
}



