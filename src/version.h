#ifndef VERSION_H_  
#define VERSION_H_  
//major.minor.svn(git)version.datetime_base   
#define MAJOR_VERSION   ("0")  
#define MINOR_VERSION	("1")  
#if _WIN32  
#define SG_BRANCH_NAME	("*master")  
#define SG_COMMIT_HASH	("1bd400a85760d4406150d0f6683991062cdbba88")  
#define SG_COMMIT_DATE	("2020-08-0119:55:17+0800")  
#define SG_VERSION		(SG_COMMIT_HASH)  
#else  
#define SG_VERSION		(GIT_VERSION)  
#endif  
#endif  
